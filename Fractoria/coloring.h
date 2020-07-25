#ifndef COLORING_H
#define COLORING_H

#include <QWidget>

namespace Ui {
class Coloring;
}

class Coloring : public QWidget
{
    Q_OBJECT

public:
    explicit Coloring(QWidget *parent = 0);
    ~Coloring();

    int m_iColoringAlgorithm;
    int m_iAlgorithmAttributes;

    double m_dRedMultiplier;
    double m_dGreenMultiplier;
    double m_dBlueMultiplier;

    bool m_bRedMainColor;
    bool m_bGreenMainColor;
    bool m_bBlueMainColor;

public slots:
    void setColoringData(int iColoringAlgorithm, int iAlgorithmAttributes,
                         double dRedMultiplier, double dGreenMultiplier, double dBlueMultiplier,
                         bool bRedMainColor, bool bGreenMainColor, bool bBlueMainColor);

private slots:
    void on_coloringAlgorithmComboBox_currentIndexChanged(int index);
    void on_algorithmAttributesComboBox_currentIndexChanged(int index);

    void coloringAlgorithmValueCheck();

    void on_redMultiplier_textChanged(const QString &arg1);
    void on_greenMultiplier_textChanged(const QString &arg1);
    void on_blueMultiplier_textChanged(const QString &arg1);

    void on_redMainColor_toggled(bool checked);
    void on_greenMainColor_toggled(bool checked);
    void on_blueMainColor_toggled(bool checked);

    void on_okButton_clicked();

    void keyPressEvent(QKeyEvent *event);

private:
    Ui::Coloring *ui;

signals:
    void finished(int m_iColoringAlgorithm, int m_iAlgorithmAttributes,
                  double m_dRedMultiplier, double m_dGreenMultiplier, double m_dBlueMultiplier,
                  bool m_bRedMainColor, bool m_bGreenMainColor, bool m_bBlueMainColor);
};

#endif // COLORING_H
