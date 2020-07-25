#ifndef IFS_H
#define IFS_H

#include <QWidget>
#include <QImage>

namespace Ui {
class IFS;
}

class IFS : public QWidget
{
    Q_OBJECT

public:
    explicit IFS(QWidget *parent = 0);
    ~IFS();

    QImage img;

    long long m_iIterations;

    int m_iRed;
    int m_iGreen;
    int m_iBlue;

    bool m_bSpecialColoring;

    double m_dZoomRatio;

    double m_dCenterX;
    double m_dCenterY;

    double m_dCoordX;
    double m_dCoordY;

private slots:
    void on_renderButton_clicked();
    void showIFSImage();
    
    void renderSierpinskiTriangle();
    void renderSierpinskiCarpet();
    void renderKochCurve();
    void renderSpiral();
    void renderFern();
    void renderTricurve();
    void renderCatalyst();
    void renderKnife();

    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void on_redLineEdit_textChanged(const QString &arg1);
    void on_greenLineEdit_textChanged(const QString &arg1);
    void on_blueLineEdit_textChanged(const QString &arg1);

    void on_iterationsLineEdit_textChanged(const QString &arg1);

    void on_specialColoringCheckBox_toggled(bool checked);

    void on_zoomRatioLineEdit_textChanged(const QString &arg1);

    void on_centerXLineEdit_textChanged(const QString &arg1);

    void on_centerYLineEdit_textChanged(const QString &arg1);

    void on_colorPickerPushButton_clicked();

private:
    Ui::IFS *ui;
};

#endif // IFS_H
