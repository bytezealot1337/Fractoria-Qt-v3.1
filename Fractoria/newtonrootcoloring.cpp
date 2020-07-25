#include "newtonrootcoloring.h"
#include "ui_newtonrootcoloring.h"
#include <QMouseEvent>

NewtonRootColoring::NewtonRootColoring(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewtonRootColoring)
{
    ui->setupUi(this);
}

NewtonRootColoring::~NewtonRootColoring()
{
    delete ui;
}

void NewtonRootColoring::on_firstRootRedLineEdit_textChanged(const QString &arg1)
{
    m_iFirstRootRed = QString(arg1).toInt();
}

void NewtonRootColoring::on_firstRootGreenLineEdit_textChanged(const QString &arg1)
{
    m_iFirstRootGreen = QString(arg1).toInt();
}

void NewtonRootColoring::on_firstRootBlueLineEdit_textChanged(const QString &arg1)
{
    m_iFirstRootBlue = QString(arg1).toInt();
}

void NewtonRootColoring::on_secondRootRedLineEdit_textChanged(const QString &arg1)
{
    m_iSecondRootRed = QString(arg1).toInt();
}

void NewtonRootColoring::on_secondRootGreenLineEdit_textChanged(const QString &arg1)
{
    m_iSecondRootGreen = QString(arg1).toInt();
}

void NewtonRootColoring::on_secondRootBlueLineEdit_textChanged(const QString &arg1)
{
    m_iSecondRootBlue = QString(arg1).toInt();
}

void NewtonRootColoring::on_thirdRootRedLineEdit_textChanged(const QString &arg1)
{
    m_iThirdRootRed = QString(arg1).toInt();
}

void NewtonRootColoring::on_thirdRootGreenLineEdit_textChanged(const QString &arg1)
{
    m_iThirdRootGreen = QString(arg1).toInt();
}

void NewtonRootColoring::on_thirdRootBlueLineEdit_textChanged(const QString &arg1)
{
    m_iThirdRootBlue = QString(arg1).toInt();
}

void NewtonRootColoring::on_okButton_clicked()
{
    emit finished(m_iFirstRootRed, m_iFirstRootGreen, m_iFirstRootBlue,
                  m_iSecondRootRed, m_iSecondRootGreen, m_iSecondRootBlue,
                  m_iThirdRootRed, m_iThirdRootGreen, m_iThirdRootBlue);

    this->close();
}

void NewtonRootColoring::setNewtonRootColoringData(int iFirstRootRed, int iFirstRootGreen, int iFirstRootBlue,
                                                   int iSecondRootRed, int iSecondRootGreen, int iSecondRootBlue,
                                                   int iThirdRootRed, int iThirdRootGreen, int iThirdRootBlue)
{
    m_iFirstRootRed = iFirstRootRed;
    this->ui->firstRootRedLineEdit->setText(QString::number(m_iFirstRootRed));
    m_iFirstRootGreen = iFirstRootGreen;
    this->ui->firstRootGreenLineEdit->setText(QString::number(m_iFirstRootGreen));
    m_iFirstRootBlue = iFirstRootBlue;
    this->ui->firstRootBlueLineEdit->setText(QString::number(m_iFirstRootBlue));

    m_iSecondRootRed = iSecondRootRed;
    this->ui->secondRootRedLineEdit->setText(QString::number(m_iSecondRootRed));
    m_iSecondRootGreen = iSecondRootGreen;
    this->ui->secondRootGreenLineEdit->setText(QString::number(m_iSecondRootGreen));
    m_iSecondRootBlue = iSecondRootBlue;
    this->ui->secondRootBlueLineEdit->setText(QString::number(m_iSecondRootBlue));

    m_iThirdRootRed = iThirdRootRed;
    this->ui->thirdRootRedLineEdit->setText(QString::number(m_iThirdRootRed));
    m_iThirdRootGreen = iThirdRootGreen;
    this->ui->thirdRootGreenLineEdit->setText(QString::number(m_iThirdRootGreen));
    m_iThirdRootBlue = iThirdRootBlue;
    this->ui->thirdRootBlueLineEdit->setText(QString::number(m_iThirdRootBlue));
}

void NewtonRootColoring::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return)
    {
        on_okButton_clicked();
    }
}
