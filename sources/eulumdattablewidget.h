#ifndef __EULUMDATTABLEWIDGET_H__
#define __EULUMDATTABLEWIDGET_H__

#include <QtWidgets>

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
