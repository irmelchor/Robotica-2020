//
// Created by salabeta on 24/11/20.
//

#ifndef GOTOXY_GRID_H
#define GOTOXY_GRID_H

#include <QGraphicsItem>

template<typename HMIN, HMIN hmin, typename WIDTH, WIDTH width, typename TILE, TILE tile>
class Grid
{
    //int width, tile;
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
      //      this->width = width; this->tile = tile;
        };

        struct Value
        {
            bool occupied = false;
            QGraphicsRectItem * paint_cell = nullptr;
            int cx, cy;
            int dist = 0; //dist vecinos
        };

        std::vector<std::vector<Value>> array;

        void create_graphic_items(QGraphicsScene &scene)
        {
            for (auto &row : array)
                for (auto &elem : row)
                {
               	    elem.paint_cell = scene.addRect(-tile / 2, -tile / 2, tile, tile, QPen(QColor("DarkGreen")), QBrush(QColor("Green")));
               	    elem.paint_cell->setPos(elem.cx, elem.cy);
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

    void set_ocuppied_w(int x, int z)
    {
        if( auto r = transformar(x,z); r.has_value())
        {
            auto[i, j] = r.value();
            array[i][j].occupied = true;
            array[i][j].paint_cell->setBrush(QColor("Red"));
        }
        else
            qInfo() << "out of bounds";
    }


    void set_free_w(int x, int z)
    {
        auto [i, j] = transformar(x,z);
        array[i][j].occupied = false;
        array[i][j].paint_cell->setBrush(QColor("Green"));
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

    std::optional<std::tuple<int, int>> transformar(int i, int j)
    {
        int v1= i/tile + width/tile/2;
        int v2= j/tile + width/tile/2;
        if(v1<0 or v1>=array.size() or v2<0 or v2>=array.size())
            return {};
        else
            return std::make_tuple(v1,v2);
    }

};


#endif //GOTOXY_GRID_H
