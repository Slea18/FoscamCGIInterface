/*
 * commandecamera.cpp
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

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>
#include <QDebug>
#include <QMessageBox>

#include "commandecamera.h"

CommandeCamera::CommandeCamera(T_ParametresCameras P_ParametresCamera, QObject *parent) :
    QObject(parent)
{
    NetworkAccessManager = new QNetworkAccessManager(this);

    Requete  = "http://";
    Requete += P_ParametresCamera.AdresseIP;
    Requete += ":";
    Requete += P_ParametresCamera.Port;
    Requete += "/cgi-bin/CGIProxy.fcgi?usr=";
    Requete += P_ParametresCamera.User;
    Requete += "&pwd=";
    Requete += P_ParametresCamera.Password;
    Requete += "&cmd=";

    connect(NetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(RequeteTerminee(QNetworkReply*)));
}

void CommandeCamera::EnvoyerCommande(QString P_Commande)
{
    Commande = P_Commande;
    Requete += P_Commande;
    emit sigLog("-I- Requete : " + Requete);
    NetworkAccessManager->get(QNetworkRequest(QUrl(Requete)));
}

CommandeCamera::~CommandeCamera()
{
    delete NetworkAccessManager;
}

void CommandeCamera::RequeteTerminee(QNetworkReply* P_Reponse)
{
    if( P_Reponse->error() )
        emit sigLog("-I- Reponse : ERREUR (" + P_Reponse->errorString() + ")");
    else
    {
        QMap<QString, QString> Reponse;

        QString Donnees = P_Reponse->readAll();
        emit sigLog("-I- Reponse : \n" + Donnees);

        QDomDocument DomDocument("Reponse");
        DomDocument.setContent(Donnees);
        QDomElement DomElement = DomDocument.documentElement();

        QDomNode DomNode = DomElement.firstChild();
        while(!DomNode.isNull())
        {
            QDomElement DomElement = DomNode.toElement();
            if(!DomElement.isNull())
                Reponse[DomElement.tagName()] = DomElement.text();
            DomNode = DomNode.nextSibling();
        }

        int CodeRetour = Reponse["result"].toInt();
        if( CodeRetour == 0 )
        {
            emit sigReponse(Commande, Reponse);
        }
        else
        {
            QString ErrorMessage;
            switch( CodeRetour )
            {
                case -1 : ErrorMessage = "CGI request string format error"; break;
                case -2 : ErrorMessage = "Username or password error"; break;
                case -3 : ErrorMessage = "Access deny"; break;
                case -4 : ErrorMessage = "CGI execute fail"; break;
                case -5 : ErrorMessage = "Timeout"; break;
                case -6 : ErrorMessage = "Reserve"; break;
                case -7 : ErrorMessage = "Unknown error"; break;
                case -8 : ErrorMessage = "Reserve"; break;
            }

            QMessageBox::warning(NULL, QString("Erreur de communication"), QString("La caméra à répondu une erreur : ") + ErrorMessage);
        }
    }

    P_Reponse->deleteLater();
    P_Reponse = NULL;
}

