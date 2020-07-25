#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QImage>
#include <QDebug>

class RenderThread : public QObject
{
Q_OBJECT
public:
    RenderThread();
    ~RenderThread();

    QImage img;

    bool m_bSuspendValue;

    void geometryParameters(double xCenter, double yCenter, double zoomRatio, double rotation, long long iterations, double bailout);

    double m_dCenterX;
    double m_dCenterY;
    double m_dZoomRatio;
    double m_dRotation;
    long long m_iIteration;
    double m_dBailout;

    void formulaParameters(double control1, double control2, double control3, double control4, double control5, double control6);

    double m_dControl1;
    double m_dControl2;
    double m_dControl3;
    double m_dControl4;
    double m_dControl5;
    double m_dControl6;

    void outsideColoringParameters(int iOutsideRedColorAdder, int iOutsideGreenColorAdder, int iOutsideBlueColorAdder,
                                   double dOutsideRedColorMultiplier, double dOutsideGreenColorMultiplier, double dOutsideBlueColorMultiplier);

    int m_iOutsideRedColorAdder;
    int m_iOutsideGreenColorAdder;
    int m_iOutsideBlueColorAdder;
    double m_dOutsideRedColorMultiplier;
    double m_dOutsideGreenColorMultiplier;
    double m_dOutsideBlueColorMultiplier;

    void insideColoringParameters(int iInsideRedColorAdder, int iInsideGreenColorAdder, int iInsideBlueColorAdder,
                                  double dInsideRedColorMultiplier, double dInsideGreenColorMultiplier, double dInsideBlueColorMultiplier);

    int m_iInsideRedColorAdder;
    int m_iInsideGreenColorAdder;
    int m_iInsideBlueColorAdder;
    double m_dInsideRedColorMultiplier;
    double m_dInsideGreenColorMultiplier;
    double m_dInsideBlueColorMultiplier;

    void newtonRootColoringParameters(int iFirstRootRed, int iFirstRootGreen, int iFirstRootBlue,
                                      int iSecondRootRed, int iSecondRootGreen, int iSecondRootBlue,
                                      int iThirdRootRed, int iThirdRootGreen, int iThirdRootBlue);

    int m_iFirstRootRed;
    int m_iFirstRootGreen;
    int m_iFirstRootBlue;
    int m_iSecondRootRed;
    int m_iSecondRootGreen;
    int m_iSecondRootBlue;
    int m_iThirdRootRed;
    int m_iThirdRootGreen;
    int m_iThirdRootBlue;


    void setColoringParameters(int iColoringAlgorithm, int iAlgorithmAttributes,
                               double dRedMultiplier, double dGreenMultiplier, double dBlueMultiplier,
                               bool bRedMainColor, bool bGreenMainColor, bool bBlueMainColor);

    int m_iColoringAlgorithm;
    int m_iAlgorithmAttributes;

    double m_dRedMultiplier;
    double m_dGreenMultiplier;
    double m_dBlueMultiplier;

    bool m_bRedMainColor;
    bool m_bGreenMainColor;
    bool m_bBlueMainColor;

    void setSwitchFractalModeParameter(bool bSwitchFractalMode);

    bool m_bSwitchFractalMode;

    void setShowProgressionParameter(bool bShowProgression);

    bool m_bShowProgression;


public slots:
    void renderMandelbrotQuick();
    void renderMandelbrot();
    void renderJuliaQuick();
    void renderJulia();
    void renderNewton();
    void renderNewtonRoot();
    void renderNova();
    void renderNovaSpecial();
    void renderGlynn();
    void renderPhoenix();

signals:
    void workFinished();
    void rendered(QImage img);
    void progress(int progressValue);
    void timerUpdate(double duration);
};

#endif // RENDERTHREAD_H
