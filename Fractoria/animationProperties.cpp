#include "animationProperties.h"
#include "ui_animationProperties.h"
#include <QMouseEvent>

AnimationProperties::AnimationProperties(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnimationProperties)
{
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
        m_dAnimOutsideRedMultiplier << 0.0;
        m_iAnimOutsideRedMultFunction << 0;
        m_dAnimOutsideGreenMultiplier << 0.0;
        m_iAnimOutsideGreenMultFunction << 0;
        m_dAnimOutsideBlueMultiplier << 0.0;
        m_iAnimOutsideBlueMultFunction << 0;

        m_iAnimInsideRedAdder << 0;
        m_iAnimInsideRedAddFunction << 0;
        m_iAnimInsideGreenAdder << 0;
        m_iAnimInsideGreenAddFunction << 0;
        m_iAnimInsideBlueAdder << 0;
        m_iAnimInsideBlueAddFunction << 0;
        m_dAnimInsideRedMultiplier << 0.0;
        m_iAnimInsideRedMultFunction << 0;
        m_dAnimInsideGreenMultiplier << 0.0;
        m_iAnimInsideGreenMultFunction << 0;
        m_dAnimInsideBlueMultiplier << 0.0;
        m_iAnimInsideBlueMultFunction << 0;
    }

    ui->setupUi(this);


    // Geometry ComboBox Values

    QStringList triggerPointList = (QStringList()<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8");
    this->ui->triggerPointComboBox->addItems(triggerPointList);

    m_iTriggerIndex = 0;

    QStringList translationFunctionList = (QStringList()<<"Linear"<<"Positive Quadratic"<<"Negative Quadratic");
    this->ui->translationFunctionComboBox->addItems(translationFunctionList);

    QStringList functionList = (QStringList()<<"Linear"<<"Positive Quadratic"<<"Negative Quadratic");
    this->ui->zoomRatioFunctionComboBox->addItems(functionList);
    this->ui->rotationFunctionComboBox->addItems(functionList);
    this->ui->iterationFunctionComboBox->addItems(functionList);

    this->ui->outsideRedAdderFunctionComboBox->addItems(functionList);
    this->ui->outsideGreenAdderFunctionComboBox->addItems(functionList);
    this->ui->outsideBlueAdderFunctionComboBox->addItems(functionList);
    this->ui->outsideRedMultFunctionComboBox->addItems(functionList);
    this->ui->outsideGreenMultFunctionComboBox->addItems(functionList);
    this->ui->outsideBlueMultFunctionComboBox->addItems(functionList);

    this->ui->insideRedAdderFunctionComboBox->addItems(functionList);
    this->ui->insideGreenAdderFunctionComboBox->addItems(functionList);
    this->ui->insideBlueAdderFunctionComboBox->addItems(functionList);
    this->ui->insideRedMultFunctionComboBox->addItems(functionList);
    this->ui->insideGreenMultFunctionComboBox->addItems(functionList);
    this->ui->insideBlueMultFunctionComboBox->addItems(functionList);

    // Formula ComboBox Values

    QStringList control1List = (QStringList()<<"Linear"<<"Positive Quadratic"<<"Negative Quadratic");
    this->ui->control1ComboBox->addItems(control1List);

    QStringList control2List = (QStringList()<<"Linear"<<"Positive Quadratic"<<"Negative Quadratic");
    this->ui->control2ComboBox->addItems(control1List);

    QStringList control3List = (QStringList()<<"Linear"<<"Positive Quadratic"<<"Negative Quadratic");
    this->ui->control3ComboBox->addItems(control1List);

    QStringList control4List = (QStringList()<<"Linear"<<"Positive Quadratic"<<"Negative Quadratic");
    this->ui->control4ComboBox->addItems(control1List);

    QStringList control5List = (QStringList()<<"Linear"<<"Positive Quadratic"<<"Negative Quadratic");
    this->ui->control5ComboBox->addItems(control1List);

    QStringList control6List = (QStringList()<<"Linear"<<"Positive Quadratic"<<"Negative Quadratic");
    this->ui->control6ComboBox->addItems(control1List);

}

AnimationProperties::~AnimationProperties()
{
    delete ui;
}

void AnimationProperties::on_okButton_clicked()
{
    emit geometryFinished(m_iTotalFrameNumber, m_iTriggerFrame, m_dEndCenterX, m_dEndCenterY, m_iTranslationFunction,
                  m_dEndZoomRatio, m_iZoomRatioFunction, m_dEndRotation, m_iRotationFunction, m_iEndIteration, m_iIterationFunction);

    emit formulaFinished(m_dAnimCtrl1, m_iAnimCtrl1Function, m_dAnimCtrl2, m_iAnimCtrl2Function, m_dAnimCtrl3, m_iAnimCtrl3Function,
                         m_dAnimCtrl4, m_iAnimCtrl4Function, m_dAnimCtrl5, m_iAnimCtrl5Function, m_dAnimCtrl6, m_iAnimCtrl6Function);

    emit outsideColoringFinished(m_iAnimOutsideRedAdder, m_iAnimOutsideRedAddFunction, m_iAnimOutsideGreenAdder, m_iAnimOutsideGreenAddFunction,
                                 m_iAnimOutsideBlueAdder, m_iAnimOutsideBlueAddFunction, m_dAnimOutsideRedMultiplier, m_iAnimOutsideRedMultFunction,
                                 m_dAnimOutsideGreenMultiplier, m_iAnimOutsideGreenMultFunction, m_dAnimOutsideBlueMultiplier, m_iAnimOutsideBlueMultFunction);

    emit insideColoringFinished(m_iAnimInsideRedAdder, m_iAnimInsideRedAddFunction, m_iAnimInsideGreenAdder, m_iAnimInsideGreenAddFunction,
                                 m_iAnimInsideBlueAdder, m_iAnimInsideBlueAddFunction, m_dAnimInsideRedMultiplier, m_iAnimInsideRedMultFunction,
                                 m_dAnimInsideGreenMultiplier, m_iAnimInsideGreenMultFunction, m_dAnimInsideBlueMultiplier, m_iAnimInsideBlueMultFunction);

    this->close();
}

