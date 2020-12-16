/*
 *    Copyright (C) 2020 by jvallero & mtorocom
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

/**
	\brief
	@author jvallero & mtorocom
*/



#ifndef SPECIFICWORKER_H
#define SPECIFICWORKER_H

#include <genericworker.h>
#include <innermodel/innermodel.h>
#include <Eigen/Dense>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include "grid.h"

using namespace Eigen;

class SpecificWorker : public GenericWorker{

    template<typename T>
    struct Target {
        T data;
        std::mutex mutex;
        bool activate = false;
        bool empty = true;

        void put(const T &Data) {
            std::lock_guard<std::mutex> guard(mutex);
            data = Data;
            activate = true;
            empty = false;
        }
        std::optional<T> get() {
            std::lock_guard<std::mutex> guard(mutex);
            if (not empty)
            {
                empty = true;
                return data;
            }
            else
                return {};
        }
        void set_task_finished() {
            std::lock_guard<std::mutex> guard(mutex);
            activate = false;
        }
        bool is_active()  {
            std::lock_guard<std::mutex> guard(mutex);
            return activate;
        }
        
    };

public:
    SpecificWorker(TuplePrx tprx, bool startup_check);
    ~SpecificWorker();
    bool setParams(RoboCompCommonBehavior::ParameterList params);
    void RCISMousePicker_setPick(RoboCompRCISMousePicker::Pick myPick);



public slots:

    void compute();
    int startup_check();
    void initialize(int period);
    
    
protected:
       void resizeEvent(QResizeEvent * event)
       {
           graphicsView->fitInView(scene.sceneRect(), Qt::KeepAspectRatio);
       }


private:
    std::shared_ptr<InnerModel> innerModel;
    bool startup_check_flag;

    //Tupla de 3 variables float para las coordenadas x,y,z.
    using Tpose = std::tuple<float, float, float>;

    //variable tipo Target con la Tupla Tpose
    Target<Tpose> target_buffer;
    Tpose target;
    using Tupla = std::tuple<float, float, float, float, float>;
    Eigen::Vector2f transformar_targetRW( RoboCompGenericBase::TBaseState bState);

    //e4
    std::vector<Tupla> calcularPuntos(float vOrigen, float wOrigen);
    std::optional<Tupla> ordenar(std::vector<Tupla> vector, float x, float z);
    std::vector<Tupla> obstaculos(std::vector<Tupla> vector, float aph, const RoboCompLaser::TLaserData &ldata);
    std::tuple<std::vector<Tupla>, Tupla> dynamicWindowApproach(RoboCompGenericBase::TBaseState bState, RoboCompLaser::TLaserData &ldata);

    //grid
    using MyGrid= Grid<int, -2500, int, 5000, int, 100>;
    MyGrid grid;

    //e5
    void compute_navigation_function(MyGrid::Value &target_cell);
    
    //draw
    QGraphicsScene scene;
    QGraphicsView *graphicsView;
    QGraphicsItem *robot_polygon = nullptr;
    QGraphicsItem *laser_polygon = nullptr;
    QGraphicsItem *map_polygon = nullptr;
    const float ROBOT_LENGTH = 400;
    void draw_things(const RoboCompGenericBase::TBaseState &bState, const RoboCompLaser::TLaserData &ldata, const std::tuple<std::vector<Tupla>, Tupla>&puntos);
    std::vector<QGraphicsEllipseItem*> arcs_vector;



    void fill_grid_with_obstacles();
};

#endif
