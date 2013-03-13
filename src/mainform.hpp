#ifndef MAINFORM_HPP
#define MAINFORM_HPP

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QFileDialog>
#include <QSpinBox>
#include <QLabel>
#include "kinectcapture.hpp"
#include "luxframe.hpp"
#include <QCloseEvent>
class MainForm : public QWidget
{
    Q_OBJECT
public:
    explicit MainForm(QWidget *parent = 0);
    void getFramesLoop();
protected:
        void closeEvent (QCloseEvent * event);

private:
    QPushButton *browseOpenButton = new QPushButton(this);
    QPushButton *browseSaveButton = new QPushButton(this);
    QPushButton *recordNButton = new QPushButton(this);
    QPushButton *recordOneButton = new QPushButton(this);
    QPushButton *recordAnyButton = new QPushButton(this);
    QPushButton *playButton = new QPushButton(this);
    QLineEdit *filenameSaveEdit = new QLineEdit(this);
    QLineEdit *filenameOpenEdit = new QLineEdit(this);
    QGridLayout *layout = new QGridLayout(this);
    QSpinBox *frames_spinbox = new QSpinBox(this);
    QLabel *video_frame = new QLabel(this);
    //QPushButton *previewButton = new QPushButton(this);
    QPushButton *cleanButton = new QPushButton(this);
    FILE* file = NULL;
    bool is_record = false;
    bool is_preview = true;
    int col_frames;
    int iter_frames;
    ICapture *capture;
    LuxFrame *frame;

    void record(char type);

    void myimshow(cv::Mat&);
    void preview();
    char* findFileName();
    QImage Mat2QImage(cv::Mat const& src);

signals:
    
public slots:
    void browseSavePressed();
    void browseOpenPressed();
    void recordAnyPressed();
    void recordOnePressed();
    void recordNPressed();
    void playPressed();
    //void previewPressed();
    void cleanPressed();
};

#endif // MAINFORM_HPP
