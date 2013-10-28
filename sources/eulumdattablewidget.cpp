#include <QtWidgets>

#include "vars.h"
#include "eulumdattablewidget.h"
#include "mainwindow.h"

EulumdatTableWidget::EulumdatTableWidget(Eulumdat &ldt, QWidget *parent) : QWidget(parent)
{	
	pldt = &ldt;
	
	importPushButton = new QPushButton(tr("Paste from Clipboard"));	
    connect(importPushButton, SIGNAL(clicked()), this, SLOT(importFromClipboard()));	
    
	exportPushButton = new QPushButton(tr("Copy to Clipboard"));	
    connect(exportPushButton, SIGNAL(clicked()), this, SLOT(exportToClipboard()));	
    
    importPushButton->setToolTip(tr("Compatible with") + ": OpenOffice.org Calc, Microsoft Excel");
    exportPushButton->setToolTip(tr("Compatible with") + ": OpenOffice.org Calc, Microsoft Excel");    
    
	QHBoxLayout *HLayout1 = new QHBoxLayout; 
	HLayout1->addWidget(exportPushButton);
	HLayout1->addWidget(importPushButton);	
	
	tableWidget = new QTableWidget(this);

	
    QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(HLayout1);
	mainLayout->addWidget(tableWidget);
    setLayout(mainLayout);	
    
    setTable();
    
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
}

void EulumdatTableWidget::exportToClipboard() {
	QClipboard *clipboard = QApplication::clipboard();
	QMimeData *mimeData = new QMimeData;
	QString s = "\t";

    int j=0;
    if(pldt->iIsym == 3) {
    	while(pldt->dC[j] < 90.0) {
    		j++;
   		}
   	}

	QLocale locale;

    for(int i=0; i<pldt->iMc; i++) {    	
    	s.append(locale.toString(pldt->dC[i+j]));
    	if(i < pldt->iMc-1) {
	    	s.append("\t"); 
   		}   	
   	}
  	s.append("\n");
	
	for(int j=0; j<pldt->iNg; j++) {    	 	
    	s.append(locale.toString(pldt->dG[j]));
    	s.append("\t"); 
		for(int i=0; i<pldt->iMc; i++) {			
    		s.append(locale.toString(pldt->dLcd[i][j]));
    		if(i < pldt->iMc-1) {
		    	s.append("\t"); 
   			}   			
		}
	  	s.append("\n");		
   	}

	mimeData->setText(s);
	clipboard->setMimeData(mimeData);
	
	Vars().mainWindow->statusBar()->showMessage(tr("Data copied"), 5000);	
}

void EulumdatTableWidget::importFromClipboard() {
	
	try {
		tryToImportFromClipboard();
	}
	catch (QString error) {
	    QMessageBox::warning(this, tr("Paste from Clipboard"), error);
	    return;
	}
	catch(...) {
	    QMessageBox::critical(this, tr("Paste from Clipboard"),
	    		tr("Unknown error!"));	
	    return;		
	}

	setTable();
	Vars().mainWindow->central->general->setValues();
	Vars().mainWindow->central->render->setVectors();
	
	Vars().mainWindow->statusBar()->showMessage(tr("Data pasted"), 5000);	

}

