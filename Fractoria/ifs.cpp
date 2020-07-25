#include "ifs.h"
#include "ui_ifs.h"
#include <ctime>
#include <cstdlib>
#include <QMouseEvent>
#include <QColorDialog>

IFS::IFS(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IFS)
{
    ui->setupUi(this);

    this->ui->ifsLabel->installEventFilter(this);
    this->ui->ifsLabel->setMouseTracking(true);
    
    QStringList ifsList = (QStringList()<<"Sierpinski Triangle"<<"Sierpinski Carpet"<<"Koch Curve"<<"Spiral"
                               <<"Fern"<<"Tricurve"<<"Catalyst"<<"Knife");
    this->ui->ifsComboBox->addItems(ifsList);


    m_iIterations = 1000000;
    this->ui->iterationsLineEdit->setText(QString::number(m_iIterations));

    m_iRed = 0;
    this->ui->redLineEdit->setText(QString::number(m_iRed));
    m_iGreen = 0;
    this->ui->greenLineEdit->setText(QString::number(m_iGreen));
    m_iBlue = 0;
    this->ui->blueLineEdit->setText(QString::number(m_iBlue));

    m_bSpecialColoring = false;

    m_dZoomRatio = 1.0;
    this->ui->zoomRatioLineEdit->setText(QString::number(m_dZoomRatio));

    m_dCenterX = 0.5;
    this->ui->centerXLineEdit->setText(QString::number(m_dCenterX));
    m_dCenterY = 0.5;
    this->ui->centerYLineEdit->setText(QString::number(m_dCenterY));

    m_dCoordX = 0.5;
    m_dCoordY = 0.5;
}

IFS::~IFS()
{
    delete ui;
}

bool IFS::eventFilter(QObject *obj, QEvent *event)
{
    obj->parent();

    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        mouseMoveEvent(mouseEvent);
    }

    return false;
}

void IFS::mouseMoveEvent(QMouseEvent *event)
{
    int iXAxis;
    int iYAxis;
    m_dCoordX = 0.0;
    m_dCoordY = 0.0;

    if( (event->x() >= 0) && (event->x() < 600) )
    {
        if( (event->y() >= 0) && (event->y() < 600) )
        {
            iXAxis = event->x();
            iYAxis = event->y() - 600;

            m_dCoordX = m_dCenterX-0.5/m_dZoomRatio + ((1.0 / m_dZoomRatio) * double(iXAxis) / 600.0);
            m_dCoordY = m_dCenterY-0.5/m_dZoomRatio - ((1.0 / m_dZoomRatio) * double(iYAxis) / 600.0);
        }
    }

    this->ui->centerXLabel->setText(QString::number(m_dCoordX));
    this->ui->centerYLabel->setText(QString::number(m_dCoordY));
}

void IFS::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if( (event->x() >= 25) && (event->x() < 625) )
        {
            if( (event->y() >= 25) && (event->y() < 625) )
            {
                m_dCenterX = m_dCoordX;
                m_dCenterY = m_dCoordY;

                this->ui->centerXLineEdit->setText(QString::number(m_dCenterX));
                this->ui->centerYLineEdit->setText(QString::number(m_dCenterY));
            }
        }
    }
}

void IFS::on_renderButton_clicked()
{
    if(this->ui->ifsComboBox->currentText() == "Sierpinski Triangle")
    {
        renderSierpinskiTriangle();
    }
    else if(this->ui->ifsComboBox->currentText() == "Sierpinski Carpet")
    {
        renderSierpinskiCarpet();
    }
    else if(this->ui->ifsComboBox->currentText() == "Koch Curve")
    {
        renderKochCurve();
    }
    else if(this->ui->ifsComboBox->currentText() == "Spiral")
    {
        renderSpiral();
    }
    else if(this->ui->ifsComboBox->currentText() == "Fern")
    {
        renderFern();
    }
    else if(this->ui->ifsComboBox->currentText() == "Tricurve")
    {
        renderTricurve();
    }
    else if(this->ui->ifsComboBox->currentText() == "Catalyst")
    {
        renderCatalyst();
    }
    else if(this->ui->ifsComboBox->currentText() == "Knife")
    {
        renderKnife();
    }
}

