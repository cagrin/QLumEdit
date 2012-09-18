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

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>

#include "eulumdat.h"
#include "maintabwidget.h"

class QAction;
class QMenu;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void documentWasModified();

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void options();
    void validator();    
    void about();
    
    void exportToHTML();
   	void exportToIesna();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    
    void setCurrentFile(const QString &fileName = QString());
    
    QString strippedName(const QString &fullFileName);
    
    QMenu *fileMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;
    QMenu *exportSubMenu;
    QToolBar *fileToolBar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exportToHTMLAct;
    QAction *exportToIesnaAct;    
    QAction *exitAct;
    
    QAction *optionsAct;
    QAction *validatorAct;    
    QAction *aboutAct;
    QAction *aboutQtAct;
   
    QString curFile;    
    bool mods;
    
    void saveFromWidget(void);
    
    void setMaybeSaveTriggers();

public:
    Eulumdat * ldt;
    MainTabWidget *central;
    
    void setValidatorOptions();  
    void loadFile(const QString &fileName);
            
};

#endif
