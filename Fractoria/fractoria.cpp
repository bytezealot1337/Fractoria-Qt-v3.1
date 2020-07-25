#include "fractoria.h"
#include "ui_fractoria.h"
#include <math.h>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <complex>
#include <QColorDialog>
#include <QDebug>

#define DEGtoRAD 0.01745329251994329576923690768489
#define PI 3.14159265358979323846264338327950288419

#define TRIGGER_POINTS_NUMBER 8

using namespace std;

Fractoria::Fractoria(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Fractoria)
{
    ui->setupUi(this);

    // Newton (Root) Coloring Member Varibales
    m_iFirstRootRed = 15;
    m_iFirstRootGreen = 0;
    m_iFirstRootBlue = 0;
    m_iSecondRootRed = 0;
    m_iSecondRootGreen = 15;
    m_iSecondRootBlue = 0;
    m_iThirdRootRed = 0;
    m_iThirdRootGreen = 0;
    m_iThirdRootBlue = 15;

    // ColoringAlgorithm Member Variables
    m_iColoringAlgorithm = 0;
    m_iAlgorithmAttributes = 0;

    m_dRedMultiplier = 10.0;
    m_dGreenMultiplier = 10.0;
    m_dBlueMultiplier = 10.0;

    m_bRedMainColor = true;
    m_bGreenMainColor = true;
    m_bBlueMainColor = true;

    // Animation Properties Variables
    m_iTotalFrameNumber = 1;

    for(int n = 0; n < TRIGGER_POINTS_NUMBER; n++)
    {
        m_iTriggerFrame << 0;
        m_dEndCenterX << 0.0;
        m_dEndCenterY << 0.0;
        m_iTranslationFunction << 0;
        m_dEndZoomRatio << 1.0;
        m_iZoomRatioFunction << 0;
        m_dEndRotation << 0.0;
        m_iRotationFunction << 0;
        m_iEndIteration << 25;
        m_iIterationFunction << 0;

        m_dAnimCtrl1 << 0.0;
        m_iAnimCtrl1Function << 0;
        m_dAnimCtrl2 << 0.0;
        m_iAnimCtrl2Function << 0;
        m_dAnimCtrl3 << 0.0;
        m_iAnimCtrl3Function << 0;
        m_dAnimCtrl4 << 0.0;
        m_iAnimCtrl4Function << 0;
        m_dAnimCtrl5 << 0.0;
        m_iAnimCtrl5Function << 0;
        m_dAnimCtrl6 << 0.0;
        m_iAnimCtrl6Function << 0;

        m_iAnimOutsideRedAdder << 0;
        m_iAnimOutsideRedAddFunction << 0;
        m_iAnimOutsideGreenAdder << 0;
        m_iAnimOutsideGreenAddFunction << 0;
        m_iAnimOutsideBlueAdder << 0;
        m_iAnimOutsideBlueAddFunction << 0;
        m_dAnimOutsideRedMultiplier << 1.0;
        m_iAnimOutsideRedMultFunction << 0;
        m_dAnimOutsideGreenMultiplier << 1.0;
        m_iAnimOutsideGreenMultFunction << 0;
        m_dAnimOutsideBlueMultiplier << 1.0;
        m_iAnimOutsideBlueMultFunction << 0;

        m_iAnimInsideRedAdder << 0;
        m_iAnimInsideRedAddFunction << 0;
        m_iAnimInsideGreenAdder << 0;
        m_iAnimInsideGreenAddFunction << 0;
        m_iAnimInsideBlueAdder << 0;
        m_iAnimInsideBlueAddFunction << 0;
        m_dAnimInsideRedMultiplier << 1.0;
        m_iAnimInsideRedMultFunction << 0;
        m_dAnimInsideGreenMultiplier << 1.0;
        m_iAnimInsideGreenMultFunction << 0;
        m_dAnimInsideBlueMultiplier << 1.0;
        m_iAnimInsideBlueMultFunction << 0;
    }

    // init Controls
    m_dControl1 = 0.0;
    m_dControl2 = 0.0;
    m_dControl3 = 0.0;
    m_dControl4 = 0.0;
    m_dControl5 = 0.0;
    m_dControl6 = 0.0;


    this->ui->fractalLabel->installEventFilter(this);
    this->setMouseTracking(true);

    QStringList fractalList = (QStringList()<<"Mandelbrot (Quick)"<<"Mandelbrot"<<"Julia (Quick)"<<"Julia"
                               <<"Newton"<<"Newton (Root)"<<"Nova"<<"Nova (Special)"<<"Glynn"<<"Phoenix");
    this->ui->fractalTypeComboBox1->addItems(fractalList);
    this->ui->fractalTypeComboBox2->addItems(fractalList);

    this->ui->xCenterLineEdit->setText("0.0");
    m_dCenterX = 0.0;
    this->ui->yCenterLineEdit->setText("0.0");
    m_dCenterY = 0.0;
    this->ui->zoomRatioLineEdit->setText("1.0");
    m_dZoomRatio = 1.0;
    this->ui->rotationLineEdit->setText("0.0");
    m_dRotation = 0.0;
    this->ui->iterationsLineEdit->setText("25");
    m_iIteration = 25;
    this->ui->bailoutLineEdit->setText("4.0");
    m_dBailout = 4.0;

    controlsFunction();
    outsideColoringFunction();
    insideColoringFunction();

    connect(this->ui->actionSave_Fractal, SIGNAL(triggered()), this, SLOT(saveFractalImage()));
    connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(closeApplication()));

    connect(this->ui->actionProperties, SIGNAL(triggered()), this, SLOT(coloringPropertiesFunction()));
    connect(this->ui->actionAnimationProperties, SIGNAL(triggered()), this, SLOT(animationPropertiesFunction()));

    connect(this->ui->actionOpen_Last_Animation, SIGNAL(triggered()), this, SLOT(startAnimationViewer()));

    connect(this->ui->actionOptions, SIGNAL(triggered()), this, SLOT(optionsFunction()));

    connect(this->ui->actionReset_Animation_Properties, SIGNAL(triggered()), this, SLOT(resetAnimationProperties()));

    connect(this->ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));

    connect(this->ui->actionIFS, SIGNAL(triggered()), this, SLOT(presentIFSController()));

    firstTime = true;

    renderState = false;
    animationState = false;

    m_bSwitchModeFractal = false;

    this->ui->suspendButton->setEnabled(false);

    if(this->ui->fractalTypeComboBox1->currentText() == "Mandelbrot (Quick)"
            || this->ui->fractalTypeComboBox1->currentText() == "Mandelbrot"
            || this->ui->fractalTypeComboBox1->currentText() == "Nova")
    {
        this->ui->switchModeCheckBox->setEnabled(true);
    }
    else
    {
        this->ui->switchModeCheckBox->setEnabled(false);
        this->ui->switchModeCheckBox->setChecked(false);
        m_bSwitchModeFractal = false;
    }

    m_bShowProgression = false;
}

Fractoria::~Fractoria()
{
    delete ui;
}

bool Fractoria::eventFilter(QObject *obj, QEvent *event)
{
    obj->parent();

    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        mouseMoveEvent(mouseEvent);
    }

    return false;
}

void Fractoria::mouseMoveEvent(QMouseEvent *event)
{
    int iXAxis;
    int iYAxis;
    double xCenter;
    double yCenter;
    m_dCoordX = 0.0;
    m_dCoordY = 0.0;
    double m_dCoordXBU;
    double m_dCoordYBU;

    if( (event->x() >= 0) && (event->x() < 900) )
    {
        if( (event->y() >= 0) && (event->y() < 600) )
        {
            iXAxis = event->x() - 450;
            iYAxis = event->y() - 300;

            xCenter = m_dCenterX;
            yCenter = m_dCenterY;

            m_dCoordXBU = + ((6.0 / m_dZoomRatio) * double(iXAxis) / 900.0);
            m_dCoordYBU = - ((4.0 / m_dZoomRatio) * double(iYAxis) / 600.0);

/* NE */	if( (iXAxis > 0) && (iYAxis <= 0) )
            {
                double hyp = sqrt(pow(m_dCoordXBU,2) + pow(m_dCoordYBU,2));
                double theta = atan(m_dCoordYBU / m_dCoordXBU);

                m_dCoordX = ( xCenter + hyp * cos(theta - m_dRotation*DEGtoRAD) );
                m_dCoordY = ( yCenter + hyp * sin(theta - m_dRotation*DEGtoRAD) );
            }
/* SE */	else if( (iXAxis > 0) && (iYAxis >= 0) )
            {
                double hyp = sqrt(pow(m_dCoordXBU,2) + pow(m_dCoordYBU,2));
                double theta = atan(m_dCoordYBU / m_dCoordXBU);

                m_dCoordX = ( xCenter + hyp * cos(theta - m_dRotation*DEGtoRAD) );
                m_dCoordY = ( yCenter + hyp * sin(theta - m_dRotation*DEGtoRAD) );
            }
/* SW */	else if( (iXAxis < 0) && (iYAxis >= 0) )
            {
                double hyp = sqrt(pow(m_dCoordXBU,2) + pow(m_dCoordYBU,2));
                double theta = atan(m_dCoordYBU / m_dCoordXBU);

                m_dCoordX = ( xCenter - hyp * cos(theta - m_dRotation*DEGtoRAD) );
                m_dCoordY = ( yCenter - hyp * sin(theta - m_dRotation*DEGtoRAD) );
            }
/* NW */	else if( (iXAxis < 0) && (iYAxis <= 0) )
            {
                double hyp = sqrt(pow(m_dCoordXBU,2) + pow(m_dCoordYBU,2));
                double theta = atan(m_dCoordYBU / m_dCoordXBU);

                m_dCoordX = ( xCenter - hyp * cos(theta - m_dRotation*DEGtoRAD) );
                m_dCoordY = ( yCenter - hyp * sin(theta - m_dRotation*DEGtoRAD) );
            }
/* E */ 	else if( (iXAxis == 0) && (iYAxis > 0) )
            {
                double hyp = sqrt(pow(m_dCoordXBU,2) + pow(m_dCoordYBU,2));
                double theta = atan(m_dCoordYBU / m_dCoordXBU);

                m_dCoordX = ( xCenter + hyp * cos(theta - m_dRotation*DEGtoRAD) );
                m_dCoordY = ( yCenter + hyp * sin(theta - m_dRotation*DEGtoRAD) );
            }

/* W */		else if( (iXAxis == 0) && (iYAxis < 0) )
            {
                double hyp = sqrt(pow(m_dCoordXBU,2) + pow(m_dCoordYBU,2));
                double theta = atan(m_dCoordYBU / m_dCoordXBU);

                m_dCoordX = ( xCenter - hyp * cos(theta - m_dRotation*DEGtoRAD) );
                m_dCoordY = ( yCenter - hyp * sin(theta - m_dRotation*DEGtoRAD) );
            }

/* x = 0 */ if(m_dCoordX == 0)
            {
                m_dCoordX = m_dCenterX;
            }

/* y = 0 */ if(m_dCoordY == 0)
            {
                m_dCoordY = m_dCenterY;
            }

            this->ui->xAxisMouseCoordinates->setText(QString::number(m_dCoordX));
            this->ui->yAxisMouseCoordinates->setText(QString::number(m_dCoordY));
        }
        else
        {
            this->ui->xAxisMouseCoordinates->setText(QString::number(m_dCoordX));
            this->ui->yAxisMouseCoordinates->setText(QString::number(m_dCoordY));
        }
    }
    else
    {
        this->ui->xAxisMouseCoordinates->setText(QString::number(m_dCoordX));
        this->ui->yAxisMouseCoordinates->setText(QString::number(m_dCoordY));
    }
}

void Fractoria::on_fractalTypeComboBox1_currentIndexChanged(int index)
{
    this->ui->fractalTypeComboBox2->setCurrentIndex(index);

    controlsFunction();
    outsideColoringFunction();
    insideColoringFunction();

    if(this->ui->fractalTypeComboBox1->currentText() == "Mandelbrot (Quick)"
            || this->ui->fractalTypeComboBox1->currentText() == "Mandelbrot"
            || this->ui->fractalTypeComboBox1->currentText() == "Nova")
    {
        this->ui->switchModeCheckBox->setEnabled(true);
    }
    else
    {
        this->ui->switchModeCheckBox->setEnabled(false);
        this->ui->switchModeCheckBox->setChecked(false);
        m_bSwitchModeFractal = false;
    }
}

void Fractoria::on_fractalTypeComboBox2_currentIndexChanged(int index)
{
    this->ui->fractalTypeComboBox1->setCurrentIndex(index);

    controlsFunction();
    outsideColoringFunction();
    insideColoringFunction();

    if(this->ui->fractalTypeComboBox1->currentText() == "Mandelbrot (Quick)"
            || this->ui->fractalTypeComboBox1->currentText() == "Mandelbrot"
            || this->ui->fractalTypeComboBox1->currentText() == "Nova")
    {
        this->ui->switchModeCheckBox->setEnabled(true);
    }
    else
    {
        this->ui->switchModeCheckBox->setEnabled(false);
        this->ui->switchModeCheckBox->setChecked(false);
        m_bSwitchModeFractal = false;
    }
}

