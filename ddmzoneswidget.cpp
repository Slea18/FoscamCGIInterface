#include "ddmzoneswidget.h"

#include <QPainter>
#include <QDebug>

DDMZonesWidget::DDMZonesWidget(QImage P_Fond, int *P_ListeZones, QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    Fond = P_Fond;
    for( int i = 0 ; i < 10 ; i++ )
        ListeZones[i] = P_ListeZones[i];

}

void DDMZonesWidget::MajListeZones(int *P_ListeZones)
{
    for( int i = 0 ; i < 10 ; i++ )
        ListeZones[i] = P_ListeZones[i];
}

void DDMZonesWidget::GetListeZones(int *P_ListeZones)
{
    for( int i = 0 ; i < 10 ; i++ )
        P_ListeZones[i] = ListeZones[i];
}

void DDMZonesWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.drawImage(0, 0, Fond);

    for( int ligne = 0 ; ligne < 10 ; ligne++ )
    {
        for( int colone = 0 ; colone < 10 ; colone ++)
        {
            if( ListeZones[ligne] & (1<<colone) )
                painter.drawImage(80*colone, 60*ligne, QImage(":/Images/Images/DDM_Actif.png"));
            else
                painter.drawImage(80*colone, 60*ligne, QImage(":/Images/Images/DDM_Inactif.png"));
        }
    }

}

void DDMZonesWidget::mouseReleaseEvent(QMouseEvent *event)
{

    if( event->button() == Qt::LeftButton )
    {
        int Ligne = int (event->pos().y() / 60);
        int Colonne = int (event->pos().x() / 80);

        ListeZones[Ligne] ^= (1<<Colonne);
    }

    update();
}
