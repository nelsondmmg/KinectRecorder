#include "mainform.hpp"
#include <QDebug>
#include "icapture.hpp"
#include "kinectcapture.hpp"
#include "filecapture.hpp"
#include <QtTest/QTest>

MainForm::MainForm(QWidget *parent) :
    QWidget(parent)
{
    this->setGeometry (10, 10, 400, 100);

    this->setLayout(layout);

    //filenameEdit->setGeometry(10,10, 100,20);
    browseSaveButton->setText("Browse to Save");
    browseOpenButton->setText("Browse to Open");
    recordAnyButton->setText("Record");
    recordOneButton->setText("Record one frame");
    recordNButton->setText("Record N frames");

    playButton->setText("Play");

    //filenameEdit->setText("test.kinvideo");
    layout->addWidget(filenameSaveEdit, 0, 0);
    layout->addWidget(browseSaveButton, 0, 1);
    layout->addWidget(recordAnyButton, 1, 0);
    layout->addWidget(recordOneButton, 2, 0);
    layout->addWidget(recordNButton, 3, 0);
    layout->addWidget(frames_spinbox, 3, 1);
    layout->addWidget(filenameOpenEdit, 4, 0);
    layout->addWidget(browseOpenButton, 4, 1);

    layout->addWidget(playButton, 5, 0);

    recordAnyButton->setMinimumHeight(40);
    recordOneButton->setMinimumHeight(40);
    recordNButton->setMinimumHeight(40);

    //layout->setRowStretch(0, 1);



    connect(browseSaveButton, SIGNAL(clicked()), this, SLOT(browseSavePressed()));
    connect(browseOpenButton, SIGNAL(clicked()), this, SLOT(browseOpenPressed()));

    connect(recordAnyButton, SIGNAL(clicked()), this, SLOT(recordAnyPressed()));
    connect(recordOneButton, SIGNAL(clicked()), this, SLOT(recordOnePressed()));
    connect(recordNButton, SIGNAL(clicked()), this, SLOT(recordNPressed()));

    connect(playButton, SIGNAL(clicked()), this, SLOT(playPressed()));

    capture = new KinectCapture();
    frame = capture->getFrame();

    this->show();
}


void MainForm::browseSavePressed()
{
    filenameSaveEdit->setText(QFileDialog::getSaveFileName (this,
              tr ("Open Video"), "~", tr ("Kinect Video(*.kinvideo)")));
    file = fopen(filenameSaveEdit->text().toLatin1().data(), "wb");
    int width = capture->getFrameWidth();
    fwrite(&width, 1, sizeof(int), file);

}




void MainForm::playPressed()
{

    ICapture *capture1 = new FileCapture(filenameOpenEdit->text().toStdString());
    LuxFrame *frame1 = capture1->getFrame();
    while(1)
    {
        try{
        capture1->readFrame();
        }catch(FileCapture::HaveNotFrameException e){
            qDebug()<<"End of frames";
            break;
        }

        cv::imshow("ee", frame1->image);
        cvWaitKey(1);

    }

    delete capture1;

}


void MainForm::record(char type)
{
    int iter;
    switch(type){
    case 'o':
        iter = 1;
        break;
    case 'n':
        iter = frames_spinbox->value();
        break;
    }

    while(1){
        capture->readFrame();
        cv::imshow("ee", frame->image);
        fwrite(frame->image.data, sizeof(unsigned char), 640*480*3, file);
        fwrite(frame->depth_map.data, sizeof(float), 640*480*3, file);

        //if(frame->img.empty())
       //     qDebug()<<"fuck";

        cvWaitKey(1);
        if(type == 'a')
        {
            if(!is_record)
                break;
        }
        else
        {
            iter--;
            if(iter == 0)
                break;
        }
    }



}


void MainForm::recordAnyPressed()
{
    if(recordAnyButton->text().at(0) == 'R')
    {
        recordAnyButton->setText("STOP");
        is_record = true;
        record('a');
    }
    else
    {
        recordAnyButton->setText("Record");
        is_record = false;
    }

}


void MainForm::recordNPressed()
{
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

}
