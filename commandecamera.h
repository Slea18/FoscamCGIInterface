/*
 * commandecamera.h
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

#ifndef COMMANDECAMERA_H
#define COMMANDECAMERA_H

#include <QString>
#include <QNetworkAccessManager>
#include <QMap>

typedef struct T_ParametresCameras {
    QString AdresseIP;
    QString Port;
    QString User;
    QString Password;
}T_ParametresCameras;

class CommandeCamera : public QObject
{
    Q_OBJECT

public:
    CommandeCamera(T_ParametresCameras P_ParametresCamera, QObject *parent = 0);
    ~CommandeCamera();
    void EnvoyerCommande(QString P_Commande);

private:
    QNetworkAccessManager* NetworkAccessManager;
    QString Requete;
    QString Commande;

private slots:
    void RequeteTerminee(QNetworkReply* P_Reponse);

signals:
    void sigLog(QString P_Message);
    void sigReponse(QString Commande, QMap<QString, QString> Reponse);
};

#endif // COMMANDECAMERA_H
