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

#ifndef __EULUMDATWIDGET_H__
#define __EULUMDATWIDGET_H__

#include <QWidget>

#include "eulumdat.h"

class QTextEdit;
class QComboBox;
class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;
class QLabel;
class QPushButton;

class EulumdatWidget : public QWidget
{
    Q_OBJECT

public:
	EulumdatWidget(Eulumdat &ldt, QWidget *parent = 0);
    
	QLineEdit *LineEdit1;
	QComboBox *ComboBox2;	
	QLineEdit *LineEdit8;
	QLineEdit *LineEdit9;
	QLineEdit *LineEdit10;
	QLineEdit *LineEdit11;
	QLineEdit *LineEdit12;		
	
	Eulumdat * pldt;
	
	QLabel *Label2;
	QLabel *Label3;						
	QLabel *Label4;
	QLabel *Label5;
	QLabel *Label6;
	QLabel *Label7;
	
	void setValues();
	
	QPushButton *optimizePushButton;
private slots:
	void optimize();

};

#endif // __EULUMDATWIDGET_H__
