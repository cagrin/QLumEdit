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

#include "eulumdatwidget.h"
#include "vars.h"

EulumdatWidget::EulumdatWidget(Eulumdat &ldt, QWidget *parent) : QWidget(parent)
{   
	pldt = &ldt;

    QLabel *Label1 = new QLabel(tr("Identification"));

	Label2 = new QLabel;
    Label3 = new QLabel;
    Label4 = new QLabel;
    Label5 = new QLabel;
    Label6 = new QLabel;
    Label7 = new QLabel;

    QLabel *Label8 = new QLabel(tr("Measurement report number"));   
    QLabel *Label9 = new QLabel(tr("Luminaire name"));
    QLabel *Label10 = new QLabel(tr("Luminaire number"));
    QLabel *Label11 = new QLabel(tr("File name"));
    QLabel *Label12 = new QLabel(tr("Date/user"));               
    
	LineEdit1 = new QLineEdit(ldt.sIden);  
	
	ComboBox2 = new QComboBox;   	
    Label2->setText(tr("Type indicator"));
    ComboBox2->addItem(tr("with point source") + "  ");
    ComboBox2->addItem(tr("linear luminaire") + "  ");    
    if(ldt.iItyp != 2) ComboBox2->setCurrentIndex(0);    	
   	else ComboBox2->setCurrentIndex(1);   		
	
	LineEdit8 = new QLineEdit(ldt.sMrn);
	LineEdit9 = new QLineEdit(ldt.sLnam);
	LineEdit10 = new QLineEdit(ldt.sLnum);
	LineEdit11 = new QLineEdit(ldt.sFnam);
	LineEdit12 = new QLineEdit(ldt.sDate);
	
	LineEdit1->setMaxLength(Vars().line);
	LineEdit8->setMaxLength(Vars().line);
	LineEdit9->setMaxLength(Vars().line);
	LineEdit10->setMaxLength(Vars().line);
	LineEdit11->setMaxLength(Vars().lineFNam);
	LineEdit12->setMaxLength(Vars().line);					
    
	QHBoxLayout *HLayout1 = new QHBoxLayout; 
	HLayout1->addWidget(Label1);          			
	HLayout1->addWidget(LineEdit1);	
	QHBoxLayout *HLayout2 = new QHBoxLayout; 
	HLayout2->addWidget(Label2);          			
	HLayout2->addWidget(ComboBox2);    	
    HLayout2->addStretch(1);		
	QHBoxLayout *HLayout3 = new QHBoxLayout; 
	HLayout3->addWidget(Label3);          				
	QHBoxLayout *HLayout4 = new QHBoxLayout; 
	HLayout4->addWidget(Label4);
	QHBoxLayout *HLayout5 = new QHBoxLayout; 
	HLayout5->addWidget(Label5);
	QHBoxLayout *HLayout6 = new QHBoxLayout; 
	HLayout6->addWidget(Label6);
	QHBoxLayout *HLayout7 = new QHBoxLayout; 
	HLayout7->addWidget(Label7);
	QHBoxLayout *HLayout8 = new QHBoxLayout; 
	HLayout8->addWidget(Label8);
	HLayout8->addWidget(LineEdit8);
	QHBoxLayout *HLayout9 = new QHBoxLayout; 
	HLayout9->addWidget(Label9);
	HLayout9->addWidget(LineEdit9);
	QHBoxLayout *HLayout10 = new QHBoxLayout; 
	HLayout10->addWidget(Label10);
	HLayout10->addWidget(LineEdit10);
	QHBoxLayout *HLayout11 = new QHBoxLayout; 
	HLayout11->addWidget(Label11);
	HLayout11->addWidget(LineEdit11);
	QHBoxLayout *HLayout12 = new QHBoxLayout; 
	HLayout12->addWidget(Label12);
	HLayout12->addWidget(LineEdit12);
	
	optimizePushButton = new QPushButton(tr("Optimize data"));	
    connect(optimizePushButton, SIGNAL(clicked()), this, SLOT(optimize()));	
	QHBoxLayout *HLayout13 = new QHBoxLayout; 
	HLayout13->addWidget(optimizePushButton);
	HLayout13->addStretch(1);  	
	
    QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(HLayout1);
	mainLayout->addLayout(HLayout2);
	mainLayout->addLayout(HLayout3);
	mainLayout->addLayout(HLayout4);
	mainLayout->addLayout(HLayout5);
	mainLayout->addLayout(HLayout6);
	mainLayout->addLayout(HLayout7);
	mainLayout->addLayout(HLayout8);
	mainLayout->addLayout(HLayout9);
	mainLayout->addLayout(HLayout10);
	mainLayout->addLayout(HLayout11);
	mainLayout->addLayout(HLayout12);
	//mainLayout->addLayout(HLayout13);						
    mainLayout->addStretch(1);	       
    setLayout(mainLayout);
	
	setValues();
}

void EulumdatWidget::setValues() {
	
	QString begin = " = ", end = "";
	QLocale locale;
	QString text;
	/*
	QStringList indicator;
	indicator << tr("point source with symmetry about the vertical axis")
    	<< tr("linear luminaire")
   		<< tr("point source with any other symmetry");	
    Label2->setText(tr("Type indicator") + begin + indicator[pldt->iItyp-1] + end);	
    */

	
	QStringList symmetry;
	symmetry << tr("no symmetry")
		<< tr("symmetry about the vertical axis")
		<< tr("symmetry to plane C0-C180")
		<< tr("symmetry to plane C90-C270")
		<< tr("symmetry to plane C0-C180 and to plane C90-C270");	
		
    Label3->setText(tr("Symmetry indicator") + begin + symmetry[pldt->iIsym] + end);
    
    Label4->setText(tr("Number of C-planes")
    	+ begin + locale.toString(pldt->iNc) + end);

	text = tr("Distance between C-planes") + begin;
	if(pldt->dDc != 0.0) {
    	text += locale.toString(pldt->dDc) + QString("%1").arg((char)176) + end;
	}
	else {
    	text += tr("non-equidistantly") + end;
	}
    Label5->setText(text);
    	
    Label6->setText(tr("Number of luminous intensities in each C-plane")
    	+ begin + locale.toString(pldt->iNg) + end);
    	
	text = tr("Distance between luminous intensities per C-plane") + begin;
	if(pldt->dDg != 0.0) {
    	text += locale.toString(pldt->dDg) + QString("%1").arg((char)176) + end;
	}
	else {
    	text += tr("non-equidistantly") + end;
	}
    Label7->setText(text);    	
    
}

void EulumdatWidget::optimize() {
	Vars().mainWindow->central->table->exportToClipboard();
	Vars().mainWindow->central->table->importFromClipboard();
	Vars().mainWindow->statusBar()->showMessage(tr("Data optimized"), 5000);		
}


