#ifndef FILECAPTURE_H
#define FILECAPTURE_H
#include "icapture.h"
class FileCapture : public ICapture
{
public:
    FileCapture(std::string path);
    ~FileCapture();
    virtual void readFrame();
    class HaveNotFrameException{};
private:
    int iter;
    std::string cur_path;
    FILE* file;
    int frame_count;
};

#endif // FILECAPTURE_H
