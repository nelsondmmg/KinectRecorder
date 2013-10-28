#ifndef ICAPTURE_HPP
#define ICAPTURE_HPP

#include <luxframe.hpp>
#include <cstdint>

enum Resolutions { LUX_RES_320x240, LUX_RES_640x480, LUX_RES_1280x1024 };

class ICapture
{
public:
    virtual void readFrame() = 0;
    virtual bool isConnected() = 0;
    virtual bool setFrameNumber(int n) = 0;
    virtual int getFrameCount () = 0;

    ICapture() { frame = new LuxFrame(); }
    virtual ~ICapture() { delete frame; }

    inline LuxFrame * getFrame() { return frame; }
    inline long getFrameWidth() { return frame_width; }
    inline long getFrameHeight() { return frame_height; }
protected:
    uint32_t frame_width, frame_height;
    LuxFrame *frame;
};

#endif // ICAPTURE_HPP
