#include "animationthread.h"
#include <math.h>
#include <QColor>
#include <complex>
#include <QElapsedTimer>
#include <QMessageBox>

#define DEGtoRAD 0.01745329251994329576923690768489
#define PI 3.14159265358979323846264338327950288419

using namespace std;

AnimationThread::AnimationThread(QObject *parent) :
    QObject(parent)
{
    m_bSuspendValue = false;
    m_bShowProgression = false;
}

void AnimationThread::setCurrentGeometryParameters(double dCenterX, double dCenterY, double dZoomRatio,
                                                   double dRotation, long long iIteration, double dBailout)
{
    m_dCenterX = dCenterX;
    m_dCenterY = dCenterY;
    m_dZoomRatio = dZoomRatio;
    m_dRotation = dRotation;
    m_iIteration = iIteration;
    m_dBailout = dBailout;
}

void AnimationThread::setCurrentControlParameters(double dControl1, double dControl2, double dControl3, double dControl4, double dControl5, double dControl6)
{
    m_dControl1 = dControl1;
    m_dControl2 = dControl2;
    m_dControl3 = dControl3;
    m_dControl4 = dControl4;
    m_dControl5 = dControl5;
    m_dControl6 = dControl6;
}

void AnimationThread::setCurrentOutsideColoringParameters(int iOutsideRedColorAdder, int iOutsideGreenColorAdder,
                                                          int iOutsideBlueColorAdder, double dOutsideRedColorMultiplier,
                                                          double dOutsideGreenColorMultiplier, double dOutsideBlueColorMultiplier)
{
    m_iOutsideRedColorAdder = iOutsideRedColorAdder;
    m_iOutsideGreenColorAdder = iOutsideGreenColorAdder;
    m_iOutsideBlueColorAdder = iOutsideBlueColorAdder;
    m_dOutsideRedColorMultiplier = dOutsideRedColorMultiplier;
    m_dOutsideGreenColorMultiplier = dOutsideGreenColorMultiplier;
    m_dOutsideBlueColorMultiplier = dOutsideBlueColorMultiplier;
}

void AnimationThread::setCurrentInsideColoringParameters(int iInsideRedColorAdder, int iInsideGreenColorAdder,
                                                         int iInsideBlueColorAdder, double dInsideRedColorMultiplier,
                                                         double dInsideGreenColorMultiplier, double dInsideBlueColorMultiplier)
{
    m_iInsideRedColorAdder = iInsideRedColorAdder;
    m_iInsideGreenColorAdder = iInsideGreenColorAdder;
    m_iInsideBlueColorAdder = iInsideBlueColorAdder;
    m_dInsideRedColorMultiplier = dInsideRedColorMultiplier;
    m_dInsideGreenColorMultiplier = dInsideGreenColorMultiplier;
    m_dInsideBlueColorMultiplier = dInsideBlueColorMultiplier;
}

void AnimationThread::setFractalType(QString sFractalType)
{
    m_sFractalType = sFractalType;
}

void AnimationThread::setTotalFrameNumber(int iTotalFrameNumber)
{
    m_iTotalFrameNumber = iTotalFrameNumber;
}

void AnimationThread::setGeometryParameters(QList<int> iTriggerFrame, QList<double> dEndCenterX, QList<double> dEndCenterY,
                                            QList<int> iTranslationFunction, QList<double> dEndZoomRatio, QList<int> iZoomRatioFunction,
                                            QList<double> dEndRotation, QList<int> iRotationFunction,
                                            QList<long long> iEndIteration, QList<int> iIterationFunction)
{
    for(int n = 0; n < TRIGGER_POINTS_NUMBER; n++)
    {
        m_iTriggerFrame << iTriggerFrame[n];
        m_dEndCenterX << dEndCenterX[n];
        m_dEndCenterY << dEndCenterY[n];
        m_iTranslationFunction << iTranslationFunction[n];
        m_dEndZoomRatio << dEndZoomRatio[n];
        m_iZoomRatioFunction << iZoomRatioFunction[n];
        m_dEndRotation << dEndRotation[n];
        m_iRotationFunction << iRotationFunction[n];
        m_iEndIteration << iEndIteration[n];
        m_iIterationFunction << iIterationFunction[n];
    }
}

void AnimationThread::setFormulaParameters(QList<double> dAnimCtrl1, QList<int> iAnimCtrl1Function, QList<double> dAnimCtrl2,
                                           QList<int> iAnimCtrl2Function, QList<double> dAnimCtrl3, QList<int> iAnimCtrl3Function,
                                           QList<double> dAnimCtrl4, QList<int> iAnimCtrl4Function, QList<double> dAnimCtrl5,
                                           QList<int> iAnimCtrl5Function, QList<double> dAnimCtrl6, QList<int> iAnimCtrl6Function)
{
    for(int n = 0; n < TRIGGER_POINTS_NUMBER; n++)
    {
        m_dAnimCtrl1 << dAnimCtrl1[n];
        m_iAnimCtrl1Function << iAnimCtrl1Function[n];
        m_dAnimCtrl2 << dAnimCtrl2[n];
        m_iAnimCtrl2Function << iAnimCtrl2Function[n];
        m_dAnimCtrl3 << dAnimCtrl3[n];
        m_iAnimCtrl3Function << iAnimCtrl3Function[n];
        m_dAnimCtrl4 << dAnimCtrl4[n];
        m_iAnimCtrl4Function << iAnimCtrl4Function[n];
        m_dAnimCtrl5 << dAnimCtrl5[n];
        m_iAnimCtrl5Function << iAnimCtrl5Function[n];
        m_dAnimCtrl6 << dAnimCtrl6[n];
        m_iAnimCtrl6Function << iAnimCtrl6Function[n];
    }
}

void AnimationThread::setOutsideColoringParameters(QList<int> iAnimOutsideRedAdder, QList<int> iAnimOutsideRedAddFunction,
                                                   QList<int> iAnimOutsideGreenAdder, QList<int> iAnimOutsideGreenAddFunction,
                                                   QList<int> iAnimOutsideBlueAdder, QList<int> iAnimOutsideBlueAddFunction,
                                                   QList<double> dAnimOutsideRedMultiplier, QList<int> iAnimOutsideRedMultFunction,
                                                   QList<double> dAnimOutsideGreenMultiplier, QList<int> iAnimOutsideGreenMultFunction,
                                                   QList<double> dAnimOutsideBlueMultiplier, QList<int> iAnimOutsideBlueMultFunction)
{
    for(int n = 0; n < TRIGGER_POINTS_NUMBER; n++)
    {
        m_iAnimOutsideRedAdder << iAnimOutsideRedAdder[n];
        m_iAnimOutsideRedAddFunction << iAnimOutsideRedAddFunction[n];
        m_iAnimOutsideGreenAdder << iAnimOutsideGreenAdder[n];
        m_iAnimOutsideGreenAddFunction << iAnimOutsideGreenAddFunction[n];
        m_iAnimOutsideBlueAdder << iAnimOutsideBlueAdder[n];
        m_iAnimOutsideBlueAddFunction << iAnimOutsideBlueAddFunction[n];
        m_dAnimOutsideRedMultiplier << dAnimOutsideRedMultiplier[n];
        m_iAnimOutsideRedMultFunction << iAnimOutsideRedMultFunction[n];
        m_dAnimOutsideGreenMultiplier << dAnimOutsideGreenMultiplier[n];
        m_iAnimOutsideGreenMultFunction << iAnimOutsideGreenMultFunction[n];
        m_dAnimOutsideBlueMultiplier << dAnimOutsideBlueMultiplier[n];
        m_iAnimOutsideBlueMultFunction << iAnimOutsideBlueMultFunction[n];
    }
}

void AnimationThread::setInsideColoringParameters(QList<int> iAnimInsideRedAdder, QList<int> iAnimInsideRedAddFunction,
                                                  QList<int> iAnimInsideGreenAdder, QList<int> iAnimInsideGreenAddFunction,
                                                  QList<int> iAnimInsideBlueAdder, QList<int> iAnimInsideBlueAddFunction,
                                                  QList<double> dAnimInsideRedMultiplier, QList<int> iAnimInsideRedMultFunction,
                                                  QList<double> dAnimInsideGreenMultiplier, QList<int> iAnimInsideGreenMultFunction,
                                                  QList<double> dAnimInsideBlueMultiplier, QList<int> iAnimInsideBlueMultFunction)
{
    for(int n = 0; n < TRIGGER_POINTS_NUMBER; n++)
    {
        m_iAnimInsideRedAdder << iAnimInsideRedAdder[n];
        m_iAnimInsideRedAddFunction << iAnimInsideRedAddFunction[n];
        m_iAnimInsideGreenAdder << iAnimInsideGreenAdder[n];
        m_iAnimInsideGreenAddFunction << iAnimInsideGreenAddFunction[n];
        m_iAnimInsideBlueAdder << iAnimInsideBlueAdder[n];
        m_iAnimInsideBlueAddFunction << iAnimInsideBlueAddFunction[n];
        m_dAnimInsideRedMultiplier << dAnimInsideRedMultiplier[n];
        m_iAnimInsideRedMultFunction << iAnimInsideRedMultFunction[n];
        m_dAnimInsideGreenMultiplier << dAnimInsideGreenMultiplier[n];
        m_iAnimInsideGreenMultFunction << iAnimInsideGreenMultFunction[n];
        m_dAnimInsideBlueMultiplier << dAnimInsideBlueMultiplier[n];
        m_iAnimInsideBlueMultFunction << iAnimInsideBlueMultFunction[n];
    }
}

void AnimationThread::setColoringParameters(int iColoringAlgorithm, int iAlgorithmAttributes,
                                            double dRedMultiplier, double dGreenMultiplier, double dBlueMultiplier,
                                            bool bRedMainColor, bool bGreenMainColor, bool bBlueMainColor)
{
    m_iColoringAlgorithm = iColoringAlgorithm;
    m_iAlgorithmAttributes = iAlgorithmAttributes;
    m_dRedMultiplier = dRedMultiplier;
    m_dGreenMultiplier = dGreenMultiplier;
    m_dBlueMultiplier = dBlueMultiplier;
    m_bRedMainColor = bRedMainColor;
    m_bGreenMainColor = bGreenMainColor;
    m_bBlueMainColor = bBlueMainColor;
}

void AnimationThread::setNewtonRootColoringPrameters(int iFirstRootRed, int iFirstRootGreen, int iFirstRootBlue,
                                                      int iSecondRootRed, int iSecondRootGreen, int iSecondRootBlue,
                                                      int iThirdRootRed, int iThirdRootGreen, int iThirdRootBlue)
{
    m_iFirstRootRed = iFirstRootRed;
    m_iFirstRootGreen = iFirstRootGreen;
    m_iFirstRootBlue = iFirstRootBlue;

    m_iSecondRootRed = iSecondRootRed;
    m_iSecondRootGreen = iSecondRootGreen;
    m_iSecondRootBlue = iSecondRootBlue;

    m_iThirdRootRed = iThirdRootRed;
    m_iThirdRootGreen = iThirdRootGreen;
    m_iThirdRootBlue = iThirdRootBlue;
}

void AnimationThread::setShowProgressionParameter(bool bShowProgression)
{
    m_bShowProgression = bShowProgression;
}

