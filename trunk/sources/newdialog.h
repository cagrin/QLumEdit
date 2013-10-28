#ifndef __NEWDIALOG_H__
#define __NEWDIALOG_H__

#include <QDialog>

class QPushButton;
class QComboBox;

class NewDialog : public QDialog
{
    Q_OBJECT

public:
    NewDialog(QWidget *parent = 0);
    
private slots:
	void setA();
	void setB();
	void setC();
	void setD();	
	
	void setTemplate();		

private:

	QPushButton *pushButtonA;
	QPushButton *pushButtonB;
	QPushButton *pushButtonC;
	QPushButton *pushButtonD;			
	
	QPushButton *pushTemplate;
	
	QComboBox *ComboBox1;
	QComboBox *ComboBox2;
	QComboBox *ComboBox3;

};

#endif // __NEWDIALOG_H__
