#include "renderthread.h"
#include <math.h>
#include <QColor>
#include <complex>
#include <QElapsedTimer>
#include <QMessageBox>

#define DEGtoRAD 0.01745329251994329576923690768489
#define PI 3.14159265358979323846264338327950288419

using namespace std;

RenderThread::RenderThread()
{
    m_bSuspendValue = false;
    m_bSwitchFractalMode = false;
    m_bShowProgression = false;
}

RenderThread::~RenderThread()
{
}

void RenderThread::renderMandelbrotQuick()
{
    if(m_bSwitchFractalMode == true)
    {
        renderJuliaQuick();
    }
    else
    {
        img = QImage(900, 600, QImage::Format_ARGB32);

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

        QElapsedTimer timer;
        timer.start();


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
                        img.setPixel(xPix, yPix, insideColor.rgba());
                    }
                    else
                    {
                        img.setPixel(xPix, yPix, outsideColor.rgba());
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
                        img.setPixel(xPix, yPix, insideColor.rgba());
                    }
                    else
                    {
                        img.setPixel(xPix, yPix, outsideColor.rgba());
                    }
                }

                if(m_bSuspendValue == true)
                {
                    emit progress(0);
                    emit timerUpdate(0.0);
                    emit workFinished();
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
/*
            if(m_bShowProgression == true)
            {
                emit rendered(img);
            }
*/
            emit timerUpdate(timer.elapsed()/1000.0);
        }

        emit rendered(img);
        emit progress(100);
        emit timerUpdate(timer.elapsed()/1000.0);
        timer.invalidate();

        emit workFinished();
    }
}

void RenderThread::renderMandelbrot()
{
    if(m_bSwitchFractalMode == true)
    {
        renderJulia();
    }
    else
    {
        img = QImage(900, 600, QImage::Format_ARGB32);

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

        QElapsedTimer timer;
        timer.start();

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
                        img.setPixel(xPix, yPix, insideColor.rgba());
                    }
                    else
                    {
                        img.setPixel(xPix, yPix, outsideColor.rgba());
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
                        img.setPixel(xPix, yPix, insideColor.rgba());
                    }
                    else
                    {
                        img.setPixel(xPix, yPix, outsideColor.rgba());
                    }
                }

                if(m_bSuspendValue == true)
                {
                    emit progress(0);
                    emit timerUpdate(0.0);
                    emit workFinished();
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

            emit timerUpdate(timer.elapsed()/1000.0);

            if(m_bShowProgression == true)
            {
                emit rendered(img);
            }
        }

        emit rendered(img);
        emit progress(100);
        emit timerUpdate(timer.elapsed()/1000.0);
        timer.invalidate();

        emit workFinished();
    }
}

void RenderThread::renderJuliaQuick()
{
    img = QImage(900, 600, QImage::Format_ARGB32);

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

    QElapsedTimer timer;
    timer.start();

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
                    img.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img.setPixel(xPix, yPix, outsideColor.rgba());
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
                    img.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img.setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            if(m_bSuspendValue == true)
            {
                emit progress(0);
                emit timerUpdate(0.0);
                emit workFinished();
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
/*
        if(m_bShowProgression == true)
        {
            emit rendered(img);
        }
*/
        emit timerUpdate(timer.elapsed()/1000.0);
    }

    emit rendered(img);
    emit progress(100);
    emit timerUpdate(timer.elapsed()/1000.0);
    timer.invalidate();

    emit workFinished();
}

void RenderThread::renderJulia()
{
    img = QImage(900, 600, QImage::Format_ARGB32);

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

    QElapsedTimer timer;
    timer.start();

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
                    img.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img.setPixel(xPix, yPix, outsideColor.rgba());
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
                    img.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img.setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            if(m_bSuspendValue == true)
            {
                emit progress(0);
                emit timerUpdate(0.0);
                emit workFinished();
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
            emit rendered(img);
        }

        emit timerUpdate(timer.elapsed()/1000.0);
    }

    emit rendered(img);
    emit progress(100);
    emit timerUpdate(timer.elapsed()/1000.0);
    timer.invalidate();

    emit workFinished();
}

