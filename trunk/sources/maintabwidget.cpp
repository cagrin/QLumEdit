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

#include "maintabwidget.h"

MainTabWidget::MainTabWidget(Eulumdat &ldt, QWidget *parent) : QWidget(parent)
{
	tabWidget = new QTabWidget;
	general = new EulumdatWidget(ldt, this);
	luminaire = new EulumdatWidget2(ldt, this);
	lamps = new EulumdatWidget3(ldt, this);
	UFtable = new EulumdatWidget4(ldt, this);
	table = new EulumdatTableWidget(ldt, this);	
	render = new RenderWidget(ldt, this);
	
    tabWidget->addTab(general, tr("General"));
    tabWidget->addTab(luminaire, tr("Luminaire"));    
	tabWidget->addTab(lamps, tr("Lamps"));
	tabWidget->addTab(UFtable, tr("Utilization factors"));	
	tabWidget->addTab(table, tr("Luminous intensity"));	
	tabWidget->addTab(render, tr("Diagram"));	
	   
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);  
}

