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

#ifndef __EULUMDATTABLEWIDGET_H__
#define __EULUMDATTABLEWIDGET_H__

#include <QWidget>

#include "eulumdat.h"

class QDoubleSpinBox;
class QTableWidget;
class QPushButton;

class EulumdatTableWidget : public QWidget
{
    Q_OBJECT

public:
    EulumdatTableWidget(Eulumdat &ldt, QWidget *parent = 0);
    void setTable();
    
    QTableWidget *tableWidget;
 							
	QPushButton *importPushButton;
	QPushButton *exportPushButton;	
	
	Eulumdat * pldt;

public slots:
	void importFromClipboard();
	void exportToClipboard();		
private slots:
	void tryToImportFromClipboard();	
};

#endif // __EULUMDATTABLEWIDGET_H__
