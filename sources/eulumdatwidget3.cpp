#include <QtWidgets>

#include "eulumdatwidget3.h"
#include "vars.h"

EulumdatWidget3::EulumdatWidget3(Eulumdat &ldt, QWidget *parent) : QWidget(parent)
{
    pldt = &ldt;    
    
    Label1 = new QLabel; 
    
    Label1->setText(tr("Number of standard sets of lamps") + QString(" = %1").arg(ldt.iN));
    
    addButton = new QPushButton(tr("Add"));    
    connect(addButton, SIGNAL(clicked()), this, SLOT(addSet()));    
    
    removeButton = new QPushButton(tr("Remove"));    
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeSet()));        
    
   
    QHBoxLayout *HLayout1 = new QHBoxLayout; 
    HLayout1->addWidget(Label1);  
    HLayout1->addStretch(1);        
    HLayout1->addWidget(addButton); 
    HLayout1->addWidget(removeButton);     

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(HLayout1);
    
    tabWidget = new QTabWidget;
    for(int i=0; i<ldt.iN; i++) {
        createSetPage();
        SpinBox2[i]->setValue(ldt.iNL[i]);    
        LineEdit3[i]->setText(ldt.sTL[i]);
        DoubleSpinBox4[i]->setValue(ldt.dTLF[i]);    
        LineEdit5[i]->setText(ldt.sCA[i]);
        LineEdit6[i]->setText(ldt.sCRG[i]);
        DoubleSpinBox7[i]->setValue(ldt.dWB[i]);            
        tabWidget->addTab(GroupBox[i], tr("Set") + QString(" %1").arg(i+1));
    }
    mainLayout->addWidget(tabWidget);
    mainLayout->addStretch(1);           
    setLayout(mainLayout);        
}

// Appends the widgets of the next set; the page is owned by tabWidget once added as a tab.
void EulumdatWidget3::createSetPage() {

    QLabel *Label2 = new QLabel(tr("Number of lamps"));
    QSpinBox *spinBox2 = new QSpinBox;
    spinBox2->setRange(1,1000);

    QLabel *Label3 = new QLabel(tr("Type of lamps"));
    QLineEdit *lineEdit3 = new QLineEdit;
    lineEdit3->setMaxLength(Vars().lineTL);

    QLabel *Label4 = new QLabel(tr("Total luminous flux of lamps"));
    QDoubleSpinBox *doubleSpinBox4 = new QDoubleSpinBox;
    doubleSpinBox4->setRange(1,9999999);
    doubleSpinBox4->setSuffix(" lm");
    doubleSpinBox4->setDecimals(0);

    QLabel *Label5 = new QLabel(tr("Color temperature of lamps"));
    QLineEdit *lineEdit5 = new QLineEdit;
    lineEdit5->setMaxLength(Vars().lineCA);

    QLabel *Label6 = new QLabel(tr("Color rendering index"));
    QLineEdit *lineEdit6 = new QLineEdit;
    lineEdit6->setMaxLength(Vars().lineCRG);

    QLabel *Label7 = new QLabel(tr("Wattage including ballast"));
    QDoubleSpinBox *doubleSpinBox7 = new QDoubleSpinBox;
    doubleSpinBox7->setRange(0.1,10000);
    doubleSpinBox7->setSuffix(" W");
    doubleSpinBox7->setDecimals(1);

    QHBoxLayout *HLayout2 = new QHBoxLayout;
    HLayout2->addWidget(Label2);
    HLayout2->addWidget(spinBox2);
    HLayout2->addStretch(1);
    QHBoxLayout *HLayout3 = new QHBoxLayout;
    HLayout3->addWidget(Label3);
    HLayout3->addWidget(lineEdit3);
    QHBoxLayout *HLayout4 = new QHBoxLayout;
    HLayout4->addWidget(Label4);
    HLayout4->addWidget(doubleSpinBox4);
    HLayout4->addStretch(1);
    QHBoxLayout *HLayout5 = new QHBoxLayout;
    HLayout5->addWidget(Label5);
    HLayout5->addWidget(lineEdit5);
    QHBoxLayout *HLayout6 = new QHBoxLayout;
    HLayout6->addWidget(Label6);
    HLayout6->addWidget(lineEdit6);
    QHBoxLayout *HLayout7 = new QHBoxLayout;
    HLayout7->addWidget(Label7);
    HLayout7->addWidget(doubleSpinBox7);
    HLayout7->addStretch(1);

    QVBoxLayout *mainRadioLayout = new QVBoxLayout;
    mainRadioLayout->addLayout(HLayout2);
    mainRadioLayout->addLayout(HLayout3);
    mainRadioLayout->addLayout(HLayout4);
    mainRadioLayout->addLayout(HLayout5);
    mainRadioLayout->addLayout(HLayout6);
    mainRadioLayout->addLayout(HLayout7);

    QWidget *page = new QWidget;
    page->setLayout(mainRadioLayout);

    SpinBox2 << spinBox2;
    LineEdit3 << lineEdit3;
    DoubleSpinBox4 << doubleSpinBox4;
    LineEdit5 << lineEdit5;
    LineEdit6 << lineEdit6;
    DoubleSpinBox7 << doubleSpinBox7;
    GroupBox << page;
}

void EulumdatWidget3::addSet(void) {
    
    int n = pldt->iN;
    
    if(n < 20) {
        // Pages of removed sets are kept by tabWidget and reused.
        if(n == GroupBox.size()) {
            createSetPage();
        }
        tabWidget->addTab(GroupBox[n], tr("Set") + QString(" %1").arg(n+1));
        pldt->iN++;    
        Label1->setText(tr("Number of standard sets of lamps") + QString(" = %1").arg(pldt->iN));        
        Vars().mainWindow->documentWasModified();
    }    
}
void EulumdatWidget3::removeSet(void) {
    
    int n = pldt->iN;
    
    if(n > 1) {
        tabWidget->removeTab(n-1);
        pldt->iN--;    
        Label1->setText(tr("Number of standard sets of lamps") + QString(" = %1").arg(pldt->iN));        
        Vars().mainWindow->documentWasModified();        
    }        
}
