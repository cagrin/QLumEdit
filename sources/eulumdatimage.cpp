#include <QtGui>
#include <math.h>

#include "eulumdat.h"

#include "vars.h"

void Eulumdat::saveImage(QString, int, int) {
	
    QVector<QPointF> vectorC0C180, vectorC90C270;
    double max;	
    
	vectorC0C180.clear();
	vectorC90C270.clear();
	
	int a, b;    
    double x=0, y=0;    
   	max = 0;   
    
    // C0
    if(iIsym != 3) {
		for(int j=0; j<iNg; j++) {
			x = dLcd[0][j] * cos(-(dG[j] * M_PI / 180.0) + M_PI_2);
			y = dLcd[0][j] * sin(-(dG[j] * M_PI / 180.0) + M_PI_2);		
	    	vectorC0C180.append(QPointF(x,y));
   		}
   		
   		for(int j=0; j<iNg; j++) {
   			if(max < dLcd[0][j])
   				max = dLcd[0][j];
	  	}
   		
   	}
   	else {
   		// poszukac C180 w katach, ktory jest taki sam jak C0	 
   		b=0;  		
   		while(dC[b] < 90.0) {
    		b++;
   		}
   		for(a=0; a<iMc; a++) {
   			if(dC[a+b] == 180.0) {
				break;
  			}
  		}
		for(int j=0; j<iNg; j++) {
			x = dLcd[a][j] * cos(-(dG[j] * M_PI / 180.0) + M_PI_2);
			y = dLcd[a][j] * sin(-(dG[j] * M_PI / 180.0) + M_PI_2);		
	    	vectorC0C180.append(QPointF(x,y));
   		}  		
   		
   		for(int j=0; j<iNg; j++) {
   			if(max < dLcd[a][j])
   				max = dLcd[a][j];
	  	}   		

  	}

   	
   	// C180
   	if( (iIsym == 1) || (iIsym == 3) || (iIsym == 4) ) {
   		for(int j=0; j<iNg; j++) {
   			x = vectorC0C180[iNg-1-j].x();
   			y = vectorC0C180[iNg-1-j].y();
			vectorC0C180.append(QPointF(-x,y));
	  	}
  	}
  	else {
  		// poszukac C180 w katach	
   		for(a=0; a<iMc; a++) {
   			if(dC[a] == 180.0) {
				break;
  			}
  		}
		for(int j=iNg-1; j>=0; j--) {
			x = dLcd[a][j] * cos(-(dG[j] * M_PI / 180.0) + M_PI_2);
			y = dLcd[a][j] * sin(-(dG[j] * M_PI / 180.0) + M_PI_2);		
	    	vectorC0C180.append(QPointF(-x,y));
   		}  		
   		
   		for(int j=0; j<iNg; j++) {
   			if(max < dLcd[a][j])
   				max = dLcd[a][j];
	  	}   		
 	}

   	
   	// C90
   	if(iIsym == 1) {
   		//vectorC90C270 = vectorC0C180;
  	}
  	else if(iIsym == 3) {
		for(int j=0; j<iNg; j++) {
			x = dLcd[0][j] * cos(-(dG[j] * M_PI / 180.0) + M_PI_2);
			y = dLcd[0][j] * sin(-(dG[j] * M_PI / 180.0) + M_PI_2);		
	    	vectorC90C270.append(QPointF(x,y));
   		}
   		
   		for(int j=0; j<iNg; j++) {
   			if(max < dLcd[0][j])
   				max = dLcd[0][j];
	  	}  		
 	}
 	else {
   		// poszukac C90
   		b=0;  		
   		for(a=0; a<iMc; a++) {
   			if(dC[a+b] == 90.0) {
				break;
  			}
  		}
		for(int j=0; j<iNg; j++) {
			x = dLcd[a][j] * cos(-(dG[j] * M_PI / 180.0) + M_PI_2);
			y = dLcd[a][j] * sin(-(dG[j] * M_PI / 180.0) + M_PI_2);		
	    	vectorC90C270.append(QPointF(x,y));
   		}  		
   		
   		for(int j=0; j<iNg; j++) {
   			if(max < dLcd[a][j])
   				max = dLcd[a][j];
	  	} 		
	}

    
    // C270
   	if(iIsym == 0) {
   		b=0;  		
   		for(a=0; a<iMc; a++) {
   			if(dC[a+b] == 270.0) {
				break;
  			}
  		}
		for(int j=iNg-1; j>=0; j--) {
			x = dLcd[a][j] * cos(-(dG[j] * M_PI / 180.0) + M_PI_2);
			y = dLcd[a][j] * sin(-(dG[j] * M_PI / 180.0) + M_PI_2);		
	    	vectorC90C270.append(QPointF(-x,y));
   		}  		
   		
   		for(int j=0; j<iNg; j++) {
   			if(max < dLcd[a][j])
   				max = dLcd[a][j];
	  	} 	   		
  	}
	if(iIsym == 3) {
   		b=0;  	
   		while(dC[b] < 90.0) {
    		b++;
   		}   			
   		for(a=0; a<iMc; a++) {
   			if(dC[a+b] == 270.0) {
				break;
  			}
  		}
		for(int j=iNg-1; j>=0; j--) {
			x = dLcd[a][j] * cos(-(dG[j] * M_PI / 180.0) + M_PI_2);
			y = dLcd[a][j] * sin(-(dG[j] * M_PI / 180.0) + M_PI_2);		
	    	vectorC90C270.append(QPointF(-x,y));
   		}  		
   		
   		for(int j=0; j<iNg; j++) {
   			if(max < dLcd[a][j])
   				max = dLcd[a][j];
	  	} 			
  	}
   	if( (iIsym == 2) || (iIsym == 4) ) {
   		for(int j=0; j<iNg; j++) {
   			x = vectorC90C270[iNg-1-j].x();
   			y = vectorC90C270[iNg-1-j].y();
			vectorC90C270.append(QPointF(-x,y));
	  	}   		
  	}  	    
}
