#include <QtWidgets>

#include "mainwindow.h"

#include "vars.h"
#include "eulumdatwidget.h"
#include "eulumdatwidget2.h"
#include "eulumdatwidget3.h"
#include "eulumdatwidget4.h"
#include "eulumdattablewidget.h"
#include "renderwidget.h"
#include "configtabdialog.h"
#include "validator.h"
#include "newdialog.h"

MainWindow::MainWindow()
{
	Vars().mainWindow = this;	
	
    mods = false;
    setMinimumWidth(640);
    //setMinimumHeight(480);    
      
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();
	setValidatorOptions();
    setCurrentFile("");
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About QLumEdit"), "<b><font size=\"+4\">QLumEdit 1.0.2</font></b><br><br>"+
            tr("An ultimate open-source Eulumdat file editor.")+"<br><br>"
                "Copyright (C) 2007-2021 Krzysztof Strugi&#324;ski "
            	"<a href=\"mailto:cagrin@gmail.com\">cagrin@gmail.com</a>" "<br><br>"
	            "<a href=\"http://sourceforge.net/projects/qlumedit/\">http://sourceforge.net/projects/qlumedit/</a>" "<br><br>"            	
            	+ tr("You may use, distribute and copy the QLumEdit under "
 				"the terms of GPL version 2.")
 				+ "<br><br>"
 				+ "<img src=\":images/lumen.png\" />&nbsp;&nbsp;"
 				+ tr("This version of QLumEdit is a part of master thesis "
 				"and it was made under supervision of Poznan University of Technology") + "<br>"
 				+ tr("Institute of Electrical Engineering and Electronics") + "<br>"
 				+ tr("Laboratory of Lighting Engineering and Electrothermics")
 				);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile()
{
    maybeSave();
    
    saveAct->setEnabled(false);
	saveAsAct->setEnabled(false);    
    exportSubMenu->setEnabled(false);
	setCentralWidget(new QWidget);	   

	
	NewDialog *newDialog;
	newDialog = new NewDialog(this);
	newDialog->exec();
	
	if(Vars().newFile != "") {
		ldt = new Eulumdat();
		ldt->loadFile(Vars().newFile);	
		
		saveAsAct->setEnabled(true);
		exportSubMenu->setEnabled(true);
	
		central = new MainTabWidget(*ldt, this);
	    setCentralWidget(central);
		setMaybeSaveTriggers();
	    setCurrentFile();		
	}    
}

void MainWindow::open()
{
    if (maybeSave()) {
    	
	    QSettings settings("Cagrinlabs", "QLumEdit");
	    QFileInfo info(settings.value("loaddir", QString()).toString());
	    		    	
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), info.path(), tr("Eulumdat file (*.ldt)"));
	                                                 
        if(!fileName.isEmpty()) {
            loadFile(fileName);
   			settings.setValue("loaddir", fileName);
       	}

    }
}

bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QSettings settings("Cagrinlabs", "QLumEdit");
    QFileInfo info(settings.value("savedir", QString()).toString());
    		    	
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), info.path()+"/"+QFileInfo(curFile).fileName(), tr("Eulumdat file (*.ldt)"));
                                                 
    if (fileName.isEmpty()) {
        return false;
    }

	settings.setValue("savedir", fileName);

    return saveFile(fileName);
}

void MainWindow::documentWasModified()
{
    mods = true;
   	saveAct->setEnabled(true);
   	saveAsAct->setEnabled(true);   	
}

void MainWindow::createActions()
{
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New..."), this);
    newAct->setShortcut(tr("Ctrl+N"));
    newAct->setStatusTip(tr("Create a new Eulumdat file from template"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("Open an existing Eulumdat file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save the file to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setStatusTip(tr("Save the file under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));
    
    exportToHTMLAct = new QAction("HTML/PNG...", this);
    connect(exportToHTMLAct, SIGNAL(triggered()), this, SLOT(exportToHTML()));   
    exportToIesnaAct = new QAction("Iesna LM-63-1995...", this);
    connect(exportToIesnaAct, SIGNAL(triggered()), this, SLOT(exportToIesna()));    
    

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));


    validatorAct = new QAction(tr("Eulumdat &Validator..."), this);
    validatorAct->setShortcut(tr("Ctrl+V"));
    validatorAct->setStatusTip(tr("Run multiple file validator"));
    connect(validatorAct, SIGNAL(triggered()), this, SLOT(validator()));

    optionsAct = new QAction(tr("&Options..."), this);
    optionsAct->setStatusTip(tr("Show options"));
    connect(optionsAct, SIGNAL(triggered()), this, SLOT(options()));


    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show informations about QLumEdit"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show informations about Qt library"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    
    saveAct->setEnabled(false);
    saveAsAct->setEnabled(false);    
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    
    exportSubMenu = new QMenu(tr("Export as"));
    exportSubMenu->addAction(exportToHTMLAct);
    exportSubMenu->addAction(exportToIesnaAct);   
    fileMenu->addMenu(exportSubMenu); 
    
    fileMenu->addSeparator();    
    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();

	toolsMenu = menuBar()->addMenu(tr("&Tools"));
	toolsMenu->addAction(validatorAct);	
	toolsMenu->addSeparator();
	toolsMenu->addAction(optionsAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
    
    exportSubMenu->setEnabled(false);
    
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings("Cagrinlabs", "QLumEdit");
    QPoint pos = settings.value("position", QPoint(200, 200)).toPoint();
	resize(QSize(400, 400));
    move(pos);
	setValidatorOptions();
}

