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

#include "eulumdatwidget4.h"

EulumdatWidget4::EulumdatWidget4(Eulumdat &ldt, QWidget *parent) : QWidget(parent)
{
	pldt = &ldt;	
	
    QLabel *Label13 = new QLabel(tr("k = 0.60")); 
    QLabel *Label14 = new QLabel(tr("k = 0.80"));
    QLabel *Label15 = new QLabel(tr("k = 1.00"));
    QLabel *Label16 = new QLabel(tr("k = 1.25"));
    QLabel *Label17 = new QLabel(tr("k = 1.50"));
    QLabel *Label18 = new QLabel(tr("k = 2.00"));
    QLabel *Label19 = new QLabel(tr("k = 2.50"));
    QLabel *Label20 = new QLabel(tr("k = 3.00"));
    QLabel *Label21 = new QLabel(tr("k = 4.00"));
    QLabel *Label22 = new QLabel(tr("k = 5.00"));
    
    DoubleSpinBox13 = new QDoubleSpinBox;
    DoubleSpinBox14 = new QDoubleSpinBox;
    DoubleSpinBox15 = new QDoubleSpinBox;
    DoubleSpinBox16 = new QDoubleSpinBox;
    DoubleSpinBox17 = new QDoubleSpinBox;
	DoubleSpinBox18 = new QDoubleSpinBox;
	DoubleSpinBox19 = new QDoubleSpinBox;
	DoubleSpinBox20 = new QDoubleSpinBox;
	DoubleSpinBox21 = new QDoubleSpinBox;
	DoubleSpinBox22 = new QDoubleSpinBox;

	DoubleSpinBox13->setRange(0,1);
	DoubleSpinBox14->setRange(0,1);
	DoubleSpinBox15->setRange(0,1);
	DoubleSpinBox16->setRange(0,1);
	DoubleSpinBox17->setRange(0,1);
	DoubleSpinBox18->setRange(0,1);
	DoubleSpinBox19->setRange(0,1);
	DoubleSpinBox20->setRange(0,1);
	DoubleSpinBox21->setRange(0,1);
	DoubleSpinBox22->setRange(0,1);
	
	DoubleSpinBox13->setDecimals(3);
	DoubleSpinBox14->setDecimals(3);
	DoubleSpinBox15->setDecimals(3);
	DoubleSpinBox16->setDecimals(3);
	DoubleSpinBox17->setDecimals(3);
	DoubleSpinBox18->setDecimals(3);
	DoubleSpinBox19->setDecimals(3);
	DoubleSpinBox20->setDecimals(3);
	DoubleSpinBox21->setDecimals(3);
	DoubleSpinBox22->setDecimals(3);

	DoubleSpinBox13->setSingleStep(0.01);
	DoubleSpinBox14->setSingleStep(0.01);
	DoubleSpinBox15->setSingleStep(0.01);
	DoubleSpinBox16->setSingleStep(0.01);
	DoubleSpinBox17->setSingleStep(0.01);
	DoubleSpinBox18->setSingleStep(0.01);
	DoubleSpinBox19->setSingleStep(0.01);
	DoubleSpinBox20->setSingleStep(0.01);
	DoubleSpinBox21->setSingleStep(0.01);
	DoubleSpinBox22->setSingleStep(0.01);	

	DoubleSpinBox13->setValue(ldt.dDR[0]);
	DoubleSpinBox14->setValue(ldt.dDR[1]);
	DoubleSpinBox15->setValue(ldt.dDR[2]);
	DoubleSpinBox16->setValue(ldt.dDR[3]);
	DoubleSpinBox17->setValue(ldt.dDR[4]);
	DoubleSpinBox18->setValue(ldt.dDR[5]);
	DoubleSpinBox19->setValue(ldt.dDR[6]);
	DoubleSpinBox20->setValue(ldt.dDR[7]);
	DoubleSpinBox21->setValue(ldt.dDR[8]);
	DoubleSpinBox22->setValue(ldt.dDR[9]);
      
	QHBoxLayout *HLayout13 = new QHBoxLayout; 
	HLayout13->addWidget(Label13);          			
	HLayout13->addWidget(DoubleSpinBox13);	    
	HLayout13->addStretch(1);
	QHBoxLayout *HLayout14 = new QHBoxLayout; 
	HLayout14->addWidget(Label14);          			
	HLayout14->addWidget(DoubleSpinBox14);	    
	HLayout14->addStretch(1);
	QHBoxLayout *HLayout15 = new QHBoxLayout; 
	HLayout15->addWidget(Label15);          			
	HLayout15->addWidget(DoubleSpinBox15);	    
	HLayout15->addStretch(1);
	QHBoxLayout *HLayout16 = new QHBoxLayout; 
	HLayout16->addWidget(Label16);          			
	HLayout16->addWidget(DoubleSpinBox16);	    
	HLayout16->addStretch(1);
	QHBoxLayout *HLayout17 = new QHBoxLayout; 
	HLayout17->addWidget(Label17);          			
	HLayout17->addWidget(DoubleSpinBox17);	    
	HLayout17->addStretch(1);
	QHBoxLayout *HLayout18 = new QHBoxLayout; 
	HLayout18->addWidget(Label18);          			
	HLayout18->addWidget(DoubleSpinBox18);	    
	HLayout18->addStretch(1);
	QHBoxLayout *HLayout19 = new QHBoxLayout; 
	HLayout19->addWidget(Label19);          			
	HLayout19->addWidget(DoubleSpinBox19);	    
	HLayout19->addStretch(1);
	QHBoxLayout *HLayout20 = new QHBoxLayout; 
	HLayout20->addWidget(Label20);          			
	HLayout20->addWidget(DoubleSpinBox20);	    
	HLayout20->addStretch(1);
	QHBoxLayout *HLayout21 = new QHBoxLayout; 
	HLayout21->addWidget(Label21);          			
	HLayout21->addWidget(DoubleSpinBox21);	    
	HLayout21->addStretch(1);
	QHBoxLayout *HLayout22 = new QHBoxLayout; 
	HLayout22->addWidget(Label22);          			
	HLayout22->addWidget(DoubleSpinBox22);	    
	HLayout22->addStretch(1);
	
	computePushButton = new QPushButton(tr("Calculate"));
	connect(computePushButton, SIGNAL(clicked()), this, SLOT(compute()));	
	
	QHBoxLayout *HLayout23 = new QHBoxLayout; 
	HLayout23->addWidget(computePushButton);          			  
	HLayout23->addStretch(1);	
	
    QLabel *Label12 = new QLabel(tr("Direct ratios") + ":");
	QHBoxLayout *HLayout12 = new QHBoxLayout; 
	HLayout12->addWidget(Label12);   	
	
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(HLayout12);
	mainLayout->addLayout(HLayout13);
	mainLayout->addLayout(HLayout14);
	mainLayout->addLayout(HLayout15);
	mainLayout->addLayout(HLayout16);
	mainLayout->addLayout(HLayout17);
	mainLayout->addLayout(HLayout18);
	mainLayout->addLayout(HLayout19);
	mainLayout->addLayout(HLayout20);
	mainLayout->addLayout(HLayout21);
	mainLayout->addLayout(HLayout22);
	mainLayout->addLayout(HLayout23);	
    mainLayout->addStretch(1);	       
    setLayout(mainLayout);	
}

