#include "coloring.h"
#include "ui_coloring.h"
#include <QMouseEvent>

Coloring::Coloring(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Coloring)
{
    ui->setupUi(this);

    QStringList coloringList = (QStringList()<<"Bailout Linear"<<"Smooth Normalization");
    this->ui->coloringAlgorithmComboBox->addItems(coloringList);

    QStringList attributesList = (QStringList()<<"Regular"<<"Reverse");
    this->ui->algorithmAttributesComboBox->addItems(attributesList);

    coloringAlgorithmValueCheck();
}

Coloring::~Coloring()
{
    delete ui;
}

void Coloring::on_coloringAlgorithmComboBox_currentIndexChanged(int index)
{
    m_iColoringAlgorithm = index;

    coloringAlgorithmValueCheck();
}

void Coloring::on_algorithmAttributesComboBox_currentIndexChanged(int index)
{
    m_iAlgorithmAttributes = index;

    coloringAlgorithmValueCheck();
}

void Coloring::on_redMultiplier_textChanged(const QString &arg1)
{
    m_dRedMultiplier = QString(arg1).toDouble();
}

void Coloring::on_greenMultiplier_textChanged(const QString &arg1)
{
    m_dGreenMultiplier = QString(arg1).toDouble();
}

void Coloring::on_blueMultiplier_textChanged(const QString &arg1)
{
    m_dBlueMultiplier = QString(arg1).toDouble();
}

void Coloring::on_redMainColor_toggled(bool checked)
{
    m_bRedMainColor = checked;
}

void Coloring::on_greenMainColor_toggled(bool checked)
{
    m_bGreenMainColor = checked;
}

void Coloring::on_blueMainColor_toggled(bool checked)
{
    m_bBlueMainColor = checked;
}

void Coloring::on_okButton_clicked()
{
    emit finished(m_iColoringAlgorithm, m_iAlgorithmAttributes,
                  m_dRedMultiplier, m_dGreenMultiplier, m_dBlueMultiplier,
                  m_bRedMainColor, m_bGreenMainColor, m_bBlueMainColor);

    this->close();
}

void Coloring::coloringAlgorithmValueCheck()
{
    if(this->ui->coloringAlgorithmComboBox->currentText() == "Bailout Linear")
    {
        this->ui->algorithmAttributesComboBox->setEnabled(false);

        this->ui->redMultiplier->setEnabled(false);
        this->ui->greenMultiplier->setEnabled(false);
        this->ui->blueMultiplier->setEnabled(false);

        this->ui->redMainColor->setEnabled(false);
        this->ui->greenMainColor->setEnabled(false);
        this->ui->blueMainColor->setEnabled(false);
    }
    else if(this->ui->coloringAlgorithmComboBox->currentText() == "Smooth Normalization")
    {
        this->ui->algorithmAttributesComboBox->setEnabled(true);

        this->ui->redMultiplier->setEnabled(true);
        this->ui->greenMultiplier->setEnabled(true);
        this->ui->blueMultiplier->setEnabled(true);

        if(this->ui->algorithmAttributesComboBox->currentText() == "Regular")
        {
            this->ui->redMainColor->setEnabled(false);
            this->ui->greenMainColor->setEnabled(false);
            this->ui->blueMainColor->setEnabled(false);
        }
        else if(this->ui->algorithmAttributesComboBox->currentText() == "Reverse")
        {
            this->ui->redMainColor->setEnabled(true);
            this->ui->greenMainColor->setEnabled(true);
            this->ui->blueMainColor->setEnabled(true);
        }
    }
}

void Coloring::setColoringData(int iColoringAlgorithm, int iAlgorithmAttributes,
                               double dRedMultiplier, double dGreenMultiplier, double dBlueMultiplier,
                               bool bRedMainColor, bool bGreenMainColor, bool bBlueMainColor)
{
    m_iColoringAlgorithm = iColoringAlgorithm;
    this->ui->coloringAlgorithmComboBox->setCurrentIndex(m_iColoringAlgorithm);
    m_iAlgorithmAttributes = iAlgorithmAttributes;
    this->ui->algorithmAttributesComboBox->setCurrentIndex(m_iAlgorithmAttributes);

    m_dRedMultiplier = dRedMultiplier;
    this->ui->redMultiplier->setText(QString::number(m_dRedMultiplier));
    m_dGreenMultiplier = dGreenMultiplier;
    this->ui->greenMultiplier->setText(QString::number(m_dGreenMultiplier));
    m_dBlueMultiplier = dBlueMultiplier;
    this->ui->blueMultiplier->setText(QString::number(m_dBlueMultiplier));

    m_bRedMainColor = bRedMainColor;
    this->ui->redMainColor->setChecked(m_bRedMainColor);
    m_bGreenMainColor = bGreenMainColor;
    this->ui->greenMainColor->setChecked(m_bGreenMainColor);
    m_bBlueMainColor = bBlueMainColor;
    this->ui->blueMainColor->setChecked(m_bBlueMainColor);
}

void Coloring::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return)
    {
        on_okButton_clicked();
    }
}
