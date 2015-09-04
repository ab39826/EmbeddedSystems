#include <stdio.h>
#include "fixed.h"
#include "Output.h"

const struct outTestCase{       // used to test routines
  unsigned long InNumber;       // test input number
  char OutBuffer[10];           // Output String  
};

typedef const struct outTestCase outTestCaseType;
outTestCaseType outTests256[8]={ 
{     0,  "  0.00" }, //      0/256 = 0.00  
{     4,  "  0.01" }, //      4/256 = 0.01  
{   505,  "  1.97" }, //    505/256 = 1.97
{  1070,  "  4.17" }, //   1070/256 = 4.17
{  5120,  " 20.00" }, //   5120/256 = 20.00
{ 32768,  "128.00" }, //  32768/256 = 128
{255998,  "999.99" }, // 255998/256 = 999.99
{256000,  "***.**" }  // error
};

outTestCaseType outTests3[8]={
{-10000,  "**.***" },
{ -9999,  "-9.999" },
{ -1000,  "-1.000" },
{   -50,  "-0.050" },
{     0,  " 0.000" },
{   789,  " 0.789" },
{  9999,  " 9.999" },
{ 10000,  "**.***" }
};

outTestCaseType outTests2[8]={
{     0,  "  0.00" },
{     9,  "  0.09" },
{    10,  "  0.10" },
{   100,  "  1.00" },
{   999,  "  9.99" },
{  9999,  " 99.99" },
{ 99999,  "999.99" },
{100000,  "***.**" }
};

unsigned int Errors,AnError;
char Buffer[10];
void main(void){ // possible main program that tests your functions
  unsigned int i;
  Errors = 0;
  AnError = -1;
  Output_Init();
  Output_Color(15);
	printf("Input! Output Expect ");
	
	for(i=0; i<8; i++){
    Fixed_uDecOut2s(outTests2[i].InNumber,Buffer);
		printf("%6d %s %s ",outTests2[i].InNumber, Buffer, outTests2[i].OutBuffer);
    if(strcmp(Buffer, outTests2[i].OutBuffer) != 0){
      Errors++;
      AnError = i;
    }
  }
  if (AnError != -1) {
    printf("Error found in Fixed_sDecOut3s: %d", outTests2[AnError].InNumber);
	AnError = -1;
  }
  for(i=0; i<8; i++){
    Fixed_sDecOut3s(outTests3[i].InNumber,Buffer);
		printf("%6d %s %s ",outTests3[i].InNumber, Buffer, outTests3[i].OutBuffer);
    if(strcmp(Buffer, outTests3[i].OutBuffer) != 0){
      Errors++;
      AnError = i;
    }
  }
  if (AnError != -1) {
    printf("Error found in Fixed_sDecOut3s: %d", outTests3[AnError].InNumber);
	AnError = -1;
  }
  for(i=0; i<8; i++){
    Fixed_uBinOut8s(outTests256[i].InNumber, Buffer);
		printf("%6d %s %s ",outTests256[i].InNumber, Buffer, outTests256[i].OutBuffer);
	if(strcmp(Buffer, outTests256[i].OutBuffer) != 0) {
	  Errors++;
	  AnError = i;
	}
  }
  if (AnError != -1) {
    printf("Error found in Fixed_uBinOut8s: %d", outTests256[AnError].InNumber);
  }
  if (Errors == 0) {
    printf("All tests passed!");
  }
  
  for( ; ; ) {}
}