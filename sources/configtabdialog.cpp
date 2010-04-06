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

#include "configtabdialog.h"
#include "vars.h"

TabDialog::TabDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Options"));
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);   
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    

	QLabel *label1 = new QLabel(tr("Language selection"));
	QLabel *label2 = new QLabel("(" + tr("requires application restart") + ")");
	comboBox1 = new QComboBox; 
	comboBox1->setMinimumContentsLength(4);
	comboBox1->addItem("en");
    comboBox1->addItem("pl");
    
    QSettings settings("Cagrinlabs", "QLumEdit");
    QString language = settings.value("lang", QString("")).toString();    
    comboBox1->setCurrentIndex(comboBox1->findText(language));    
     
    QGroupBox *transBox = new QGroupBox(tr("User interface")); 
	QHBoxLayout *HLayout1 = new QHBoxLayout; 
	HLayout1->addWidget(label1);          			
	HLayout1->addWidget(comboBox1);	
    HLayout1->addStretch(1);	
    QVBoxLayout *VLayout1 = new QVBoxLayout;
    VLayout1->addLayout(HLayout1);
    VLayout1->addWidget(label2);
    VLayout1->addStretch(1);    
    transBox->setLayout(VLayout1); 

    radio1 = new QRadioButton(tr("Eulumdat/1 restricted (78/24 chars)"));
    radio2 = new QRadioButton(tr("limit to 255 chars"));

    QString restricted = settings.value("restricted", QString("false")).toString();
    if(restricted == "true") {
	    radio1->setChecked(true);    	
   	}
   	else {
	   	radio2->setChecked(true);
  	}

    QGroupBox *groupBox = new QGroupBox(tr("Text line length"));
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(radio1);
    vbox->addWidget(radio2);
    vbox->addStretch(1);
    groupBox->setLayout(vbox); 
    
   
    checkBox = new QCheckBox(tr("Fill curves of luminous intesity"));    
    QString filldiagram = settings.value("filldiagram", QString("true")).toString();
    if(filldiagram == "true") {
	    checkBox->setChecked(true);    	
   	}
   	else {
	   	checkBox->setChecked(false);
  	}    
  	
    legendBox = new QCheckBox(tr("Draw legend"));    
    QString drawlegend = settings.value("drawlegend", QString("true")).toString();
    if(drawlegend == "true") {
	    legendBox->setChecked(true);    	
   	}
   	else {
	   	legendBox->setChecked(false);
  	}     	
  	
  	
    QGroupBox *drawBox = new QGroupBox(tr("Diagram drawing"));
    QVBoxLayout *vbox2 = new QVBoxLayout;
    vbox2->addWidget(checkBox);
    vbox2->addWidget(legendBox);
    vbox2->addStretch(1);
    drawBox->setLayout(vbox2);   	
  	
  	

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(transBox);	
	mainLayout->addWidget(groupBox);		
	mainLayout->addWidget(drawBox);	
    mainLayout->addWidget(buttonBox);		       
    setLayout(mainLayout);
}

void TabDialog::accept()
{
	QSettings settings("Cagrinlabs", "QLumEdit");
	settings.setValue("lang", comboBox1->currentText());
	
	if(radio1->isChecked()) {
		settings.setValue("restricted", "true");			
	}
	else {
		settings.setValue("restricted", "false");					
	}
	
	if(checkBox->isChecked()) {
		settings.setValue("filldiagram", "true");	
    	Vars().fill = true;				
	}
	else {
		settings.setValue("filldiagram", "false");	
		Vars().fill = false;				
	}	
	
	if(legendBox->isChecked()) {
		settings.setValue("drawlegend", "true");	
    	Vars().legend = true;				
	}
	else {
		settings.setValue("drawlegend", "false");	
		Vars().legend = false;				
	}		
   	
	
	Vars().mainWindow->setValidatorOptions();
	
    QDialog::accept();

}