void MainWindow::writeSettings()
{
    QSettings settings("Cagrinlabs", "QLumEdit");
    settings.setValue("position", pos());
}

bool MainWindow::maybeSave()
{
	if(mods == true) {    	
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("QLumEdit"),
                     tr("The file has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("QLumEdit"), tr("Cannot read file")
        	+ QString(" %1:\n%2.").arg(fileName).arg(file.errorString()));                                                          
        return;

    }

    saveAct->setEnabled(false);
	saveAsAct->setEnabled(false);  
	exportSubMenu->setEnabled(false);  
	setCentralWidget(new QWidget);	    

	ldt = new Eulumdat();
	try {
		QStringList warnings = ldt->loadFile(fileName);
		if(!warnings.isEmpty()) {
			QString warn = QString("<b>%1 ").arg(warnings.size()) + tr("warnings") + ":</b><br><br>";
			
			for(int i=0; i<warnings.size(); i++) {
				warn += warnings[i];
				warn += "<br>";
			}
			
			warn += "<br><b>" + tr("Auto corrected") + ".</b>";
			
			QMessageBox::warning(this, tr("Loading file"), warn);				
		}
	}
	catch (QString error) {        
		QMessageBox::critical(this, tr("Cannot load file"), error);	
		return;
	}
	catch(...) {
		QMessageBox::critical(this, tr("Cannot load file"), tr("Unknown error!"));	
		return;		
	}

	saveAsAct->setEnabled(true); 
	exportSubMenu->setEnabled(true);
	 	
	central = new MainTabWidget(*ldt, this);
    setCentralWidget(central);

	setMaybeSaveTriggers();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 5000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("QLumEdit"), tr("Cannot write file")
        	+ QString(" %1:\n%2.").arg(fileName).arg(file.errorString()));    
        return false;
    }


	saveFromWidget();

	ldt->saveFile(fileName);

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 5000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
	mods = false;
    setWindowModified(false);

    QString shownName;
    if (curFile.isNull()) {
		curFile = shownName = tr("untitled.ldt");   	
	    setWindowTitle(QString("%1[*] - %2").arg(shownName).arg("QLumEdit"));	
   	}
    else if (curFile.isEmpty()) {
		curFile = shownName = tr("untitled.ldt");
		setWindowTitle("QLumEdit");			   	
   	}   	
   	else {
		shownName = strippedName(curFile);   	
	    setWindowTitle(QString("%1[*] - %2").arg(shownName).arg("QLumEdit"));			
  	}
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::saveFromWidget(void) {
	
	ldt->sIden = central->general->LineEdit1->text();	
	ldt->iItyp = central->general->ComboBox2->currentIndex()+1;
	ldt->sMrn = central->general->LineEdit8->text();
	ldt->sLnam = central->general->LineEdit9->text();
	ldt->sLnum = central->general->LineEdit10->text();
	ldt->sFnam = central->general->LineEdit11->text();
	ldt->sDate = central->general->LineEdit12->text();
	
	ldt->dL = central->luminaire->DoubleSpinBox13->value();
	ldt->dB = central->luminaire->DoubleSpinBox14->value();
	ldt->dH = central->luminaire->DoubleSpinBox15->value();
	ldt->dLa = central->luminaire->DoubleSpinBox16->value();
	ldt->dB1 = central->luminaire->DoubleSpinBox17->value();
	ldt->dHC0 = central->luminaire->DoubleSpinBox18->value();
	ldt->dHC90 = central->luminaire->DoubleSpinBox19->value();
	ldt->dHC180 = central->luminaire->DoubleSpinBox20->value();
	ldt->dHC270 = central->luminaire->DoubleSpinBox21->value();
	ldt->dDFF = central->luminaire->DoubleSpinBox22->value();
	ldt->dLORL = central->luminaire->DoubleSpinBox23->value();
	ldt->dCFLI = central->luminaire->DoubleSpinBox24->value();
	ldt->dTILT = central->luminaire->DoubleSpinBox25->value();

	int i;
	ldt->iNL = new int[ldt->iN];
	ldt->sTL = new QString[ldt->iN];
	ldt->dTLF = new double [ldt->iN];
	ldt->sCA = new QString[ldt->iN];
	ldt->sCRG = new QString[ldt->iN];
	ldt->dWB = new double[ldt->iN];
	
    for(i=0; i<ldt->iN; i++) {
    	ldt->iNL[i] = central->lamps->SpinBox2[i]->value();
    	ldt->sTL[i] = central->lamps->LineEdit3[i]->text();
    	ldt->dTLF[i] = central->lamps->DoubleSpinBox4[i]->value();
    	ldt->sCA[i] = central->lamps->LineEdit5[i]->text();
    	ldt->sCRG[i] = central->lamps->LineEdit6[i]->text();
    	ldt->dWB[i] = central->lamps->DoubleSpinBox7[i]->value();
    }
    
    ldt->dDR[0] = central->UFtable->DoubleSpinBox13->value();
    ldt->dDR[1] = central->UFtable->DoubleSpinBox14->value();
    ldt->dDR[2] = central->UFtable->DoubleSpinBox15->value();
    ldt->dDR[3] = central->UFtable->DoubleSpinBox16->value();
    ldt->dDR[4] = central->UFtable->DoubleSpinBox17->value();
    ldt->dDR[5] = central->UFtable->DoubleSpinBox18->value();
    ldt->dDR[6] = central->UFtable->DoubleSpinBox19->value();
    ldt->dDR[7] = central->UFtable->DoubleSpinBox20->value();
    ldt->dDR[8] = central->UFtable->DoubleSpinBox21->value();
    ldt->dDR[9] = central->UFtable->DoubleSpinBox22->value();

}

