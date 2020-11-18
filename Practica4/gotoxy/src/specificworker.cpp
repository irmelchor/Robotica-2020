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
 * Módulo que sirve para calcular el segundo parametro de la formula
 * adv_speed = MAX_ADV_SPEED * reduce_speed_if_turning * reduce_speed_if close_to_target
 */
float SpecificWorker::reduce_speed_if_turning(float rot_speed, float s, float x)
{
	float y = 0.0;
	y = (-x / log(s));
	return exp((-pow(rot_speed, 2)) / y);
}

/*
 * Módulo que sirve para calcular la función exponencial de la distancia
 * a la que nos encontramos del target
 */
float SpecificWorker::reduce_speed_if_close_to_target(float dist)
{

	return std::min((dist / 1000), 1.f);
}


std::vector<SpecificWorker::tupla> SpecificWorker::calcularPuntos(float vOrigen, float wOrigen)
{
   std::vector<tupla> vectorT; //vector de tuplas <x,y,av,giro>
   std::cout << "////////////////////////////" << std::endl;
   for (float dt = 0.3; dt < 1.5; dt += 0.1)  // several steps in the future
   {
       for (float v = -1000; v <= 1000; v += 100) // several advance velocities
       {
           for (float w = -1; w <= 1; w += 0.1) // several rotacion velocities
           {
               float vNuevo = vOrigen + v;
               float wNuevo = wOrigen + w;
               if (fabs(w) > 0.1) {
                   // Coordenadas posibles después de moverse imaginariamente
                   float r = vNuevo / wNuevo; //radio
                   float x = r - r * cos(wNuevo * dt);
                   float y = r * sin(wNuevo * dt);
                   float alp = wNuevo * dt;
                   std::cout << __FUNCTION__ << " " << x << " " << y << " " << r << " " << vNuevo << " " << wNuevo
                             << " " << std::endl;
                   vectorT.emplace_back(std::make_tuple(x, y, vNuevo, wNuevo, alp)); //lo añadimos al vector de tuplas
               } else // para evitar la división por cero en el cálculo de r
                   vectorT.emplace_back(
                           std::make_tuple(0, v * dt, vNuevo, wNuevo, wNuevo * dt)); //lo añadimos al vector de tuplas
           }
       }
   }
   return vectorT;
}

void SpecificWorker::ordenarVector(std::vector <SpecificWorker::tupla> vPuntos,RoboCompGenericBase::TBaseState bState)
{
	std::sort(vPuntos.begin(), vPuntos.end());
}


//Comprueba los puntos dentro del poligono del laser
/*bool SpecificWorker::checkPointsInsideLaserPolygon(RoboCompLaser::TLaserData ldata,){
	// create laser polygon
	QPolygonF laser_poly;
	QPoint& point(bState.x,bState.z);
	for( auto &l : ldata)
   		laser_poly << QPointF(l.dist * sin(l.angle), l.dist * cos(l.angle));
    // check intersection
	if (laser_poly.containsPoint(point))  // point to check. Must be in robot’s coordinate system
    	return true;
	else
    	return false;
}*/

void SpecificWorker::compute()
{

	RoboCompGenericBase::TBaseState bState;
	differentialrobot_proxy->getBaseState(bState);
	RoboCompLaser::TLaserData ldata = laser_proxy->getLaserData();


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
		float rot_speed = beta;
		auto adv_speed = (1000 * reduce_speed_if_turning(rot_speed, 0.1, 0.5) * reduce_speed_if_close_to_target(dist));
		//differentialrobot_proxy->setSpeedBase(adv_speed, beta);
		
		float vOrigen =bState.advVz;
		float wOrigen = bState.rotV;

		std::vector <tupla> vPuntos = calcularPuntos(vOrigen, wOrigen);
		//sort
		ordenarVector(vPuntos, bState);
		//differentialrobot_proxy->setSpeedBase(vPuntos.vNuevo, vPuntos.wNuevo);


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
	std::cout << "PRESSED ON: X: " << myPick.x << " Z: " << myPick.z;
	target.put(Eigen::Vector2f(myPick.x, myPick.z));
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
