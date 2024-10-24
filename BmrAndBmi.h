#ifndef BMRANDBMI_H_INCLUDED
#define BMRANDBMI_H_INCLUDED

//Patrick Leimer 4/9/2023

void getMBMR(float W, float H, float A, float *bmr) { //mens
*bmr = 10*W + 6.25*H -5*A + 5 ;
}
void getWBMR(float W, float H, float A, float *bmr) { //womans
*bmr = 10*W + 6.25*H -5*A - 161 ;
}
//BMR Basal metabolic rate, amount of of energy expended while at rest. If one eats less calories than what it uses, it helps with loosing weight,
//the same thing applies to gaining, eat more than, and gain.

void getBMI(float W, float H, float *bmi) {
*bmi = W / ( (H/100)*(H/100) ) ;
}
//Body Mass Index
#include <ctype.h>
char sentenceCase(char *foodWord) {
    *foodWord = toupper( *foodWord );
    for(int i = 1; i < 15; i++) {
*(foodWord + i) = tolower( *(foodWord + i) );
    }
}
#endif // BMRANDBMI_H_INCLUDED
