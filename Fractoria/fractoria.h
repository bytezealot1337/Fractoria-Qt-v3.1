#ifndef FRACTORIA_H
#define FRACTORIA_H

#include <QMainWindow>
#include <QThread>
#include "renderthread.h"
#include <QImage>
#include "newtonrootcoloring.h"
#include "coloring.h"
#include "animationProperties.h"
#include "animationthread.h"
#include "animationviewer.h"
#include "options.h"
#include "ifs.h"

#define TRIGGER_POINTS_NUMBER 8

namespace Ui {
class Fractoria;
}

class Fractoria : public QMainWindow
{
    Q_OBJECT

public:
    explicit Fractoria(QWidget *parent = 0);
    ~Fractoria();

    Ui::Fractoria *ui;

    QImage img;
    QImage imgPreview;

private slots:
    void on_fractalTypeComboBox1_currentIndexChanged(int index);
    void on_fractalTypeComboBox2_currentIndexChanged(int index);

    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void on_xCenterLineEdit_textChanged(const QString &arg1);
    void on_yCenterLineEdit_textChanged(const QString &arg1);
    void on_zoomRatioLineEdit_textChanged(const QString &arg1);
    void on_rotationLineEdit_textChanged(const QString &arg1);
    void on_iterationsLineEdit_textChanged(const QString &arg1);
    void on_bailoutLineEdit_textChanged(const QString &arg1);

    void on_control1LineEdit_textChanged(const QString &arg1);
    void on_control2LineEdit_textChanged(const QString &arg1);
    void on_control3LineEdit_textChanged(const QString &arg1);
    void on_control4LineEdit_textChanged(const QString &arg1);
    void on_control5LineEdit_textChanged(const QString &arg1);
    void on_control6LineEdit_textChanged(const QString &arg1);

    void controlsFunction();
    void outsideColoringFunction();
    void insideColoringFunction();

    void on_renderButton_clicked();
    void showImage(QImage image);
    void showProgress(int progressValue);
    void animationShowProgress(int progressValue);
    void updateTimer(double duration);
    void animationUpdateTimer(double duration);

    void saveFractalImage();

    void closeApplication();
    void closeEvent(QCloseEvent *event);
    bool mayExit();

    void keyPressEvent(QKeyEvent *event);

    void preview();
    void previewMandelbrotQuick();
    void previewMandelbrot();
    void previewJuliaQuick();
    void previewJulia();
    void previewNewton();
    void previewNewtonRoot();
    void previewNova();
    void previewNovaSpecial();
    void previewGlynn();
    void previewPhoenix();

    void mousePressEvent(QMouseEvent *event);

    void on_outsideRedColorAdderLineEdit_textChanged(const QString &arg1);
    void on_outsideGreenColorAdderLineEdit_textChanged(const QString &arg1);
    void on_outsideBlueColorAdderLineEdit_textChanged(const QString &arg1);
    void on_outsideRedColorMultiplierLineEdit_textChanged(const QString &arg1);
    void on_outsideGreenColorMultiplierLineEdit_textChanged(const QString &arg1);
    void on_outsideBlueColorMultiplierLineEdit_textChanged(const QString &arg1);

    void on_insideRedColorAdderLineEdit_textChanged(const QString &arg1);
    void on_insideGreenColorAdderLineEdit_textChanged(const QString &arg1);
    void on_insideBlueColorAdderLineEdit_textChanged(const QString &arg1);
    void on_insideRedColorMultiplierLineEdit_textChanged(const QString &arg1);
    void on_insideGreenColorMultiplierLineEdit_textChanged(const QString &arg1);
    void on_insideBlueColorMultiplierLineEdit_textChanged(const QString &arg1);

    void on_outsideColorPalette_clicked();
    void on_insideColorPalette_clicked();

    void coloringPropertiesFunction();

    void getNewtonColoringData(int iFirstRootRed, int iFirstRootGreen, int iFirstRootBlue,
                               int iSecondRootRed, int iSecondRootGreen, int iSecondRootBlue,
                               int iThirdRootRed, int iThirdRootGreen, int iThirdRootBlue);

    void getColoringData(int iColoringAlgorithm, int iAlgorithmAttributes,
                         double dRedMultiplier, double dGreenMultiplier, double dBlueMultiplier,
                         bool bRedMainColor, bool bGreenMainColor, bool bBlueMainColor);

    void smoothNormalizationDefaultChanges();

    void animationPropertiesFunction();

    void on_animateButton_clicked();

    void updateGeometryParameters(double xCenter, double yCenter, double zoomRatio, double rotation, long long iterations, double bailout);

    void updateFormulaParameters(double control1, double control2, double control3, double control4, double control5, double control6);

    void updateOutsideColoringParameters(int iOutsideRedColorAdder, int iOutsideGreenColorAdder, int iOutsideBlueColorAdder,
                                   double dOutsideRedColorMultiplier, double dOutsideGreenColorMultiplier, double dOutsideBlueColorMultiplier);

    void updateInsideColoringParameters(int iInsideRedColorAdder, int iInsideGreenColorAdder, int iInsideBlueColorAdder,
                                  double dInsideRedColorMultiplier, double dInsideGreenColorMultiplier, double dInsideBlueColorMultiplier);

