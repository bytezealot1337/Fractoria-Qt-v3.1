#ifndef ANIMATIONPROPERTIES_H
#define ANIMATIONPROPERTIES_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>

#define TRIGGER_POINTS_NUMBER 8

namespace Ui {
class AnimationProperties;
}

class AnimationProperties : public QWidget
{
    Q_OBJECT

public:
    explicit AnimationProperties(QWidget *parent = 0);
    ~AnimationProperties();

    QString m_sFractalType;

    int m_iTotalFrameNumber;
    int m_iTriggerIndex;

    // Geometry Animation Variables
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

    // Formula Animation Variables
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

public slots:
    void setFractalType(QString sFractalType);


    void setAnimationGeometryPropertiesData(int iTotalFrameNumber, QList<int> iTriggerFrame, QList<double> dEndCenterX,
                                            QList<double> dEndCenterY, QList<int> iTranslationFunction, QList<double> dEndZoomRatio,
                                            QList<int> iZoomRatioFunction, QList<double> dEndRotation, QList<int> iRotationFunction,
                                            QList<long long> iEndIteration, QList<int> iIterationFunction);

    void setAnimationFormulaPropertiesData(QList<double> dAnimCtrl1, QList<int> iAnimCtrl1Function, QList<double> dAnimCtrl2,
                                           QList<int> iAnimCtrl2Function, QList<double> dAnimCtrl3, QList<int> iAnimCtrl3Function,
                                           QList<double> dAnimCtrl4, QList<int> iAnimCtrl4Function, QList<double> dAnimCtrl5,
                                           QList<int> iAnimCtrl5Function, QList<double> dAnimCtrl6, QList<int> iAnimCtrl6Function);


    void setAnimationOutsideColoringPropertiesData(QList<int> iAnimOutsideRedAdder, QList<int> iAnimOutsideRedAddFunction,
                                                   QList<int> iAnimOutsideGreenAdder, QList<int> iAnimOutsideGreenAddFunction,
                                                   QList<int> iAnimOutsideBlueAdder, QList<int> iAnimOutsideBlueAddFunction,
                                                   QList<double> dAnimOutsideRedMultiplier, QList<int> iAnimOutsideRedMultFunction,
                                                   QList<double> dAnimOutsideGreenMultiplier, QList<int> iAnimOutsideGreenMultFunction,
                                                   QList<double> dAnimOutsideBlueMultiplier, QList<int> iAnimOutsideBlueMultFunction);

    void setAnimationInsideColoringPropertiesData(QList<int> iAnimInsideRedAdder, QList<int> iAnimInsideRedAddFunction,
                                                  QList<int> iAnimInsideGreenAdder, QList<int> iAnimInsideGreenAddFunction,
                                                  QList<int> iAnimInsideBlueAdder, QList<int> iAnimInsideBlueAddFunction,
                                                  QList<double> dAnimInsideRedMultiplier, QList<int> iAnimInsideRedMultFunction,
                                                  QList<double> dAnimInsideGreenMultiplier, QList<int> iAnimInsideGreenMultFunction,
                                                  QList<double> dAnimInsideBlueMultiplier, QList<int> iAnimInsideBlueMultFunction);

private slots:
    void on_okButton_clicked();

    void on_totalFrameNumberLineEdit_textChanged(const QString &arg1);
    void on_triggerPointComboBox_currentIndexChanged(int index);
    void on_triggerFrameLineEdit_textChanged(const QString &arg1);
    void on_endCenterXLineEdit_textChanged(const QString &arg1);
    void on_endCenterYLineEdit_textChanged(const QString &arg1);
    void on_translationFunctionComboBox_currentIndexChanged(int index);
    void on_endZoomRatioLineEdit_textChanged(const QString &arg1);
    void on_zoomRatioFunctionComboBox_currentIndexChanged(int index);
    void on_endRotationLineEdit_textChanged(const QString &arg1);
    void on_rotationFunctionComboBox_currentIndexChanged(int index);
    void on_endIterationLineEdit_textChanged(const QString &arg1);
    void on_iterationFunctionComboBox_currentIndexChanged(int index);

