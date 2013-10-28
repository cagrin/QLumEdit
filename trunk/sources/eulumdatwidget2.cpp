#include <QtWidgets>

#include "eulumdatwidget2.h"

EulumdatWidget2::EulumdatWidget2(Eulumdat &ldt, QWidget *parent) : QWidget(parent)
{
	pldt = &ldt;
	
    QLabel *Label13 = new QLabel(tr("Length/diameter of luminaire")); 
    QLabel *Label14 = new QLabel(tr("Width of luminaire"));
    QLabel *Label15 = new QLabel(tr("Height of luminaire"));
    QLabel *Label16 = new QLabel(tr("Length/diameter of luminous area"));
    QLabel *Label17 = new QLabel(tr("Width of luminous area"));
    QLabel *Label18 = new QLabel(tr("Height of luminous area C0-plane"));
    QLabel *Label19 = new QLabel(tr("Height of luminous area C90-plane"));
    QLabel *Label20 = new QLabel(tr("Height of luminous area C180-plane"));
    QLabel *Label21 = new QLabel(tr("Height of luminous area C270-plane"));
    QLabel *Label22 = new QLabel(tr("Downward flux fraction"));
    QLabel *Label23 = new QLabel(tr("Light output ratio of luminaire"));
    QLabel *Label24 = new QLabel(tr("Conversion factor for luminous intensities"));
    QLabel *Label25 = new QLabel(tr("Tilt of luminaire during measurement"));
    
    DoubleSpinBox13 = new QDoubleSpinBox;
    DoubleSpinBox14 = new QDoubleSpinBox;
    DoubleSpinBox15 = new QDoubleSpinBox;
    DoubleSpinBox16 = new QDoubleSpinBox;
    DoubleSpinBox17 = new QDoubleSpinBox;
	DoubleSpinBox18 = new QDoubleSpinBox;
	DoubleSpinBox19 = new QDoubleSpinBox;
	DoubleSpinBox20 = new QDoubleSpinBox;
	DoubleSpinBox21 = new QDoubleSpinBox;
	DoubleSpinBox22 = new QDoubleSpinBox;
	DoubleSpinBox23 = new QDoubleSpinBox;
	DoubleSpinBox24 = new QDoubleSpinBox;
	DoubleSpinBox25 = new QDoubleSpinBox;

	DoubleSpinBox13->setRange(1,9999);
	DoubleSpinBox14->setRange(0,9999);
	DoubleSpinBox15->setRange(0,9999);
	DoubleSpinBox16->setRange(0,9999);
	DoubleSpinBox17->setRange(0,9999);
	DoubleSpinBox18->setRange(0,9999);
	DoubleSpinBox19->setRange(0,9999);
	DoubleSpinBox20->setRange(0,9999);
	DoubleSpinBox21->setRange(0,9999);
	DoubleSpinBox22->setRange(0,100);
	DoubleSpinBox23->setRange(0,100);
	DoubleSpinBox24->setRange(0,10);
	DoubleSpinBox25->setRange(-180,180);
	
	DoubleSpinBox13->setDecimals(1);
	DoubleSpinBox14->setDecimals(1);
	DoubleSpinBox15->setDecimals(1);
	DoubleSpinBox16->setDecimals(1);
	DoubleSpinBox17->setDecimals(1);
	DoubleSpinBox18->setDecimals(1);
	DoubleSpinBox19->setDecimals(1);
	DoubleSpinBox20->setDecimals(1);
	DoubleSpinBox21->setDecimals(1);
	DoubleSpinBox25->setDecimals(1);	

	DoubleSpinBox13->setValue(ldt.dL);
	DoubleSpinBox14->setValue(ldt.dB);
	DoubleSpinBox15->setValue(ldt.dH);
	DoubleSpinBox16->setValue(ldt.dLa);
	DoubleSpinBox17->setValue(ldt.dB1);
	DoubleSpinBox18->setValue(ldt.dHC0);
	DoubleSpinBox19->setValue(ldt.dHC90);
	DoubleSpinBox20->setValue(ldt.dHC180);
	DoubleSpinBox21->setValue(ldt.dHC270);
	DoubleSpinBox22->setValue(ldt.dDFF);
	DoubleSpinBox23->setValue(ldt.dLORL);
	DoubleSpinBox24->setValue(ldt.dCFLI);
	DoubleSpinBox25->setValue(ldt.dTILT);
    
	QHBoxLayout *HLayout13 = new QHBoxLayout; 
	HLayout13->addWidget(Label13);          			
	HLayout13->addWidget(DoubleSpinBox13);	    
	HLayout13->addStretch(1);
	QHBoxLayout *HLayout14 = new QHBoxLayout; 
	HLayout14->addWidget(Label14);          			
	HLayout14->addWidget(DoubleSpinBox14);	    
	HLayout14->addStretch(1);
	QHBoxLayout *HLayout15 = new QHBoxLayout; 
	HLayout15->addWidget(Label15);          			
	HLayout15->addWidget(DoubleSpinBox15);	    
	HLayout15->addStretch(1);
	QHBoxLayout *HLayout16 = new QHBoxLayout; 
	HLayout16->addWidget(Label16);          			
	HLayout16->addWidget(DoubleSpinBox16);	    
	HLayout16->addStretch(1);
	QHBoxLayout *HLayout17 = new QHBoxLayout; 
	HLayout17->addWidget(Label17);          			
	HLayout17->addWidget(DoubleSpinBox17);	    
	HLayout17->addStretch(1);
	QHBoxLayout *HLayout18 = new QHBoxLayout; 
	HLayout18->addWidget(Label18);          			
	HLayout18->addWidget(DoubleSpinBox18);	    
	HLayout18->addStretch(1);
	QHBoxLayout *HLayout19 = new QHBoxLayout; 
	HLayout19->addWidget(Label19);          			
	HLayout19->addWidget(DoubleSpinBox19);	    
	HLayout19->addStretch(1);
	QHBoxLayout *HLayout20 = new QHBoxLayout; 
	HLayout20->addWidget(Label20);          			
	HLayout20->addWidget(DoubleSpinBox20);	    
	HLayout20->addStretch(1);
	QHBoxLayout *HLayout21 = new QHBoxLayout; 
	HLayout21->addWidget(Label21);          			
	HLayout21->addWidget(DoubleSpinBox21);	    
	HLayout21->addStretch(1);
	
	downwardPushButton = new QPushButton(tr("Calculate"));	
    connect(downwardPushButton, SIGNAL(clicked()), this, SLOT(downward()));			
	QHBoxLayout *HLayout22 = new QHBoxLayout; 
	HLayout22->addWidget(Label22);          			
	HLayout22->addWidget(DoubleSpinBox22);
	HLayout22->addWidget(downwardPushButton);	    
	HLayout22->addStretch(1);
	
	outputPushButton = new QPushButton(tr("Calculate"));	
    connect(outputPushButton, SIGNAL(clicked()), this, SLOT(output()));			
	QHBoxLayout *HLayout23 = new QHBoxLayout; 
	HLayout23->addWidget(Label23);          			
	HLayout23->addWidget(DoubleSpinBox23);
	HLayout23->addWidget(outputPushButton);			    
	HLayout23->addStretch(1);
	
	QHBoxLayout *HLayout24 = new QHBoxLayout; 
	HLayout24->addWidget(Label24);          			
	HLayout24->addWidget(DoubleSpinBox24);	    
	HLayout24->addStretch(1);
	QHBoxLayout *HLayout25 = new QHBoxLayout; 
	HLayout25->addWidget(Label25);          			
	HLayout25->addWidget(DoubleSpinBox25);	    
	HLayout25->addStretch(1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(HLayout13);
	mainLayout->addLayout(HLayout14);
	mainLayout->addLayout(HLayout15);
	mainLayout->addLayout(HLayout16);
	mainLayout->addLayout(HLayout17);
	mainLayout->addLayout(HLayout18);
	mainLayout->addLayout(HLayout19);
	mainLayout->addLayout(HLayout20);
	mainLayout->addLayout(HLayout21);
	mainLayout->addLayout(HLayout22);
	mainLayout->addLayout(HLayout23);
	mainLayout->addLayout(HLayout24);
	mainLayout->addLayout(HLayout25);
    mainLayout->addStretch(1);	       
    setLayout(mainLayout);	
    
    DoubleSpinBox13->setSuffix(" mm");
	DoubleSpinBox14->setSuffix(" mm");
	DoubleSpinBox15->setSuffix(" mm");
	DoubleSpinBox16->setSuffix(" mm");
	DoubleSpinBox17->setSuffix(" mm");
	DoubleSpinBox18->setSuffix(" mm");
	DoubleSpinBox19->setSuffix(" mm");
	DoubleSpinBox20->setSuffix(" mm");
	DoubleSpinBox21->setSuffix(" mm");
	DoubleSpinBox22->setSuffix(" %");
	DoubleSpinBox23->setSuffix(" %");
	DoubleSpinBox25->setSuffix(QString("%1").arg((char)176));
}

void EulumdatWidget2::downward() {

	double down = pldt->downward();
	
	if(down > 100) {
		QMessageBox::warning(this, "QLumEdit", tr("Downward flux fraction")
		+ QString(" = %1").arg(down)
		+ " -> " + tr("value is out of range") + "<br><br>"
		+ tr("Check distribution of luminous intesity to be in cd / 1000lm")
		);
		return;
	}	
	
	pldt->dDFF = down;
	DoubleSpinBox22->setValue(down);
}

void EulumdatWidget2::output() {

	double out = pldt->output();
	
	if(out > 100) {
		QMessageBox::warning(this, "QLumEdit", tr("Light output ratio of luminaire")
		+ QString(" = %1").arg(out)
		+ " -> " + tr("value is out of range") + "<br><br>"
		+ tr("Check distribution of luminous intesity to be in cd / 1000lm")
		);
		return;
	}
	
	pldt->dLORL = out;
	DoubleSpinBox23->setValue(out);
}
