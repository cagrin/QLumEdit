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

#ifndef __MAINTABWIDGET_H__
#define __MAINTABWIDGET_H__

#include <QWidget>

#include "eulumdat.h"
#include "eulumdatwidget.h"
#include "eulumdatwidget2.h"
#include "eulumdatwidget3.h"
#include "eulumdatwidget4.h"
#include "eulumdattablewidget.h"
#include "renderwidget.h"

class QTabWidget;

class MainTabWidget : public QWidget
{
    Q_OBJECT

public:
    MainTabWidget(Eulumdat &ldt, QWidget *parent = 0);
    
    QTabWidget * tabWidget;
	EulumdatWidget * general;
	EulumdatWidget2 * luminaire;
	EulumdatWidget3 * lamps;
	EulumdatWidget4 * UFtable;
	EulumdatTableWidget * table;
	RenderWidget * render;	
};

#endif // __MAINTABWIDGET_H__