void AnimationProperties::on_totalFrameNumberLineEdit_textChanged(const QString &arg1)
{
    m_iTotalFrameNumber = QString(arg1).toInt();
}

void AnimationProperties::on_triggerPointComboBox_currentIndexChanged(int index)
{
    m_iTriggerIndex = index;

    // Geometry Animation Variables
    this->ui->triggerFrameLineEdit->setText(QString::number(m_iTriggerFrame[m_iTriggerIndex]));
    this->ui->endCenterXLineEdit->setText(QString::number(m_dEndCenterX[m_iTriggerIndex]));
    this->ui->endCenterYLineEdit->setText(QString::number(m_dEndCenterY[m_iTriggerIndex]));
    this->ui->translationFunctionComboBox->setCurrentIndex(m_iTranslationFunction[m_iTriggerIndex]);
    this->ui->endZoomRatioLineEdit->setText(QString::number(m_dEndZoomRatio[m_iTriggerIndex]));
    this->ui->zoomRatioFunctionComboBox->setCurrentIndex(m_iZoomRatioFunction[m_iTriggerIndex]);
    this->ui->endRotationLineEdit->setText(QString::number(m_dEndRotation[m_iTriggerIndex]));
    this->ui->rotationFunctionComboBox->setCurrentIndex(m_iRotationFunction[m_iTriggerIndex]);
    this->ui->endIterationLineEdit->setText(QString::number(m_iEndIteration[m_iTriggerIndex]));
    this->ui->iterationFunctionComboBox->setCurrentIndex(m_iIterationFunction[m_iTriggerIndex]);

    // Formula Animation Variables
    this->ui->control1LineEdit->setText(QString::number(m_dAnimCtrl1[m_iTriggerIndex]));
    this->ui->control1ComboBox->setCurrentIndex(m_iAnimCtrl1Function[m_iTriggerIndex]);
    this->ui->control2LineEdit->setText(QString::number(m_dAnimCtrl2[m_iTriggerIndex]));
    this->ui->control2ComboBox->setCurrentIndex(m_iAnimCtrl2Function[m_iTriggerIndex]);
    this->ui->control3LineEdit->setText(QString::number(m_dAnimCtrl3[m_iTriggerIndex]));
    this->ui->control3ComboBox->setCurrentIndex(m_iAnimCtrl3Function[m_iTriggerIndex]);
    this->ui->control4LineEdit->setText(QString::number(m_dAnimCtrl4[m_iTriggerIndex]));
    this->ui->control4ComboBox->setCurrentIndex(m_iAnimCtrl4Function[m_iTriggerIndex]);
    this->ui->control5LineEdit->setText(QString::number(m_dAnimCtrl5[m_iTriggerIndex]));
    this->ui->control5ComboBox->setCurrentIndex(m_iAnimCtrl5Function[m_iTriggerIndex]);
    this->ui->control6LineEdit->setText(QString::number(m_dAnimCtrl6[m_iTriggerIndex]));
    this->ui->control6ComboBox->setCurrentIndex(m_iAnimCtrl6Function[m_iTriggerIndex]);

    // Outside Coloring Animation Variables
    this->ui->outsideRedAdderLineEdit->setText(QString::number(m_iAnimOutsideRedAdder[m_iTriggerIndex]));
    this->ui->outsideRedAdderFunctionComboBox->setCurrentIndex(m_iAnimOutsideRedAddFunction[m_iTriggerIndex]);
    this->ui->outsideGreenAdderLineEdit->setText(QString::number(m_iAnimOutsideGreenAdder[m_iTriggerIndex]));
    this->ui->outsideGreenAdderFunctionComboBox->setCurrentIndex(m_iAnimOutsideGreenAddFunction[m_iTriggerIndex]);
    this->ui->outsideBlueAdderLineEdit->setText(QString::number(m_iAnimOutsideBlueAdder[m_iTriggerIndex]));
    this->ui->outsideBlueAdderFunctionComboBox->setCurrentIndex(m_iAnimOutsideBlueAddFunction[m_iTriggerIndex]);
    this->ui->outsideRedMultLineEdit->setText(QString::number(m_dAnimOutsideRedMultiplier[m_iTriggerIndex]));
    this->ui->outsideRedMultFunctionComboBox->setCurrentIndex( m_iAnimOutsideRedMultFunction[m_iTriggerIndex]);
    this->ui->outsideGreenMultLineEdit->setText(QString::number(m_dAnimOutsideGreenMultiplier[m_iTriggerIndex]));
    this->ui->outsideGreenMultFunctionComboBox->setCurrentIndex( m_iAnimOutsideGreenMultFunction[m_iTriggerIndex]);
    this->ui->outsideBlueMultLineEdit->setText(QString::number(m_dAnimOutsideBlueMultiplier[m_iTriggerIndex]));
    this->ui->outsideBlueMultFunctionComboBox->setCurrentIndex( m_iAnimOutsideBlueMultFunction[m_iTriggerIndex]);

    // Inside Coloring Animation Variables
    this->ui->insideRedAdderLineEdit->setText(QString::number(m_iAnimInsideRedAdder[m_iTriggerIndex]));
    this->ui->insideRedAdderFunctionComboBox->setCurrentIndex(m_iAnimInsideRedAddFunction[m_iTriggerIndex]);
    this->ui->insideGreenAdderLineEdit->setText(QString::number(m_iAnimInsideGreenAdder[m_iTriggerIndex]));
    this->ui->insideGreenAdderFunctionComboBox->setCurrentIndex(m_iAnimInsideGreenAddFunction[m_iTriggerIndex]);
    this->ui->insideBlueAdderLineEdit->setText(QString::number(m_iAnimInsideBlueAdder[m_iTriggerIndex]));
    this->ui->insideBlueAdderFunctionComboBox->setCurrentIndex(m_iAnimInsideBlueAddFunction[m_iTriggerIndex]);
    this->ui->insideRedMultLineEdit->setText(QString::number(m_dAnimInsideRedMultiplier[m_iTriggerIndex]));
    this->ui->insideRedMultFunctionComboBox->setCurrentIndex( m_iAnimInsideRedMultFunction[m_iTriggerIndex]);
    this->ui->insideGreenMultLineEdit->setText(QString::number(m_dAnimInsideGreenMultiplier[m_iTriggerIndex]));
    this->ui->insideGreenMultFunctionComboBox->setCurrentIndex( m_iAnimInsideGreenMultFunction[m_iTriggerIndex]);
    this->ui->insideBlueMultLineEdit->setText(QString::number(m_dAnimInsideBlueMultiplier[m_iTriggerIndex]));
    this->ui->insideBlueMultFunctionComboBox->setCurrentIndex( m_iAnimInsideBlueMultFunction[m_iTriggerIndex]);
}

