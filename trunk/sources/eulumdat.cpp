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

#include "eulumdat.h"

#include "vars.h"

Eulumdat::Eulumdat() {
	Init();
}

Eulumdat::~Eulumdat() {
	Reset();
}

void Eulumdat::Init(void) {
	iNL = NULL;	
	sTL = NULL;
	dTLF = NULL;
	sCA = NULL;
	sCRG = NULL;
	dWB = NULL;
	dC = NULL;
	dG = NULL;
	dLcd = NULL;
}
void Eulumdat::Reset(void){

	if(iNL != NULL) delete[] iNL;		
	if(sTL != NULL) delete[] sTL;
	if(dTLF != NULL) delete[] dTLF;
	if(sCA != NULL) delete[] sCA;
	if(sCRG != NULL) delete[] sCRG;
	if(dWB != NULL) delete[] dWB;
	if(dC != NULL) delete[] dC;
	if(dG != NULL) delete[] dG;
	if(dLcd != NULL) {
		for(int i=0; i<iMc; i++) {
			delete dLcd[i];
		}	
		delete[] dLcd;	
	}
	
}

double Eulumdat::getDouble(QTextStream & text) {
	QString line = text.readLine();
	
	line.replace("_", "");
	line.replace(" ", "");	
	line.replace(",", ".");
	
	if(line.isEmpty() || line.isNull()) {
		return 0;
	}

	bool ok;
	double temp = line.toDouble(&ok);
    if(!ok) {
    	QString error;
    	QTextStream out(&error);
    	out << tr("Error reading file at") << " " << text.pos() << tr(" byte");

		throw error;
   	}
   	return temp;
}

int Eulumdat::getInt(QTextStream & text) {
	QString line = text.readLine();

	if(line.isEmpty() || line.isNull()) {
		return 0;
	}

	bool ok;
	int temp = line.toInt(&ok);
    if(!ok) {
    	QString error;
    	QTextStream out(&error);
    	out << tr("Error reading file at") << " " << text.pos() << tr(" byte");
		throw error;    	
   	}
   	return temp;
}

QStringList Eulumdat::loadFile(QString fileName) {
	
	Reset();
	Init();
	
	QFile file(fileName); 
	file.open(QFile::ReadOnly | QFile::Text);  	
   	QTextStream in(&file);

   	sIden = in.readLine();
   	iItyp = getInt(in);
   	iIsym = getInt(in);
   	iNc = getInt(in);
   	dDc = getDouble(in);
   	iNg = getInt(in);
   	dDg = getDouble(in);
	sMrn = in.readLine();
	sLnam = in.readLine();
	sLnum = in.readLine();
	sFnam = in.readLine();
	sDate = in.readLine();
	dL = getDouble(in);
	dB = getDouble(in);
	dH = getDouble(in);
	dLa = getDouble(in);
	dB1 = getDouble(in);
	dHC0 = getDouble(in);
	dHC90 = getDouble(in);
	dHC180 = getDouble(in);
	dHC270 = getDouble(in);	
	dDFF = getDouble(in);
	dLORL = getDouble(in);
	dCFLI = getDouble(in);
	dTILT = getDouble(in);
	iN = getInt(in);
	
	iNL = new int[iN];
	sTL = new QString[iN];
	dTLF = new double[iN];
	sCA = new QString[iN];
	sCRG = new QString[iN];
	dWB = new double[iN];
	
	for(int i=0; i<iN; i++) {
		iNL[i] = getInt(in);
		sTL[i] = in.readLine();
		dTLF[i] = getDouble(in);
		sCA[i] = in.readLine();
		sCRG[i] = in.readLine();
		dWB[i] = getDouble(in);	
	}
	
	for(int i=0; i<10; i++) {
		dDR[i] = getDouble(in);	
	}	
	
	dC = new double[iNc];
	dG = new double[iNg];

	for(int i=0; i<iNc; i++) {
		dC[i] = getDouble(in);
	}
	for(int i=0; i<iNg; i++) {
		dG[i] = getDouble(in);
	}	

	calcMc();	
	
	dLcd = new double*[iMc];
	for(int i=0; i<iMc; i++) {
		dLcd[i] = new double[iNg];
	}
	
	for(int i=0; i<iMc; i++) {
		for(int j=0; j<iNg; j++) {
			dLcd[i][j] = getDouble(in);
		}
	}
	
	file.close();

	return validate();
	
}

