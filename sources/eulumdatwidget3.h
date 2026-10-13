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
    QList<QSpinBox *> SpinBox2;
    QList<QLineEdit *> LineEdit3;
    QList<QDoubleSpinBox *> DoubleSpinBox4;
    QList<QLineEdit *> LineEdit5;
    QList<QLineEdit *> LineEdit6;
    QList<QDoubleSpinBox *> DoubleSpinBox7;

    QList<QWidget *> GroupBox;
    QTabWidget * tabWidget;

private slots:
    void addSet(void);
    void removeSet(void);

private:
    void createSetPage();
};

#endif // __EULUMDATWIDGET3_H__