void RenderThread::renderNewton()
{
    img = QImage(900, 600, QImage::Format_ARGB32);

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

    QElapsedTimer timer;
    timer.start();

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
                    img.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img.setPixel(xPix, yPix, outsideColor.rgba());
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
                    img.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img.setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            if(m_bSuspendValue == true)
            {
                emit progress(0);
                emit timerUpdate(0.0);
                emit workFinished();
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
            emit rendered(img);
        }

        emit timerUpdate(timer.elapsed()/1000.0);
    }

    emit rendered(img);
    emit progress(100);
    emit timerUpdate(timer.elapsed()/1000.0);
    timer.invalidate();

    emit workFinished();
}

void RenderThread::renderNewtonRoot()
{
    img = QImage(900, 600, QImage::Format_ARGB32);

    double t, x, y;

    int xPix, yPix;
//    double bailout = 4.0;

//    int iInsideRed, iInsideGreen, iInsideBlue;

    double xStart, yStart;
    double hyp, hx, hy = 0.0;
    double theta;

    complex<double> z;

    int width = 900;
    int height = 600;

    float progressValue = 0.0;

    QElapsedTimer timer;
    timer.start();


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

//                bailout = abs(z);

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
                img.setPixel(xPix, yPix, redColor.rgba());
            }

            if(abs(z - zGreen) <= tol)
            {
                img.setPixel(xPix, yPix, greenColor.rgba());
            }

            if(abs(z - zBlue) <= tol)
            {
                img.setPixel(xPix, yPix, blueColor.rgba());
            }

            if(m_bSuspendValue == true)
            {
                emit progress(0);
                emit timerUpdate(0.0);
                emit workFinished();
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
            emit rendered(img);
        }

        emit timerUpdate(timer.elapsed()/1000.0);
    }

    emit rendered(img);
    emit progress(100);
    emit timerUpdate(timer.elapsed()/1000.0);
    timer.invalidate();

    emit workFinished();
}

void RenderThread::renderNova()
{
    if(m_bSwitchFractalMode == true)
    {
        renderNovaSpecial();
    }
    else
    {
        img = QImage(900, 600, QImage::Format_ARGB32);

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

        QElapsedTimer timer;
        timer.start();


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
                        img.setPixel(xPix, yPix, insideColor.rgba());
                    }
                    else
                    {
                        img.setPixel(xPix, yPix, outsideColor.rgba());
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
                        img.setPixel(xPix, yPix, insideColor.rgba());
                    }
                    else
                    {
                        img.setPixel(xPix, yPix, outsideColor.rgba());
                    }
                }

                if(m_bSuspendValue == true)
                {
                    emit progress(0);
                    emit timerUpdate(0.0);
                    emit workFinished();
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
                emit rendered(img);
            }

            emit timerUpdate(timer.elapsed()/1000.0);
        }

        emit rendered(img);
        emit progress(100);
        emit timerUpdate(timer.elapsed()/1000.0);
        timer.invalidate();

        emit workFinished();
    }
}

void RenderThread::renderNovaSpecial()
{
    img = QImage(900, 600, QImage::Format_ARGB32);

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

    QElapsedTimer timer;
    timer.start();


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
                    img.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img.setPixel(xPix, yPix, outsideColor.rgba());
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
                    img.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img.setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            if(m_bSuspendValue == true)
            {
                emit progress(0);
                emit timerUpdate(0.0);
                emit workFinished();
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
            emit rendered(img);
        }

        emit timerUpdate(timer.elapsed()/1000.0);
    }

    emit rendered(img);
    emit progress(100);
    emit timerUpdate(timer.elapsed()/1000.0);
    timer.invalidate();

    emit workFinished();
}

