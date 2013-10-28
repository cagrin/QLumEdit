#ifndef __VARS_H__
#define __VARS_H__

#include "mainwindow.h"

class Singleton {
public:

	MainWindow * mainWindow;
	
	int line, lineFNam, lineTL, lineCA, lineCRG;
	
	bool fill, legend;
	
	QString newFile;

private:
    Singleton();
    Singleton(const Singleton&);

    friend Singleton& Vars();
};

Singleton& Vars();

#endif // __VARS_H__
