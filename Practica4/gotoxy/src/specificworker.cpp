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
const float landa = -0.5 / log(0.1);

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
    //		try
    //		{
    //			RoboCompCommonBehavior::Parameter par = params.at("InnerModelPath");
    //			std::string innermodel_path = par.value;
    //            innerModel = std::make_shared<InnerModel>(innermodel_path);
    //		}
    //		catch(const std::exception &e) { qFatal("Error reading config params"); }

    return true;
}

void SpecificWorker::initialize(int period)
{
    std::cout << "Initialize worker" << std::endl;

    // graphics
    graphicsView = new QGraphicsView(this);
    graphicsView->resize(this->size());
    graphicsView->setScene(&scene);
    graphicsView->setMinimumSize(400, 400);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    struct Dimensions
    {
        int TILE_SIZE = 100;
        float HMIN = -2500, VMIN = -2500, WIDTH = 5000, HEIGHT = 5000;
    };
    Dimensions dim;
    scene.setSceneRect(dim.HMIN, dim.VMIN, dim.WIDTH, dim.HEIGHT);
    graphicsView->scale(1, -1);

    graphicsView->show();

    //robot
    QPolygonF poly2;
    float size = ROBOT_LENGTH / 2.f;
    poly2 << QPoint(-size, -size)
          << QPoint(-size, size)
          << QPoint(-size / 3, size * 1.6)
          << QPoint(size / 3, size * 1.6)
          << QPoint(size, size)
          << QPoint(size, -size);
    QBrush brush;
    brush.setColor(QColor("DarkRed"));
    brush.setStyle(Qt::SolidPattern);
    robot_polygon = (QGraphicsItem *)scene.addPolygon(poly2, QPen(QColor("DarkRed")), brush);
    robot_polygon->setZValue(5);
    RoboCompGenericBase::TBaseState bState;
    try
    {
        differentialrobot_proxy->getBaseState(bState);
        robot_polygon->setRotation(qRadiansToDegrees(-bState.alpha));
        robot_polygon->setPos(bState.x, bState.z);
    }
    catch (const Ice::Exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    // box
    //auto caja = innerModel->getTransform("caja1");
    //if( caja )
    //    scene.addRect(caja->backtX-200, caja->backtZ-200, 400, 400, QPen(QColor("Magenta")), QBrush(QColor("Magenta")));

    graphicsView->fitInView(scene.sceneRect(), Qt::KeepAspectRatio);

    this->Period = 100;
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
    float dt = 1;
    // for (float dt = 0.3; dt < 1.5; dt += 0.1)  // several steps in the future
    // {
    for (float v = 0; v <= 1000; v += 100) // several advance velocities
    {
        for (float w = -M_PI; w <= M_PI; w += 0.1) // several rotacion velocities
        {
            float vNuevo = vOrigen + v;
            float wNuevo = wOrigen + w;
            if (fabs(w) > 0.1)
            {
                // Coordenadas posibles después de moverse imaginariamente
                float r = vNuevo / wNuevo; //radio
                float x = r - r * cos(wNuevo * dt);
                float y = r * sin(wNuevo * dt);
                float alp = wNuevo * dt;
                std::cout << __FUNCTION__ << " " << x << " " << y << " " << r << " " << vNuevo << " " << wNuevo
                          << " " << std::endl;
                vectorT.emplace_back(std::make_tuple(x, y, vNuevo, wNuevo, alp)); //lo añadimos al vector de tuplas
            }
            else // para evitar la división por cero en el cálculo de r
                vectorT.emplace_back(
                    std::make_tuple(0, v * dt, vNuevo, wNuevo, wNuevo * dt)); //lo añadimos al vector de tuplas
        }
    }
    //}
    return vectorT;
}

std::vector<SpecificWorker::tupla> SpecificWorker::ordenar(std::vector<tupla> vector, float x, float z)
{
    std::sort(vector.begin(), vector.end(), [x, z](const auto &a, const auto &b) {
        const auto &[ax, ay, ca, cw, aa] = a;
        const auto &[bx, by, ba, bw, bb] = b;
        return ((ax - x) * (ax - x) + (ay - z) * (ay - z)) < ((bx - x) * (bx - x) + (by - z) * (by - z));
    });

    return vector;
}

//Comprueba los puntos dentro del poligono del laser
/*bool SpecificWorker::checkPointsInsideLaserPolygon(RoboCompLaser::TLaserData ldata){
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

std::vector<SpecificWorker::tupla> SpecificWorker::obstaculos(std::vector<tupla> vector, float aph, const RoboCompLaser::TLaserData &ldata)
{
    QPolygonF polygonF;
    const float semiancho = 350; // el semiancho del robot
    std::vector<tupla> vectorOBs;
    for (auto &l : ldata)
        polygonF << QPointF(l.dist * sin(l.angle), l.dist * cos(l.angle));

    for (auto &[x, y, a, g, ang] : vector)
    {
        // GENERAR UN CUADRADO CON EL CENTRO EN X, Y Y ORIENTACION ANG.
        QPolygonF polyRobot;

        polyRobot << QPointF(x - semiancho, y + semiancho)
                  << QPointF(x + semiancho, y + semiancho)
                  << QPointF(x + semiancho, y - semiancho)
                  << QPointF(x - semiancho, y - semiancho);

        polyRobot = QTransform().rotate(aph).map(polyRobot);
        //   qDebug () <<  "Despues de girar " <<  polyRobot;

        bool cuatroEsquinas = true;
        for (auto &p : polyRobot)
        {
            // si NO contiene alguna de las 4 esquinas, no tenemos en cuenta esa x, y .
            //En el momento en el que un punto no coincide, pasamos a otro.
            if (!polygonF.containsPoint(p, Qt::OddEvenFill))
            {
                cuatroEsquinas = false;
                break;
            }
        }
        // SI contiene las 4 esquinas , metemos el valor.
        if (cuatroEsquinas)
        {
            // qDebug() << "CuatroEsquinas es true";
            //    std::cout << FUNCTION << " " << x << " " << y << " " << a << " " << g << " " << ang << std::endl;
            vectorOBs.emplace_back(std::make_tuple(x, y, a, g, ang));
        }
    }
    return vectorOBs;
}


void SpecificWorker::draw_things(const RoboCompGenericBase::TBaseState &bState, const RoboCompLaser::TLaserData &ldata,
                                 const std::vector<tupla> &puntos, const tupla &front)
{
    //draw robot
    //innerModel->updateTransformValues("base", bState.x, 0, bState.z, 0, bState.alpha, 0);
    robot_polygon->setRotation(qRadiansToDegrees(-bState.alpha));
    robot_polygon->setPos(bState.x, bState.z);
    graphicsView->resize(this->size());

    //draw laser
    if (laser_polygon != nullptr)
        scene.removeItem(laser_polygon);
    QPolygonF poly;
    for (auto &l : ldata)
        poly << robot_polygon->mapToScene(QPointF(l.dist * sin(l.angle), l.dist * cos(l.angle)));
    QColor color("LightGreen");
    color.setAlpha(40);
    laser_polygon = scene.addPolygon(poly, QPen(color), QBrush(color));
    laser_polygon->setZValue(13);

    // draw future. Draw and arch going out from the robot
    // remove existing arcs
    for (auto arc : arcs_vector)
        scene.removeItem(arc);
    arcs_vector.clear();
    QColor col("Red");
    for (auto &[x, y, vx, wx, a] : puntos)
    {
        QPointF centro = robot_polygon->mapToScene(x, y);
        arcs_vector.push_back(scene.addEllipse(centro.x(), centro.y(), 20, 20, QPen(col), QBrush(col)));
    }

    QPointF center = robot_polygon->mapToScene(std::get<0>(front), std::get<1>(front));
    arcs_vector.push_back(scene.addEllipse(center.x(), center.y(), 40, 40, QPen(Qt::black), QBrush(Qt::black)));
}

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
        auto beta = atan2(tr.x(), tr.y());     //Angulo
        std::cout << tw << " " << rw << " " << rot << " " << beta << std::endl;
        auto dist = tr.norm(); //Distancia a recorrer
                               //float rot_speed = beta;
                               //auto adv_speed = (1000 * reduce_speed_if_turning(rot_speed, 0.1, 0.5) * reduce_speed_if_close_to_target(dist));
                               //differentialrobot_proxy->setSpeedBase(adv_speed, beta);

        if (dist < 50)
        {
            differentialrobot_proxy->setSpeedBase(0, 0);
            target.active = false;
            return;
        }
        else
        {
            float vOrigen = bState.advVz;
            float wOrigen = bState.rotV;

            std::vector<tupla> vPuntos = calcularPuntos(vOrigen, wOrigen);
            std::vector<tupla> vSinObstaculos = obstaculos(vPuntos, bState.alpha, ldata);
            std::vector<tupla> vOrdenado = ordenar(vSinObstaculos, tr.x(), tr.y());
            std::cout << vOrdenado.size() << std::endl;
            if (vOrdenado.size() > 0)
            {

                auto [x, y, v, w, alpha] = vOrdenado.front();
                std::cout << __FUNCTION__ << " " << x << " " << y << " " << v << " " << w << " " << alpha
                          << std::endl;

                if (w > M_PI)w = M_PI;
                if (w < -M_PI)w = -M_PI;
                if (v < 0) v = 0;
                // Aquí debemos hacer algo para lo de negativo

                try
                {
                    differentialrobot_proxy->setSpeedBase(std::min(v/5, 1000.f), w);
                    draw_things(bState, ldata, vOrdenado, vOrdenado.front());
                }
                catch (const Ice::Exception &e)
                {
                    std::cout << e.what() << std::endl;
                     
                    
                }
            }
            else
            {
                std::cout << "Vector vacio" << std::endl;
            }

            
        }

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
