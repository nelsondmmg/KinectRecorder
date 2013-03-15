#include "icapture.hpp"

ICapture::ICapture()
{
    frame = new LuxFrame();
}


ICapture::~ICapture()
{
    delete frame;
}


LuxFrame *ICapture::getFrame()
{
    return frame;
}


long ICapture::getFrameWidth()
{
    return frame_width;
}


long ICapture::getFrameHeight()
{
    return frame_height;
}