void AnimationProperties::on_triggerFrameLineEdit_textChanged(const QString &arg1)
{
    m_iTriggerFrame[m_iTriggerIndex] = QString(arg1).toInt();
}

void AnimationProperties::on_endCenterXLineEdit_textChanged(const QString &arg1)
{
    m_dEndCenterX[m_iTriggerIndex] = QString(arg1).toDouble();
}

void AnimationProperties::on_endCenterYLineEdit_textChanged(const QString &arg1)
{
    m_dEndCenterY[m_iTriggerIndex] = QString(arg1).toDouble();
}

void AnimationProperties::on_translationFunctionComboBox_currentIndexChanged(int index)
{
    m_iTranslationFunction[m_iTriggerIndex] = index;
}

void AnimationProperties::on_endZoomRatioLineEdit_textChanged(const QString &arg1)
{
    m_dEndZoomRatio[m_iTriggerIndex] = QString(arg1).toDouble();
}

void AnimationProperties::on_zoomRatioFunctionComboBox_currentIndexChanged(int index)
{
    m_iZoomRatioFunction[m_iTriggerIndex] = index;
}

void AnimationProperties::on_endRotationLineEdit_textChanged(const QString &arg1)
{
    m_dEndRotation[m_iTriggerIndex] = QString(arg1).toDouble();
}

void AnimationProperties::on_rotationFunctionComboBox_currentIndexChanged(int index)
{
    m_iRotationFunction[m_iTriggerIndex] = index;
}

void AnimationProperties::on_endIterationLineEdit_textChanged(const QString &arg1)
{
    m_iEndIteration[m_iTriggerIndex] = QString(arg1).toLongLong();
}

void AnimationProperties::on_iterationFunctionComboBox_currentIndexChanged(int index)
{
    m_iIterationFunction[m_iTriggerIndex] = index;
}

void AnimationProperties::setAnimationGeometryPropertiesData(int iTotalFrameNumber, QList<int> iTriggerFrame, QList<double> dEndCenterX,
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

    this->ui->totalFrameNumberLineEdit->setText(QString::number(m_iTotalFrameNumber));
    this->ui->triggerFrameLineEdit->setText(QString::number(m_iTriggerFrame[0]));
    this->ui->endCenterXLineEdit->setText(QString::number(m_dEndCenterX[0]));
    this->ui->endCenterYLineEdit->setText(QString::number(m_dEndCenterY[0]));
    this->ui->translationFunctionComboBox->setCurrentIndex(m_iTranslationFunction[0]);
    this->ui->endZoomRatioLineEdit->setText(QString::number(m_dEndZoomRatio[0]));
    this->ui->zoomRatioFunctionComboBox->setCurrentIndex(m_iZoomRatioFunction[0]);
    this->ui->endRotationLineEdit->setText(QString::number(m_dEndRotation[0]));
    this->ui->rotationFunctionComboBox->setCurrentIndex(m_iRotationFunction[0]);
    this->ui->endIterationLineEdit->setText(QString::number(m_iEndIteration[0]));
    this->ui->iterationFunctionComboBox->setCurrentIndex(m_iIterationFunction[0]);
}

void AnimationProperties::on_control1LineEdit_textChanged(const QString &arg1)
{
    m_dAnimCtrl1[m_iTriggerIndex] = QString(arg1).toDouble();
}

