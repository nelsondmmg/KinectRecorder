#ifndef LUXKINECTDEV_HPP
#define LUXKINECTDEV_HPP
#include "icapture.h"
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include "QDebug"
class KinectCapture : public ICapture
{
public:
    KinectCapture(Resolutions res = LUX_RES_640x480);
    virtual void readFrame();
    virtual ~KinectCapture();
private:
    cv::VideoCapture *capture;
};

#endif // KINECTCAPTURE_HPP
