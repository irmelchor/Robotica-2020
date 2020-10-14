/*
 *    Copyright (C) 2020 by YOUR NAME HERE
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
SpecificWorker::SpecificWorker(MapPrx& mprx, bool startup_check) : GenericWorker(mprx)
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
//  THE FOLLOWING IS JUST AN EXAMPLE
//  To use innerModelPath parameter you should uncomment specificmonitor.cpp readConfig method content
//  try
//  {
//    RoboCompCommonBehavior::Parameter par = params.at("InnerModelPath");
//    std::string innermodel_path = par.value;
//    innerModel = std::make_shared(innermodel_path);
//  }
//  catch(const std::exception &e) { qFatal("Error reading config params"); }

  return true;
}


void SpecificWorker::initialize(int period)
{
	std::cout << "Initialize worker" << std::endl;
	this->Period = period;
	if(this->startup_check_flag)
	{
		this->startup_check();
	}
	else
	{
		timer.start(Period);
	}

}

void SpecificWorker::initializeMatrix(){
 	for( long int i = 0 ; i < 5000 ; i++)
 		for( long int j = 0 ; j < 5000 ; j++)
			map[i][j]=false;
	//std::cout << "Map size: " << map.size() << std::endl;
}


void SpecificWorker::checkMatrix(int x, int z, float alpha)
{
	x = x+2500;
	z = z+2500;



	vec vecinos[9]={
		vecinos[0]= {x-1,z-1},
		vecinos[1]={x,z-1},
		vecinos[2]={x+1,z-1},
		vecinos[3]={x-1,z},
		vecinos[4]={x,z},
		vecinos[5]={x+1,z},
		vecinos[6]={x-1,z+1},
		vecinos[7]={x,z+1},
		vecinos[8]={x+1,z+1}
		};

    if(map[x][z]== false){
        map[x][z] = true;

		for(int k=0; k<9;k++){
				if(map[x][z]!=map[vecinos[k].v1][vecinos[k].v2])
					differentialrobot_proxy->setSpeedBase(500, 0.8);
				else
					differentialrobot_proxy->setSpeedBase(500, 0);

		}
	}else{
		std::cout << "ERRORRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR" << std::endl;
	}


}


/*void SpecificWorker::checkMatrix(int x, int z){
	//std::cout << "x: " <<x << ", " << "z: " << z << std::endl;

	for(long int i=0; i<5000;i++){
		for(long int j=0; j<5000;j++){

			if(map[i][j]==map[x][z]){
				map[i][j]==true;
			}
		}
	}
}*/

//Metodo que sirve para que el robot gire para evitar objetos
void SpecificWorker::turnMethod(RoboCompLaser::TLaserData ldata, float rot){
    std::cout << "DISTANCIA " <<ldata.front().dist << std::endl;
    if(ldata.front().dist < 500){
        differentialrobot_proxy->setSpeedBase(200, 0);
        std::cout << "ESTOY BAJANDO LA VELOCIDADDDDD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;

    }
        differentialrobot_proxy->setSpeedBase(5, rot);
   
    usleep(rand()%(1500000-100000 + 1) + 100000);  // random wait between 1.5s and 0.1sec

}

//Metodo que sirve para avanzar hacia delante
void SpecificWorker::goAheadMethod(){
    differentialrobot_proxy->setSpeedBase(1000, 0);
    //differentialrobot_proxy->setSpeedBase(200, 0);
}

void SpecificWorker::checkDirection(){
    int x = 0, z = 0;
                float alpha=0;
                this->differentialrobot_proxy->getBasePose(x, z, alpha);
                /*std::cout << "X: "<< x << std::endl;
                std::cout << "Z: "<< z << std::endl;
                std::cout << "ALPHA: "<< alpha << std::endl;*/

                /******************************/
                checkMatrix(x,z, alpha);
                /******************************/
}


void SpecificWorker::compute()
{
 const float threshold = 200; // 200 millimeters
    float rot = 0.6; //0.6  // rads per second
    try
    {
       
        // read laser data
        RoboCompLaser::TLaserData ldata = laser_proxy->getLaserData();
        //sort laser data from small to large distances using a lambda function.
        std::sort( ldata.begin(), ldata.end(), [](RoboCompLaser::TData a, RoboCompLaser::TData b){ return     a.dist < b.dist; });
        switch (estado) {

            case 0: //Caso 0 -> Inicializar
                initializeMatrix();
                std::cout << "MATRIZ INICIALIZADA!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
                estado=2;
            break;
            case 1: //Caso 1 -> Evitar objetos
                turnMethod(ldata, rot);
                std::cout << "HE GIRADO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
                 estado=3;
            break;
            case 2: //Caso 2 -> Seguir hacia delante
                if(ldata.front().dist < threshold){
                    std::cout << "VOY A GIRAR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
                    estado=1;
                } else {
                    goAheadMethod();
                    std::cout << "ESTOY AVANZANDO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
                }
            break;
            case 3: //Comprobar direccion
                checkDirection();
                estado = 2;
            break;

        }

	//initializeMatrix();



    }
    catch(const Ice::Exception &ex)
    {
        std::cout << ex << std::endl;
    }

}

int SpecificWorker::startup_check()
{
	std::cout << "Startup check" << std::endl;
	QTimer::singleShot(200, qApp, SLOT(quit()));
	return 0;
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