void AnimationProperties::on_control1ComboBox_currentIndexChanged(int index)
{
    m_iAnimCtrl1Function[m_iTriggerIndex] = index;
}

void AnimationProperties::on_control2LineEdit_textChanged(const QString &arg1)
{
    m_dAnimCtrl2[m_iTriggerIndex] = QString(arg1).toDouble();
}

void AnimationProperties::on_control2ComboBox_currentIndexChanged(int index)
{
    m_iAnimCtrl2Function[m_iTriggerIndex] = index;
}

void AnimationProperties::on_control3LineEdit_textChanged(const QString &arg1)
{
    m_dAnimCtrl3[m_iTriggerIndex] = QString(arg1).toDouble();
}

void AnimationProperties::on_control3ComboBox_currentIndexChanged(int index)
{
    m_iAnimCtrl3Function[m_iTriggerIndex] = index;
}

void AnimationProperties::on_control4LineEdit_textChanged(const QString &arg1)
{
    m_dAnimCtrl4[m_iTriggerIndex] = QString(arg1).toDouble();
}

void AnimationProperties::on_control4ComboBox_currentIndexChanged(int index)
{
    m_iAnimCtrl4Function[m_iTriggerIndex] = index;
}

void AnimationProperties::on_control5LineEdit_textChanged(const QString &arg1)
{
    m_dAnimCtrl5[m_iTriggerIndex] = QString(arg1).toDouble();
}

void AnimationProperties::on_control5ComboBox_currentIndexChanged(int index)
{
    m_iAnimCtrl5Function[m_iTriggerIndex] = index;
}

void AnimationProperties::on_control6LineEdit_textChanged(const QString &arg1)
{
    m_dAnimCtrl6[m_iTriggerIndex] = QString(arg1).toDouble();
}

void AnimationProperties::on_control6ComboBox_currentIndexChanged(int index)
{
    m_iAnimCtrl6Function[m_iTriggerIndex] = index;
}

void AnimationProperties::setAnimationFormulaPropertiesData(QList<double> dAnimCtrl1, QList<int> iAnimCtrl1Function, QList<double> dAnimCtrl2,
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

    this->ui->control1LineEdit->setText(QString::number(m_dAnimCtrl1[0]));
    this->ui->control1ComboBox->setCurrentIndex(m_iAnimCtrl1Function[0]);
    this->ui->control2LineEdit->setText(QString::number(m_dAnimCtrl2[0]));
    this->ui->control2ComboBox->setCurrentIndex(m_iAnimCtrl2Function[0]);
    this->ui->control3LineEdit->setText(QString::number(m_dAnimCtrl3[0]));
    this->ui->control3ComboBox->setCurrentIndex(m_iAnimCtrl3Function[0]);
    this->ui->control4LineEdit->setText(QString::number(m_dAnimCtrl4[0]));
    this->ui->control4ComboBox->setCurrentIndex(m_iAnimCtrl4Function[0]);
    this->ui->control5LineEdit->setText(QString::number(m_dAnimCtrl5[0]));
    this->ui->control5ComboBox->setCurrentIndex(m_iAnimCtrl5Function[0]);
    this->ui->control6LineEdit->setText(QString::number(m_dAnimCtrl6[0]));
    this->ui->control6ComboBox->setCurrentIndex(m_iAnimCtrl6Function[0]);

}

void AnimationProperties::setFractalType(QString sFractalType)
{
    m_sFractalType = sFractalType;
/*
    QMessageBox msgBox;
    msgBox.setText(m_sFractalType);
    msgBox.exec();
*/
    controlDefaultsFunction();
}

