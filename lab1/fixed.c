#include "fixed.h"
#include <stdio.h>

const char* stars = "***.**";
const char* stars2 = "**.***";

void Fixed_uDecOut2s(unsigned long n,  char *string) {

	if(n > 99999) {
		sprintf(string, "%s", stars);
	} else {
		sprintf(string, "%3d.%.2d", n/100, n%100);
	}
}

void Fixed_sDecOut3s(long n, char *string) {

	if (n > 9999 || n < -9999) {
		sprintf(string, "%s", stars2);
	} else {
		if(n < 0) {
		n= n*(-1);
			sprintf(string, "-%1d.%.3d", n/1000, n%1000);
		} else {
			sprintf(string, "%2d.%.3d", n/1000, n%1000);
		}
		
	}
}

void Fixed_uBinOut8s(unsigned long n,  char *string) {
	if(n >= 256000) {
		sprintf(string, "%s", stars);
	} else {
		n = n*100/256;
		sprintf(string, "%3d.%.2d", n/100, n%100);
	}
}