    void on_control1LineEdit_textChanged(const QString &arg1);
    void on_control1ComboBox_currentIndexChanged(int index);
    void on_control2LineEdit_textChanged(const QString &arg1);
    void on_control2ComboBox_currentIndexChanged(int index);
    void on_control3LineEdit_textChanged(const QString &arg1);
    void on_control3ComboBox_currentIndexChanged(int index);
    void on_control4LineEdit_textChanged(const QString &arg1);
    void on_control4ComboBox_currentIndexChanged(int index);
    void on_control5LineEdit_textChanged(const QString &arg1);
    void on_control5ComboBox_currentIndexChanged(int index);
    void on_control6LineEdit_textChanged(const QString &arg1);
    void on_control6ComboBox_currentIndexChanged(int index);

    void controlDefaultsFunction();

    void on_outsideRedAdderLineEdit_textChanged(const QString &arg1);
    void on_outsideRedAdderFunctionComboBox_currentIndexChanged(int index);
    void on_outsideGreenAdderLineEdit_textChanged(const QString &arg1);
    void on_outsideGreenAdderFunctionComboBox_currentIndexChanged(int index);
    void on_outsideBlueAdderLineEdit_textChanged(const QString &arg1);
    void on_outsideBlueAdderFunctionComboBox_currentIndexChanged(int index);
    void on_outsideRedMultLineEdit_textChanged(const QString &arg1);
    void on_outsideRedMultFunctionComboBox_currentIndexChanged(int index);
    void on_outsideGreenMultLineEdit_textChanged(const QString &arg1);
    void on_outsideGreenMultFunctionComboBox_currentIndexChanged(int index);
    void on_outsideBlueMultLineEdit_textChanged(const QString &arg1);
    void on_outsideBlueMultFunctionComboBox_currentIndexChanged(int index);

    void on_insideRedAdderLineEdit_textChanged(const QString &arg1);
    void on_insideRedAdderFunctionComboBox_currentIndexChanged(int index);
    void on_insideGreenAdderLineEdit_textChanged(const QString &arg1);
    void on_insideGreenAdderFunctionComboBox_currentIndexChanged(int index);
    void on_insideBlueAdderLineEdit_textChanged(const QString &arg1);
    void on_insideBlueAdderFunctionComboBox_currentIndexChanged(int index);
    void on_insideRedMultLineEdit_textChanged(const QString &arg1);
    void on_insideRedMultFunctionComboBox_currentIndexChanged(int index);
    void on_insideGreenMultLineEdit_textChanged(const QString &arg1);
    void on_insideGreenMultFunctionComboBox_currentIndexChanged(int index);
    void on_insideBlueMultLineEdit_textChanged(const QString &arg1);
    void on_insideBlueMultFunctionComboBox_currentIndexChanged(int index);

    void keyPressEvent(QKeyEvent *event);

private:
    Ui::AnimationProperties *ui;

signals:
    void geometryFinished(int, QList<int>, QList<double>, QList<double>, QList<int>, QList<double>,
                          QList<int>, QList<double>, QList<int>, QList<long long>, QList<int>);

    void formulaFinished(QList<double>, QList<int>, QList<double>, QList<int>, QList<double>, QList<int>,
                         QList<double>, QList<int>, QList<double>, QList<int>, QList<double>, QList<int>);

    void outsideColoringFinished(QList<int>, QList<int>, QList<int>, QList<int>, QList<int>, QList<int>,
                                 QList<double>, QList<int>, QList<double>, QList<int>, QList<double>, QList<int>);

    void insideColoringFinished(QList<int>, QList<int>, QList<int>, QList<int>, QList<int>, QList<int>,
                                 QList<double>, QList<int>, QList<double>, QList<int>, QList<double>, QList<int>);
};

#endif // ANIMATIONPROPERTIES_H
