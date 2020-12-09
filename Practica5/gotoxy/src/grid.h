//
// Created by salabeta on 24/11/20.
//

#ifndef GOTOXY_GRID_H
#define GOTOXY_GRID_H

#include <QGraphicsItem>

template <typename HMIN, HMIN hmin, typename WIDTH, WIDTH width, typename TILE, TILE tile>
class Grid
{
    //int width, tile;
public:
    Grid()
    {
        array.resize((int)(width / tile));
        for (auto &row : array)
            row.resize((int)(width / tile));
        int k = 0;
        for (int i = hmin; i < width / 2; i += tile, k++)
        {
            int l = 0;
            for (int j = hmin; j < width / 2; j += tile, l++)
            {
                array[k][l] = Value{false, nullptr, nullptr, i, j};
            }
        }
        //      this->width = width; this->tile = tile;
    };

    struct Value
    {
        bool occupied = false;
        QGraphicsRectItem *paint_cell = nullptr;
        QGraphicsTextItem *text_cell = nullptr;
        int cx, cy;
        int k, l;
        int dist = 0; //dist vecinos
    };

    std::vector<std::vector<Value>> array;

    /* void create_graphic_items(QGraphicsScene &scene)
    {
        for (auto &row : array)
            for (auto &elem : row)
            {
                elem.paint_cell = scene.addRect(-tile / 2, -tile / 2, tile, tile, QPen(QColor("DarkGreen")), QBrush(QColor("Green")));
                elem.paint_cell->setPos(elem.cx, elem.cy);
            }
    }*/

    void create_graphic_items(QGraphicsScene &scene)
    {
        auto fondo = QColor("LightGreen");
        fondo.setAlpha(40);
        QFont font("Bavaria");
        font.setPointSize(40);
        font.setWeight(QFont::TypeWriter);
        for (auto &row : array)
            for (auto &elem : row)
            {
                elem.paint_cell = scene.addRect(-tile / 2, -tile / 2, tile, tile, QPen(QColor("DarkGreen")), QBrush(fondo));
                elem.paint_cell->setPos(elem.cx, elem.cy);
                elem.text_cell = scene.addText("-1", font);
                elem.text_cell->setPos(elem.cx - tile / 2, elem.cy - tile / 2);
                // Get the current transform
                QTransform transform(elem.text_cell->transform());
                qreal m11 = transform.m11(); // Horizontal scaling
                qreal m12 = transform.m12(); // Vertical shearing
                qreal m13 = transform.m13(); // Horizontal Projection
                qreal m21 = transform.m21(); // Horizontal shearing
                qreal m22 = transform.m22(); // vertical scaling
                qreal m23 = transform.m23(); // Vertical Projection
                qreal m31 = transform.m31(); // Horizontal Position (DX)
                qreal m32 = transform.m32(); // Vertical Position (DY)
                qreal m33 = transform.m33(); // Addtional Projection Factor
                // Vertical flip
                m22 = -m22;
                // Write back to the matrix
                transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
                // Set the items transformation
                elem.text_cell->setTransform(transform);
            }
    }

/*
 * Inicializamos el array a false, osea, no ocupades.
 */
    void inicializate()
    {
        for (int i = 0; i < this->tam; ++i)
        {
            for (int j = 0; j < this->tam; ++j)
            {
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
        auto [i, j] = transformar(x, z);
        array[i][j].occupied = v;
        if (v)
            array[i][j].paint_cell->setColor(QColor());
    }

    void set_ocuppied_w(int x, int z)
    {
        if (auto r = transformar(x, z); r.has_value())
        {
            auto [i, j] = r.value();
            array[i][j].occupied = true;
            array[i][j].paint_cell->setBrush(QColor("Red"));
        }
        else
            qInfo() << "out of bounds";
    }

    void set_free_w(int x, int z)
    {
        auto [i, j] = transformar(x, z);
        array[i][j].occupied = false;
        array[i][j].paint_cell->setBrush(QColor("Green"));
    }
    /**
     * devolvemos el valor de la coordenada x,z
     * @param x
     * @param z
     * @return
     */
    std::optional<Value> get_value(int x, int z)
    {
        if(auto r  = transformar(x, z); r.has_value())
        {
            auto [x,y] = r.value();
            return this->array[x][y];
        }
        else
            return {};
    }

    std::optional<std::tuple<int, int>> transformar(int i, int j)
    {
        int v1 = i / tile + width / tile / 2;
        int v2 = j / tile + width / tile / 2;
        if (v1 < 0 or v1 >= array.size() or v2 < 0 or v2 >= array.size())
            return {};
        else
            return std::make_tuple(v1, v2);
    }

    std::vector<Grid::Value> lista_neighboors(Value v, int dist)
    {
//        std::vector<std::tuple<int, int>> listaCoorVec{{ -1, -1 }, { 0, -1 }, { 1, -1 }, { -1, 0 }, { 1, 0 }, { -1, 1 }, { 0, 1 }, { -1, 1 }};
//
//        std::vector<Value> lista;
//        for (auto[dk, dl] : listaCoorVec) {
//            int k = v.k + dk; // OJO hay que añadir al struct Value las coordenadas de array
//            int l = v.l + dl;
//            if (k, l is_in_limits and and grid[k][l].free and grid[k][l].dist != -1)
//            {
//                set_dist(k.l, dist);
//                lista.append(array[k][l]);
//            }
//        }
        //return lista;
    }

    void reset_cell_distances()
    {
        for (auto &row : array)
            for (auto &elem : row)
                elem.dist = -1;
    }

};

#endif //GOTOXY_GRID_H
