#include "mainform.hpp"
#include <QDebug>
#include "icapture.hpp"
#include "kinectcapture.hpp"
#include "filecapture.hpp"
#include <QThread>
#include <unistd.h>

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
    playButton->setText("Play");
    cleanButton->setText("New");
    frames_spinbox->setMaximum(200);

    layout->addWidget(frame_number_label, 0, 0);
    layout->addWidget(filenameSaveEdit, 1, 0);
    layout->addWidget(browseSaveButton, 1, 1);
    layout->addWidget(cleanButton, 2, 0);
    layout->addWidget(recordAnyButton, 3, 0);
    layout->addWidget(recordOneButton, 4, 0);
    layout->addWidget(recordNButton, 5, 0);
    layout->addWidget(frames_spinbox, 5, 1);
    layout->addWidget(filenameOpenEdit, 6, 0);
    layout->addWidget(browseOpenButton, 6, 1);

    layout->addWidget(playButton, 7, 0);
    layout->addWidget(video_frame, 1, 2, 7, 1);

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
    connect(cleanButton, SIGNAL(clicked()), this, SLOT(cleanPressed()));

    qRegisterMetaType<cv::Mat>("cv::Mat&");
    connect(this, SIGNAL(showImage(cv::Mat&)), this, SLOT(myimshow(cv::Mat&)));


    capture = new KinectCapture();
    if(capture->isConnected())
    {
        frame = capture->getFrame();
    }
    else
    {
        recordAnyButton->setEnabled(false);
        recordOneButton->setEnabled(false);
        recordNButton->setEnabled(false);
        browseSaveButton->setEnabled(false);
        filenameSaveEdit->setEnabled(false);
        cleanButton->setEnabled(false);
        frames_spinbox->setEnabled(false);
    }
    thread_read_frames = QtConcurrent::run(this, &MainForm::getFramesLoop);

}


void MainForm::browseSavePressed()
{
    filenameSaveEdit->setText(QFileDialog::getSaveFileName (this,
              tr ("Open Video"), QDir::homePath(), tr ("Kinect Video(*.kinvideo)")));
    if(filenameSaveEdit->text().isEmpty())
        return;


    QString format = ".kinvideo";
    if(filenameSaveEdit->text().indexOf(format) == -1)
        filenameSaveEdit->setText(filenameSaveEdit->text()+format);


}




void MainForm::playPressed()
{
    is_preview = false;
    while(!thread_read_frames.isFinished());
    qDebug()<<"finished";
    std::string filename = filenameOpenEdit->text().toStdString();
    if(filename.empty())
        filename = filenameSaveEdit->text().toStdString();
    if(filename.empty())
        return;
    playButton->setEnabled(false);
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
        qApp->processEvents();
        //cv::imshow("ee", frame1->image);
        //cvWaitKey(1);

    }

    recordAnyButton->setEnabled(true);
    recordOneButton->setEnabled(true);
    recordNButton->setEnabled(true);
    delete capture1;
    playButton->setEnabled(true);

    is_preview = true;
    thread_read_frames = QtConcurrent::run(this, &MainForm::getFramesLoop);
}


void MainForm::record(char type)
{
    if(type != 'o')
    {
        browseSaveButton->setEnabled(false);
        browseOpenButton->setEnabled(false);
        playButton->setEnabled(false);
        cleanButton->setEnabled(false);
    }
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
        fwrite(&width, 1, sizeof(uint32_t), file);
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
    frames_count = 0;
    is_record = true;
    QFuture<void> thread = QtConcurrent::run(this, &MainForm::queueRecord);

    //recordAnyButton->setEnabled(true);
    //recordOneButton->setEnabled(true);
    //recordNButton->setEnabled(true);
    //browseSaveButton->setEnabled(true);
    //browseOpenButton->setEnabled(true);
    //playButton->setEnabled(true);

}

