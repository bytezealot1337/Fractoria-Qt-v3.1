#ifndef ANIMATIONVIEWER_H
#define ANIMATIONVIEWER_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class AnimationViewer;
}

class AnimationViewer : public QWidget
{
    Q_OBJECT

public:
    explicit AnimationViewer(QWidget *parent = 0);
    ~AnimationViewer();

private slots:
    void dirFunction();
    void showImage();

    void on_delayComboBox_currentIndexChanged(int index);
    void animationFunction();
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void closeEvent(QCloseEvent *event);
    bool mayExit();

    void on_pushButton_clicked();

private:
    Ui::AnimationViewer *ui;

    QString folderString;
    QStringList fileList;
    int index;
    int fileCount;
    QTimer *timer;
    int delay;
    bool animationActive;
};

#endif // ANIMATIONVIEWER_H