void Fractoria::on_renderButton_clicked()
{
    renderState = true;
    this->ui->suspendButton->setEnabled(true);

    if(this->ui->fractalTypeComboBox1->currentText() == "Mandelbrot (Quick)")
    {
        thread = new QThread(this);
        renderTask = new RenderThread();

        renderTask->geometryParameters(m_dCenterX, m_dCenterY, m_dZoomRatio, m_dRotation, m_iIteration, m_dBailout);

        renderTask->formulaParameters(m_dControl1, m_dControl2, m_dControl3, m_dControl4, m_dControl5, m_dControl6);

        renderTask->outsideColoringParameters(m_iOutsideRedColorAdder, m_iOutsideGreenColorAdder, m_iOutsideBlueColorAdder,
                                              m_dOutsideRedColorMultiplier, m_dOutsideGreenColorMultiplier, m_dOutsideBlueColorMultiplier);

        renderTask->insideColoringParameters(m_iInsideRedColorAdder, m_iInsideGreenColorAdder, m_iInsideBlueColorAdder,
                                             m_dInsideRedColorMultiplier, m_dInsideGreenColorMultiplier, m_dInsideBlueColorMultiplier);

        renderTask->setColoringParameters(m_iColoringAlgorithm, m_iAlgorithmAttributes,
                                          m_dRedMultiplier, m_dGreenMultiplier, m_dBlueMultiplier,
                                          m_bRedMainColor, m_bGreenMainColor, m_bBlueMainColor);

        renderTask->setSwitchFractalModeParameter(m_bSwitchModeFractal);

        renderTask->setShowProgressionParameter(m_bShowProgression);

        renderTask->moveToThread(thread);

        connect(thread, SIGNAL(started()), renderTask, SLOT(renderMandelbrotQuick()));
        connect(renderTask, SIGNAL(workFinished()), this, SLOT(updateSuspendButtonState()));
        connect(renderTask, SIGNAL(workFinished()), thread, SLOT(quit()));
        connect(renderTask, SIGNAL(timerUpdate(double)), this, SLOT(updateTimer(double)));
        connect(renderTask, SIGNAL(rendered(QImage)), this, SLOT(showImage(QImage)));
        connect(renderTask, SIGNAL(progress(int)), this, SLOT(showProgress(int)));

        // Automatically delete thread and task object when work is done:
        connect(thread, SIGNAL(finished()), renderTask, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        thread->start();
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Mandelbrot")
    {
        thread = new QThread(this);
        renderTask = new RenderThread();

        renderTask->geometryParameters(m_dCenterX, m_dCenterY, m_dZoomRatio, m_dRotation, m_iIteration, m_dBailout);

        renderTask->formulaParameters(m_dControl1, m_dControl2, m_dControl3, m_dControl4, m_dControl5, m_dControl6);

        renderTask->outsideColoringParameters(m_iOutsideRedColorAdder, m_iOutsideGreenColorAdder, m_iOutsideBlueColorAdder,
                                              m_dOutsideRedColorMultiplier, m_dOutsideGreenColorMultiplier, m_dOutsideBlueColorMultiplier);

        renderTask->insideColoringParameters(m_iInsideRedColorAdder, m_iInsideGreenColorAdder, m_iInsideBlueColorAdder,
                                             m_dInsideRedColorMultiplier, m_dInsideGreenColorMultiplier, m_dInsideBlueColorMultiplier);

        renderTask->setColoringParameters(m_iColoringAlgorithm, m_iAlgorithmAttributes,
                                          m_dRedMultiplier, m_dGreenMultiplier, m_dBlueMultiplier,
                                          m_bRedMainColor, m_bGreenMainColor, m_bBlueMainColor);

        renderTask->setSwitchFractalModeParameter(m_bSwitchModeFractal);

        renderTask->setShowProgressionParameter(m_bShowProgression);

        renderTask->moveToThread(thread);

        connect(thread, SIGNAL(started()), renderTask, SLOT(renderMandelbrot()));
        connect(renderTask, SIGNAL(workFinished()), this, SLOT(updateSuspendButtonState()));
        connect(renderTask, SIGNAL(workFinished()), thread, SLOT(quit()));
        connect(renderTask, SIGNAL(timerUpdate(double)), this, SLOT(updateTimer(double)));
        connect(renderTask, SIGNAL(rendered(QImage)), this, SLOT(showImage(QImage)));
        connect(renderTask, SIGNAL(progress(int)), this, SLOT(showProgress(int)));

        // Automatically delete thread and task object when work is done:
        connect(thread, SIGNAL(finished()), renderTask, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        thread->start();
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Julia (Quick)")
    {
        thread = new QThread(this);
        renderTask = new RenderThread();

        renderTask->geometryParameters(m_dCenterX, m_dCenterY, m_dZoomRatio, m_dRotation, m_iIteration, m_dBailout);

        renderTask->formulaParameters(m_dControl1, m_dControl2, m_dControl3, m_dControl4, m_dControl5, m_dControl6);

        renderTask->outsideColoringParameters(m_iOutsideRedColorAdder, m_iOutsideGreenColorAdder, m_iOutsideBlueColorAdder,
                                              m_dOutsideRedColorMultiplier, m_dOutsideGreenColorMultiplier, m_dOutsideBlueColorMultiplier);

        renderTask->insideColoringParameters(m_iInsideRedColorAdder, m_iInsideGreenColorAdder, m_iInsideBlueColorAdder,
                                             m_dInsideRedColorMultiplier, m_dInsideGreenColorMultiplier, m_dInsideBlueColorMultiplier);

        renderTask->setColoringParameters(m_iColoringAlgorithm, m_iAlgorithmAttributes,
                                          m_dRedMultiplier, m_dGreenMultiplier, m_dBlueMultiplier,
                                          m_bRedMainColor, m_bGreenMainColor, m_bBlueMainColor);

        renderTask->setShowProgressionParameter(m_bShowProgression);

        renderTask->moveToThread(thread);

        connect(thread, SIGNAL(started()), renderTask, SLOT(renderJuliaQuick()));
        connect(renderTask, SIGNAL(workFinished()), this, SLOT(updateSuspendButtonState()));
        connect(renderTask, SIGNAL(workFinished()), thread, SLOT(quit()));
        connect(renderTask, SIGNAL(timerUpdate(double)), this, SLOT(updateTimer(double)));
        connect(renderTask, SIGNAL(rendered(QImage)), this, SLOT(showImage(QImage)));
        connect(renderTask, SIGNAL(progress(int)), this, SLOT(showProgress(int)));

        // Automatically delete thread and task object when work is done:
        connect(thread, SIGNAL(finished()), renderTask, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        thread->start();
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Julia")
    {
        thread = new QThread(this);
        renderTask = new RenderThread();

        renderTask->geometryParameters(m_dCenterX, m_dCenterY, m_dZoomRatio, m_dRotation, m_iIteration, m_dBailout);

        renderTask->formulaParameters(m_dControl1, m_dControl2, m_dControl3, m_dControl4, m_dControl5, m_dControl6);

        renderTask->outsideColoringParameters(m_iOutsideRedColorAdder, m_iOutsideGreenColorAdder, m_iOutsideBlueColorAdder,
                                              m_dOutsideRedColorMultiplier, m_dOutsideGreenColorMultiplier, m_dOutsideBlueColorMultiplier);

        renderTask->insideColoringParameters(m_iInsideRedColorAdder, m_iInsideGreenColorAdder, m_iInsideBlueColorAdder,
                                             m_dInsideRedColorMultiplier, m_dInsideGreenColorMultiplier, m_dInsideBlueColorMultiplier);

        renderTask->setColoringParameters(m_iColoringAlgorithm, m_iAlgorithmAttributes,
                                          m_dRedMultiplier, m_dGreenMultiplier, m_dBlueMultiplier,
                                          m_bRedMainColor, m_bGreenMainColor, m_bBlueMainColor);

        renderTask->setShowProgressionParameter(m_bShowProgression);

        renderTask->moveToThread(thread);

        connect(thread, SIGNAL(started()), renderTask, SLOT(renderJulia()));
        connect(renderTask, SIGNAL(workFinished()), this, SLOT(updateSuspendButtonState()));
        connect(renderTask, SIGNAL(workFinished()), thread, SLOT(quit()));
        connect(renderTask, SIGNAL(timerUpdate(double)), this, SLOT(updateTimer(double)));
        connect(renderTask, SIGNAL(rendered(QImage)), this, SLOT(showImage(QImage)));
        connect(renderTask, SIGNAL(progress(int)), this, SLOT(showProgress(int)));

        // Automatically delete thread and task object when work is done:
        connect(thread, SIGNAL(finished()), renderTask, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        thread->start();
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Newton")
    {
        thread = new QThread(this);
        renderTask = new RenderThread();

        renderTask->geometryParameters(m_dCenterX, m_dCenterY, m_dZoomRatio, m_dRotation, m_iIteration, m_dBailout);

        renderTask->formulaParameters(m_dControl1, m_dControl2, m_dControl3, m_dControl4, m_dControl5, m_dControl6);

        renderTask->outsideColoringParameters(m_iOutsideRedColorAdder, m_iOutsideGreenColorAdder, m_iOutsideBlueColorAdder,
                                              m_dOutsideRedColorMultiplier, m_dOutsideGreenColorMultiplier, m_dOutsideBlueColorMultiplier);

        renderTask->insideColoringParameters(m_iInsideRedColorAdder, m_iInsideGreenColorAdder, m_iInsideBlueColorAdder,
                                             m_dInsideRedColorMultiplier, m_dInsideGreenColorMultiplier, m_dInsideBlueColorMultiplier);

        renderTask->setColoringParameters(m_iColoringAlgorithm, m_iAlgorithmAttributes,
                                          m_dRedMultiplier, m_dGreenMultiplier, m_dBlueMultiplier,
                                          m_bRedMainColor, m_bGreenMainColor, m_bBlueMainColor);

        renderTask->setShowProgressionParameter(m_bShowProgression);

        renderTask->moveToThread(thread);

        connect(thread, SIGNAL(started()), renderTask, SLOT(renderNewton()));
        connect(renderTask, SIGNAL(workFinished()), this, SLOT(updateSuspendButtonState()));
        connect(renderTask, SIGNAL(workFinished()), thread, SLOT(quit()));
        connect(renderTask, SIGNAL(timerUpdate(double)), this, SLOT(updateTimer(double)));
        connect(renderTask, SIGNAL(rendered(QImage)), this, SLOT(showImage(QImage)));
        connect(renderTask, SIGNAL(progress(int)), this, SLOT(showProgress(int)));

        // Automatically delete thread and task object when work is done:
        connect(thread, SIGNAL(finished()), renderTask, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        thread->start();
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Newton (Root)")
    {
        thread = new QThread(this);
        renderTask = new RenderThread();

        renderTask->geometryParameters(m_dCenterX, m_dCenterY, m_dZoomRatio, m_dRotation, m_iIteration, m_dBailout);

        renderTask->formulaParameters(m_dControl1, m_dControl2, m_dControl3, m_dControl4, m_dControl5, m_dControl6);

        renderTask->newtonRootColoringParameters(m_iFirstRootRed, m_iFirstRootGreen, m_iFirstRootBlue,
                                                 m_iSecondRootRed, m_iSecondRootGreen, m_iSecondRootBlue,
                                                 m_iThirdRootRed, m_iThirdRootGreen, m_iThirdRootBlue);

        renderTask->setShowProgressionParameter(m_bShowProgression);

        renderTask->moveToThread(thread);

        connect(thread, SIGNAL(started()), renderTask, SLOT(renderNewtonRoot()));
        connect(renderTask, SIGNAL(workFinished()), this, SLOT(updateSuspendButtonState()));
        connect(renderTask, SIGNAL(workFinished()), thread, SLOT(quit()));
        connect(renderTask, SIGNAL(timerUpdate(double)), this, SLOT(updateTimer(double)));
        connect(renderTask, SIGNAL(rendered(QImage)), this, SLOT(showImage(QImage)));
        connect(renderTask, SIGNAL(progress(int)), this, SLOT(showProgress(int)));

        // Automatically delete thread and task object when work is done:
        connect(thread, SIGNAL(finished()), renderTask, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        thread->start();
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Nova")
    {
        thread = new QThread(this);
        renderTask = new RenderThread();

        renderTask->geometryParameters(m_dCenterX, m_dCenterY, m_dZoomRatio, m_dRotation, m_iIteration, m_dBailout);

        renderTask->formulaParameters(m_dControl1, m_dControl2, m_dControl3, m_dControl4, m_dControl5, m_dControl6);

        renderTask->outsideColoringParameters(m_iOutsideRedColorAdder, m_iOutsideGreenColorAdder, m_iOutsideBlueColorAdder,
                                              m_dOutsideRedColorMultiplier, m_dOutsideGreenColorMultiplier, m_dOutsideBlueColorMultiplier);

        renderTask->insideColoringParameters(m_iInsideRedColorAdder, m_iInsideGreenColorAdder, m_iInsideBlueColorAdder,
                                             m_dInsideRedColorMultiplier, m_dInsideGreenColorMultiplier, m_dInsideBlueColorMultiplier);

        renderTask->setColoringParameters(m_iColoringAlgorithm, m_iAlgorithmAttributes,
                                          m_dRedMultiplier, m_dGreenMultiplier, m_dBlueMultiplier,
                                          m_bRedMainColor, m_bGreenMainColor, m_bBlueMainColor);

        renderTask->setSwitchFractalModeParameter(m_bSwitchModeFractal);

        renderTask->setShowProgressionParameter(m_bShowProgression);

        renderTask->moveToThread(thread);

        connect(thread, SIGNAL(started()), renderTask, SLOT(renderNova()));
        connect(renderTask, SIGNAL(workFinished()), this, SLOT(updateSuspendButtonState()));
        connect(renderTask, SIGNAL(workFinished()), thread, SLOT(quit()));
        connect(renderTask, SIGNAL(timerUpdate(double)), this, SLOT(updateTimer(double)));
        connect(renderTask, SIGNAL(rendered(QImage)), this, SLOT(showImage(QImage)));
        connect(renderTask, SIGNAL(progress(int)), this, SLOT(showProgress(int)));

        // Automatically delete thread and task object when work is done:
        connect(thread, SIGNAL(finished()), renderTask, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        thread->start();
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Nova (Special)")
    {
        thread = new QThread(this);
        renderTask = new RenderThread();

        renderTask->geometryParameters(m_dCenterX, m_dCenterY, m_dZoomRatio, m_dRotation, m_iIteration, m_dBailout);

        renderTask->formulaParameters(m_dControl1, m_dControl2, m_dControl3, m_dControl4, m_dControl5, m_dControl6);

        renderTask->outsideColoringParameters(m_iOutsideRedColorAdder, m_iOutsideGreenColorAdder, m_iOutsideBlueColorAdder,
                                              m_dOutsideRedColorMultiplier, m_dOutsideGreenColorMultiplier, m_dOutsideBlueColorMultiplier);

        renderTask->insideColoringParameters(m_iInsideRedColorAdder, m_iInsideGreenColorAdder, m_iInsideBlueColorAdder,
                                             m_dInsideRedColorMultiplier, m_dInsideGreenColorMultiplier, m_dInsideBlueColorMultiplier);

        renderTask->setColoringParameters(m_iColoringAlgorithm, m_iAlgorithmAttributes,
                                          m_dRedMultiplier, m_dGreenMultiplier, m_dBlueMultiplier,
                                          m_bRedMainColor, m_bGreenMainColor, m_bBlueMainColor);

        renderTask->setShowProgressionParameter(m_bShowProgression);

        renderTask->moveToThread(thread);

        connect(thread, SIGNAL(started()), renderTask, SLOT(renderNovaSpecial()));
        connect(renderTask, SIGNAL(workFinished()), this, SLOT(updateSuspendButtonState()));
        connect(renderTask, SIGNAL(workFinished()), thread, SLOT(quit()));
        connect(renderTask, SIGNAL(timerUpdate(double)), this, SLOT(updateTimer(double)));
        connect(renderTask, SIGNAL(rendered(QImage)), this, SLOT(showImage(QImage)));
        connect(renderTask, SIGNAL(progress(int)), this, SLOT(showProgress(int)));

        // Automatically delete thread and task object when work is done:
        connect(thread, SIGNAL(finished()), renderTask, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        thread->start();
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Glynn")
    {
        thread = new QThread(this);
        renderTask = new RenderThread();

        renderTask->geometryParameters(m_dCenterX, m_dCenterY, m_dZoomRatio, m_dRotation, m_iIteration, m_dBailout);

        renderTask->formulaParameters(m_dControl1, m_dControl2, m_dControl3, m_dControl4, m_dControl5, m_dControl6);

        renderTask->outsideColoringParameters(m_iOutsideRedColorAdder, m_iOutsideGreenColorAdder, m_iOutsideBlueColorAdder,
                                              m_dOutsideRedColorMultiplier, m_dOutsideGreenColorMultiplier, m_dOutsideBlueColorMultiplier);

        renderTask->insideColoringParameters(m_iInsideRedColorAdder, m_iInsideGreenColorAdder, m_iInsideBlueColorAdder,
                                             m_dInsideRedColorMultiplier, m_dInsideGreenColorMultiplier, m_dInsideBlueColorMultiplier);

        renderTask->setColoringParameters(m_iColoringAlgorithm, m_iAlgorithmAttributes,
                                          m_dRedMultiplier, m_dGreenMultiplier, m_dBlueMultiplier,
                                          m_bRedMainColor, m_bGreenMainColor, m_bBlueMainColor);

        renderTask->setShowProgressionParameter(m_bShowProgression);

        renderTask->moveToThread(thread);

        connect(thread, SIGNAL(started()), renderTask, SLOT(renderGlynn()));
        connect(renderTask, SIGNAL(workFinished()), this, SLOT(updateSuspendButtonState()));
        connect(renderTask, SIGNAL(workFinished()), thread, SLOT(quit()));
        connect(renderTask, SIGNAL(timerUpdate(double)), this, SLOT(updateTimer(double)));
        connect(renderTask, SIGNAL(rendered(QImage)), this, SLOT(showImage(QImage)));
        connect(renderTask, SIGNAL(progress(int)), this, SLOT(showProgress(int)));

        // Automatically delete thread and task object when work is done:
        connect(thread, SIGNAL(finished()), renderTask, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        thread->start();
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Phoenix")
    {
        thread = new QThread(this);
        renderTask = new RenderThread();

        renderTask->geometryParameters(m_dCenterX, m_dCenterY, m_dZoomRatio, m_dRotation, m_iIteration, m_dBailout);

        renderTask->formulaParameters(m_dControl1, m_dControl2, m_dControl3, m_dControl4, m_dControl5, m_dControl6);

        renderTask->outsideColoringParameters(m_iOutsideRedColorAdder, m_iOutsideGreenColorAdder, m_iOutsideBlueColorAdder,
                                              m_dOutsideRedColorMultiplier, m_dOutsideGreenColorMultiplier, m_dOutsideBlueColorMultiplier);

        renderTask->insideColoringParameters(m_iInsideRedColorAdder, m_iInsideGreenColorAdder, m_iInsideBlueColorAdder,
                                             m_dInsideRedColorMultiplier, m_dInsideGreenColorMultiplier, m_dInsideBlueColorMultiplier);

        renderTask->setColoringParameters(m_iColoringAlgorithm, m_iAlgorithmAttributes,
                                          m_dRedMultiplier, m_dGreenMultiplier, m_dBlueMultiplier,
                                          m_bRedMainColor, m_bGreenMainColor, m_bBlueMainColor);

        renderTask->setShowProgressionParameter(m_bShowProgression);

        renderTask->moveToThread(thread);

        connect(thread, SIGNAL(started()), renderTask, SLOT(renderPhoenix()));
        connect(renderTask, SIGNAL(workFinished()), this, SLOT(updateSuspendButtonState()));
        connect(renderTask, SIGNAL(workFinished()), thread, SLOT(quit()));
        connect(renderTask, SIGNAL(timerUpdate(double)), this, SLOT(updateTimer(double)));
        connect(renderTask, SIGNAL(rendered(QImage)), this, SLOT(showImage(QImage)));
        connect(renderTask, SIGNAL(progress(int)), this, SLOT(showProgress(int)));

        // Automatically delete thread and task object when work is done:
        connect(thread, SIGNAL(finished()), renderTask, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        thread->start();
    }
}

void Fractoria::saveFractalImage()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Fractal Image", "untitled", "jpeg (*.jpg)");
    QFile file(fileName);

    if(file.open(QIODevice::WriteOnly))
    {
        img.save(fileName, "JPG", -1);

        file.close();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Fractal was not saved...");
        msgBox.exec();

        return;
    }
}

void Fractoria::closeApplication()
{
    if(mayExit() == true)
    {
        qApp->exit(0);
    }
}

void Fractoria::closeEvent(QCloseEvent *event)
{
    if(mayExit() == true)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

bool Fractoria::mayExit()
{
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Fractoria"),
                 tr("Are you sure you want to exit ?"),
                 QMessageBox::Cancel | QMessageBox::Ok);

    if(ret == QMessageBox::Cancel)
    {
        return false;
    }
    else if(ret == QMessageBox::Ok)
    {
        return true;
    }

    return true;
}

void Fractoria::showImage(QImage image)
{
    this->ui->fractalLabel->setPixmap(QPixmap::fromImage(image));

    img = image;
}

void Fractoria::showProgress(int progressValue)
{
    this->ui->progressBar->setValue(progressValue);
}

void Fractoria::animationShowProgress(int progressValue)
{
    this->ui->animationProgressBar->setValue(progressValue);
}

void Fractoria::updateTimer(double duration)
{
    this->ui->elapsedTimeLabel->setText(QString::number(duration));
}

void Fractoria::animationUpdateTimer(double duration)
{
    this->ui->elapsedTimeLabel->setText(QString::number(duration));
}

void Fractoria::on_xCenterLineEdit_textChanged(const QString &arg1)
{
    m_dCenterX = QString(arg1).toDouble();
}

void Fractoria::on_yCenterLineEdit_textChanged(const QString &arg1)
{
    m_dCenterY = QString(arg1).toDouble();
}

void Fractoria::on_zoomRatioLineEdit_textChanged(const QString &arg1)
{
    m_dZoomRatio = QString(arg1).toDouble();
}

void Fractoria::on_rotationLineEdit_textChanged(const QString &arg1)
{
    m_dRotation = QString(arg1).toDouble();
}

void Fractoria::on_iterationsLineEdit_textChanged(const QString &arg1)
{
    m_iIteration = QString(arg1).toLongLong();
}

void Fractoria::on_bailoutLineEdit_textChanged(const QString &arg1)
{
    m_dBailout = QString(arg1).toDouble();
}

void Fractoria::on_control1LineEdit_textChanged(const QString &arg1)
{
    m_dControl1 = QString(arg1).toDouble();
}

void Fractoria::on_control2LineEdit_textChanged(const QString &arg1)
{
    m_dControl2 = QString(arg1).toDouble();
}

void Fractoria::on_control3LineEdit_textChanged(const QString &arg1)
{
    m_dControl3 = QString(arg1).toDouble();
}

void Fractoria::on_control4LineEdit_textChanged(const QString &arg1)
{
    m_dControl4 = QString(arg1).toDouble();
}

void Fractoria::on_control5LineEdit_textChanged(const QString &arg1)
{
    m_dControl5 = QString(arg1).toDouble();
}

void Fractoria::on_control6LineEdit_textChanged(const QString &arg1)
{
    m_dControl6 = QString(arg1).toDouble();
}

void Fractoria::controlsFunction()
{
    this->ui->control1Label->setText("Control #1 :");
    this->ui->control2Label->setText("Control #2 :");
    this->ui->control3Label->setText("Control #3 :");
    this->ui->control4Label->setText("Control #4 :");
    this->ui->control5Label->setText("Control #5 :");
    this->ui->control6Label->setText("Control #6 :");

    this->ui->control1LineEdit->setText("");
    this->ui->control2LineEdit->setText("");
    this->ui->control3LineEdit->setText("");
    this->ui->control4LineEdit->setText("");
    this->ui->control5LineEdit->setText("");
    this->ui->control6LineEdit->setText("");

    if(this->ui->fractalTypeComboBox1->currentText() == "Mandelbrot (Quick)")
    {
        this->ui->control1Label->setEnabled(true);
        this->ui->control1LineEdit->setEnabled(true);
        this->ui->control1Label->setText("Start (Re) :");
        this->ui->control1LineEdit->setText("0.0");
        m_dControl1 = 0.0;
        this->ui->control2Label->setEnabled(true);
        this->ui->control2LineEdit->setEnabled(true);
        this->ui->control2Label->setText("Start (Im) :");
        this->ui->control2LineEdit->setText("0.0");
        m_dControl2 = 0.0;
        this->ui->control3Label->setEnabled(false);
        this->ui->control3LineEdit->setEnabled(false);
        this->ui->control3LineEdit->setText("");
        this->ui->control4Label->setEnabled(false);
        this->ui->control4LineEdit->setEnabled(false);
        this->ui->control4LineEdit->setText("");
        this->ui->control5Label->setEnabled(false);
        this->ui->control5LineEdit->setEnabled(false);
        this->ui->control5LineEdit->setText("");
        this->ui->control6Label->setEnabled(false);
        this->ui->control6LineEdit->setEnabled(false);
        this->ui->control6LineEdit->setText("");
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Mandelbrot")
    {
        this->ui->control1Label->setEnabled(true);
        this->ui->control1LineEdit->setEnabled(true);
        this->ui->control1Label->setText("Start (Re) :");
        this->ui->control1LineEdit->setText("0.0");
        m_dControl1 = 0.0;
        this->ui->control2Label->setEnabled(true);
        this->ui->control2LineEdit->setEnabled(true);
        this->ui->control2Label->setText("Start (Im) :");
        this->ui->control2LineEdit->setText("0.0");
        m_dControl2 = 0.0;
        this->ui->control3Label->setEnabled(true);
        this->ui->control3LineEdit->setEnabled(true);
        this->ui->control3Label->setText("Power :");
        this->ui->control3LineEdit->setText("2.0");
        m_dControl3 = 2.0;
        this->ui->control4Label->setEnabled(false);
        this->ui->control4LineEdit->setEnabled(false);
        this->ui->control4LineEdit->setText("");
        this->ui->control5Label->setEnabled(false);
        this->ui->control5LineEdit->setEnabled(false);
        this->ui->control5LineEdit->setText("");
        this->ui->control6Label->setEnabled(false);
        this->ui->control6LineEdit->setEnabled(false);
        this->ui->control6LineEdit->setText("");
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Julia (Quick)")
    {
        this->ui->control1Label->setEnabled(true);
        this->ui->control1LineEdit->setEnabled(true);
        this->ui->control1Label->setText("Seed (Re) :");
        this->ui->control1LineEdit->setText("-0.235");
        m_dControl1 = -0.235;
        this->ui->control2Label->setEnabled(true);
        this->ui->control2LineEdit->setEnabled(true);
        this->ui->control2Label->setText("Seed (Im) :");
        this->ui->control2LineEdit->setText("0.8");
        m_dControl2 = 0.8;
        this->ui->control3Label->setEnabled(false);
        this->ui->control3LineEdit->setEnabled(false);
        this->ui->control3LineEdit->setText("");
        this->ui->control4Label->setEnabled(false);
        this->ui->control4LineEdit->setEnabled(false);
        this->ui->control4LineEdit->setText("");
        this->ui->control5Label->setEnabled(false);
        this->ui->control5LineEdit->setEnabled(false);
        this->ui->control5LineEdit->setText("");
        this->ui->control6Label->setEnabled(false);
        this->ui->control6LineEdit->setEnabled(false);
        this->ui->control6LineEdit->setText("");
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Julia")
    {
        this->ui->control1Label->setEnabled(true);
        this->ui->control1LineEdit->setEnabled(true);
        this->ui->control1Label->setText("Seed (Re) :");
        this->ui->control1LineEdit->setText("-0.235");
        m_dControl1 = -0.235;
        this->ui->control2Label->setEnabled(true);
        this->ui->control2LineEdit->setEnabled(true);
        this->ui->control2Label->setText("Seed (Im) :");
        this->ui->control2LineEdit->setText("0.8");
        m_dControl2 = 0.8;
        this->ui->control3Label->setEnabled(true);
        this->ui->control3LineEdit->setEnabled(true);
        this->ui->control3Label->setText("Power :");
        this->ui->control3LineEdit->setText("2.0");
        m_dControl3 = 2.0;
        this->ui->control4Label->setEnabled(false);
        this->ui->control4LineEdit->setEnabled(false);
        this->ui->control4LineEdit->setText("");
        this->ui->control5Label->setEnabled(false);
        this->ui->control5LineEdit->setEnabled(false);
        this->ui->control5LineEdit->setText("");
        this->ui->control6Label->setEnabled(false);
        this->ui->control6LineEdit->setEnabled(false);
        this->ui->control6LineEdit->setText("");
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Newton")
    {
        this->ui->control1Label->setEnabled(true);
        this->ui->control1LineEdit->setEnabled(true);
        this->ui->control1Label->setText("Relax (Re) :");
        this->ui->control1LineEdit->setText("1.0");
        m_dControl1 = 1.0;
        this->ui->control2Label->setEnabled(true);
        this->ui->control2LineEdit->setEnabled(true);
        this->ui->control2Label->setText("Relax (Im) :");
        this->ui->control2LineEdit->setText("0.0");
        m_dControl2 = 0.0;
        this->ui->control3Label->setEnabled(true);
        this->ui->control3LineEdit->setEnabled(true);
        this->ui->control3Label->setText("Power :");
        this->ui->control3LineEdit->setText("3.0");
        m_dControl3 = 3.0;
        this->ui->control4Label->setEnabled(false);
        this->ui->control4LineEdit->setEnabled(false);
        this->ui->control4LineEdit->setText("");
        this->ui->control5Label->setEnabled(false);
        this->ui->control5LineEdit->setEnabled(false);
        this->ui->control5LineEdit->setText("");
        this->ui->control6Label->setEnabled(false);
        this->ui->control6LineEdit->setEnabled(false);
        this->ui->control6LineEdit->setText("");
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Newton (Root)")
    {
        this->ui->control1Label->setEnabled(true);
        this->ui->control1LineEdit->setEnabled(true);
        this->ui->control1Label->setText("Relax (Re) :");
        this->ui->control1LineEdit->setText("1.0");
        m_dControl1 = 1.0;
        this->ui->control2Label->setEnabled(true);
        this->ui->control2LineEdit->setEnabled(true);
        this->ui->control2Label->setText("Relax (Im) :");
        this->ui->control2LineEdit->setText("0.0");
        m_dControl2 = 0.0;
        this->ui->control3Label->setEnabled(true);
        this->ui->control3LineEdit->setEnabled(true);
        this->ui->control3Label->setText("Tolerance :");
        this->ui->control3LineEdit->setText("0.0001");
        m_dControl3 = 0.0001;
        this->ui->control4Label->setEnabled(false);
        this->ui->control4LineEdit->setEnabled(false);
        this->ui->control4LineEdit->setText("");
        this->ui->control5Label->setEnabled(false);
        this->ui->control5LineEdit->setEnabled(false);
        this->ui->control5LineEdit->setText("");
        this->ui->control6Label->setEnabled(false);
        this->ui->control6LineEdit->setEnabled(false);
        this->ui->control6LineEdit->setText("");
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Nova")
    {
        this->ui->control1Label->setEnabled(true);
        this->ui->control1LineEdit->setEnabled(true);
        this->ui->control1Label->setText("Seed (Re) :");
        this->ui->control1LineEdit->setText("1.0");
        m_dControl1 = 1.0;
        this->ui->control2Label->setEnabled(true);
        this->ui->control2LineEdit->setEnabled(true);
        this->ui->control2Label->setText("Seed (Im) :");
        this->ui->control2LineEdit->setText("1.0");
        m_dControl2 = 1.0;
        this->ui->control3Label->setEnabled(true);
        this->ui->control3LineEdit->setEnabled(true);
        this->ui->control3Label->setText("Relax (Re) :");
        this->ui->control3LineEdit->setText("3.0");
        m_dControl3 = 3.0;
        this->ui->control4Label->setEnabled(true);
        this->ui->control4LineEdit->setEnabled(true);
        this->ui->control4Label->setText("Relax (Im) :");
        this->ui->control4LineEdit->setText("2.0");
        m_dControl4 = 2.0;
        this->ui->control5Label->setEnabled(true);
        this->ui->control5LineEdit->setEnabled(true);
        this->ui->control5Label->setText("Power :");
        this->ui->control5LineEdit->setText("3.0");
        m_dControl5 = 3.0;
        this->ui->control6Label->setEnabled(true);
        this->ui->control6LineEdit->setEnabled(true);
        this->ui->control6Label->setText("Deriv. Mult. :");
        this->ui->control6LineEdit->setText("1.0");
        m_dControl6 = 1.0;
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Nova (Special)")
    {
        this->ui->control1Label->setEnabled(true);
        this->ui->control1LineEdit->setEnabled(true);
        this->ui->control1Label->setText("Relax (Re) :");
        this->ui->control1LineEdit->setText("7.0");
        m_dControl1 = 7.0;
        this->ui->control2Label->setEnabled(true);
        this->ui->control2LineEdit->setEnabled(true);
        this->ui->control2Label->setText("Relax (Im) :");
        this->ui->control2LineEdit->setText("-3.0");
        m_dControl2 = -3.0;
        this->ui->control3Label->setEnabled(true);
        this->ui->control3LineEdit->setEnabled(true);
        this->ui->control3Label->setText("Power :");
        this->ui->control3LineEdit->setText("3.0");
        m_dControl3 = 3.0;
        this->ui->control4Label->setEnabled(true);
        this->ui->control4LineEdit->setEnabled(true);
        this->ui->control4Label->setText("Deriv. Mult. :");
        this->ui->control4LineEdit->setText("2.0");
        m_dControl4 = 2.0;
        this->ui->control5Label->setEnabled(false);
        this->ui->control5LineEdit->setEnabled(false);
        this->ui->control5LineEdit->setText("");
        this->ui->control6Label->setEnabled(false);
        this->ui->control6LineEdit->setEnabled(false);
        this->ui->control6LineEdit->setText("");
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Glynn")
    {
        this->ui->control1Label->setEnabled(true);
        this->ui->control1LineEdit->setEnabled(true);
        this->ui->control1Label->setText("Seed (Re) :");
        this->ui->control1LineEdit->setText("-0.2");
        m_dControl1 = -0.2;
        this->ui->control2Label->setEnabled(true);
        this->ui->control2LineEdit->setEnabled(true);
        this->ui->control2Label->setText("Seed (Im) :");
        this->ui->control2LineEdit->setText("0.0");
        m_dControl2 = 0.0;
        this->ui->control3Label->setEnabled(true);
        this->ui->control3LineEdit->setEnabled(true);
        this->ui->control3Label->setText("Power :");
        this->ui->control3LineEdit->setText("1.5");
        m_dControl3 = 1.5;
        this->ui->control4Label->setEnabled(false);
        this->ui->control4LineEdit->setEnabled(false);
        this->ui->control4LineEdit->setText("");
        this->ui->control5Label->setEnabled(false);
        this->ui->control5LineEdit->setEnabled(false);
        this->ui->control5LineEdit->setText("");
        this->ui->control6Label->setEnabled(false);
        this->ui->control6LineEdit->setEnabled(false);
        this->ui->control6LineEdit->setText("");
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Phoenix")
    {
        this->ui->control1Label->setEnabled(true);
        this->ui->control1LineEdit->setEnabled(true);
        this->ui->control1Label->setText("P1 (Re) :");
        this->ui->control1LineEdit->setText("0.56667");
        m_dControl1 = 0.56667;
        this->ui->control2Label->setEnabled(true);
        this->ui->control2LineEdit->setEnabled(true);
        this->ui->control2Label->setText("P1 (Im) :");
        this->ui->control2LineEdit->setText("0.0");
        m_dControl2 = 0.0;
        this->ui->control3Label->setEnabled(true);
        this->ui->control3LineEdit->setEnabled(true);
        this->ui->control3Label->setText("P2 (Re) :");
        this->ui->control3LineEdit->setText("0.0");
        m_dControl3 = 0.0;
        this->ui->control4Label->setEnabled(true);
        this->ui->control4LineEdit->setEnabled(true);
        this->ui->control4Label->setText("P2 (Im) :");
        this->ui->control4LineEdit->setText("-0.5");
        m_dControl4 = -0.5;
        this->ui->control5Label->setEnabled(true);
        this->ui->control5LineEdit->setEnabled(true);
        this->ui->control5Label->setText("Power :");
        this->ui->control5LineEdit->setText("2.0");
        m_dControl5 = 2.0;
        this->ui->control6Label->setEnabled(false);
        this->ui->control6LineEdit->setEnabled(false);
        this->ui->control6LineEdit->setText("");
    }

    for(int n = 0; n < TRIGGER_POINTS_NUMBER; n++)
    {
        m_dAnimCtrl1[n] = m_dControl1;
        m_dAnimCtrl2[n] = m_dControl2;
        m_dAnimCtrl3[n] = m_dControl3;
        m_dAnimCtrl4[n] = m_dControl4;
        m_dAnimCtrl5[n] = m_dControl5;
        m_dAnimCtrl6[n] = m_dControl6;
    }
}

void Fractoria::on_outsideRedColorAdderLineEdit_textChanged(const QString &arg1)
{
    m_iOutsideRedColorAdder = QString(arg1).toInt();
}

void Fractoria::on_outsideGreenColorAdderLineEdit_textChanged(const QString &arg1)
{
    m_iOutsideGreenColorAdder = QString(arg1).toInt();
}

void Fractoria::on_outsideBlueColorAdderLineEdit_textChanged(const QString &arg1)
{
    m_iOutsideBlueColorAdder = QString(arg1).toInt();
}

void Fractoria::on_outsideRedColorMultiplierLineEdit_textChanged(const QString &arg1)
{
    m_dOutsideRedColorMultiplier = QString(arg1).toDouble();
}

void Fractoria::on_outsideGreenColorMultiplierLineEdit_textChanged(const QString &arg1)
{
    m_dOutsideGreenColorMultiplier = QString(arg1).toDouble();
}

void Fractoria::on_outsideBlueColorMultiplierLineEdit_textChanged(const QString &arg1)
{
    m_dOutsideBlueColorMultiplier = QString(arg1).toDouble();
}

void Fractoria::outsideColoringFunction()
{
    if(m_iColoringAlgorithm != 1)
    {
        this->ui->outsideRedColorAdderLineEdit->setEnabled(true);
        this->ui->outsideGreenColorAdderLineEdit->setEnabled(true);
        this->ui->outsideBlueColorAdderLineEdit->setEnabled(true);
        this->ui->outsideRedColorMultiplierLineEdit->setEnabled(true);
        this->ui->outsideGreenColorMultiplierLineEdit->setEnabled(true);
        this->ui->outsideBlueColorMultiplierLineEdit->setEnabled(true);

        this->ui->outsideColorPalette->setEnabled(true);

        this->ui->outsideRedColorAdderLineEdit->setText("255");
        m_iOutsideRedColorAdder = 255;
        this->ui->outsideGreenColorAdderLineEdit->setText("255");
        m_iOutsideGreenColorAdder = 255;
        this->ui->outsideBlueColorAdderLineEdit->setText("255");
        m_iOutsideBlueColorAdder = 255;
        this->ui->outsideRedColorMultiplierLineEdit->setText("1.0");
        m_dOutsideRedColorMultiplier = 1.0;
        this->ui->outsideGreenColorMultiplierLineEdit->setText("1.0");
        m_dOutsideGreenColorMultiplier = 1.0;
        this->ui->outsideBlueColorMultiplierLineEdit->setText("1.0");
        m_dOutsideBlueColorMultiplier = 1.0;

        if(this->ui->fractalTypeComboBox1->currentText() == "Mandelbrot (Quick)")
        {
            this->ui->outsideRedColorAdderLineEdit->setText("255");
            m_iOutsideRedColorAdder = 255;
            this->ui->outsideGreenColorAdderLineEdit->setText("255");
            m_iOutsideGreenColorAdder = 255;
            this->ui->outsideBlueColorAdderLineEdit->setText("255");
            m_iOutsideBlueColorAdder = 255;
            this->ui->outsideRedColorMultiplierLineEdit->setText("1.0");
            m_dOutsideRedColorMultiplier = 1.0;
            this->ui->outsideGreenColorMultiplierLineEdit->setText("1.0");
            m_dOutsideGreenColorMultiplier = 1.0;
            this->ui->outsideBlueColorMultiplierLineEdit->setText("1.0");
            m_dOutsideBlueColorMultiplier = 1.0;
        }
        else if(this->ui->fractalTypeComboBox1->currentText() == "Mandelbrot")
        {
            this->ui->outsideRedColorAdderLineEdit->setText("255");
            m_iOutsideRedColorAdder = 255;
            this->ui->outsideGreenColorAdderLineEdit->setText("255");
            m_iOutsideGreenColorAdder = 255;
            this->ui->outsideBlueColorAdderLineEdit->setText("255");
            m_iOutsideBlueColorAdder = 255;
            this->ui->outsideRedColorMultiplierLineEdit->setText("1.0");
            m_dOutsideRedColorMultiplier = 1.0;
            this->ui->outsideGreenColorMultiplierLineEdit->setText("1.0");
            m_dOutsideGreenColorMultiplier = 1.0;
            this->ui->outsideBlueColorMultiplierLineEdit->setText("1.0");
            m_dOutsideBlueColorMultiplier = 1.0;
        }
        else if(this->ui->fractalTypeComboBox1->currentText() == "Julia (Quick)")
        {
            this->ui->outsideRedColorAdderLineEdit->setText("255");
            m_iOutsideRedColorAdder = 255;
            this->ui->outsideGreenColorAdderLineEdit->setText("255");
            m_iOutsideGreenColorAdder = 255;
            this->ui->outsideBlueColorAdderLineEdit->setText("255");
            m_iOutsideBlueColorAdder = 255;
            this->ui->outsideRedColorMultiplierLineEdit->setText("1.0");
            m_dOutsideRedColorMultiplier = 1.0;
            this->ui->outsideGreenColorMultiplierLineEdit->setText("1.0");
            m_dOutsideGreenColorMultiplier = 1.0;
            this->ui->outsideBlueColorMultiplierLineEdit->setText("1.0");
            m_dOutsideBlueColorMultiplier = 1.0;
        }
        else if(this->ui->fractalTypeComboBox1->currentText() == "Julia")
        {
            this->ui->outsideRedColorAdderLineEdit->setText("255");
            m_iOutsideRedColorAdder = 255;
            this->ui->outsideGreenColorAdderLineEdit->setText("255");
            m_iOutsideGreenColorAdder = 255;
            this->ui->outsideBlueColorAdderLineEdit->setText("255");
            m_iOutsideBlueColorAdder = 255;
            this->ui->outsideRedColorMultiplierLineEdit->setText("1.0");
            m_dOutsideRedColorMultiplier = 1.0;
            this->ui->outsideGreenColorMultiplierLineEdit->setText("1.0");
            m_dOutsideGreenColorMultiplier = 1.0;
            this->ui->outsideBlueColorMultiplierLineEdit->setText("1.0");
            m_dOutsideBlueColorMultiplier = 1.0;
        }
        else if(this->ui->fractalTypeComboBox1->currentText() == "Newton")
        {
            this->ui->outsideRedColorAdderLineEdit->setText("255");
            m_iOutsideRedColorAdder = 255;
            this->ui->outsideGreenColorAdderLineEdit->setText("255");
            m_iOutsideGreenColorAdder = 255;
            this->ui->outsideBlueColorAdderLineEdit->setText("255");
            m_iOutsideBlueColorAdder = 255;
            this->ui->outsideRedColorMultiplierLineEdit->setText("1.0");
            m_dOutsideRedColorMultiplier = 1.0;
            this->ui->outsideGreenColorMultiplierLineEdit->setText("1.0");
            m_dOutsideGreenColorMultiplier = 1.0;
            this->ui->outsideBlueColorMultiplierLineEdit->setText("1.0");
            m_dOutsideBlueColorMultiplier = 1.0;
        }
        else if(this->ui->fractalTypeComboBox1->currentText() == "Newton (Root)")
        {
            this->ui->outsideRedColorAdderLineEdit->setEnabled(false);
            this->ui->outsideRedColorAdderLineEdit->setText("N/A");
            this->ui->outsideGreenColorAdderLineEdit->setEnabled(false);
            this->ui->outsideGreenColorAdderLineEdit->setText("N/A");
            this->ui->outsideBlueColorAdderLineEdit->setEnabled(false);
            this->ui->outsideBlueColorAdderLineEdit->setText("N/A");
            this->ui->outsideRedColorMultiplierLineEdit->setEnabled(false);
            this->ui->outsideRedColorMultiplierLineEdit->setText("N/A");
            this->ui->outsideGreenColorMultiplierLineEdit->setEnabled(false);
            this->ui->outsideGreenColorMultiplierLineEdit->setText("N/A");
            this->ui->outsideBlueColorMultiplierLineEdit->setEnabled(false);
            this->ui->outsideBlueColorMultiplierLineEdit->setText("N/A");

            this->ui->outsideColorPalette->setEnabled(false);
        }
        else if(this->ui->fractalTypeComboBox1->currentText() == "Nova")
        {
            this->ui->outsideRedColorAdderLineEdit->setText("255");
            m_iOutsideRedColorAdder = 255;
            this->ui->outsideGreenColorAdderLineEdit->setText("255");
            m_iOutsideGreenColorAdder = 255;
            this->ui->outsideBlueColorAdderLineEdit->setText("255");
            m_iOutsideBlueColorAdder = 255;
            this->ui->outsideRedColorMultiplierLineEdit->setText("1.0");
            m_dOutsideRedColorMultiplier = 1.0;
            this->ui->outsideGreenColorMultiplierLineEdit->setText("1.0");
            m_dOutsideGreenColorMultiplier = 1.0;
            this->ui->outsideBlueColorMultiplierLineEdit->setText("1.0");
            m_dOutsideBlueColorMultiplier = 1.0;
        }
        else if(this->ui->fractalTypeComboBox1->currentText() == "Nova (Special)")
        {
            this->ui->outsideRedColorAdderLineEdit->setText("255");
            m_iOutsideRedColorAdder = 255;
            this->ui->outsideGreenColorAdderLineEdit->setText("255");
            m_iOutsideGreenColorAdder = 255;
            this->ui->outsideBlueColorAdderLineEdit->setText("255");
            m_iOutsideBlueColorAdder = 255;
            this->ui->outsideRedColorMultiplierLineEdit->setText("1.0");
            m_dOutsideRedColorMultiplier = 1.0;
            this->ui->outsideGreenColorMultiplierLineEdit->setText("1.0");
            m_dOutsideGreenColorMultiplier = 1.0;
            this->ui->outsideBlueColorMultiplierLineEdit->setText("1.0");
            m_dOutsideBlueColorMultiplier = 1.0;
        }
        else if(this->ui->fractalTypeComboBox1->currentText() == "Glynn")
        {
            this->ui->outsideRedColorAdderLineEdit->setText("255");
            m_iOutsideRedColorAdder = 255;
            this->ui->outsideGreenColorAdderLineEdit->setText("255");
            m_iOutsideGreenColorAdder = 255;
            this->ui->outsideBlueColorAdderLineEdit->setText("255");
            m_iOutsideBlueColorAdder = 255;
            this->ui->outsideRedColorMultiplierLineEdit->setText("1.0");
            m_dOutsideRedColorMultiplier = 1.0;
            this->ui->outsideGreenColorMultiplierLineEdit->setText("1.0");
            m_dOutsideGreenColorMultiplier = 1.0;
            this->ui->outsideBlueColorMultiplierLineEdit->setText("1.0");
            m_dOutsideBlueColorMultiplier = 1.0;
        }
        else if(this->ui->fractalTypeComboBox1->currentText() == "Phoenix")
        {
            this->ui->outsideRedColorAdderLineEdit->setText("255");
            m_iOutsideRedColorAdder = 255;
            this->ui->outsideGreenColorAdderLineEdit->setText("255");
            m_iOutsideGreenColorAdder = 255;
            this->ui->outsideBlueColorAdderLineEdit->setText("255");
            m_iOutsideBlueColorAdder = 255;
            this->ui->outsideRedColorMultiplierLineEdit->setText("1.0");
            m_dOutsideRedColorMultiplier = 1.0;
            this->ui->outsideGreenColorMultiplierLineEdit->setText("1.0");
            m_dOutsideGreenColorMultiplier = 1.0;
            this->ui->outsideBlueColorMultiplierLineEdit->setText("1.0");
            m_dOutsideBlueColorMultiplier = 1.0;
        }
    }

    for(int n = 0; n < TRIGGER_POINTS_NUMBER; n++)
    {
        m_iAnimOutsideRedAdder[n] = m_iOutsideRedColorAdder;
        m_iAnimOutsideGreenAdder[n] = m_iOutsideGreenColorAdder;
        m_iAnimOutsideBlueAdder[n] = m_iOutsideBlueColorAdder;
        m_dAnimOutsideRedMultiplier[n] = m_dOutsideRedColorMultiplier;
        m_dAnimOutsideGreenMultiplier[n] = m_dOutsideGreenColorMultiplier;
        m_dAnimOutsideBlueMultiplier[n] = m_dOutsideBlueColorMultiplier;
    }
}

void Fractoria::on_insideRedColorAdderLineEdit_textChanged(const QString &arg1)
{
    m_iInsideRedColorAdder = QString(arg1).toInt();
}

void Fractoria::on_insideGreenColorAdderLineEdit_textChanged(const QString &arg1)
{
    m_iInsideGreenColorAdder = QString(arg1).toInt();
}

void Fractoria::on_insideBlueColorAdderLineEdit_textChanged(const QString &arg1)
{
    m_iInsideBlueColorAdder = QString(arg1).toInt();
}

void Fractoria::on_insideRedColorMultiplierLineEdit_textChanged(const QString &arg1)
{
    m_dInsideRedColorMultiplier = QString(arg1).toDouble();
}

void Fractoria::on_insideGreenColorMultiplierLineEdit_textChanged(const QString &arg1)
{
    m_dInsideGreenColorMultiplier = QString(arg1).toDouble();
}

void Fractoria::on_insideBlueColorMultiplierLineEdit_textChanged(const QString &arg1)
{
    m_dInsideBlueColorMultiplier = QString(arg1).toDouble();
}

void Fractoria::insideColoringFunction()
{
    if(m_iColoringAlgorithm != 1)
    {
        this->ui->insideRedColorAdderLineEdit->setEnabled(true);
        this->ui->insideGreenColorAdderLineEdit->setEnabled(true);
        this->ui->insideBlueColorAdderLineEdit->setEnabled(true);
        this->ui->insideRedColorMultiplierLineEdit->setEnabled(true);
        this->ui->insideGreenColorMultiplierLineEdit->setEnabled(true);
        this->ui->insideBlueColorMultiplierLineEdit->setEnabled(true);

        this->ui->insideColorPalette->setEnabled(true);

        this->ui->insideRedColorAdderLineEdit->setText("0");
        m_iInsideRedColorAdder = 0;
        this->ui->insideGreenColorAdderLineEdit->setText("0");
        m_iInsideGreenColorAdder = 0;
        this->ui->insideBlueColorAdderLineEdit->setText("0");
        m_iInsideBlueColorAdder = 0;
        this->ui->insideRedColorMultiplierLineEdit->setText("1.0");
        m_dInsideRedColorMultiplier = 1.0;
        this->ui->insideGreenColorMultiplierLineEdit->setText("1.0");
        m_dInsideGreenColorMultiplier = 1.0;
        this->ui->insideBlueColorMultiplierLineEdit->setText("1.0");
        m_dInsideBlueColorMultiplier = 1.0;

        if(this->ui->fractalTypeComboBox1->currentText() == "Mandelbrot (Quick)")
        {
            this->ui->insideRedColorAdderLineEdit->setText("0");
            m_iInsideRedColorAdder = 0;
            this->ui->insideGreenColorAdderLineEdit->setText("0");
            m_iInsideGreenColorAdder = 0;
            this->ui->insideBlueColorAdderLineEdit->setText("0");
            m_iInsideBlueColorAdder = 0;
            this->ui->insideRedColorMultiplierLineEdit->setText("1.0");
            m_dInsideRedColorMultiplier = 1.0;
            this->ui->insideGreenColorMultiplierLineEdit->setText("1.0");
            m_dInsideGreenColorMultiplier = 1.0;
            this->ui->insideBlueColorMultiplierLineEdit->setText("1.0");
            m_dInsideBlueColorMultiplier = 1.0;
        }
        else if(this->ui->fractalTypeComboBox1->currentText() == "Mandelbrot")
        {
            this->ui->insideRedColorAdderLineEdit->setText("0");
            m_iInsideRedColorAdder = 0;
            this->ui->insideGreenColorAdderLineEdit->setText("0");
            m_iInsideGreenColorAdder = 0;
            this->ui->insideBlueColorAdderLineEdit->setText("0");
            m_iInsideBlueColorAdder = 0;
            this->ui->insideRedColorMultiplierLineEdit->setText("1.0");
            m_dInsideRedColorMultiplier = 1.0;
            this->ui->insideGreenColorMultiplierLineEdit->setText("1.0");
            m_dInsideGreenColorMultiplier = 1.0;
            this->ui->insideBlueColorMultiplierLineEdit->setText("1.0");
            m_dInsideBlueColorMultiplier = 1.0;
        }
        else if(this->ui->fractalTypeComboBox1->currentText() == "Julia (Quick)")
        {
            this->ui->insideRedColorAdderLineEdit->setText("0");
            m_iInsideRedColorAdder = 0;
            this->ui->insideGreenColorAdderLineEdit->setText("0");
            m_iInsideGreenColorAdder = 0;
            this->ui->insideBlueColorAdderLineEdit->setText("0");
            m_iInsideBlueColorAdder = 0;
            this->ui->insideRedColorMultiplierLineEdit->setText("1.0");
            m_dInsideRedColorMultiplier = 1.0;
            this->ui->insideGreenColorMultiplierLineEdit->setText("1.0");
            m_dInsideGreenColorMultiplier = 1.0;
            this->ui->insideBlueColorMultiplierLineEdit->setText("1.0");
            m_dInsideBlueColorMultiplier = 1.0;
        }
        else if(this->ui->fractalTypeComboBox1->currentText() == "Julia")
        {
            this->ui->insideRedColorAdderLineEdit->setText("0");
            m_iInsideRedColorAdder = 0;
            this->ui->insideGreenColorAdderLineEdit->setText("0");
            m_iInsideGreenColorAdder = 0;
            this->ui->insideBlueColorAdderLineEdit->setText("0");
            m_iInsideBlueColorAdder = 0;
            this->ui->insideRedColorMultiplierLineEdit->setText("1.0");
            m_dInsideRedColorMultiplier = 1.0;
            this->ui->insideGreenColorMultiplierLineEdit->setText("1.0");
            m_dInsideGreenColorMultiplier = 1.0;
            this->ui->insideBlueColorMultiplierLineEdit->setText("1.0");
            m_dInsideBlueColorMultiplier = 1.0;
        }
        else if(this->ui->fractalTypeComboBox1->currentText() == "Newton")
        {
            this->ui->insideRedColorAdderLineEdit->setText("0");
            m_iInsideRedColorAdder = 0;
            this->ui->insideGreenColorAdderLineEdit->setText("0");
            m_iInsideGreenColorAdder = 0;
            this->ui->insideBlueColorAdderLineEdit->setText("0");
            m_iInsideBlueColorAdder = 0;
            this->ui->insideRedColorMultiplierLineEdit->setText("1.0");
            m_dInsideRedColorMultiplier = 1.0;
            this->ui->insideGreenColorMultiplierLineEdit->setText("1.0");
            m_dInsideGreenColorMultiplier = 1.0;
            this->ui->insideBlueColorMultiplierLineEdit->setText("1.0");
            m_dInsideBlueColorMultiplier = 1.0;
        }
        else if(this->ui->fractalTypeComboBox1->currentText() == "Newton (Root)")
        {
            this->ui->insideRedColorAdderLineEdit->setEnabled(false);
            this->ui->insideRedColorAdderLineEdit->setText("N/A");
            this->ui->insideGreenColorAdderLineEdit->setEnabled(false);
            this->ui->insideGreenColorAdderLineEdit->setText("N/A");
            this->ui->insideBlueColorAdderLineEdit->setEnabled(false);
            this->ui->insideBlueColorAdderLineEdit->setText("N/A");

            this->ui->insideRedColorMultiplierLineEdit->setEnabled(false);
            this->ui->insideRedColorMultiplierLineEdit->setText("N/A");
            this->ui->insideGreenColorMultiplierLineEdit->setEnabled(false);
            this->ui->insideGreenColorMultiplierLineEdit->setText("N/A");
            this->ui->insideBlueColorMultiplierLineEdit->setEnabled(false);
            this->ui->insideBlueColorMultiplierLineEdit->setText("N/A");

            this->ui->insideColorPalette->setEnabled(false);
        }
        else if(this->ui->fractalTypeComboBox1->currentText() == "Nova")
        {
            this->ui->insideRedColorAdderLineEdit->setText("0");
            m_iInsideRedColorAdder = 0;
            this->ui->insideGreenColorAdderLineEdit->setText("0");
            m_iInsideGreenColorAdder = 0;
            this->ui->insideBlueColorAdderLineEdit->setText("0");
            m_iInsideBlueColorAdder = 0;
            this->ui->insideRedColorMultiplierLineEdit->setText("1.0");
            m_dInsideRedColorMultiplier = 1.0;
            this->ui->insideGreenColorMultiplierLineEdit->setText("1.0");
            m_dInsideGreenColorMultiplier = 1.0;
            this->ui->insideBlueColorMultiplierLineEdit->setText("1.0");
            m_dInsideBlueColorMultiplier = 1.0;
        }
        else if(this->ui->fractalTypeComboBox1->currentText() == "Nova (Special)")
        {
            this->ui->insideRedColorAdderLineEdit->setText("0");
            m_iInsideRedColorAdder = 0;
            this->ui->insideGreenColorAdderLineEdit->setText("0");
            m_iInsideGreenColorAdder = 0;
            this->ui->insideBlueColorAdderLineEdit->setText("0");
            m_iInsideBlueColorAdder = 0;
            this->ui->insideRedColorMultiplierLineEdit->setText("1.0");
            m_dInsideRedColorMultiplier = 1.0;
            this->ui->insideGreenColorMultiplierLineEdit->setText("1.0");
            m_dInsideGreenColorMultiplier = 1.0;
            this->ui->insideBlueColorMultiplierLineEdit->setText("1.0");
            m_dInsideBlueColorMultiplier = 1.0;
        }
        else if(this->ui->fractalTypeComboBox1->currentText() == "Glynn")
        {
            this->ui->insideRedColorAdderLineEdit->setText("0");
            m_iInsideRedColorAdder = 0;
            this->ui->insideGreenColorAdderLineEdit->setText("0");
            m_iInsideGreenColorAdder = 0;
            this->ui->insideBlueColorAdderLineEdit->setText("0");
            m_iInsideBlueColorAdder = 0;
            this->ui->insideRedColorMultiplierLineEdit->setText("1.0");
            m_dInsideRedColorMultiplier = 1.0;
            this->ui->insideGreenColorMultiplierLineEdit->setText("1.0");
            m_dInsideGreenColorMultiplier = 1.0;
            this->ui->insideBlueColorMultiplierLineEdit->setText("1.0");
            m_dInsideBlueColorMultiplier = 1.0;
        }
        else if(this->ui->fractalTypeComboBox1->currentText() == "Phoenix")
        {
            this->ui->insideRedColorAdderLineEdit->setText("0");
            m_iInsideRedColorAdder = 0;
            this->ui->insideGreenColorAdderLineEdit->setText("0");
            m_iInsideGreenColorAdder = 0;
            this->ui->insideBlueColorAdderLineEdit->setText("0");
            m_iInsideBlueColorAdder = 0;
            this->ui->insideRedColorMultiplierLineEdit->setText("1.0");
            m_dInsideRedColorMultiplier = 1.0;
            this->ui->insideGreenColorMultiplierLineEdit->setText("1.0");
            m_dInsideGreenColorMultiplier = 1.0;
            this->ui->insideBlueColorMultiplierLineEdit->setText("1.0");
            m_dInsideBlueColorMultiplier = 1.0;
        }
    }

    for(int n = 0; n < TRIGGER_POINTS_NUMBER; n++)
    {
        m_iAnimInsideRedAdder[n] = m_iInsideRedColorAdder;
        m_iAnimInsideGreenAdder[n] = m_iInsideGreenColorAdder;
        m_iAnimInsideBlueAdder[n] = m_iInsideBlueColorAdder;
        m_dAnimInsideRedMultiplier[n] = m_dInsideRedColorMultiplier;
        m_dAnimInsideGreenMultiplier[n] = m_dInsideGreenColorMultiplier;
        m_dAnimInsideBlueMultiplier[n] = m_dInsideBlueColorMultiplier;
    }
}

void Fractoria::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return)
    {
        on_renderButton_clicked();
    }

    if(event->key() == Qt::Key_F9)
    {
        preview();
    }
}

void Fractoria::preview()
{
    if(this->ui->fractalTypeComboBox1->currentText() == "Mandelbrot (Quick)")
    {
        if(m_bSwitchModeFractal == true)
        {
            previewJuliaQuick();
        }
        else
        {
            previewMandelbrotQuick();
        }
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Mandelbrot")
    {
        if(m_bSwitchModeFractal == true)
        {
            previewJulia();
        }
        else
        {
            previewMandelbrot();
        }
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Julia (Quick)")
    {
        previewJuliaQuick();
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Julia")
    {
        previewJulia();
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Newton")
    {
        previewNewton();
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Newton (Root)")
    {
        previewNewtonRoot();
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Nova")
    {
        if(m_bSwitchModeFractal == true)
        {
            previewNovaSpecial();
        }
        else
        {
            previewNova();
        }
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Nova (Special)")
    {
        previewNovaSpecial();
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Glynn")
    {
        previewGlynn();
    }
    else if(this->ui->fractalTypeComboBox1->currentText() == "Phoenix")
    {
        previewPhoenix();
    }
}

void Fractoria::previewMandelbrotQuick()
{
    imgPreview = QImage(180, 120, QImage::Format_ARGB32);

    double a, b, d, q, t, tBU = 0.0;
    double x, y;

    int xPix = 0, yPix = 0;
    double bailout = 4.0;

    double xStart, yStart;
    double hyp, hx, hy = 0.0;
    double theta;

    int iOutsideRed = 255, iOutsideGreen = 255, iOutsideBlue = 255;
    int iInsideRed = 0, iInsideGreen = 0, iInsideBlue = 0;

    int width = 180;
    int height = 120;


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
                    imgPreview.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    imgPreview.setPixel(xPix, yPix, outsideColor.rgba());
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
                    imgPreview.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    imgPreview.setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;
    }

    this->ui->previewLabel->setPixmap(QPixmap::fromImage(imgPreview));
}

void Fractoria::previewMandelbrot()
{
    imgPreview = QImage(180, 120, QImage::Format_ARGB32);

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

    int width = 180;
    int height = 120;

    a = m_dControl1;
    b = m_dControl2;
    power = m_dControl3;

/*
    qDebug() << m_iColoringAlgorithm;
    qDebug() << m_iAlgorithmAttributes;
    qDebug() << m_dRedMultiplier;
    qDebug() << m_dGreenMultiplier;
    qDebug() << m_dBlueMultiplier;
*/

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

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%255;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%255;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%255;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%255);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%255);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%255);}

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
                    imgPreview.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    imgPreview.setPixel(xPix, yPix, outsideColor.rgba());
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

                if(iInsideRed > 255) {iInsideRed = iInsideRed%255;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%255;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%255;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%255);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%255);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%255);}


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
                    imgPreview.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    imgPreview.setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;
    }

    this->ui->previewLabel->setPixmap(QPixmap::fromImage(imgPreview));
}

void Fractoria::previewJuliaQuick()
{
    imgPreview = QImage(180, 120, QImage::Format_ARGB32_Premultiplied);

    double a, b, d, e, f, q, t, tBU = 0.0;
    double x, y;

    int xPix = 0, yPix = 0;
    double bailout = 0.0;

    double xStart, yStart;
    double hyp, hx, hy = 0.0;
    double theta;

    int iOutsideRed = 255, iOutsideGreen = 255, iOutsideBlue = 255;
    int iInsideRed = 0, iInsideGreen = 0, iInsideBlue = 0;

    int width = 180;
    int height = 120;

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

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%255;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%255;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%255;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%255);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%255);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%255);}

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
                    imgPreview.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    imgPreview.setPixel(xPix, yPix, outsideColor.rgba());
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

                if(iInsideRed > 255) {iInsideRed = iInsideRed%255;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%255;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%255;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%255);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%255);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%255);}


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
                    imgPreview.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    imgPreview.setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;
    }

    this->ui->previewLabel->setPixmap(QPixmap::fromImage(imgPreview));
}

void Fractoria::previewJulia()
{
    imgPreview = QImage(180, 120, QImage::Format_ARGB32);

    double a, b, t, tBU = 0, x, y, power;

    int xPix, yPix;
    double bailout = 4.0;

    int iOutsideRed = 255, iOutsideGreen = 255, iOutsideBlue = 255;
    int iInsideRed = 0, iInsideGreen = 0, iInsideBlue = 0;

    int width = 180;
    int height = 120;

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

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%255;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%255;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%255;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%255);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%255);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%255);}

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
                    imgPreview.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    imgPreview.setPixel(xPix, yPix, outsideColor.rgba());
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

                if(iInsideRed > 255) {iInsideRed = iInsideRed%255;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%255;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%255;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%255);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%255);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%255);}


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
                    imgPreview.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    imgPreview.setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;
    }

    this->ui->previewLabel->setPixmap(QPixmap::fromImage(imgPreview));
}

void Fractoria::previewNewton()
{
    imgPreview = QImage(180, 120, QImage::Format_ARGB32);

    double t, tBU = 0.0, x, y, power = 3.0;
    double dRelaxReal, dRelaxImag;

    int xPix, yPix;
    double bailout = 4.0;

    int iOutsideRed = 255, iOutsideGreen = 255, iOutsideBlue = 255;
    int iInsideRed = 0, iInsideGreen = 0, iInsideBlue = 0;

    int width = 180;
    int height = 120;

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
                    imgPreview.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    imgPreview.setPixel(xPix, yPix, outsideColor.rgba());
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
                    imgPreview.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    imgPreview.setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;
    }

    this->ui->previewLabel->setPixmap(QPixmap::fromImage(imgPreview));
}

void Fractoria::previewNewtonRoot()
{
    imgPreview = QImage(180, 120, QImage::Format_ARGB32);

    double t, x, y;

    int xPix, yPix;

//            double bailout = 4.0;

//            int iInsideRed, iInsideGreen, iInsideBlue;

    double xStart, yStart;
    double hyp, hx, hy = 0.0;
    double theta;

    complex<double> z;

    int width = 180;
    int height = 120;

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

//                        bailout = abs(z);

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
                imgPreview.setPixel(xPix, yPix, redColor.rgba());
            }

            if(abs(z - zGreen) <= tol)
            {
                imgPreview.setPixel(xPix, yPix, greenColor.rgba());
            }

            if(abs(z - zBlue) <= tol)
            {
                imgPreview.setPixel(xPix, yPix, blueColor.rgba());
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;
    }

    this->ui->previewLabel->setPixmap(QPixmap::fromImage(imgPreview));
}

void Fractoria::previewNova()
{
    imgPreview = QImage(180, 120, QImage::Format_ARGB32);

    double t, tBU = 0.0, x, y;

    int xPix, yPix;
    double bailout = 4.0;

    int iOutsideRed = 255, iOutsideGreen = 255, iOutsideBlue = 255;
    int iInsideRed = 0, iInsideGreen = 0, iInsideBlue = 0;

    int width = 180;
    int height = 120;

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

                if(iOutsideRed > 255) {iOutsideRed = iOutsideRed%255;}
                if(iOutsideGreen > 255) {iOutsideGreen = iOutsideGreen%255;}
                if(iOutsideBlue > 255) {iOutsideBlue = iOutsideBlue%255;}

                if(iOutsideRed < 0) {iOutsideRed = -(iOutsideRed%255);}
                if(iOutsideGreen < 0) {iOutsideGreen = -(iOutsideGreen%255);}
                if(iOutsideBlue < 0) {iOutsideBlue = -(iOutsideBlue%255);}

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
                    imgPreview.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    imgPreview.setPixel(xPix, yPix, outsideColor.rgba());
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

                if(iInsideRed > 255) {iInsideRed = iInsideRed%255;}
                if(iInsideGreen > 255) {iInsideGreen = iInsideGreen%255;}
                if(iInsideBlue > 255) {iInsideBlue = iInsideBlue%255;}

                if(iInsideRed < 0) {iInsideRed = -(iInsideRed%255);}
                if(iInsideGreen < 0) {iInsideGreen = -(iInsideGreen%255);}
                if(iInsideBlue < 0) {iInsideBlue = -(iInsideBlue%255);}


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
                    imgPreview.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    imgPreview.setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;
    }

    this->ui->previewLabel->setPixmap(QPixmap::fromImage(imgPreview));
}

void Fractoria::previewNovaSpecial()
{
    imgPreview = QImage(180, 120, QImage::Format_ARGB32);

    double t, tBU = 0.0, x, y;

    int xPix, yPix;
    double bailout = 4.0;

    int iOutsideRed = 255, iOutsideGreen = 255, iOutsideBlue = 255;
    int iInsideRed = 0, iInsideGreen = 0, iInsideBlue = 0;

    int width = 180;
    int height = 120;

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
                    imgPreview.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    imgPreview.setPixel(xPix, yPix, outsideColor.rgba());
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
                    imgPreview.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    imgPreview.setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;
    }

    this->ui->previewLabel->setPixmap(QPixmap::fromImage(imgPreview));
}

void Fractoria::previewGlynn()
{
    imgPreview = QImage(180, 120, QImage::Format_ARGB32);

    double a, b, t, tBU = 0, x, y, power;

    int xPix, yPix;
    double bailout = 4.0;

    int iOutsideRed = 255, iOutsideGreen = 255, iOutsideBlue = 255;
    int iInsideRed = 0, iInsideGreen = 0, iInsideBlue = 0;

    int width = 180;
    int height = 120;

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
                    imgPreview.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    imgPreview.setPixel(xPix, yPix, outsideColor.rgba());
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
                    imgPreview.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    imgPreview.setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;
    }

    this->ui->previewLabel->setPixmap(QPixmap::fromImage(imgPreview));
}

void Fractoria::previewPhoenix()
{
    imgPreview = QImage(180, 120, QImage::Format_ARGB32);

    double t, tBU = 0, x, y, power;

    int xPix, yPix;
    double bailout = 4.0;

    int iOutsideRed = 255, iOutsideGreen = 255, iOutsideBlue = 255;
    int iInsideRed = 0, iInsideGreen = 0, iInsideBlue = 0;

    int width = 180;
    int height = 120;

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
                    imgPreview.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    imgPreview.setPixel(xPix, yPix, outsideColor.rgba());
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
                    imgPreview.setPixel(xPix, yPix, insideColor.rgba());
                }
                else
                {
                    imgPreview.setPixel(xPix, yPix, outsideColor.rgba());
                }
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            hx = (6.0/(double)width)/m_dZoomRatio;

            x = x + hx*cos(m_dRotation*DEGtoRAD);
            y = y - hx*sin(m_dRotation*DEGtoRAD);
        }

        hy = hy + (4.0/(double)height)/m_dZoomRatio;
    }

    this->ui->previewLabel->setPixmap(QPixmap::fromImage(imgPreview));
}

