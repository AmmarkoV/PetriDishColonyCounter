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
};

int loadAnImage(const char * filename);

int processLoadedImage(struct processingInformation * settings);

const char * accessRGBPixels(unsigned int * width,unsigned int * height);
