#ifndef FILECAPTURE_H
#define FILECAPTURE_H
#include "icapture.hpp"
#include <stdexcept>
class FileCapture : public ICapture
{
public:
    FileCapture(std::string path);
    ~FileCapture();
    virtual void readFrame();
    int getFrameCount () { return frame_count; };

private:
    int iter;
    std::string cur_path;
    FILE* file;
    int frame_count;
};

#endif // FILECAPTURE_H
