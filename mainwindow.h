/*
 * mainwindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMap>
#include <QTimer>

#include "commandecamera.h"

namespace Ui {
class MainWindow;
}

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    VlcInstance *myVlcInstance;
    VlcMedia *myVlcMedia;
    VlcMediaPlayer *myVlcMediaPlayer;

    QTimer TimerTestConnexion;

    QMap<QString, CommandeCamera*> ListeCommande;
    T_ParametresCameras ParametresCameras;

    void ChargerConnextion(void);
    void TesterConnexion(void);
    void MAJDonnees(void);
    void AfficherVideo(void);
    void VisualisationMajListePresetsRecuperer(void);
    void VisualisationParametresImageRecuperer(void);
    void VisualisationRetournerMirroirRecuperer(void);
    void VisualisationOSDRecuperer(void);
    void ConfigurationInfosCameraRecuperer(void);
    void ConfigurationIPRecuperer(void);
    void ConfigurationDDMRecuperer(void);

private slots:
    void ConnexionTimeout(void);

    void TraiterReponseTesterConnexion(QString P_Commande, QMap<QString, QString> P_Reponse);

    void TraiterReponseInformationsCameraRecuperer(QString P_Commande, QMap<QString, QString> P_Reponse);

    void TraiterReponseVisualiserMajListePresets(QString P_Commande, QMap<QString, QString> P_Reponse);
    void TraiterReponseVisualisationParametresImageRecuperer(QString P_Commande, QMap<QString, QString> P_Reponse);
    void TraiterReponseVisualisationRetournerMirroirRecuperer(QString P_Commande, QMap<QString, QString> P_Reponse);
    void TraiterReponseVisualisationOSDRecuperer(QString P_Commande, QMap<QString, QString> P_Reponse);

    void TraiterReponseDebug(QString P_Reponse);

    void TraiterReponseConfigurationIPRecuperer(QString P_Commande, QMap<QString, QString> P_Reponse);
    void TraiterReponseConfigurationDDMRecuperer(QString P_Commande, QMap<QString, QString> P_Reponse);

    void AddLog(QString P_Message);
    void MasquerAfficherFenetreLog(bool P_AfficherFenetreLog);

    void VisualisationZoomIn(void);
    void VisualisationZoomOut(void);
    void VisualisationZoomStop(void);
    void VisualisationPTZTopLeft(void);
    void VisualisationPTZTop(void);
    void VisualisationPTZTopRight(void);
    void VisualisationPTZLeft(void);
    void VisualisationPTZRight(void);
    void VisualisationPTZBottomLeft(void);
    void VisualisationPTZBottom(void);
    void VisualisationPTZBottomRight(void);
    void VisualisationAllerAuPreset(void);
    void VisualisationPTZStop(void);
    void VisualisationVideoActive(bool P_VideoActive);
    void VisualisationMirroir(bool P_MirroirActif);
    void VisualisationRetourner(bool P_RetournerActif);
    void VisualisationChangerOSD(void);

    void ParametresImageChangerContraste(int P_Valeur);
    void ParametresImageChangerLuminosite(int P_Valeur);
    void ParametresImageChangerNettete(int P_Valeur);
    void ParametresImageChangerSaturation(int P_Valeur);
    void ParametresImageChangerTeinte(int P_Valeur);

    void ConfigurationNomPeripheriqueDefinir(void);
    void ConfigurationDDMAppliquer(void);
    void ConfigurationDDMProgrammationActiver(void);
    void ConfigurationDDMProgrammationDesactiver(void);

    void ConnexionSauver(void);
    void ConnexionAppliquer(void);

    void DebugEnvoyerCommande(void);

};

#endif // MAINWINDOW_H
