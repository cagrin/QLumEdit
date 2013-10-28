#ifndef __RENDERWIDGET_H__
#define __RENDERWIDGET_H__

#include <QtWidgets>

#include <math.h>

#include "eulumdat.h"

class RenderWidget: public QWidget
{
    Q_OBJECT

public:
    RenderWidget(Eulumdat &ldt, QWidget *parent = 0);
    
    void setVectors();
	void saveImage(QString, int);    
    
    QVector<QPointF> vectorC0C180, vectorC90C270;
    double max;
	Eulumdat * pldt;    

protected:
    void paintEvent(QPaintEvent *event);
};

#endif
