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

#include "eulumdatwidget3.h"
#include "vars.h"

EulumdatWidget3::EulumdatWidget3(Eulumdat &ldt, QWidget *parent) : QWidget(parent)
{
	pldt = &ldt;	
	
    Label1 = new QLabel; 
    
    Label1->setText(tr("Number of standard sets of lamps") + QString(" = %1").arg(ldt.iN));
    
	addButton = new QPushButton(tr("Add"));	
    connect(addButton, SIGNAL(clicked()), this, SLOT(addSet()));	
    
	removeButton = new QPushButton(tr("Remove"));	
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeSet()));	    
    
   
    QHBoxLayout *HLayout1 = new QHBoxLayout; 
	HLayout1->addWidget(Label1);  
	HLayout1->addStretch(1);		
	HLayout1->addWidget(addButton); 
	HLayout1->addWidget(removeButton); 	
	
	
	QString s1 = tr("Number of lamps");
	QString s2 = tr("Type of lamps");
	QString s3 = tr("Total luminous flux of lamps");
	QString s4 = tr("Color temperature of lamps");
	QString s5 = tr("Color rendering index");
	QString s6 = tr("Wattage including ballast");
	
	int max = 29;
	
	QLabel **Label2;
	Label2 = new QLabel*[max];
	QLabel **Label3;
	Label3 = new QLabel*[max];
	QLabel **Label4;
	Label4 = new QLabel*[max];
	QLabel **Label5;
	Label5 = new QLabel*[max];
	QLabel **Label6;
	Label6 = new QLabel*[max];
	QLabel **Label7;
	Label7 = new QLabel*[max];					
	
	QHBoxLayout **HLayout2;
	HLayout2 = new QHBoxLayout*[max];
	QHBoxLayout **HLayout3;
	HLayout3 = new QHBoxLayout*[max];
	QHBoxLayout **HLayout4;
	HLayout4 = new QHBoxLayout*[max];
	QHBoxLayout **HLayout5;
	HLayout5 = new QHBoxLayout*[max];
	QHBoxLayout **HLayout6;
	HLayout6 = new QHBoxLayout*[max];
	QHBoxLayout **HLayout7;
	HLayout7 = new QHBoxLayout*[max];	
	
	QVBoxLayout **mainRadioLayout;
	mainRadioLayout = new QVBoxLayout*[max];
	
	SpinBox2 = new QSpinBox*[max];	
	LineEdit3 = new QLineEdit*[max];
	DoubleSpinBox4 = new QDoubleSpinBox*[max];
	LineEdit5 = new QLineEdit*[max];
	LineEdit6 = new QLineEdit*[max];
	DoubleSpinBox7 = new QDoubleSpinBox*[max];
	
	GroupBox = new QWidget*[max];

	for(int i=0; i<max; i++) {
		
		Label2[i] = new QLabel(s1);
		SpinBox2[i] = new QSpinBox;
		SpinBox2[i]->setRange(1,1000);

		Label3[i] = new QLabel(s2);		
		LineEdit3[i] = new QLineEdit;

		LineEdit3[i]->setMaxLength(Vars().lineTL);	
		Label4[i] = new QLabel(s3);
		DoubleSpinBox4[i] = new QDoubleSpinBox;
		DoubleSpinBox4[i]->setRange(1,9999999);
		DoubleSpinBox4[i]->setSuffix(" lm");
		DoubleSpinBox4[i]->setDecimals(0);

		Label5[i] = new QLabel(s4);	
		LineEdit5[i] = new QLineEdit;	

		LineEdit5[i]->setMaxLength(Vars().lineCA);	
		Label6[i] = new QLabel(s5);	
		LineEdit6[i] = new QLineEdit;	

		LineEdit6[i]->setMaxLength(Vars().lineCRG);				
		Label7[i] = new QLabel(s6);
		DoubleSpinBox7[i] = new QDoubleSpinBox;
		DoubleSpinBox7[i]->setRange(0.1,10000);
		DoubleSpinBox7[i]->setSuffix(" W");
		DoubleSpinBox7[i]->setDecimals(1);
		
		
	    HLayout2[i] = new QHBoxLayout; 
		HLayout2[i]->addWidget(Label2[i]);  
		HLayout2[i]->addWidget(SpinBox2[i]); 
		HLayout2[i]->addStretch(1);
		HLayout3[i] = new QHBoxLayout;
		HLayout3[i]->addWidget(Label3[i]); 		
		HLayout3[i]->addWidget(LineEdit3[i]); 
	    HLayout4[i] = new QHBoxLayout; 
		HLayout4[i]->addWidget(Label4[i]);  
		HLayout4[i]->addWidget(DoubleSpinBox4[i]);
		HLayout4[i]->addStretch(1); 
	    HLayout5[i] = new QHBoxLayout; 
		HLayout5[i]->addWidget(Label5[i]);  
		HLayout5[i]->addWidget(LineEdit5[i]); 	
	    HLayout6[i] = new QHBoxLayout; 
		HLayout6[i]->addWidget(Label6[i]);  
		HLayout6[i]->addWidget(LineEdit6[i]); 	
	    HLayout7[i] = new QHBoxLayout; 
		HLayout7[i]->addWidget(Label7[i]);  
		HLayout7[i]->addWidget(DoubleSpinBox7[i]);
		HLayout7[i]->addStretch(1); 						
		
		mainRadioLayout[i] = new QVBoxLayout;
		mainRadioLayout[i]->addLayout(HLayout2[i]);
		mainRadioLayout[i]->addLayout(HLayout3[i]);
		mainRadioLayout[i]->addLayout(HLayout4[i]);
		mainRadioLayout[i]->addLayout(HLayout5[i]);		
		mainRadioLayout[i]->addLayout(HLayout6[i]);
		mainRadioLayout[i]->addLayout(HLayout7[i]);			
		
		GroupBox[i] = new QWidget;
		GroupBox[i]->setLayout(mainRadioLayout[i]);				
	}
	
	for(int i=0; i<ldt.iN; i++) {
		SpinBox2[i]->setValue(ldt.iNL[i]);	
		LineEdit3[i]->setText(ldt.sTL[i]);
		DoubleSpinBox4[i]->setValue(ldt.dTLF[i]);	
		LineEdit5[i]->setText(ldt.sCA[i]);
		LineEdit6[i]->setText(ldt.sCRG[i]);
		DoubleSpinBox7[i]->setValue(ldt.dWB[i]);			
	}

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(HLayout1);
	
	tabWidget = new QTabWidget;
	for(int i=0; i<ldt.iN; i++) {
		tabWidget->addTab(GroupBox[i], tr("Set") + QString(" %1").arg(i+1));
	}
	mainLayout->addWidget(tabWidget);
    mainLayout->addStretch(1);	       
    setLayout(mainLayout);		
}

void EulumdatWidget3::addSet(void) {
	
	int n = pldt->iN;
	
	if(n < 20) {
		tabWidget->addTab(GroupBox[n], tr("Set") + QString(" %1").arg(n+1));
		pldt->iN++;	
	    Label1->setText(tr("Number of standard sets of lamps") + QString(" = %1").arg(pldt->iN));		
		Vars().mainWindow->documentWasModified();
	}	
}
void EulumdatWidget3::removeSet(void) {
	
	int n = pldt->iN;
	
	if(n > 1) {
		tabWidget->removeTab(n-1);
		pldt->iN--;	
	    Label1->setText(tr("Number of standard sets of lamps") + QString(" = %1").arg(pldt->iN));		
		Vars().mainWindow->documentWasModified();		
	}		
}
