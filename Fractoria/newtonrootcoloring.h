#ifndef NEWTONROOTCOLORING_H
#define NEWTONROOTCOLORING_H

#include <QWidget>

namespace Ui {
class NewtonRootColoring;
}

class NewtonRootColoring : public QWidget
{
    Q_OBJECT

public:
    explicit NewtonRootColoring(QWidget *parent = 0);
    ~NewtonRootColoring();

    int m_iFirstRootRed;
    int m_iFirstRootGreen;
    int m_iFirstRootBlue;
    int m_iSecondRootRed;
    int m_iSecondRootGreen;
    int m_iSecondRootBlue;
    int m_iThirdRootRed;
    int m_iThirdRootGreen;
    int m_iThirdRootBlue;

public slots:
    void setNewtonRootColoringData(int iFirstRootRed, int iFirstRootGreen, int iFirstRootBlue,
                                   int iSecondRootRed, int iSecondRootGreen, int iSecondRootBlue,
                                   int iThirdRootRed, int iThirdRootGreen, int iThirdRootBlue);

private slots:
    void on_firstRootRedLineEdit_textChanged(const QString &arg1);
    void on_firstRootGreenLineEdit_textChanged(const QString &arg1);
    void on_firstRootBlueLineEdit_textChanged(const QString &arg1);
    void on_secondRootRedLineEdit_textChanged(const QString &arg1);
    void on_secondRootGreenLineEdit_textChanged(const QString &arg1);
    void on_secondRootBlueLineEdit_textChanged(const QString &arg1);
    void on_thirdRootRedLineEdit_textChanged(const QString &arg1);
    void on_thirdRootGreenLineEdit_textChanged(const QString &arg1);
    void on_thirdRootBlueLineEdit_textChanged(const QString &arg1);

    void on_okButton_clicked();

    void keyPressEvent(QKeyEvent *event);

private:
    Ui::NewtonRootColoring *ui;

signals:
    void finished(int m_iFirstRootRed, int m_iFirstRootGreen, int m_iFirstRootBlue,
                  int m_iSecondRootRed, int m_iSecondRootGreen, int m_iSecondRootBlue,
                  int m_iThirdRootRed, int m_iThirdRootGreen, int m_iThirdRootBlue);
};

#endif // NEWTONROOTCOLORING_H
