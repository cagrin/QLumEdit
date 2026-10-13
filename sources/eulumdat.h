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
	QList<int> iNL;	// 26a
	QStringList sTL;
	QList<double> dTLF;
	QStringList sCA;
	QStringList sCRG;
	QList<double> dWB;  // 26f
	double dDR[10];
	QList<double> dC;
	QList<double> dG;
	QList<QList<double>> dLcd;
	int iMc = 0;


	QStringList loadFile(QString);
	int saveFile(QString);
	void calcMc();
	
	QStringList validate();
	double downward();
	double output();	
	QList<double> directRadios(QString);

	
private:
	double getDouble(QTextStream &);
	int getInt(QTextStream &);	
	
	double downwardForPlane(int i);
	double outputForPlane(int i);	
	
	double downwardDFL(double arc);
	double downwardForPlaneDFL(int i, double arc);	
};


#endif // __EULUMDAT_H__