void AnimationProperties::controlDefaultsFunction()
{
    this->ui->control1Label->setEnabled(true);
    this->ui->control1Label->setText("Control #1 :");
    this->ui->control1LineEdit->setEnabled(true);
    this->ui->control1FunctionLabel->setEnabled(true);
    this->ui->control1FunctionLabel->setText("Control #1 Function :");
    this->ui->control1ComboBox->setEnabled(true);

    this->ui->control2Label->setEnabled(true);
    this->ui->control2Label->setText("Control #2 :");
    this->ui->control2LineEdit->setEnabled(true);
    this->ui->control2FunctionLabel->setEnabled(true);
    this->ui->control2FunctionLabel->setText("Control #2 Function :");
    this->ui->control2ComboBox->setEnabled(true);

    this->ui->control3Label->setEnabled(true);
    this->ui->control3Label->setText("Control #3 :");
    this->ui->control3LineEdit->setEnabled(true);
    this->ui->control3FunctionLabel->setEnabled(true);
    this->ui->control3FunctionLabel->setText("Control #3 Function :");
    this->ui->control3ComboBox->setEnabled(true);

    this->ui->control4Label->setEnabled(true);
    this->ui->control4Label->setText("Control #4 :");
    this->ui->control4LineEdit->setEnabled(true);
    this->ui->control4FunctionLabel->setEnabled(true);
    this->ui->control4FunctionLabel->setText("Control #4 Function :");
    this->ui->control4ComboBox->setEnabled(true);

    this->ui->control5Label->setEnabled(true);
    this->ui->control5Label->setText("Control #5 :");
    this->ui->control5LineEdit->setEnabled(true);
    this->ui->control5FunctionLabel->setEnabled(true);
    this->ui->control5FunctionLabel->setText("Control #5 Function :");
    this->ui->control5ComboBox->setEnabled(true);

    this->ui->control6Label->setEnabled(true);
    this->ui->control6Label->setText("Control #6 :");
    this->ui->control6LineEdit->setEnabled(true);
    this->ui->control6FunctionLabel->setEnabled(true);
    this->ui->control6FunctionLabel->setText("Control #6 Function :");
    this->ui->control6ComboBox->setEnabled(true);

    if(m_sFractalType == "Mandelbrot (Quick)")
    {
        this->ui->control1Label->setText("End Start (X) :");
        this->ui->control1FunctionLabel->setText("Start (X) Function :");
        this->ui->control2Label->setText("End Start (Y) :");
        this->ui->control2FunctionLabel->setText("Start (Y) Function :");
        this->ui->control3Label->setEnabled(false);
        this->ui->control3LineEdit->setEnabled(false);
        this->ui->control3LineEdit->setText("N/A");
        this->ui->control3FunctionLabel->setEnabled(false);
        this->ui->control3ComboBox->setEnabled(false);
        this->ui->control4Label->setEnabled(false);
        this->ui->control4LineEdit->setEnabled(false);
        this->ui->control4LineEdit->setText("N/A");
        this->ui->control4FunctionLabel->setEnabled(false);
        this->ui->control4ComboBox->setEnabled(false);
        this->ui->control5Label->setEnabled(false);
        this->ui->control5LineEdit->setEnabled(false);
        this->ui->control5LineEdit->setText("N/A");
        this->ui->control5FunctionLabel->setEnabled(false);
        this->ui->control5ComboBox->setEnabled(false);
        this->ui->control6Label->setEnabled(false);
        this->ui->control6LineEdit->setEnabled(false);
        this->ui->control6LineEdit->setText("N/A");
        this->ui->control6FunctionLabel->setEnabled(false);
        this->ui->control6ComboBox->setEnabled(false);
    }
    else if(m_sFractalType == "Mandelbrot")
    {
        this->ui->control1Label->setText("End Start (X) :");
        this->ui->control1FunctionLabel->setText("Start (X) Function :");
        this->ui->control2Label->setText("End Start (Y) :");
        this->ui->control2FunctionLabel->setText("Start (Y) Function :");
        this->ui->control3Label->setText("End Power :");
        this->ui->control3FunctionLabel->setText("Power Function :");
        this->ui->control4Label->setEnabled(false);
        this->ui->control4LineEdit->setEnabled(false);
        this->ui->control4LineEdit->setText("N/A");
        this->ui->control4FunctionLabel->setEnabled(false);
        this->ui->control4ComboBox->setEnabled(false);
        this->ui->control5Label->setEnabled(false);
        this->ui->control5LineEdit->setEnabled(false);
        this->ui->control5LineEdit->setText("N/A");
        this->ui->control5FunctionLabel->setEnabled(false);
        this->ui->control5ComboBox->setEnabled(false);
        this->ui->control6Label->setEnabled(false);
        this->ui->control6LineEdit->setEnabled(false);
        this->ui->control6LineEdit->setText("N/A");
        this->ui->control6FunctionLabel->setEnabled(false);
        this->ui->control6ComboBox->setEnabled(false);
    }
    else if(m_sFractalType == "Julia (Quick)")
    {
        this->ui->control1Label->setText("End Seed (X) :");
        this->ui->control1FunctionLabel->setText("Seed (X) Function :");
        this->ui->control2Label->setText("End Seed (Y) :");
        this->ui->control2FunctionLabel->setText("Seed (Y) Function :");
        this->ui->control3Label->setEnabled(false);
        this->ui->control3LineEdit->setEnabled(false);
        this->ui->control3LineEdit->setText("N/A");
        this->ui->control3FunctionLabel->setEnabled(false);
        this->ui->control3ComboBox->setEnabled(false);
        this->ui->control4Label->setEnabled(false);
        this->ui->control4LineEdit->setEnabled(false);
        this->ui->control4LineEdit->setText("N/A");
        this->ui->control4FunctionLabel->setEnabled(false);
        this->ui->control4ComboBox->setEnabled(false);
        this->ui->control5Label->setEnabled(false);
        this->ui->control5LineEdit->setEnabled(false);
        this->ui->control5LineEdit->setText("N/A");
        this->ui->control5FunctionLabel->setEnabled(false);
        this->ui->control5ComboBox->setEnabled(false);
        this->ui->control6Label->setEnabled(false);
        this->ui->control6LineEdit->setEnabled(false);
        this->ui->control6LineEdit->setText("N/A");
        this->ui->control6FunctionLabel->setEnabled(false);
        this->ui->control6ComboBox->setEnabled(false);
    }
    else if(m_sFractalType == "Julia")
    {
        this->ui->control1Label->setText("End Seed (Re) :");
        this->ui->control1FunctionLabel->setText("Seed (Re) Function :");
        this->ui->control2Label->setText("End Seed (Im) :");
        this->ui->control2FunctionLabel->setText("Seed (Im) Function :");
        this->ui->control3Label->setText("End Power :");
        this->ui->control3FunctionLabel->setText("Power Function :");
        this->ui->control4Label->setEnabled(false);
        this->ui->control4LineEdit->setEnabled(false);
        this->ui->control4LineEdit->setText("N/A");
        this->ui->control4FunctionLabel->setEnabled(false);
        this->ui->control4ComboBox->setEnabled(false);
        this->ui->control5Label->setEnabled(false);
        this->ui->control5LineEdit->setEnabled(false);
        this->ui->control5LineEdit->setText("N/A");
        this->ui->control5FunctionLabel->setEnabled(false);
        this->ui->control5ComboBox->setEnabled(false);
        this->ui->control6Label->setEnabled(false);
        this->ui->control6LineEdit->setEnabled(false);
        this->ui->control6LineEdit->setText("N/A");
        this->ui->control6FunctionLabel->setEnabled(false);
        this->ui->control6ComboBox->setEnabled(false);
    }
    else if(m_sFractalType == "Newton")
    {
        this->ui->control1Label->setText("End Relax (Re) :");
        this->ui->control1FunctionLabel->setText("Relax (Re) Function :");
        this->ui->control2Label->setText("End Relax (Im) :");
        this->ui->control2FunctionLabel->setText("Relax (Im) Function :");
        this->ui->control3Label->setText("End Power :");
        this->ui->control3FunctionLabel->setText("Power Function :");
        this->ui->control4Label->setEnabled(false);
        this->ui->control4LineEdit->setEnabled(false);
        this->ui->control4LineEdit->setText("N/A");
        this->ui->control4FunctionLabel->setEnabled(false);
        this->ui->control4ComboBox->setEnabled(false);
        this->ui->control5Label->setEnabled(false);
        this->ui->control5LineEdit->setEnabled(false);
        this->ui->control5LineEdit->setText("N/A");
        this->ui->control5FunctionLabel->setEnabled(false);
        this->ui->control5ComboBox->setEnabled(false);
        this->ui->control6Label->setEnabled(false);
        this->ui->control6LineEdit->setEnabled(false);
        this->ui->control6LineEdit->setText("N/A");
        this->ui->control6FunctionLabel->setEnabled(false);
        this->ui->control6ComboBox->setEnabled(false);
    }
    else if(m_sFractalType == "Newton (Root)")
    {
        this->ui->control1Label->setText("End Relax (Re) :");
        this->ui->control1FunctionLabel->setText("Relax (Re) Function :");
        this->ui->control2Label->setText("End Relax (Im) :");
        this->ui->control2FunctionLabel->setText("Relax (Im) Function :");
        this->ui->control3Label->setText("End Tolerance :");
        this->ui->control3FunctionLabel->setText("Tolerance Function :");
        this->ui->control4Label->setEnabled(false);
        this->ui->control4LineEdit->setEnabled(false);
        this->ui->control4LineEdit->setText("N/A");
        this->ui->control4FunctionLabel->setEnabled(false);
        this->ui->control4ComboBox->setEnabled(false);
        this->ui->control5Label->setEnabled(false);
        this->ui->control5LineEdit->setEnabled(false);
        this->ui->control5LineEdit->setText("N/A");
        this->ui->control5FunctionLabel->setEnabled(false);
        this->ui->control5ComboBox->setEnabled(false);
        this->ui->control6Label->setEnabled(false);
        this->ui->control6LineEdit->setEnabled(false);
        this->ui->control6LineEdit->setText("N/A");
        this->ui->control6FunctionLabel->setEnabled(false);
        this->ui->control6ComboBox->setEnabled(false);
    }
    else if(m_sFractalType == "Nova")
    {
        this->ui->control1Label->setText("End Seed (Re) :");
        this->ui->control1FunctionLabel->setText("Seed (Re) Function :");
        this->ui->control2Label->setText("End Seed (Im) :");
        this->ui->control2FunctionLabel->setText("Seed (Im) Function :");
        this->ui->control3Label->setText("End Relax (Re) :");
        this->ui->control3FunctionLabel->setText("Relax (Re) Function :");
        this->ui->control4Label->setText("End Relax (Im) :");
        this->ui->control4FunctionLabel->setText("Relax (Im) Function :");
        this->ui->control5Label->setText("End Power) :");
        this->ui->control5FunctionLabel->setText("Power Function :");
        this->ui->control6Label->setText("End Deriv. Mult. :");
        this->ui->control6FunctionLabel->setText("Deriv. Mult. Function :");
    }
    else if(m_sFractalType == "Nova (Special)")
    {
        this->ui->control1Label->setText("End Relax (Re) :");
        this->ui->control1FunctionLabel->setText("Relax (Re) Function :");
        this->ui->control2Label->setText("End Relax (Im) :");
        this->ui->control2FunctionLabel->setText("Relax (Im) Function :");
        this->ui->control3Label->setText("End Power :");
        this->ui->control3FunctionLabel->setText("Power Function :");
        this->ui->control3Label->setText("End Deriv. Mult. :");
        this->ui->control3FunctionLabel->setText("Deriv. Mult. Function :");
        this->ui->control5Label->setEnabled(false);
        this->ui->control5LineEdit->setEnabled(false);
        this->ui->control5LineEdit->setText("N/A");
        this->ui->control5FunctionLabel->setEnabled(false);
        this->ui->control5ComboBox->setEnabled(false);
        this->ui->control6Label->setEnabled(false);
        this->ui->control6LineEdit->setEnabled(false);
        this->ui->control6LineEdit->setText("N/A");
        this->ui->control6FunctionLabel->setEnabled(false);
        this->ui->control6ComboBox->setEnabled(false);
    }
    else if(m_sFractalType == "Glynn")
    {
        this->ui->control1Label->setText("End Seed (Re) :");
        this->ui->control1FunctionLabel->setText("Seed (Re) Function :");
        this->ui->control2Label->setText("End Seed (Im) :");
        this->ui->control2FunctionLabel->setText("Seed (Im) Function :");
        this->ui->control3Label->setText("End Power :");
        this->ui->control3FunctionLabel->setText("Power Function :");
        this->ui->control4Label->setEnabled(false);
        this->ui->control4LineEdit->setEnabled(false);
        this->ui->control4LineEdit->setText("N/A");
        this->ui->control4FunctionLabel->setEnabled(false);
        this->ui->control4ComboBox->setEnabled(false);
        this->ui->control5Label->setEnabled(false);
        this->ui->control5LineEdit->setEnabled(false);
        this->ui->control5LineEdit->setText("N/A");
        this->ui->control5FunctionLabel->setEnabled(false);
        this->ui->control5ComboBox->setEnabled(false);
        this->ui->control6Label->setEnabled(false);
        this->ui->control6LineEdit->setEnabled(false);
        this->ui->control6LineEdit->setText("N/A");
        this->ui->control6FunctionLabel->setEnabled(false);
        this->ui->control6ComboBox->setEnabled(false);
    }
    else if(m_sFractalType == "Phoenix")
    {
        this->ui->control1Label->setText("End P1 (Re) :");
        this->ui->control1FunctionLabel->setText("P1 (Re) Function :");
        this->ui->control2Label->setText("End P1 (Im) :");
        this->ui->control2FunctionLabel->setText("P2 (Im) Function :");
        this->ui->control3Label->setText("End P2 (Re) :");
        this->ui->control3FunctionLabel->setText("P2 (Re) Function :");
        this->ui->control4Label->setText("End P2 (Im) :");
        this->ui->control4FunctionLabel->setText("P1 (Im) Function :");
        this->ui->control5Label->setText("End Power :");
        this->ui->control5FunctionLabel->setText("Power Function :");
        this->ui->control6Label->setEnabled(false);
        this->ui->control6LineEdit->setEnabled(false);
        this->ui->control6LineEdit->setText("N/A");
        this->ui->control6FunctionLabel->setEnabled(false);
        this->ui->control6ComboBox->setEnabled(false);
    }
}

