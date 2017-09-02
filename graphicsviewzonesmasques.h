/*
 * graphicsviewzonesmasques.h
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

#ifndef GRAPHICSVIEWZONESMASQUES_H
#define GRAPHICSVIEWZONESMASQUES_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QPoint>
#include <QGraphicsRectItem>

#define C_NB_MASQUES 4

class GraphicsViewZonesMasques : public QGraphicsView
{
    Q_OBJECT

public:
    explicit GraphicsViewZonesMasques(QGraphicsScene *P_scene, QWidget *parent = Q_NULLPTR);
    void RetirerMasques(void);
    int getNbMasques(void);
    void getMasque(int i, int* x1, int* y1, int* x2, int* y2);

private:
    QGraphicsScene *scene;
    int NbMasquesUtilises;
    QPointF posDepart[C_NB_MASQUES];
    QGraphicsRectItem* Zones[C_NB_MASQUES];

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void sigNbMasquesRestant(int p);
};

#endif // GRAPHICSVIEWZONESMASQUES_H
