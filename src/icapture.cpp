#include "icapture.hpp"

ICapture::ICapture()
{
    frame = new LuxFrame();
}


void ICapture::readFrame()
{
}


ICapture::~ICapture()
{
    delete frame;
}


LuxFrame *ICapture::getFrame()
{
    return frame;
}


int ICapture::getFrameWidth()
{
    return frame_width;
}


int ICapture::getFrameHeight()
{
    return frame_height;
}


bool ICapture::isConnected()
{

}
