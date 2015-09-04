#include "rit128x96x4.h"
#include "drawLine.h"

#define NUM_ROWS 96
#define NUM_COLS 128

unsigned char bitmap[NUM_ROWS][NUM_COLS / 2];

//*************RIT128x96x4_ClearImage************************************** 
// Clears the RAM version of the image 
// Inputs: none 
// Outputs: none 
void RIT128x96x4_ClearImage(void){
	int row;
	int col;
	for (row = 0; row < NUM_ROWS; ++row) {
		for (col = 0; col < NUM_COLS / 2; ++col) {
			bitmap[row][col] = 0;
		}
	}
} 

void RIT128x96x4_WhiteImage(void){
	int row;
	int col;
	for (row = 0; row < NUM_ROWS; ++row) {
		for (col = 0; col < NUM_COLS / 2; ++col) {
			bitmap[row][col] = 15;
		}
	}
	
	
}
//*************RIT128x96x4_Line******************************************** 
// Draws one line in the RAM version of the image 
// Inputs: (x1,y1) is the start point 
// (x2,y2) is the end point 
// color is 0 (off) to 15 (white) 
// coordinates range from 0 to MAX, 0 is bottom or left, MAX is top or right 
// Outputs: none 
void RIT128x96x4_Line(int x1, int y1, int x2, int y2, unsigned char color){
//best way to do this will be bitmap
	int column_offset;
	int dx = x2 - x1;
	int dy = y2 - y1;
	int directionh = x1 < x2 ? 1 : -1;
	int directionv;
	int end_yoff;
	int current_yoff;
	if (dx != 0) {
		for (column_offset = 0; column_offset != dx; column_offset += directionh) {		
	  	current_yoff = dy*column_offset/dx;
			end_yoff = dy*(column_offset +1)/dx;
			
			directionv = current_yoff < end_yoff ? 1 : -1;
			for ( ; current_yoff != end_yoff; current_yoff += directionv) {
				setPixel(column_offset + x1, current_yoff + y1, color);
			}
			setPixel(column_offset + x1, end_yoff + y1, color);
		}
		setPixel(x2, y2,color);
	} else {
		directionv = y1 < y2 ? 1 : -1;
		for ( ; y1 != y2; y1 += directionv) {
			setPixel(x1, y1,color);
		}
		setPixel(x2, y2, color);
	}
}

void setPixel(int x, int y, unsigned char color) {
	unsigned char bit;
	
	bit = bitmap[NUM_ROWS - y - 1][x/2];
	if (x % 2 == 0) {
		bit = bit & 0x0F;
		bit |= (color << 4);
	} else {
		bit = bit & 0xF0;
		bit |= (color);
	}
	
	bitmap[NUM_ROWS-y -1][x/2] = bit;
	
}

//*************RIT128x96x4_ShowImage************************************** 
// Copies the RAM version of the image to the OLED 
// Inputs: none 
// Outputs: none 
void RIT128x96x4_ShowImage(void){
	RIT128x96x4ImageDraw(*bitmap, 0, 0, NUM_COLS, NUM_ROWS);
}

