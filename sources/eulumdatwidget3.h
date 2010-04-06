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

#ifndef __EULUMDATWIDGET3_H__
#define __EULUMDATWIDGET3_H__

#include <QWidget>

#include "eulumdat.h"

class QComboBox;
class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;
class QPushButton;
class QTabWidget;
class QLabel;

class EulumdatWidget3 : public QWidget
{
    Q_OBJECT

public:
    EulumdatWidget3(Eulumdat &ldt, QWidget *parent = 0);
    
	Eulumdat * pldt;    
    
	QLabel *Label1;
	QPushButton *addButton, *removeButton;
	QSpinBox **SpinBox2;
	QLineEdit **LineEdit3;
	QDoubleSpinBox **DoubleSpinBox4;
	QLineEdit **LineEdit5;
	QLineEdit **LineEdit6;
	QDoubleSpinBox **DoubleSpinBox7;

	QWidget **GroupBox;
	QTabWidget * tabWidget;	
	
private slots:
	void addSet(void);
	void removeSet(void);
									
};

#endif // __EULUMDATWIDGET3_H__
