#include "options.h"
#include "ui_options.h"
#include <QMouseEvent>

Options::Options(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
}

Options::~Options()
{
    delete ui;
}

void Options::on_showProgressionCheckBox_toggled(bool checked)
{
    m_bShowProgression = checked;
}

void Options::on_okButton_clicked()
{
    emit showProgressionParameter(m_bShowProgression);

    this->close();
}

void Options::setShowProgressionParameter(bool bShowProgression)
{
    m_bShowProgression = bShowProgression;

    this->ui->showProgressionCheckBox->setChecked(m_bShowProgression);
}


void Options::on_resetAnimationPropertiesButton_clicked()
{
    m_bFirstTime = true;

    emit animationFirstTimeParameter(m_bFirstTime);
}

void Options::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return)
    {
        on_okButton_clicked();
    }
}
