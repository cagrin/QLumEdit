/*
* This file is part of QLumEdit, an ultimate Eulumdat file editor
* Copyright (C) 2007-2012 Krzysztof Strugiñski
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
* Contact e-mail: Krzysztof Strugiñski <cagrin@gmail.com>
*   Program page: http://sourceforge.net/projects/qlumedit
*/

#include <QtGui>
#include <math.h>

#include "eulumdat.h"

#include "vars.h"

double* Eulumdat::directRadios(QString SHR) {
	double a, b, c, d;
	QVector<double> e, f, g, h; //e[10], f[10], g[10], h[10];
	double * DFL = new double[10];

	if(SHR == tr("1.00")) {	
		e << 943 << 752 << 636 << 510 << 429 << 354 << 286 << 258 << 236 << 231;
		f << -317 << -33 << 121 << 238 << 275 << 248 << 190 << 118 << -6 << -99;
		g << 145 << 81 << 88 << 131 << 202 << 350 << 470 << 563 << 684 << 748;
		h << -27 << -16 << -15 << -16 << -18 << -15 << -3 << 16 << 60 << 107;
	}
	else if(SHR == tr("1.25")) {	
		e << 1013 << 893 << 692 << 569 << 498 << 355 << 317 << 268 << 242 << 234;
		f << -338 << -112 << 151 << 256 << 274 << 284 << 184 << 132 << 5 << -91;
		g << 144 << 102 << 65 << 119 << 197 << 337 << 471 << 563 << 685 << 751;
		h << -26 << -19 << -11 << -14 << -17 << -13 << -2 << 16 << 61 << 108;
	}
	//if(SHR == tr("1.50")) {	
	else {
		e << 1070 << 934 << 774 << 591 << 435 << 353 << 293 << 279 << 236 << 231;
		f << -340 << -45 << 168 << 342 << 420 << 334 << 254 << 161 << -26 << -76;
		g << 133 << 58 << 38 << 62 << 141 << 316 << 447 << 541 << 679 << 746;
		h << -24 << -11 << -6 << -4 << -9 << -9 << 2 << 20 << 62 << 109;
	}		


	a = downwardDFL(41.4);
	b = downwardDFL(60);
	c = downwardDFL(75.5);
	d = downwardDFL(90);	
	
	double t = 0;
	
	for(int i=0; i<10; i++) {
		t = a*e[i]+b*f[i]+c*g[i]+d*h[i];
		t /= 100000;
		DFL[i] = t;
	}
	
	return DFL;
}

double Eulumdat::downwardDFL(double arc) {
	double d = 0;
	int i;
	
	if(iIsym == 0) {
		for(i=1; i<iMc; i++) {
			d += (dC[i]-dC[i-1])*downwardForPlaneDFL(i-1, arc);
		}	
		d += (360-dC[iMc-1])*downwardForPlaneDFL(iMc-1, arc);
		d /= 360;
	}
	else if(iIsym == 1) {
		d += downwardForPlaneDFL(0, arc);			
	}
	else if(iIsym == 2) {
		for(i=1; i<iMc; i++) {
			d += 2*(dC[i]-dC[i-1])*downwardForPlaneDFL(i-1, arc);
		}	
		d += 2*(180-dC[iMc-1])*downwardForPlaneDFL(iMc-1, arc);
		d /= 360;		
	}
	else if(iIsym == 3) {
		int j;
		for(j=1; j<iNc; j++) {
			if(dC[j] == 90) {
				break;
			}
		}
		for(i=1, j++; i<iMc; i++, j++) {
			d += 2*(dC[j]-dC[j-1])*downwardForPlaneDFL(i-1, arc);
		}	
		d += 2*(270-dC[--j])*downwardForPlaneDFL(iMc-1, arc);
		d /= 360;		
	}
	else if(iIsym == 4) {
		for(i=1; i<iMc; i++) {
			d += 4*(dC[i]-dC[i-1])*downwardForPlaneDFL(i-1, arc);
		}	
		d += 4*(90-dC[iMc-1])*downwardForPlaneDFL(iMc-1, arc);
		d /= 360;
	}
	else {

	}
	return 100*d/output();
}

