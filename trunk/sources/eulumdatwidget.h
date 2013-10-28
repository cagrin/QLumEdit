#ifndef __EULUMDATWIDGET_H__
#define __EULUMDATWIDGET_H__

#include <QtWidgets>

#include "eulumdat.h"

class QTextEdit;
class QComboBox;
class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;
class QLabel;
class QPushButton;

class EulumdatWidget : public QWidget
{
    Q_OBJECT

public:
	EulumdatWidget(Eulumdat &ldt, QWidget *parent = 0);
    
	QLineEdit *LineEdit1;
	QComboBox *ComboBox2;	
	QLineEdit *LineEdit8;
	QLineEdit *LineEdit9;
	QLineEdit *LineEdit10;
	QLineEdit *LineEdit11;
	QLineEdit *LineEdit12;		
	
	Eulumdat * pldt;
	
	QLabel *Label2;
	QLabel *Label3;						
	QLabel *Label4;
	QLabel *Label5;
	QLabel *Label6;
	QLabel *Label7;
	
	void setValues();
	
	QPushButton *optimizePushButton;
private slots:
	void optimize();

};

#endif // __EULUMDATWIDGET_H__
