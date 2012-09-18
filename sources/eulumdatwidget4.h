/*
* This file is part of QLumEdit, an ultimate Eulumdat file editor
* Copyright (C) 2007-2012 Krzysztof Strugiński
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
* Contact e-mail: Krzysztof Strugiński <cagrin@gmail.com>
*   Program page: http://sourceforge.net/projects/qlumedit
*/

#ifndef __EULUMDATWIDGET4_H__
#define __EULUMDATWIDGET4_H__

#include <QWidget>

#include "eulumdat.h"

class QLineEdit;
class QDoubleSpinBox;
class QPushButton;
class QComboBox;

class EulumdatWidget4 : public QWidget
{
    Q_OBJECT

public:
    EulumdatWidget4(Eulumdat &ldt, QWidget *parent = 0);
    
   	Eulumdat * pldt;    
    
	QDoubleSpinBox *DoubleSpinBox13;
	QDoubleSpinBox *DoubleSpinBox14;
	QDoubleSpinBox *DoubleSpinBox15;
	QDoubleSpinBox *DoubleSpinBox16;
	QDoubleSpinBox *DoubleSpinBox17;
	QDoubleSpinBox *DoubleSpinBox18;
	QDoubleSpinBox *DoubleSpinBox19;
	QDoubleSpinBox *DoubleSpinBox20;
	QDoubleSpinBox *DoubleSpinBox21;
	QDoubleSpinBox *DoubleSpinBox22;
	
	QPushButton *computePushButton;
	
public slots:
	void compute();
												
								
};

#endif // __EULUMDATWIDGET4_H__