void Fractoria::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if( (event->x() >= 10) && (event->x() < 910) )
        {
            if( (event->y() >= 15) && (event->y() < 615) )
            {
                m_dCenterX = m_dCoordX;
                m_dCenterY = m_dCoordY;

                this->ui->xCenterLineEdit->setText(QString::number(m_dCenterX));
                this->ui->yCenterLineEdit->setText(QString::number(m_dCenterY));
            }
        }
    }
    if(event->button() == Qt::RightButton)
    {
        if( (event->x() >= 10) && (event->x() < 910) )
        {
            if( (event->y() >= 15) && (event->y() < 615) )
            {
                if(m_bSwitchModeFractal == true)
                {
                    m_dControl1 = m_dCoordX;
                    m_dControl2 = m_dCoordY;

                    this->ui->control1LineEdit->setText(QString::number(m_dControl1));
                    this->ui->control2LineEdit->setText(QString::number(m_dControl2));

                    preview();
                }
            }
        }
    }
}

void Fractoria::on_outsideColorPalette_clicked()
{
    QColorDialog *colorDialog = new QColorDialog(this);

    QColor outsideColor = colorDialog->getColor(QColor(m_iOutsideRedColorAdder, m_iOutsideGreenColorAdder, m_iOutsideBlueColorAdder),
                                                this, "Outside Color Adder", 0);

    if(outsideColor.isValid())
    {
        m_iOutsideRedColorAdder = outsideColor.red();
        this->ui->outsideRedColorAdderLineEdit->setText(QString::number(m_iOutsideRedColorAdder));
        m_iOutsideGreenColorAdder = outsideColor.green();
        this->ui->outsideGreenColorAdderLineEdit->setText(QString::number(m_iOutsideGreenColorAdder));
        m_iOutsideBlueColorAdder = outsideColor.blue();
        this->ui->outsideBlueColorAdderLineEdit->setText(QString::number(m_iOutsideBlueColorAdder));
    }
}

