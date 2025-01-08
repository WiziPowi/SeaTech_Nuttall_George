#ifndef ROBOT_H
#define ROBOT_H
typedef struct robotStateBITS {
union {
struct {
unsigned char taskEnCours;
float vitesseGaucheConsigne;
float vitesseGaucheCommandeCourante;
float vitesseDroiteConsigne;
float vitesseDroiteCommandeCourante;
<<<<<<< HEAD
float distanceTelemetreGauche;
float distanceTelemetreExtrGauche;
float distanceTelemetreDroit;
float distanceTelemetreExtrDroit;
float distanceTelemetreCentre;
=======
>>>>>>> parent of 2493ede (fin code pdf presque)
};
};
} ROBOT_STATE_BITS;
extern volatile ROBOT_STATE_BITS robotState;
#endif /* ROBOT_H */