void MainForm::getFramesLoop()
{
    if(capture->isConnected())
    while(is_preview){
        mut.lock();
        capture->readFrame();
        //myimshow(frame->image);

        showImage(frame->image);
        if(is_record)
        {

            //fwrite(frame->image.data, sizeof(unsigned char), 640*480*3, file);
            //fwrite(frame->depth_map.data, sizeof(float), 640*480*3, file);
            frames_queue->push(*frame);
            frames_count++;
            frame_number_label->setText(QString("%1").arg(frames_count));
            //qDebug()<<"push";
            if(iter_frames != -1)
            {
                iter_frames--;

                if(iter_frames == 0)
                {
                    is_record = false;
                    cleanButton->setEnabled(true);
                    recordOneButton->setEnabled(true);
                    recordNButton->setEnabled(true);
                    recordAnyButton->setEnabled(true);
                    browseSaveButton->setEnabled(true);
                    browseOpenButton->setEnabled(true);
                    playButton->setEnabled(true);


                }
            }

        }
    }
    //mut.unlock();
    //thread_read_frames.cancel();
}

void MainForm::recordAnyPressed()
{
    if(recordAnyButton->text().at(0) == 'R')
    {
        recordAnyButton->setText("STOP");
        recordOneButton->setEnabled(false);
        recordNButton->setEnabled(false);
        if(file != NULL){
            fclose(file);
            file = NULL;
        }
        record('a');
    }
    else
    {
        is_record = false;
        recordAnyButton->setText("Record");

        cleanButton->setEnabled(true);
        recordOneButton->setEnabled(true);
        recordNButton->setEnabled(true);
        browseSaveButton->setEnabled(true);
        browseOpenButton->setEnabled(true);
        playButton->setEnabled(true);

    }

}


void MainForm::recordNPressed()
{
    if(frames_spinbox->value() <= 0)
        return;
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
              tr ("Open Video"), QDir::homePath(), tr ("Kinect Video(*.kinvideo)")));
    playButton->setEnabled(true);
}

void MainForm::myimshow(cv::Mat & frame)
{

    video_frame->setPixmap(QPixmap::fromImage(Mat2QImage(frame)));
    //video_frame->update();
    video_frame->show();
    mut.unlock();
    //qApp->processEvents();
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



char* MainForm::findFileName()
{
    int iter = 1;
    char *buff = new char[255];

    while(1)
    {
         sprintf(buff, "%s/test%d.kinvideo", QDir::homePath().toLocal8Bit().data(),
                 iter);
        if(access(buff, 0) == -1)
            break;
        iter++;
    }
    return buff;
}


void MainForm::closeEvent(QCloseEvent *event)
{
    is_preview = false;
    while(!thread_read_frames.isFinished());
    event->accept();

}



void MainForm::cleanPressed()
{
    if(file != NULL)
    {
        fclose(file);
        file = NULL;
    }
    filenameSaveEdit->clear();
}


void MainForm::recordFromQueue()
{

}


MainForm::~MainForm()
{
    delete browseOpenButton;
    delete browseSaveButton;
    delete recordNButton;
    delete recordOneButton;
    delete recordAnyButton;
    delete playButton;
    delete filenameSaveEdit;
    delete filenameOpenEdit;
    delete layout;
    delete frames_spinbox;
    delete video_frame;
    delete cleanButton;
    delete capture;
    delete frames_queue;
}

void MainForm::queueRecord()
{
    LuxFrame *temp_frame;
    //qDebug()<<"start of record";
    while(is_record || !frames_queue->empty())
    {
    while(!frames_queue->empty())
    {
        temp_frame = &frames_queue->front();
        //qDebug()<<"pop";
        fwrite(temp_frame->image.data, sizeof(unsigned char), 640*480*3, file);
        fwrite(temp_frame->depth_map.data, sizeof(float), 640*480*3, file);
        frames_queue->pop();
    }
    //qDebug()<<"frames end";
    usleep(100000);
    }
    qDebug()<<"end of record";
}

