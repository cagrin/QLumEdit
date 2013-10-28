#include <QtWidgets>

#include "maintabwidget.h"

MainTabWidget::MainTabWidget(Eulumdat &ldt, QWidget *parent) : QWidget(parent)
{
	tabWidget = new QTabWidget;
	general = new EulumdatWidget(ldt, this);
	luminaire = new EulumdatWidget2(ldt, this);
	lamps = new EulumdatWidget3(ldt, this);
	UFtable = new EulumdatWidget4(ldt, this);
	table = new EulumdatTableWidget(ldt, this);	
	render = new RenderWidget(ldt, this);
	
    tabWidget->addTab(general, tr("General"));
    tabWidget->addTab(luminaire, tr("Luminaire"));    
	tabWidget->addTab(lamps, tr("Lamps"));
	tabWidget->addTab(UFtable, tr("Utilization factors"));	
	tabWidget->addTab(table, tr("Luminous intensity"));	
	tabWidget->addTab(render, tr("Diagram"));	
	   
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);  
}

