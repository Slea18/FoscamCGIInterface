/*
 * graphicsviewzonesmasques.cpp
 * This file is part of FoscamCGIInterface
 *
 * Copyright (C) 2017-2017 Ludovic Girardet
 *
 * FoscamCGIInterface is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FoscamCGIInterface is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>
 *
 */

#include "graphicsviewzonesmasques.h"

GraphicsViewZonesMasques::GraphicsViewZonesMasques(QGraphicsScene *P_scene, QWidget *parent) :
    QGraphicsView(P_scene, parent)
{
    scene = P_scene;

    NbMasquesUtilises = 0;
    Zones[0] = NULL;
    Zones[1] = NULL;
    Zones[2] = NULL;
    Zones[3] = NULL;
}

void GraphicsViewZonesMasques::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton && NbMasquesUtilises < C_NB_MASQUES )
    {
        posDepart[NbMasquesUtilises] = event->pos();
    }

    QGraphicsView::mousePressEvent(event);
}

void GraphicsViewZonesMasques::mouseReleaseEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton && NbMasquesUtilises < C_NB_MASQUES )
    {
        NbMasquesUtilises++;
        emit sigNbMasquesRestant(C_NB_MASQUES-NbMasquesUtilises);
    }
}

void GraphicsViewZonesMasques::mouseMoveEvent(QMouseEvent *event)
{
    if( event->buttons() & Qt::LeftButton && NbMasquesUtilises < C_NB_MASQUES )
    {
        if( Zones[NbMasquesUtilises] != NULL )
            scene->removeItem(Zones[NbMasquesUtilises]);

        QBrush Brush(Qt::blue);
        QPen outlinePen(Qt::blue);
        outlinePen.setWidth(1);

        qreal x = posDepart[NbMasquesUtilises].x();
        qreal y = posDepart[NbMasquesUtilises].y();
        qreal w = event->x() - posDepart[NbMasquesUtilises].x();
        qreal h = event->y() - posDepart[NbMasquesUtilises].y();

        if( w < 0 )
        {
            x += w;
            w = -w;
        }
        if( h < 0 )
        {
            y += h;
            h = -h;
        }
        Zones[NbMasquesUtilises] = scene->addRect(x, y, w, h, outlinePen, Brush);
    }

    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsViewZonesMasques::RetirerMasques()
{
    for( int i = 0 ; i < NbMasquesUtilises ; i++ )
    {
        scene->removeItem(Zones[i]);
        Zones[i] = NULL;
    }
    NbMasquesUtilises = 0;
    emit sigNbMasquesRestant(C_NB_MASQUES);
}

int GraphicsViewZonesMasques::getNbMasques()
{
    return NbMasquesUtilises;
}

void GraphicsViewZonesMasques::getMasque(int i, int *x1, int *y1, int *x2, int *y2)
{
    *x1 = Zones[i]->rect().x();
    *y1 = Zones[i]->rect().y();
    *x2 = Zones[i]->rect().x() + Zones[i]->rect().width();
    *y2 = Zones[i]->rect().y() + Zones[i]->rect().height();
}