void EulumdatTableWidget::tryToImportFromClipboard() {
	
	QClipboard *clipboard = QApplication::clipboard();
	const QMimeData *mimeData = clipboard->mimeData();
	int n = 0;
	QString data = "";

	if(mimeData->hasHtml()) {	
		data = mimeData->html();
		n = data.indexOf("/tr", 0, Qt::CaseInsensitive);				
		data.remove(n, data.length()-n);			
		n = 0;
		while( data.indexOf("/td", 0, Qt::CaseInsensitive) != -1) {
			n++;
			data.remove(0, data.indexOf("/td", 0, Qt::CaseInsensitive)+3);	
		}	
	}
	else if(mimeData->hasText()) {	
		data = mimeData->text();
		n = data.indexOf("\n", 0, Qt::CaseInsensitive);				
		data.remove(n, data.length()-n);			
		n = 1;
		while( data.indexOf("\t", 0, Qt::CaseInsensitive) != -1) {
			n++;
			data.remove(0, data.indexOf("\t", 0, Qt::CaseInsensitive)+1);	
		}	
	}	
	else {	
		throw tr("Nothing to paste");
	}

	if(n < 2) {			
		throw tr("None useful data");
	}
	
	data = mimeData->text();
	data.replace("\t",";");
	data.replace("\n",";");
	data.replace(",",".");
	
	QStringList list = data.split(";", QString::SkipEmptyParts);
	
	int a = list.size()+1;
	int m = a/n;

	if( (n < 2) || (m < 5) || (a < 10) || ((a % n) != 0) ) {
		throw tr("Lack of useful data");
	}
	bool ok;
	double d=0;
	for(int i=0; i<list.size(); i++) {
		d = list[i].toDouble(&ok);
		if(ok == false) {
			throw tr("Data are not real numbers");
		}
		if( (d < 0) || (d > 1000000) ) {
			throw tr("Values out of range");
		}
	}
			
	int i;			
	double *aC;
	double *aG;
	double **aLcd;		
	aC = new double[n-1];
	aG = new double[m-1];
	aLcd = new double*[n-1];
	
	for(i=0; i<(n-1); i++) {
		aLcd[i] = new double[m-1];
	}
	
	for(i=0; i<(n-1); i++) {
		aC[i] = list[i].toDouble();
	}
	
	int g=0;
	int j, h;
	for(j=0; j<(m-1); j++) {
		aG[g++] = list[i++].toDouble();
		for(h=0; h<(n-1); h++) {
			aLcd[h][j] = list[i++].toDouble();
		}			
	}			
	
	for(i=1; i<(n-1); i++) {
		if(aC[i-1] >= aC[i]) {
			throw tr("C-planes not sorted");
		}
	}
	for(i=1; i<(m-1); i++) {
		if(aG[i-1] >= aG[i]) {
			throw tr("Gamma-planes not sorted");
		}		
	}		
	if( (aG[0] < 0) || (aG[m-2] > 180) ){
		throw tr("Wrong Gamma-planes scheme");
	}
	
	bool underone = true;
	for(j=0; j<(m-1); j++) {
		for(h=0; h<(n-1); h++) {
			if(aLcd[h][j] >= 1) {
				underone = false;
				break;
			}
		}			
	}
	if(underone) {
		throw tr("Values are too low");
	}	

	// iSym == 1
	if( (aC[0] == 0) && (n == 2) ) {
		pldt->iIsym = 1;
		pldt->iNc = n-1;
		pldt->iNg = m-1;	
		
		double staticG = aG[1] - aG[0];
		for(i=1; i<(m-1); i++) {
			if( (aG[i] - aG[i-1]) != staticG) {
				staticG = 0;
			}			
		}			
				
		pldt->dDc = 0;
		pldt->dDg = staticG;
		pldt->calcMc();
			
		pldt->dC = aC;
		pldt->dG = aG;
		pldt->dLcd = aLcd;	
		
	}
	// isym == 0
	else if( (aC[0] == 0) && (aC[n-2] > 270) && (n > 4) ) {	
		for(i=0; i<(n-1); i++) {
			if(aC[i] == 90) {
				break;
			}
		}	
		if(i == (n-1)) {
			throw tr("Missing C90-plane");
		}
		for(i=0; i<(n-1); i++) {
			if(aC[i] == 180) {
				break;
			}
		}	
		if(i == (n-1)) {
			throw tr("Missing C180-plane");
		}	
		for(i=0; i<(n-1); i++) {
			if(aC[i] == 270) {
				break;
			}
		}	
		if(i == (n-1)) {
			throw tr("Missing C270-plane");
		}			
					
		pldt->iIsym = 0;
		pldt->iNc = n-1;
		pldt->iNg = m-1;	
		
		double staticC = aC[1] - aC[0];
		for(i=1; i<(n-1); i++) {
			if( (aC[i] - aC[i-1]) != staticC) {
				staticC = 0;
			}
		}
		double staticG = aG[1] - aG[0];
		for(i=1; i<(m-1); i++) {
			if( (aG[i] - aG[i-1]) != staticG) {
				staticG = 0;
			}			
		}			
			
		pldt->dDc = staticC;
		pldt->dDg = staticG;
		pldt->calcMc();
			
		pldt->dC = aC;
		pldt->dG = aG;
		pldt->dLcd = aLcd;	

	}
	// Isym == 2
	else if( (aC[0] == 0) && (aC[n-2] == 180) && (n > 3) ) {
		for(i=0; i<(n-1); i++) {
			if(aC[i] == 90) {
				break;
			}
		}	
		if(i == (n-1)) {
			throw tr("Missing C90-plane");
		}
				
		pldt->iIsym = 2;
		pldt->iNc = 2*(n-1)-2;
		pldt->iNg = m-1;	
		
		double staticC = aC[1] - aC[0];
		for(i=1; i<(n-1); i++) {
			if( (aC[i] - aC[i-1]) != staticC) {
				staticC = 0;
			}
		}
		double staticG = aG[1] - aG[0];
		for(i=1; i<(m-1); i++) {
			if( (aG[i] - aG[i-1]) != staticG) {
				staticG = 0;
			}			
		}			
				
		pldt->dDc = staticC;
		pldt->dDg = staticG;
		pldt->calcMc();
		pldt->dC = new double[pldt->iNc];
		
		for(i=0; i<(n-1); i++) {	
			pldt->dC[i] = aC[i];
		}
		
		for(j=n-1-2; j>0; j--) {	
			pldt->dC[i++] = 360 - aC[j];
		}			
					
		pldt->dG = aG;
		pldt->dLcd = aLcd;	
	}		
	// Isym == 4
	else if( (aC[0] == 0) && (aC[n-2] == 90) && (n > 2) ) {
		pldt->iIsym = 4;
		pldt->iNc = 4*(n-1)-4;
		pldt->iNg = m-1;	
		
		double staticC = aC[1] - aC[0];
		for(i=1; i<(n-1); i++) {
			if( (aC[i] - aC[i-1]) != staticC) {
				staticC = 0;
			}
		}
		double staticG = aG[1] - aG[0];
		for(i=1; i<(m-1); i++) {
			if( (aG[i] - aG[i-1]) != staticG) {
				staticG = 0;
			}			
		}			
				
		pldt->dDc = staticC;
		pldt->dDg = staticG;
		pldt->calcMc();
		pldt->dC = new double[pldt->iNc];		

		for(i=0; i<(n-1); i++) {	
			pldt->dC[i] = aC[i];
		}
		for(j=n-1-2; j>=0; j--) {	
			pldt->dC[i++] = 180 - aC[j];
		}	
		for(j=i-2; j>0; j--) {	
			pldt->dC[i++] = 360 - pldt->dC[j];
		}
					
		pldt->dG = aG;
		pldt->dLcd = aLcd;	
	}	
	// Isym == 3
	else if( (aC[0] == 90) && (aC[n-2] == 270) && (n > 3) ) {		
		for(i=0; i<(n-1); i++) {
			if(aC[i] == 180) {
				break;
			}
		}	
		if(i == (n-1)) {
			throw tr("Missing C180-plane");
		}
		
		pldt->iIsym = 3;
		pldt->iNc = 2*(n-1)-2;
		pldt->iNg = m-1;	
			
		double staticC = aC[1] - aC[0];
		for(i=1; i<(n-1); i++) {
			if( (aC[i] - aC[i-1]) != staticC) {
				staticC = 0;
				break;
			}
		}
		double staticG = aG[1] - aG[0];
		for(i=1; i<(m-1); i++) {
			if( (aG[i] - aG[i-1]) != staticG) {
				staticG = 0;
				break;
			}			
		}			
				
		pldt->dDc = staticC;
		pldt->dDg = staticG;
		pldt->calcMc();		
		pldt->dC = new double[pldt->iNc];
			
		i = 0;
		j = n-1-2;
		while(aC[j] != 180) {
			j--;
		}	
		for(; j>0; j--) {		
			pldt->dC[i++] = 180 - aC[j];
		}
		for(j=0; j<(n-1); j++) {	
			pldt->dC[i++] = aC[j];
		}	
		j = n-1-2;
		while(aC[j] != 180) {
			pldt->dC[i++] = 540 - aC[j];
			j--;
		}			
		pldt->dG = aG;
		pldt->dLcd = aLcd;	
	}						
	else {
		throw tr("Wrong C-planes scheme");			
	}
}

