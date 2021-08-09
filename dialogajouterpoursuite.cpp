#include "dialogajouterpoursuite.h"
#include "ui_dialogajouterpoursuite.h"

#include <QMessageBox>

DialogAjouterPoursuite::DialogAjouterPoursuite(T_ParametresCameras P_ParametresCameras, QStringList P_ListePreset, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAjouterPoursuite)
{
    ui->setupUi(this);

    ParametresCameras = P_ParametresCameras;

    for( int i = 0 ; i < P_ListePreset.count() ; i++ )
    {
        ui->comboBoxDepart->addItem(P_ListePreset[i]);
        ui->comboBoxArrivee->addItem(P_ListePreset[i]);
    }

    connect(ui->pushButtonAnnuler, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->pushButtonAjouter, SIGNAL(clicked()), this, SLOT(AjouterPoursuite()));
}

DialogAjouterPoursuite::~DialogAjouterPoursuite()
{
    delete ui;
}

void DialogAjouterPoursuite::SendLog(QString P_Message)
{
    emit sigLog(P_Message);
}

void DialogAjouterPoursuite::AjouterPoursuite()
{
    if( ui->lineEditNom->text().isEmpty() )
    {
        QMessageBox::warning(this, "Ajouter une poursuite", "La poursuite doit avoir un nom");
        return;
    }

    CommandeAjouterPoursuite = new CommandeCamera(ParametresCameras, this);
    connect(CommandeAjouterPoursuite, SIGNAL(sigLog(QString)), this, SLOT(SendLog(QString)));

    QString Commande = "ptzSetCruiseMap&name=" + ui->lineEditNom->text();
    Commande += "&point1=" + ui->comboBoxDepart->currentText();
    Commande += "&point2=" + ui->comboBoxArrivee->currentText();
    CommandeAjouterPoursuite->EnvoyerCommande(Commande);

    QMessageBox::information(this, "Ajouter une poursuite", "La poursuite a été ajoutée");
    ui->lineEditNom->clear();
}
