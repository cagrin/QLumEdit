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

#ifndef __NEWDIALOG_H__
#define __NEWDIALOG_H__

#include <QDialog>

class QPushButton;
class QComboBox;

class NewDialog : public QDialog
{
    Q_OBJECT

public:
    NewDialog(QWidget *parent = 0);
    
private slots:
	void setA();
	void setB();
	void setC();
	void setD();	
	
	void setTemplate();		

private:

	QPushButton *pushButtonA;
	QPushButton *pushButtonB;
	QPushButton *pushButtonC;
	QPushButton *pushButtonD;			
	
	QPushButton *pushTemplate;
	
	QComboBox *ComboBox1;
	QComboBox *ComboBox2;
	QComboBox *ComboBox3;

};

#endif // __NEWDIALOG_H__
