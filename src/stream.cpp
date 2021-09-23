/** @file stream.cpp
 *  @brief This is a simple test file to make sure your camera or video files can be opened using OpenCV
 *  @author Ammar Qammaz (AmmarkoV)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace std;

#define NORMAL   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */


#include "AmmClient/AmmClient.h"
#include <unistd.h>

// '\xff\xd8'
const char JPEG_START_BYTE_A=255;
const char JPEG_START_BYTE_B=216;

// '\xff\xd9'
const char JPEG_EOF_BYTE_A=255;
const char JPEG_EOF_BYTE_B=217;


#define STREAMING_BUFFER_SIZE 524288
struct streamingBuffer
{
   char scanForJPEG;
   char * lastJPEGImageStart;

   unsigned int bufferSize;
   char globalBuffer[STREAMING_BUFFER_SIZE];
};


char * foundJPEGStart(char * buffer,unsigned int bufferSize)
{
  fprintf(stderr,"foundJPEGStart %u..\n",bufferSize);
  char * ptr    = buffer;
  char * ptrEnd = buffer + bufferSize;

  while (ptr<=ptrEnd-1)
  {
    switch (*ptr)
    {
       case JPEG_START_BYTE_A:
             switch (*(ptr+1))
              {
               case JPEG_START_BYTE_B:
                  return ptr;
               break;
              };
       break;
    };
    ptr++;
  }

  return 0;
}

char * foundJPEGEnd(char * buffer,unsigned int bufferSize)
{
  fprintf(stderr,"foundJPEGEnd %u..\n",bufferSize);
  char * ptr    = buffer;
  char * ptrEnd = buffer + bufferSize;

  while (ptr<=ptrEnd-1)
  {
    switch (*ptr)
    {
       case (char) JPEG_EOF_BYTE_A:
             switch (*(ptr+1))
              {
               case (char) JPEG_EOF_BYTE_B:
                  return ptr;
               break;
              };
       break;
    };
    ptr++;
  }

  return 0;
}


int flushJPEGFileToDisk(const char * filename, char * buffer, unsigned int bufferSize)
{
  FILE * fp = fopen(filename,"wb");

  if (fp!=0)
  {
      fwrite(buffer,1,bufferSize,fp);
      fclose(fp);
      return 1;
  }
 return 0;
}

int stream(int argc, char *argv[])
{
 const char IP[]={"192.168.1.33"};
 const int port = 80;
 const int timeoutInSeconds = 100;

 fprintf(stderr,"AmmClient JPEG Multipart streaming connecting to %s:%u \n",IP,port);
 struct AmmClient_Instance * inst = AmmClient_Initialize(IP,port,timeoutInSeconds);
 fprintf(stderr,"Initialized..\n");

 unsigned int receivedFiles=0;

 if (inst)
 {
  const unsigned int SEND_BUFFER_SIZE = 1024;
  char buf[SEND_BUFFER_SIZE+1]={0};
  unsigned int recvdSize=0;

  struct streamingBuffer sBuf={0};

  unsigned long startTime,endTime;

  char sentRequest=0;
  unsigned int i=0;
  while (1)
  {
   startTime = AmmClient_GetTickCountMicroseconds();

   snprintf(buf,SEND_BUFFER_SIZE,"GET / HTTP/1.1\nConnection: keep-alive\n\n");

   if (!sentRequest)
   {
      fprintf(stderr,"Send #%u..\n",i);
      if (AmmClient_Send(inst,buf,strlen(buf),1))
      {
        sentRequest=1;
      }
   }

   if (sentRequest)
   {
    usleep(10000);
    fprintf(stderr,"Recv #%u - %u bytes..\n",i,sBuf.bufferSize);

    char * startOfCurrentChunk = sBuf.globalBuffer+sBuf.bufferSize;
    recvdSize=STREAMING_BUFFER_SIZE-sBuf.bufferSize;
    if (!AmmClient_Recv(inst,startOfCurrentChunk,&recvdSize) )
      {
       fprintf(stderr,"Failed to recv.. \n");
      } else
      {
        //Go further down our buffer..
        sBuf.bufferSize += recvdSize;
        //----------------------------

        if (!sBuf.scanForJPEG)
        {
          sBuf.lastJPEGImageStart = foundJPEGStart(startOfCurrentChunk,recvdSize);
          if (sBuf.lastJPEGImageStart)
           {
            fprintf(stderr,GREEN "Found Start..\n" NORMAL);
            sBuf.scanForJPEG = 1;
           }
        } else
        {
          char * jpegEnd = foundJPEGEnd(startOfCurrentChunk,recvdSize);
          if (jpegEnd)
           {
            jpegEnd+=2;
            fprintf(stderr,GREEN "Found End..\n" NORMAL);
            fprintf(stderr,GREEN "JPEG File started @ %p , sized %lu bytes\n" NORMAL,sBuf.lastJPEGImageStart,jpegEnd-sBuf.lastJPEGImageStart);

            char filename[512]={0};
            snprintf(filename,512,"image_%u.jpg",receivedFiles);
            flushJPEGFileToDisk(filename,sBuf.lastJPEGImageStart,jpegEnd-sBuf.lastJPEGImageStart);
            ++receivedFiles;

            sBuf.scanForJPEG = 0;
            sentRequest=0; //Send again..

            unsigned long remainingSize = (startOfCurrentChunk+recvdSize)-jpegEnd;
            fprintf(stderr,"There are still %lu bytes that we need to copy to the start of the buffer..\n",remainingSize);
            memcpy(sBuf.globalBuffer,jpegEnd+1,remainingSize-1);
            sBuf.bufferSize = remainingSize;
           }
        }

      }


   endTime = AmmClient_GetTickCountMicroseconds();

   fprintf(stderr,"Took %lu microseconds \n",endTime-startTime);
   } else
   {
     fprintf(stderr,"Failed to Send.. \n");
   }
   usleep(100);

   ++i;

  }

  AmmClient_Close(inst);
 }

  return 0;
}






int main(int argc, char *argv[])
{
       stream(argc,argv);
       return 0;
}
