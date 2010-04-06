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

#ifndef __EULUMDAT_H__
#define __EULUMDAT_H__

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QTextStream>

class Eulumdat : public QWidget
{ 
    Q_OBJECT

public:
	Eulumdat();
	~Eulumdat();

	QString sIden;
	int iItyp;
	int iIsym;
	int iNc;
	double dDc;
	int iNg;
	double dDg;
	QString sMrn;
	QString sLnam;
	QString sLnum;
	QString sFnam;
	QString sDate;   // 12
	double dL;
	double dB;
	double dH;
	double dLa;
	double dB1;
	double dHC0;
	double dHC90;
	double dHC180;
	double dHC270;
	double dDFF; // 22
	double dLORL;
	double dCFLI;
	double dTILT;
	int iN;
	int * iNL;	// 26a
	QString * sTL;
	double * dTLF;
	QString * sCA;
	QString * sCRG;
	double * dWB;  // 26f
	double dDR[10];
	double * dC;
	double * dG;
	double ** dLcd;
	int iMc;


	QStringList loadFile(QString);
	int saveFile(QString);
	void calcMc();
	
	QStringList validate();
	double downward();
	double output();	
	double* directRadios(QString);

	
private:
	void Init(void);
	void Reset(void);
	
	double getDouble(QTextStream &);
	int getInt(QTextStream &);	
	
	double downwardForPlane(int i);
	double outputForPlane(int i);	
	
	double downwardDFL(double arc);
	double downwardForPlaneDFL(int i, double arc);	
};


#endif // __EULUMDAT_H__
