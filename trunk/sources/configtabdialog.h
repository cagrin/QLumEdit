#ifndef __CONFIGTABDIALOG_H__
#define __CONFIGTABDIALOG_H__

#include <QDialog>

class QDialogButtonBox;
class QComboBox;
class QTabWidget;
class QRadioButton;
class QCheckBox;

class TabDialog : public QDialog
{
    Q_OBJECT

public:
    TabDialog(QWidget *parent = 0);
    
private slots:
	void accept();

private:
    QDialogButtonBox *buttonBox; 
    QRadioButton *radio1;
    QRadioButton *radio2; 
    QComboBox * comboBox1;
    QCheckBox * checkBox;
    QCheckBox * legendBox;
};

#endif
