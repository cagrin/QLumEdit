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

#include "newdialog.h"
#include "vars.h"

NewDialog::NewDialog(QWidget *parent) : QDialog(parent)
{
    setMinimumWidth(320);	
	
    setWindowTitle(tr("New file"));
	Vars().newFile = "";
	
	pushButtonA = new QPushButton(tr("Road luminaire"));
	pushButtonB = new QPushButton(tr("Projector"));
	pushButtonC = new QPushButton(tr("Fluorescent luminaire"));
	pushButtonD = new QPushButton(tr("Floor standing uplight"));			
	
    connect(pushButtonA, SIGNAL(clicked()), this, SLOT(setA()));		
    connect(pushButtonB, SIGNAL(clicked()), this, SLOT(setB()));			
    connect(pushButtonC, SIGNAL(clicked()), this, SLOT(setC()));		
    connect(pushButtonD, SIGNAL(clicked()), this, SLOT(setD()));	
    
    QWidget *examples = new QWidget(this);
    
    QVBoxLayout *VLayout1 = new QVBoxLayout;
	VLayout1->addWidget(pushButtonA);
	VLayout1->addWidget(pushButtonB);
	VLayout1->addWidget(pushButtonC);
	VLayout1->addWidget(pushButtonD);								
    VLayout1->addStretch(1);	       
    examples->setLayout(VLayout1);	    
    
    
    
    QWidget *templates = new QWidget(this);    
    
    pushTemplate = new QPushButton(tr("Create"));
    
    connect(pushTemplate, SIGNAL(clicked()), this, SLOT(setTemplate()));	    

    ComboBox1 = new QComboBox;
    QLabel *Label1 = new QLabel(tr("Type indicator") + ":");
    ComboBox1->addItem(tr("with point source"));
    ComboBox1->addItem(tr("linear luminaire"));   
    
    ComboBox2 = new QComboBox;
    QLabel *Label2 = new QLabel(tr("Symmetry indicator") + ":");
    ComboBox2->addItem(tr("no symmetry"));
    ComboBox2->addItem(tr("symmetry about the vertical axis"));       
    ComboBox2->addItem(tr("symmetry to plane C0-C180")); 
    ComboBox2->addItem(tr("symmetry to plane C90-C270")); 
    ComboBox2->addItem(tr("symmetry to plane C0-C180 and to plane C90-C270"));             
    
    ComboBox3 = new QComboBox;
    QLabel *Label3 = new QLabel(tr("Type of output flux") + ":");
    ComboBox3->addItem(tr("downlighting"));
    ComboBox3->addItem(tr("downlighting with upward component"));      
    ComboBox3->addItem(tr("uplighting"));          
    
    QHBoxLayout *HLayout2 = new QHBoxLayout;
    HLayout2->addStretch(1);
	HLayout2->addWidget(pushTemplate);	    
    HLayout2->addStretch(1);    
    
    QVBoxLayout *VLayout2 = new QVBoxLayout;
	VLayout2->addWidget(Label1);								
	VLayout2->addWidget(ComboBox1);									
	VLayout2->addWidget(Label2);								
	VLayout2->addWidget(ComboBox2);			
	VLayout2->addWidget(Label3);								
	VLayout2->addWidget(ComboBox3);		
	VLayout2->addLayout(HLayout2);
    VLayout2->addStretch(1);	       
    templates->setLayout(VLayout2);	     
    
    
    
    QTabWidget *tabWidget = new QTabWidget;    
    tabWidget->addTab(templates, tr("Templates"));
    tabWidget->addTab(examples, tr("Examples"));  

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);     
    
}

void NewDialog::setA() {
	Vars().newFile = ":/templates/road_luminaire.ldt";
	QDialog::close();
}

void NewDialog::setB() {
	Vars().newFile = ":/templates/projector.ldt";
	QDialog::close();	
}

void NewDialog::setC() {
	Vars().newFile = ":/templates/fluorescent_luminaire.ldt";
	QDialog::close();
}

void NewDialog::setD() {
	Vars().newFile = ":/templates/floor_uplight.ldt";
	QDialog::close();
}

void NewDialog::setTemplate() {
	Vars().newFile = QString(":/templates/%1-%2-%3.ldt").arg(ComboBox1->currentIndex()).arg(ComboBox2->currentIndex()).arg(ComboBox3->currentIndex());
	QDialog::close();	
}
