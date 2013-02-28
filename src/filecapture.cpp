#include "filecapture.hpp"
#include <QtTest/QTest>
#include <QDebug>
FileCapture::FileCapture(std::string path)
{
    iter = 1;
    cur_path = path;
    file = fopen(path.data(), "rb");
    fread(&frame_width, sizeof(int), 1, file);
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
    frame_count = (ftell(file) - sizeof(int))/(frame_width*frame_height*15);
    qDebug()<<frame_count;
    fseek(file, sizeof(int), SEEK_SET);



}


void FileCapture::readFrame()
{
    /*
    char cname[20];
    sprintf(cname,"%s/pc%d", cur_path ,iter);
    XnPoint3D * arr_point=new XnPoint3D[640*480];
    std::ifstream inp_file(cname);

    inp_file.read((char*)arr_point,640*480*sizeof(XnPoint3D));

    sprintf(cname,"pointCloud/pc%d.jpg",iter);
    IplImage *image=cvLoadImage(cname,1);
    if (!image) throw HaveNotFrameException();
    */
    if(iter > frame_count)
    {
        throw std::out_of_range (std::string("no more frames"));
        return;
    }
    if(frame->image.empty())
    {
        frame->image.create(cv::Size(frame_width, frame_height),CV_8UC3);
        frame->depth_map.create(cv::Size(frame_width,frame_height),CV_32FC3);
    }

    fread(frame->image.data, sizeof(unsigned char), frame_width*frame_height*3, file);
    fread(frame->depth_map.data, sizeof(float), frame_width*frame_height*3, file);
    QTest::qSleep(1000/15);

    iter++;
}





FileCapture::~FileCapture()
{
    fclose(file);
}
