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
        //Método que hace la transición entre START y STOP del botón implementado.
        void doButton();

        //Método que incrementa el valor mostrado en lcdNumber.
        void contador();

        //Método que modifica la velocidad de lcdNumber siempre y cuando button se encuentre en START.
        void periodo();

        //Método que cuenta el tiempo, en segundos, transcurrido desde el inicio del programa
        void elapsedTime();
};

#endif // ejemplo1_H
