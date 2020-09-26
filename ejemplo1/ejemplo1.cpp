#include "ejemplo1.h"

ejemplo1::ejemplo1(): Ui_Counter()
{
    setupUi(this);
    show();
    connect(button, SIGNAL(clicked()), this, SLOT(doButton()) );
    connect(&timer, SIGNAL(timeout()), this, SLOT(contador()) );
    connect(horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(periodo()) );
    connect(&timerElapsed, SIGNAL(timeout()), this, SLOT(elapsedTime()) );

    timer.start(period);
    timerElapsed.start(elapsed);



}

void ejemplo1::doButton()
{
    //qDebug() << "click on button";
    button->setText("STOP");
    if(button->isChecked()){
        button->setText("START");
        timer.stop();
    }else{
        timer.start();
    }

}
void ejemplo1::contador(){
    static int i =  0 ;
    //qDebug() << "timeout";
    lcdNumber->display(i++);
}

void ejemplo1::periodo(){
   period = horizontalSlider->value();
   timer.start(period);
}

void ejemplo1::elapsedTime(){
    static int j = 0;
    elapsedLcd->display(j++);

}






