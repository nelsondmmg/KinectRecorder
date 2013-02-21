#ifndef ICAPTURE_HPP
#define ICAPTURE_HPP
#include "luxframe.hpp"
#include <cstdio>
enum Resolutions{LUX_RES_320x240, LUX_RES_640x480, LUX_RES_1280x1024};
class ICapture
{
public:
    ICapture();
    virtual ~ICapture();
    virtual void readFrame();
    LuxFrame* getFrame();
    int getFrameWidth();
    int getFrameHeight();
protected:
    int frame_width, frame_height;
    LuxFrame *frame;


};

#endif // ICAPTURE_HPP