void EulumdatTableWidget::setTable() {
	
	tableWidget->setRowCount(pldt->iNg);  
    tableWidget->setColumnCount(pldt->iMc);
    
    QStringList CPlanes, Gamma;    
    QLocale locale;
    
    int j=0;
    if(pldt->iIsym == 3) {
    	while(pldt->dC[j] < 90.0) {
    		j++;
   		}
   	}
    
    for(int i=0; i<pldt->iMc; i++) {
		CPlanes << QString("C%1%2").arg(locale.toString(pldt->dC[i+j])).arg((char)176);   	
   	}
   	tableWidget->setHorizontalHeaderLabels(CPlanes);
    for(int i=0; i<pldt->iNg; i++) {    	
    	Gamma << QString("%1%2").arg(locale.toString(pldt->dG[i])).arg((char)176);
   	}
   	tableWidget->setVerticalHeaderLabels(Gamma);	
	
	QTableWidgetItem *newItem;

	for(int i=0; i<pldt->iMc; i++) {		
		for(int j=0; j<pldt->iNg; j++) {
			newItem = new QTableWidgetItem(QString("%1").arg(locale.toString(pldt->dLcd[i][j])));
			newItem->setTextAlignment(Qt::AlignCenter);
			tableWidget->setItem(j, i, newItem);
		}
	}
	tableWidget->resizeColumnsToContents();
	tableWidget->resizeRowsToContents();

}
