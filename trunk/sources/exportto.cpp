#include <QtWidgets>

#include "mainwindow.h"

#include "vars.h"
#include "eulumdatwidget.h"
#include "eulumdatwidget2.h"
#include "eulumdatwidget3.h"
#include "eulumdatwidget4.h"
#include "eulumdattablewidget.h"
#include "renderwidget.h"
#include "configtabdialog.h"
#include "validator.h"
#include "newdialog.h"

void MainWindow::exportToHTML() {
    QSettings settings("Cagrinlabs", "QLumEdit");
    QFileInfo info(settings.value("exportxhtml", QString()).toString());
    		    	
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), info.path()+"/"+QFileInfo(QString(curFile).remove(QFileInfo(curFile).completeSuffix())).fileName()+"html", tr("HTML file (*.html)"));
         
    if (fileName.isEmpty())
        return;     

	QFile file(fileName);  
      
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("QLumEdit"), tr("Cannot write file")
        	+ QString(" %1:\n%2.").arg(fileName).arg(file.errorString()));    
        return;
    }
    
	settings.setValue("exportxhtml", fileName);
    QTextStream out(&file);
    
    QString begin = ":&nbsp;&nbsp;";
    QString end = "<br />";
    QString sec = ")&nbsp;&nbsp;";
	QLocale locale;
	QString text;  
	int i, j;  
    
    out << "<?xml version=\"1.0\" encoding=\"Utf-8\" ?>\n"
"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"\n"
"\"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n"
"<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
"<head>\n"
"<title>" + QFileInfo(curFile).fileName() + " - QLumEdit</title>\n"
#ifdef WIN32
	"<meta http-equiv=\"Content_type\" content=\"text/xtml; charset=Windows-1250\"/>\n"
#else
	"<meta http-equiv=\"Content_type\" content=\"text/xtml; charset=Utf-8\"/>\n"
#endif
"</head>\n"
"<body>\n"
"<h3>" + QFileInfo(curFile).fileName() + "</h3>\n"
+ tr("Identification") + begin + ldt->sIden + end;

	QStringList indicator;
	indicator << tr("point source with symmetry about the vertical axis")
    	<< tr("linear luminaire")
   		<< tr("point source with any other symmetry");	
    out << tr("Type indicator") + begin + indicator[ldt->iItyp-1] + end;	

	QStringList symmetry;
	symmetry << tr("no symmetry")
		<< tr("symmetry about the vertical axis")
		<< tr("symmetry to plane C0-C180")
		<< tr("symmetry to plane C90-C270")
		<< tr("symmetry to plane C0-C180 and to plane C90-C270");	
		
    out << tr("Symmetry indicator") + begin + symmetry[ldt->iIsym] + end;
    
    out << tr("Number of C-planes")
    	+ begin + locale.toString(ldt->iNc) + end;

	text = tr("Distance between C-planes") + begin;
	if(ldt->dDc != 0.0) {
    	text += locale.toString(ldt->dDc) + QString("%1").arg((char)176) + end;
	}
	else {
    	text += tr("non-equidistantly") + end;
	}
    out << text;
    	
    out << tr("Number of luminous intensities in each C-plane")
    	+ begin + locale.toString(ldt->iNg) + end;
    	
	text = tr("Distance between luminous intensities per C-plane") + begin;
	if(ldt->dDg != 0.0) {
    	text += locale.toString(ldt->dDg) + QString("%1").arg((char)176) + end;
	}
	else {
    	text += tr("non-equidistantly") + end;
	}
    out << text;  

	out <<


