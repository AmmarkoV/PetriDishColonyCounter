#include "opencv2/opencv.hpp"
/** @file webcam.cpp
 *  @brief This is a simple test file to make sure your camera or video files can be opened using OpenCV
 *  @author Ammar Qammaz (AmmarkoV)
 */
#include <stdio.h>


using namespace cv;


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
            ) {
                rgb.at<Vec3b>(y,x)[0]=0;
                rgb.at<Vec3b>(y,x)[1]=0;
                rgb.at<Vec3b>(y,x)[2]=0;
            }


        }
    }
}



int main(int argc, char *argv[])
{
    cv::Mat rgb = imread("data/117429389_705588883323143_362726271946068301_n.jpg",CV_LOAD_IMAGE_COLOR);

    imshow("Input",rgb);
    cv::moveWindow("Input",0,0);

    boostContrast(rgb,1.99,-250);
    eliminateDark(rgb,25);

    std::vector<KeyPoint> keypoints;



    // Setup SimpleBlobDetector parameters.
    SimpleBlobDetector::Params params;

    // Change thresholds
    params.minThreshold = 10;
    params.maxThreshold = 250;

    // Filter by Area.
    params.filterByArea = true;
    params.minArea = 10;

    // Filter by Circularity
    params.filterByCircularity = true;
    params.minCircularity = 0.1;

    // Filter by Convexity
    params.filterByConvexity = true;
    params.minConvexity = 0.57;

    // Filter by Inertia
    params.filterByInertia = false;
    params.minInertiaRatio = 0.01;

    cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);  // create blob detector


    // Set up the detector with default parameters.
    // Detect blobs.
    detector->detect(rgb,keypoints);
 

    fprintf(stderr,"Keypoints detected = %lu \n",keypoints.size());

    // Draw detected blobs as red circles.
    // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
    Mat im_with_keypoints;
    drawKeypoints(rgb,keypoints,im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS ); //DRAW_RICH_KEYPOINTS

    char str[512]= {0};
    snprintf(str,512,"Counted blobs %lu",keypoints.size());
    cv::Point jointPoint(10,50);
    cv::putText(im_with_keypoints, str, jointPoint, cv::FONT_HERSHEY_DUPLEX, 1.5, cv::Scalar::all(255), 0.2, 8 );

    imshow("Output",im_with_keypoints);
    cv::moveWindow("Output",1000,0);

    waitKey(0);
}
