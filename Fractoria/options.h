#ifndef OPTIONS_H
#define OPTIONS_H

#include <QWidget>

namespace Ui {
class Options;
}

class Options : public QWidget
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = 0);
    ~Options();

    bool m_bShowProgression;
    bool m_bFirstTime;

public slots:
    void setShowProgressionParameter(bool bShowProgression);

private slots:
    void on_showProgressionCheckBox_toggled(bool checked);
    void on_okButton_clicked();
    void on_resetAnimationPropertiesButton_clicked();

    void keyPressEvent(QKeyEvent *event);

private:
    Ui::Options *ui;

signals:
    void showProgressionParameter(bool bShowProgression);
    void animationFirstTimeParameter(bool bFirstTime);
};

#endif // OPTIONS_H
