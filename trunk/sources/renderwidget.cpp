/*
* This file is part of QLumEdit, an ultimate Eulumdat file editor
* Copyright (C) 2007  Krzysztof Struginski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact e-mail: Krzysztof Struginski <cagrin@gmail.com>
*   Program page: http://sourceforge.net/projects/qlumedit
*/

#include <QtGui>
#include <math.h>
#include "renderwidget.h"
#include "vars.h"

RenderWidget::RenderWidget(Eulumdat &ldt, QWidget *parent) : QWidget(parent)
{   
	pldt = &ldt;
	
	QSettings settings("Cagrinlabs", "QLumEdit");
    QString filldiagram = settings.value("filldiagram", QString("true")).toString();
    if(filldiagram == "true") {
    	Vars().fill = true;
   	}
   	else {
    	Vars().fill = false;   		
  	}	
    QString drawlegend = settings.value("drawlegend", QString("true")).toString();
    if(drawlegend == "true") {
    	Vars().legend = true;
   	}
   	else {
    	Vars().legend = false;   		
  	}	  	
  	
	
	setVectors();
}

void RenderWidget::setVectors()
{	
	vectorC0C180.clear();
	vectorC90C270.clear();
	
	int a, b;    
    double x=0, y=0;    
   	max = 0;   
    
    // C0
    if(pldt->iIsym != 3) {
		for(int j=0; j<pldt->iNg; j++) {
			x = pldt->dLcd[0][j] * cos(-(pldt->dG[j] * M_PI / 180.0) + M_PI_2);
			y = pldt->dLcd[0][j] * sin(-(pldt->dG[j] * M_PI / 180.0) + M_PI_2);		
	    	vectorC0C180.append(QPointF(x,y));
   		}
   		
   		for(int j=0; j<pldt->iNg; j++) {
   			if(max < pldt->dLcd[0][j])
   				max = pldt->dLcd[0][j];
	  	}
   		
   	}
   	else {
   		// poszukac C180 w katach, ktory jest taki sam jak C0	 
   		b=0;  		
   		while(pldt->dC[b] < 90.0) {
    		b++;
   		}
   		for(a=0; a<pldt->iMc; a++) {
   			if(pldt->dC[a+b] == 180.0) {
				break;
  			}
  		}
		for(int j=0; j<pldt->iNg; j++) {
			x = pldt->dLcd[a][j] * cos(-(pldt->dG[j] * M_PI / 180.0) + M_PI_2);
			y = pldt->dLcd[a][j] * sin(-(pldt->dG[j] * M_PI / 180.0) + M_PI_2);		
	    	vectorC0C180.append(QPointF(x,y));
   		}  		
   		
   		for(int j=0; j<pldt->iNg; j++) {
   			if(max < pldt->dLcd[a][j])
   				max = pldt->dLcd[a][j];
	  	}   		

  	}

   	
   	// C180
   	if( (pldt->iIsym == 1) || (pldt->iIsym == 3) || (pldt->iIsym == 4) ) {
   		for(int j=0; j<pldt->iNg; j++) {
   			x = vectorC0C180[pldt->iNg-1-j].x();
   			y = vectorC0C180[pldt->iNg-1-j].y();
			vectorC0C180.append(QPointF(-x,y));
	  	}
  	}
  	else {
  		// poszukac C180 w katach	
   		for(a=0; a<pldt->iMc; a++) {
   			if(pldt->dC[a] == 180.0) {
				break;
  			}
  		}
		for(int j=pldt->iNg-1; j>=0; j--) {
			x = pldt->dLcd[a][j] * cos(-(pldt->dG[j] * M_PI / 180.0) + M_PI_2);
			y = pldt->dLcd[a][j] * sin(-(pldt->dG[j] * M_PI / 180.0) + M_PI_2);		
	    	vectorC0C180.append(QPointF(-x,y));
   		}  		
   		
   		for(int j=0; j<pldt->iNg; j++) {
   			if(max < pldt->dLcd[a][j])
   				max = pldt->dLcd[a][j];
	  	}   		
 	}

   	
   	// C90
   	if(pldt->iIsym == 1) {
   		//vectorC90C270 = vectorC0C180;
  	}
  	else if(pldt->iIsym == 3) {
		for(int j=0; j<pldt->iNg; j++) {
			x = pldt->dLcd[0][j] * cos(-(pldt->dG[j] * M_PI / 180.0) + M_PI_2);
			y = pldt->dLcd[0][j] * sin(-(pldt->dG[j] * M_PI / 180.0) + M_PI_2);		
	    	vectorC90C270.append(QPointF(x,y));
   		}
   		
   		for(int j=0; j<pldt->iNg; j++) {
   			if(max < pldt->dLcd[0][j])
   				max = pldt->dLcd[0][j];
	  	}  		
 	}
 	else {
   		// poszukac C90
   		b=0;  		
   		for(a=0; a<pldt->iMc; a++) {
   			if(pldt->dC[a+b] == 90.0) {
				break;
  			}
  		}
		for(int j=0; j<pldt->iNg; j++) {
			x = pldt->dLcd[a][j] * cos(-(pldt->dG[j] * M_PI / 180.0) + M_PI_2);
			y = pldt->dLcd[a][j] * sin(-(pldt->dG[j] * M_PI / 180.0) + M_PI_2);		
	    	vectorC90C270.append(QPointF(x,y));
   		}  		
   		
   		for(int j=0; j<pldt->iNg; j++) {
   			if(max < pldt->dLcd[a][j])
   				max = pldt->dLcd[a][j];
	  	} 		
	}

    
    // C270
   	if(pldt->iIsym == 0) {
   		b=0;  		
   		for(a=0; a<pldt->iMc; a++) {
   			if(pldt->dC[a+b] == 270.0) {
				break;
  			}
  		}
		for(int j=pldt->iNg-1; j>=0; j--) {
			x = pldt->dLcd[a][j] * cos(-(pldt->dG[j] * M_PI / 180.0) + M_PI_2);
			y = pldt->dLcd[a][j] * sin(-(pldt->dG[j] * M_PI / 180.0) + M_PI_2);		
	    	vectorC90C270.append(QPointF(-x,y));
   		}  		
   		
   		for(int j=0; j<pldt->iNg; j++) {
   			if(max < pldt->dLcd[a][j])
   				max = pldt->dLcd[a][j];
	  	} 	   		
  	}
	if(pldt->iIsym == 3) {
   		b=0;  	
   		while(pldt->dC[b] < 90.0) {
    		b++;
   		}   			
   		for(a=0; a<pldt->iMc; a++) {
   			if(pldt->dC[a+b] == 270.0) {
				break;
  			}
  		}
		for(int j=pldt->iNg-1; j>=0; j--) {
			x = pldt->dLcd[a][j] * cos(-(pldt->dG[j] * M_PI / 180.0) + M_PI_2);
			y = pldt->dLcd[a][j] * sin(-(pldt->dG[j] * M_PI / 180.0) + M_PI_2);		
	    	vectorC90C270.append(QPointF(-x,y));
   		}  		
   		
   		for(int j=0; j<pldt->iNg; j++) {
   			if(max < pldt->dLcd[a][j])
   				max = pldt->dLcd[a][j];
	  	} 			
  	}
   	if( (pldt->iIsym == 2) || (pldt->iIsym == 4) ) {
   		for(int j=0; j<pldt->iNg; j++) {
   			x = vectorC90C270[pldt->iNg-1-j].x();
   			y = vectorC90C270[pldt->iNg-1-j].y();
			vectorC90C270.append(QPointF(-x,y));
	  	}   		
  	}  	
    
}