    void updateSuspendButtonState();
    void updateSuspendButtonStateAnimation();
    void on_suspendButton_clicked();

    void startAnimationViewer();

    void on_switchModeCheckBox_toggled(bool checked);

    void optionsFunction();

    void getShowProgressionParameter(bool bShowProgression);

    void resetAnimationProperties();

    void about();

    void presentIFSController();

private:

    QThread *thread;
    RenderThread *renderTask;

    double m_dCenterX;
    double m_dCenterY;
    double m_dCoordX;
    double m_dCoordY;
    double m_dZoomRatio;
    double m_dRotation;
    long long m_iIteration;
    double m_dBailout;

    double m_dControl1;
    double m_dControl2;
    double m_dControl3;
    double m_dControl4;
    double m_dControl5;
    double m_dControl6;

    int m_iOutsideRedColorAdder;
    int m_iOutsideGreenColorAdder;
    int m_iOutsideBlueColorAdder;
    double m_dOutsideRedColorMultiplier;
    double m_dOutsideGreenColorMultiplier;
    double m_dOutsideBlueColorMultiplier;

    int m_iInsideRedColorAdder;
    int m_iInsideGreenColorAdder;
    int m_iInsideBlueColorAdder;
    double m_dInsideRedColorMultiplier;
    double m_dInsideGreenColorMultiplier;
    double m_dInsideBlueColorMultiplier;

    NewtonRootColoring *newtonRootColoring;

    int m_iFirstRootRed;
    int m_iFirstRootGreen;
    int m_iFirstRootBlue;
    int m_iSecondRootRed;
    int m_iSecondRootGreen;
    int m_iSecondRootBlue;
    int m_iThirdRootRed;
    int m_iThirdRootGreen;
    int m_iThirdRootBlue;

    Coloring *coloring;

    int m_iColoringAlgorithm;
    int m_iAlgorithmAttributes;

    double m_dRedMultiplier;
    double m_dGreenMultiplier;
    double m_dBlueMultiplier;

    bool m_bRedMainColor;
    bool m_bGreenMainColor;
    bool m_bBlueMainColor;

public:
    QThread *animThread;
    AnimationThread *animateTask;

    AnimationProperties *animationProperties;

    bool firstTime;

    bool renderState;
    bool animationState;

    int m_iTotalFrameNumber;
    int m_iTriggerIndex;

    // Geometry Animation Values
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

    // Formula Animation Values
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

    AnimationViewer *animationViewer;

    bool m_bSwitchModeFractal;

    Options *options;

    bool m_bShowProgression;

    IFS *ifs;



public slots:
    void getAnimationGeometryPropertiesData(int iTotalFrameNumber, QList<int> iTriggerFrame, QList<double> dEndCenterX,
                                            QList<double> dEndCenterY, QList<int> iTranslationFunction, QList<double> dEndZoomRatio,
                                            QList<int> iZoomRatioFunction, QList<double> dEndRotation, QList<int> iRotationFunction,
                                            QList<long long> iEndIteration, QList<int> iIterationFunction);

    void getAnimtationFormulaPropertiesData(QList<double> dAnimCtrl1, QList<int> iAnimCtrl1Function, QList<double> dAnimCtrl2,
                                            QList<int> iAnimCtrl2Function, QList<double> dAnimCtrl3, QList<int> iAnimCtrl3Function,
                                            QList<double> dAnimCtrl4, QList<int> iAnimCtrl4Function, QList<double> dAnimCtrl5,
                                            QList<int> iAnimCtrl5Function, QList<double> dAnimCtrl6, QList<int> iAnimCtrl6Function);

    void getAnimationOutsideColoringPropertiesData(QList<int> iAnimOutsideRedAdder, QList<int> iAnimOutsideRedAddFunction,
                                                   QList<int> iAnimOutsideGreenAdder, QList<int> iAnimOutsideGreenAddFunction,
                                                   QList<int> iAnimOutsideBlueAdder, QList<int> iAnimOutsideBlueAddFunction,
                                                   QList<double> dAnimOutsideRedMultiplier, QList<int> iAnimOutsideRedMultFunction,
                                                   QList<double> dAnimOutsideGreenMultiplier, QList<int> iAnimOutsideGreenMultFunction,
                                                   QList<double> dAnimOutsideBlueMultiplier, QList<int> iAnimOutsideBlueMultFunction);

    void getAnimationInsideColoringPropertiesData(QList<int> iAnimInsideRedAdder, QList<int> iAnimInsideRedAddFunction,
                                                   QList<int> iAnimInsideGreenAdder, QList<int> iAnimInsideGreenAddFunction,
                                                   QList<int> iAnimInsideBlueAdder, QList<int> iAnimInsideBlueAddFunction,
                                                   QList<double> dAnimInsideRedMultiplier, QList<int> iAnimInsideRedMultFunction,
                                                   QList<double> dAnimInsideGreenMultiplier, QList<int> iAnimInsideGreenMultFunction,
                                                   QList<double> dAnimInsideBlueMultiplier, QList<int> iAnimInsideBlueMultFunction);

    void getAnimationProperties(bool bFirstTime);
};


#endif // FRACTORIA_H
