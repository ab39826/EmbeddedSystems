#include <stdio.h>
#include "Output.h"
#include "drawLine.h"


int main(void){
  Output_Init();
  Output_Color(15);
  int color = 10;
  
  RIT128x96x4_ClearImage();
  RIT128x96x4_ShowImage();
  
  //vertical line, delay, clear
  RIT128x96x4_Line(64,0,64,90,color);
  RIT128x96x4_ShowImage();
  SysCtlDelay(8000000);           // delay ~2 sec at 12 MHz
  RIT128x96x4_ClearImage();
  RIT128x96x4_ShowImage();	
  SysCtlDelay(4000000);
  
    //horizontal line, delay, clear
  RIT128x96x4_Line(0,46,128,46,color);
  RIT128x96x4_ShowImage();
  SysCtlDelay(8000000);         
  RIT128x96x4_ClearImage();
  RIT128x96x4_ShowImage();	
  SysCtlDelay(4000000);
  
	//cross, delay, clear
  RIT128x96x4_Line(64,0,64,90,color);
  RIT128x96x4_Line(0,46,128,46,color);
  RIT128x96x4_ShowImage();
  SysCtlDelay(8000000);         
  RIT128x96x4_ClearImage();
  RIT128x96x4_ShowImage();	
  SysCtlDelay(4000000);
  
  
  
  
   RIT128x96x4_Line(96/2,128/2,20,90,color);
   RIT128x96x4_ShowImage();
  SysCtlDelay(8000000);         
  RIT128x96x4_ClearImage();
  RIT128x96x4_ShowImage();	
  SysCtlDelay(4000000);
  
  
  
  

}