void EulumdatWidget4::compute() {
	
	QStringList items;
    items << tr("1.00") << tr("1.25") << tr("1.50");

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Calculate"),
										    tr("Spacing to height ratio") +  
                                         "<br>SHR<sub>nom</sub>:", items, 0, false, &ok);
               
    if ( !(ok && !item.isEmpty()) ) return;
    	
	double* DFL = pldt->directRadios(item);
	for(int i=0; i<10; i++) {
		if(DFL[i] > 1) {
			QMessageBox::warning(this, "QLumEdit",
			QString("k[%1]").arg(i+1) + QString(" = %1").arg(DFL[i])
			+ " -> " + tr("value is out of range") + "<br><br>"
			+ tr("Check distribution of luminous intesity to be in cd / 1000lm")
			);			
			return;
		}	
	}
	
	for(int i=0; i<10; i++) {
		pldt->dDR[i] = DFL[i];
	}
	
	DoubleSpinBox13->setValue(DFL[0]);
	DoubleSpinBox14->setValue(DFL[1]);
	DoubleSpinBox15->setValue(DFL[2]);
	DoubleSpinBox16->setValue(DFL[3]);
	DoubleSpinBox17->setValue(DFL[4]);
	DoubleSpinBox18->setValue(DFL[5]);
	DoubleSpinBox19->setValue(DFL[6]);
	DoubleSpinBox20->setValue(DFL[7]);
	DoubleSpinBox21->setValue(DFL[8]);
	DoubleSpinBox22->setValue(DFL[9]);
	
}
