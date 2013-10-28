#ifndef __EULUMDATWIDGET3_H__
#define __EULUMDATWIDGET3_H__

#include <QtWidgets>

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
