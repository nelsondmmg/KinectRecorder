#include <kinectcapture.hpp>

KinectCapture::KinectCapture(Resolutions res)
{
   capture = new cv::VideoCapture(CV_CAP_OPENNI);
    if(!capture->isOpened())
        std::cerr << "Kinect is not opened" << std::endl;
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

}

void KinectCapture::readFrame()
{
    capture->grab();
    capture->retrieve( frame->depth_map, CV_CAP_OPENNI_POINT_CLOUD_MAP );
    capture->retrieve( frame->image, CV_CAP_OPENNI_BGR_IMAGE );
}
