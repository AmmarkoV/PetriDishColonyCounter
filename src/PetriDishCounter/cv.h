#pragma once


struct processingInformation
{
  unsigned int minimumThreshold;
  unsigned int maximumThreshold;

  unsigned char doFilteringByArea;
  unsigned int minimumArea;
  unsigned int maximumArea;

  unsigned char doFilteringByCircularity;
  float circularity;

  unsigned char doFilteringByInertia;
  float inertia;

  unsigned char doFilteringByConvexity;
  float convexity;

  unsigned char doFilteringByDilation;
  unsigned char doFilteringByErosion;

  float contrast;
  signed int brightness;
  signed int darkness;

  char viewIntermediate;

  float elapsedTimeInSeconds;
};

void defaultSettings(struct processingInformation * settings);

int loadANetworkImage(const char * URI);

int loadAnImage(const char * filename);

int processLoadedImage(struct processingInformation * settings);

unsigned char * accessRGBPixels(unsigned int * width,unsigned int * height);
unsigned char * accessPrettyPixels(unsigned int * width,unsigned int * height);
unsigned char * accessInternalPixels(unsigned int * width,unsigned int * height);
