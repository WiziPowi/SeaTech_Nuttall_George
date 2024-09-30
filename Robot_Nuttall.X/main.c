/* 
 * File:   main.c
 * Author: TABLE 6
 *
 * Created on 30 septembre 2024, 09:47
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"

int main (void){
    
        InitOscillator();
        InitTimer1();
        InitTimer23();
        InitIO();
        
        LED_BLANCHE_1 = 0 ;
        LED_BLEUE_1 = 0;
        LED_ORANGE_1 = 0;
        LED_ROUGE_1 = 0;
        LED_VERTE_1 = 0;
        LED_BLANCHE_2 = 0;
        LED_BLEUE_2 = 0;
        LED_ORANGE_2 = 0;
        LED_ROUGE_2 = 0;
        LED_VERTE_2 = 0;
        
        while(1)
        {
        } // fin main
        }