void RenderThread::renderGlynn()
{
    img = QImage(900, 600, QImage::Format_ARGB32);

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

    QElapsedTimer timer;
    timer.start();


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
                    img.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img.setPixel(xPix, yPix, outsideColor.rgba());
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
                    img.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img.setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            if(m_bSuspendValue == true)
            {
                emit progress(0);
                emit timerUpdate(0.0);
                emit workFinished();
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
            emit rendered(img);
        }

        emit timerUpdate(timer.elapsed()/1000.0);
    }

    emit rendered(img);
    emit progress(100);
    emit timerUpdate(timer.elapsed()/1000.0);
    timer.invalidate();

    emit workFinished();
}

void RenderThread::renderPhoenix()
{
    img = QImage(900, 600, QImage::Format_ARGB32);

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

    QElapsedTimer timer;
    timer.start();


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
                    img.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img.setPixel(xPix, yPix, outsideColor.rgba());
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
                    img.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    img.setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            if(m_bSuspendValue == true)
            {
                emit progress(0);
                emit timerUpdate(0.0);
                emit workFinished();
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
            emit rendered(img);
        }

        emit timerUpdate(timer.elapsed()/1000.0);
    }

    emit rendered(img);
    emit progress(100);

    emit timerUpdate(timer.elapsed()/1000.0);
    timer.invalidate();

    emit workFinished();
}

void RenderThread::geometryParameters(double xCenter, double yCenter, double zoomRatio, double rotation, long long iterations, double bailout)
{
    m_dCenterX = xCenter;
    m_dCenterY = yCenter;
    m_dZoomRatio = zoomRatio;
    m_dRotation = rotation;
    m_iIteration = iterations;
    m_dBailout = bailout;
}

void RenderThread::formulaParameters(double control1, double control2, double control3, double control4, double control5, double control6)
{
    m_dControl1 = control1;
    m_dControl2 = control2;
    m_dControl3 = control3;
    m_dControl4 = control4;
    m_dControl5 = control5;
    m_dControl6 = control6;
}

void RenderThread::outsideColoringParameters(int iOutsideRedColorAdder, int iOutsideGreenColorAdder, int iOutsideBlueColorAdder,
                                             double dOutsideRedColorMultiplier, double dOutsideGreenColorMultiplier, double dOutsideBlueColorMultiplier)
{
    m_iOutsideRedColorAdder = iOutsideRedColorAdder;
    m_iOutsideGreenColorAdder = iOutsideGreenColorAdder;
    m_iOutsideBlueColorAdder = iOutsideBlueColorAdder;
    m_dOutsideRedColorMultiplier = dOutsideRedColorMultiplier;
    m_dOutsideGreenColorMultiplier = dOutsideGreenColorMultiplier;
    m_dOutsideBlueColorMultiplier = dOutsideBlueColorMultiplier;
}

void RenderThread::insideColoringParameters(int iInsideRedColorAdder, int iInsideGreenColorAdder, int iInsideBlueColorAdder,
                                             double dInsideRedColorMultiplier, double dInsideGreenColorMultiplier, double dInsideBlueColorMultiplier)
{
    m_iInsideRedColorAdder = iInsideRedColorAdder;
    m_iInsideGreenColorAdder = iInsideGreenColorAdder;
    m_iInsideBlueColorAdder = iInsideBlueColorAdder;
    m_dInsideRedColorMultiplier = dInsideRedColorMultiplier;
    m_dInsideGreenColorMultiplier = dInsideGreenColorMultiplier;
    m_dInsideBlueColorMultiplier = dInsideBlueColorMultiplier;
}

void RenderThread::newtonRootColoringParameters(int iFirstRootRed, int iFirstRootGreen, int iFirstRootBlue,
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

void RenderThread::setColoringParameters(int iColoringAlgorithm, int iAlgorithmAttributes,
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

void RenderThread::setSwitchFractalModeParameter(bool bSwitchFractalMode)
{
    m_bSwitchFractalMode = bSwitchFractalMode;
}

void RenderThread::setShowProgressionParameter(bool bShowProgression)
{
    m_bShowProgression = bShowProgression;
}
