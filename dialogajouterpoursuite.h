#ifndef DIALOGAJOUTERPOURSUITE_H
#define DIALOGAJOUTERPOURSUITE_H

#include <QDialog>
#include <QStringList>

#include "commandecamera.h"

namespace Ui {
class DialogAjouterPoursuite;
}

class DialogAjouterPoursuite : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAjouterPoursuite(T_ParametresCameras P_ParametresCameras, QStringList P_ListePreset, QWidget *parent = nullptr);
    ~DialogAjouterPoursuite();

private:
    Ui::DialogAjouterPoursuite *ui;

    T_ParametresCameras ParametresCameras;
    QStringList ListePreset;
    CommandeCamera *CommandeAjouterPoursuite;

private slots:
    void SendLog(QString P_Message);
    void AjouterPoursuite();

signals:
    void sigLog(QString P_Message);
};

#endif // DIALOGAJOUTERPOURSUITE_H
