#ifndef DDMZONESWIDGET_H
#define DDMZONESWIDGET_H

#include <QWidget>
#include <QImage>
#include <QMouseEvent>

class DDMZonesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DDMZonesWidget(QImage P_Fond, int* P_ListeZones, QWidget *parent = nullptr);
    void MajListeZones(int* P_ListeZones);
    void GetListeZones(int* P_ListeZones);

private:
    QImage Fond;
    int ListeZones[10];

protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent( QMouseEvent * event );
signals:

};

#endif // DDMZONESWIDGET_H