void IFS::renderSierpinskiTriangle()
{
    img = QImage(600, 600, QImage::Format_ARGB32);

    float x = 0.0;
    float y = 0.0;
    float xp, yp;

    srand (static_cast <unsigned> (time(0)));

    for(int i = 0; i < m_iIterations; i++)
    {
        float k = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        if(k < 1.0/3.0)
        {
            xp = x/2.0;
            yp = y/2.0;
        }
        else if(k >= 1.0/3.0 && k < 2.0/3.0)
        {
            xp = x/2.0 + 0.5;
            yp = y/2.0;
        }
        else
        {
            xp = x/2.0 + 0.25;
            yp = y/2.0 + 0.5;
        }

        if(m_bSpecialColoring == true)
        {
            if((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio < 599
                    && (xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio >= 0
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 < 599
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 >= 0)
            {
                QColor specialColor(xp*255, yp*255, 255-xp*255, 255);
                img.setPixel((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio,
                             -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0, specialColor.rgba());
            }
        }
        else
        {
            if((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio < 599
                    && (xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio >= 0
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 < 599
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 >= 0)
            {
                QColor color(m_iRed, m_iGreen, m_iBlue, 255);
                img.setPixel((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio,
                             -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0, color.rgba());
            }
        }

        x = xp;
        y = yp;
    }

    showIFSImage();
}

void IFS::renderSierpinskiCarpet()
{
    img = QImage(600, 600, QImage::Format_ARGB32);

    float x = 0.0;
    float y = 0.0;
    float xp, yp;

    srand (static_cast <unsigned> (time(0)));

    for(int i = 0; i < m_iIterations; i++)
    {
        float k = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        if(k < 1.0/8.0)
        {
            xp = x/3.0;
            yp = y/3.0;
        }
        else if(k >= 1.0/8.0 && k < 1.0/4.0)
        {
            xp = x/3.0 + 1.0/3.0;
            yp = y/3.0;
        }
        else if(k >= 1.0/4.0 && k < 3.0/8.0)
        {
            xp = x/3.0 + 2.0/3.0;
            yp = y/3.0;
        }
        else if(k >= 3.0/8.0 && k < 1.0/2.0)
        {
            xp = x/3.0;
            yp = y/3.0 +1.0/3.0;
        }
        else if(k >= 1.0/2.0 && k < 5.0/8.0)
        {
            xp = x/3.0 + 2.0/3.0;
            yp = y/3.0 + 1.0/3.0;
        }
        else if(k >= 5.0/8.0 && k < 3.0/4.0)
        {
            xp = x/3.0;
            yp = y/3.0 + 2.0/3.0;
        }
        else if(k >= 3.0/4.0 && k < 7.0/8.0)
        {
            xp = x/3.0 + 1.0/3.0;
            yp = y/3.0 + 2.0/3.0;
        }
        else
        {
            xp = x/3.0 + 2.0/3.0;
            yp = y/3.0 + 2.0/3.0;
        }

        if(m_bSpecialColoring == true)
        {
            if((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio < 599
                    && (xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio >= 0
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 < 599
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 >= 0)
            {
                QColor specialColor(xp*255, yp*255, 255-xp*255, 255);
                img.setPixel((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio,
                             -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0, specialColor.rgba());
            }
        }
        else
        {
            if((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio < 599
                    && (xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio >= 0
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 < 599
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 >= 0)
            {
                QColor color(m_iRed, m_iGreen, m_iBlue, 255);
                img.setPixel((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio,
                             -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0, color.rgba());
            }
        }

        x = xp;
        y = yp;
    }

    showIFSImage();
}

void IFS::renderKochCurve()
{
    img = QImage(600, 600, QImage::Format_ARGB32);

    float x = 0.0;
    float y = 0.0;
    float xp, yp;

    srand (static_cast <unsigned> (time(0)));

    for(int i = 0; i < m_iIterations; i++)
    {
        float k = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        if(k < 1.0/4.0)
        {
            xp = x/3.0;
            yp = y/3.0;
        }
        else if(k >= 1.0/4.0 && k < 1.0/2.0)
        {
            xp = x/6.0 - 0.2886751346*y + 1.0/3.0;
            yp = 0.2886751346*x + y/6.0;
        }
        else if(k >= 1.0/2.0 && k < 3.0/4.0)
        {
            xp = x/6.0 + 0.2886751346*y + 1.0/2.0;
            yp = -0.2886751346*x + y/6.0 + 0.2886751346;
        }
        else
        {
            xp = x/3.0 + 2.0/3.0;
            yp = y/3.0;
        }

        if(m_bSpecialColoring == true)
        {
            if((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio < 599
                    && (xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio >= 0
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 < 599
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 >= 0)
            {
                QColor specialColor(xp*255, yp*255, 255-xp*255, 255);
                img.setPixel((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio,
                             -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0, specialColor.rgba());
            }
        }
        else
        {
            if((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio < 599
                    && (xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio >= 0
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 < 599
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 >= 0)
            {
                QColor color(m_iRed, m_iGreen, m_iBlue, 255);
                img.setPixel((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio,
                             -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0, color.rgba());
            }
        }

        x = xp;
        y = yp;
    }

    showIFSImage();
}

void IFS::renderSpiral()
{
    img = QImage(600, 600, QImage::Format_ARGB32);

    float x = 0.35;
    float y = 0.1;
    float xp, yp;

    srand (static_cast <unsigned> (time(0)));

    for(int i = 0; i < m_iIterations; i++)
    {
        float k = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        if(k < 0.0531981)
        {
            xp = 0.0064564*x - 0.1848873*y + 0.383;
            yp = 0.1848873*x + 0.0064564*y - 0.002;
        }
        else
        {
            xp = 0.9521784*x + 0.1850847*y - 0.076;
            yp = -0.1850847*x + 0.9521784*y + 0.119;
        }

        if(m_bSpecialColoring == true)
        {
            if((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio < 599
                    && (xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio >= 0
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 < 599
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 >= 0)
            {
                QColor specialColor(xp*255, yp*255, 255-xp*255, 255);
                img.setPixel((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio,
                             -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0, specialColor.rgba());
            }
        }
        else
        {
            if((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio < 599
                    && (xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio >= 0
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 < 599
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 >= 0)
            {
                QColor color(m_iRed, m_iGreen, m_iBlue, 255);
                img.setPixel((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio,
                             -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0, color.rgba());
            }
        }

        x = xp;
        y = yp;
    }

    showIFSImage();
}

void IFS::renderFern()
{
    img = QImage(600, 600, QImage::Format_ARGB32);

    float x = 0.5;
    float y = 0.1;
    float xp, yp;

    srand (static_cast <unsigned> (time(0)));

    for(int i = 0; i < m_iIterations; i++)
    {
        float k = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        if(k < 0.1026716)
        {
            xp = 0.1762366*x - 0.220643*y + 0.408;
            yp = 0.217634*x + 0.1723852*y + 0.0893;
        }
        else if(k >= 0.1026716 && k < 0.231011)
        {
            xp = -0.1799805*x + 0.2523837*y + 0.594;
            yp = 0.2144924*x + 0.2437241*y - 0.036;
        }
        else if(k >= 0.231011 && k < 0.2441069)
        {
            xp = 0.001*x + 0.5;
            yp = 0.35*y;
        }
        else
        {
            xp = 0.7795248*x + 0.0258256*y + 0.107;
            yp = -0.0272216*x + 0.7395492*y + 0.27;
        }

        if(m_bSpecialColoring == true)
        {
            if((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio < 599
                    && (xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio >= 0
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 < 599
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 >= 0)
            {
                QColor specialColor(xp*255, yp*255, 255-xp*255, 255);
                img.setPixel((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio,
                             -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0, specialColor.rgba());
            }
        }
        else
        {
            if((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio < 599
                    && (xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio >= 0
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 < 599
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 >= 0)
            {
                QColor color(m_iRed, m_iGreen, m_iBlue, 255);
                img.setPixel((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio,
                             -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0, color.rgba());
            }
        }

        x = xp;
        y = yp;
    }

    showIFSImage();
}

void IFS::renderTricurve()
{
    img = QImage(600, 600, QImage::Format_ARGB32);

    float x = 0.35;
    float y = 0.1;
    float xp, yp;

    srand (static_cast <unsigned> (time(0)));

    for(int i = 0; i < m_iIterations; i++)
    {
        float k = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        if(k < 0.8714597)
        {
            xp = -0.5*x + 0.8660254*y + 0.317;
            yp = -0.8660254*x - 0.5*y + 1.183;
        }
        else
        {
            xp = 0.25*x + 0.375;
            yp = 0.59*y + 0.035;
        }

        if(m_bSpecialColoring == true)
        {
            if((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio < 599
                    && (xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio >= 0
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 < 599
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 >= 0)
            {
                QColor specialColor(xp*255, yp*255, 255-xp*255, 255);
                img.setPixel((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio,
                             -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0, specialColor.rgba());
            }
        }
        else
        {
            if((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio < 599
                    && (xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio >= 0
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 < 599
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 >= 0)
            {
                QColor color(m_iRed, m_iGreen, m_iBlue, 255);
                img.setPixel((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio,
                             -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0, color.rgba());
            }
        }

        x = xp;
        y = yp;
    }

    showIFSImage();
}

void IFS::renderCatalyst()
{
    img = QImage(600, 600, QImage::Format_ARGB32);

    float x = 0.35;
    float y = 0.1;
    float xp, yp;

    srand (static_cast <unsigned> (time(0)));

    for(int i = 0; i < m_iIterations; i++)
    {
        float k = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        if(k < 0.2385533)
        {
            xp = x/3.0 + 0.45;
            yp = 2.0/3.0*y + 0.08;
        }
        else
        {
            xp = 0.421*x + 0.72919334*y - 0.079;
            yp = -0.72919334*x + 0.421*y + 0.658;
        }

        if(m_bSpecialColoring == true)
        {
            if((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio < 599
                    && (xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio >= 0
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 < 599
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 >= 0)
            {
                QColor specialColor(xp*255, yp*255, 255-xp*255, 255);
                img.setPixel((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio,
                             -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0, specialColor.rgba());
            }
        }
        else
        {
            if((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio < 599
                    && (xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio >= 0
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 < 599
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 >= 0)
            {
                QColor color(m_iRed, m_iGreen, m_iBlue, 255);
                img.setPixel((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio,
                             -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0, color.rgba());
            }
        }

        x = xp;
        y = yp;
    }

    showIFSImage();
}

void IFS::renderKnife()
{
    img = QImage(600, 600, QImage::Format_ARGB32);

    float x = 0.35;
    float y = 0.1;
    float xp, yp;

    srand (static_cast <unsigned> (time(0)));

    for(int i = 0; i < m_iIterations; i++)
    {
        float k = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        if(k < 0.2385533)
        {
            xp = x/3.0 + 0.45;
            yp = 2.0/3.0*y + 0.08;
        }
        else
        {
            xp = -0.3289956*x + 0.7750651*y + 0.289;
            yp = -0.7750651*x - 0.3289956*y +1.026;
        }

        if(m_bSpecialColoring == true)
        {
            if((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio < 599
                    && (xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio >= 0
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 < 599
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 >= 0)
            {
                QColor specialColor(xp*255, yp*255, 255-xp*255, 255);
                img.setPixel((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio,
                             -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0, specialColor.rgba());
            }
        }
        else
        {
            if((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio < 599
                    && (xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio >= 0
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 < 599
                    && -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0 >= 0)
            {
                QColor color(m_iRed, m_iGreen, m_iBlue, 255);
                img.setPixel((xp+0.5/m_dZoomRatio-m_dCenterX)*599.0*m_dZoomRatio,
                             -((yp+0.5/m_dZoomRatio-m_dCenterY)*599.0*m_dZoomRatio)+599.0, color.rgba());
            }
        }

        x = xp;
        y = yp;
    }

    showIFSImage();
}

void IFS::showIFSImage()
{
    this->ui->ifsLabel->setPixmap(QPixmap::fromImage(img));
}

void IFS::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return)
    {
        on_renderButton_clicked();
    }
}

void IFS::on_iterationsLineEdit_textChanged(const QString &arg1)
{
   m_iIterations = QString(arg1).toLongLong();
}

void IFS::on_redLineEdit_textChanged(const QString &arg1)
{
    m_iRed = QString(arg1).toInt();
}

void IFS::on_greenLineEdit_textChanged(const QString &arg1)
{
    m_iGreen = QString(arg1).toInt();
}

void IFS::on_blueLineEdit_textChanged(const QString &arg1)
{
    m_iBlue = QString(arg1).toInt();
}

void IFS::on_specialColoringCheckBox_toggled(bool checked)
{
    m_bSpecialColoring = checked;

    if(m_bSpecialColoring == true)
    {
        this->ui->redLineEdit->setEnabled(false);
        this->ui->redLineEdit->setText("N/A");
        this->ui->greenLineEdit->setEnabled(false);
        this->ui->greenLineEdit->setText("N/A");
        this->ui->blueLineEdit->setEnabled(false);
        this->ui->blueLineEdit->setText("N/A");

        this->ui->colorPickerPushButton->setEnabled(false);
    }
    else
    {
        this->ui->redLineEdit->setEnabled(true);
        this->ui->redLineEdit->setText(QString::number(m_iRed));
        this->ui->greenLineEdit->setEnabled(true);
        this->ui->greenLineEdit->setText(QString::number(m_iGreen));
        this->ui->blueLineEdit->setEnabled(true);
        this->ui->blueLineEdit->setText(QString::number(m_iBlue));

        this->ui->colorPickerPushButton->setEnabled(true);
    }
}

void IFS::on_zoomRatioLineEdit_textChanged(const QString &arg1)
{
    m_dZoomRatio = QString(arg1).toDouble();
}

void IFS::on_centerXLineEdit_textChanged(const QString &arg1)
{
    m_dCenterX = QString(arg1).toDouble();
}

void IFS::on_centerYLineEdit_textChanged(const QString &arg1)
{
    m_dCenterY = QString(arg1).toDouble();
}

void IFS::on_colorPickerPushButton_clicked()
{
    QColorDialog *colorDialog = new QColorDialog(this);

    QColor color = colorDialog->getColor(QColor(m_iRed, m_iGreen, m_iBlue), this, "IFS Color", 0);

    if(color.isValid())
    {
        m_iRed = color.red();
        this->ui->redLineEdit->setText(QString::number(m_iRed));
        m_iGreen = color.green();
        this->ui->greenLineEdit->setText(QString::number(m_iGreen));
        m_iBlue = color.blue();
        this->ui->blueLineEdit->setText(QString::number(m_iBlue));
    }
}
