/********************************************************************************
** Form generated from reading UI file 'counterDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COUNTERDLG_H
#define UI_COUNTERDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Counter
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_3;
    QLCDNumber *lcdNumber;
    QPushButton *button;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLCDNumber *elapsedLcd;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSlider *horizontalSlider;
    QLCDNumber *periodLcd;

    void setupUi(QWidget *Counter)
    {
        if (Counter->objectName().isEmpty())
            Counter->setObjectName(QString::fromUtf8("Counter"));
        Counter->resize(457, 406);
        verticalLayout = new QVBoxLayout(Counter);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget_2 = new QWidget(Counter);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        verticalLayout_3 = new QVBoxLayout(widget_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        lcdNumber = new QLCDNumber(widget_2);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));

        verticalLayout_3->addWidget(lcdNumber);

        button = new QPushButton(widget_2);
        button->setObjectName(QString::fromUtf8("button"));
        button->setMinimumSize(QSize(306, 0));
        button->setCheckable(true);

        verticalLayout_3->addWidget(button);


        verticalLayout->addWidget(widget_2);

        widget_3 = new QWidget(Counter);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        horizontalLayout = new QHBoxLayout(widget_3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(widget_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        elapsedLcd = new QLCDNumber(widget_3);
        elapsedLcd->setObjectName(QString::fromUtf8("elapsedLcd"));

        horizontalLayout->addWidget(elapsedLcd);


        verticalLayout->addWidget(widget_3);

        widget = new QWidget(Counter);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        horizontalSlider = new QSlider(widget);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setMinimum(10);
        horizontalSlider->setMaximum(2000);
        horizontalSlider->setValue(500);
        horizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(horizontalSlider);

        periodLcd = new QLCDNumber(widget);
        periodLcd->setObjectName(QString::fromUtf8("periodLcd"));
        periodLcd->setProperty("intValue", QVariant(500));

        horizontalLayout_2->addWidget(periodLcd);


        verticalLayout->addWidget(widget);


        retranslateUi(Counter);
        QObject::connect(horizontalSlider, SIGNAL(valueChanged(int)), periodLcd, SLOT(display(int)));

        QMetaObject::connectSlotsByName(Counter);
    } // setupUi

    void retranslateUi(QWidget *Counter)
    {
        Counter->setWindowTitle(QApplication::translate("Counter", "Counter", nullptr));
        button->setText(QApplication::translate("Counter", "STOP", nullptr));
        label_2->setText(QApplication::translate("Counter", "Elapsed Time", nullptr));
        label->setText(QApplication::translate("Counter", "Period", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Counter: public Ui_Counter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COUNTERDLG_H
