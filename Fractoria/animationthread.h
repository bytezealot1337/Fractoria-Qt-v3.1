#ifndef ANIMATIONTHREAD_H
#define ANIMATIONTHREAD_H

#include <QObject>
#include <QImage>
#include <QDir>

#define TRIGGER_POINTS_NUMBER 8

class AnimationThread : public QObject
{
    Q_OBJECT
public:
    explicit AnimationThread(QObject *parent = 0);

    QImage img;

    bool m_bSuspendValue;

    // Current Geometry Variables
    double m_dCenterX;
    double m_dCenterY;
    double m_dZoomRatio;
    double m_dRotation;
    long long m_iIteration;
    double m_dBailout;

    // Current Control Variables
    double m_dControl1;
    double m_dControl2;
    double m_dControl3;
    double m_dControl4;
    double m_dControl5;
    double m_dControl6;

    // Current Outside Color Variables
    int m_iOutsideRedColorAdder;
    int m_iOutsideGreenColorAdder;
    int m_iOutsideBlueColorAdder;
    double m_dOutsideRedColorMultiplier;
    double m_dOutsideGreenColorMultiplier;
    double m_dOutsideBlueColorMultiplier;

    // Current Inside Color Variables
    int m_iInsideRedColorAdder;
    int m_iInsideGreenColorAdder;
    int m_iInsideBlueColorAdder;
    double m_dInsideRedColorMultiplier;
    double m_dInsideGreenColorMultiplier;
    double m_dInsideBlueColorMultiplier;


    QString m_sFractalType;

    int m_iTotalFrameNumber;
    int m_iTriggerIndex;

    // Geometry Animation Variables
    QList<int> m_iTriggerFrame;
    QList<double> m_dEndCenterX;
    QList<double> m_dEndCenterY;
    QList<int> m_iTranslationFunction;
    QList<double> m_dEndZoomRatio;
    QList<int> m_iZoomRatioFunction;
    QList<double> m_dEndRotation;
    QList<int> m_iRotationFunction;
    QList<long long> m_iEndIteration;
    QList<int> m_iIterationFunction;

    // Formula Animation Variables
    QList<double> m_dAnimCtrl1;
    QList<int> m_iAnimCtrl1Function;
    QList<double> m_dAnimCtrl2;
    QList<int> m_iAnimCtrl2Function;
    QList<double> m_dAnimCtrl3;
    QList<int> m_iAnimCtrl3Function;
    QList<double> m_dAnimCtrl4;
    QList<int> m_iAnimCtrl4Function;
    QList<double> m_dAnimCtrl5;
    QList<int> m_iAnimCtrl5Function;
    QList<double> m_dAnimCtrl6;
    QList<int> m_iAnimCtrl6Function;

    // Outside Color Animation Variables
    QList<int> m_iAnimOutsideRedAdder;
    QList<int> m_iAnimOutsideRedAddFunction;
    QList<int> m_iAnimOutsideGreenAdder;
    QList<int> m_iAnimOutsideGreenAddFunction;
    QList<int> m_iAnimOutsideBlueAdder;
    QList<int> m_iAnimOutsideBlueAddFunction;
    QList<double> m_dAnimOutsideRedMultiplier;
    QList<int> m_iAnimOutsideRedMultFunction;
    QList<double> m_dAnimOutsideGreenMultiplier;
    QList<int> m_iAnimOutsideGreenMultFunction;
    QList<double> m_dAnimOutsideBlueMultiplier;
    QList<int> m_iAnimOutsideBlueMultFunction;

    // Inside Color Animation Variables
    QList<int> m_iAnimInsideRedAdder;
    QList<int> m_iAnimInsideRedAddFunction;
    QList<int> m_iAnimInsideGreenAdder;
    QList<int> m_iAnimInsideGreenAddFunction;
    QList<int> m_iAnimInsideBlueAdder;
    QList<int> m_iAnimInsideBlueAddFunction;
    QList<double> m_dAnimInsideRedMultiplier;
    QList<int> m_iAnimInsideRedMultFunction;
    QList<double> m_dAnimInsideGreenMultiplier;
    QList<int> m_iAnimInsideGreenMultFunction;
    QList<double> m_dAnimInsideBlueMultiplier;
    QList<int> m_iAnimInsideBlueMultFunction;

    // Coloring Parameters
    int m_iColoringAlgorithm;
    int m_iAlgorithmAttributes;

    double m_dRedMultiplier;
    double m_dGreenMultiplier;
    double m_dBlueMultiplier;

    bool m_bRedMainColor;
    bool m_bGreenMainColor;
    bool m_bBlueMainColor;

    // Newton Root Parameters
    int m_iFirstRootRed;
    int m_iFirstRootGreen;
    int m_iFirstRootBlue;
    int m_iSecondRootRed;
    int m_iSecondRootGreen;
    int m_iSecondRootBlue;
    int m_iThirdRootRed;
    int m_iThirdRootGreen;
    int m_iThirdRootBlue;

    // Delete Previous jpg files Parameters
    QStringList fileList;
    int fileCount;

    void setShowProgressionParameter(bool bShowProgression);

    bool m_bShowProgression;

signals:
    void workFinished();
    void animationRendered(QImage img);
    void animationProgress(int progressValue);
    void animationTimerUpdate(double duration);
    void progress(int progressValue);
    void timerUpdate(double duration);
    void rendered(QImage img);

