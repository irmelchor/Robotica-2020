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
        QTimer timer;
        int period = 500;

    public slots:
        void doButton();
        void contador();
        void periodo();
};

#endif // ejemplo1_H