void AnimationProperties::setAnimationOutsideColoringPropertiesData(QList<int> iAnimOutsideRedAdder, QList<int> iAnimOutsideRedAddFunction,
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

    this->ui->outsideRedAdderLineEdit->setText(QString::number(m_iAnimOutsideRedAdder[0]));
    this->ui->outsideRedAdderFunctionComboBox->setCurrentIndex(m_iAnimOutsideRedAddFunction[0]);
    this->ui->outsideGreenAdderLineEdit->setText(QString::number(m_iAnimOutsideGreenAdder[0]));
    this->ui->outsideGreenAdderFunctionComboBox->setCurrentIndex(m_iAnimOutsideGreenAddFunction[0]);
    this->ui->outsideBlueAdderLineEdit->setText(QString::number(m_iAnimOutsideBlueAdder[0]));
    this->ui->outsideBlueAdderFunctionComboBox->setCurrentIndex(m_iAnimOutsideBlueAddFunction[0]);
    this->ui->outsideRedMultLineEdit->setText(QString::number(m_dAnimOutsideRedMultiplier[0]));
    this->ui->outsideRedMultFunctionComboBox->setCurrentIndex( m_iAnimOutsideRedMultFunction[0]);
    this->ui->outsideGreenMultLineEdit->setText(QString::number(m_dAnimOutsideGreenMultiplier[0]));
    this->ui->outsideGreenMultFunctionComboBox->setCurrentIndex( m_iAnimOutsideGreenMultFunction[0]);
    this->ui->outsideBlueMultLineEdit->setText(QString::number(m_dAnimOutsideBlueMultiplier[0]));
    this->ui->outsideBlueMultFunctionComboBox->setCurrentIndex( m_iAnimOutsideBlueMultFunction[0]);
}

