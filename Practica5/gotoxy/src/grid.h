//
// Created by salabeta on 24/11/20.
//

#ifndef GOTOXY_GRID_H
#define GOTOXY_GRID_H

#include <QGraphicsItem>

template<typename HMIN, HMIN hmin, typename WIDTH, WIDTH width, typename TILE, TILE tile>
class Grid
{
    public:
        Grid()
        {
            array.resize((int)(width/tile));
            for (auto &row : array)
                row.resize((int)(width/tile));
            int k=0;
            for (int i = hmin; i < width/2; i += tile, k++)
            {
                int l=0;
                for (int j = hmin; j < width/2; j += tile, l++)
                {
                    array[k][l] = Value{false, nullptr, i, j};
                }
            }
        };

        struct Value
        {
            bool occupied = false;
            QGraphicsItem * paint_cell = nullptr;
            int cx, cy;
            int dist = 0; //dist vecinos
        };

        std::vector<std::vector<Value>> array;

        void create_graphic_items(QGraphicsScene &scene)
        {
            for (auto &row : array)
                for (auto &elem : row)
                {
                	//if(elem.cx == -2500 || elem.cx==2400 || elem.cy==-2500 || elem.cy==2400){
                    		elem.paint_cell = scene.addRect(-tile / 2, -tile / 2, tile, tile, QPen(QColor("DarkGreen")),
                                                    QBrush(QColor("Green")));
                    		elem.paint_cell->setPos(elem.cx, elem.cy);
               	/*} else{
               	elem.paint_cell = scene.addRect(-tile / 2, -tile / 2, tile, tile, QPen(QColor("DarkGreen")),
                                                    QBrush(QColor("Green")));
                    		elem.paint_cell->setPos(elem.cx, elem.cy);
                    		//std::cout << "(" << elem.cx << ", " << elem.cy << ")"<<endl;
               	
               	}*/
                }
        }
    /*
 * Inicializamos el array a false, osea, no ocupades.
 */
    void inicializate()
    {
        for (int i = 0; i < this->tam; ++i) {
            for (int j = 0; j < this->tam; ++j) {
                this->array[i][j] = false;
            }
        }
    }

public:
    /**
     * modificamos en funcion de v la coordenada x,z
     * @param x
     * @param z
     * @param v
     */
    void set_Value(int x, int z, bool v)
    {
       this->array[x][z] = v;
       auto [i, j] = transformar(x,z);
       array[i][j].occupied = v;
       if(v)
            array[i][j].paint_cell->setColor(QColor());

    }
    /**
     * devolvemos el valor de la coordenada x,z
     * @param x
     * @param z
     * @return
     */
    std::vector<std::vector<Value>> get_value(int x, int z)
    {
        auto [i, j] = transformar(x,z);
        return  this->array[x][z];
       // return true;
    }

    std::tuple<int, int> transformar(int i, int j){
        int v1= (size/tile)/5000*i+(size/tile)/2;
        int v2= (size/tile)/5000*j+(size/tile)/2;
        return std::make_tuple(v1,v2);
    }

};


#endif //GOTOXY_GRID_H
