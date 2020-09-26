#ifndef ejemplo1_H
#define ejemplo1_H

#include <QtGui>
#include "ui_counterDlg.h"

class ejemplo1 : public QWidget, public Ui_Counter
{
    Q_OBJECT
    public:
        ejemplo1();

    private:
        QTimer timer, timerElapsed;
        int period = 500;
        int elapsed = 1000;

    public slots:
        void doButton();
        void contador();
        void periodo();
        void elapsedTime();
};

#endif // ejemplo1_H