void AnimationProperties::on_outsideRedAdderLineEdit_textChanged(const QString &arg1)
{
    m_iAnimOutsideRedAdder[m_iTriggerIndex] = QString(arg1).toInt();
}

void AnimationProperties::on_outsideRedAdderFunctionComboBox_currentIndexChanged(int index)
{
    m_iAnimOutsideRedAddFunction[m_iTriggerIndex] = index;
}

void AnimationProperties::on_outsideGreenAdderLineEdit_textChanged(const QString &arg1)
{
    m_iAnimOutsideGreenAdder[m_iTriggerIndex] = QString(arg1).toInt();
}

void AnimationProperties::on_outsideGreenAdderFunctionComboBox_currentIndexChanged(int index)
{
    m_iAnimOutsideGreenAddFunction[m_iTriggerIndex] = index;
}

void AnimationProperties::on_outsideBlueAdderLineEdit_textChanged(const QString &arg1)
{
    m_iAnimOutsideBlueAdder[m_iTriggerIndex] = QString(arg1).toInt();
}

void AnimationProperties::on_outsideBlueAdderFunctionComboBox_currentIndexChanged(int index)
{
    m_iAnimOutsideBlueAddFunction[m_iTriggerIndex] = index;
}

void AnimationProperties::on_outsideRedMultLineEdit_textChanged(const QString &arg1)
{
    m_dAnimOutsideRedMultiplier[m_iTriggerIndex] = QString(arg1).toDouble();
}

