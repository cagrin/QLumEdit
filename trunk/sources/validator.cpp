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

#include "validator.h"

#include <QtGui>

#include "eulumdat.h"

ValidatorDialog::ValidatorDialog(QWidget *parent) : QDialog(parent)
{
	setWindowTitle(tr("Eulumdat Validator"));
	setMinimumWidth(480);	
	setMinimumHeight(320);	
	
	textEdit = new QTextEdit;
	textEdit->setWordWrapMode(QTextOption::NoWrap);
	
	openDir = new QPushButton(tr("Open directory..."));
	stopButton = new QPushButton(tr("Stop"));	
	dirName = new QLabel(tr("Choose directory to scan for Eulumdat files"));

    connect(this, SIGNAL(rejected()), this, SLOT(reject()));    
    connect(openDir, SIGNAL(clicked()), this, SLOT(openDirectory()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stopAll())); 
    
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));    

    QHBoxLayout *HLayout1 = new QHBoxLayout; 
	HLayout1->addWidget(openDir);  
	HLayout1->addWidget(stopButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(HLayout1);
    mainLayout->addWidget(dirName);
    mainLayout->addWidget(textEdit);
	mainLayout->addWidget(buttonBox);    
    setLayout(mainLayout);

}

void ValidatorDialog::stopAll() {
	stop = true;
}

void ValidatorDialog::reject() {
	stopAll();
	QDialog::close();
}

void ValidatorDialog::openDirectory() {
	
	QSettings settings("Cagrinlabs", "QLumEdit");
	QDir info(settings.value("validatordir", QString()).toString());	
	
    QString directory = QFileDialog::getExistingDirectory(this, tr("Open directory"),
                        info.path(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
                                                 
    if(!directory.isEmpty()) {
   		settings.setValue("validatordir", directory);
	}
	else {
		return;
	}
	
    QStringList filters;
    filters << "*.ldt";    
    QStringList files;	
	
	dirName->setText(tr("Searching..."));	
    textEdit->setHtml("");	
	stop = false;
	openDir->setEnabled(false);

    QApplication::setOverrideCursor(Qt::BusyCursor);        
    QCoreApplication::processEvents(); 

	try {
	    files = getAllFilesFromPath(directory, filters);			
	}
	catch(int) {
		openDir->setEnabled(true);
		dirName->setText(tr("Choose directory to scan for Eulumdat files"));
	    QApplication::restoreOverrideCursor();		
		return;
	}
    
    QApplication::restoreOverrideCursor();
    QCoreApplication::processEvents(); 
    
	dirName->setText(directory);       
    
    QString text;
    Eulumdat * ldt = 0;
    int w=0, e=0, i=0;

    for(i=0; i<files.size(); i++) {	
    	ldt = new Eulumdat();
    	dirName->setText(QString("<b> %1 / %2 </b>").arg(i).arg(files.size())
    	+ QString(" (<font color=blue>%1</font> / <font color=red>%2</font>)").arg(w).arg(e)
    	+ "<br>" + files[i]);
    	try {
			QStringList warnings = ldt->loadFile(files[i]);
			if(!warnings.isEmpty()) {
							
				text = files[i] + "<br>";
				//text = "<a href=\"file://" + files[i] + "\">" + files[i] + "</a><br>";

				for(int i=0; i<warnings.size(); i++) {
					text += "<font color=blue>" + tr("warning") + ": "+ warnings[i];
					text += "</font>";
					if(i != warnings.size()-1) {
						text += "<br>";
					}
				}		
				textEdit->append(text);	
				w++;				
			}
		}
		catch (QString error) {  
			text = files[i] + "<br>"; 
			text += "<font color=red>" + tr("error") + ": " + error + "</font>";
			textEdit->append(text);	
			e++;			
		}
		catch(...) {
			text = files[i] + "<br>"; 
			text += "<b>" + tr("Unknown error!") + "</b>";
			textEdit->append(text);	
			e++;		
		}
		    	
    	QCoreApplication::processEvents();
    	delete ldt;
    	if(stop) {
    		break;
   		}
   	}
   	
   	if(stop) {
	   	dirName->setText(tr("Stopped") + ". " + tr("Validated")
	   	+ QString(" <b> %1 / %2 </b>").arg(i).arg(files.size()) + tr("files")
	   	+ QString(" (<font color=blue>%1 ").arg(w) + tr("warnings")
	   	+ QString("</font> / <font color=red>%2 ").arg(e) + tr("errors")
	   	+ "</font>) <br>" + tr("from directory (including subfolders)") + " " + directory  );  		   		
  	}
  	else {  	
	   	dirName->setText(tr("Validated") + QString(" <b> %1 </b>").arg(files.size()) + tr("files")
	   	+ QString(" (<font color=blue>%1 ").arg(w) + tr("warnings")
	   	+ QString("</font> / <font color=red>%2 ").arg(e) + tr("errors")
	   	+ "</font>) <br>" + tr("from directory (including subfolders)") + " " + directory  );  		
 	}
 	
 	openDir->setEnabled(true);
}


QStringList ValidatorDialog::getAllSubdirs(const QString & path) {
	
	QStringList list, subdirs;
	QDir dir(path);
	
	dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Hidden);
	list = dir.entryList();
	for(int i=0; i<list.size(); i++) {
		subdirs << dir.absoluteFilePath(list.at(i));	
		subdirs << getAllSubdirs(dir.absoluteFilePath(list.at(i)));
		
			QCoreApplication::processEvents();
		   	if(stop) throw 1;		
		
	}

	return subdirs;
}

QStringList ValidatorDialog::getAllFilesFromPath(const QString & path, const QStringList & nameFilters) {
	
	QStringList list, dirs, files;
	QDir dir;
	
	dirs << path << getAllSubdirs(path);
	dir.setFilter(QDir::Files | QDir::Hidden);
	dir.setNameFilters(nameFilters);
	
	for(int i=0; i<dirs.size(); i++) {
		dir.setPath(dirs.at(i));
		list = dir.entryList();
		for(int j=0; j<list.size(); j++) {
			files << dir.absoluteFilePath(list.at(j));
			
			QCoreApplication::processEvents();
		   	if(stop) throw 1;
		
		}
	}
	
	return files;
}
