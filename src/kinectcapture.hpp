#ifndef LUXKINECTDEV_HPP
#define LUXKINECTDEV_HPP
#include "icapture.hpp"
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include "QDebug"
class KinectCapture : public ICapture
{
public:
    KinectCapture(Resolutions res = LUX_RES_640x480);
    void readFrame();
    virtual ~KinectCapture();
    bool isConnected();
private:
    cv::VideoCapture *capture;
};

#endif // KINECTCAPTURE_HPP
