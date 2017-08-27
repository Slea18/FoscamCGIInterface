/*
 * ddmzones.cpp
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

#include "ddmzones.h"
#include "ui_ddmzones.h"

DDMZones::DDMZones(int *P_DDMListeZones, QImage *P_Image, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DDMZones)
{
    ui->setupUi(this);

    for( int i = 0 ; i < 10 ; i++ )
        DDMListeZones[i] = P_DDMListeZones[i];

    WidgetDDMZone = new DDMZonesWidget(P_Image->scaled(800, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation), DDMListeZones, this);

    ui->horizontalLayoutWidget_2->insertWidget(0, WidgetDDMZone);

    connect(ui->pushButtonActiverTout, SIGNAL(clicked(bool)), this, SLOT(ActiverTout()));
    connect(ui->pushButtonDesactiverTout, SIGNAL(clicked(bool)), this, SLOT(DesactiverTout()));
    connect(ui->pushButtonDDMZonesAnnuler, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(ui->pushButtonDDMZonesAppliquer, SIGNAL(clicked(bool)), this, SLOT(Appliquer()));
}

DDMZones::~DDMZones()
{
    delete WidgetDDMZone;
    delete ui;
}

void DDMZones::getZones(int *P_DDMListeZones)
{
    for( int i = 0 ; i < 10 ; i++ )
        P_DDMListeZones[i] = DDMListeZones[i];
}

void DDMZones::Appliquer()
{
    WidgetDDMZone->GetListeZones(DDMListeZones);

    accept();
}

void DDMZones::ActiverTout()
{
    for( int i = 0 ; i < 10 ; i++ )
        DDMListeZones[i] = 1023;

    WidgetDDMZone->MajListeZones(DDMListeZones);
    WidgetDDMZone->update();
}

void DDMZones::DesactiverTout()
{
    for( int i = 0 ; i < 10 ; i++ )
        DDMListeZones[i] = 0;

    WidgetDDMZone->MajListeZones(DDMListeZones);
    WidgetDDMZone->update();
}