    void updateGeometryParameters(double xCenter, double yCenter, double zoomRatio, double rotation, long long iterations, double bailout);

    void updateFormulaParameters(double control1, double control2, double control3, double control4, double control5, double control6);

    void updateOutsideColoringParameters(int iOutsideRedColorAdder, int iOutsideGreenColorAdder, int iOutsideBlueColorAdder,
                                   double dOutsideRedColorMultiplier, double dOutsideGreenColorMultiplier, double dOutsideBlueColorMultiplier);

    void updateInsideColoringParameters(int iInsideRedColorAdder, int iInsideGreenColorAdder, int iInsideBlueColorAdder,
                                  double dInsideRedColorMultiplier, double dInsideGreenColorMultiplier, double dInsideBlueColorMultiplier);

public slots:
    void setCurrentGeometryParameters(double dCenterX, double dCenterY, double dZoomRatio, double dRotation, long long iIteration, double dBailout);

    void setCurrentControlParameters(double dControl1, double dControl2, double dControl3, double dControl4, double dControl5, double dControl6);

    void setCurrentOutsideColoringParameters(int iOutsideRedColorAdder, int iOutsideGreenColorAdder, int iOutsideBlueColorAdder,
                                             double dOutsideRedColorMultiplier, double dOutsideGreenColorMultiplier, double dOutsideBlueColorMultiplier);

    void setCurrentInsideColoringParameters(int iInsideRedColorAdder, int iInsideGreenColorAdder, int iInsideBlueColorAdder,
                                             double dInsideRedColorMultiplier, double dInsideGreenColorMultiplier, double dInsideBlueColorMultiplier);

    void setFractalType(QString sFractalType);

    void setTotalFrameNumber(int iTotalFrameNumber);

    void setGeometryParameters(QList<int> iTriggerFrame, QList<double> dEndCenterX, QList<double> dEndCenterY,
                               QList<int> iTranslationFunction, QList<double> dEndZoomRatio, QList<int> iZoomRatioFunction,
                               QList<double> dEndRotation, QList<int> iRotationFunction, QList<long long> iEndIteration, QList<int> iIterationFunction);

    void setFormulaParameters(QList<double> dAnimCtrl1, QList<int> iAnimCtrl1Function, QList<double> dAnimCtrl2,
                               QList<int> iAnimCtrl2Function, QList<double> dAnimCtrl3, QList<int> iAnimCtrl3Function,
                               QList<double> dAnimCtrl4, QList<int> iAnimCtrl4Function, QList<double> dAnimCtrl5,
                               QList<int> iAnimCtrl5Function, QList<double> dAnimCtrl6, QList<int> iAnimCtrl6Function);

    void setOutsideColoringParameters(QList<int> iAnimOutsideRedAdder, QList<int> iAnimOutsideRedAddFunction,
                                      QList<int> iAnimOutsideGreenAdder, QList<int> iAnimOutsideGreenAddFunction,
                                      QList<int> iAnimOutsideBlueAdder, QList<int> iAnimOutsideBlueAddFunction,
                                      QList<double> dAnimOutsideRedMultiplier, QList<int> iAnimOutsideRedMultFunction,
                                      QList<double> dAnimOutsideGreenMultiplier, QList<int> iAnimOutsideGreenMultFunction,
                                      QList<double> dAnimOutsideBlueMultiplier, QList<int> iAnimOutsideBlueMultFunction);

    void setInsideColoringParameters(QList<int> iAnimInsideRedAdder, QList<int> iAnimInsideRedAddFunction,
                                     QList<int> iAnimInsideGreenAdder, QList<int> iAnimInsideGreenAddFunction,
                                     QList<int> iAnimInsideBlueAdder, QList<int> iAnimInsideBlueAddFunction,
                                     QList<double> dAnimInsideRedMultiplier, QList<int> iAnimInsideRedMultFunction,
                                     QList<double> dAnimInsideGreenMultiplier, QList<int> iAnimInsideGreenMultFunction,
                                     QList<double> dAnimInsideBlueMultiplier, QList<int> iAnimInsideBlueMultFunction);

    void setColoringParameters(int iColoringAlgorithm, int iAlgorithmAttributes,
                               double dRedMultiplier, double dGreenMultiplier, double dBlueMultiplier,
                               bool bRedMainColor, bool bGreenMainColor, bool bBlueMainColor);

    void setNewtonRootColoringPrameters(int iFirstRootRed, int iFirstRootGreen, int iFirstRootBlue,
                                        int iSecondRootRed, int iSecondRootGreen, int iSecondRootBlue,
                                        int iThirdRootRed, int iThirdRootGreen, int iThirdRootBlue);

    void renderAnimation();

    void removeAllJpgFilesInFolder();

    void renderAnimationMandelbrotQuick(QImage *img);
    void renderAnimationMandelbrot(QImage *img);
    void renderAnimationJuliaQuick(QImage *img);
    void renderAnimationJulia(QImage *img);
    void renderAnimationNewton(QImage *img);
    void renderAnimationNewtonRoot(QImage *img);
    void renderAnimationNova(QImage *img);
    void renderAnimationNovaSpecial(QImage *img);
    void renderAnimationGlynn(QImage *img);
    void renderAnimationPhoenix(QImage *img);
};

#endif // ANIMATIONTHREAD_H
