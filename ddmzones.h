/*
 * ddmzones.h
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

#ifndef DDMZONES_H
#define DDMZONES_H

#include "ddmzoneswidget.h"

#include <QDialog>
#include <QImage>

namespace Ui {
class DDMZones;
}

class DDMZones : public QDialog
{
    Q_OBJECT
public:
    explicit DDMZones(int* P_DDMListeZones, QImage* P_Image, QWidget *parent = nullptr);
    ~DDMZones();

    void getZones(int* P_DDMListeZones);

private:
    Ui::DDMZones *ui;

    DDMZonesWidget* WidgetDDMZone;

    int DDMListeZones[10];

private slots:
    void Appliquer(void);
    void ActiverTout(void);
    void DesactiverTout(void);

signals:

public slots:
};

#endif // DDMZONES_H
