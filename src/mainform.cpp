#include "mainform.hpp"
#include <QDebug>
#include "icapture.hpp"
#include "kinectcapture.hpp"
#include "filecapture.hpp"
#include <QtTest/QTest>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <cstdlib>


MainForm::MainForm(QWidget *parent) :
    QWidget(parent)
{
    this->setGeometry (10, 10, 400, 100);

    this->setLayout(layout);

    browseSaveButton->setText("Browse to Save");
    browseOpenButton->setText("Browse to Open");
    recordAnyButton->setText("Record");
    recordOneButton->setText("Record one frame");
    recordNButton->setText("Record N frames");
    //previewButton->setText("Preview");
    playButton->setText("Play");
    cleanButton->setText("New");

    layout->addWidget(filenameSaveEdit, 0, 0);
    layout->addWidget(browseSaveButton, 0, 1);
    layout->addWidget(cleanButton, 1, 0);
    layout->addWidget(recordAnyButton, 2, 0);
    layout->addWidget(recordOneButton, 3, 0);
    layout->addWidget(recordNButton, 4, 0);
    layout->addWidget(frames_spinbox, 4, 1);
    layout->addWidget(filenameOpenEdit, 5, 0);
    layout->addWidget(browseOpenButton, 5, 1);

    layout->addWidget(playButton, 6, 0);
    layout->addWidget(video_frame, 0, 2, 6, 1);
    //layout->addWidget(previewButton, 6, 2);

    recordAnyButton->setMinimumHeight(40);
    recordOneButton->setMinimumHeight(40);
    recordNButton->setMinimumHeight(40);

    video_frame->setPixmap(QPixmap(640,480));



    connect(browseSaveButton, SIGNAL(clicked()), this, SLOT(browseSavePressed()));
    connect(browseOpenButton, SIGNAL(clicked()), this, SLOT(browseOpenPressed()));

    connect(recordAnyButton, SIGNAL(clicked()), this, SLOT(recordAnyPressed()));
    connect(recordOneButton, SIGNAL(clicked()), this, SLOT(recordOnePressed()));
    connect(recordNButton, SIGNAL(clicked()), this, SLOT(recordNPressed()));

    connect(playButton, SIGNAL(clicked()), this, SLOT(playPressed()));
    //connect(previewButton, SIGNAL(clicked()), this, SLOT(previewPressed()));
    connect(cleanButton, SIGNAL(clicked()), this, SLOT(cleanPressed()));

    //recordAnyButton->setEnabled(false);
    //recordNButton->setEnabled(false);
    //recordOneButton->setEnabled(false);
    playButton->setEnabled(false);

    capture = new KinectCapture();
    if(capture->isConnected())
    {
        frame = capture->getFrame();
    }


}


void MainForm::browseSavePressed()
{
    filenameSaveEdit->setText(QFileDialog::getSaveFileName (this,
              tr ("Open Video"), "~", tr ("Kinect Video(*.kinvideo)")));
    if(filenameSaveEdit->text().isEmpty())
        return;


    QString format = ".kinvideo";
    if(filenameSaveEdit->text().indexOf(format) == -1)
        filenameSaveEdit->setText(filenameSaveEdit->text()+format);


}




void MainForm::playPressed()
{
    //is_preview = false;
    std::string filename = filenameOpenEdit->text().toStdString();
    if(filename.empty())
        filename = filenameSaveEdit->text().toStdString();
    ICapture *capture1 = new FileCapture(filename);
    LuxFrame *frame1 = capture1->getFrame();
    recordAnyButton->setEnabled(false);
    recordOneButton->setEnabled(false);
    recordNButton->setEnabled(false);
    while(1)
    {
        try{
        capture1->readFrame();
        }catch(std::out_of_range){
            qDebug()<<"End of frames";
            break;
        }
        myimshow(frame1->image);
        //cv::imshow("ee", frame1->image);
        //cvWaitKey(1);

    }

    recordAnyButton->setEnabled(true);
    recordOneButton->setEnabled(true);
    recordNButton->setEnabled(true);
    delete capture1;
    //is_preview = true;
    //getFramesLoop();

}