tr("Measurement report number") + begin + ldt->sMrn + end
+ tr("Luminaire name") + begin + ldt->sLnam + end
+ tr("Luminaire number") + begin + ldt->sLnum + end
+ tr("File name") + begin + ldt->sFnam + end
+ tr("Date/user") + begin + ldt->sDate + end
+ end
+ tr("Length/diameter of luminaire") + begin + locale.toString(ldt->dL) + " mm" + end
+ tr("Width of luminaire") + begin + locale.toString(ldt->dB) + " mm" + end
+ tr("Height of luminaire") + begin + locale.toString(ldt->dH) + " mm" + end
+ tr("Length/diameter of luminous area") + begin + locale.toString(ldt->dLa) + " mm" + end
+ tr("Width of luminous area") + begin + locale.toString(ldt->dB1) + " mm" + end
+ tr("Height of luminous area C0-plane") + begin + locale.toString(ldt->dHC0) + " mm" + end
+ tr("Height of luminous area C90-plane") + begin + locale.toString(ldt->dHC90) + " mm" + end
+ tr("Height of luminous area C180-plane") + begin + locale.toString(ldt->dHC180) + " mm" + end
+ tr("Height of luminous area C270-plane") + begin + locale.toString(ldt->dHC270) + " mm" + end
+ tr("Downward flux fraction") + begin + locale.toString(ldt->dDFF) + " %" + end
+ tr("Light output ratio of luminaire") + begin + locale.toString(ldt->dLORL) + " %" + end
+ tr("Conversion factor for luminous intensities") + begin + locale.toString(ldt->dCFLI) + end
+ tr("Tilt of luminaire during measurement") + begin + locale.toString(ldt->dTILT) + QString("%1").arg((char)176) + end
+ end
+ tr("Number of standard sets of lamps") + begin + locale.toString(ldt->iN) + end;

	for(int i=0; i<ldt->iN; i++) {
out <<	
tr("Set") + begin + QString(" %1").arg(i+1) + end
+ tr("Number of lamps") + begin + locale.toString(ldt->iNL[i]) + end
+ tr("Type of lamps") + begin + ldt->sTL[i] + end
+ tr("Total luminous flux of lamps") + begin + locale.toString(ldt->dTLF[i]) + " lm" + end
+ tr("Color temperature of lamps") + begin + ldt->sCA[i] + end
+ tr("Color rendering index") + begin + ldt->sCRG[i] + end
+ tr("Wattage including ballast") + begin + locale.toString(ldt->dWB[i]) + " W" + end;
}
out << end;
out <<
tr("Utilization factors") + " / " + tr("Direct ratios") + begin + end
+ "(" + tr("k = 0.60") + sec + locale.toString(ldt->dDR[0]) + end
+ "(" + tr("k = 0.80") + sec + locale.toString(ldt->dDR[1]) + end
+ "(" + tr("k = 1.00") + sec + locale.toString(ldt->dDR[2]) + end
+ "(" + tr("k = 1.25") + sec + locale.toString(ldt->dDR[3]) + end
+ "(" + tr("k = 1.50") + sec + locale.toString(ldt->dDR[4]) + end
+ "(" + tr("k = 2.00") + sec + locale.toString(ldt->dDR[5]) + end
+ "(" + tr("k = 2.50") + sec + locale.toString(ldt->dDR[6]) + end
+ "(" + tr("k = 3.00") + sec + locale.toString(ldt->dDR[7]) + end
+ "(" + tr("k = 4.00") + sec + locale.toString(ldt->dDR[8]) + end
+ "(" + tr("k = 5.00") + sec + locale.toString(ldt->dDR[9]) + end
+ end;
out << tr("Luminous intensity") + begin + end;

out << "<table border=\"1\"><tbody>";
	out << "<tr>";
	out << "<td></td>";
	j=0;
    if(ldt->iIsym == 3) {
    	while(ldt->dC[j] < 90.0) {
    		j++;
   		}
   	}   	
	for(i=0; i<ldt->iMc; i++) {
		out << "<td>";
		out << QString("C%1%2").arg(locale.toString(ldt->dC[i+j])).arg((char)176);
		out << "</td>";
	}
	out << "</tr>";
	for(i=0; i<ldt->iNg; i++) {
		out << "<tr>";
		out << "<td>";
		out << QString("%1%2").arg(locale.toString(ldt->dG[i])).arg((char)176);
		out << "</td>";		
		
		for(j=0; j<ldt->iMc; j++) {
			out << "<td>";
			out << locale.toString(ldt->dLcd[j][i]);
			out << "</td>";
		}
		out << "</tr>";
	}