double Eulumdat::downwardForPlaneDFL(int c, double arc) {

	double omega;
	double avg;
	double sum = 0;
	
	for(int i=1; i<iNg; i++) {
		if(dG[i] > arc) {
			break;				
		}
		omega = 2*M_PI * (cos(dG[i-1] * M_PI / 180.0) - cos(dG[i] * M_PI / 180.0));
		avg = (dLcd[c][i-1] + dLcd[c][i]) / 2;
		sum += (omega * avg);
	}
	return sum/10;
}




double Eulumdat::downward() {
	double d = 0;
	int i;
	
	if(iIsym == 0) {
		for(i=1; i<iMc; i++) {
			d += (dC[i]-dC[i-1])*downwardForPlane(i-1);
		}	
		d += (360-dC[iMc-1])*downwardForPlane(iMc-1);
		d /= 360;
	}
	else if(iIsym == 1) {
		d += downwardForPlane(0);			
	}
	else if(iIsym == 2) {
		for(i=1; i<iMc; i++) {
			d += 2*(dC[i]-dC[i-1])*downwardForPlane(i-1);
		}	
		d += 2*(180-dC[iMc-1])*downwardForPlane(iMc-1);
		d /= 360;		
	}
	else if(iIsym == 3) {
		int j;
		for(j=1; j<iNc; j++) {
			if(dC[j] == 90) {
				break;
			}
		}
		for(i=1, j++; i<iMc; i++, j++) {
			d += 2*(dC[j]-dC[j-1])*downwardForPlane(i-1);
		}	
		d += 2*(270-dC[--j])*downwardForPlane(iMc-1);
		d /= 360;		
	}
	else if(iIsym == 4) {
		for(i=1; i<iMc; i++) {
			d += 4*(dC[i]-dC[i-1])*downwardForPlane(i-1);
		}	
		d += 4*(90-dC[iMc-1])*downwardForPlane(iMc-1);
		d /= 360;
	}
	else {

	}
	return 100*d/output();
}

double Eulumdat::downwardForPlane(int c) {

	double omega;
	double avg;
	double sum = 0;
	
	for(int i=1; i<iNg; i++) {
		if(dG[i] > 90) {
			break;				
		}
		omega = 2*M_PI * (cos(dG[i-1] * M_PI / 180.0) - cos(dG[i] * M_PI / 180.0));
		avg = (dLcd[c][i-1] + dLcd[c][i]) / 2;
		sum += (omega * avg);
	}
	return sum/10;
}

double Eulumdat::output() {
	double d = 0;
	int i;
	
	if(iIsym == 0) {
		for(i=1; i<iMc; i++) {
			d += (dC[i]-dC[i-1])*outputForPlane(i-1);
		}	
		d += (360-dC[iMc-1])*outputForPlane(iMc-1);		
		d /= 360;
	}
	else if(iIsym == 1) {
		d += outputForPlane(0);			
	}
	else if(iIsym == 2) {
		for(i=1; i<iMc; i++) {
			d += 2*(dC[i]-dC[i-1])*outputForPlane(i-1);
		}	
		d += 2*(180-dC[iMc-1])*outputForPlane(iMc-1);	
		d /= 360;		
	}
	else if(iIsym == 3) {
		int j;
		for(j=1; j<iNc; j++) {
			if(dC[j] == 90) {
				break;
			}
		}
		for(i=1, j++; i<iMc; i++, j++) {
			d += 2*(dC[j]-dC[j-1])*outputForPlane(i-1);
		}	
		d += 2*(270-dC[--j])*outputForPlane(iMc-1);	
		d /= 360;		
	}
	else if(iIsym == 4) {
		for(i=1; i<iMc; i++) {
			d += 4*(dC[i]-dC[i-1])*outputForPlane(i-1);
		}	
		d += 4*(90-dC[iMc-1])*outputForPlane(iMc-1);	
		d /= 360;
	}
	else {

	}
	return d;
}

double Eulumdat::outputForPlane(int c) {

	double omega;
	double avg;
	double sum = 0;
	
	for(int i=1; i<iNg; i++) {
		omega = 2*M_PI * (cos(dG[i-1] * M_PI / 180.0) - cos(dG[i] * M_PI / 180.0));
		avg = (dLcd[c][i-1] + dLcd[c][i]) / 2;
		sum += (omega * avg);
	}
	return sum/10;
}
