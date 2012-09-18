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

#ifndef __VALIDATOR_H__
#define __VALIDATOR_H__

#include <QDialog>
#include <QDir>

class QDialogButtonBox;
class QTextEdit;
class QPushButton;
class QLabel;

class ValidatorDialog : public QDialog
{
    Q_OBJECT

public:
    ValidatorDialog(QWidget *parent = 0);
    
private slots:
	void reject();
	void openDirectory();
	void stopAll();

private:
	QTextEdit *textEdit;
	QPushButton *openDir;
	QPushButton *stopButton;	
	QLabel *dirName;
    QDialogButtonBox *buttonBox;
    
	QStringList getAllFilesFromPath(const QString &, const QStringList &);
	QStringList getAllSubdirs(const QString &);  	
	bool stop;
};

#endif // __VALIDATOR_H__
