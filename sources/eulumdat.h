#ifndef __EULUMDAT_H__
#define __EULUMDAT_H__

#include <QtWidgets>
#include <QString>
#include <QStringList>
#include <QTextStream>

class Eulumdat : public QWidget
{ 
    Q_OBJECT

public:
	Eulumdat();
	~Eulumdat();

	QString sIden;
	int iItyp;
	int iIsym;
	int iNc;
	double dDc;
	int iNg;
	double dDg;
	QString sMrn;
	QString sLnam;
	QString sLnum;
	QString sFnam;
	QString sDate;   // 12
	double dL;
	double dB;
	double dH;
	double dLa;
	double dB1;
	double dHC0;
	double dHC90;
	double dHC180;
	double dHC270;
	double dDFF; // 22
	double dLORL;
	double dCFLI;
	double dTILT;
	int iN;
	int * iNL;	// 26a
	QString * sTL;
	double * dTLF;
	QString * sCA;
	QString * sCRG;
	double * dWB;  // 26f
	double dDR[10];
	double * dC;
	double * dG;
	double ** dLcd;
	int iMc;


	QStringList loadFile(QString);
	int saveFile(QString);
	void calcMc();
	
	QStringList validate();
	double downward();
	double output();	
	double* directRadios(QString);

	
private:
	void Init(void);
	void Reset(void);
	
	double getDouble(QTextStream &);
	int getInt(QTextStream &);	
	
	double downwardForPlane(int i);
	double outputForPlane(int i);	
	
	double downwardDFL(double arc);
	double downwardForPlaneDFL(int i, double arc);	
};


#endif // __EULUMDAT_H__