void RenderWidget::paintEvent(QPaintEvent *)
{	
	max = 20.0*ceil(max / 20.0);

	double marg = 60;	
	
	double side = qMin(width()-(int)marg, height()-(int)marg);
	
    double scale = max / side * 2;
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);

    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.setPen(Qt::NoPen); 
    painter.drawRect(QRectF(-side/2-marg/2, -side/2-marg/2, side+marg, side+marg));
    painter.setBrush(Qt::NoBrush);
    
    painter.setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));  
    painter.drawEllipse(QRectF(-side/2, -side/2, side, side));
    painter.drawEllipse(QRectF(-side/4, -side/4, side/2, side/2));
    painter.drawEllipse(QRectF(-side/8, -side/8, side/4, side/4));
    painter.drawEllipse(QRectF(-side/8*3, -side/8*3, side/4*3, side/4*3));     
      
    
    for (int i = 0; i < 12; ++i) {
        //painter.drawLine(QLineF(side/8, 0, side/2, 0));
        painter.drawLine(QLineF(0, 0, side/2, 0));
        painter.rotate(30.0);
    }          


    QRectF * test;
    test = new QRectF;
	int space = 2;    
    
    painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
    painter.setBrush(Qt::NoBrush);    
    painter.drawText(QRectF(QPointF(-100, side/2-marg), QSizeF(200, 2*marg)), Qt::AlignCenter,  QString("%1").arg(max), test);    
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawRect(QRectF(QPointF(test->x()-space, test->y()), QSizeF(test->width()+2*space, test->height())));
    painter.drawRect(QRectF(QPointF(test->x()-space, -(test->y())-test->height()), QSizeF(test->width()+2*space, test->height())));        
	painter.setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));      
    painter.setBrush(Qt::NoBrush);	
	painter.drawText(QRectF(QPointF(test->x()-test->width(), test->y()), QSizeF(3*test->width(), 2*test->height())), Qt::AlignCenter, QString("%1\n 0%2").arg(max).arg((char)176));   	

	painter.drawText(QRectF(QPointF(test->x()-test->width(), -(test->y())-3*test->height()), QSizeF(3*test->width(), 3*test->height())), Qt::AlignHCenter | Qt::AlignBottom, QString(" 180%1\n%2").arg((char)176).arg(max));   		
	
  	
	painter.drawText(QRectF(QPointF(side/2, -test->height()/2), QSizeF(marg, test->height())), Qt::AlignVCenter | Qt::AlignLeft, QString(" 90%1").arg((char)176));
	painter.drawText(QRectF(QPointF(-side/2-marg, -test->height()/2), QSizeF(marg, test->height())), Qt::AlignVCenter | Qt::AlignRight, QString("90%1 ").arg((char)176));	

	double x, y;
	x = side/2 * cos(-(30 * M_PI / 180.0) + M_PI_2);
	y = side/2 * sin(-(30 * M_PI / 180.0) + M_PI_2);		
	painter.drawText(QRectF(QPointF(x, y), QSizeF(marg, 2*test->height())), Qt::AlignTop | Qt::AlignLeft, QString(" 30%1").arg((char)176));	
	x = side/2 * cos(-(60 * M_PI / 180.0) + M_PI_2);
	y = side/2 * sin(-(60 * M_PI / 180.0) + M_PI_2);		
	painter.drawText(QRectF(QPointF(x, y), QSizeF(marg, 2*test->height())), Qt::AlignTop | Qt::AlignLeft, QString(" 60%1").arg((char)176));		
	x = side/2 * cos(-(120 * M_PI / 180.0) + M_PI_2);
	y = side/2 * sin(-(120 * M_PI / 180.0) + M_PI_2);		
	painter.drawText(QRectF(QPointF(x, y-2*test->height()), QSizeF(marg, 2*test->height())), Qt::AlignBottom | Qt::AlignLeft, QString(" 120%1").arg((char)176));	
	x = side/2 * cos(-(150 * M_PI / 180.0) + M_PI_2);
	y = side/2 * sin(-(150 * M_PI / 180.0) + M_PI_2);		
	painter.drawText(QRectF(QPointF(x, y-2*test->height()), QSizeF(marg, 2*test->height())), Qt::AlignBottom | Qt::AlignLeft, QString(" 150%1").arg((char)176));		
	x = side/2 * cos(-(30 * M_PI / 180.0) + M_PI_2);
	y = side/2 * sin(-(30 * M_PI / 180.0) + M_PI_2);		
	painter.drawText(QRectF(QPointF(-x-marg, y), QSizeF(marg, 2*test->height())), Qt::AlignTop | Qt::AlignRight, QString("30%1 ").arg((char)176));	
	x = side/2 * cos(-(60 * M_PI / 180.0) + M_PI_2);
	y = side/2 * sin(-(60 * M_PI / 180.0) + M_PI_2);		
	painter.drawText(QRectF(QPointF(-x-marg, y), QSizeF(marg, 2*test->height())), Qt::AlignTop | Qt::AlignRight, QString("60%1 ").arg((char)176));		
	x = side/2 * cos(-(120 * M_PI / 180.0) + M_PI_2);
	y = side/2 * sin(-(120 * M_PI / 180.0) + M_PI_2);		
	painter.drawText(QRectF(QPointF(-x-marg, y-2*test->height()), QSizeF(marg, 2*test->height())), Qt::AlignBottom | Qt::AlignRight, QString("120%1 ").arg((char)176));	
	x = side/2 * cos(-(150 * M_PI / 180.0) + M_PI_2);
	y = side/2 * sin(-(150 * M_PI / 180.0) + M_PI_2);		
	painter.drawText(QRectF(QPointF(-x-marg, y-2*test->height()), QSizeF(marg, 2*test->height())), Qt::AlignBottom | Qt::AlignRight, QString("150%1 ").arg((char)176));
	
    painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
    painter.setBrush(Qt::NoBrush);    
    painter.drawText(QRectF(QPointF(-100, side/4-marg), QSizeF(200, 2*marg)), Qt::AlignCenter,  QString("%1").arg(max/2), test);       
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawRect(QRectF(QPointF(test->x()-space, test->y()), QSizeF(test->width()+2*space, test->height())));    
    painter.drawRect(QRectF(QPointF(test->x()-space, -(test->y())-test->height()), QSizeF(test->width()+2*space, test->height())));    
	painter.setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));      
    painter.setBrush(Qt::NoBrush);	
	painter.drawText(*test, Qt::AlignCenter,  QString("%1").arg(max/2));
	painter.drawText(QRectF(QPointF(test->x(), -(test->y())-test->height()), QSizeF(test->width(), test->height())), Qt::AlignCenter,  QString("%1").arg(max/2));	

    painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
    painter.setBrush(Qt::NoBrush);    
    painter.drawText(QRectF(QPointF(-100, side/8-marg), QSizeF(200, 2*marg)), Qt::AlignCenter,  QString("%1").arg(max/4), test);       
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawRect(QRectF(QPointF(test->x()-space, test->y()), QSizeF(test->width()+2*space, test->height())));
    painter.drawRect(QRectF(QPointF(test->x()-space, -(test->y())-test->height()), QSizeF(test->width()+2*space, test->height())));        
	painter.setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));      
    painter.setBrush(Qt::NoBrush);	
	painter.drawText(*test, Qt::AlignCenter,  QString("%1").arg(max/4));
	painter.drawText(QRectF(QPointF(test->x(), -(test->y())-test->height()), QSizeF(test->width(), test->height())), Qt::AlignCenter,  QString("%1").arg(max/4));		

	painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
    painter.setBrush(Qt::NoBrush);    
    painter.drawText(QRectF(QPointF(-100, side/8*3-marg), QSizeF(200, 2*marg)), Qt::AlignCenter,  QString("%1").arg(max/4*3), test);       
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawRect(QRectF(QPointF(test->x()-space, test->y()), QSizeF(test->width()+2*space, test->height())));
    painter.drawRect(QRectF(QPointF(test->x()-space, -(test->y())-test->height()), QSizeF(test->width()+2*space, test->height())));    
	painter.setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));      
    painter.setBrush(Qt::NoBrush);	
	painter.drawText(*test, Qt::AlignCenter,  QString("%1").arg(max/4*3));
	painter.drawText(QRectF(QPointF(test->x(), -(test->y())-test->height()), QSizeF(test->width(), test->height())), Qt::AlignCenter,  QString("%1").arg(max/4*3));		
   
    
	int alpha = 48;
	
	// C90-C270	
	
	QVector<QPointF> pointsC90C270;       
    for(int i=0; i<vectorC90C270.size(); i++) {
    	pointsC90C270.append(QPointF(-vectorC90C270[i].x() / scale, vectorC90C270[i].y() / scale));   	
   	}
    
    QPainterPath pathC90C270;   
    pathC90C270.moveTo(pointsC90C270[0]);
    
    for(int i=1; i<pointsC90C270.size(); i++) {
    	pathC90C270.lineTo(pointsC90C270[i]);
   	}
   
    painter.setPen(QPen(Qt::red, 2, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));   
    painter.setBrush(QBrush(QColor(255, 0, 0, alpha), Qt::SolidPattern));
    if(Vars().fill) {
	    painter.setBrush(QBrush(QColor(255, 0, 0, alpha), Qt::SolidPattern));    	
   	}
   	else {
   		painter.setBrush(Qt::NoBrush);
  	}    
    painter.drawPath(pathC90C270);

    // C0-C180
    
    QVector<QPointF> pointsC0C180;       
    for(int i=0; i<vectorC0C180.size(); i++) {
    	pointsC0C180.append(QPointF(-vectorC0C180[i].x() / scale, vectorC0C180[i].y() / scale));   	
   	}
    
    QPainterPath pathC0C180;   
    pathC0C180.moveTo(pointsC0C180[0]);
    
    for(int i=1; i<pointsC0C180.size(); i++) {
    	pathC0C180.lineTo(pointsC0C180[i]);
   	}
   
    painter.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)); 
    if(Vars().fill) {
	    painter.setBrush(QBrush(QColor(0, 0, 255, alpha), Qt::SolidPattern));      	
   	}
   	else {
   		painter.setBrush(Qt::NoBrush);
  	}
  
    painter.drawPath(pathC0C180);
    
	if(Vars().legend == true) {  
	    // legend
	    painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
	    painter.setBrush(Qt::NoBrush);    
	    painter.drawText(QRectF(QPointF(side/2-100, side/2-test->height()), QSizeF(100, 2*test->height())), Qt::AlignRight | Qt::AlignBottom,  "cd/1000lm"); 
	    
	    if(Vars().mainWindow->ldt->iIsym == 1) {
	    	painter.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		    painter.setBrush(QBrush(QColor(0, 0, 255, alpha), Qt::SolidPattern));
		    painter.drawRect(QRectF(QPointF(-side/2-test->height(), side/2), QSizeF(test->height(), test->height())));    
		        
		    painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
		    painter.setBrush(Qt::NoBrush);    
		    painter.drawText(QRectF(QPointF(-side/2+0.5*test->height(), side/2), QSizeF(side, 2*test->height())), Qt::AlignLeft,  "C0-C180 / C90-C270");
	     	
	   	}
		else {
		    painter.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)); 
		    painter.setBrush(QBrush(QColor(0, 0, 255, alpha), Qt::SolidPattern));
		    painter.drawRect(QRectF(QPointF(-side/2-test->height(), side/2-1.5*test->height()), QSizeF(test->height(), test->height())));    
		        
		    painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
		    painter.setBrush(Qt::NoBrush);    
		    painter.drawText(QRectF(QPointF(-side/2+0.5*test->height(), side/2-1.5*test->height()), QSizeF(side, 2*test->height())), Qt::AlignLeft,  "C0-C180");    
		    
		    painter.setPen(QPen(Qt::red, 2, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin)); 
		    painter.setBrush(QBrush(QColor(255, 0, 0, alpha), Qt::SolidPattern));
		    painter.drawRect(QRectF(QPointF(-side/2-test->height(), side/2), QSizeF(test->height(), test->height())));    
		        
		    painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
		    painter.setBrush(Qt::NoBrush);    
		    painter.drawText(QRectF(QPointF(-side/2+0.5*test->height(), side/2), QSizeF(side, 2*test->height())), Qt::AlignLeft,  "C90-C270");
	    		
		}
	}

}


