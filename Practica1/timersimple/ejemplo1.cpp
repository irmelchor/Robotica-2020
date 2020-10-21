#include "ejemplo1.h"


ejemplo1::ejemplo1(): Ui_Counter()
{
	setupUi(this);
	show();
	connect(button, SIGNAL(clicked()), this, SLOT(doButton()));
    connect(horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(periodo()));

    mytimer.connect(std::bind(&ejemplo1::cuenta, this));
	mytimerLong.connect(std::bind(&ejemplo1::elapsedTime, this));

    mytimer.start(period);
    mytimerLong.start(1000);

}

ejemplo1::~ejemplo1()
{}

void ejemplo1::doButton()
{
	static bool stopped = false;
	stopped = !stopped;
    button->setText("STOP");
    if(stopped) {
        button->setText("START");
        mytimer.stop();
    }else
		mytimer.start(500);
	qDebug() << "click on button";
}

void ejemplo1::cuenta()
{
    lcdNumber->display(++cont);
	trick++;
}

void ejemplo1::periodo(){
    if(!button->isChecked()) {
        period = horizontalSlider->value();
        mytimer.start(period);
    }
}

void ejemplo1::elapsedTime(){
    elapsedLcd->display(++j);
}

