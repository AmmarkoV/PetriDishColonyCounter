#include "opencv2/opencv.hpp"
/** @file webcam.cpp
 *  @brief This is a simple test file to make sure your camera or video files can be opened using OpenCV
 *  @author Ammar Qammaz (AmmarkoV)
 */
#include <stdio.h>
#include <vector>

using namespace std;
using namespace cv;

#define NORMAL   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

int webcamLoop(int argc, char *argv[])
{
    unsigned int width = 640;
    unsigned int height = 480;


    const char * webcam = 0;
    for (int i=0; i<argc; i++) {
        if (strcmp(argv[i],"--from")==0) {
            if (argc>i+1) {
                webcam = argv[i+1];
            }
        }   else if (strcmp(argv[i],"--size")==0) {
            if (argc>i+2) {
                width = atoi(argv[i+1]);
                height = atoi(argv[i+2]);
            }
        }
    }


    VideoCapture cap(webcam); // open the default camera
    if (webcam==0) {
        std::cerr<<"Trying to open webcam\n";
        cap.set(CAP_PROP_FRAME_WIDTH,width);
        cap.set(CAP_PROP_FRAME_HEIGHT,height);
    } else {
        std::cerr<<"Trying to open "<<webcam<<"\n";
    }

    if (!cap.isOpened()) { // check if succeeded to connect to the camera
        std::cerr<<"Openning camera failed\n";
        return 1;
    }



    Mat edges;
    namedWindow("feed",1);
    for(;;) {
        Mat frame;
        cap >> frame;
        if ( (frame.size().width>0) && (frame.size().height>0) ) {
            imshow("feed", frame);
        } else {
            std::cerr<<"Broken frame.. \n";
        }
        waitKey(1);
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}



void boostContrast(cv::Mat &rgb,double contrast,signed int brightness)
{
    for( int y = 0; y < rgb.rows; y++ ) {
        for( int x = 0; x < rgb.cols; x++ ) {
            for( int c = 0; c < rgb.channels(); c++ ) {
                rgb.at<Vec3b>(y,x)[c] =
                    saturate_cast<uchar>( contrast*rgb.at<Vec3b>(y,x)[c] + brightness );
            }
        }
    }
}




void eliminateDark(cv::Mat &rgb,int threshold)
{
    for( int y = 0; y < rgb.rows; y++ ) {
        for( int x = 0; x < rgb.cols; x++ ) {

            if
            (
                ( rgb.at<Vec3b>(y,x)[0]<threshold ) ||
                ( rgb.at<Vec3b>(y,x)[1]<threshold ) ||
                ( rgb.at<Vec3b>(y,x)[2]<threshold )
            )
            {
                rgb.at<Vec3b>(y,x)[0]=0;
                rgb.at<Vec3b>(y,x)[1]=0;
                rgb.at<Vec3b>(y,x)[2]=0;
            } else
            {
                rgb.at<Vec3b>(y,x)[0]=255;
                rgb.at<Vec3b>(y,x)[1]=255;
                rgb.at<Vec3b>(y,x)[2]=255;
            }


        }
    }
}



#include "AmmClient/AmmClient.h"
#include <unistd.h>

// '\xff\xd8'
const char JPEG_START_BYTE_A=255;
const char JPEG_START_BYTE_B=216;

// '\xff\xd9'
const char JPEG_EOF_BYTE_A=255;
const char JPEG_EOF_BYTE_B=217;


#define STREAMING_BUFFER_SIZE 262144
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

            char filename[512];
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

    cv::Mat rgb = imread(argv[1],cv::IMREAD_COLOR);

    if(rgb.empty()){
        printf(" Error opening image\n");
        printf(" Program Arguments: [image_name] \n");
        return 1;
    }

    imshow("Input",rgb);
    cv::moveWindow("Input",0,0);

    boostContrast(rgb,1.99,-210);
    eliminateDark(rgb,25);

    int dilation_size=1;
    int dilation_elem = 0;
    int dilation_type = 0;
    if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; } else
    if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; } else
    if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

     cv::Mat kernel = getStructuringElement( dilation_type,
                       Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                       Point( dilation_size, dilation_size ) );

    kernel.at<float>(0,0)= 0;     kernel.at<float>(0,1)= 0;     kernel.at<float>(0,2)= 0;
    kernel.at<float>(1,0)= 0;     kernel.at<float>(1,1)= 0;     kernel.at<float>(1,2)= 0;
    kernel.at<float>(2,0)= 0;     kernel.at<float>(2,1)= 0;     kernel.at<float>(2,2)= 0;


    cv::dilate(rgb,rgb, kernel, Point(-1, -1), 2, 1, 1);
    //cv::erode(rgb,rgb, kernel, Point(-1, -1), 2, 1, 1);
    std::vector<KeyPoint> keypoints;



    // Setup SimpleBlobDetector parameters.
    SimpleBlobDetector::Params params;

    params.blobColor=255; //Light Blobs

    // Change thresholds
    params.minThreshold = 5;
    params.maxThreshold = 45;

    // Filter by Area.
    params.filterByArea = true;
    params.minArea = 5;
    params.maxArea = 44;

    // Filter by Circularity
    params.filterByCircularity = true;
    params.minCircularity = 0.6;

    // Filter by Convexity
    params.filterByConvexity = false;
    params.minConvexity = 0.87;

    // Filter by Inertia
    params.filterByInertia = true;
    params.minInertiaRatio = 0.01;

    cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);  // create blob detector


    // Set up the detector with default parameters.
    // Detect blobs.
    detector->detect(rgb,keypoints);


    fprintf(stderr,"Keypoints detected = %lu \n",keypoints.size());

    Mat gray;
    cvtColor(rgb, gray, COLOR_BGR2GRAY);
    vector<Vec3f> circles;
    HoughCircles(gray,
                 circles,
                 HOUGH_GRADIENT,
                 1, // 	Inverse ratio of the accumulator resolution to the image resolution. For example, if dp=1 , the accumulator has the same resolution as the input image
                 gray.rows/2,  // 	Minimum distance between the centers of the detected circles.
                 100, //the higher threshold of the two passed to the Canny edge detector (the lower one is twice smaller)
                 50,  // it is the accumulator threshold for the circle centers at the detection stage. The smaller it is, the more false circles may be detected

                 200,  // min_radius to detect smaller circles
                 300   // max_radius to detect larger circles
    );

   if (circles.size()>0)
   {

    std::vector<KeyPoint> filteredKeypoints;
    cv::Point petriDishCenter(0,0);
    petriDishCenter.x =  circles[0][0];
    petriDishCenter.y =  circles[0][1];
    float petriDishRadious = circles[0][2];
    fprintf(stderr,"Petri dish is at %u %u %0.2f",petriDishCenter.x,petriDishCenter.y,petriDishRadious);
    for (unsigned int blobID=0; blobID<keypoints.size(); blobID++)
    {
        cv::Point kp = keypoints[blobID].pt;
        double distanceFromPetriCenter = cv::norm(petriDishCenter - kp);
        if (distanceFromPetriCenter<petriDishRadious)
        {
            filteredKeypoints.push_back(keypoints[blobID]);
        } else
        {
            fprintf(stderr,"Blob @ %u,%u filtered out because of being out of petri dish\n",kp.x,kp.y);
        }
    }

    keypoints = filteredKeypoints;
   }

    // Draw detected blobs as red circles.
    // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
    Mat im_with_keypoints;
    drawKeypoints(rgb,keypoints,im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS ); //DRAW_RICH_KEYPOINTS

    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        // circle center
        //circle( im_with_keypoints, center, 1, Scalar(0,100,100), 3, LINE_AA);
        cv::drawMarker (im_with_keypoints,center,Scalar(0,0,255),MARKER_CROSS,20,1,8);
        // circle outline
        int radius = c[2];
        circle( im_with_keypoints, center, radius, Scalar(255,0,255), 3, LINE_AA);
    }


    char str[512]= {0};
    snprintf(str,512,"Counted blobs %lu",keypoints.size());
    cv::Point jointPoint(10,50);
    cv::putText(im_with_keypoints, str, jointPoint, cv::FONT_HERSHEY_DUPLEX, 1.5, Scalar(0,0,255) , 0.5, 8 );



    imshow("Output",im_with_keypoints);
    cv::moveWindow("Output",1000,0);

    waitKey(0);
}