int Eulumdat::saveFile(QString fileName) {
	
	QFile file(fileName);
	file.open(QFile::WriteOnly | QFile::Text);
    QTextStream out(&file);
	
   	out << sIden << endl;	
   	
   	if(iItyp != 2) {
   		if(iIsym == 1) {
	   		out << 1 << endl;	
  		}
  		else {
			out << 3 << endl;  			
 		}
  	}
  	else {
  		out << 2 << endl;
 	}
   	   	
   	out << iIsym << endl;
   	out << iNc << endl;
   	out << dDc << endl;
   	out << iNg << endl;
   	out << dDg << endl;
   	out << sMrn << endl;
   	out << sLnam << endl;
   	out << sLnum << endl;
   	out << sFnam << endl;
   	out << sDate << endl;
   	out << dL << endl;
   	out << dB << endl;
   	out << dH << endl;
   	out << dLa << endl;
   	out << dB1 << endl;
   	out << dHC0 << endl;
   	out << dHC90 << endl;
   	out << dHC180 << endl;  	   	   	
   	out << dHC270 << endl;
   	out << dDFF << endl;
   	out << dLORL << endl;
   	out << dCFLI << endl;
   	out << dTILT << endl;
	
   	out << iN << endl;   	 
	for(int i=0; i<iN; i++) {
		out << iNL[i] << endl;
		out << sTL[i] << endl;
		out << dTLF[i] << endl;
		out << sCA[i] << endl;
		out << sCRG[i] << endl;
		out << dWB[i] << endl;
	}   
	for(int i=0; i<10; i++) {
		out << dDR[i] << endl;
	}
	for(int i=0; i<iNc; i++) {
		out << dC[i] << endl;
	}
	for(int i=0; i<iNg; i++) {
		out << dG[i] << endl;
	}
	for(int i=0; i<iMc; i++) {
		for(int j=0; j<iNg; j++) {
			out << dLcd[i][j] << endl;
		}
	}
	
	out << flush;
	file.close();
	
	return 0;
}

void Eulumdat::calcMc() {
    switch(iIsym) {
    	case 0: iMc=iNc;
    	break;
    	case 1: iMc=1;
    	break;
    	case 2: case 3: iMc=iNc/2 + 1;
    	break;
    	case 4: iMc=iNc/4 + 1;
    	break;
   	}	
}

