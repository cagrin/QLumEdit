#ifndef __MAINTABWIDGET_H__
#define __MAINTABWIDGET_H__

#include <QtWidgets>

#include "eulumdat.h"
#include "eulumdatwidget.h"
#include "eulumdatwidget2.h"
#include "eulumdatwidget3.h"
#include "eulumdatwidget4.h"
#include "eulumdattablewidget.h"
#include "renderwidget.h"

class QTabWidget;

class MainTabWidget : public QWidget
{
    Q_OBJECT

public:
    MainTabWidget(Eulumdat &ldt, QWidget *parent = 0);
    
    QTabWidget * tabWidget;
	EulumdatWidget * general;
	EulumdatWidget2 * luminaire;
	EulumdatWidget3 * lamps;
	EulumdatWidget4 * UFtable;
	EulumdatTableWidget * table;
	RenderWidget * render;	
};

#endif // __MAINTABWIDGET_H__
