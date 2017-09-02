/*
 * dialogmaskarea.h
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

#ifndef DIALOGMASKAREA_H
#define DIALOGMASKAREA_H

#include <QDialog>
#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include "graphicsviewzonesmasques.h"
#include "commandecamera.h"

namespace Ui {
class DialogMaskArea;
}

class DialogMaskArea : public QDialog
{
    Q_OBJECT

public:

    typedef struct T_Masque {
        int x1;
        int y1;
        int x2;
        int y2;
    }T_Masque;

    explicit DialogMaskArea(T_ParametresCameras P_ParametresCameras, QWidget *parent = 0);
    ~DialogMaskArea();

private:
    Ui::DialogMaskArea *ui;

    int NbMasques;
    T_Masque Masques[C_NB_MASQUES];

    QGraphicsScene* scene;
    GraphicsViewZonesMasques* view;
    QGraphicsPixmapItem* FondItem;

    T_ParametresCameras ParametresCameras;
    CommandeCamera *CommandeImage;
    CommandeCamera *CommandeAppliquer;
    CommandeCamera *CommandeSetActif;
    CommandeCamera *CommandeGetActif;

    void MajImage(void);
    void GetMasqueActif(void);

private slots:
    void ChangeNbMasquesRestant(int P_NbMasquesRestant);
    void RetirerMasques(void);
    void Appliquer(void);
    void AppliquerEtQuitter(void);
    void TraiterReponseDemandeImage(QString, QImage*P_Image);
    void TraiterReponseAppliquer(QString,QMap<QString,QString>);
    void TraiterReponseGetActif(QString, QMap<QString,QString> P_Reponse);
    void SendLog(QString P_Message);

signals:
    void sigLog(QString P_Message);

};

#endif // DIALOGMASKAREA_H