void RenderWidget::saveImage(QString fileName, int resolution) {
	
	QImage *image = new QImage(resolution, resolution, QImage::Format_RGB32);
	
	max = 20.0*ceil(max / 20.0);
	double marg = 60;	
	double side = resolution-(int)marg;
    double scale = max / side * 2;
    
    QPainter painter(image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(resolution / 2, resolution / 2);

    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.setPen(Qt::NoPen); 
    painter.drawRect(QRectF(-side/2-marg/2, -side/2-marg/2, side+marg, side+marg));
    painter.setBrush(Qt::NoBrush);
    
    painter.setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));  
    painter.drawEllipse(QRectF(-side/2, -side/2, side, side));
    painter.drawEllipse(QRectF(-side/4, -side/4, side/2, side/2));
    painter.drawEllipse(QRectF(-side/8, -side/8, side/4, side/4));
    painter.drawEllipse(QRectF(-side/8*3, -side/8*3, side/4*3, side/4*3));     
      
    
    for (int i = 0; i < 12; ++i) {
        //painter.drawLine(QLineF(side/8, 0, side/2, 0));
        painter.drawLine(QLineF(0, 0, side/2, 0));
        painter.rotate(30.0);
    }          


    QRectF * test;
    test = new QRectF;
	int space = 2;    
    
    painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
    painter.setBrush(Qt::NoBrush);    
    painter.drawText(QRectF(QPointF(-100, side/2-marg), QSizeF(200, 2*marg)), Qt::AlignCenter,  QString("%1").arg(max), test);    
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawRect(QRectF(QPointF(test->x()-space, test->y()), QSizeF(test->width()+2*space, test->height())));
    painter.drawRect(QRectF(QPointF(test->x()-space, -(test->y())-test->height()), QSizeF(test->width()+2*space, test->height())));        
	painter.setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));      
    painter.setBrush(Qt::NoBrush);	
	painter.drawText(QRectF(QPointF(test->x()-test->width(), test->y()), QSizeF(3*test->width(), 2*test->height())), Qt::AlignCenter, QString("%1\n 0%2").arg(max).arg((char)176));   	
	painter.drawText(QRectF(QPointF(test->x()-test->width(), -(test->y())-3*test->height()), QSizeF(3*test->width(), 3*test->height())), Qt::AlignHCenter | Qt::AlignBottom, QString(" 180%1\n%2").arg((char)176).arg(max));   		
	
	//painter.drawText(QRectF(QPointF(-100, -side/2-test->height()), QSizeF(200, 2*test->height())), Qt::AlignHCenter | Qt::AlignBottom, QString(" 180%1\n").arg((char)176)); 
	  	
	painter.drawText(QRectF(QPointF(side/2, -test->height()/2), QSizeF(marg, test->height())), Qt::AlignVCenter | Qt::AlignLeft, QString(" 90%1").arg((char)176));
	painter.drawText(QRectF(QPointF(-side/2-marg, -test->height()/2), QSizeF(marg, test->height())), Qt::AlignVCenter | Qt::AlignRight, QString("90%1 ").arg((char)176));	

	double x, y;
	x = side/2 * cos(-(30 * M_PI / 180.0) + M_PI_2);
	y = side/2 * sin(-(30 * M_PI / 180.0) + M_PI_2);		
	painter.drawText(QRectF(QPointF(x, y), QSizeF(marg, 2*test->height())), Qt::AlignTop | Qt::AlignLeft, QString(" 30%1").arg((char)176));	
	x = side/2 * cos(-(60 * M_PI / 180.0) + M_PI_2);
	y = side/2 * sin(-(60 * M_PI / 180.0) + M_PI_2);		
	painter.drawText(QRectF(QPointF(x, y), QSizeF(marg, 2*test->height())), Qt::AlignTop | Qt::AlignLeft, QString(" 60%1").arg((char)176));		
	x = side/2 * cos(-(120 * M_PI / 180.0) + M_PI_2);
	y = side/2 * sin(-(120 * M_PI / 180.0) + M_PI_2);		
	painter.drawText(QRectF(QPointF(x, y-2*test->height()), QSizeF(marg, 2*test->height())), Qt::AlignBottom | Qt::AlignLeft, QString(" 120%1").arg((char)176));	
	x = side/2 * cos(-(150 * M_PI / 180.0) + M_PI_2);
	y = side/2 * sin(-(150 * M_PI / 180.0) + M_PI_2);		
	painter.drawText(QRectF(QPointF(x, y-2*test->height()), QSizeF(marg, 2*test->height())), Qt::AlignBottom | Qt::AlignLeft, QString(" 150%1").arg((char)176));		
	x = side/2 * cos(-(30 * M_PI / 180.0) + M_PI_2);
	y = side/2 * sin(-(30 * M_PI / 180.0) + M_PI_2);		
	painter.drawText(QRectF(QPointF(-x-marg, y), QSizeF(marg, 2*test->height())), Qt::AlignTop | Qt::AlignRight, QString("30%1 ").arg((char)176));	
	x = side/2 * cos(-(60 * M_PI / 180.0) + M_PI_2);
	y = side/2 * sin(-(60 * M_PI / 180.0) + M_PI_2);		
	painter.drawText(QRectF(QPointF(-x-marg, y), QSizeF(marg, 2*test->height())), Qt::AlignTop | Qt::AlignRight, QString("60%1 ").arg((char)176));		
	x = side/2 * cos(-(120 * M_PI / 180.0) + M_PI_2);
	y = side/2 * sin(-(120 * M_PI / 180.0) + M_PI_2);		
	painter.drawText(QRectF(QPointF(-x-marg, y-2*test->height()), QSizeF(marg, 2*test->height())), Qt::AlignBottom | Qt::AlignRight, QString("120%1 ").arg((char)176));	
	x = side/2 * cos(-(150 * M_PI / 180.0) + M_PI_2);
	y = side/2 * sin(-(150 * M_PI / 180.0) + M_PI_2);		
	painter.drawText(QRectF(QPointF(-x-marg, y-2*test->height()), QSizeF(marg, 2*test->height())), Qt::AlignBottom | Qt::AlignRight, QString("150%1 ").arg((char)176));
	
    painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
    painter.setBrush(Qt::NoBrush);    
    painter.drawText(QRectF(QPointF(-100, side/4-marg), QSizeF(200, 2*marg)), Qt::AlignCenter,  QString("%1").arg(max/2), test);       
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawRect(QRectF(QPointF(test->x()-space, test->y()), QSizeF(test->width()+2*space, test->height())));    
    painter.drawRect(QRectF(QPointF(test->x()-space, -(test->y())-test->height()), QSizeF(test->width()+2*space, test->height())));    
	painter.setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));      
    painter.setBrush(Qt::NoBrush);	
	painter.drawText(*test, Qt::AlignCenter,  QString("%1").arg(max/2));
	painter.drawText(QRectF(QPointF(test->x(), -(test->y())-test->height()), QSizeF(test->width(), test->height())), Qt::AlignCenter,  QString("%1").arg(max/2));	

    painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
    painter.setBrush(Qt::NoBrush);    
    painter.drawText(QRectF(QPointF(-100, side/8-marg), QSizeF(200, 2*marg)), Qt::AlignCenter,  QString("%1").arg(max/4), test);       
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawRect(QRectF(QPointF(test->x()-space, test->y()), QSizeF(test->width()+2*space, test->height())));
    painter.drawRect(QRectF(QPointF(test->x()-space, -(test->y())-test->height()), QSizeF(test->width()+2*space, test->height())));        
	painter.setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));      
    painter.setBrush(Qt::NoBrush);	
	painter.drawText(*test, Qt::AlignCenter,  QString("%1").arg(max/4));
	painter.drawText(QRectF(QPointF(test->x(), -(test->y())-test->height()), QSizeF(test->width(), test->height())), Qt::AlignCenter,  QString("%1").arg(max/4));		

	painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
    painter.setBrush(Qt::NoBrush);    
    painter.drawText(QRectF(QPointF(-100, side/8*3-marg), QSizeF(200, 2*marg)), Qt::AlignCenter,  QString("%1").arg(max/4*3), test);       
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawRect(QRectF(QPointF(test->x()-space, test->y()), QSizeF(test->width()+2*space, test->height())));
    painter.drawRect(QRectF(QPointF(test->x()-space, -(test->y())-test->height()), QSizeF(test->width()+2*space, test->height())));    
	painter.setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));      
    painter.setBrush(Qt::NoBrush);	
	painter.drawText(*test, Qt::AlignCenter,  QString("%1").arg(max/4*3));
	painter.drawText(QRectF(QPointF(test->x(), -(test->y())-test->height()), QSizeF(test->width(), test->height())), Qt::AlignCenter,  QString("%1").arg(max/4*3));		
   
    
	int alpha = 48;
	
	// C90-C270	
	
	QVector<QPointF> pointsC90C270;       
    for(int i=0; i<vectorC90C270.size(); i++) {
    	pointsC90C270.append(QPointF(-vectorC90C270[i].x() / scale, vectorC90C270[i].y() / scale));   	
   	}
    
    QPainterPath pathC90C270;   
    pathC90C270.moveTo(pointsC90C270[0]);
    
    for(int i=1; i<pointsC90C270.size(); i++) {
    	pathC90C270.lineTo(pointsC90C270[i]);
   	}
   
    painter.setPen(QPen(Qt::red, 2, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));   
    if(Vars().fill) {
	    painter.setBrush(QBrush(QColor(255, 0, 0, alpha), Qt::SolidPattern));    	
   	}
   	else {
   		painter.setBrush(Qt::NoBrush);
  	}       
    painter.drawPath(pathC90C270);

    // C0-C180
    
    QVector<QPointF> pointsC0C180;       
    for(int i=0; i<vectorC0C180.size(); i++) {
    	pointsC0C180.append(QPointF(-vectorC0C180[i].x() / scale, vectorC0C180[i].y() / scale));   	
   	}
    
    QPainterPath pathC0C180;   
    pathC0C180.moveTo(pointsC0C180[0]);
    
    for(int i=1; i<pointsC0C180.size(); i++) {
    	pathC0C180.lineTo(pointsC0C180[i]);
   	}
   
    painter.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)); 
    if(Vars().fill) {
	    painter.setBrush(QBrush(QColor(0, 0, 255, alpha), Qt::SolidPattern));    	
   	}
   	else {
   		painter.setBrush(Qt::NoBrush);
  	}       
    painter.drawPath(pathC0C180);
    
	if(Vars().legend == true) {      
	    // legend
	    painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
	    painter.setBrush(Qt::NoBrush);    
	    painter.drawText(QRectF(QPointF(side/2-100, side/2-test->height()), QSizeF(100, 2*test->height())), Qt::AlignRight | Qt::AlignBottom,  "cd/1000lm"); 
	    
	    if(Vars().mainWindow->ldt->iIsym == 1) {
	    	painter.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		    painter.setBrush(QBrush(QColor(0, 0, 255, alpha), Qt::SolidPattern));
		    painter.drawRect(QRectF(QPointF(-side/2-test->height(), side/2), QSizeF(test->height(), test->height())));    
		        
		    painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
		    painter.setBrush(Qt::NoBrush);    
		    painter.drawText(QRectF(QPointF(-side/2+0.5*test->height(), side/2), QSizeF(side, 2*test->height())), Qt::AlignLeft,  "C0-C180 / C90-C270");
	     	
	   	}
		else {
		    painter.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)); 
		    painter.setBrush(QBrush(QColor(0, 0, 255, alpha), Qt::SolidPattern));
		    painter.drawRect(QRectF(QPointF(-side/2-test->height(), side/2-1.5*test->height()), QSizeF(test->height(), test->height())));    
		        
		    painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
		    painter.setBrush(Qt::NoBrush);    
		    painter.drawText(QRectF(QPointF(-side/2+0.5*test->height(), side/2-1.5*test->height()), QSizeF(side, 2*test->height())), Qt::AlignLeft,  "C0-C180");    
		    
		    painter.setPen(QPen(Qt::red, 2, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin)); 
		    painter.setBrush(QBrush(QColor(255, 0, 0, alpha), Qt::SolidPattern));
		    painter.drawRect(QRectF(QPointF(-side/2-test->height(), side/2), QSizeF(test->height(), test->height())));    
		        
		    painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
		    painter.setBrush(Qt::NoBrush);    
		    painter.drawText(QRectF(QPointF(-side/2+0.5*test->height(), side/2), QSizeF(side, 2*test->height())), Qt::AlignLeft,  "C90-C270");
	    		
		}	
	}
	
	image->save(fileName, "PNG");
}
