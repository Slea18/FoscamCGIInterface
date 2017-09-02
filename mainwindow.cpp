/*
 * mainwindow.cpp
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ddmzones.h"
#include "dialogmaskarea.h"

#include <VLCQtCore/Common.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>

#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStringList EnteteHeure;
    for( int i = 0 ; i < 48 ; i++ )
    {
        int Hours = i/2;
        int Minutes = 30*(i - 2*Hours);

        QTime Time(Hours, Minutes);
        EnteteHeure.append(Time.toString("hh:mm"));
    }
    ui->tableWidgetConfigurationDDMProgrammation->setHorizontalHeaderLabels(EnteteHeure);

    for( int Jour = 0 ; Jour < 7 ; Jour++ )
    {
        for( int Creneau = 0 ; Creneau < 48 ; Creneau++ )
        {
            QTableWidgetItem* item = new QTableWidgetItem();
            ui->tableWidgetConfigurationDDMProgrammation->setItem(Jour, Creneau, item);
        }
    }
    myVlcMedia = NULL;

    myVlcInstance = new VlcInstance(VlcCommon::args(), this);
    myVlcMediaPlayer = new VlcMediaPlayer(myVlcInstance);
    myVlcMediaPlayer->setVideoWidget(ui->widgetVideo);
    ui->widgetVideo->setMediaPlayer(myVlcMediaPlayer);

    ui->groupBoxLog->setVisible(false);

    ui->tabWidget->setCurrentIndex(0);
    ui->toolBoxConfiguration->setCurrentIndex(0);

    showMaximized();

    ChargerConnextion();
    TesterConnexion();

    connect(ui->pushButtonVisualisationBottom, SIGNAL(clicked()), this, SLOT(VisualisationPTZBottom()));
    connect(ui->pushButtonVisualisationBottomRight, SIGNAL(clicked()), this, SLOT(VisualisationPTZBottomRight()));
    connect(ui->pushButtonVisualisationBottomLeft, SIGNAL(clicked()), this, SLOT(VisualisationPTZBottomLeft()));
    connect(ui->pushButtonVisualisationLeft, SIGNAL(clicked()), this, SLOT(VisualisationPTZLeft()));
    connect(ui->pushButtonVisualisationRight, SIGNAL(clicked()), this, SLOT(VisualisationPTZRight()));
    connect(ui->pushButtonVisualisationTopLeft, SIGNAL(clicked()), this, SLOT(VisualisationPTZTopLeft()));
    connect(ui->pushButtonVisualisationTop, SIGNAL(clicked()), this, SLOT(VisualisationPTZTop()));
    connect(ui->pushButtonVisualisationTopRight, SIGNAL(clicked()), this, SLOT(VisualisationPTZTopRight()));
    connect(ui->pushButtonVisualisationAllerAuPreset, SIGNAL(clicked()), this, SLOT(VisualisationAllerAuPreset()));
    connect(ui->pushButtonVisualisationZoomIn, SIGNAL(clicked()), this, SLOT(VisualisationZoomIn()));
    connect(ui->pushButtonVisualisationZoomOut, SIGNAL(clicked()), this, SLOT(VisualisationZoomOut()));
    connect(ui->pushButtonVisualisationCaptureEcran, SIGNAL(clicked(bool)), this, SLOT(VisualisationCapture()));
    connect(ui->pushButtonVisualisationDefinirMasques, SIGNAL(clicked(bool)), this, SLOT(VisualisationDefinirMasques()));

    connect(ui->checkBoxVisualisationVideoActive, SIGNAL(toggled(bool)), this, SLOT(VisualisationVideoActive(bool)));
    connect(ui->checkBoxVisualisationOSDTemps, SIGNAL(toggled(bool)), this, SLOT(VisualisationChangerOSD()));
    connect(ui->checkBoxVisualisationOSDNomCamera, SIGNAL(toggled(bool)), this, SLOT(VisualisationChangerOSD()));

    connect(ui->horizontalSliderVisualisationParametresImageConstraste, SIGNAL(valueChanged(int)), this, SLOT(ParametresImageChangerContraste(int)));
    connect(ui->horizontalSliderVisualisationParametresImageLuminosite, SIGNAL(valueChanged(int)), this, SLOT(ParametresImageChangerLuminosite(int)));
    connect(ui->horizontalSliderVisualisationParametresImageNettete, SIGNAL(valueChanged(int)), this, SLOT(ParametresImageChangerNettete(int)));
    connect(ui->horizontalSliderVisualisationParametresImageSaturation, SIGNAL(valueChanged(int)), this, SLOT(ParametresImageChangerSaturation(int)));
    connect(ui->horizontalSliderVisualisationParametresImageTeinte, SIGNAL(valueChanged(int)), this, SLOT(ParametresImageChangerTeinte(int)));

    connect(ui->radioButtonVisualiserMirroirOui, SIGNAL(toggled(bool)), this, SLOT(VisualisationMirroir(bool)));
    connect(ui->radioButtonVisualiserRetournerOui, SIGNAL(toggled(bool)), this, SLOT(VisualisationRetourner(bool)));

    connect(ui->pushButtonConfigurationNomPeripheriqueDefinir, SIGNAL(clicked(bool)), this, SLOT(ConfigurationNomPeripheriqueDefinir()));
    connect(ui->pushButtonConfigurationDDMAppliquer, SIGNAL(clicked(bool)), this, SLOT(ConfigurationDDMAppliquer()));
    connect(ui->pushButtonConfigurationDDMProgrammationActiver, SIGNAL(clicked(bool)), this, SLOT(ConfigurationDDMProgrammationActiver()));
    connect(ui->pushButtonConfigurationDDMProgrammationDesctiver, SIGNAL(clicked(bool)), this, SLOT(ConfigurationDDMProgrammationDesactiver()));
    connect(ui->pushButtonConfigurationDDMDefinirZones, SIGNAL(clicked(bool)), this, SLOT(ConfigurationDDMDefinirZones()));

    connect(ui->pushButtonConnexionAppliquer, SIGNAL(clicked(bool)), this, SLOT(ConnexionAppliquer()));
    connect(ui->pushButtonConnexionSauver, SIGNAL(clicked(bool)), this, SLOT(ConnexionSauver()));

    connect(ui->pushButtonDebugEnvoyer, SIGNAL(clicked()), this, SLOT(DebugEnvoyerCommande()));

    connect(ui->pushButtonLogClear, SIGNAL(clicked(bool)), this, SLOT(LogVider()));

    connect(ui->actionAfficher_la_fen_tre_de_log, SIGNAL(toggled(bool)), this, SLOT(MasquerAfficherFenetreLog(bool)));
    connect(ui->actionQuitter, SIGNAL(triggered(bool)), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete myVlcMediaPlayer;
    delete myVlcMedia;
    delete myVlcInstance;
    delete ui;
}

void MainWindow::ChargerConnextion()
{
    QSettings settings(QDir().homePath() + "/.foscamcgiinterface/settings", QSettings::IniFormat);

    ui->lineEditConnexionAdresseIPCamera->setText(settings.value("connexion/ip").toString());
    ui->lineEditConnexionPortCamera->setText(settings.value("connexion/port").toString());
    ui->lineEditConnexionUser->setText(settings.value("connexion/user").toString());
    ui->lineEditConnexionPassword->setText(settings.value("connexion/pwd").toString());
}

void MainWindow::TesterConnexion()
{
    T_ParametresCameras Parametres;

    Parametres.AdresseIP = ui->lineEditConnexionAdresseIPCamera->text();
    Parametres.Port      = ui->lineEditConnexionPortCamera->text();
    Parametres.User      = ui->lineEditConnexionUser->text();
    Parametres.Password  = ui->lineEditConnexionPassword->text();

    ListeCommande["usrBeatHeart"] = new CommandeCamera(Parametres, this);
    connect(ListeCommande["usrBeatHeart"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    connect(ListeCommande["usrBeatHeart"], SIGNAL(sigReponse(QString, QMap<QString,QString>)), this, SLOT(TraiterReponseTesterConnexion(QString,QMap<QString,QString>)));

    QString Requete;
    Requete  = "usrBeatHeart&usrName=" + Parametres.User;
    Requete += "&remoteIp=" + Parametres.AdresseIP;
    Requete += "&groupId=" + QString::number(qrand());

    ListeCommande["usrBeatHeart"]->EnvoyerCommande(Requete);

    TimerTestConnexion.setInterval(3000);
    TimerTestConnexion.setSingleShot(true);
    connect(&TimerTestConnexion, SIGNAL(timeout()), this, SLOT(ConnexionTimeout()));
    TimerTestConnexion.start();
}

void MainWindow::MAJDonnees()
{
    ParametresCameras.AdresseIP = ui->lineEditConnexionAdresseIPCamera->text();
    ParametresCameras.Port      = ui->lineEditConnexionPortCamera->text();
    ParametresCameras.User      = ui->lineEditConnexionUser->text();
    ParametresCameras.Password  = ui->lineEditConnexionPassword->text();

    AfficherVideo();
    VisualisationMajListePresetsRecuperer();
    VisualisationParametresImageRecuperer();
    VisualisationRetournerMirroirRecuperer();
    VisualisationOSDRecuperer();

    ConfigurationInfosCameraRecuperer();
    ConfigurationDDMRecuperer();
    ConfigurationIPRecuperer();
}

void MainWindow::ConnexionTimeout()
{
    QMessageBox::critical(this, "Erreur de connexion", "La connexion à la caméra a échouée.\nVeuiller vérifier la configuration");
    ui->tabWidget->setCurrentIndex(2);
}

void MainWindow::TraiterReponseTesterConnexion(QString P_Commande, QMap<QString, QString> P_Reponse)
{
    if( P_Reponse["result"] == "0" )
    {
        TimerTestConnexion.stop();
        MAJDonnees();
    }

    ListeCommande[P_Commande]->deleteLater();
}

void MainWindow::AfficherVideo()
{
    QString url = "rtsp://";
    url += ParametresCameras.User;
    url += ":";
    url += ParametresCameras.Password;
    url += "@";
    url += ParametresCameras.AdresseIP;
    url += ":";
    url += ParametresCameras.Port;
    url += "/videoMain";

    if( myVlcMedia != NULL )
        delete myVlcMedia;

    myVlcMedia = new VlcMedia(url, myVlcInstance);
    myVlcMediaPlayer->open(myVlcMedia);
}

void MainWindow::TraiterReponseInformationsCameraRecuperer(QString P_Commande, QMap<QString, QString> P_Reponse)
{
    ui->lineEditConfigurationInfosCameraNomPeripherique->setText(P_Reponse["devName"]);
    ui->lineEditConfigurationInfosCameraNomProduit->setText(P_Reponse["productName"]);
    ui->lineEditConfigurationInfosCameraVersionFirmware->setText(P_Reponse["firmwareVer"]);

    ListeCommande[P_Commande]->deleteLater();
}

void MainWindow::TraiterReponseVisualiserMajListePresets(QString P_Commande, QMap<QString, QString> P_Reponse)
{
    ui->comboBoxVisualisationListePresets->clear();

    for( int i = 0 ; i < 16 ; i++ )
    {
        QString value = P_Reponse["point" + QString::number(i)];

        if( value != "" )
            ui->comboBoxVisualisationListePresets->addItem(value);
    }

    ListeCommande[P_Commande]->deleteLater();
}

void MainWindow::TraiterReponseVisualisationParametresImageRecuperer(QString P_Commande, QMap<QString, QString> P_Reponse)
{
    ui->labelVisualisationParametresImageContrastePourcentage->setText(P_Reponse["contrast"] + "%");
    ui->labelVisualisationParametresImageLuminositePourcentage->setText(P_Reponse["brightness"] + "%");
    ui->labelVisualisationParametresImageNettetePourcentage->setText(P_Reponse["sharpness"] + "%");
    ui->labelVisualisationParametresImageSaturationPourcentage->setText(P_Reponse["saturation"] + "%");
    ui->labelVisualisationParametresImageTeintePourcentage->setText(P_Reponse["hue"] + "%");

    ui->horizontalSliderVisualisationParametresImageConstraste->setValue(P_Reponse["contrast"].toInt());
    ui->horizontalSliderVisualisationParametresImageLuminosite->setValue(P_Reponse["brightness"].toInt());
    ui->horizontalSliderVisualisationParametresImageNettete->setValue(P_Reponse["sharpness"].toInt());
    ui->horizontalSliderVisualisationParametresImageSaturation->setValue(P_Reponse["saturation"].toInt());
    ui->horizontalSliderVisualisationParametresImageTeinte->setValue(P_Reponse["hue"].toInt());

    ListeCommande[P_Commande]->deleteLater();
}

void MainWindow::TraiterReponseVisualisationRetournerMirroirRecuperer(QString P_Commande, QMap<QString, QString> P_Reponse)
{
    if( P_Reponse["isMirror"] == "1" )
        ui->radioButtonVisualiserMirroirOui->setChecked(true);
    else
        ui->radioButtonVisualiserMirroirNon->setChecked(true);

    if( P_Reponse["isFlip"] == "1" )
        ui->radioButtonVisualiserRetournerOui->setChecked(true);
    else
        ui->radioButtonVisualiserRetournerNon->setChecked(true);

    ListeCommande[P_Commande]->deleteLater();
}

void MainWindow::TraiterReponseVisualisationOSDRecuperer(QString P_Commande, QMap<QString, QString> P_Reponse)
{
    ui->checkBoxVisualisationOSDTemps->setChecked( P_Reponse["isEnableTimeStamp"] == "1" );
    ui->checkBoxVisualisationOSDNomCamera->setChecked( P_Reponse["isEnableDevName"] == "1" );

    ListeCommande[P_Commande]->deleteLater();
}

void MainWindow::TraiterReponseVisualisationCapture(QString P_Commande, QImage* P_Image)
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", "./capture.jpg", tr("Jpg Files (*.jpg)"));

    if( fileName != "" )
    {
        P_Image->save(fileName);
        QMessageBox::information(this, "Capture d'écran", "Image sauvegardée");
    }

    delete P_Image;
    ListeCommande[P_Commande]->deleteLater();
}

void MainWindow::TraiterReponseVisualisationCaptureDDM(QString P_Commande, QImage *P_Image)
{

    DDMZones FenetreDDMZones(DDMListeZones, P_Image, this);

    if( FenetreDDMZones.exec() == QDialog::Accepted )
        FenetreDDMZones.getZones(DDMListeZones);

    ListeCommande[P_Commande]->deleteLater();
}

void MainWindow::TraiterReponseDebug(QString P_Reponse)
{
    ui->textEditDebugReponse->append(P_Reponse);
}

void MainWindow::TraiterReponseConfigurationIPRecuperer(QString P_Commande, QMap<QString, QString> P_Reponse)
{
    ui->lineEditConfigurationIPAdresseIP->setText(P_Reponse["ip"]);
    ui->lineEditConfigurationIPPasserelle->setText(P_Reponse["gate"]);
    ui->lineEditConfigurationIPDNS1->setText(P_Reponse["dns1"]);
    ui->lineEditConfigurationIPDNS2->setText(P_Reponse["dns2"]);
    ui->lineEditConfigurationIPMasque->setText(P_Reponse["mask"]);

    ListeCommande[P_Commande]->deleteLater();
}

void MainWindow::TraiterReponseConfigurationDDMRecuperer(QString P_Commande, QMap<QString, QString> P_Reponse)
{
    ui->checkBoxConfigurationDDMActif->setChecked( P_Reponse["isEnable"] == "1" );

    int linkage = P_Reponse["linkage"].toInt();
    ui->checkBoxConfigurationDDMActionSonnerie->setChecked(linkage & 0x1);
    ui->checkBoxConfigurationDDMActionMail->setChecked(linkage & 0x2);
    ui->checkBoxConfigurationDDMActionCaptureEcran->setChecked(linkage & 0x4);
    ui->checkBoxConfigurationDDMActionEnregistrement->setChecked(linkage & 0x8);

    ui->spinBoxConfigurationDDMSnapInterval->setValue( P_Reponse["snapInterval"].toInt() );

    switch (P_Reponse["sensitivity"].toInt())
    {
    case 0: ui->radioButtonConfigurationDDMSensibiliteLow->setChecked(true); break;
    case 1: ui->radioButtonConfigurationDDMSensibiliteNormal->setChecked(true); break;
    case 2: ui->radioButtonConfigurationDDMSensibiliteHigh->setChecked(true); break;
    case 3: ui->radioButtonConfigurationDDMSensibiliteLower->setChecked(true); break;
    case 4: ui->radioButtonConfigurationDDMSensibiliteLowest->setChecked(true); break;
    }

    ui->spinBoxConfigurationDDMTriggerInterval->setValue( P_Reponse["triggerInterval"].toInt() );

    for( int Jour = 0 ; Jour < 7 ; Jour++ )
    {
        long DonneesDuJour = P_Reponse["schedule" + QString::number(Jour)].toLong();
        for( int Creneau = 0 ; Creneau < 48 ; Creneau++ )
        {
            long tmp = ((long)1<<Creneau);
            if( DonneesDuJour & tmp )
                ui->tableWidgetConfigurationDDMProgrammation->item(Jour, Creneau)->setBackgroundColor(QColor(Qt::green));
        }
    }

    for( int NumArea = 0 ; NumArea < 10 ; NumArea++ )
        DDMListeZones[NumArea] = P_Reponse["area" + QString::number(NumArea)].toInt();

    ListeCommande[P_Commande]->deleteLater();
}

void MainWindow::ConfigurationInfosCameraRecuperer(void)
{
    ListeCommande["getDevInfo"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["getDevInfo"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    connect(ListeCommande["getDevInfo"], SIGNAL(sigReponse(QString, QMap<QString,QString>)), this, SLOT(TraiterReponseInformationsCameraRecuperer(QString, QMap<QString,QString>)));
    ListeCommande["getDevInfo"]->EnvoyerCommande("getDevInfo");
}

void MainWindow::AddLog(QString P_Message)
{
    ui->textEditLog->append(P_Message);
}

void MainWindow::MasquerAfficherFenetreLog(bool P_AfficherFenetreLog)
{
    ui->groupBoxLog->setVisible(P_AfficherFenetreLog);
}

void MainWindow::VisualisationZoomIn()
{
    ListeCommande["zoomIn"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["zoomIn"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["zoomIn"]->EnvoyerCommande("zoomIn");
    QTimer::singleShot(ui->spinBoxVisualisationDureeCommande->value(), this, SLOT(VisualisationZoomStop()));
}

void MainWindow::VisualisationZoomOut()
{
    ListeCommande["zoomIn"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["zoomIn"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["zoomIn"]->EnvoyerCommande("zoomOut");
    QTimer::singleShot(ui->spinBoxVisualisationDureeCommande->value(), this, SLOT(VisualisationZoomStop()));
}

void MainWindow::VisualisationZoomStop()
{
    ListeCommande["zoomStop"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["zoomStop"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["zoomStop"]->EnvoyerCommande("zoomStop");
}

void MainWindow::VisualisationPTZTopLeft()
{
    ListeCommande["ptzMoveTopLeft"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["ptzMoveTopLeft"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["ptzMoveTopLeft"]->EnvoyerCommande("ptzMoveTopLeft");
    QTimer::singleShot(ui->spinBoxVisualisationDureeCommande->value(), this, SLOT(VisualisationPTZStop()));
}

void MainWindow::VisualisationPTZTop()
{
    ListeCommande["ptzMoveUp"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["ptzMoveUp"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["ptzMoveUp"]->EnvoyerCommande("ptzMoveUp");
    QTimer::singleShot(ui->spinBoxVisualisationDureeCommande->value(), this, SLOT(VisualisationPTZStop()));
}

void MainWindow::VisualisationPTZTopRight()
{
    ListeCommande["ptzMoveTopRight"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["ptzMoveTopRight"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["ptzMoveTopRight"]->EnvoyerCommande("ptzMoveTopRight");
    QTimer::singleShot(ui->spinBoxVisualisationDureeCommande->value(), this, SLOT(VisualisationPTZStop()));
}

void MainWindow::VisualisationPTZLeft()
{
    ListeCommande["ptzMoveLeft"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["ptzMoveLeft"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["ptzMoveLeft"]->EnvoyerCommande("ptzMoveLeft");
    QTimer::singleShot(ui->spinBoxVisualisationDureeCommande->value(), this, SLOT(VisualisationPTZStop()));
}

void MainWindow::VisualisationPTZRight()
{
    ListeCommande["ptzMoveTopRight"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["ptzMoveTopRight"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["ptzMoveTopRight"]->EnvoyerCommande("ptzMoveTopRight");
    QTimer::singleShot(ui->spinBoxVisualisationDureeCommande->value(), this, SLOT(VisualisationPTZStop()));
}

void MainWindow::VisualisationPTZBottomLeft()
{
    ListeCommande["ptzMoveBottomLeft"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["ptzMoveBottomLeft"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["ptzMoveBottomLeft"]->EnvoyerCommande("ptzMoveBottomLeft");
    QTimer::singleShot(ui->spinBoxVisualisationDureeCommande->value(), this, SLOT(VisualisationPTZStop()));
}

void MainWindow::VisualisationPTZBottom()
{
    ListeCommande["ptzMoveDown"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["ptzMoveDown"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["ptzMoveDown"]->EnvoyerCommande("ptzMoveDown");
    QTimer::singleShot(ui->spinBoxVisualisationDureeCommande->value(), this, SLOT(VisualisationPTZStop()));
}

void MainWindow::VisualisationPTZBottomRight()
{
    ListeCommande["ptzMoveBottomRight"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["ptzMoveBottomRight"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["ptzMoveBottomRight"]->EnvoyerCommande("ptzMoveBottomRight");
    QTimer::singleShot(ui->spinBoxVisualisationDureeCommande->value(), this, SLOT(VisualisationPTZStop()));
}

void MainWindow::VisualisationMajListePresetsRecuperer()
{
    ListeCommande["getPTZPresetPointList"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["getPTZPresetPointList"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    connect(ListeCommande["getPTZPresetPointList"], SIGNAL(sigReponse(QString,QMap<QString,QString>)), this, SLOT(TraiterReponseVisualiserMajListePresets(QString, QMap<QString,QString>)));
    ListeCommande["getPTZPresetPointList"]->EnvoyerCommande("getPTZPresetPointList");
}

void MainWindow::VisualisationAllerAuPreset()
{
    ListeCommande["ptzGotoPresetPoint"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["ptzGotoPresetPoint"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["ptzGotoPresetPoint"]->EnvoyerCommande("ptzGotoPresetPoint&name=" + ui->comboBoxVisualisationListePresets->currentText());
}

void MainWindow::VisualisationPTZStop()
{
    ListeCommande["ptzStopRun"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["ptzStopRun"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["ptzStopRun"]->EnvoyerCommande("ptzStopRun");
}

void MainWindow::VisualisationVideoActive(bool P_VideoActive)
{
   if( P_VideoActive )
       myVlcMediaPlayer->play();
   else
       myVlcMediaPlayer->stop();
}

void MainWindow::VisualisationMirroir(bool P_MirroirActif)
{
    ListeCommande["mirrorVideo"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["mirrorVideo"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));

    if( P_MirroirActif)
        ListeCommande["mirrorVideo"]->EnvoyerCommande("mirrorVideo&isMirror=1");
    else
        ListeCommande["mirrorVideo"]->EnvoyerCommande("mirrorVideo&isMirror=0");
}

void MainWindow::VisualisationRetourner(bool P_RetournerActif)
{
    ListeCommande["flipVideo"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["flipVideo"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));

    if( P_RetournerActif)
        ListeCommande["flipVideo"]->EnvoyerCommande("flipVideo&isFlip=1");
    else
        ListeCommande["flipVideo"]->EnvoyerCommande("flipVideo&isFlip=0");
}

void MainWindow::VisualisationChangerOSD()
{
    QString isEnableTimeStamp = ui->checkBoxVisualisationOSDTemps->isChecked() ? "1":"0";
    QString isEnableDevName   = ui->checkBoxVisualisationOSDNomCamera->isChecked() ? "1":"0";

    ListeCommande["setOSDSetting"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["setOSDSetting"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["setOSDSetting"]->EnvoyerCommande("setOSDSetting&isEnableTimeStamp=" + isEnableTimeStamp + "&isEnableDevName=" + isEnableDevName);
}

void MainWindow::VisualisationCapture()
{
    ListeCommande["snapPicture"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["snapPicture"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    connect(ListeCommande["snapPicture"], SIGNAL(sigReponseSnapPicture(QString,QImage*)), this, SLOT(TraiterReponseVisualisationCapture(QString,QImage*)));
    ListeCommande["snapPicture"]->EnvoyerCommande("snapPicture");
}

void MainWindow::VisualisationDefinirMasques()
{
    DialogMaskArea FenetreMasques(ParametresCameras, this);
    connect(&FenetreMasques, SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    FenetreMasques.exec();
}

void MainWindow::VisualisationRetournerMirroirRecuperer()
{
    ListeCommande["getMirrorAndFlipSetting"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["getMirrorAndFlipSetting"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    connect(ListeCommande["getMirrorAndFlipSetting"], SIGNAL(sigReponse(QString,QMap<QString,QString>)), this, SLOT(TraiterReponseVisualisationRetournerMirroirRecuperer(QString,QMap<QString,QString>)));
    ListeCommande["getMirrorAndFlipSetting"]->EnvoyerCommande("getMirrorAndFlipSetting");
}

void MainWindow::VisualisationOSDRecuperer()
{
    ListeCommande["getOSDSetting"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["getOSDSetting"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    connect(ListeCommande["getOSDSetting"], SIGNAL(sigReponse(QString,QMap<QString,QString>)), this, SLOT(TraiterReponseVisualisationOSDRecuperer(QString,QMap<QString,QString>)));
    ListeCommande["getOSDSetting"]->EnvoyerCommande("getOSDSetting");
}

void MainWindow::VisualisationParametresImageRecuperer()
{
    ListeCommande["getImageSetting"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["getImageSetting"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    connect(ListeCommande["getImageSetting"], SIGNAL(sigReponse(QString,QMap<QString,QString>)), this, SLOT(TraiterReponseVisualisationParametresImageRecuperer(QString,QMap<QString,QString>)));
    ListeCommande["getImageSetting"]->EnvoyerCommande("getImageSetting");
}

void MainWindow::ParametresImageChangerContraste(int P_Valeur)
{
    ui->labelVisualisationParametresImageContrastePourcentage->setText(QString::number(P_Valeur) + "%");

    ListeCommande["setContrast"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["setContrast"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["setContrast"]->EnvoyerCommande("setContrast&constrast=" + QString::number(P_Valeur));
}

void MainWindow::ParametresImageChangerLuminosite(int P_Valeur)
{
    ui->labelVisualisationParametresImageLuminositePourcentage->setText(QString::number(P_Valeur) + "%");

    ListeCommande["setBrightness"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["setBrightness"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["setBrightness"]->EnvoyerCommande("setBrightness&brightness=" + QString::number(P_Valeur));
}

void MainWindow::ParametresImageChangerNettete(int P_Valeur)
{
    ui->labelVisualisationParametresImageNettetePourcentage->setText(QString::number(P_Valeur) + "%");

    ListeCommande["setSharpness"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["setSharpness"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["setSharpness"]->EnvoyerCommande("setSharpness&sharpness=" + QString::number(P_Valeur));
}

void MainWindow::ParametresImageChangerSaturation(int P_Valeur)
{
    ui->labelVisualisationParametresImageSaturationPourcentage->setText(QString::number(P_Valeur) + "%");

    ListeCommande["setSaturation"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["setSaturation"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["setSaturation"]->EnvoyerCommande("setSaturation&saturation=" + QString::number(P_Valeur));
}

void MainWindow::ParametresImageChangerTeinte(int P_Valeur)
{
    ui->labelVisualisationParametresImageTeintePourcentage->setText(QString::number(P_Valeur) + "%");

    ListeCommande["setHue"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["setHue"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["setHue"]->EnvoyerCommande("setHue&hue=" + QString::number(P_Valeur));
}

void MainWindow::ConfigurationNomPeripheriqueDefinir()
{
    ListeCommande["setDevName"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["setDevName"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    ListeCommande["setDevName"]->EnvoyerCommande("setDevName&devName=" + ui->lineEditConfigurationInfosCameraNomPeripherique->text());
}

void MainWindow::ConfigurationIPRecuperer()
{
    ListeCommande["getIPInfo"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["getIPInfo"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    connect(ListeCommande["getIPInfo"], SIGNAL(sigReponse(QString,QMap<QString,QString>)), this, SLOT(TraiterReponseConfigurationIPRecuperer(QString,QMap<QString,QString>)));
    ListeCommande["getIPInfo"]->EnvoyerCommande("getIPInfo");
}

void MainWindow::ConfigurationDDMRecuperer()
{
    ListeCommande["getMotionDetectConfig"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["getMotionDetectConfig"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    connect(ListeCommande["getMotionDetectConfig"], SIGNAL(sigReponse(QString,QMap<QString,QString>)), this, SLOT(TraiterReponseConfigurationDDMRecuperer(QString,QMap<QString,QString>)));
    ListeCommande["getMotionDetectConfig"]->EnvoyerCommande("getMotionDetectConfig");
}

void MainWindow::ConfigurationDDMAppliquer()
{
    ListeCommande["setMotionDetectConfig"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["setMotionDetectConfig"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));

    QString Commande = "setMotionDetectConfig&isEnable=";
    if( ui->checkBoxConfigurationDDMActif->isChecked() )
        Commande += "1";
    else
        Commande += "0";

    int linkage = 0;
    if( ui->checkBoxConfigurationDDMActionSonnerie->isChecked() )
        linkage |= 0x1;
    if( ui->checkBoxConfigurationDDMActionMail->isChecked() )
        linkage |= 0x2;
    if( ui->checkBoxConfigurationDDMActionCaptureEcran->isChecked() )
        linkage |= 0x4;
    if( ui->checkBoxConfigurationDDMActionEnregistrement->isChecked() )
        linkage |= 0x8;
    Commande += "&linkage=" + QString::number(linkage);

    Commande += "&snapInterval=" + QString::number(ui->spinBoxConfigurationDDMSnapInterval->value());

    if( ui->radioButtonConfigurationDDMSensibiliteLow->isChecked()    ) Commande += "&sensitivity=0";
    if( ui->radioButtonConfigurationDDMSensibiliteNormal->isChecked() ) Commande += "&sensitivity=1";
    if( ui->radioButtonConfigurationDDMSensibiliteHigh->isChecked()   ) Commande += "&sensitivity=2";
    if( ui->radioButtonConfigurationDDMSensibiliteLower->isChecked()  ) Commande += "&sensitivity=3";
    if( ui->radioButtonConfigurationDDMSensibiliteLowest->isChecked() ) Commande += "&sensitivity=4";

    Commande += "&triggerInterval=" + QString::number(ui->spinBoxConfigurationDDMTriggerInterval->value());

    for( int Jour = 0 ; Jour < 7 ; Jour++ )
    {
        long DonneesDuJour = 0;
        for( int Creneau = 0 ; Creneau < 48 ; Creneau++ )
        {
            long tmp = ((long)1<<Creneau);
            if( ui->tableWidgetConfigurationDDMProgrammation->item(Jour, Creneau)->backgroundColor() == QColor(Qt::green) )
                DonneesDuJour |= tmp;
        }
        Commande += "&schedule" + QString::number(Jour) + "=" + QString::number(DonneesDuJour);
    }

    for( int zone = 0 ; zone < 10 ; zone++ )
        Commande += "&area" + QString::number(zone) + "=" + QString::number(DDMListeZones[zone]);

    ListeCommande["setMotionDetectConfig"]->EnvoyerCommande(Commande);

}

void MainWindow::ConfigurationDDMProgrammationActiver()
{
    QList<QTableWidgetItem *> ListeCreneaux = ui->tableWidgetConfigurationDDMProgrammation->selectedItems();
    for( int i = 0 ; i < ListeCreneaux.count() ; i++ )
        ListeCreneaux[i]->setBackgroundColor(QColor(Qt::green));
}

void MainWindow::ConfigurationDDMProgrammationDesactiver()
{
    QList<QTableWidgetItem *> ListeCreneaux = ui->tableWidgetConfigurationDDMProgrammation->selectedItems();
    for( int i = 0 ; i < ListeCreneaux.count() ; i++ )
        ListeCreneaux[i]->setBackgroundColor(QColor(Qt::white));
}

void MainWindow::ConfigurationDDMDefinirZones()
{
    ListeCommande["snapPicture"] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande["snapPicture"], SIGNAL(sigLog(QString)), this, SLOT(AddLog(QString)));
    connect(ListeCommande["snapPicture"], SIGNAL(sigReponseSnapPicture(QString,QImage*)), this, SLOT(TraiterReponseVisualisationCaptureDDM(QString,QImage*)));
    ListeCommande["snapPicture"]->EnvoyerCommande("snapPicture");
}

void MainWindow::ConnexionSauver()
{
    QSettings settings(QDir().homePath() + "/.foscamcgiinterface/settings", QSettings::IniFormat);

    settings.setValue("connexion/ip", ui->lineEditConnexionAdresseIPCamera->text());
    settings.setValue("connexion/port", ui->lineEditConnexionPortCamera->text());;
    settings.setValue("connexion/user", ui->lineEditConnexionUser->text());
    settings.setValue("connexion/pwd", ui->lineEditConnexionPassword->text());
}

void MainWindow::ConnexionAppliquer()
{
    TesterConnexion();
}

void MainWindow::LogVider()
{
    ui->textEditLog->clear();
}

void MainWindow::DebugEnvoyerCommande()
{
    ListeCommande[ui->lineEditDebugCommande->text()] = new CommandeCamera(ParametresCameras, this);
    connect(ListeCommande[ui->lineEditDebugCommande->text()], SIGNAL(sigLog(QString)), this, SLOT(TraiterReponseDebug(QString)));

    QString Requete;
    Requete = ui->lineEditDebugCommande->text();

    if( ui->lineEditDebugParam1->text() != "" && ui->lineEditDebugValue1->text() != "" )
        Requete += "&" + ui->lineEditDebugParam1->text() + "=" + ui->lineEditDebugValue1->text();
    if( ui->lineEditDebugParam2->text() != "" && ui->lineEditDebugValue2->text() != "" )
        Requete += "&" + ui->lineEditDebugParam2->text() + "=" + ui->lineEditDebugValue2->text();
    if( ui->lineEditDebugParam3->text() != "" && ui->lineEditDebugValue3->text() != "" )
        Requete += "&" + ui->lineEditDebugParam3->text() + "=" + ui->lineEditDebugValue3->text();

    ListeCommande[ui->lineEditDebugCommande->text()]->EnvoyerCommande(Requete);
}