QStringList Eulumdat::validate() {
	QStringList warnings;
	QString error;
	QString sep = " -> ";

	if(sIden.length() > Vars().line) {
		 warnings << tr("Identification") + QString(" = %1").arg(sIden.length())
		  + sep + tr("line is too long");
	}
	if( (iItyp < 1) || (iItyp > 3) ) {
		 error = tr("Type indicator") + QString(" = %1").arg(iItyp)
		 + sep + tr("value is out of range");
		 throw error;
	}
	if( (iIsym < 0) || (iIsym > 4) ) {
		 error = tr("Symmetry indicator") + QString(" = %1").arg(iIsym)
		 + sep + tr("value is out of range");		 
		 throw error;
	}	
	if( (iNc < 0) || (iNc > 721) ) {
		 error = tr("Number of C-planes") + QString(" = %1").arg(iNc)
		 + sep + tr("value is out of range");
		 throw error;
	}
	if( (dDc < 0) || (dDc > 360) ) {
		 error = tr("Distance between C-planes") + QString(" = %1").arg(dDc)
		 + sep + tr("value is out of range");
		 throw error;
	}
	if( (iNg < 0) || (iNg > 361) ) {
		 error = tr("Number of luminous intensities in each C-plane") + QString(" = %1").arg(iNg)
		 + sep + tr("value is out of range");
		 throw error;
	}
	if( (dDg < 0) || (dDg > 180) ) {
		 error = tr("Distance between luminous intensities per C-plane") + QString(" = %1").arg(dDg)
		 + sep + tr("value is out of range");
		 throw error;
	}				
	if(sMrn.length() > Vars().line) {
		 warnings << tr("Measurement report number") + QString(" = %1").arg(sMrn.length())
		 + sep + tr("line is too long");
	}	
	if(sLnam.length() > Vars().line) {
		 warnings << tr("Luminaire name") + QString(" = %1").arg(sLnam.length())
		 + sep + tr("line is too long");
	}		
	if(sLnum.length() > Vars().line) {
		 warnings << tr("Luminaire number") + QString(" = %1").arg(sLnum.length())
		 + sep + tr("line is too long");
	}
	if(sFnam.length() > Vars().lineFNam) {
		 warnings << tr("File name") + QString(" = %1").arg(sFnam.length())
		 + sep + tr("line is too long");
	}
	if(sDate.length() > Vars().line) {
		 warnings << tr("Date/user") + QString(" = %1").arg(sDate.length())
		 + sep + tr("line is too long");
	}
	if( (dL < 1) || (dL > 9999) ) {
		 warnings << tr("Length/diameter of luminaire") + QString(" = %1").arg(dL)
		 + sep + tr("value is out of range");
	}
	if( (dB < 0) || (dB > 9999) ) {
		 warnings << tr("Width of luminaire") + QString(" = %1").arg(dB)
		 + sep + tr("value is out of range");
	}
	if( (dH < 0) || (dH > 9999) ) {
		 warnings << tr("Height of luminaire") + QString(" = %1").arg(dH)
		 + sep + tr("value is out of range");
	}
	if( (dLa < 0) || (dLa > 9999) ) {
		 warnings << tr("Length/diameter of luminous area") + QString(" = %1").arg(dLa)
		 + sep + tr("value is out of range");
	}
	if( (dB1 < 0) || (dB1 > 9999) ) {
		 warnings << tr("Width of luminous area") + QString(" = %1").arg(dB1)
		 + sep + tr("value is out of range");
	}
	if( (dHC0 < 0) || (dHC0 > 9999) ) {
		 warnings << tr("Height of luminous area C0-plane") + QString(" = %1").arg(dHC0)
		 + sep + tr("value is out of range");
	}
	if( (dHC90 < 0) || (dHC90 > 9999) ) {
		 warnings << tr("Height of luminous area C90-plane") + QString(" = %1").arg(dHC90)
		 + sep + tr("value is out of range");
	}
	if( (dHC180 < 0) || (dHC180 > 9999) ) {
		 warnings << tr("Height of luminous area C180-plane") + QString(" = %1").arg(dHC180)
		 + sep + tr("value is out of range");
	}
	if( (dHC270 < 0) || (dHC270 > 9999) ) {
		 warnings << tr("Height of luminous area C270-plane") + QString(" = %1").arg(dHC270)
		 + sep + tr("value is out of range");
	}
	if( (dDFF < 0) || (dDFF > 100) ) {
		 warnings << tr("Downward flux fraction") + QString(" = %1").arg(dDFF)
		 + sep + tr("value is out of range");
	}
	if( (dLORL < 0) || (dLORL > 100) ) {
		 warnings << tr("Light output ratio of luminaire") + QString(" = %1").arg(dLORL)
		 + sep + tr("value is out of range");
	}
	if( (dCFLI < 0) || (dCFLI > 10) ) {
		 warnings << tr("Conversion factor for luminous intensities") + QString(" = %1").arg(dCFLI)
		 + sep + tr("value is out of range");
	}
	if( (dTILT < -180) || (dTILT > 180) ) {
		 warnings << tr("Tilt of luminaire during measurement") + QString(" = %1").arg(dTILT)
		 + sep + tr("value is out of range");
	}	
	if( (iN < 1) || (iN > 20) ) {	
		 error = tr("Number of standard sets of lamps") + QString(" = %1").arg(iN)
		 + sep + tr("value is out of range");
		 throw error;		 
	}
	for(int i=0; i<iN; i++) {
		if( (iNL[i] < 1) || (iNL[i] > 1000) ) {
			warnings << tr("Number of lamps") + QString(" = %1").arg(iNL[i])
			+ sep + tr("value is out of range");
		}
		if(sTL[i].length() > Vars().lineTL) {
			 warnings << tr("Type of lamps") + QString(" = %1").arg(sTL[i].length())
			 + sep + tr("line is too long");
		}	
		if( (dTLF[i] < 1) || (dTLF[i] > 9999999) ) {
			warnings << tr("Total luminous flux of lamps") + QString(" = %1").arg(dTLF[i])
			+ sep + tr("value is out of range");
		}	
		if(sCA[i].length() > Vars().lineCA) {
			warnings << tr("Color temperature of lamps") + QString(" = %1").arg(sCA[i].length())
			+ sep + tr("line is too long");
		}	
		if(sCRG[i].length() > Vars().lineCRG) {
			warnings << tr("Color rendering index") + QString(" = %1").arg(sCRG[i].length())
			+ sep + tr("line is too long");
		}							
		if( (dWB[i] < 0.1) || (dWB[i] > 10000) ) {
			warnings << tr("Wattage including ballast") + QString(" = %1").arg(dWB[i])
			+ sep + tr("value is out of range");
		}					
	}
	for(int i=0; i<10; i++) {
		if( (dDR[i] < 0) || (dDR[i] > 10) ) {
			warnings << QString("k[%1]").arg(i+1) + QString(" = %1").arg(dDR[i])
			+ sep + tr("value is out of range");
		}			
	}
	for(int i=0; i<iMc; i++) {
		for(int j=0; j<iNg; j++) {
			if( (dLcd[i][j] < 0) || (dLcd[i][j] > 1000000) ) {
				error = tr("Luminous intensity distribution") + QString(" = %1").arg(dLcd[i][j])
				+ sep + tr("values are out of range");
				throw error;
			}				
		}
	}	
	
	bool underone = true;
	double avg = 0;
	for(int i=0; i<iMc; i++) {
		for(int j=0; j<iNg; j++) {
			avg += dLcd[i][j];
			if(dLcd[i][j] >= 1) {
				underone = false;
				break;
			}
		}			
	}
	if(underone) {
		avg /= (iMc * iNg);
		error = tr("Distribution of luminous intensity") + " (" + tr("avg") + QString(" = %1)").arg(avg)
		+ sep + tr("values are too low");		
		throw error;
	}	
	
	for(int i=1; i<iNc; i++) {
		if(dC[i-1] >= dC[i]) {
			throw tr("C-planes not sorted");
		}
	}
	for(int i=1; i<iNg; i++) {
		if(dG[i-1] >= dG[i]) {
			throw tr("Gamma-planes not sorted");
		}		
	}		
	if( (dG[0] < 0) || (dG[iNg-1] > 180) ){
		throw tr("Wrong Gamma-planes scheme");
	}
	
	int i=0;
	if( (iIsym == 0) && (iMc >= 4) ) {
		for(i=0; i<iNc; i++) {
			if(dC[i] == 90) {
				break;
			}
		}	
		if(i == iNc) {
			throw tr("Missing C90-plane");
		}
		for(i=0; i<iNc; i++) {
			if(dC[i] == 180) {
				break;
			}
		}	
		if(i == iNc) {
			throw tr("Missing C180-plane");		
		}
		for(i=0; i<iNc; i++) {
			if(dC[i] == 270) {
				break;
			}
		}	
		if(i == iNc) {
			throw tr("Missing C270-plane");		
		}		
	}
	else if( (iIsym == 1) && (iMc == 1) ) {	
		//
	}	
	else if( (iIsym == 2) && (iMc >= 3) ) {	
		for(i=0; i<iNc; i++) {
			if(dC[i] == 90) {
				break;
			}
		}	
		if(i == iNc) {
			throw tr("Missing C90-plane");
		}
	}	
	else if( (iIsym == 3) && (iMc >= 3) ) {	
		for(i=0; i<iNc; i++) {
			if(dC[i] == 180) {
				break;
			}
		}	
		if(i == iNc) {
			throw tr("Missing C180-plane");
		}
	}		
	else if( (iIsym == 4) && (iMc >= 2) ) {	
		//
	}	
	else {
		throw tr("Wrong C-planes scheme");		
	}


	return warnings;
}
