#include "animationviewer.h"
#include "ui_animationviewer.h"
#include <QDir>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include <QDebug>

AnimationViewer::AnimationViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnimationViewer)
{
    ui->setupUi(this);

    this->setFixedSize(this->size());

    QStringList list = (QStringList()<<"5 frame/sec"<<"10 fram/sec"<<"15 frame/sec"
                        <<"20 frame/sec"<<"25 frame/sec"<<"Maximum");
    this->ui->delayComboBox->addItems(list);
    this->ui->delayComboBox->setCurrentIndex(5);

    this->ui->startButton->setEnabled(false);
    this->ui->stopButton->setEnabled(false);

    dirFunction();

    animationActive = false;
}

AnimationViewer::~AnimationViewer()
{
    delete ui;
}

void AnimationViewer::dirFunction()
{
    QDir folder("");
    folderString = folder.absolutePath();
    folder.setNameFilters(QStringList()<<"*.jpg");
    fileList = folder.entryList();

    fileCount = fileList.count();

    if(fileCount > 1)
    {
        this->ui->startButton->setEnabled(true);
    }
    else if(fileCount == 1)
    {
        this->ui->startButton->setEnabled(false);
        this->ui->stopButton->setEnabled(false);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("No compatible pictures in folder...");
        msgBox.exec();

        return;
    }

    index = 0;

    showImage();
}

void AnimationViewer::showImage()
{
    QPixmap pix(folderString + "/" + fileList[index]);
    this->ui->imageLabel->setPixmap(pix);
    this->ui->imageLabel->setAlignment(Qt::AlignCenter);
}

void AnimationViewer::on_delayComboBox_currentIndexChanged(int index)
{
   delay = (index+1)*5;
}

void AnimationViewer::animationFunction()
{
    this->ui->delayComboBox->setEnabled(false);
    this->ui->delayLabel->setEnabled(false);

    if(index < fileCount-1)
    {
        index++;
    }
    else
    {
        index = 0;
    }

    showImage();
}

void AnimationViewer::on_startButton_clicked()
{
    animationActive = true;

    this->ui->startButton->setEnabled(false);
    this->ui->stopButton->setEnabled(true);
    this->ui->delayComboBox->setEnabled(false);
    this->ui->delayLabel->setEnabled(false);

    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(animationFunction()));
    timer->start(1000/delay);
}

void AnimationViewer::on_stopButton_clicked()
{
    animationActive = false;

    this->ui->startButton->setEnabled(true);
    this->ui->stopButton->setEnabled(false);
    this->ui->delayComboBox->setEnabled(true);
    this->ui->delayLabel->setEnabled(true);

    timer->stop();
}

void AnimationViewer::closeEvent(QCloseEvent *event)
{
    if(animationActive == false)
    {
        event->accept();
    }
    else if(mayExit())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

bool AnimationViewer::mayExit()
{
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Fractoria"), tr("Animation is still running.\nAre you sure you want to exit ?"), QMessageBox::Cancel | QMessageBox::Ok);

    if(ret == QMessageBox::Cancel)
    {
        return false;
    }
    else if(ret == QMessageBox::Ok)
    {
        return true;
    }

    return true;
}

void AnimationViewer::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Fractal Animation", "", "AVI (*.avi)");
    QFile file(fileName);

    if(file.open(QIODevice::WriteOnly))
    {
        QString sPath = QDir::currentPath();

        QProcess *mencodeProcess = new QProcess(this);
        mencodeProcess->setWorkingDirectory(sPath);

        QStringList arg;
        QString program = sPath + "/mencoder";

        arg << "mf://*.jpg" << "-mf" << "w=900:h=600:fps=30:type=jpg" << "-ovc"
            << "lavc" << "-lavcopts" << "vcodec=mjpeg" << "-oac" << "copy"
            << "-o" << fileName;

        mencodeProcess->start(program, arg);
        QString msg;
        if (!mencodeProcess->waitForFinished(-1))
            msg = "Mencoder failed:" + mencodeProcess->errorString();
        else
            msg = "Mencoder output:" + mencodeProcess->readAll();
        qDebug() << msg;
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Fractal animation was not saved...");
        msgBox.exec();

        return;
    }
}