void MainWindow::setMaybeSaveTriggers() {

    connect(central->general->LineEdit1, SIGNAL(textChanged(QString)), this, SLOT(documentWasModified())); 
	connect(central->general->ComboBox2, SIGNAL(currentIndexChanged(QString)), this, SLOT(documentWasModified()));      
    connect(central->general->LineEdit8, SIGNAL(textChanged(QString)), this, SLOT(documentWasModified())); 
    connect(central->general->LineEdit9, SIGNAL(textChanged(QString)), this, SLOT(documentWasModified())); 
    connect(central->general->LineEdit10, SIGNAL(textChanged(QString)), this, SLOT(documentWasModified())); 
    connect(central->general->LineEdit11, SIGNAL(textChanged(QString)), this, SLOT(documentWasModified())); 
    connect(central->general->LineEdit12, SIGNAL(textChanged(QString)), this, SLOT(documentWasModified()));                     
    
    connect(central->luminaire->DoubleSpinBox13, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->luminaire->DoubleSpinBox14, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->luminaire->DoubleSpinBox15, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->luminaire->DoubleSpinBox16, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->luminaire->DoubleSpinBox17, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->luminaire->DoubleSpinBox18, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->luminaire->DoubleSpinBox19, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->luminaire->DoubleSpinBox20, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->luminaire->DoubleSpinBox21, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->luminaire->DoubleSpinBox22, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->luminaire->DoubleSpinBox23, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));    
    connect(central->luminaire->DoubleSpinBox24, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->luminaire->DoubleSpinBox25, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));    
    
    for(int i=0; i<ldt->iN; i++) {
    	connect(central->lamps->SpinBox2[i], SIGNAL(valueChanged(int)), this, SLOT(documentWasModified()));    
    	connect(central->lamps->LineEdit3[i], SIGNAL(textChanged(QString)), this, SLOT(documentWasModified()));    
    	connect(central->lamps->DoubleSpinBox4[i], SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));    
    	connect(central->lamps->LineEdit5[i], SIGNAL(textChanged(QString)), this, SLOT(documentWasModified()));    
    	connect(central->lamps->LineEdit6[i], SIGNAL(textChanged(QString)), this, SLOT(documentWasModified()));    
	   	connect(central->lamps->DoubleSpinBox7[i], SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));        	    	
    }
    
    connect(central->UFtable->DoubleSpinBox13, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->UFtable->DoubleSpinBox14, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->UFtable->DoubleSpinBox15, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->UFtable->DoubleSpinBox16, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->UFtable->DoubleSpinBox17, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->UFtable->DoubleSpinBox18, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->UFtable->DoubleSpinBox19, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->UFtable->DoubleSpinBox20, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->UFtable->DoubleSpinBox21, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));
    connect(central->UFtable->DoubleSpinBox22, SIGNAL(valueChanged(double)), this, SLOT(documentWasModified()));    

	connect(central->table->tableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(documentWasModified()));    
 
	
}


void MainWindow::options()
{	
	TabDialog *tabdialog;
	tabdialog = new TabDialog(this);
	tabdialog->exec();

}


void MainWindow::validator()
{	
	ValidatorDialog *validatorDialog;
	validatorDialog = new ValidatorDialog(this);
	validatorDialog->exec();
}

void MainWindow::setValidatorOptions() {
    QSettings settings("Cagrinlabs", "QLumEdit");	
    QString restricted = settings.value("restricted", QString("false")).toString();
    if(restricted == "true") {
	    Vars().line = 78;
	    Vars().lineFNam = 8;
	    Vars().lineTL = 24;
	    Vars().lineCA = 16;
	    Vars().lineCRG = 6;
   	}
   	else {
   		int length = 255;
	    Vars().line = length;
	    Vars().lineFNam = length;
	    Vars().lineTL = length;
	    Vars().lineCA = length;
	    Vars().lineCRG = length;   		
  	}	
}