void Fractoria::on_insideColorPalette_clicked()
{
    QColorDialog *colorDialog = new QColorDialog(this);

    QColor insideColor = colorDialog->getColor(QColor(m_iInsideRedColorAdder, m_iInsideGreenColorAdder, m_iInsideBlueColorAdder),
                                               this, "Inside Color Adder", 0);

    if(insideColor.isValid())
    {
        m_iInsideRedColorAdder = insideColor.red();
        this->ui->insideRedColorAdderLineEdit->setText(QString::number(m_iInsideRedColorAdder));
        m_iInsideGreenColorAdder = insideColor.green();
        this->ui->insideGreenColorAdderLineEdit->setText(QString::number(m_iInsideGreenColorAdder));
        m_iInsideBlueColorAdder = insideColor.blue();
        this->ui->insideBlueColorAdderLineEdit->setText(QString::number(m_iInsideBlueColorAdder));
    }
}

void Fractoria::coloringPropertiesFunction()
{
    if(this->ui->fractalTypeComboBox1->currentText() == "Newton (Root)")
    {
        newtonRootColoring = new NewtonRootColoring();

        newtonRootColoring->setNewtonRootColoringData(m_iFirstRootRed, m_iFirstRootGreen, m_iFirstRootBlue,
                                                      m_iSecondRootRed, m_iSecondRootGreen, m_iSecondRootBlue,
                                                      m_iThirdRootRed, m_iThirdRootGreen, m_iThirdRootBlue);

        newtonRootColoring->show();

        connect(newtonRootColoring, SIGNAL(finished(int,int,int,int,int,int,int,int,int)),
                this, SLOT(getNewtonColoringData(int,int,int,int,int,int,int,int,int)));
    }
    else
    {
        coloring = new Coloring();

        coloring->setColoringData(m_iColoringAlgorithm, m_iAlgorithmAttributes,
                                  m_dRedMultiplier, m_dGreenMultiplier, m_dBlueMultiplier,
                                  m_bRedMainColor, m_bGreenMainColor, m_bBlueMainColor);

        coloring->show();

        connect(coloring, SIGNAL(finished(int,int,double,double,double,bool,bool,bool)),
                this, SLOT(getColoringData(int,int,double,double,double,bool,bool,bool)));
    }
}

