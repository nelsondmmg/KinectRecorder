#include "kinectcapture.hpp"
#include <QDebug>
KinectCapture::KinectCapture(Resolutions res, bool cs): coordinate_system(cs)
{
   capture = new cv::VideoCapture(CV_CAP_OPENNI);
    if(!capture->isOpened())
        qDebug()<<"Kinect is not opened";
    int resolution;
    switch(res)
    {
    case LUX_RES_320x240:
        resolution = CV_CAP_OPENNI_QVGA_60HZ;
        frame_width = 320;
        frame_height = 240;
        break;
    case LUX_RES_640x480:
        resolution = CV_CAP_OPENNI_VGA_30HZ;
        frame_width = 640;
        frame_height = 480;
        break;
    case LUX_RES_1280x1024:
        resolution = CV_CAP_OPENNI_SXGA_30HZ;
        frame_width = 1280;
        frame_height = 1024;
        break;
    }

    capture->set(CV_CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE, resolution);
    capture->set(CV_CAP_PROP_OPENNI_REGISTRATION, 0);

}



void KinectCapture::readFrame()
{
    capture->grab();
    capture->retrieve( frame->depth_map, CV_CAP_OPENNI_POINT_CLOUD_MAP );
    capture->retrieve( frame->image, CV_CAP_OPENNI_BGR_IMAGE );
    if (coordinate_system)
    {
        for (int i = 0 ; i< frame_width ; i++)
            for (int j = 0 ; j < frame_height ; j++)
                 frame->depth_map.at<cv::Vec3f>(j, i)[2] = - frame->depth_map.at<cv::Vec3f>(j, i)[2];
    }

}



KinectCapture::~KinectCapture()
{


}


bool KinectCapture::isConnected()
{
    return capture->isOpened();
}
