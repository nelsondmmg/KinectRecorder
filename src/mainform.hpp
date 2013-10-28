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
#include <queue>
#include <QMetaType>
#include <QFuture>
#include <QtConcurrent>
#include <mutex>

class MainForm : public QWidget
{
    Q_OBJECT
public:
    explicit MainForm(QWidget *parent = 0);
    virtual ~MainForm();
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
    QPushButton *cleanButton = new QPushButton(this);
    QLabel *frame_number_label = new QLabel(this);
    FILE* file = NULL;
    bool is_record = false;
    bool is_preview = true;
    int col_frames;
    int iter_frames;
    ICapture *capture;
    LuxFrame *frame;
    std::queue<LuxFrame> *frames_queue = new std::queue<LuxFrame>();
    const char *homedir;
    QFuture <void> thread_read_frames;
    std::mutex mut;
    unsigned int frames_count = 0;
    void record(char type);
    void recordFromQueue();

    void queueRecord();
    char* findFileName();
    QImage Mat2QImage(cv::Mat const& src);
signals:
    void showImage(cv::Mat &frame);

public slots:
    void browseSavePressed();
    void browseOpenPressed();
    void recordAnyPressed();
    void recordOnePressed();
    void recordNPressed();
    void playPressed();
    void myimshow(cv::Mat&);
    //void previewPressed();
    void cleanPressed();
};

#endif // MAINFORM_HPP
