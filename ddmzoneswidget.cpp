/*
 * ddmzoneswidget.cpp
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

#include "ddmzoneswidget.h"

#include <QPainter>
#include <QDebug>

DDMZonesWidget::DDMZonesWidget(QImage P_Fond, int *P_ListeZones, QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    Fond = P_Fond;
    for( int i = 0 ; i < 10 ; i++ )
        ListeZones[i] = P_ListeZones[i];

}

void DDMZonesWidget::MajListeZones(int *P_ListeZones)
{
    for( int i = 0 ; i < 10 ; i++ )
        ListeZones[i] = P_ListeZones[i];
}

void DDMZonesWidget::GetListeZones(int *P_ListeZones)
{
    for( int i = 0 ; i < 10 ; i++ )
        P_ListeZones[i] = ListeZones[i];
}

void DDMZonesWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.drawImage(0, 0, Fond);

    for( int ligne = 0 ; ligne < 10 ; ligne++ )
    {
        for( int colone = 0 ; colone < 10 ; colone ++)
        {
            if( ListeZones[ligne] & (1<<colone) )
                painter.drawImage(80*colone, 60*ligne, QImage(":/Images/Images/DDM_Actif.png"));
            else
                painter.drawImage(80*colone, 60*ligne, QImage(":/Images/Images/DDM_Inactif.png"));
        }
    }

}

void DDMZonesWidget::mouseReleaseEvent(QMouseEvent *event)
{

    if( event->button() == Qt::LeftButton )
    {
        int Ligne = int (event->pos().y() / 60);
        int Colonne = int (event->pos().x() / 80);

        ListeZones[Ligne] ^= (1<<Colonne);
    }

    update();
}
