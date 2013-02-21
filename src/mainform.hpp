#ifndef MAINFORM_HPP
#define MAINFORM_HPP

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QFileDialog>
#include <QSpinBox>
#include "kinectcapture.hpp"
#include "luxframe.hpp"
class MainForm : public QWidget
{
    Q_OBJECT
public:
    explicit MainForm(QWidget *parent = 0);


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

    FILE* file;
    bool is_record = false;
    int col_frames;
    ICapture *capture;
    LuxFrame *frame;

    void record(char type);
signals:
    
public slots:
    void browseSavePressed();
    void browseOpenPressed();
    void recordAnyPressed();
    void recordOnePressed();
    void recordNPressed();
    void playPressed();
};

#endif // MAINFORM_HPP
