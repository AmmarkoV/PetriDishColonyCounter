#include "opencv2/opencv.hpp"
/** @file webcam.cpp
 *  @brief This is a simple test file to make sure your camera or video files can be opened using OpenCV
 *  @author Ammar Qammaz (AmmarkoV)
 */
#include <stdio.h>
#include <vector>
#include "cv.h"

using namespace std;
using namespace cv;

cv::Mat rgb;
cv::Mat intermediate;
cv::Mat pretty;


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

unsigned char * accessRGBPixels(unsigned int * width,unsigned int * height)
{
  *width = rgb.size().width;
  *height = rgb.size().height;
  return rgb.data;
}


unsigned char * accessPrettyPixels(unsigned int * width,unsigned int * height)
{
  *width = pretty.size().width;
  *height = pretty.size().height;
  return pretty.data;
}

unsigned char * accessInternalPixels(unsigned int * width,unsigned int * height)
{
  *width = intermediate.size().width;
  *height = intermediate.size().height;
  return intermediate.data;
}

int loadAnImage(const char * filename)
{
    rgb = imread(filename,cv::IMREAD_COLOR);

    if(rgb.empty()){
        printf(" Error opening image\n");
        printf(" Program Arguments: [image_name] \n");
        return 0;
    }

    //imshow("Input",rgb);
    //cv::moveWindow("Input",0,0);


    blur(rgb,rgb,Size(2,2));
    cv::cvtColor(rgb,rgb,CV_BGR2RGB);

    return 1;
}


int processLoadedImage(struct processingInformation * settings)
{
    int64 t0 = cv::getTickCount();
    pretty = rgb.clone();
    intermediate = rgb.clone();
    boostContrast(intermediate,1.99,-210);
    eliminateDark(intermediate,25);

    int dilation_size=1;
    int dilation_elem = 0;
    int dilation_type = 0;
    if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; } else
    if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; } else
    if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

     cv::Mat kernel = getStructuringElement(
                                             dilation_type,
                                             Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                             Point( dilation_size, dilation_size )
                                            );

    kernel.at<float>(0,0)= 0;     kernel.at<float>(0,1)= 0;     kernel.at<float>(0,2)= 0;
    kernel.at<float>(1,0)= 0;     kernel.at<float>(1,1)= 0;     kernel.at<float>(1,2)= 0;
    kernel.at<float>(2,0)= 0;     kernel.at<float>(2,1)= 0;     kernel.at<float>(2,2)= 0;


    cv::dilate(intermediate,intermediate, kernel, Point(-1, -1), 2, 1, 1);
    //cv::erode(intermediate,intermediate, kernel, Point(-1, -1), 2, 1, 1);
    std::vector<KeyPoint> keypoints;



    // Setup SimpleBlobDetector parameters.
    SimpleBlobDetector::Params params;

    params.blobColor=255; //Light Blobs

    // Change thresholds
    params.minThreshold = 1;
    params.maxThreshold = 50;

    // Filter by Area.
    params.filterByArea = true;
    params.minArea = 1;
    params.maxArea = 50;

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
    detector->detect(intermediate,keypoints);


    fprintf(stderr,"Keypoints detected = %lu \n",keypoints.size());

    Mat gray;
    cvtColor(intermediate, gray, COLOR_BGR2GRAY);
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
    drawKeypoints(intermediate,keypoints,intermediate, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS ); //DRAW_RICH_KEYPOINTS
    drawKeypoints(pretty,keypoints,pretty, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS ); //DRAW_RICH_KEYPOINTS

    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        // circle center
        //circle( intermediate, center, 1, Scalar(0,100,100), 3, LINE_AA);
        cv::drawMarker (intermediate,center,Scalar(0,0,255),MARKER_CROSS,20,1,8);
        cv::drawMarker (pretty,center,Scalar(0,0,255),MARKER_CROSS,20,1,8);
        // circle outline
        int radius = c[2];
        cv::circle( intermediate, center, radius, Scalar(255,0,255), 3, LINE_AA);
        cv::circle( pretty, center, radius, Scalar(255,0,255), 3, LINE_AA);
    }


    char str[512]= {0};
    snprintf(str,512,"Counted blobs %lu",keypoints.size());
    cv::Point jointPoint(10,50);
    //cv::putText(intermediate, str, jointPoint, cv::FONT_HERSHEY_DUPLEX, 1.5, Scalar(0,0,255) , 0.5, 8 );


    if (settings->viewIntermediate)
    {
     imshow("Output",intermediate);
     cv::moveWindow("Output",1000,0);
    }

    cv::cvtColor(intermediate,intermediate,CV_BGR2RGB);
    cv::cvtColor(pretty,pretty,CV_BGR2RGB);
    //waitKey(0);


    int64 t1 = cv::getTickCount();
    settings->elapsedTimeInSeconds = (float) (t1-t0)/cv::getTickFrequency();
    return keypoints.size();
}