void AnimationThread::renderAnimation()
{
    removeAllJpgFilesInFolder();

    img = QImage(900, 600, QImage::Format_ARGB32);

    int triggerPoint = 1;
    int iCurrentFrame = 1;

    float progressValue = 0.0;
    emit animationProgress(progressValue);

    QElapsedTimer timer;
    timer.start();


    if(m_sFractalType == "Mandelbrot (Quick)")
    {
        renderAnimationMandelbrotQuick(&img);
    }
    else if(m_sFractalType == "Mandelbrot")
    {
        renderAnimationMandelbrot(&img);
    }
    else if(m_sFractalType == "Julia (Quick)")
    {
        renderAnimationJuliaQuick(&img);
    }
    else if(m_sFractalType == "Julia")
    {
        renderAnimationJulia(&img);
    }
    else if(m_sFractalType == "Newton")
    {
        renderAnimationNewton(&img);
    }
    else if(m_sFractalType == "Newton (Root)")
    {
        renderAnimationNewtonRoot(&img);
    }
    else if(m_sFractalType == "Nova")
    {
        renderAnimationNova(&img);
    }
    else if(m_sFractalType == "Nova (Special)")
    {
        renderAnimationNovaSpecial(&img);
    }
    else if(m_sFractalType == "Glynn")
    {
        renderAnimationGlynn(&img);
    }
    else if(m_sFractalType == "Phoenix")
    {
        renderAnimationPhoenix(&img);
    }

    img.save("000001.jpg", "JPG", -1);

    progressValue = (100*iCurrentFrame/m_iTotalFrameNumber);

    emit animationProgress(progressValue);
//    emit animationTimerUpdate(timer.elapsed()/1000.0);


    // Geometry Back-Up Variables
    double dXAxisBU = m_dCenterX;
    double dYAxisBU = m_dCenterY;
    double dZoomRatioBU = m_dZoomRatio;
    double dRotationBU = m_dRotation;
    long long iIterationBU = m_iIteration;
    double dIteration = iIterationBU;

    // Formulae Back-Up Variables
    double dControl1BU = m_dControl1;
    double dControl2BU = m_dControl2;
    double dControl3BU = m_dControl3;
    double dControl4BU = m_dControl4;
    double dControl5BU = m_dControl5;
    double dControl6BU = m_dControl6;

    // Coloring Back-Up Variables
    int iOutRedAdderBU = m_iOutsideRedColorAdder;
    double dOutRedAdder = iOutRedAdderBU;
    int iOutGreenAdderBU = m_iOutsideGreenColorAdder;
    double dOutGreenAdder = iOutGreenAdderBU;
    int iOutBlueAdderBU = m_iOutsideBlueColorAdder;
    double dOutBlueAdder = iOutBlueAdderBU;
    double dOutRedMultBU = m_dOutsideRedColorMultiplier;
    double dOutGreenMultBU = m_dOutsideGreenColorMultiplier;
    double dOutBlueMultBU = m_dOutsideBlueColorMultiplier;
    int iInRedAdderBU = m_iInsideRedColorAdder;
    double dInRedAdder = iInRedAdderBU;
    int iInGreenAdderBU = m_iInsideGreenColorAdder;
    double dInGreenAdder = iInGreenAdderBU;
    int iInBlueAdderBU = m_iInsideBlueColorAdder;
    double dInBlueAdder = iInBlueAdderBU;
    double dInRedMultBU = m_dInsideRedColorMultiplier;
    double dInGreenMultBU = m_dInsideGreenColorMultiplier;
    double dInBlueMultBU = m_dInsideBlueColorMultiplier;


    for(iCurrentFrame = 2; iCurrentFrame <= m_iTotalFrameNumber; iCurrentFrame++)
    {

        if(iCurrentFrame <= m_iTriggerFrame[0])
        {// First Trigger Point Functions

            if(m_iTranslationFunction[0] == 0)
            {// Linear Translation Function

                m_dCenterX += (m_dEndCenterX[0] - dXAxisBU)/(double)(m_iTriggerFrame[0]-1);

                m_dCenterY += (m_dEndCenterY[0] - dYAxisBU)/(double)(m_iTriggerFrame[0]-1);

            }
            else if(m_iTranslationFunction[0] == 1)
            {// Positive Quadratic Translation Function

                double counter = (double)(iCurrentFrame-1);

                double aX = (m_dEndCenterX[0]-dXAxisBU)/pow((m_iTriggerFrame[0]-1),2.0);

                m_dCenterX = aX*counter*counter + dXAxisBU;

                double aY = (m_dEndCenterY[0]-dYAxisBU)/pow((m_iTriggerFrame[0]-1),2.0);

                m_dCenterY = aY*counter*counter + dYAxisBU;

            }
            else if(m_iTranslationFunction[0] == 2)
            {// Negative Quadratic Translation Function

                double counter = (double)(iCurrentFrame-1);

                double bX = (m_dEndCenterX[0]-dXAxisBU)/((m_iTriggerFrame[0]-1)/2.0);
                double aX = -bX/((m_iTriggerFrame[0]-1)*2.0);

                m_dCenterX = aX*counter*counter + bX*counter + dXAxisBU;

                double bY = (m_dEndCenterY[0]-dYAxisBU)/((m_iTriggerFrame[0]-1)/2.0);
                double aY = -bY/((m_iTriggerFrame[0]-1)*2.0);

                m_dCenterY = aY*counter*counter + bY*counter + dYAxisBU;

            }

            if(m_iRotationFunction[0] == 0)
            {// Linear Rotation Function

                m_dRotation += (m_dEndRotation[0] - dRotationBU)/(double)(m_iTriggerFrame[0]-1);

            }
            else if(m_iRotationFunction[0] == 1)
            {// Positive Quadratic Rotation Function

                double counter = (double)(iCurrentFrame-1);

                double a = (m_dEndRotation[0]-dRotationBU) / pow((double)(m_iTriggerFrame[0]-1),2);

                m_dRotation = a*counter*counter + dRotationBU;

            }
            else if(m_iRotationFunction[0] == 2)
            {// Negative Quadratic Rotation Function

                double counter = (double)(iCurrentFrame-1);

                double b = (m_dEndRotation[0]-dRotationBU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                m_dRotation = a*counter*counter + b*counter + dRotationBU;

            }

            if(m_iZoomRatioFunction[0] == 0)
            {// Linear Zoom Ratio Function

                m_dZoomRatio += (m_dEndZoomRatio[0]-dZoomRatioBU) / (double)(m_iTriggerFrame[0]-1);

            }
            else if(m_iZoomRatioFunction[0] == 1)
            {// Positive Quadratic Zoom Ratio Function

                double a = (m_dEndZoomRatio[0]-dZoomRatioBU) / pow((double)(m_iTriggerFrame[0]-1),2);

                double counter = (double)(iCurrentFrame-1);

                m_dZoomRatio = a*counter*counter + dZoomRatioBU;

            }
            else if(m_iZoomRatioFunction[0] == 2)
            {// Negative Quadratic Zoom Ratio Function

                double b = (m_dEndZoomRatio[0]-dZoomRatioBU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                m_dZoomRatio = a*counter*counter + b*counter + dZoomRatioBU;

            }

            if(m_iIterationFunction[0] == 0)
            {// Linear Iteration Function

                dIteration += (double)((double)(m_iEndIteration[0]-iIterationBU) / (double)(m_iTriggerFrame[0]-1));

                m_iIteration = (long long)(dIteration+0.5);

            }
            else if(m_iIterationFunction[0] == 1)
            {// Positive Quadratic Iteration Function

                double a = (double)((m_iEndIteration[0]-iIterationBU) / pow((double)(m_iTriggerFrame[0]-1),2));

                double counter = (double)(iCurrentFrame-1);

                dIteration = a*counter*counter + iIterationBU;

                m_iIteration = (long long)(dIteration+0.5);

            }
            else if(m_iIterationFunction[0] == 2)
            {// Negative Quadratic Iteration Function

                double b = (m_iEndIteration[0]-iIterationBU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                dIteration = a*counter*counter + b*counter + iIterationBU;

                m_iIteration = (long long)(dIteration+0.5);

            }


            // Formula Control Functions

            if(m_iAnimCtrl1Function[0] == 0)
            {// Linear Control #1 Function

                m_dControl1 += (m_dAnimCtrl1[0]-dControl1BU) / (m_iTriggerFrame[0]-1);

            }
            else if(m_iAnimCtrl1Function[0] == 1)
            {// Positive Quadratic Control #1 Function

                double a = (m_dAnimCtrl1[0]-dControl1BU) / pow((double)(m_iTriggerFrame[0]-1),2);

                double counter = (double)(iCurrentFrame-1);

                m_dControl1 = a*counter*counter + dControl1BU;

            }
            else if(m_iAnimCtrl1Function[0] == 2)
            {// Negative Quadratic Control #1 Function

                double b = (m_dAnimCtrl1[0]-dControl1BU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                m_dControl1 = a*counter*counter + b*counter + dControl1BU;

            }

            if(m_iAnimCtrl2Function[0] == 0)
            {// Linear Control #2 Function

                m_dControl2 += (m_dAnimCtrl2[0]-dControl2BU) / (m_iTriggerFrame[0]-1);

            }
            else if(m_iAnimCtrl2Function[0] == 1)
            {// Positive Quadratic Control #2 Function

                double a = (m_dAnimCtrl2[0]-dControl2BU) / pow((double)(m_iTriggerFrame[0]-1),2);

                double counter = (double)(iCurrentFrame-1);

                m_dControl2 = a*counter*counter + dControl2BU;

            }
            else if(m_iAnimCtrl2Function[0] == 2)
            {// Negative Quadratic Control #2 Function

                double b = (m_dAnimCtrl2[0]-dControl2BU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                m_dControl2 = a*counter*counter + b*counter + dControl2BU;

            }

            if(m_iAnimCtrl3Function[0] == 0)
            {// Linear Control #3 Function

                m_dControl3 += (m_dAnimCtrl3[0]-dControl3BU) / (m_iTriggerFrame[0]-1);

            }
            else if(m_iAnimCtrl3Function[0] == 1)
            {// Positive Quadratic Control #3 Function

                double a = (m_dAnimCtrl3[0]-dControl3BU) / pow((double)(m_iTriggerFrame[0]-1),2);

                double counter = (double)(iCurrentFrame-1);

                m_dControl3 = a*counter*counter + dControl3BU;

            }
            else if(m_iAnimCtrl3Function[0] == 2)
            {// Negative Quadratic Control #3 Function

                double b = (m_dAnimCtrl3[0]-dControl3BU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                m_dControl3 = a*counter*counter + b*counter + dControl3BU;

            }

            if(m_iAnimCtrl4Function[0] == 0)
            {// Linear Control #4 Function

                m_dControl4 += (m_dAnimCtrl4[0]-dControl4BU) / (m_iTriggerFrame[0]-1);

            }
            else if(m_iAnimCtrl4Function[0] == 1)
            {// Positive Quadratic Control #4 Function

                double a = (m_dAnimCtrl4[0]-dControl4BU) / pow((double)(m_iTriggerFrame[0]-1),2);

                double counter = (double)(iCurrentFrame-1);

                m_dControl4 = a*counter*counter + dControl4BU;

            }
            else if(m_iAnimCtrl4Function[0] == 2)
            {// Negative Quadratic Control #4 Function

                double b = (m_dAnimCtrl4[0]-dControl4BU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                m_dControl4 = a*counter*counter + b*counter + dControl4BU;

            }

            if(m_iAnimCtrl5Function[0] == 0)
            {// Linear Control #5 Function

                m_dControl5 += (m_dAnimCtrl5[0]-dControl5BU) / (m_iTriggerFrame[0]-1);

            }
            else if(m_iAnimCtrl5Function[0] == 1)
            {// Positive Quadratic Control #5 Function

                double a = (m_dAnimCtrl5[0]-dControl5BU) / pow((double)(m_iTriggerFrame[0]-1),2);

                double counter = (double)(iCurrentFrame-1);

                m_dControl5 = a*counter*counter + dControl5BU;

            }
            else if(m_iAnimCtrl5Function[0] == 2)
            {// Negative Quadratic Control #5 Function

                double b = (m_dAnimCtrl5[0]-dControl5BU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                m_dControl5 = a*counter*counter + b*counter + dControl5BU;

            }

            if(m_iAnimCtrl6Function[0] == 0)
            {// Linear Control #6 Function

                m_dControl6 += (m_dAnimCtrl6[0]-dControl6BU) / (m_iTriggerFrame[0]-1);

            }
            else if(m_iAnimCtrl6Function[0] == 1)
            {// Positive Quadratic Control #6 Function

                double a = (m_dAnimCtrl6[0]-dControl6BU) / pow((double)(m_iTriggerFrame[0]-1),2);

                double counter = (double)(iCurrentFrame-1);

                m_dControl6 = a*counter*counter + dControl6BU;

            }
            else if(m_iAnimCtrl6Function[0] == 2)
            {// Negative Quadratic Control #6 Function

                double b = (m_dAnimCtrl6[0]-dControl6BU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                m_dControl6 = a*counter*counter + b*counter + dControl6BU;

            }


            // Outside Coloring Functions

            if(m_iAnimOutsideRedAddFunction[0] == 0)
            {// Linear Outside Red Adder Function

                dOutRedAdder += (double)((double)(m_iAnimOutsideRedAdder[0] - iOutRedAdderBU) / (double)(m_iTriggerFrame[0]-1));

                m_iOutsideRedColorAdder = (int)(dOutRedAdder+0.5);

            }
            else if(m_iAnimOutsideRedAddFunction[0] == 1)
            {// Positive Quadratic Outside Red Adder Function

                double a = (double)((m_iAnimOutsideRedAdder[0]-iOutRedAdderBU) / pow((double)(m_iTriggerFrame[0]-1),2));

                double counter = (double)(iCurrentFrame-1);

                dOutRedAdder = a*counter*counter + iOutRedAdderBU;

                m_iOutsideRedColorAdder = (int)(dOutRedAdder+0.5);

            }
            else if(m_iAnimOutsideRedAddFunction[0] == 2)
            {// Negative Quadratic Outside Red Adder Function

                double b = (m_iAnimOutsideRedAdder[0]-iOutRedAdderBU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                dOutRedAdder = a*counter*counter + b*counter + iOutRedAdderBU;

                m_iOutsideRedColorAdder = (int)(dOutRedAdder+0.5);

            }

            if(m_iAnimOutsideGreenAddFunction[0] == 0)
            {// Linear Outside Green Adder Function

                dOutGreenAdder += (double)((double)(m_iAnimOutsideGreenAdder[0] - iOutGreenAdderBU) / (double)(m_iTriggerFrame[0]-1));

                m_iOutsideGreenColorAdder = (int)(dOutGreenAdder+0.5);

            }
            else if(m_iAnimOutsideGreenAddFunction[0] == 1)
            {// Positive Quadratic Outside Green Adder Function

                double a = (double)((m_iAnimOutsideGreenAdder[0]-iOutGreenAdderBU) / pow((double)(m_iTriggerFrame[0]-1),2));

                double counter = (double)(iCurrentFrame-1);

                dOutGreenAdder = a*counter*counter + iOutGreenAdderBU;

                m_iOutsideGreenColorAdder = (int)(dOutGreenAdder+0.5);

            }
            else if(m_iAnimOutsideGreenAddFunction[0] == 2)
            {// Negative Quadratic Outside Green Adder Function

                double b = (m_iAnimOutsideGreenAdder[0]-iOutGreenAdderBU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                dOutGreenAdder = a*counter*counter + b*counter + iOutGreenAdderBU;

                m_iOutsideGreenColorAdder = (int)(dOutGreenAdder+0.5);

            }

            if(m_iAnimOutsideBlueAddFunction[0] == 0)
            {// Linear Outside Blue Adder Function

                dOutBlueAdder += (double)((double)(m_iAnimOutsideBlueAdder[0] - iOutBlueAdderBU) / (double)(m_iTriggerFrame[0]-1));

                m_iOutsideBlueColorAdder = (int)(dOutBlueAdder+0.5);

            }
            else if(m_iAnimOutsideBlueAddFunction[0] == 1)
            {// Positive Quadratic Outside Blue Adder Function

                double a = (double)((m_iAnimOutsideBlueAdder[0]-iOutBlueAdderBU) / pow((double)(m_iTriggerFrame[0]-1),2));

                double counter = (double)(iCurrentFrame-1);

                dOutBlueAdder = a*counter*counter + iOutBlueAdderBU;

                m_iOutsideBlueColorAdder = (int)(dOutBlueAdder+0.5);

            }
            else if(m_iAnimOutsideBlueAddFunction[0] == 2)
            {// Negative Quadratic Outside Blue Adder Function

                double b = (m_iAnimOutsideBlueAdder[0]-iOutBlueAdderBU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                dOutBlueAdder = a*counter*counter + b*counter + iOutBlueAdderBU;

                m_iOutsideBlueColorAdder = (int)(dOutBlueAdder+0.5);

            }


            if(m_iAnimOutsideRedMultFunction[0] == 0)
            {// Linear Outside Red Multiplier Function

                m_dOutsideRedColorMultiplier += (m_dAnimOutsideRedMultiplier[0]-dOutRedMultBU) / (double)(m_iTriggerFrame[0]-1);

            }
            else if(m_iAnimOutsideRedMultFunction[0] == 1)
            {// Positive Quadratic Outside Red Multiplier Function

                double a = (m_dAnimOutsideRedMultiplier[0]-dOutRedMultBU) / pow((double)(m_iTriggerFrame[0]-1),2);

                double counter = (double)(iCurrentFrame-1);

                m_dOutsideRedColorMultiplier = a*counter*counter + dOutRedMultBU;

            }
            else if(m_iAnimOutsideRedMultFunction[0] == 2)
            {// Negative Quadratic Outside Red Multiplier Function

                double b = (m_dAnimOutsideRedMultiplier[0]-dOutRedMultBU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                m_dOutsideRedColorMultiplier = a*counter*counter + b*counter + dOutRedMultBU;

            }

            if(m_iAnimOutsideGreenMultFunction[0] == 0)
            {// Linear Outside Green Multiplier Function

                m_dOutsideGreenColorMultiplier += (m_dAnimOutsideGreenMultiplier[0]-dOutGreenMultBU) / (double)(m_iTriggerFrame[0]-1);

            }
            else if(m_iAnimOutsideGreenMultFunction[0] == 1)
            {// Positive Quadratic Outside Green Multiplier Function

                double a = (m_dAnimOutsideGreenMultiplier[0]-dOutGreenMultBU) / pow((double)(m_iTriggerFrame[0]-1),2);

                double counter = (double)(iCurrentFrame-1);

                m_dOutsideGreenColorMultiplier = a*counter*counter + dOutGreenMultBU;

            }
            else if(m_iAnimOutsideGreenMultFunction[0] == 2)
            {// Negative Quadratic Outside Green Multiplier Function

                double b = (m_dAnimOutsideGreenMultiplier[0]-dOutGreenMultBU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                m_dOutsideGreenColorMultiplier = a*counter*counter + b*counter + dOutGreenMultBU;

            }

            if(m_iAnimOutsideBlueMultFunction[0] == 0)
            {// Linear Outside Blue Multiplier Function

                m_dOutsideBlueColorMultiplier += (m_dAnimOutsideBlueMultiplier[0]-dOutBlueMultBU) / (double)(m_iTriggerFrame[0]-1);

            }
            else if(m_iAnimOutsideBlueMultFunction[0] == 1)
            {// Positive Quadratic Outside Blue Multiplier Function

                double a = (m_dAnimOutsideBlueMultiplier[0]-dOutBlueMultBU) / pow((double)(m_iTriggerFrame[0]-1),2);

                double counter = (double)(iCurrentFrame-1);

                m_dOutsideBlueColorMultiplier = a*counter*counter + dOutBlueMultBU;

            }
            else if(m_iAnimOutsideBlueMultFunction[0] == 2)
            {// Negative Quadratic Outside Blue Multiplier Function

                double b = (m_dAnimOutsideBlueMultiplier[0]-dOutBlueMultBU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                m_dOutsideBlueColorMultiplier = a*counter*counter + b*counter + dOutBlueMultBU;

            }


            // Inside Coloring Functions

            if(m_iAnimInsideRedAddFunction[0] == 0)
            {// Linear Inside Red Adder Function

                dInRedAdder += (double)((double)(m_iAnimInsideRedAdder[0] - iInRedAdderBU) / (double)(m_iTriggerFrame[0]-1));

                m_iInsideRedColorAdder = (int)(dInRedAdder+0.5);

            }
            else if(m_iAnimInsideRedAddFunction[0] == 1)
            {// Positive Quadratic Inside Red Adder Function

                double a = (double)((m_iAnimInsideRedAdder[0]-iInRedAdderBU) / pow((double)(m_iTriggerFrame[0]-1),2));

                double counter = (double)(iCurrentFrame-1);

                dInRedAdder = a*counter*counter + iInRedAdderBU;

                m_iInsideRedColorAdder = (int)(dInRedAdder+0.5);

            }
            else if(m_iAnimInsideRedAddFunction[0] == 2)
            {// Negative Quadratic Inside Red Adder Function

                double b = (m_iAnimInsideRedAdder[0]-iInRedAdderBU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                dInRedAdder = a*counter*counter + b*counter + iInRedAdderBU;

                m_iInsideRedColorAdder = (int)(dInRedAdder+0.5);

            }

            if(m_iAnimInsideGreenAddFunction[0] == 0)
            {// Linear Inside Green Adder Function

                dInGreenAdder += (double)((double)(m_iAnimInsideGreenAdder[0] - iInGreenAdderBU) / (double)(m_iTriggerFrame[0]-1));

                m_iInsideGreenColorAdder = (int)(dInGreenAdder+0.5);

            }
            else if(m_iAnimInsideGreenAddFunction[0] == 1)
            {// Positive Quadratic Inside Green Adder Function

                double a = (double)((m_iAnimInsideGreenAdder[0]-iInGreenAdderBU) / pow((double)(m_iTriggerFrame[0]-1),2));

                double counter = (double)(iCurrentFrame-1);

                dInGreenAdder = a*counter*counter + iInGreenAdderBU;

                m_iInsideGreenColorAdder = (int)(dInGreenAdder+0.5);

            }
            else if(m_iAnimInsideGreenAddFunction[0] == 2)
            {// Negative Quadratic Inside Green Adder Function

                double b = (m_iAnimInsideGreenAdder[0]-iInGreenAdderBU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                dInGreenAdder = a*counter*counter + b*counter + iInGreenAdderBU;

                m_iInsideGreenColorAdder = (int)(dInGreenAdder+0.5);

            }

            if(m_iAnimInsideBlueAddFunction[0] == 0)
            {// Linear Inside Blue Adder Function

                dInBlueAdder += (double)((double)(m_iAnimInsideBlueAdder[0] - iInBlueAdderBU) / (double)(m_iTriggerFrame[0]-1));

                m_iInsideBlueColorAdder = (int)(dInBlueAdder+0.5);

            }
            else if(m_iAnimInsideBlueAddFunction[0] == 1)
            {// Positive Quadratic Inside Blue Adder Function

                double a = (double)((m_iAnimInsideBlueAdder[0]-iInBlueAdderBU) / pow((double)(m_iTriggerFrame[0]-1),2));

                double counter = (double)(iCurrentFrame-1);

                dInBlueAdder = a*counter*counter + iInBlueAdderBU;

                m_iInsideBlueColorAdder = (int)(dInBlueAdder+0.5);

            }
            else if(m_iAnimInsideBlueAddFunction[0] == 2)
            {// Negative Quadratic Inside Blue Adder Function

                double b = (m_iAnimInsideBlueAdder[0]-iInBlueAdderBU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                dInBlueAdder = a*counter*counter + b*counter + iInBlueAdderBU;

                m_iInsideBlueColorAdder = (int)(dInBlueAdder+0.5);

            }


            if(m_iAnimInsideRedMultFunction[0] == 0)
            {// Linear Inside Red Multiplier Function

                m_dInsideRedColorMultiplier += (m_dAnimInsideRedMultiplier[0]-dInRedMultBU) / (double)(m_iTriggerFrame[0]-1);

            }
            else if(m_iAnimInsideRedMultFunction[0] == 1)
            {// Positive Quadratic Inside Red Multiplier Function

                double a = (m_dAnimInsideRedMultiplier[0]-dInRedMultBU) / pow((double)(m_iTriggerFrame[0]-1),2);

                double counter = (double)(iCurrentFrame-1);

                m_dInsideRedColorMultiplier = a*counter*counter + dInRedMultBU;

            }
            else if(m_iAnimInsideRedMultFunction[0] == 2)
            {// Negative Quadratic Inside Red Multiplier Function

                double b = (m_dAnimInsideRedMultiplier[0]-dInRedMultBU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                m_dInsideRedColorMultiplier = a*counter*counter + b*counter + dInRedMultBU;

            }

            if(m_iAnimInsideGreenMultFunction[0] == 0)
            {// Linear Inside Green Multiplier Function

                m_dInsideGreenColorMultiplier += (m_dAnimInsideGreenMultiplier[0]-dInGreenMultBU) / (double)(m_iTriggerFrame[0]-1);

            }
            else if(m_iAnimInsideGreenMultFunction[0] == 1)
            {// Positive Quadratic Inside Green Multiplier Function

                double a = (m_dAnimInsideGreenMultiplier[0]-dInGreenMultBU) / pow((double)(m_iTriggerFrame[0]-1),2);

                double counter = (double)(iCurrentFrame-1);

                m_dInsideGreenColorMultiplier = a*counter*counter + dInGreenMultBU;

            }
            else if(m_iAnimInsideGreenMultFunction[0] == 2)
            {// Negative Quadratic Inside Green Multiplier Function

                double b = (m_dAnimInsideGreenMultiplier[0]-dInGreenMultBU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                m_dInsideGreenColorMultiplier = a*counter*counter + b*counter + dInGreenMultBU;

            }

            if(m_iAnimInsideBlueMultFunction[0] == 0)
            {// Linear Inside Blue Multiplier Function

                m_dInsideBlueColorMultiplier += (m_dAnimInsideBlueMultiplier[0]-dInBlueMultBU) / (double)(m_iTriggerFrame[0]-1);

            }
            else if(m_iAnimInsideBlueMultFunction[0] == 1)
            {// Positive Quadratic Inside Blue Multiplier Function

                double a = (m_dAnimInsideBlueMultiplier[0]-dInBlueMultBU) / pow((double)(m_iTriggerFrame[0]-1),2);

                double counter = (double)(iCurrentFrame-1);

                m_dInsideBlueColorMultiplier = a*counter*counter + dInBlueMultBU;

            }
            else if(m_iAnimInsideBlueMultFunction[0] == 2)
            {// Negative Quadratic Inside Blue Multiplier Function

                double b = (m_dAnimInsideBlueMultiplier[0]-dInBlueMultBU) / ((m_iTriggerFrame[0]-1)/2.0);

                double a = -b/((m_iTriggerFrame[0]-1)*2.0);

                double counter = (double)(iCurrentFrame-1);

                m_dInsideBlueColorMultiplier = a*counter*counter + b*counter + dInBlueMultBU;

            }

        }


        else if( (iCurrentFrame > m_iTriggerFrame[0])
                && (iCurrentFrame <= m_iTriggerFrame[1]) )
        {
            triggerPoint = 1;
        }
        else if( (iCurrentFrame > m_iTriggerFrame[1])
                 && (iCurrentFrame <= m_iTriggerFrame[2]) )
        {
            triggerPoint = 2;
        }
        else if( (iCurrentFrame > m_iTriggerFrame[2])
                 && (iCurrentFrame <= m_iTriggerFrame[3]) )
        {
            triggerPoint = 3;
        }
        else if( (iCurrentFrame > m_iTriggerFrame[3])
                 && (iCurrentFrame <= m_iTriggerFrame[4]) )
        {
            triggerPoint = 4;
        }
        else if( (iCurrentFrame > m_iTriggerFrame[4])
                 && (iCurrentFrame <= m_iTriggerFrame[5]) )
        {
            triggerPoint = 5;
        }
        else if( (iCurrentFrame > m_iTriggerFrame[5])
                 && (iCurrentFrame <= m_iTriggerFrame[6]) )
        {
            triggerPoint = 6;
        }
        else if( (iCurrentFrame > m_iTriggerFrame[6])
                 && (iCurrentFrame <= m_iTriggerFrame[7]) )
        {
            triggerPoint = 7;
        }


        if( (iCurrentFrame > m_iTriggerFrame[triggerPoint-1])
            && (iCurrentFrame <= m_iTriggerFrame[triggerPoint]) )
        {// Trigger Point Functions

            if(m_iTranslationFunction[triggerPoint] == 0)
            {// Linear Translation Function

                m_dCenterX += (m_dEndCenterX[triggerPoint] - m_dEndCenterX[triggerPoint-1])
                              / (double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]);

                m_dCenterY += (m_dEndCenterY[triggerPoint] - m_dEndCenterY[triggerPoint-1])
                              / (double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]);

            }
            else if(m_iTranslationFunction[triggerPoint] == 1)
            {// Positive Quadratic Translation Function

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                double aX = (m_dEndCenterX[triggerPoint] - m_dEndCenterX[triggerPoint-1])
                            / pow((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2.0);

                m_dCenterX = aX*counter*counter + m_dEndCenterX[triggerPoint-1];

                double aY = (m_dEndCenterY[triggerPoint] - m_dEndCenterY[triggerPoint-1])
                            / pow((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2.0);

                m_dCenterY = aY*counter*counter + m_dEndCenterY[triggerPoint-1];

            }
            else if(m_iTranslationFunction[triggerPoint] == 2)
            {// Negative Quadratic Translation Function

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                double bX = (m_dEndCenterX[triggerPoint] - m_dEndCenterX[triggerPoint-1])
                            / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double aX = -bX/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                m_dCenterX = aX*counter*counter + bX*counter + m_dEndCenterX[triggerPoint-1];

                double bY = (m_dEndCenterY[triggerPoint] - m_dEndCenterY[triggerPoint-1])
                            / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double aY = -bY/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                m_dCenterY = aY*counter*counter + bY*counter + m_dEndCenterY[triggerPoint-1];

            }

            if(m_iRotationFunction[triggerPoint] == 0)
            {// Linear Rotation Function

               m_dRotation += (m_dEndRotation[triggerPoint] - m_dEndRotation[triggerPoint-1])
                              / (double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]);

            }
            else if(m_iRotationFunction[triggerPoint] == 1)
            {// Positive Quadratic Rotation Function

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                double a = (m_dEndRotation[triggerPoint] - m_dEndRotation[triggerPoint-1])
                           / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2);

                m_dRotation = a*counter*counter + m_dEndRotation[triggerPoint-1];

            }
            else if(m_iRotationFunction[triggerPoint] == 2)
            {// Negative Quadratic Rotation Function

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                double b = (m_dEndRotation[triggerPoint] - m_dEndRotation[triggerPoint-1])
                           / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                m_dRotation = a*counter*counter + b*counter + m_dEndRotation[triggerPoint-1];

            }

            if(m_iZoomRatioFunction[triggerPoint] == 0)
            {// Linear Zoom Ratio Function

                m_dZoomRatio += (m_dEndZoomRatio[triggerPoint] - m_dEndZoomRatio[triggerPoint-1])
                                / (double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]);

            }
            else if(m_iZoomRatioFunction[triggerPoint] == 1)
            {// Summation Zoom Ratio Function

                m_dZoomRatio += (iCurrentFrame - m_iTriggerFrame[triggerPoint-1])
                                * (m_dEndZoomRatio[triggerPoint] - m_dEndZoomRatio[triggerPoint-1])
                                / (double)((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])
                                * (m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]+1)/2.0);

            }
            else if(m_iZoomRatioFunction[triggerPoint] == 2)
            {// Negative Quadratic Zoom Ratio Function

                double b = (m_dEndZoomRatio[triggerPoint] - m_dEndZoomRatio[triggerPoint-1])
                            / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                m_dZoomRatio = a*counter*counter + b*counter + m_dEndZoomRatio[triggerPoint-1];

            }

            if(m_iIterationFunction[triggerPoint] == 0)
            {// Linear Iteration Function

                dIteration += (double)((double)(m_iEndIteration[triggerPoint] - m_iEndIteration[triggerPoint-1])
                              / (double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]));

                m_iIteration = (long long)(dIteration+0.5);

            }
            else if(m_iIterationFunction[triggerPoint] == 1)
            {// Positive Quadratic Iteration Function

                double a = (double)((m_iEndIteration[triggerPoint] - m_iEndIteration[triggerPoint-1])
                           / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2));

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                dIteration = a*counter*counter + m_iEndIteration[triggerPoint-1];

                m_iIteration = (long long)(dIteration+0.5);

            }
            else if(m_iIterationFunction[triggerPoint] == 2)
            {// Negative Quadratic Iteration Function

                double b = (double)((m_iEndIteration[triggerPoint] - m_iEndIteration[triggerPoint-1])
                           / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0));

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                dIteration = a*counter*counter + b*counter + m_iEndIteration[triggerPoint-1];

                m_iIteration = (long long)(dIteration+0.5);

            }


            // Formula Control Functions

            if(m_iAnimCtrl1Function[triggerPoint] == 0)
            {// Linear Control #1 Function

                m_dControl1 += (m_dAnimCtrl1[triggerPoint] - m_dAnimCtrl1[triggerPoint-1])
                               / (m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]);

            }
            else if(m_iAnimCtrl1Function[triggerPoint] == 1)
            {// Positive Quadratic Control #1 Function

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                double a = (m_dAnimCtrl1[triggerPoint] - m_dAnimCtrl1[triggerPoint-1])
                            / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2);

                m_dControl1 = a*counter*counter + m_dAnimCtrl1[triggerPoint-1];

            }
            else if(m_iAnimCtrl1Function[triggerPoint] == 2)
            {// Negative Quadratic Control #1 Function

                double b = (m_dAnimCtrl1[triggerPoint] - m_dAnimCtrl1[triggerPoint-1])
                           / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                m_dControl1 = a*counter*counter + b*counter + m_dAnimCtrl1[triggerPoint-1];

            }

            if(m_iAnimCtrl2Function[triggerPoint] == 0)
            {// Linear Control #2 Function

                m_dControl2 += (m_dAnimCtrl2[triggerPoint] - m_dAnimCtrl2[triggerPoint-1])
                               / (m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]);

            }
            else if(m_iAnimCtrl2Function[triggerPoint] == 1)
            {// Positive Quadratic Control #2 Function

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                double a = (m_dAnimCtrl2[triggerPoint] - m_dAnimCtrl2[triggerPoint-1])
                            / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2);

                m_dControl2 = a*counter*counter + m_dAnimCtrl2[triggerPoint-1];

            }
            else if(m_iAnimCtrl2Function[triggerPoint] == 2)
            {// Negative Quadratic Control #2 Function

                double b = (m_dAnimCtrl2[triggerPoint] - m_dAnimCtrl2[triggerPoint-1])
                           / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                m_dControl2 = a*counter*counter + b*counter + m_dAnimCtrl2[triggerPoint-1];

            }

            if(m_iAnimCtrl3Function[triggerPoint] == 0)
            {// Linear Control #3 Function

                m_dControl3 += (m_dAnimCtrl3[triggerPoint] - m_dAnimCtrl3[triggerPoint-1])
                               / (m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]);

            }
            else if(m_iAnimCtrl3Function[triggerPoint] == 1)
            {// Positive Quadratic Control #3 Function

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                double a = (m_dAnimCtrl3[triggerPoint] - m_dAnimCtrl3[triggerPoint-1])
                            / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2);

                m_dControl3 = a*counter*counter + m_dAnimCtrl3[triggerPoint-1];

            }
            else if(m_iAnimCtrl3Function[triggerPoint] == 2)
            {// Negative Quadratic Control #3 Function

                double b = (m_dAnimCtrl3[triggerPoint] - m_dAnimCtrl3[triggerPoint-1])
                           / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                m_dControl3 = a*counter*counter + b*counter + m_dAnimCtrl3[triggerPoint-1];

            }

            if(m_iAnimCtrl4Function[triggerPoint] == 0)
            {// Linear Control #4 Function

                m_dControl4 += (m_dAnimCtrl4[triggerPoint] - m_dAnimCtrl4[triggerPoint-1])
                               / (m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]);

            }
            else if(m_iAnimCtrl4Function[triggerPoint] == 1)
            {// Positive Quadratic Control #4 Function

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                double a = (m_dAnimCtrl4[triggerPoint] - m_dAnimCtrl4[triggerPoint-1])
                            / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2);

                m_dControl4 = a*counter*counter + m_dAnimCtrl4[triggerPoint-1];

            }
            else if(m_iAnimCtrl4Function[triggerPoint] == 2)
            {// Negative Quadratic Control #4 Function

                double b = (m_dAnimCtrl4[triggerPoint] - m_dAnimCtrl4[triggerPoint-1])
                           / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                m_dControl4 = a*counter*counter + b*counter + m_dAnimCtrl4[triggerPoint-1];

            }

            if(m_iAnimCtrl5Function[triggerPoint] == 0)
            {// Linear Control #5 Function

                m_dControl5 += (m_dAnimCtrl5[triggerPoint] - m_dAnimCtrl5[triggerPoint-1])
                               / (m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]);

            }
            else if(m_iAnimCtrl5Function[triggerPoint] == 1)
            {// Positive Quadratic Control #5 Function

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                double a = (m_dAnimCtrl5[triggerPoint] - m_dAnimCtrl5[triggerPoint-1])
                            / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2);

                m_dControl5 = a*counter*counter + m_dAnimCtrl5[triggerPoint-1];

            }
            else if(m_iAnimCtrl5Function[triggerPoint] == 2)
            {// Negative Quadratic Control #5 Function

                double b = (m_dAnimCtrl5[triggerPoint] - m_dAnimCtrl5[triggerPoint-1])
                           / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                m_dControl5 = a*counter*counter + b*counter + m_dAnimCtrl5[triggerPoint-1];

            }

            if(m_iAnimCtrl6Function[triggerPoint] == 0)
            {// Linear Control #6 Function

                m_dControl6 += (m_dAnimCtrl6[triggerPoint] - m_dAnimCtrl6[triggerPoint-1])
                               / (m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]);

            }
            else if(m_iAnimCtrl6Function[triggerPoint] == 1)
            {// Positive Quadratic Control #6 Function

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                double a = (m_dAnimCtrl6[triggerPoint] - m_dAnimCtrl6[triggerPoint-1])
                            / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2);

                m_dControl6 = a*counter*counter + m_dAnimCtrl6[triggerPoint-1];

            }
            else if(m_iAnimCtrl6Function[triggerPoint] == 2)
            {// Negative Quadratic Control #6 Function

                double b = (m_dAnimCtrl6[triggerPoint] - m_dAnimCtrl6[triggerPoint-1])
                           / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                m_dControl6 = a*counter*counter + b*counter + m_dAnimCtrl6[triggerPoint-1];

            }


            // Outside Coloring Functions

            if(m_iAnimOutsideRedAddFunction[triggerPoint] == 0)
            {// Linear Outside Red Adder Function

                dOutRedAdder += (double)((double)(m_iAnimOutsideRedAdder[triggerPoint] - m_iAnimOutsideRedAdder[triggerPoint-1])
                                / (m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]));

                m_iOutsideRedColorAdder = (int)(dOutRedAdder+0.5);

            }
            else if(m_iAnimOutsideRedAddFunction[triggerPoint] == 1)
            {// Positive Quadratic Outside Red Adder Function

                double a = (double)((m_iAnimOutsideRedAdder[triggerPoint] - m_iAnimOutsideRedAdder[triggerPoint-1])
                            / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2));

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                dOutRedAdder = a*counter*counter + m_iAnimOutsideRedAdder[triggerPoint-1];

                m_iOutsideRedColorAdder = (int)(dOutRedAdder+0.5);

            }
            else if(m_iAnimOutsideRedAddFunction[triggerPoint] == 2)
            {// Negative Quadratic Outside Red Adder Function

                double b = (m_iAnimOutsideRedAdder[triggerPoint] - m_iAnimOutsideRedAdder[triggerPoint-1])
                            / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame-m_iTriggerFrame[triggerPoint-1]);

                dOutRedAdder = a*counter*counter + b*counter + m_iAnimOutsideRedAdder[triggerPoint-1];

                m_iOutsideRedColorAdder = (int)(dOutRedAdder+0.5);

            }

            if(m_iAnimOutsideGreenAddFunction[triggerPoint] == 0)
            {// Linear Outside Green Adder Function

                dOutGreenAdder += (double)((double)(m_iAnimOutsideGreenAdder[triggerPoint] - m_iAnimOutsideGreenAdder[triggerPoint-1])
                                / (m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]));

                m_iOutsideGreenColorAdder = (int)(dOutGreenAdder+0.5);

            }
            else if(m_iAnimOutsideGreenAddFunction[triggerPoint] == 1)
            {// Positive Quadratic Outside Green Adder Function

                double a = (double)((m_iAnimOutsideGreenAdder[triggerPoint] - m_iAnimOutsideGreenAdder[triggerPoint-1])
                            / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2));

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                dOutGreenAdder = a*counter*counter + m_iAnimOutsideGreenAdder[triggerPoint-1];

                m_iOutsideGreenColorAdder = (int)(dOutGreenAdder+0.5);

            }
            else if(m_iAnimOutsideGreenAddFunction[triggerPoint] == 2)
            {// Negative Quadratic Outside Green Adder Function

                double b = (m_iAnimOutsideGreenAdder[triggerPoint] - m_iAnimOutsideGreenAdder[triggerPoint-1])
                            / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame-m_iTriggerFrame[triggerPoint-1]);

                dOutGreenAdder = a*counter*counter + b*counter + m_iAnimOutsideGreenAdder[triggerPoint-1];

                m_iOutsideGreenColorAdder = (int)(dOutGreenAdder+0.5);

            }

            if(m_iAnimOutsideBlueAddFunction[triggerPoint] == 0)
            {// Linear Outside Blue Adder Function

                dOutBlueAdder += (double)((double)(m_iAnimOutsideBlueAdder[triggerPoint] - m_iAnimOutsideBlueAdder[triggerPoint-1])
                                / (m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]));

                m_iOutsideBlueColorAdder = (int)(dOutBlueAdder+0.5);

            }
            else if(m_iAnimOutsideBlueAddFunction[triggerPoint] == 1)
            {// Positive Quadratic Outside Blue Adder Function

                double a = (double)((m_iAnimOutsideBlueAdder[triggerPoint] - m_iAnimOutsideBlueAdder[triggerPoint-1])
                            / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2));

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                dOutBlueAdder = a*counter*counter + m_iAnimOutsideBlueAdder[triggerPoint-1];

                m_iOutsideBlueColorAdder = (int)(dOutBlueAdder+0.5);

            }
            else if(m_iAnimOutsideBlueAddFunction[triggerPoint] == 2)
            {// Negative Quadratic Outside Blue Adder Function

                double b = (m_iAnimOutsideBlueAdder[triggerPoint] - m_iAnimOutsideBlueAdder[triggerPoint-1])
                            / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame-m_iTriggerFrame[triggerPoint-1]);

                dOutBlueAdder = a*counter*counter + b*counter + m_iAnimOutsideBlueAdder[triggerPoint-1];

                m_iOutsideBlueColorAdder = (int)(dOutBlueAdder+0.5);

            }

            if(m_iAnimOutsideRedMultFunction[triggerPoint] == 0)
            {// Linear Outside Red Multiplier Function

                m_dOutsideRedColorMultiplier += (m_dAnimOutsideRedMultiplier[triggerPoint] - m_dAnimOutsideRedMultiplier[triggerPoint-1])
                                                / (double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]);

            }
            else if(m_iAnimOutsideRedMultFunction[triggerPoint] == 1)
            {// Positive Quadratic Outside Red Multiplier Function

                double a = (m_dAnimOutsideRedMultiplier[triggerPoint] - m_dAnimOutsideRedMultiplier[triggerPoint-1])
                           / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                m_dOutsideRedColorMultiplier = a*counter*counter + m_dAnimOutsideRedMultiplier[triggerPoint-1];

            }
            else if(m_iAnimOutsideRedMultFunction[triggerPoint] == 2)
            {// Negative Quadratic Outside Red Multiplier Function

                double b = (m_dAnimOutsideRedMultiplier[triggerPoint] - m_dAnimOutsideRedMultiplier[triggerPoint-1])
                           / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                m_dOutsideRedColorMultiplier = a*counter*counter + b*counter + m_dAnimOutsideRedMultiplier[triggerPoint-1];

            }

            if(m_iAnimOutsideGreenMultFunction[triggerPoint] == 0)
            {// Linear Outside Green Multiplier Function

                m_dOutsideGreenColorMultiplier += (m_dAnimOutsideGreenMultiplier[triggerPoint] - m_dAnimOutsideGreenMultiplier[triggerPoint-1])
                                                / (double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]);

            }
            else if(m_iAnimOutsideGreenMultFunction[triggerPoint] == 1)
            {// Positive Quadratic Outside Green Multiplier Function

                double a = (m_dAnimOutsideGreenMultiplier[triggerPoint] - m_dAnimOutsideGreenMultiplier[triggerPoint-1])
                           / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                m_dOutsideGreenColorMultiplier = a*counter*counter + m_dAnimOutsideGreenMultiplier[triggerPoint-1];

            }
            else if(m_iAnimOutsideGreenMultFunction[triggerPoint] == 2)
            {// Negative Quadratic Outside Green Multiplier Function

                double b = (m_dAnimOutsideGreenMultiplier[triggerPoint] - m_dAnimOutsideGreenMultiplier[triggerPoint-1])
                           / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                m_dOutsideGreenColorMultiplier = a*counter*counter + b*counter + m_dAnimOutsideGreenMultiplier[triggerPoint-1];

            }

            if(m_iAnimOutsideBlueMultFunction[triggerPoint] == 0)
            {// Linear Outside Blue Multiplier Function

                m_dOutsideBlueColorMultiplier += (m_dAnimOutsideBlueMultiplier[triggerPoint] - m_dAnimOutsideBlueMultiplier[triggerPoint-1])
                                                / (double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]);

            }
            else if(m_iAnimOutsideBlueMultFunction[triggerPoint] == 1)
            {// Positive Quadratic Outside Blue Multiplier Function

                double a = (m_dAnimOutsideBlueMultiplier[triggerPoint] - m_dAnimOutsideBlueMultiplier[triggerPoint-1])
                           / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                m_dOutsideBlueColorMultiplier = a*counter*counter + m_dAnimOutsideBlueMultiplier[triggerPoint-1];

            }
            else if(m_iAnimOutsideBlueMultFunction[triggerPoint] == 2)
            {// Negative Quadratic Outside Blue Multiplier Function

                double b = (m_dAnimOutsideBlueMultiplier[triggerPoint] - m_dAnimOutsideBlueMultiplier[triggerPoint-1])
                           / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                m_dOutsideBlueColorMultiplier = a*counter*counter + b*counter + m_dAnimOutsideBlueMultiplier[triggerPoint-1];

            }


            // Inside Coloring Functions

            if(m_iAnimInsideRedAddFunction[triggerPoint] == 0)
            {// Linear Inside Red Adder Function

                dInRedAdder += (double)((double)(m_iAnimInsideRedAdder[triggerPoint] - m_iAnimInsideRedAdder[triggerPoint-1])
                               / (m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]));

                m_iInsideRedColorAdder = (int)(dInRedAdder+0.5);

            }
            else if(m_iAnimInsideRedAddFunction[triggerPoint] == 1)
            {// Positive Quadratic Inside Red Adder Function

                double a = (double)((m_iAnimInsideRedAdder[triggerPoint] - m_iAnimInsideRedAdder[triggerPoint-1])
                           / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2));

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                dInRedAdder = a*counter*counter + m_iAnimInsideRedAdder[triggerPoint-1];

                m_iInsideRedColorAdder = (int)(dInRedAdder+0.5);

            }
            else if(m_iAnimInsideRedAddFunction[triggerPoint] == 2)
            {// Negative Quadratic Inside Red Adder Function

                double b = (m_iAnimInsideRedAdder[triggerPoint] - m_iAnimInsideRedAdder[triggerPoint-1])
                           / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                dInRedAdder = a*counter*counter + b*counter + m_iAnimInsideRedAdder[triggerPoint-1];

                m_iInsideRedColorAdder = (int)(dInRedAdder+0.5);

            }

            if(m_iAnimInsideGreenAddFunction[triggerPoint] == 0)
            {// Linear Inside Green Adder Function

                dInGreenAdder += (double)((double)(m_iAnimInsideGreenAdder[triggerPoint] - m_iAnimInsideGreenAdder[triggerPoint-1])
                               / (m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]));

                m_iInsideGreenColorAdder = (int)(dInGreenAdder+0.5);

            }
            else if(m_iAnimInsideGreenAddFunction[triggerPoint] == 1)
            {// Positive Quadratic Inside Green Adder Function

                double a = (double)((m_iAnimInsideGreenAdder[triggerPoint] - m_iAnimInsideGreenAdder[triggerPoint-1])
                           / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2));

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                dInGreenAdder = a*counter*counter + m_iAnimInsideGreenAdder[triggerPoint-1];

                m_iInsideGreenColorAdder = (int)(dInGreenAdder+0.5);

            }
            else if(m_iAnimInsideGreenAddFunction[triggerPoint] == 2)
            {// Negative Quadratic Inside Green Adder Function

                double b = (m_iAnimInsideGreenAdder[triggerPoint] - m_iAnimInsideGreenAdder[triggerPoint-1])
                           / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                dInGreenAdder = a*counter*counter + b*counter + m_iAnimInsideGreenAdder[triggerPoint-1];

                m_iInsideGreenColorAdder = (int)(dInGreenAdder+0.5);

            }

            if(m_iAnimInsideBlueAddFunction[triggerPoint] == 0)
            {// Linear Inside Blue Adder Function

                dInBlueAdder += (double)((double)(m_iAnimInsideBlueAdder[triggerPoint] - m_iAnimInsideBlueAdder[triggerPoint-1])
                               / (m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]));

                m_iInsideBlueColorAdder = (int)(dInBlueAdder+0.5);

            }
            else if(m_iAnimInsideBlueAddFunction[triggerPoint] == 1)
            {// Positive Quadratic Inside Blue Adder Function

                double a = (double)((m_iAnimInsideBlueAdder[triggerPoint] - m_iAnimInsideBlueAdder[triggerPoint-1])
                           / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2));

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                dInBlueAdder = a*counter*counter + m_iAnimInsideBlueAdder[triggerPoint-1];

                m_iInsideBlueColorAdder = (int)(dInBlueAdder+0.5);

            }
            else if(m_iAnimInsideBlueAddFunction[triggerPoint] == 2)
            {// Negative Quadratic Inside Blue Adder Function

                double b = (m_iAnimInsideBlueAdder[triggerPoint] - m_iAnimInsideBlueAdder[triggerPoint-1])
                           / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                dInBlueAdder = a*counter*counter + b*counter + m_iAnimInsideBlueAdder[triggerPoint-1];

                m_iInsideBlueColorAdder = (int)(dInBlueAdder+0.5);

            }

            if(m_iAnimInsideRedMultFunction[triggerPoint] == 0)
            {// Linear Inside Red Multiplier Function

                m_dInsideRedColorMultiplier += (m_dAnimInsideRedMultiplier[triggerPoint] - m_dAnimInsideRedMultiplier[triggerPoint-1])
                                               / (double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]);

            }
            else if(m_iAnimInsideRedMultFunction[triggerPoint] == 1)
            {// Positive Quadratic Inside Red Multiplier Function

                double a = (m_dAnimInsideRedMultiplier[triggerPoint] - m_dAnimInsideRedMultiplier[triggerPoint-1])
                           / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                m_dInsideRedColorMultiplier = a*counter*counter + m_dAnimInsideRedMultiplier[triggerPoint-1];

            }
            else if(m_iAnimInsideRedMultFunction[triggerPoint] == 2)
            {// Negative Quadratic Inside Red Multiplier Function

                double b = (m_dAnimInsideRedMultiplier[triggerPoint] - m_dAnimInsideRedMultiplier[triggerPoint-1])
                           / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                m_dInsideRedColorMultiplier = a*counter*counter + b*counter + m_dAnimInsideRedMultiplier[triggerPoint-1];

            }

            if(m_iAnimInsideGreenMultFunction[triggerPoint] == 0)
            {// Linear Inside Green Multiplier Function

                m_dInsideGreenColorMultiplier += (m_dAnimInsideGreenMultiplier[triggerPoint] - m_dAnimInsideGreenMultiplier[triggerPoint-1])
                                               / (double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]);

            }
            else if(m_iAnimInsideGreenMultFunction[triggerPoint] == 1)
            {// Positive Quadratic Inside Green Multiplier Function

                double a = (m_dAnimInsideGreenMultiplier[triggerPoint] - m_dAnimInsideGreenMultiplier[triggerPoint-1])
                           / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                m_dInsideGreenColorMultiplier = a*counter*counter + m_dAnimInsideGreenMultiplier[triggerPoint-1];

            }
            else if(m_iAnimInsideGreenMultFunction[triggerPoint] == 2)
            {// Negative Quadratic Inside Green Multiplier Function

                double b = (m_dAnimInsideGreenMultiplier[triggerPoint] - m_dAnimInsideGreenMultiplier[triggerPoint-1])
                           / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                m_dInsideGreenColorMultiplier = a*counter*counter + b*counter + m_dAnimInsideGreenMultiplier[triggerPoint-1];

            }

            if(m_iAnimInsideBlueMultFunction[triggerPoint] == 0)
            {// Linear Inside Blue Multiplier Function

                m_dInsideBlueColorMultiplier += (m_dAnimInsideBlueMultiplier[triggerPoint] - m_dAnimInsideBlueMultiplier[triggerPoint-1])
                                               / (double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]);

            }
            else if(m_iAnimInsideBlueMultFunction[triggerPoint] == 1)
            {// Positive Quadratic Inside Blue Multiplier Function

                double a = (m_dAnimInsideBlueMultiplier[triggerPoint] - m_dAnimInsideBlueMultiplier[triggerPoint-1])
                           / pow((double)(m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1]),2);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                m_dInsideBlueColorMultiplier = a*counter*counter + m_dAnimInsideBlueMultiplier[triggerPoint-1];

            }
            else if(m_iAnimInsideBlueMultFunction[triggerPoint] == 2)
            {// Negative Quadratic Inside Blue Multiplier Function

                double b = (m_dAnimInsideBlueMultiplier[triggerPoint] - m_dAnimInsideBlueMultiplier[triggerPoint-1])
                           / ((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])/2.0);

                double a = -b/((m_iTriggerFrame[triggerPoint] - m_iTriggerFrame[triggerPoint-1])*2.0);

                double counter = (double)(iCurrentFrame - m_iTriggerFrame[triggerPoint-1]);

                m_dInsideBlueColorMultiplier = a*counter*counter + b*counter + m_dAnimInsideBlueMultiplier[triggerPoint-1];

            }
        }

        if(m_bSuspendValue == true)
        {
            emit animationProgress(0);
            emit progress(0);
            emit animationTimerUpdate(0.0);
            emit workFinished();
            return;
        }

        emit updateGeometryParameters(m_dCenterX, m_dCenterY, m_dZoomRatio, m_dRotation, m_iIteration, m_dBailout);

        emit updateFormulaParameters(m_dControl1, m_dControl2, m_dControl3, m_dControl4, m_dControl5, m_dControl6);

        emit updateOutsideColoringParameters(m_iOutsideRedColorAdder, m_iOutsideGreenColorAdder, m_iOutsideBlueColorAdder,
                                             m_dOutsideRedColorMultiplier, m_dOutsideGreenColorMultiplier, m_dOutsideBlueColorMultiplier);

        emit updateInsideColoringParameters(m_iInsideRedColorAdder, m_iInsideGreenColorAdder, m_iInsideBlueColorAdder,
                                             m_dInsideRedColorMultiplier, m_dInsideGreenColorMultiplier, m_dInsideBlueColorMultiplier);


        if(m_sFractalType == "Mandelbrot (Quick)")
        {
            renderAnimationMandelbrotQuick(&img);
        }
        else if(m_sFractalType == "Mandelbrot")
        {
            renderAnimationMandelbrot(&img);
        }
        else if(m_sFractalType == "Julia (Quick)")
        {
            renderAnimationJuliaQuick(&img);
        }
        else if(m_sFractalType == "Julia")
        {
            renderAnimationJulia(&img);
        }
        else if(m_sFractalType == "Newton")
        {
            renderAnimationNewton(&img);
        }
        else if(m_sFractalType == "Newton (Root)")
        {
            renderAnimationNewtonRoot(&img);
        }
        else if(m_sFractalType == "Nova")
        {
            renderAnimationNova(&img);
        }
        else if(m_sFractalType == "Nova (Special)")
        {
            renderAnimationNovaSpecial(&img);
        }
        else if(m_sFractalType == "Glynn")
        {
            renderAnimationGlynn(&img);
        }
        else if(m_sFractalType == "Phoenix")
        {
            renderAnimationPhoenix(&img);
        }

        progressValue = (int)(100*iCurrentFrame/m_iTotalFrameNumber);
        emit animationProgress(progressValue);

        emit animationTimerUpdate(timer.elapsed()/1000.0);


        QString fname(QString::number(iCurrentFrame));

        if(iCurrentFrame < 10)
        {
            fname = "00000" + fname + ".jpg";
        }
        else if(iCurrentFrame < 100)
        {
            fname = "0000" + fname + ".jpg";
        }
        else if(iCurrentFrame < 1000)
        {
            fname = "000" + fname + ".jpg";
        }
        else if(iCurrentFrame < 10000)
        {
            fname = "00" + fname + ".jpg";
        }
        else if(iCurrentFrame < 100000)
        {
            fname = "0" + fname + ".jpg";
        }
        else if(iCurrentFrame < 1000000)
        {
            fname = fname + ".jpg";
        }

        img.save(fname, "JPG", -1);
    }

    emit animationProgress(100);

    emit animationTimerUpdate(timer.elapsed()/1000.0);
    timer.invalidate();


    emit updateGeometryParameters(m_dCenterX, m_dCenterY, m_dZoomRatio, m_dRotation, m_iIteration, m_dBailout);

    emit updateFormulaParameters(m_dControl1, m_dControl2, m_dControl3, m_dControl4, m_dControl5, m_dControl6);

    emit updateOutsideColoringParameters(m_iOutsideRedColorAdder, m_iOutsideGreenColorAdder, m_iOutsideBlueColorAdder,
                                         m_dOutsideRedColorMultiplier, m_dOutsideGreenColorMultiplier, m_dOutsideBlueColorMultiplier);

    emit updateInsideColoringParameters(m_iInsideRedColorAdder, m_iInsideGreenColorAdder, m_iInsideBlueColorAdder,
                                         m_dInsideRedColorMultiplier, m_dInsideGreenColorMultiplier, m_dInsideBlueColorMultiplier);

    emit workFinished();
}

