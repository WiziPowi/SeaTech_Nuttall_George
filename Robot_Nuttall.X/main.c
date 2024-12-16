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
#include "PWM.h"
#include "ADC.h"
#include "robot.h"
#include "main.h"

unsigned int valueGauche;
unsigned int valueCentre;
unsigned int valueDroite;

int main(void) {

    InitOscillator();
    InitIO();
    InitTimer1();
    InitTimer23();
    InitTimer4();
    InitPWM();
    InitADC1();


    LED_BLANCHE_1 = 0;
    LED_BLEUE_1 = 0;
    LED_ORANGE_1 = 0;
    LED_ROUGE_1 = 0;
    LED_VERTE_1 = 0;
    LED_BLANCHE_2 = 0;
    LED_BLEUE_2 = 0;
    LED_ORANGE_2 = 0;
    LED_ROUGE_2 = 0;
    LED_VERTE_2 = 0;

    while (1) {
        if (ADCIsConversionFinished()) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [0])* 3.3 / 4096;
            robotState.distanceTelemetreExtrGauche = 34 / volts - 5;
            float volts = ((float) result [1])* 3.3 / 4096;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            volts = ((float) result [2])* 3.3 / 4096;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [3])* 3.3 / 4096;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result [4])* 3.3 / 4096;
            robotState.distanceTelemetreExtrDroit = 34 / volts - 5;
            
        };
        if (robotState.distanceTelemetreExtrGauche < 20){
            LED_BLANCHE_1 = 1;
        }
        else{
            LED_BLANCHE_1 = 0;
        }
        if (robotState.distanceTelemetreGauche < 20){
            LED_BLEUE_1 = 1;
        }
        else{
            LED_BLEUE_1 = 0;
        }
        if (robotState.distanceTelemetreCentre < 20){
            LED_ORANGE_1 = 1;
        }
        else{
            LED_ORANGE_1 = 0;
        }
        if (robotState.distanceTelemetreDroit < 20){
            LED_ROUGE_1 = 1;
        }
        else{
            LED_ROUGE_1 = 0;
        }
        if (robotState.distanceTelemetreExtrDroit < 20){
            LED_VERTE_1 = 1;
        }
        else{
            LED_VERTE_1 = 0;
        }
                
       

    } // fin main
}


unsigned char stateRobot;

void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;
        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;
        case STATE_AVANCE:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_RECULE:
            PWMSetSpeedConsigne(-30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-30, MOTEUR_GAUCHE);
            stateRobot = STATE_RECULE_EN_COURS;
            break;
        case STATE_RECULE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}
unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {
    unsigned char positionObstacle = PAS_D_OBSTACLE;
    //éDtermination de la position des obstacles en fonction des ééètlmtres
    if (robotState.distanceTelemetreDroit < 15 &&
            robotState.distanceTelemetreGauche < 15)
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreCentre < 20)
        positionObstacle = OBSTACLE_EN_FACE;
    
    else if (robotState.distanceTelemetreExtrGauche < 20)
        positionObstacle = OBSTACLE_EXTR_GAUCHE;
    
    else if (robotState.distanceTelemetreExtrDroit < 20)
        positionObstacle = OBSTACLE_EXTR_DROITE;
    
    else if (robotState.distanceTelemetreDroit < 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 30)
        positionObstacle = OBSTACLE_EXTR_DROITE;
    
    else if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche < 30)
        positionObstacle = OBSTACLE_EXTR_GAUCHE;
    
    else if (robotState.distanceTelemetreDroit < 50 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 30)
        positionObstacle = OBSTACLE_A_DROITE;
    
    else if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche < 50)
        positionObstacle = OBSTACLE_A_GAUCHE;
    
    else if (robotState.distanceTelemetreDroit > 50 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 50)
        positionObstacle = PAS_D_OBSTACLE;
    
    //éDtermination de lé?tat àvenir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_EXTR_DROITE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
    else if (positionObstacle == OBSTACLE_EXTR_GAUCHE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_RECULE;
    //Si l?on n?est pas dans la transition de lé?tape en cours
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;
}
