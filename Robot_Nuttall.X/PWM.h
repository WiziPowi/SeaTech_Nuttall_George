#ifndef PWM1_H
#define	PWM1_H


#define PWMPER 24.0
#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1

void InitPWM(void) ;
//void PWMSetSpeed(float vitesseEnPourcents,int numMoteur);
void PWMUpdateSpeed();
void PWMSetSpeedConsigne(float vitesseEnPourcents, unsigned char Moteur) ;

#endif	/* PWM1_H */