void AnimationThread::renderAnimationMandelbrotQuick(QImage *img)
{
    double a, b, d, q, t, tBU = 0.0;
    double x, y;

    int xPix = 0, yPix = 0;
    double bailout = 4.0;

    double xStart, yStart;
    double hyp, hx, hy = 0.0;
    double theta;

    int iOutsideRed = 255, iOutsideGreen = 255, iOutsideBlue = 255;
    int iInsideRed = 0, iInsideGreen = 0, iInsideBlue = 0;

    float progressValue = 0.0;

    int width = 900;
    int height = 600;


    hyp = sqrt(pow(3.0,2) + pow(2.0,2));
    theta = atan(2.0/3.0);

    xStart = m_dCenterX - hyp * cos(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;
    yStart = m_dCenterY + hyp * sin(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;

    for(yPix = 0; yPix < height; yPix++)
    {
        x = xStart - hy*sin(m_dRotation*DEGtoRAD);
        y = yStart - hy*cos(m_dRotation*DEGtoRAD);

        hx = 0.0;

        for(xPix = 0; xPix < width; xPix++)
        {
            a = m_dControl1;
            b = m_dControl2;

            for(t = 0; t < m_iIteration; t++)
            {
                q = a;
                a = (a * a) - (b * b) + x;
                b = 2 * (q * b) + y;

                d = (a * a) + (b * b);

                bailout = (sqrt(d));

                if(bailout > m_dBailout)
                {
                    tBU = t;
                    t = m_iIteration;
                }
            }

            if(m_iColoringAlgorithm == 0) // Bailout Linear
            {
                // Outside Coloring

                iOutsideRed = bailout * m_dOutsideRedColorMultiplier;
                iOutsideGreen = bailout * m_dOutsideGreenColorMultiplier;
                iOutsideBlue = bailout * m_dOutsideBlueColorMultiplier;

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%256;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%256;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%256;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%256);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%256);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%256);}

                // Inside Coloring

                iInsideRed = bailout * m_dInsideRedColorMultiplier;
                iInsideGreen = bailout * m_dInsideGreenColorMultiplier;
                iInsideBlue = bailout * m_dInsideBlueColorMultiplier;

                if(iInsideRed > 255) {iInsideRed = iInsideRed%256;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%256;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%256;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%256);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%256);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%256);}

                // Drawing

                QColor insideColor(qBound(0, iInsideRed + m_iInsideRedColorAdder, 255),
                                   qBound(0, iInsideGreen + m_iInsideGreenColorAdder, 255),
                                   qBound(0, iInsideBlue + m_iInsideBlueColorAdder, 255),
                                   255);

                QColor outsideColor(qBound(0, iOutsideRed + m_iOutsideRedColorAdder, 255),
                                    qBound(0, iOutsideGreen + m_iOutsideGreenColorAdder, 255),
                                    qBound(0, iOutsideBlue + m_iOutsideBlueColorAdder, 255),
                                    255);

                if(bailout < m_dBailout)
                {
                    img->setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img->setPixel(xPix, yPix, outsideColor.rgba());
                }
            }
            else if(m_iColoringAlgorithm == 1) // Smooth Normalization
            {
                if(m_iAlgorithmAttributes == 0) // Regular
                {
                    iOutsideRed = (int)(m_dRedMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                    iOutsideGreen = (int)(m_dGreenMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                    iOutsideBlue = (int)(m_dBlueMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                }
                else if(m_iAlgorithmAttributes == 1) // Reverse
                {
                    if(m_bRedMainColor == true)
                    {
                        iOutsideRed = (int)(100 - m_dRedMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideRed = (int)(m_dRedMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }

                    if(m_bGreenMainColor == true)
                    {
                        iOutsideGreen = (int)(100 - m_dGreenMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideGreen = (int)(m_dGreenMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }

                    if(m_bBlueMainColor == true)
                    {
                        iOutsideBlue = (int)(100 - m_dBlueMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideBlue = (int)(m_dBlueMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                }

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%256;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%256;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%256;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%256);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%256);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%256);}

                // Inside Coloring

                iInsideRed = bailout * m_dInsideRedColorMultiplier;
                iInsideGreen = bailout * m_dInsideGreenColorMultiplier;
                iInsideBlue = bailout * m_dInsideBlueColorMultiplier;

                if(iInsideRed > 255) {iInsideRed = iInsideRed%256;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%256;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%256;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%256);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%256);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%256);}


                // Drawing

                QColor insideColor(qBound(0, iInsideRed + m_iInsideRedColorAdder, 255),
                                   qBound(0, iInsideGreen + m_iInsideGreenColorAdder, 255),
                                   qBound(0, iInsideBlue + m_iInsideBlueColorAdder, 255),
                                   255);

                QColor outsideColor(qBound(0, iOutsideRed + m_iOutsideRedColorAdder, 255),
                                    qBound(0, iOutsideGreen + m_iOutsideGreenColorAdder, 255),
                                    qBound(0, iOutsideBlue + m_iOutsideBlueColorAdder, 255),
                                    255);

                if(bailout < m_dBailout)
                {
                    img->setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img->setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            if(m_bSuspendValue == true)
            {
                emit animationProgress(0);
                emit progress(0);
                emit animationTimerUpdate(0.0);
                return;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;

        progressValue += 1.0/6.0;

        emit progress(progressValue);;

        if(m_bShowProgression == true)
        {
            emit rendered(*img);
        }
    }

    emit rendered(*img);
    emit progress(100);
}

void AnimationThread::renderAnimationMandelbrot(QImage *img)
{
    double a, b, t, tBU = 0.0, x, y, power;

    int xPix, yPix;
    double bailout = 4.0;

    double xStart, yStart;
    double hyp, hx, hy = 0.0;
    double theta;

    std::complex<double> z(0.0,0.0);
    std::complex<double> c(0.0,0.0);

    int iOutsideRed = 255, iOutsideGreen = 255, iOutsideBlue = 255;
    int iInsideRed = 0, iInsideGreen = 0, iInsideBlue = 0;

    float progressValue = 0.0;

    int width = 900;
    int height = 600;

    a = m_dControl1;
    b = m_dControl2;
    power = m_dControl3;


    hyp = sqrt(pow(3.0,2) + pow(2.0,2));
    theta = atan(2.0/3.0);

    xStart = m_dCenterX - hyp * cos(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;
    yStart = m_dCenterY + hyp * sin(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;

    for(yPix = 0; yPix < height; yPix++)
    {
        x = xStart - hy*sin(m_dRotation*DEGtoRAD);
        y = yStart - hy*cos(m_dRotation*DEGtoRAD);

        hx = 0.0;

        for(xPix = 0; xPix < width; xPix++)
        {
            z = complex<double>(a, b);

            for (t = 0; t < m_iIteration; t++)
            {
                c = complex<double>(x,y);

                z = pow(z, power) + c;

                bailout = abs(z);

                if(bailout > m_dBailout)
                {
                    tBU = t;
                    t = m_iIteration;
                }
            }

            if(m_iColoringAlgorithm == 0) // Bailout Linear
            {
                // Outside Coloring

                iOutsideRed = bailout * m_dOutsideRedColorMultiplier;
                iOutsideGreen = bailout * m_dOutsideGreenColorMultiplier;
                iOutsideBlue = bailout * m_dOutsideBlueColorMultiplier;

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%256;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%256;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%256;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%256);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%256);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%256);}

                // Inside Coloring

                iInsideRed = bailout * m_dInsideRedColorMultiplier;
                iInsideGreen = bailout * m_dInsideGreenColorMultiplier;
                iInsideBlue = bailout * m_dInsideBlueColorMultiplier;

                if(iInsideRed > 255) {iInsideRed = iInsideRed%256;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%256;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%256;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%256);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%256);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%256);}

                // Drawing

                QColor insideColor(qBound(0, iInsideRed + m_iInsideRedColorAdder, 255),
                                   qBound(0, iInsideGreen + m_iInsideGreenColorAdder, 255),
                                   qBound(0, iInsideBlue + m_iInsideBlueColorAdder, 255),
                                   255);

                QColor outsideColor(qBound(0, iOutsideRed + m_iOutsideRedColorAdder, 255),
                                    qBound(0, iOutsideGreen + m_iOutsideGreenColorAdder, 255),
                                    qBound(0, iOutsideBlue + m_iOutsideBlueColorAdder, 255),
                                    255);

                if(bailout < m_dBailout)
                {
                    img->setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img->setPixel(xPix, yPix, outsideColor.rgba());
                }
            }
            else if(m_iColoringAlgorithm == 1) // Smooth Normalization
            {
                if(m_iAlgorithmAttributes == 0) // Regular
                {
                    iOutsideRed = (int)(m_dRedMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                    iOutsideGreen = (int)(m_dGreenMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                    iOutsideBlue = (int)(m_dBlueMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                }
                else if(m_iAlgorithmAttributes == 1) // Reverse
                {
                    if(m_bRedMainColor == true)
                    {
                        iOutsideRed = (int)(100 - m_dRedMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideRed = (int)(m_dRedMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }

                    if(m_bGreenMainColor == true)
                    {
                        iOutsideGreen = (int)(100 - m_dGreenMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideGreen = (int)(m_dGreenMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }

                    if(m_bBlueMainColor == true)
                    {
                        iOutsideBlue = (int)(100 - m_dBlueMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideBlue = (int)(m_dBlueMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                }

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%256;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%256;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%256;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%256);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%256);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%256);}

                // Inside Coloring

                iInsideRed = bailout * m_dInsideRedColorMultiplier;
                iInsideGreen = bailout * m_dInsideGreenColorMultiplier;
                iInsideBlue = bailout * m_dInsideBlueColorMultiplier;

                if(iInsideRed > 255) {iInsideRed = iInsideRed%256;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%256;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%256;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%256);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%256);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%256);}


                // Drawing

                QColor insideColor(qBound(0, iInsideRed + m_iInsideRedColorAdder, 255),
                                   qBound(0, iInsideGreen + m_iInsideGreenColorAdder, 255),
                                   qBound(0, iInsideBlue + m_iInsideBlueColorAdder, 255),
                                   255);

                QColor outsideColor(qBound(0, iOutsideRed + m_iOutsideRedColorAdder, 255),
                                    qBound(0, iOutsideGreen + m_iOutsideGreenColorAdder, 255),
                                    qBound(0, iOutsideBlue + m_iOutsideBlueColorAdder, 255),
                                    255);

                if(bailout < m_dBailout)
                {
                    img->setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img->setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            if(m_bSuspendValue == true)
            {
                emit animationProgress(0);
                emit progress(0);
                emit animationTimerUpdate(0.0);
                return;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;

        progressValue += 1.0/6.0;

        emit progress(progressValue);;

        if(m_bShowProgression == true)
        {
            emit rendered(*img);
        }
    }

    emit rendered(*img);
    emit progress(100);
}

void AnimationThread::renderAnimationJuliaQuick(QImage *img)
{
    double a, b, d, e, f, q, t, tBU = 0.0;
    double x, y;

    int xPix = 0, yPix = 0;
    double bailout = 0.0;

    double xStart, yStart;
    double hyp, hx, hy = 0.0;
    double theta;

    int iOutsideRed = 255, iOutsideGreen = 255, iOutsideBlue = 255;
    int iInsideRed = 0, iInsideGreen = 0, iInsideBlue = 0;

    float progressValue = 0.0;

    int width = 900;
    int height = 600;

    a = m_dControl1;
    b = m_dControl2;


    hyp = sqrt(pow(3.0,2) + pow(2.0,2));
    theta = atan(2.0/3.0);

    xStart = m_dCenterX - hyp * cos(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;
    yStart = m_dCenterY + hyp * sin(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;

    for(yPix = 0; yPix < height; yPix++)
    {
        x = xStart - hy*sin(m_dRotation*DEGtoRAD);
        y = yStart - hy*cos(m_dRotation*DEGtoRAD);

        hx = 0.0;

        for(xPix = 0; xPix < width; xPix++)
        {
            e = x;
            f = y;

            for (t = 0; t < m_iIteration; t++)
            {
                q = e;
                e = (e * e) - (f * f) + a;

                f = 2 * (q * f) + b;

                d = (e * e) + (f * f);

                bailout = (sqrt(d));

                if(bailout > m_dBailout)
                {
                    tBU = t;
                    t = m_iIteration;
                }
            }

            if(m_iColoringAlgorithm == 0) // Bailout Linear
            {
                // Outside Coloring

                iOutsideRed = bailout * m_dOutsideRedColorMultiplier;
                iOutsideGreen = bailout * m_dOutsideGreenColorMultiplier;
                iOutsideBlue = bailout * m_dOutsideBlueColorMultiplier;

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%256;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%256;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%256;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%256);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%256);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%256);}

                // Inside Coloring

                iInsideRed = bailout * m_dInsideRedColorMultiplier;
                iInsideGreen = bailout * m_dInsideGreenColorMultiplier;
                iInsideBlue = bailout * m_dInsideBlueColorMultiplier;

                if(iInsideRed > 255) {iInsideRed = iInsideRed%256;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%256;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%256;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%256);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%256);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%256);}

                // Drawing

                QColor insideColor(qBound(0, iInsideRed + m_iInsideRedColorAdder, 255),
                                   qBound(0, iInsideGreen + m_iInsideGreenColorAdder, 255),
                                   qBound(0, iInsideBlue + m_iInsideBlueColorAdder, 255),
                                   255);

                QColor outsideColor(qBound(0, iOutsideRed + m_iOutsideRedColorAdder, 255),
                                    qBound(0, iOutsideGreen + m_iOutsideGreenColorAdder, 255),
                                    qBound(0, iOutsideBlue + m_iOutsideBlueColorAdder, 255),
                                    255);

                if(bailout < m_dBailout)
                {
                    img->setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img->setPixel(xPix, yPix, outsideColor.rgba());
                }
            }
            else if(m_iColoringAlgorithm == 1) // Smooth Normalization
            {
                if(m_iAlgorithmAttributes == 0) // Regular
                {
                    iOutsideRed = (int)(m_dRedMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                    iOutsideGreen = (int)(m_dGreenMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                    iOutsideBlue = (int)(m_dBlueMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                }
                else if(m_iAlgorithmAttributes == 1) // Reverse
                {
                    if(m_bRedMainColor == true)
                    {
                        iOutsideRed = (int)(100 - m_dRedMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideRed = (int)(m_dRedMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }

                    if(m_bGreenMainColor == true)
                    {
                        iOutsideGreen = (int)(100 - m_dGreenMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideGreen = (int)(m_dGreenMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }

                    if(m_bBlueMainColor == true)
                    {
                        iOutsideBlue = (int)(100 - m_dBlueMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideBlue = (int)(m_dBlueMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                }

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%256;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%256;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%256;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%256);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%256);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%256);}

                // Inside Coloring

                iInsideRed = bailout * m_dInsideRedColorMultiplier;
                iInsideGreen = bailout * m_dInsideGreenColorMultiplier;
                iInsideBlue = bailout * m_dInsideBlueColorMultiplier;

                if(iInsideRed > 255) {iInsideRed = iInsideRed%256;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%256;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%256;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%256);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%256);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%256);}


                // Drawing

                QColor insideColor(qBound(0, iInsideRed + m_iInsideRedColorAdder, 255),
                                   qBound(0, iInsideGreen + m_iInsideGreenColorAdder, 255),
                                   qBound(0, iInsideBlue + m_iInsideBlueColorAdder, 255),
                                   255);

                QColor outsideColor(qBound(0, iOutsideRed + m_iOutsideRedColorAdder, 255),
                                    qBound(0, iOutsideGreen + m_iOutsideGreenColorAdder, 255),
                                    qBound(0, iOutsideBlue + m_iOutsideBlueColorAdder, 255),
                                    255);

                if(bailout < m_dBailout)
                {
                    img->setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img->setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            if(m_bSuspendValue == true)
            {
                emit animationProgress(0);
                emit progress(0);
                emit animationTimerUpdate(0.0);
                return;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;

        progressValue += 1.0/6.0;

        emit progress(progressValue);;

        if(m_bShowProgression == true)
        {
            emit rendered(*img);
        }
    }

    emit rendered(*img);
    emit progress(100);
}

void AnimationThread::renderAnimationJulia(QImage *img)
{
    double a, b, t, tBU = 0, x, y, power;

    int xPix, yPix;
    double bailout = 4.0;

    int iOutsideRed = 255, iOutsideGreen = 255, iOutsideBlue = 255;
    int iInsideRed = 0, iInsideGreen = 0, iInsideBlue = 0;

    float progressValue = 0.0;

    int width = 900;
    int height = 600;

    double xStart, yStart;
    double hyp, hx, hy = 0.0;
    double theta;

    complex<double> z;


    a = m_dControl1;
    b = m_dControl2;
    power = m_dControl3;

    complex<double> c(a,b);


    hyp = sqrt(pow(3.0,2) + pow(2.0,2));
    theta = atan(2.0/3.0);

    xStart = m_dCenterX - hyp * cos(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;
    yStart = m_dCenterY + hyp * sin(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;

    for(yPix = 0; yPix < height; yPix++)
    {
        x = xStart - hy*sin(m_dRotation*DEGtoRAD);
        y = yStart - hy*cos(m_dRotation*DEGtoRAD);

        hx = 0.0;

        for(xPix = 0; xPix < width; xPix++)
        {
            z = complex<double>(x,y);

            for (t = 0; t < m_iIteration; t++)
            {
                z = pow(z, power) + c;

                bailout = abs(z);

                if(bailout > m_dBailout)
                {
                    tBU = t;
                    t = m_iIteration;
                }
            }

            if(m_iColoringAlgorithm == 0) // Bailout Linear
            {
                // Outside Coloring

                iOutsideRed = bailout * m_dOutsideRedColorMultiplier;
                iOutsideGreen = bailout * m_dOutsideGreenColorMultiplier;
                iOutsideBlue = bailout * m_dOutsideBlueColorMultiplier;

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%256;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%256;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%256;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%256);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%256);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%256);}

                // Inside Coloring

                iInsideRed = bailout * m_dInsideRedColorMultiplier;
                iInsideGreen = bailout * m_dInsideGreenColorMultiplier;
                iInsideBlue = bailout * m_dInsideBlueColorMultiplier;

                if(iInsideRed > 255) {iInsideRed = iInsideRed%256;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%256;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%256;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%256);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%256);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%256);}

                // Drawing

                QColor insideColor(qBound(0, iInsideRed + m_iInsideRedColorAdder, 255),
                                   qBound(0, iInsideGreen + m_iInsideGreenColorAdder, 255),
                                   qBound(0, iInsideBlue + m_iInsideBlueColorAdder, 255),
                                   255);

                QColor outsideColor(qBound(0, iOutsideRed + m_iOutsideRedColorAdder, 255),
                                    qBound(0, iOutsideGreen + m_iOutsideGreenColorAdder, 255),
                                    qBound(0, iOutsideBlue + m_iOutsideBlueColorAdder, 255),
                                    255);

                if(bailout < m_dBailout)
                {
                    img->setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img->setPixel(xPix, yPix, outsideColor.rgba());
                }
            }
            else if(m_iColoringAlgorithm == 1) // Smooth Normalization
            {
                if(m_iAlgorithmAttributes == 0) // Regular
                {
                    iOutsideRed = (int)(m_dRedMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                    iOutsideGreen = (int)(m_dGreenMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                    iOutsideBlue = (int)(m_dBlueMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                }
                else if(m_iAlgorithmAttributes == 1) // Reverse
                {
                    if(m_bRedMainColor == true)
                    {
                        iOutsideRed = (int)(100 - m_dRedMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideRed = (int)(m_dRedMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }

                    if(m_bGreenMainColor == true)
                    {
                        iOutsideGreen = (int)(100 - m_dGreenMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideGreen = (int)(m_dGreenMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }

                    if(m_bBlueMainColor == true)
                    {
                        iOutsideBlue = (int)(100 - m_dBlueMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideBlue = (int)(m_dBlueMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                }

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%256;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%256;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%256;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%256);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%256);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%256);}

                // Inside Coloring

                iInsideRed = bailout * m_dInsideRedColorMultiplier;
                iInsideGreen = bailout * m_dInsideGreenColorMultiplier;
                iInsideBlue = bailout * m_dInsideBlueColorMultiplier;

                if(iInsideRed > 255) {iInsideRed = iInsideRed%256;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%256;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%256;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%256);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%256);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%256);}


                // Drawing

                QColor insideColor(qBound(0, iInsideRed + m_iInsideRedColorAdder, 255),
                                   qBound(0, iInsideGreen + m_iInsideGreenColorAdder, 255),
                                   qBound(0, iInsideBlue + m_iInsideBlueColorAdder, 255),
                                   255);

                QColor outsideColor(qBound(0, iOutsideRed + m_iOutsideRedColorAdder, 255),
                                    qBound(0, iOutsideGreen + m_iOutsideGreenColorAdder, 255),
                                    qBound(0, iOutsideBlue + m_iOutsideBlueColorAdder, 255),
                                    255);

                if(bailout < m_dBailout)
                {
                    img->setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img->setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            if(m_bSuspendValue == true)
            {
                emit animationProgress(0);
                emit progress(0);
                emit animationTimerUpdate(0.0);
                return;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;

        progressValue += 1.0/6.0;

        emit progress(progressValue);;

        if(m_bShowProgression == true)
        {
            emit rendered(*img);
        }
    }

    emit rendered(*img);
    emit progress(100);
}

void AnimationThread::renderAnimationNewton(QImage *img)
{
    double t, tBU = 0.0, x, y, power = 3.0;
    double dRelaxReal, dRelaxImag;

    int xPix, yPix;
    double bailout = 4.0;

    int iOutsideRed = 255, iOutsideGreen = 255, iOutsideBlue = 255;
    int iInsideRed = 0, iInsideGreen = 0, iInsideBlue = 0;

    float progressValue = 0.0;

    int width = 900;
    int height = 600;

    double xStart, yStart;
    double hyp, hx, hy = 0.0;
    double theta;

    complex<double> z;

    dRelaxReal = m_dControl1;
    dRelaxImag = m_dControl2;
    power = m_dControl3;


    hyp = sqrt(pow(3.0,2) + pow(2.0,2));
    theta = atan(2.0/3.0);

    xStart = m_dCenterX - hyp * cos(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;
    yStart = m_dCenterY + hyp * sin(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;

    z = complex<double>(xStart,yStart);

    for(yPix = 0; yPix < height; yPix++)
    {
        x = xStart - hy*sin(m_dRotation*DEGtoRAD);
        y = yStart - hy*cos(m_dRotation*DEGtoRAD);

        hx = 0.0;

        z = complex<double>(x,y);

        for(xPix = 0; xPix < width; xPix++)
        {
            z = complex<double>(x,y);

            for(t = 0; t < m_iIteration; t++)
            {
                z = z - complex<double>(dRelaxReal, dRelaxImag) * (pow(z,power)-1.0)/((double)power*pow(z,power-1.0));

                bailout = abs(z);

                if(bailout > m_dBailout)
                {
                    tBU = t;
                    t = m_iIteration;
                }
            }

            if(m_iColoringAlgorithm == 0) // Bailout Linear
            {
                // Outside Coloring

                iOutsideRed = bailout * m_dOutsideRedColorMultiplier;
                iOutsideGreen = bailout * m_dOutsideGreenColorMultiplier;
                iOutsideBlue = bailout * m_dOutsideBlueColorMultiplier;

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%256;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%256;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%256;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%256);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%256);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%256);}

                // Inside Coloring

                iInsideRed = bailout * m_dInsideRedColorMultiplier;
                iInsideGreen = bailout * m_dInsideGreenColorMultiplier;
                iInsideBlue = bailout * m_dInsideBlueColorMultiplier;

                if(iInsideRed > 255) {iInsideRed = iInsideRed%256;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%256;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%256;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%256);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%256);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%256);}

                // Drawing

                QColor insideColor(qBound(0, iInsideRed + m_iInsideRedColorAdder, 255),
                                   qBound(0, iInsideGreen + m_iInsideGreenColorAdder, 255),
                                   qBound(0, iInsideBlue + m_iInsideBlueColorAdder, 255),
                                   255);

                QColor outsideColor(qBound(0, iOutsideRed + m_iOutsideRedColorAdder, 255),
                                    qBound(0, iOutsideGreen + m_iOutsideGreenColorAdder, 255),
                                    qBound(0, iOutsideBlue + m_iOutsideBlueColorAdder, 255),
                                    255);

                if(bailout < m_dBailout)
                {
                    img->setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img->setPixel(xPix, yPix, outsideColor.rgba());
                }
            }
            else if(m_iColoringAlgorithm == 1) // Smooth Normalization
            {
                if(m_iAlgorithmAttributes == 0) // Regular
                {
                    iOutsideRed = (int)(m_dRedMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                    iOutsideGreen = (int)(m_dGreenMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                    iOutsideBlue = (int)(m_dBlueMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                }
                else if(m_iAlgorithmAttributes == 1) // Reverse
                {
                    if(m_bRedMainColor == true)
                    {
                        iOutsideRed = (int)(100 - m_dRedMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideRed = (int)(m_dRedMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }

                    if(m_bGreenMainColor == true)
                    {
                        iOutsideGreen = (int)(100 - m_dGreenMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideGreen = (int)(m_dGreenMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }

                    if(m_bBlueMainColor == true)
                    {
                        iOutsideBlue = (int)(100 - m_dBlueMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideBlue = (int)(m_dBlueMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                }

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%256;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%256;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%256;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%256);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%256);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%256);}

                // Inside Coloring

                iInsideRed = bailout * m_dInsideRedColorMultiplier;
                iInsideGreen = bailout * m_dInsideGreenColorMultiplier;
                iInsideBlue = bailout * m_dInsideBlueColorMultiplier;

                if(iInsideRed > 255) {iInsideRed = iInsideRed%256;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%256;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%256;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%256);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%256);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%256);}


                // Drawing

                QColor insideColor(qBound(0, iInsideRed + m_iInsideRedColorAdder, 255),
                                   qBound(0, iInsideGreen + m_iInsideGreenColorAdder, 255),
                                   qBound(0, iInsideBlue + m_iInsideBlueColorAdder, 255),
                                   255);

                QColor outsideColor(qBound(0, iOutsideRed + m_iOutsideRedColorAdder, 255),
                                    qBound(0, iOutsideGreen + m_iOutsideGreenColorAdder, 255),
                                    qBound(0, iOutsideBlue + m_iOutsideBlueColorAdder, 255),
                                    255);

                if(bailout < m_dBailout)
                {
                    img->setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img->setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            if(m_bSuspendValue == true)
            {
                emit animationProgress(0);
                emit progress(0);
                emit animationTimerUpdate(0.0);
                return;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;

        progressValue += 1.0/6.0;

        emit progress(progressValue);;

        if(m_bShowProgression == true)
        {
            emit rendered(*img);
        }
    }

    emit rendered(*img);
    emit progress(100);
}

void AnimationThread::renderAnimationNewtonRoot(QImage *img)
{
    double t, x, y;

    int xPix, yPix;

    double xStart, yStart;
    double hyp, hx, hy = 0.0;
    double theta;

    complex<double> z;

    int width = 900;
    int height = 600;

    float progressValue = 0.0;

    double dRelaxReal = m_dControl1;
    double dRelaxImag = m_dControl2;
    double tol = m_dControl3;


    hyp = sqrt(pow(3.0,2) + pow(2.0,2));
    theta = atan(2.0/3.0);

    xStart = m_dCenterX - hyp * cos(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;
    yStart = m_dCenterY + hyp * sin(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;

    z = complex<double>(xStart,yStart);

    complex<double> zGreen = complex<double>(-0.5, sin(2*3.1416/3));
    complex<double> zBlue = complex<double>(-0.5, -sin(2*3.1416/3));

    for(yPix = 0; yPix < height; yPix++)
    {
        x = xStart - hy*sin(m_dRotation*DEGtoRAD);
        y = yStart - hy*cos(m_dRotation*DEGtoRAD);

        hx = 0.0;

        z = complex<double>(x,y);

        for(xPix = 0; xPix < width; xPix++)
        {
            z = complex<double>(x,y);

            t = 0;

            while((t < m_iIteration) && (abs(z-1.0) >= tol) && (abs(z-zGreen) >= tol) && (abs(z-zBlue) >= tol))
            {
                z = z - complex<double>(dRelaxReal,dRelaxImag) * (z*z*z-1.0) / ((double)3.0*z*z);

                t++;
            }

            int tProcessedFirstRootRed = ((int)t*m_iFirstRootRed)%256;
            int tProcessedFirstRootGreen = ((int)t*m_iFirstRootGreen)%256;
            int tProcessedFirstRootBlue = ((int)t*m_iFirstRootBlue)%256;

            int tProcessedSecondRootRed = ((int)t*m_iSecondRootRed)%256;
            int tProcessedSecondRootGreen = ((int)t*m_iSecondRootGreen)%256;
            int tProcessedSecondRootBlue = ((int)t*m_iSecondRootBlue)%256;

            int tProcessedThirdRootRed = ((int)t*m_iThirdRootRed)%256;
            int tProcessedThirdRootGreen = ((int)t*m_iThirdRootGreen)%256;
            int tProcessedThirdRootBlue = ((int)t*m_iThirdRootBlue)%256;

            // Drawing
            QColor redColor(tProcessedFirstRootRed, tProcessedFirstRootGreen, tProcessedFirstRootBlue, 255);
            QColor greenColor(tProcessedSecondRootRed, tProcessedSecondRootGreen, tProcessedSecondRootBlue, 255);
            QColor blueColor(tProcessedThirdRootRed, tProcessedThirdRootGreen, tProcessedThirdRootBlue, 255);

            if(abs(z - 1.0) < tol)
            {
                img->setPixel(xPix, yPix, redColor.rgba());
            }

            if(abs(z - zGreen) <= tol)
            {
                img->setPixel(xPix, yPix, greenColor.rgba());
            }

            if(abs(z - zBlue) <= tol)
            {
                img->setPixel(xPix, yPix, blueColor.rgba());
            }

            if(m_bSuspendValue == true)
            {
                emit animationProgress(0);
                emit progress(0);
                emit animationTimerUpdate(0.0);
                return;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;

        progressValue += 1.0/6.0;

        emit progress(progressValue);

        if(m_bShowProgression == true)
        {
            emit rendered(*img);
        }
    }

    emit rendered(*img);
    emit progress(100);
}

void AnimationThread::renderAnimationNova(QImage *img)
{
    double t, tBU = 0.0, x, y;

    int xPix, yPix;
    double bailout = 4.0;

    int iOutsideRed = 255, iOutsideGreen = 255, iOutsideBlue = 255;
    int iInsideRed = 0, iInsideGreen = 0, iInsideBlue = 0;

    float progressValue = 0.0;

    int width = 900;
    int height = 600;

    double xStart, yStart;
    double hyp, hx, hy = 0.0;
    double theta;

    complex<double> z;

    double dSeeda = m_dControl1;
    double dSeedB = m_dControl2;

    double dRelaxReal = m_dControl3;
    double dRelaxImag = m_dControl4;

    double power = m_dControl5;

    double dDerivMult = m_dControl6;


    hyp = sqrt(pow(3.0,2) + pow(2.0,2));
    theta = atan(2.0/3.0);

    xStart = m_dCenterX - hyp * cos(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;
    yStart = m_dCenterY + hyp * sin(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;

    z = complex<double>(xStart,yStart);

    for(yPix = 0; yPix < height; yPix++)
    {
        x = xStart - hy*sin(m_dRotation*DEGtoRAD);
        y = yStart - hy*cos(m_dRotation*DEGtoRAD);

        hx = 0.0;

        z = complex<double>(x,y);

        for(xPix = 0; xPix < width; xPix++)
        {
            z = complex<double>(x,y);

            for(t = 0; t < m_iIteration; t++)
            {
                z = z - complex<double>(dRelaxReal, dRelaxImag)
                        * (pow(z,power)-1.0)/(dDerivMult*power*pow(z,power-1.0))
                        + complex<double>(dSeeda, dSeedB);

                bailout = abs(z);

                if(bailout > m_dBailout)
                {
                    tBU = t;
                    t = m_iIteration;
                }
            }

            if(m_iColoringAlgorithm == 0) // Bailout Linear
            {
                // Outside Coloring

                iOutsideRed = bailout * m_dOutsideRedColorMultiplier;
                iOutsideGreen = bailout * m_dOutsideGreenColorMultiplier;
                iOutsideBlue = bailout * m_dOutsideBlueColorMultiplier;

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%256;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%256;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%256;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%256);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%256);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%256);}

                // Inside Coloring

                iInsideRed = bailout * m_dInsideRedColorMultiplier;
                iInsideGreen = bailout * m_dInsideGreenColorMultiplier;
                iInsideBlue = bailout * m_dInsideBlueColorMultiplier;

                if(iInsideRed > 255) {iInsideRed = iInsideRed%256;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%256;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%256;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%256);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%256);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%256);}

                // Drawing

                QColor insideColor(qBound(0, iInsideRed + m_iInsideRedColorAdder, 255),
                                   qBound(0, iInsideGreen + m_iInsideGreenColorAdder, 255),
                                   qBound(0, iInsideBlue + m_iInsideBlueColorAdder, 255),
                                   255);

                QColor outsideColor(qBound(0, iOutsideRed + m_iOutsideRedColorAdder, 255),
                                    qBound(0, iOutsideGreen + m_iOutsideGreenColorAdder, 255),
                                    qBound(0, iOutsideBlue + m_iOutsideBlueColorAdder, 255),
                                    255);

                if(bailout < m_dBailout)
                {
                    img->setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img->setPixel(xPix, yPix, outsideColor.rgba());
                }
            }
            else if(m_iColoringAlgorithm == 1) // Smooth Normalization
            {
                if(m_iAlgorithmAttributes == 0) // Regular
                {
                    iOutsideRed = (int)(m_dRedMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                    iOutsideGreen = (int)(m_dGreenMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                    iOutsideBlue = (int)(m_dBlueMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                }
                else if(m_iAlgorithmAttributes == 1) // Reverse
                {
                    if(m_bRedMainColor == true)
                    {
                        iOutsideRed = (int)(100 - m_dRedMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideRed = (int)(m_dRedMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }

                    if(m_bGreenMainColor == true)
                    {
                        iOutsideGreen = (int)(100 - m_dGreenMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideGreen = (int)(m_dGreenMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }

                    if(m_bBlueMainColor == true)
                    {
                        iOutsideBlue = (int)(100 - m_dBlueMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideBlue = (int)(m_dBlueMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                }

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%256;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%256;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%256;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%256);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%256);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%256);}

                // Inside Coloring

                iInsideRed = bailout * m_dInsideRedColorMultiplier;
                iInsideGreen = bailout * m_dInsideGreenColorMultiplier;
                iInsideBlue = bailout * m_dInsideBlueColorMultiplier;

                if(iInsideRed > 255) {iInsideRed = iInsideRed%256;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%256;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%256;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%256);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%256);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%256);}


                // Drawing

                QColor insideColor(qBound(0, iInsideRed + m_iInsideRedColorAdder, 255),
                                   qBound(0, iInsideGreen + m_iInsideGreenColorAdder, 255),
                                   qBound(0, iInsideBlue + m_iInsideBlueColorAdder, 255),
                                   255);

                QColor outsideColor(qBound(0, iOutsideRed + m_iOutsideRedColorAdder, 255),
                                    qBound(0, iOutsideGreen + m_iOutsideGreenColorAdder, 255),
                                    qBound(0, iOutsideBlue + m_iOutsideBlueColorAdder, 255),
                                    255);

                if(bailout < m_dBailout)
                {
                    img->setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img->setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            if(m_bSuspendValue == true)
            {
                emit animationProgress(0);
                emit progress(0);
                emit animationTimerUpdate(0.0);
                return;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;

        progressValue += 1.0/6.0;

        emit progress(progressValue);;

        if(m_bShowProgression == true)
        {
            emit rendered(*img);
        }
    }

    emit rendered(*img);
    emit progress(100);
}

void AnimationThread::renderAnimationNovaSpecial(QImage *img)
{
    double t, tBU = 0.0, x, y;

    int xPix, yPix;
    double bailout = 4.0;

    int iOutsideRed = 255, iOutsideGreen = 255, iOutsideBlue = 255;
    int iInsideRed = 0, iInsideGreen = 0, iInsideBlue = 0;

    float progressValue = 0.0;

    int width = 900;
    int height = 600;

    double xStart, yStart;
    double hyp, hx, hy = 0.0;
    double theta;

    complex<double> z;

    double dRelaxReal = m_dControl1;
    double dRelaxImag = m_dControl2;

    double power = m_dControl3;

    double dDerivMult = m_dControl4;


    hyp = sqrt(pow(3.0,2) + pow(2.0,2));
    theta = atan(2.0/3.0);

    xStart = m_dCenterX - hyp * cos(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;
    yStart = m_dCenterY + hyp * sin(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;

    z = complex<double>(xStart,yStart);

    for(yPix = 0; yPix < height; yPix++)
    {
        x = xStart - hy*sin(m_dRotation*DEGtoRAD);
        y = yStart - hy*cos(m_dRotation*DEGtoRAD);

        hx = 0.0;

        z = complex<double>(x,y);

        for(xPix = 0; xPix < width; xPix++)
        {
            z = complex<double>(x,y);

            for(t = 0; t < m_iIteration; t++)
            {
                z = z - complex<double>(dRelaxReal, dRelaxImag)
                        * (pow(z,power)-1.0)/(dDerivMult*power*pow(z,power-1.0))
                        + complex<double>(x,y);

                bailout = abs(z);

                if(bailout > m_dBailout)
                {
                    tBU = t;
                    t = m_iIteration;
                }
            }

            if(m_iColoringAlgorithm == 0) // Bailout Linear
            {
                // Outside Coloring

                iOutsideRed = bailout * m_dOutsideRedColorMultiplier;
                iOutsideGreen = bailout * m_dOutsideGreenColorMultiplier;
                iOutsideBlue = bailout * m_dOutsideBlueColorMultiplier;

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%256;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%256;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%256;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%256);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%256);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%256);}

                // Inside Coloring

                iInsideRed = bailout * m_dInsideRedColorMultiplier;
                iInsideGreen = bailout * m_dInsideGreenColorMultiplier;
                iInsideBlue = bailout * m_dInsideBlueColorMultiplier;

                if(iInsideRed > 255) {iInsideRed = iInsideRed%256;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%256;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%256;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%256);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%256);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%256);}

                // Drawing

                QColor insideColor(qBound(0, iInsideRed + m_iInsideRedColorAdder, 255),
                                   qBound(0, iInsideGreen + m_iInsideGreenColorAdder, 255),
                                   qBound(0, iInsideBlue + m_iInsideBlueColorAdder, 255),
                                   255);

                QColor outsideColor(qBound(0, iOutsideRed + m_iOutsideRedColorAdder, 255),
                                    qBound(0, iOutsideGreen + m_iOutsideGreenColorAdder, 255),
                                    qBound(0, iOutsideBlue + m_iOutsideBlueColorAdder, 255),
                                    255);

                if(bailout < m_dBailout)
                {
                    img->setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img->setPixel(xPix, yPix, outsideColor.rgba());
                }
            }
            else if(m_iColoringAlgorithm == 1) // Smooth Normalization
            {
                if(m_iAlgorithmAttributes == 0) // Regular
                {
                    iOutsideRed = (int)(m_dRedMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                    iOutsideGreen = (int)(m_dGreenMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                    iOutsideBlue = (int)(m_dBlueMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                }
                else if(m_iAlgorithmAttributes == 1) // Reverse
                {
                    if(m_bRedMainColor == true)
                    {
                        iOutsideRed = (int)(100 - m_dRedMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideRed = (int)(m_dRedMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }

                    if(m_bGreenMainColor == true)
                    {
                        iOutsideGreen = (int)(100 - m_dGreenMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideGreen = (int)(m_dGreenMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }

                    if(m_bBlueMainColor == true)
                    {
                        iOutsideBlue = (int)(100 - m_dBlueMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideBlue = (int)(m_dBlueMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                }

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%256;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%256;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%256;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%256);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%256);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%256);}

                // Inside Coloring

                iInsideRed = bailout * m_dInsideRedColorMultiplier;
                iInsideGreen = bailout * m_dInsideGreenColorMultiplier;
                iInsideBlue = bailout * m_dInsideBlueColorMultiplier;

                if(iInsideRed > 255) {iInsideRed = iInsideRed%256;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%256;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%256;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%256);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%256);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%256);}


                // Drawing

                QColor insideColor(qBound(0, iInsideRed + m_iInsideRedColorAdder, 255),
                                   qBound(0, iInsideGreen + m_iInsideGreenColorAdder, 255),
                                   qBound(0, iInsideBlue + m_iInsideBlueColorAdder, 255),
                                   255);

                QColor outsideColor(qBound(0, iOutsideRed + m_iOutsideRedColorAdder, 255),
                                    qBound(0, iOutsideGreen + m_iOutsideGreenColorAdder, 255),
                                    qBound(0, iOutsideBlue + m_iOutsideBlueColorAdder, 255),
                                    255);

                if(bailout < m_dBailout)
                {
                    img->setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img->setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            if(m_bSuspendValue == true)
            {
                emit animationProgress(0);
                emit progress(0);
                emit animationTimerUpdate(0.0);
                return;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;

        progressValue += 1.0/6.0;

        emit progress(progressValue);;

        if(m_bShowProgression == true)
        {
            emit rendered(*img);
        }
    }

    emit rendered(*img);
    emit progress(100);
}

void AnimationThread::renderAnimationGlynn(QImage *img)
{
    double a, b, t, tBU = 0, x, y, power;

    int xPix, yPix;
    double bailout = 4.0;

    int iOutsideRed = 255, iOutsideGreen = 255, iOutsideBlue = 255;
    int iInsideRed = 0, iInsideGreen = 0, iInsideBlue = 0;

    float progressValue = 0.0;

    int width = 900;
    int height = 600;

    double xStart, yStart;
    double hyp, hx, hy = 0.0;
    double theta;

    complex<double> z;

    a = m_dControl1;
    b = m_dControl2;
    power = m_dControl3;

    complex<double> c(a,b);


    hyp = sqrt(pow(3.0,2) + pow(2.0,2));
    theta = atan(2.0/3.0);

    xStart = m_dCenterX - hyp * cos(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;
    yStart = m_dCenterY + hyp * sin(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;

    for(yPix = 0; yPix < height; yPix++)
    {
        x = xStart - hy*sin(m_dRotation*DEGtoRAD);
        y = yStart - hy*cos(m_dRotation*DEGtoRAD);

        hx = 0.0;

        for(xPix = 0; xPix < width; xPix++)
        {
            z = complex<double>(x,y);

            for (t = 0; t < m_iIteration; t++)
            {
                z = pow(z, power) + c;

                bailout = abs(z);

                if(bailout > m_dBailout)
                {
                    tBU = t;
                    t = m_iIteration;
                }
            }

            if(m_iColoringAlgorithm == 0) // Bailout Linear
            {
                // Outside Coloring

                iOutsideRed = bailout * m_dOutsideRedColorMultiplier;
                iOutsideGreen = bailout * m_dOutsideGreenColorMultiplier;
                iOutsideBlue = bailout * m_dOutsideBlueColorMultiplier;

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%256;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%256;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%256;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%256);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%256);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%256);}

                // Inside Coloring

                iInsideRed = bailout * m_dInsideRedColorMultiplier;
                iInsideGreen = bailout * m_dInsideGreenColorMultiplier;
                iInsideBlue = bailout * m_dInsideBlueColorMultiplier;

                if(iInsideRed > 255) {iInsideRed = iInsideRed%256;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%256;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%256;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%256);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%256);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%256);}

                // Drawing

                QColor insideColor(qBound(0, iInsideRed + m_iInsideRedColorAdder, 255),
                                   qBound(0, iInsideGreen + m_iInsideGreenColorAdder, 255),
                                   qBound(0, iInsideBlue + m_iInsideBlueColorAdder, 255),
                                   255);

                QColor outsideColor(qBound(0, iOutsideRed + m_iOutsideRedColorAdder, 255),
                                    qBound(0, iOutsideGreen + m_iOutsideGreenColorAdder, 255),
                                    qBound(0, iOutsideBlue + m_iOutsideBlueColorAdder, 255),
                                    255);

                if(bailout < m_dBailout)
                {
                    img->setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img->setPixel(xPix, yPix, outsideColor.rgba());
                }
            }
            else if(m_iColoringAlgorithm == 1) // Smooth Normalization
            {
                if(m_iAlgorithmAttributes == 0) // Regular
                {
                    iOutsideRed = (int)(m_dRedMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                    iOutsideGreen = (int)(m_dGreenMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                    iOutsideBlue = (int)(m_dBlueMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                }
                else if(m_iAlgorithmAttributes == 1) // Reverse
                {
                    if(m_bRedMainColor == true)
                    {
                        iOutsideRed = (int)(100 - m_dRedMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideRed = (int)(m_dRedMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }

                    if(m_bGreenMainColor == true)
                    {
                        iOutsideGreen = (int)(100 - m_dGreenMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideGreen = (int)(m_dGreenMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }

                    if(m_bBlueMainColor == true)
                    {
                        iOutsideBlue = (int)(100 - m_dBlueMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideBlue = (int)(m_dBlueMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                }

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%256;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%256;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%256;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%256);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%256);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%256);}

                // Inside Coloring

                iInsideRed = bailout * m_dInsideRedColorMultiplier;
                iInsideGreen = bailout * m_dInsideGreenColorMultiplier;
                iInsideBlue = bailout * m_dInsideBlueColorMultiplier;

                if(iInsideRed > 255) {iInsideRed = iInsideRed%256;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%256;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%256;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%256);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%256);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%256);}


                // Drawing

                QColor insideColor(qBound(0, iInsideRed + m_iInsideRedColorAdder, 255),
                                   qBound(0, iInsideGreen + m_iInsideGreenColorAdder, 255),
                                   qBound(0, iInsideBlue + m_iInsideBlueColorAdder, 255),
                                   255);

                QColor outsideColor(qBound(0, iOutsideRed + m_iOutsideRedColorAdder, 255),
                                    qBound(0, iOutsideGreen + m_iOutsideGreenColorAdder, 255),
                                    qBound(0, iOutsideBlue + m_iOutsideBlueColorAdder, 255),
                                    255);

                if(bailout < m_dBailout)
                {
                    img->setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img->setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            if(m_bSuspendValue == true)
            {
                emit animationProgress(0);
                emit progress(0);
                emit animationTimerUpdate(0.0);
                return;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;

        progressValue += 1.0/6.0;

        emit progress(progressValue);;

        if(m_bShowProgression == true)
        {
            emit rendered(*img);
        }
    }

    emit rendered(*img);
    emit progress(100);
}

void AnimationThread::renderAnimationPhoenix(QImage *img)
{
    double t, tBU = 0, x, y, power;

    int xPix, yPix;
    double bailout = 4.0;

    int iOutsideRed = 255, iOutsideGreen = 255, iOutsideBlue = 255;
    int iInsideRed = 0, iInsideGreen = 0, iInsideBlue = 0;

    float progressValue = 0.0;

    int width = 900;
    int height = 600;

    double xStart, yStart;
    double hyp, hx, hy = 0.0;
    double theta;

    complex<double> z, zBU, zBU2;

    complex<double> p1(m_dControl1, m_dControl2);
    complex<double> p2(m_dControl3, m_dControl4);

    power = m_dControl5;


    hyp = sqrt(pow(3.0,2) + pow(2.0,2));
    theta = atan(2.0/3.0);

    xStart = m_dCenterX - hyp * cos(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;
    yStart = m_dCenterY + hyp * sin(theta + (m_dRotation*DEGtoRAD))/m_dZoomRatio;


    for(yPix = 0; yPix < height; yPix++)
    {
        x = xStart - hy*sin(m_dRotation*DEGtoRAD);
        y = yStart - hy*cos(m_dRotation*DEGtoRAD);

        hx = 0.0;

        for(xPix = 0; xPix < width; xPix++)
        {
//			z = complex<double>(0.0,0.0);
            z = complex<double>(x,y);

            for (t = 0; t < m_iIteration; t++)
            {
                if(t == 0)
                {
                    zBU = z;
                    zBU2 = complex<double>(0.0,0.0);
                }
                else
                {
                    zBU2 = zBU;
                    zBU = z;
                }

                z = pow(z, power) + p1 + real(p2)*zBU2 + imag(p2)*zBU2;

                bailout = abs(z);

                if(bailout > m_dBailout)
                {
                    tBU = t;
                    t = m_iIteration;
                }
            }

            if(m_iColoringAlgorithm == 0) // Bailout Linear
            {
                // Outside Coloring

                iOutsideRed = bailout * m_dOutsideRedColorMultiplier;
                iOutsideGreen = bailout * m_dOutsideGreenColorMultiplier;
                iOutsideBlue = bailout * m_dOutsideBlueColorMultiplier;

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%256;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%256;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%256;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%256);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%256);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%256);}

                // Inside Coloring

                iInsideRed = bailout * m_dInsideRedColorMultiplier;
                iInsideGreen = bailout * m_dInsideGreenColorMultiplier;
                iInsideBlue = bailout * m_dInsideBlueColorMultiplier;

                if(iInsideRed > 255) {iInsideRed = iInsideRed%256;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%256;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%256;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%256);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%256);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%256);}

                // Drawing

                QColor insideColor(qBound(0, iInsideRed + m_iInsideRedColorAdder, 255),
                                   qBound(0, iInsideGreen + m_iInsideGreenColorAdder, 255),
                                   qBound(0, iInsideBlue + m_iInsideBlueColorAdder, 255),
                                   255);

                QColor outsideColor(qBound(0, iOutsideRed + m_iOutsideRedColorAdder, 255),
                                    qBound(0, iOutsideGreen + m_iOutsideGreenColorAdder, 255),
                                    qBound(0, iOutsideBlue + m_iOutsideBlueColorAdder, 255),
                                    255);

                if(bailout < m_dBailout)
                {
                    img->setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img->setPixel(xPix, yPix, outsideColor.rgba());
                }
            }
            else if(m_iColoringAlgorithm == 1) // Smooth Normalization
            {
                if(m_iAlgorithmAttributes == 0) // Regular
                {
                    iOutsideRed = (int)(m_dRedMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                    iOutsideGreen = (int)(m_dGreenMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                    iOutsideBlue = (int)(m_dBlueMultiplier*(tBU+1 - log(log(bailout))/log(2.0)));
                }
                else if(m_iAlgorithmAttributes == 1) // Reverse
                {
                    if(m_bRedMainColor == true)
                    {
                        iOutsideRed = (int)(100 - m_dRedMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideRed = (int)(m_dRedMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }

                    if(m_bGreenMainColor == true)
                    {
                        iOutsideGreen = (int)(100 - m_dGreenMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideGreen = (int)(m_dGreenMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }

                    if(m_bBlueMainColor == true)
                    {
                        iOutsideBlue = (int)(100 - m_dBlueMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                    else
                    {
                        iOutsideBlue = (int)(m_dBlueMultiplier * (tBU+1 - log(log(bailout))/log(2.0)));
                    }
                }

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%256;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%256;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%256;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%256);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%256);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%256);}

                // Inside Coloring

                iInsideRed = bailout * m_dInsideRedColorMultiplier;
                iInsideGreen = bailout * m_dInsideGreenColorMultiplier;
                iInsideBlue = bailout * m_dInsideBlueColorMultiplier;

                if(iInsideRed > 255) {iInsideRed = iInsideRed%256;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%256;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%256;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%256);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%256);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%256);}


                // Drawing

                QColor insideColor(qBound(0, iInsideRed + m_iInsideRedColorAdder, 255),
                                   qBound(0, iInsideGreen + m_iInsideGreenColorAdder, 255),
                                   qBound(0, iInsideBlue + m_iInsideBlueColorAdder, 255),
                                   255);

                QColor outsideColor(qBound(0, iOutsideRed + m_iOutsideRedColorAdder, 255),
                                    qBound(0, iOutsideGreen + m_iOutsideGreenColorAdder, 255),
                                    qBound(0, iOutsideBlue + m_iOutsideBlueColorAdder, 255),
                                    255);

                if(bailout < m_dBailout)
                {
                    img->setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img->setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            if(m_bSuspendValue == true)
            {
                emit animationProgress(0);
                emit progress(0);
                emit animationTimerUpdate(0.0);
                return;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;

        progressValue += 1.0/6.0;

        emit progress(progressValue);;

        if(m_bShowProgression == true)
        {
            emit rendered(*img);
        }
    }

    emit rendered(*img);
    emit progress(100);
}

void AnimationThread::removeAllJpgFilesInFolder()
{
    QDir folder("");
    folder.setNameFilters(QStringList()<<"*.jpg");
    fileList = folder.entryList();

    fileCount = fileList.count();

    for(int index = 0; index < fileCount; index++)
    {
        folder.remove(fileList[index]);
    }
}
