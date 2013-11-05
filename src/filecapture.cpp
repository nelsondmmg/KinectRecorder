#include <filecapture.hpp>

FileCapture::FileCapture(std::string path)
    : file(NULL)
{
    iter = 1;
    cur_path = path;
    file = fopen(path.data(), "rb");
    fread(&frame_width, sizeof(uint32_t), 1, file);
    switch(frame_width)
    {
    case 640:
        frame_height = 480;
        break;
    case 320:
        frame_height = 240;
        break;
    case 1280:
        frame_height = 1024;
        break;
    }
    fseek(file, 0, SEEK_END);
    frame_count = (ftell(file))/(frame_width*frame_height*15);
    fseek(file, sizeof(uint32_t), SEEK_SET);
    frame->image.create(cv::Size(frame_width, frame_height),CV_8UC3);
    frame->depth_map.create(cv::Size(frame_width,frame_height),CV_32FC3);
}


void FileCapture::readFrame()
{
    if(iter > frame_count)
    {
        throw std::out_of_range (std::string("no more frames"));
        return;
    }

    fread(frame->image.data, sizeof(unsigned char), frame_width*frame_height*3, file);
    fread(frame->depth_map.data, sizeof(float), frame_width*frame_height*3, file);
    iter++;
}


bool FileCapture::setFrameNumber(int n)
{
    if(n < 1 || n > frame_count)
        return false;
    uint32_t location = frame_width * frame_height * 3 * (sizeof (float) + 1);
    location *= (n - 1);
    location += sizeof (uint32_t);
    fseek(file, location, SEEK_SET);
    iter = n;
    return true;
}

FileCapture::~FileCapture()
{
    fclose(file);
}

bool FileCapture::isConnected()
{
    if(file == NULL)
        return false;
    else
        return true;
}
