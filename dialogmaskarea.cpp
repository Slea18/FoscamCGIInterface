/*
 * dialogmaskarea.cpp
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

#include "dialogmaskarea.h"
#include "ui_dialogmaskarea.h"

#include <QBrush>

DialogMaskArea::DialogMaskArea(T_ParametresCameras P_ParametresCameras, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMaskArea)
{
    ui->setupUi(this);

    ParametresCameras = P_ParametresCameras;

    scene = new QGraphicsScene;
    view  = new GraphicsViewZonesMasques(scene);
    view->setFixedSize(805,605);
    ui->verticalLayout->insertWidget(1, view);
    view->show();

    FondItem = new QGraphicsPixmapItem();
    scene->addItem(FondItem);
    scene->setSceneRect(0, 0, 800, 600);

    connect(view, SIGNAL(sigNbMasquesRestant(int)), this, SLOT(ChangeNbMasquesRestant(int)));
    connect(ui->pushButtonMasquesAnnuler, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(ui->pushButtonMasquesRetirerMasques, SIGNAL(clicked(bool)), this, SLOT(RetirerMasques()));
    connect(ui->pushButtonMasquesAppliquer, SIGNAL(clicked(bool)), this, SLOT(Appliquer()));
    connect(ui->pushButtonMasquesAppliquerQuitter, SIGNAL(clicked(bool)), this, SLOT(AppliquerEtQuitter()));

    GetMasqueActif();
    MajImage();
}

DialogMaskArea::~DialogMaskArea()
{
    delete view;
    delete scene;
    delete ui;
}

void DialogMaskArea::MajImage()
{
    CommandeImage = new CommandeCamera(ParametresCameras, this);
    connect(CommandeImage, SIGNAL(sigLog(QString)), this, SLOT(SendLog(QString)));
    connect(CommandeImage, SIGNAL(sigReponseSnapPicture(QString,QImage*)), this, SLOT(TraiterReponseDemandeImage(QString,QImage*)));
    CommandeImage->EnvoyerCommande("snapPicture");
}

void DialogMaskArea::GetMasqueActif()
{
    CommandeGetActif = new CommandeCamera(ParametresCameras, this);
    connect(CommandeGetActif, SIGNAL(sigLog(QString)), this, SLOT(SendLog(QString)));
    connect(CommandeGetActif, SIGNAL(sigReponse(QString,QMap<QString,QString>)), this, SLOT(TraiterReponseGetActif(QString,QMap<QString,QString>)));
    CommandeGetActif->EnvoyerCommande("getOSDMask");
}

void DialogMaskArea::ChangeNbMasquesRestant(int P_NbMasquesRestant)
{
    ui->lineEditNbMasquesRestant->setText(QString::number(P_NbMasquesRestant));
}

void DialogMaskArea::RetirerMasques()
{
    view->RetirerMasques();
}

void DialogMaskArea::Appliquer()
{
    memset(&Masques[0], 0, sizeof(T_Masque));
    memset(&Masques[1], 0, sizeof(T_Masque));
    memset(&Masques[2], 0, sizeof(T_Masque));
    memset(&Masques[3], 0, sizeof(T_Masque));

    NbMasques = view->getNbMasques();
    for( int i = 0 ; i < NbMasques ; i++ )
    {
        view->getMasque(i, &Masques[i].x1, &Masques[i].y1, &Masques[i].x2, &Masques[i].y2);
    }

    CommandeAppliquer = new CommandeCamera(ParametresCameras, this);
    connect(CommandeAppliquer, SIGNAL(sigLog(QString)), this, SLOT(SendLog(QString)));
    connect(CommandeAppliquer, SIGNAL(sigReponse(QString,QMap<QString,QString>)), this, SLOT(TraiterReponseAppliquer(QString,QMap<QString,QString>)));

    QString Commande;
    Commande = "setOsdMaskArea";
    for( int i = 0 ; i < C_NB_MASQUES ; i++ )
    {
        Commande += "&x1_" + QString::number(i) + "=" + QString::number((int) (10000.0 * Masques[i].x1 / 800));
        Commande += "&y1_" + QString::number(i) + "=" + QString::number((int) (10000.0 * Masques[i].y1 / 600));
        Commande += "&x2_" + QString::number(i) + "=" + QString::number((int) (10000.0 * Masques[i].x2 / 800));
        Commande += "&y2_" + QString::number(i) + "=" + QString::number((int) (10000.0 * Masques[i].y2 / 600));
    }

    CommandeAppliquer->EnvoyerCommande(Commande);

    CommandeSetActif = new CommandeCamera(ParametresCameras, this);
    connect(CommandeSetActif, SIGNAL(sigLog(QString)), this, SLOT(SendLog(QString)));
    Commande = "setOSDMask&isEnableOSDMask=" + ui->checkBoxMasqueActif->isChecked() ? "1" : "0";
    CommandeSetActif->EnvoyerCommande(Commande);
    CommandeSetActif->deleteLater();
}

void DialogMaskArea::AppliquerEtQuitter()
{
    Appliquer();
    accept();
}

void DialogMaskArea::TraiterReponseDemandeImage(QString, QImage * P_Image)
{
    FondItem->setPixmap(QPixmap::fromImage(P_Image->scaled(800, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    CommandeImage->deleteLater();
}

void DialogMaskArea::TraiterReponseAppliquer(QString, QMap<QString, QString>)
{
    RetirerMasques();
    MajImage();

    CommandeAppliquer->deleteLater();
}

void DialogMaskArea::TraiterReponseGetActif(QString, QMap<QString, QString> P_Reponse)
{
    ui->checkBoxMasqueActif->setChecked( P_Reponse["isEnableOSDMask"] == "1" );
    CommandeGetActif->deleteLater();
}

void DialogMaskArea::SendLog(QString P_Message)
{
    emit sigLog(P_Message);
}
