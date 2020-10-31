/*
 *    Copyright (C) 2020 by Marta García Tornero, Irene Melchor Félix
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "specificworker.h"

/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(TuplePrx tprx, bool startup_check) : GenericWorker(tprx)
{
	this->startup_check_flag = startup_check;
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{
	std::cout << "Destroying SpecificWorker" << std::endl;
}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
	//	THE FOLLOWING IS JUST AN EXAMPLE
	//	To use innerModelPath parameter you should uncomment specificmonitor.cpp readConfig method content
	//	try
	//	{
	//		RoboCompCommonBehavior::Parameter par = params.at("InnerModelPath");
	//		std::string innermodel_path = par.value;
	//		innerModel = std::make_shared(innermodel_path);
	//	}
	//	catch(const std::exception &e) { qFatal("Error reading config params"); }

	return true;
}

void SpecificWorker::initialize(int period)
{
	std::cout << "Initialize worker" << std::endl;
	this->Period = period;
	if (this->startup_check_flag)
	{
		this->startup_check();
	}
	else
	{
		timer.start(Period);
	}
}
/* 
void SpecificWorker::girar()
{
	this->differentialrobot_proxy->setSpeedBase(500, 0.8);
	usleep(rand() % (1500000 - 100000 + 1) + 100000);

	//estado = 1;
}

void SpecificWorker::avanzar(float adv_speed)
{
	this->differentialrobot_proxy->setSpeedBase(adv_speed, 0);
	//estado = 0;
}
 */
/*
 * Módulo que sirve para calcular el segundo parametro de la formula
 * adv_speed = MAX_ADV_SPEED * reduce_speed_if_turning * reduce_speed_if close_to_target
 */
float SpecificWorker::reduce_speed_if_turning(float rot_speed, float s, float x){
	float y=0.0;
	y= (-x/log(s));
	return exp((-pow(rot_speed,2))/y);
}

/*
 * Módulo que sirve para calcular la función exponencial de la distancia
 * a la que nos encontramos del target
 */
float SpecificWorker::reduce_speed_if_close_to_target(float dist){

	return std::min((dist/1000),1.f);
}

void SpecificWorker::compute()
{

	RoboCompGenericBase::TBaseState bState;
	differentialrobot_proxy->getBaseState(bState);

	if (auto t_o = target.get(); t_o.has_value())
	{ //Si bandera activa, obtenemos valores
		auto tw = t_o.value();
		Eigen::Vector2f rw(bState.x, bState.z);
		Eigen::Matrix2f rot;
		rot << cos(bState.alpha), sin(bState.alpha), -sin(bState.alpha), cos(bState.alpha);
		auto tr = rot.transpose() * (tw - rw); // TARGET EN EL ROBOT
		auto beta = atan2(tr.x(), tr.y());	   //Angulo
		std::cout << tw << " " << rw << " " << rot << " " << beta << std::endl;
		auto dist = tr.norm(); //Distancia a recorrer

		/*********NUEVO***************/
		float rot_speed=beta;
		//adv_speed = MAX_ADV_SPEED * reduce_speed_if_turning * reduce_speed_if_close_to_target
		//Para hacer la ecuación de arriba necesitamos averiguar s con la ecuacion de abajo, utilzando logaritmos.
		//y = exp(-x2/s) ; Esto será igual a 0,1 cuando x sea 0,5 
		//Una vez tenemos s averifuada, ese será nuestro primer parámetro a multiplicar
		//float adv_speed = 1000 * exp (-beta2 / s) *;
		//El segundo parámetro sera 1 si está lejos del objetivo 0 si no.

		if(dist<30){
			target.active=false;
			differentialrobot_proxy->setSpeedBase(0,0);
		}
		else{
			auto adv_speed=(1000 * reduce_speed_if_turning(rot_speed, 0.1, 0.5) * reduce_speed_if_close_to_target(dist));
			differentialrobot_proxy->setSpeedBase(adv_speed, beta);
		}
		 

/*
		switch (estado)
		{
		case 0: //GIRAR: mientras fabs beta > 0.05
			//if (beta > 0.05)
			//{ 
				girar();
		//	}
			break;
		case 1: //AVANZAR: mientras dist > 30
			if (dist > 30)
			{ 
				avanzar();
			}
			break;
		}*/
	}
}

int SpecificWorker::startup_check()
{
	std::cout << "Startup check" << std::endl;
	QTimer::singleShot(200, qApp, SLOT(quit()));
	return 0;
}

//SUBSCRIPTION to setPick method from RCISMousePicker interface
void SpecificWorker::RCISMousePicker_setPick(RoboCompRCISMousePicker::Pick myPick)
{
	std::cout << "PRESSED ON: X: " << myPick.x << " Y: " << myPick.y;
	target.put(Eigen::Vector2f(myPick.x, myPick.y));
}

/**************************************/
// From the RoboCompDifferentialRobot you can call this methods:
// this->differentialrobot_proxy->correctOdometer(...)
// this->differentialrobot_proxy->getBasePose(...)
// this->differentialrobot_proxy->getBaseState(...)
// this->differentialrobot_proxy->resetOdometer(...)
// this->differentialrobot_proxy->setOdometer(...)
// this->differentialrobot_proxy->setOdometerPose(...)
// this->differentialrobot_proxy->setSpeedBase(...)
// this->differentialrobot_proxy->stopBase(...)

/**************************************/
// From the RoboCompDifferentialRobot you can use this types:
// RoboCompDifferentialRobot::TMechParams

/**************************************/
// From the RoboCompLaser you can call this methods:
// this->laser_proxy->getLaserAndBStateData(...)
// this->laser_proxy->getLaserConfData(...)
// this->laser_proxy->getLaserData(...)

/**************************************/
// From the RoboCompLaser you can use this types:
// RoboCompLaser::LaserConfData
// RoboCompLaser::TData

/**************************************/
// From the RoboCompRCISMousePicker you can use this types:
// RoboCompRCISMousePicker::Pick