void AnimationProperties::on_outsideRedMultFunctionComboBox_currentIndexChanged(int index)
{
    m_iAnimOutsideRedMultFunction[m_iTriggerIndex] = index;
}

void AnimationProperties::on_outsideGreenMultLineEdit_textChanged(const QString &arg1)
{
    m_dAnimOutsideGreenMultiplier[m_iTriggerIndex] = QString(arg1).toDouble();
}

void AnimationProperties::on_outsideGreenMultFunctionComboBox_currentIndexChanged(int index)
{
    m_iAnimOutsideGreenMultFunction[m_iTriggerIndex] = index;
}

void AnimationProperties::on_outsideBlueMultLineEdit_textChanged(const QString &arg1)
{
    m_dAnimOutsideBlueMultiplier[m_iTriggerIndex] = QString(arg1).toDouble();
}

void AnimationProperties::on_outsideBlueMultFunctionComboBox_currentIndexChanged(int index)
{
    m_iAnimOutsideBlueMultFunction[m_iTriggerIndex] = index;
}

void AnimationProperties::setAnimationInsideColoringPropertiesData(QList<int> iAnimInsideRedAdder, QList<int> iAnimInsideRedAddFunction,
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

    this->ui->insideRedAdderLineEdit->setText(QString::number(m_iAnimInsideRedAdder[0]));
    this->ui->insideRedAdderFunctionComboBox->setCurrentIndex(m_iAnimInsideRedAddFunction[0]);
    this->ui->insideGreenAdderLineEdit->setText(QString::number(m_iAnimInsideGreenAdder[0]));
    this->ui->insideGreenAdderFunctionComboBox->setCurrentIndex(m_iAnimInsideGreenAddFunction[0]);
    this->ui->insideBlueAdderLineEdit->setText(QString::number(m_iAnimInsideBlueAdder[0]));
    this->ui->insideBlueAdderFunctionComboBox->setCurrentIndex(m_iAnimInsideBlueAddFunction[0]);
    this->ui->insideRedMultLineEdit->setText(QString::number(m_dAnimInsideRedMultiplier[0]));
    this->ui->insideRedMultFunctionComboBox->setCurrentIndex( m_iAnimInsideRedMultFunction[0]);
    this->ui->insideGreenMultLineEdit->setText(QString::number(m_dAnimInsideGreenMultiplier[0]));
    this->ui->insideGreenMultFunctionComboBox->setCurrentIndex( m_iAnimInsideGreenMultFunction[0]);
    this->ui->insideBlueMultLineEdit->setText(QString::number(m_dAnimInsideBlueMultiplier[0]));
    this->ui->insideBlueMultFunctionComboBox->setCurrentIndex( m_iAnimInsideBlueMultFunction[0]);
}

void AnimationProperties::on_insideRedAdderLineEdit_textChanged(const QString &arg1)
{
    m_iAnimInsideRedAdder[m_iTriggerIndex] = QString(arg1).toInt();
}

void AnimationProperties::on_insideRedAdderFunctionComboBox_currentIndexChanged(int index)
{
    m_iAnimInsideRedAddFunction[m_iTriggerIndex] = index;
}

void AnimationProperties::on_insideGreenAdderLineEdit_textChanged(const QString &arg1)
{
    m_iAnimInsideGreenAdder[m_iTriggerIndex] = QString(arg1).toInt();
}

void AnimationProperties::on_insideGreenAdderFunctionComboBox_currentIndexChanged(int index)
{
    m_iAnimInsideGreenAddFunction[m_iTriggerIndex] = index;
}

void AnimationProperties::on_insideBlueAdderLineEdit_textChanged(const QString &arg1)
{
    m_iAnimInsideBlueAdder[m_iTriggerIndex] = QString(arg1).toInt();
}

void AnimationProperties::on_insideBlueAdderFunctionComboBox_currentIndexChanged(int index)
{
    m_iAnimInsideBlueAddFunction[m_iTriggerIndex] = index;
}

void AnimationProperties::on_insideRedMultLineEdit_textChanged(const QString &arg1)
{
    m_dAnimInsideRedMultiplier[m_iTriggerIndex] = QString(arg1).toDouble();
}

void AnimationProperties::on_insideRedMultFunctionComboBox_currentIndexChanged(int index)
{
    m_iAnimInsideRedMultFunction[m_iTriggerIndex] = index;
}

void AnimationProperties::on_insideGreenMultLineEdit_textChanged(const QString &arg1)
{
    m_dAnimInsideGreenMultiplier[m_iTriggerIndex] = QString(arg1).toDouble();
}

void AnimationProperties::on_insideGreenMultFunctionComboBox_currentIndexChanged(int index)
{
    m_iAnimInsideGreenMultFunction[m_iTriggerIndex] = index;
}

void AnimationProperties::on_insideBlueMultLineEdit_textChanged(const QString &arg1)
{
    m_dAnimInsideBlueMultiplier[m_iTriggerIndex] = QString(arg1).toDouble();
}

void AnimationProperties::on_insideBlueMultFunctionComboBox_currentIndexChanged(int index)
{
    m_iAnimInsideBlueMultFunction[m_iTriggerIndex] = index;
}

void AnimationProperties::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return)
    {
        on_okButton_clicked();
    }
}