out << "</tbody></table>" + end;

	RenderWidget *render = new RenderWidget(*ldt, this);
	fileName.remove(QFileInfo(fileName).completeSuffix());

	fileName += "png";
out << tr("Diagram") + begin + end;	
out << "<img src=\"" + QFileInfo(fileName).fileName() + "\" alt=\"\" />" + end;

out << end + "<font size=\"-2\">" + tr("Generated in QLumEdit") + " (C) 2007 Krzysztof Strugi&#324;ski"
"</font>" + end;
out << end + "<img src=\"http://www.w3.org/Icons/valid-xhtml10\" alt=\"\" height=\"31\" width=\"88\" />"
+ end + end;
out <<
"</body>\n"
"</html>\n";
    
    out << flush;
    
	file.close();

	render->saveImage(fileName, 640);
	
    statusBar()->showMessage(tr("File exported to HTML/PNG"), 5000);
    	
	return;

}

void MainWindow::exportToIesna() {
    QSettings settings("Cagrinlabs", "QLumEdit");
    QFileInfo info(settings.value("exportiesna", QString()).toString());
    		    	
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), info.path()+"/"+QFileInfo(QString(curFile).remove(QFileInfo(curFile).completeSuffix())).fileName()+"ies", tr("Iesna LM-63-1995 file (*.ies)"));
         
    if (fileName.isEmpty())
        return;     

	QFile file(fileName);  
      
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("QLumEdit"), tr("Cannot write file")
        	+ QString(" %1:\n%2.").arg(fileName).arg(file.errorString()));    
        return;
    }
    
	settings.setValue("exportiesna", fileName);
    QTextStream out(&file);

	QLocale locale;
	int i, j;

	out << "IESNA:LM-63-1995" << endl;
	out << "[TEST] " << ldt->sMrn << endl;
	out << "[MANUFAC] " << ldt->sIden << endl;
	out << "[LUMCAT] " << ldt->sLnum << endl;
	out << "[LUMINAIRE] " << ldt->sLnam << endl;
	out << "[LAMP] " << ldt->sTL[0] << endl;	
	out << "TILT=NONE" << endl;
	out << QString("%1").arg(ldt->iNL[0]) + " "
	+ QString("%1").arg(ldt->dTLF[0] / ldt->iNL[0]) + " "
	+ QString("%1").arg(ldt->dCFLI) + " "
	+ QString("%1").arg(ldt->iNg) + " "
	+ QString("%1").arg(ldt->iMc) + " 1 2 "
	+ QString("%1").arg(ldt->dL / 1000) + " "
	+ QString("%1").arg(ldt->dB / 1000) + " "
	+ QString("%1").arg(ldt->dH / 1000) << endl;
	out << "1 1 " << QString("%1").arg(ldt->dWB[0]) << endl;

	for(i=0; i<ldt->iNg; i++) {
		out << QString("%1").arg(ldt->dG[i]) << " ";
	}		
	out << endl;
	j=0;
    if(ldt->iIsym == 3) {
    	while(ldt->dC[j] < 90.0) {
    		j++;
   		}
   	}   	
	for(i=0; i<ldt->iMc; i++) {
		out << QString("%1").arg(ldt->dC[i+j]) << " ";
	}	
	out << endl;
	
	for(int i=0; i<ldt->iMc; i++) {
		for(int j=0; j<ldt->iNg; j++) {
			out << QString("%1").arg(ldt->dLcd[i][j]) << " ";
		}
		out << endl;
	}	
	
	out << flush;    
	file.close();

    statusBar()->showMessage(tr("File exported to Iesna LM-63-1995"), 5000);

	return;
    
}