void Fractoria::getNewtonColoringData(int iFirstRootRed, int iFirstRootGreen, int iFirstRootBlue,
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

void Fractoria::getColoringData(int iColoringAlgorithm, int iAlgorithmAttributes,
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

    smoothNormalizationDefaultChanges();
}

void Fractoria::smoothNormalizationDefaultChanges()
{
    if(m_iColoringAlgorithm == 0) // Bailout Linear
    {
        m_iIteration = 25;
        this->ui->iterationsLineEdit->setText(QString::number(m_iIteration));

        m_iOutsideRedColorAdder = 255;
        this->ui->outsideRedColorAdderLineEdit->setText(QString::number(m_iOutsideRedColorAdder));
        m_iOutsideGreenColorAdder = 255;
        this->ui->outsideGreenColorAdderLineEdit->setText(QString::number(m_iOutsideGreenColorAdder));
        m_iOutsideBlueColorAdder = 255;
        this->ui->outsideBlueColorAdderLineEdit->setText(QString::number(m_iOutsideBlueColorAdder));
    }
    else if(m_iColoringAlgorithm == 1) // Smooth Normalization
    {
        if(this->ui->fractalTypeComboBox1->currentText() != "Newton"
                && this->ui->fractalTypeComboBox1->currentText() != "Nova (Special)"
                && this->ui->fractalTypeComboBox1->currentText() != "Glynn")
        {
            m_iIteration = 250;
            this->ui->iterationsLineEdit->setText(QString::number(m_iIteration));

            m_iOutsideRedColorAdder = 0;
            this->ui->outsideRedColorAdderLineEdit->setText(QString::number(m_iOutsideRedColorAdder));
            m_iOutsideGreenColorAdder = 0;
            this->ui->outsideGreenColorAdderLineEdit->setText(QString::number(m_iOutsideGreenColorAdder));
            m_iOutsideBlueColorAdder = 0;
            this->ui->outsideBlueColorAdderLineEdit->setText(QString::number(m_iOutsideBlueColorAdder));
        }

        if(this->ui->fractalTypeComboBox1->currentText() == "Nova (Special)"
                || this->ui->fractalTypeComboBox1->currentText() == "Glynn")
        {
            m_iOutsideRedColorAdder = 0;
            this->ui->outsideRedColorAdderLineEdit->setText(QString::number(m_iOutsideRedColorAdder));
            m_iOutsideGreenColorAdder = 0;
            this->ui->outsideGreenColorAdderLineEdit->setText(QString::number(m_iOutsideGreenColorAdder));
            m_iOutsideBlueColorAdder = 0;
            this->ui->outsideBlueColorAdderLineEdit->setText(QString::number(m_iOutsideBlueColorAdder));
        }
    }
}

void Fractoria::animationPropertiesFunction()
{
    if(firstTime == true)
    {
        // Clear Variables
        m_iTriggerFrame.clear();
        m_dEndCenterX.clear();
        m_dEndCenterY.clear();
        m_iTranslationFunction.clear();
        m_dEndZoomRatio.clear();
        m_iZoomRatioFunction.clear();
        m_dEndRotation.clear();
        m_iRotationFunction.clear();
        m_iEndIteration.clear();
        m_iIterationFunction.clear();

        m_dAnimCtrl1.clear();
        m_iAnimCtrl1Function.clear();
        m_dAnimCtrl2.clear();
        m_iAnimCtrl2Function.clear();
        m_dAnimCtrl3.clear();
        m_iAnimCtrl3Function.clear();
        m_dAnimCtrl4.clear();
        m_iAnimCtrl4Function.clear();
        m_dAnimCtrl5.clear();
        m_iAnimCtrl5Function.clear();
        m_dAnimCtrl6.clear();
        m_iAnimCtrl6Function.clear();

        m_iAnimOutsideRedAdder.clear();
        m_iAnimOutsideRedAddFunction.clear();
        m_iAnimOutsideGreenAdder.clear();
        m_iAnimOutsideGreenAddFunction.clear();
        m_iAnimOutsideBlueAdder.clear();
        m_iAnimOutsideBlueAddFunction.clear();
        m_dAnimOutsideRedMultiplier.clear();
        m_iAnimOutsideRedMultFunction.clear();
        m_dAnimOutsideGreenMultiplier.clear();
        m_iAnimOutsideGreenMultFunction.clear();
        m_dAnimOutsideBlueMultiplier.clear();
        m_iAnimOutsideBlueMultFunction.clear();

        m_iAnimInsideRedAdder.clear();
        m_iAnimInsideRedAddFunction.clear();
        m_iAnimInsideGreenAdder.clear();
        m_iAnimInsideGreenAddFunction.clear();
        m_iAnimInsideBlueAdder.clear();
        m_iAnimInsideBlueAddFunction.clear();
        m_dAnimInsideRedMultiplier.clear();
        m_iAnimInsideRedMultFunction.clear();
        m_dAnimInsideGreenMultiplier.clear();
        m_iAnimInsideGreenMultFunction.clear();
        m_dAnimInsideBlueMultiplier.clear();
        m_iAnimInsideBlueMultFunction.clear();


        // Init Variables
        m_iTotalFrameNumber = 1;

        for(int n = 0; n < TRIGGER_POINTS_NUMBER; n++)
        {
            m_iTriggerFrame << 0;
            m_dEndCenterX << m_dCenterX;
            m_dEndCenterY << m_dCenterY;
            m_iTranslationFunction << 0;
            m_dEndZoomRatio << m_dZoomRatio;
            m_iZoomRatioFunction << 0;
            m_dEndRotation << m_dRotation;
            m_iRotationFunction << 0;
            m_iEndIteration << m_iIteration;
            m_iIterationFunction << 0;

            m_dAnimCtrl1 << m_dControl1;
            m_iAnimCtrl1Function << 0;
            m_dAnimCtrl2 << m_dControl2;
            m_iAnimCtrl2Function << 0;
            m_dAnimCtrl3 << m_dControl3;
            m_iAnimCtrl3Function << 0;
            m_dAnimCtrl4 << m_dControl4;
            m_iAnimCtrl4Function << 0;
            m_dAnimCtrl5 << m_dControl5;
            m_iAnimCtrl5Function << 0;
            m_dAnimCtrl6 << m_dControl6;
            m_iAnimCtrl6Function << 0;

            m_iAnimOutsideRedAdder << m_iOutsideRedColorAdder;
            m_iAnimOutsideRedAddFunction << 0;
            m_iAnimOutsideGreenAdder << m_iOutsideGreenColorAdder;
            m_iAnimOutsideGreenAddFunction << 0;
            m_iAnimOutsideBlueAdder << m_iOutsideBlueColorAdder;
            m_iAnimOutsideBlueAddFunction << 0;
            m_dAnimOutsideRedMultiplier << m_dOutsideRedColorMultiplier;
            m_iAnimOutsideRedMultFunction << 0;
            m_dAnimOutsideGreenMultiplier << m_dOutsideGreenColorMultiplier;
            m_iAnimOutsideGreenMultFunction << 0;
            m_dAnimOutsideBlueMultiplier << m_dOutsideBlueColorMultiplier;
            m_iAnimOutsideBlueMultFunction << 0;

            m_iAnimInsideRedAdder << m_iInsideRedColorAdder;
            m_iAnimInsideRedAddFunction << 0;
            m_iAnimInsideGreenAdder << m_iInsideGreenColorAdder;
            m_iAnimInsideGreenAddFunction << 0;
            m_iAnimInsideBlueAdder << m_iInsideBlueColorAdder;
            m_iAnimInsideBlueAddFunction << 0;
            m_dAnimInsideRedMultiplier << m_dInsideRedColorMultiplier;
            m_iAnimInsideRedMultFunction << 0;
            m_dAnimInsideGreenMultiplier << m_dInsideGreenColorMultiplier;
            m_iAnimInsideGreenMultFunction << 0;
            m_dAnimInsideBlueMultiplier << m_dInsideBlueColorMultiplier;
            m_iAnimInsideBlueMultFunction << 0;
        }

        firstTime = false;
    }

    animationProperties = new AnimationProperties();

    animationProperties->setFractalType(this->ui->fractalTypeComboBox1->currentText());

    animationProperties->setAnimationGeometryPropertiesData(m_iTotalFrameNumber, m_iTriggerFrame, m_dEndCenterX, m_dEndCenterY,
                                                            m_iTranslationFunction, m_dEndZoomRatio, m_iZoomRatioFunction,
                                                            m_dEndRotation, m_iRotationFunction, m_iEndIteration, m_iIterationFunction);

    animationProperties->setAnimationFormulaPropertiesData(m_dAnimCtrl1, m_iAnimCtrl1Function, m_dAnimCtrl2, m_iAnimCtrl2Function,
                                                           m_dAnimCtrl3, m_iAnimCtrl3Function, m_dAnimCtrl4, m_iAnimCtrl4Function,
                                                           m_dAnimCtrl5, m_iAnimCtrl5Function, m_dAnimCtrl6, m_iAnimCtrl6Function);

    animationProperties->setAnimationOutsideColoringPropertiesData(m_iAnimOutsideRedAdder, m_iAnimOutsideRedAddFunction,
                                                                   m_iAnimOutsideGreenAdder, m_iAnimOutsideGreenAddFunction,
                                                                   m_iAnimOutsideBlueAdder, m_iAnimOutsideBlueAddFunction,
                                                                   m_dAnimOutsideRedMultiplier, m_iAnimOutsideRedMultFunction,
                                                                   m_dAnimOutsideGreenMultiplier, m_iAnimOutsideGreenMultFunction,
                                                                   m_dAnimOutsideBlueMultiplier, m_iAnimOutsideBlueMultFunction);

    animationProperties->setAnimationInsideColoringPropertiesData(m_iAnimInsideRedAdder, m_iAnimInsideRedAddFunction,
                                                                   m_iAnimInsideGreenAdder, m_iAnimInsideGreenAddFunction,
                                                                   m_iAnimInsideBlueAdder, m_iAnimInsideBlueAddFunction,
                                                                   m_dAnimInsideRedMultiplier, m_iAnimInsideRedMultFunction,
                                                                   m_dAnimInsideGreenMultiplier, m_iAnimInsideGreenMultFunction,
                                                                   m_dAnimInsideBlueMultiplier, m_iAnimInsideBlueMultFunction);

    animationProperties->show();

    connect(animationProperties, SIGNAL(geometryFinished(int, QList<int>, QList<double>, QList<double>, QList<int>, QList<double>,
                                                 QList<int>, QList<double>, QList<int>, QList<long long>, QList<int>)),
            this, SLOT(getAnimationGeometryPropertiesData(int, QList<int>, QList<double>, QList<double>, QList<int>, QList<double>,
                                                          QList<int>, QList<double>, QList<int>, QList<long long>, QList<int>)));

    connect(animationProperties, SIGNAL(formulaFinished(QList<double>,QList<int>,QList<double>,QList<int>,QList<double>,
                                                        QList<int>,QList<double>,QList<int>,QList<double>,QList<int>,QList<double>,QList<int>)),
            this, SLOT(getAnimtationFormulaPropertiesData(QList<double>,QList<int>,QList<double>,QList<int>,QList<double>,QList<int>,
                                                          QList<double>,QList<int>,QList<double>,QList<int>,QList<double>,QList<int>)));

    connect(animationProperties, SIGNAL(outsideColoringFinished(QList<int>,QList<int>,QList<int>,QList<int>,QList<int>,QList<int>,
                                                                QList<double>,QList<int>,QList<double>,QList<int>,QList<double>,QList<int>)),
            this, SLOT(getAnimationOutsideColoringPropertiesData(QList<int>,QList<int>,QList<int>,QList<int>,QList<int>,QList<int>,
                                                                 QList<double>,QList<int>,QList<double>,QList<int>,QList<double>,QList<int>)));

    connect(animationProperties, SIGNAL(insideColoringFinished(QList<int>,QList<int>,QList<int>,QList<int>,QList<int>,QList<int>,
                                                                QList<double>,QList<int>,QList<double>,QList<int>,QList<double>,QList<int>)),
            this, SLOT(getAnimationInsideColoringPropertiesData(QList<int>,QList<int>,QList<int>,QList<int>,QList<int>,QList<int>,
                                                                 QList<double>,QList<int>,QList<double>,QList<int>,QList<double>,QList<int>)));
}

void Fractoria::getAnimationGeometryPropertiesData(int iTotalFrameNumber, QList<int> iTriggerFrame, QList<double> dEndCenterX,
                                                   QList<double> dEndCenterY, QList<int> iTranslationFunction, QList<double> dEndZoomRatio,
                                                   QList<int> iZoomRatioFunction, QList<double> dEndRotation, QList<int> iRotationFunction,
                                                   QList<long long> iEndIteration, QList<int> iIterationFunction)
{
    m_iTotalFrameNumber = iTotalFrameNumber;

    for(int n = 0; n < TRIGGER_POINTS_NUMBER; n++)
    {
        m_iTriggerFrame[n] = iTriggerFrame[n];
        m_dEndCenterX[n] = dEndCenterX[n];
        m_dEndCenterY[n] = dEndCenterY[n];
        m_iTranslationFunction[n] = iTranslationFunction[n];
        m_dEndZoomRatio[n] = dEndZoomRatio[n];
        m_iZoomRatioFunction[n] = iZoomRatioFunction[n];
        m_dEndRotation[n] = dEndRotation[n];
        m_iRotationFunction[n] = iRotationFunction[n];
        m_iEndIteration[n] = iEndIteration[n];
        m_iIterationFunction[n] = iIterationFunction[n];
    }

}

void Fractoria::getAnimtationFormulaPropertiesData(QList<double> dAnimCtrl1, QList<int> iAnimCtrl1Function, QList<double> dAnimCtrl2,
                                                   QList<int> iAnimCtrl2Function, QList<double> dAnimCtrl3, QList<int> iAnimCtrl3Function,
                                                   QList<double> dAnimCtrl4, QList<int> iAnimCtrl4Function, QList<double> dAnimCtrl5,
                                                   QList<int> iAnimCtrl5Function, QList<double> dAnimCtrl6, QList<int> iAnimCtrl6Function)
{
    for(int n = 0; n < TRIGGER_POINTS_NUMBER; n++)
    {
        m_dAnimCtrl1[n] = dAnimCtrl1[n];
        m_iAnimCtrl1Function[n] = iAnimCtrl1Function[n];
        m_dAnimCtrl2[n] = dAnimCtrl2[n];
        m_iAnimCtrl2Function[n] = iAnimCtrl2Function[n];
        m_dAnimCtrl3[n] = dAnimCtrl3[n];
        m_iAnimCtrl3Function[n] = iAnimCtrl3Function[n];
        m_dAnimCtrl4[n] = dAnimCtrl4[n];
        m_iAnimCtrl4Function[n] = iAnimCtrl4Function[n];
        m_dAnimCtrl5[n] = dAnimCtrl5[n];
        m_iAnimCtrl5Function[n] = iAnimCtrl5Function[n];
        m_dAnimCtrl6[n] = dAnimCtrl6[n];
        m_iAnimCtrl6Function[n] = iAnimCtrl6Function[n];
    }
}

void Fractoria::getAnimationOutsideColoringPropertiesData(QList<int> iAnimOutsideRedAdder, QList<int> iAnimOutsideRedAddFunction,
                                                          QList<int> iAnimOutsideGreenAdder, QList<int> iAnimOutsideGreenAddFunction,
                                                          QList<int> iAnimOutsideBlueAdder, QList<int> iAnimOutsideBlueAddFunction,
                                                          QList<double> dAnimOutsideRedMultiplier, QList<int> iAnimOutsideRedMultFunction,
                                                          QList<double> dAnimOutsideGreenMultiplier, QList<int> iAnimOutsideGreenMultFunction,
                                                          QList<double> dAnimOutsideBlueMultiplier, QList<int> iAnimOutsideBlueMultFunction)
{
    for(int n = 0; n < TRIGGER_POINTS_NUMBER; n++)
    {
        m_iAnimOutsideRedAdder[n] = iAnimOutsideRedAdder[n];
        m_iAnimOutsideRedAddFunction[n] = iAnimOutsideRedAddFunction[n];
        m_iAnimOutsideGreenAdder[n] = iAnimOutsideGreenAdder[n];
        m_iAnimOutsideGreenAddFunction[n] = iAnimOutsideGreenAddFunction[n];
        m_iAnimOutsideBlueAdder[n] = iAnimOutsideBlueAdder[n];
        m_iAnimOutsideBlueAddFunction[n] = iAnimOutsideBlueAddFunction[n];
        m_dAnimOutsideRedMultiplier[n] = dAnimOutsideRedMultiplier[n];
        m_iAnimOutsideRedMultFunction[n] = iAnimOutsideRedMultFunction[n];
        m_dAnimOutsideGreenMultiplier[n] = dAnimOutsideGreenMultiplier[n];
        m_iAnimOutsideGreenMultFunction[n] = iAnimOutsideGreenMultFunction[n];
        m_dAnimOutsideBlueMultiplier[n] = dAnimOutsideBlueMultiplier[n];
        m_iAnimOutsideBlueMultFunction[n] = iAnimOutsideBlueMultFunction[n];
    }
}

void Fractoria::getAnimationInsideColoringPropertiesData(QList<int> iAnimInsideRedAdder, QList<int> iAnimInsideRedAddFunction,
                                                          QList<int> iAnimInsideGreenAdder, QList<int> iAnimInsideGreenAddFunction,
                                                          QList<int> iAnimInsideBlueAdder, QList<int> iAnimInsideBlueAddFunction,
                                                          QList<double> dAnimInsideRedMultiplier, QList<int> iAnimInsideRedMultFunction,
                                                          QList<double> dAnimInsideGreenMultiplier, QList<int> iAnimInsideGreenMultFunction,
                                                          QList<double> dAnimInsideBlueMultiplier, QList<int> iAnimInsideBlueMultFunction)
{
    for(int n = 0; n < TRIGGER_POINTS_NUMBER; n++)
    {
        m_iAnimInsideRedAdder[n] = iAnimInsideRedAdder[n];
        m_iAnimInsideRedAddFunction[n] = iAnimInsideRedAddFunction[n];
        m_iAnimInsideGreenAdder[n] = iAnimInsideGreenAdder[n];
        m_iAnimInsideGreenAddFunction[n] = iAnimInsideGreenAddFunction[n];
        m_iAnimInsideBlueAdder[n] = iAnimInsideBlueAdder[n];
        m_iAnimInsideBlueAddFunction[n] = iAnimInsideBlueAddFunction[n];
        m_dAnimInsideRedMultiplier[n] = dAnimInsideRedMultiplier[n];
        m_iAnimInsideRedMultFunction[n] = iAnimInsideRedMultFunction[n];
        m_dAnimInsideGreenMultiplier[n] = dAnimInsideGreenMultiplier[n];
        m_iAnimInsideGreenMultFunction[n] = iAnimInsideGreenMultFunction[n];
        m_dAnimInsideBlueMultiplier[n] = dAnimInsideBlueMultiplier[n];
        m_iAnimInsideBlueMultFunction[n] = iAnimInsideBlueMultFunction[n];
    }
}

void Fractoria::on_animateButton_clicked()
{
    animationState = true;
    this->ui->suspendButton->setEnabled(true);

    animThread = new QThread(this);
    animateTask = new AnimationThread();

    animateTask->setCurrentGeometryParameters(m_dCenterX, m_dCenterY, m_dZoomRatio, m_dRotation, m_iIteration, m_dBailout);

    animateTask->setCurrentControlParameters(m_dControl1, m_dControl2, m_dControl3, m_dControl4, m_dControl5, m_dControl6);

    animateTask->setCurrentOutsideColoringParameters(m_iOutsideRedColorAdder, m_iOutsideGreenColorAdder, m_iOutsideBlueColorAdder,
                                                     m_dOutsideRedColorMultiplier, m_dOutsideGreenColorMultiplier, m_dOutsideBlueColorMultiplier);

    animateTask->setCurrentInsideColoringParameters(m_iInsideRedColorAdder, m_iInsideGreenColorAdder, m_iInsideBlueColorAdder,
                                                    m_dInsideRedColorMultiplier, m_dInsideGreenColorMultiplier, m_dInsideBlueColorMultiplier);

    animateTask->setFractalType(this->ui->fractalTypeComboBox1->currentText());

    animateTask->setTotalFrameNumber(m_iTotalFrameNumber);

    animateTask->setGeometryParameters(m_iTriggerFrame, m_dEndCenterX, m_dEndCenterY,
                                       m_iTranslationFunction, m_dEndZoomRatio, m_iZoomRatioFunction,
                                       m_dEndRotation, m_iRotationFunction, m_iEndIteration, m_iIterationFunction);

    animateTask->setFormulaParameters(m_dAnimCtrl1, m_iAnimCtrl1Function, m_dAnimCtrl2, m_iAnimCtrl2Function,
                                      m_dAnimCtrl3, m_iAnimCtrl3Function, m_dAnimCtrl4, m_iAnimCtrl4Function,
                                      m_dAnimCtrl5, m_iAnimCtrl5Function, m_dAnimCtrl6, m_iAnimCtrl6Function);

    animateTask->setOutsideColoringParameters(m_iAnimOutsideRedAdder, m_iAnimOutsideRedAddFunction,
                                              m_iAnimOutsideGreenAdder, m_iAnimOutsideGreenAddFunction,
                                              m_iAnimOutsideBlueAdder, m_iAnimOutsideBlueAddFunction,
                                              m_dAnimOutsideRedMultiplier, m_iAnimOutsideRedMultFunction,
                                              m_dAnimOutsideGreenMultiplier, m_iAnimOutsideGreenMultFunction,
                                              m_dAnimOutsideBlueMultiplier, m_iAnimOutsideBlueMultFunction);

    animateTask->setInsideColoringParameters(m_iAnimInsideRedAdder, m_iAnimInsideRedAddFunction,
                                             m_iAnimInsideGreenAdder, m_iAnimInsideGreenAddFunction,
                                             m_iAnimInsideBlueAdder, m_iAnimInsideBlueAddFunction,
                                             m_dAnimInsideRedMultiplier, m_iAnimInsideRedMultFunction,
                                             m_dAnimInsideGreenMultiplier, m_iAnimInsideGreenMultFunction,
                                             m_dAnimInsideBlueMultiplier, m_iAnimInsideBlueMultFunction);

    animateTask->setColoringParameters(m_iColoringAlgorithm, m_iAlgorithmAttributes,
                                       m_dRedMultiplier, m_dGreenMultiplier, m_dBlueMultiplier,
                                       m_bRedMainColor, m_bGreenMainColor, m_bBlueMainColor);

    animateTask->setNewtonRootColoringPrameters(m_iFirstRootRed, m_iFirstRootGreen, m_iFirstRootBlue,
                                                m_iSecondRootRed, m_iSecondRootGreen, m_iSecondRootBlue,
                                                m_iThirdRootRed, m_iThirdRootGreen, m_iThirdRootBlue);

    animateTask->setShowProgressionParameter(m_bShowProgression);

    animateTask->moveToThread(animThread);

    connect(animThread, SIGNAL(started()), animateTask, SLOT(renderAnimation()));
    connect(animateTask, SIGNAL(workFinished()), this, SLOT(updateSuspendButtonStateAnimation()));
    connect(animateTask, SIGNAL(workFinished()), this, SLOT(startAnimationViewer()));
    connect(animateTask, SIGNAL(workFinished()), animThread, SLOT(quit()));
    connect(animateTask, SIGNAL(animationTimerUpdate(double)), this, SLOT(animationUpdateTimer(double)));
    connect(animateTask, SIGNAL(animationProgress(int)), this, SLOT(animationShowProgress(int)));

    // Automatically delete thread and task object when work is done:
    connect(animThread, SIGNAL(finished()), animateTask, SLOT(deleteLater()));
    connect(animThread, SIGNAL(finished()), animThread, SLOT(deleteLater()));

    connect(animateTask, SIGNAL(timerUpdate(double)), this, SLOT(updateTimer(double)));
    connect(animateTask, SIGNAL(rendered(QImage)), this, SLOT(showImage(QImage)));
    connect(animateTask, SIGNAL(progress(int)), this, SLOT(showProgress(int)));


    connect(animateTask, SIGNAL(updateGeometryParameters(double,double,double,double,long long,double)),
            this, SLOT(updateGeometryParameters(double,double,double,double,long long,double)));

    connect(animateTask, SIGNAL(updateFormulaParameters(double,double,double,double,double,double)),
            this, SLOT(updateFormulaParameters(double,double,double,double,double,double)));

    connect(animateTask, SIGNAL(updateOutsideColoringParameters(int,int,int,double,double,double)),
            this, SLOT(updateOutsideColoringParameters(int,int,int,double,double,double)));

    connect(animateTask, SIGNAL(updateInsideColoringParameters(int,int,int,double,double,double)),
            this, SLOT(updateInsideColoringParameters(int,int,int,double,double,double)));


    animThread->start();
}

void Fractoria::updateGeometryParameters(double xCenter, double yCenter, double zoomRatio, double rotation, long long iterations, double bailout)
{
    this->ui->xCenterLineEdit->setText(QString::number(xCenter));
    this->ui->yCenterLineEdit->setText(QString::number(yCenter));
    this->ui->zoomRatioLineEdit->setText(QString::number(zoomRatio));
    this->ui->rotationLineEdit->setText(QString::number(rotation));
    this->ui->iterationsLineEdit->setText(QString::number(iterations));
    this->ui->bailoutLineEdit->setText(QString::number(bailout));
}

void Fractoria::updateFormulaParameters(double control1, double control2, double control3, double control4, double control5, double control6)
{
    this->ui->control1LineEdit->setText(QString::number(control1));
    this->ui->control2LineEdit->setText(QString::number(control2));
    this->ui->control3LineEdit->setText(QString::number(control3));
    this->ui->control4LineEdit->setText(QString::number(control4));
    this->ui->control5LineEdit->setText(QString::number(control5));
    this->ui->control6LineEdit->setText(QString::number(control6));
}

void Fractoria::updateOutsideColoringParameters(int iOutsideRedColorAdder, int iOutsideGreenColorAdder, int iOutsideBlueColorAdder,
                                                double dOutsideRedColorMultiplier, double dOutsideGreenColorMultiplier, double dOutsideBlueColorMultiplier)
{
    this->ui->outsideRedColorAdderLineEdit->setText(QString::number(iOutsideRedColorAdder));
    this->ui->outsideGreenColorAdderLineEdit->setText(QString::number(iOutsideGreenColorAdder));
    this->ui->outsideBlueColorAdderLineEdit->setText(QString::number(iOutsideBlueColorAdder));
    this->ui->outsideRedColorMultiplierLineEdit->setText(QString::number(dOutsideRedColorMultiplier));
    this->ui->outsideGreenColorMultiplierLineEdit->setText(QString::number(dOutsideGreenColorMultiplier));
    this->ui->outsideBlueColorMultiplierLineEdit->setText(QString::number(dOutsideBlueColorMultiplier));
}

void Fractoria::updateInsideColoringParameters(int iInsideRedColorAdder, int iInsideGreenColorAdder, int iInsideBlueColorAdder,
                                                double dInsideRedColorMultiplier, double dInsideGreenColorMultiplier, double dInsideBlueColorMultiplier)
{
    this->ui->insideRedColorAdderLineEdit->setText(QString::number(iInsideRedColorAdder));
    this->ui->insideGreenColorAdderLineEdit->setText(QString::number(iInsideGreenColorAdder));
    this->ui->insideBlueColorAdderLineEdit->setText(QString::number(iInsideBlueColorAdder));
    this->ui->insideRedColorMultiplierLineEdit->setText(QString::number(dInsideRedColorMultiplier));
    this->ui->insideGreenColorMultiplierLineEdit->setText(QString::number(dInsideGreenColorMultiplier));
    this->ui->insideBlueColorMultiplierLineEdit->setText(QString::number(dInsideBlueColorMultiplier));
}

void Fractoria::updateSuspendButtonState()
{
    renderState = false;
    this->ui->suspendButton->setEnabled(false);
}

void Fractoria::updateSuspendButtonStateAnimation()
{
    animationState = false;
    this->ui->suspendButton->setEnabled(false);
}

void Fractoria::on_suspendButton_clicked()
{
    if(renderState == true)
    {
        renderTask->m_bSuspendValue = true;
    }
    else if(animationState == true)
    {
        animateTask->m_bSuspendValue = true;
    }
}

void Fractoria::startAnimationViewer()
{
    animationViewer = new AnimationViewer();    
    animationViewer->show();
}

void Fractoria::on_switchModeCheckBox_toggled(bool checked)
{
    m_bSwitchModeFractal = checked;
}

void Fractoria::optionsFunction()
{
    options = new Options();

    options->setShowProgressionParameter(m_bShowProgression);

    connect(options, SIGNAL(showProgressionParameter(bool)), this, SLOT(getShowProgressionParameter(bool)));
    connect(options, SIGNAL(animationFirstTimeParameter(bool)), this, SLOT(getAnimationProperties(bool)));

    options->show();
}

void Fractoria::getShowProgressionParameter(bool bShowProgression)
{
    m_bShowProgression = bShowProgression;
}

void Fractoria::getAnimationProperties(bool bFirstTime)
{
    firstTime = bFirstTime;
}

void Fractoria::resetAnimationProperties()
{
    firstTime = true;
}

void Fractoria::about()
{
    QMessageBox::about(this, tr("Fractoria"), "Fractoria ©  Version 3.1 (x64) - All Rights Reserved.");
}

void Fractoria::presentIFSController()
{
    ifs = new IFS();
    ifs->show();
}
