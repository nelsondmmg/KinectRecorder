#ifndef LUXFRAME_HPP
#define LUXFRAME_HPP
#include "opencv/cv.h"
class LuxFrame
{
public:
    LuxFrame();
    cv::Mat img;
    cv::Mat depth;
};

#endif // LUXFRAME_HPP
