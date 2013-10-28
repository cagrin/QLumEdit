#ifndef __EULUMDATWIDGET2_H__
#define __EULUMDATWIDGET2_H__

#include <QtWidgets>

#include "eulumdat.h"

class QTextEdit;
class QComboBox;
class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;
class QPushButton;

class EulumdatWidget2 : public QWidget
{
    Q_OBJECT

public:
    EulumdatWidget2(Eulumdat &ldt, QWidget *parent = 0);
    
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
	QDoubleSpinBox *DoubleSpinBox23;
	QDoubleSpinBox *DoubleSpinBox24;
	QDoubleSpinBox *DoubleSpinBox25;	
	
	QPushButton * downwardPushButton;	
	QPushButton * outputPushButton;		
	
public slots:
	void downward();
	void output();
								
};

#endif // __EULUMDATWIDGET2_H__
