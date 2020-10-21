#ifndef ejemplo1_H
#define ejemplo1_H

#include <QtGui>
#include "ui_counterDlg.h"
#include <chrono>
#include "timer.h"


class ejemplo1 : public QWidget, public Ui_Counter
{
Q_OBJECT
    public:
        ejemplo1();
        virtual ~ejemplo1();
    
    public slots:
    //Método que incrementa el valor mostrado en lcdNumber.
	void doButton();
     //Método que modifica la velocidad de lcdNumber siempre y cuando button se encuentre en START.
    void periodo();

    private:
        Timer mytimer, mytimerLong;
        int cont = 0;
        int period = 500;
        int j = 0;


    // dos callbacks con diferente número de parámetros
        void cuenta();
		
		int trick = 5;


        //Método que cuenta el tiempo, en segundos, transcurrido desde el inicio del programa
		void elapsedTime();
};

#endif // ejemplo1_H
