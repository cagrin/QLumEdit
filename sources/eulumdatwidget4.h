#ifndef __EULUMDATWIDGET4_H__
#define __EULUMDATWIDGET4_H__

#include <QtWidgets>

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