void MainForm::record(char type)
{
    browseSaveButton->setEnabled(false);
    browseOpenButton->setEnabled(false);
    playButton->setEnabled(false);

    if(file == NULL)
    {
        if(filenameSaveEdit->text().isEmpty())
        {
            char *filename = findFileName();
            filenameSaveEdit->setText(filename);
            delete filename;
        }

        file = fopen(filenameSaveEdit->text().toLatin1().data(), "wb");
        int width = capture->getFrameWidth();
        fwrite(&width, 1, sizeof(int), file);
    }
    switch(type){
    case 'o':
        iter_frames = 1;
        break;
    case 'n':
        iter_frames = frames_spinbox->value();
        break;
    case 'a':
        iter_frames = -1;
        break;
    }
    is_record = true;

    recordAnyButton->setEnabled(true);
    recordOneButton->setEnabled(true);
    recordNButton->setEnabled(true);
    browseSaveButton->setEnabled(true);
    browseOpenButton->setEnabled(true);
    playButton->setEnabled(true);

}

void MainForm::getFramesLoop()
{
    if(!capture->isConnected())
        return;
    while(is_preview){
        capture->readFrame();
        myimshow(frame->image);
        //cv::imshow("ee", frame->image);
        if(is_record)
        {
            fwrite(frame->image.data, sizeof(unsigned char), 640*480*3, file);
            fwrite(frame->depth_map.data, sizeof(float), 640*480*3, file);

            if(iter_frames != -1)
            {
                iter_frames--;
                if(iter_frames == 0)
                    is_record = false;
            }

        }
    }

}

void MainForm::recordAnyPressed()
{
    if(recordAnyButton->text().at(0) == 'R')
    {
        recordAnyButton->setText("STOP");
        is_record = true;
        recordOneButton->setEnabled(false);
        recordNButton->setEnabled(false);
        browseSaveButton->setEnabled(false);
        browseOpenButton->setEnabled(false);
        playButton->setEnabled(false);
        record('a');
    }
    else
    {
        recordAnyButton->setText("Record");
        fclose(file);
        file = NULL;
        is_record = false;
    }

}


void MainForm::recordNPressed()
{
    recordOneButton->setEnabled(false);
    recordAnyButton->setEnabled(false);
    recordNButton->setEnabled(false);
    record('n');
}


void MainForm::recordOnePressed()
{
    record('o');
}


void MainForm::browseOpenPressed()
{
    filenameOpenEdit->setText(QFileDialog::getOpenFileName (this,
              tr ("Open Video"), "~", tr ("Kinect Video(*.kinvideo)")));
    playButton->setEnabled(true);
}

void MainForm::myimshow(cv::Mat & frame)
{

    video_frame->setPixmap(QPixmap::fromImage(Mat2QImage(frame)));
    video_frame->update();
    video_frame->show();
    qApp->processEvents();
}

QImage MainForm::Mat2QImage(cv::Mat const& src)
{
  QImage dest(src.cols, src.rows, QImage::Format_ARGB32);

  const float scale = 255.0;

  if (src.depth() == CV_8U) {
    if (src.channels() == 1) {
      for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
          int level = src.at<quint8>(i, j);
          dest.setPixel(j, i, qRgb(level, level, level));
        }
      }
    } else if (src.channels() == 3) {
      for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
          cv::Vec3b bgr = src.at<cv::Vec3b>(i, j);
          dest.setPixel(j, i, qRgb(bgr[2], bgr[1], bgr[0]));
        }
      }
    }
  } else if (src.depth() == CV_32F) {
    if (src.channels() == 1) {
      for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
          int level = scale * src.at<float>(i, j);
          dest.setPixel(j, i, qRgb(level, level, level));
        }
      }
    } else if (src.channels() == 3) {
      for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
          cv::Vec3f bgr = scale * src.at<cv::Vec3f>(i, j);
          dest.setPixel(j, i, qRgb(bgr[2], bgr[1], bgr[0]));
        }
      }
    }
  }

  return dest;
}


/*void MainForm::previewPressed()
{
    is_preview = !is_preview;
    if(is_preview)
    {
        previewButton->setText("STOP");
        preview();
    }
    else
        previewButton->setText("Preview");
}
*/


void MainForm::preview()
{
    while(is_preview)
    {
        capture->readFrame();
        myimshow(frame->image);
        //cv::imshow("e", frame->image);
        //cvWaitKey(1);
    }
}



char* MainForm::findFileName()
{
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    int iter = 1;
    char *buff = new char[255];

    while(1)
    {
         sprintf(buff, "%s/test%d.kinvideo", homedir, iter);
        if(access(buff, 0) == -1)
            break;
        iter++;
    }

    return buff;
}


void MainForm::closeEvent(QCloseEvent *event)
{
    is_preview = false;
    event->accept();
}



void MainForm::cleanPressed()
{
    if(file != NULL)
        fclose(file);
    filenameSaveEdit->clear();
}
