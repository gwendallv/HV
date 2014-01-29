#include "MainApp.h"

/** Constructeur tous paramètres en direct **/
PercImpact::PercImpact(unsigned long long timestamp, int type, int x, int y, int rayon, float amplitude, int type_impact) {
    this->timestamp = timestamp;
    this->type = type;
    this->x = x;
    this->y = y;
    this->rayon = rayon;
    this->amplitude = amplitude;
    this->type_impact = type_impact;
}


/** Constructeur avec amplitude dans le message OSC **/
PercImpact::PercImpact(unsigned long long timestamp, int type, int midi_amplitude) {
    this->timestamp = timestamp;
    this->type = type;



    switch( type ) {
    case KICK :
        /* Positionnement vers le centre de l'écran, avec une
         * petite composante de déplacement aléatoire */
        type_impact = HV_IMPACT;
        rayon = nb_max_cubes_impact;
        x = floor((float)(nb_cubes_x)/2.) + round( ofRandom(-(float)(nb_max_cubes_impact)/1.41, (float)(nb_max_cubes_impact)/1.41) );
        y = floor((float)(nb_cubes_y)/2.) + round( ofRandom(-(float)(nb_max_cubes_impact)/3., (float)(nb_max_cubes_impact)/3.) );
        amplitude = -10.*(float)(midi_amplitude)/127.;
        break;

    case SNARE :
        type_impact = HV_IMPACT;
        rayon = floor( (float)(nb_max_cubes_impact)/1.41);
        x = floor(ofRandom(rayon+2., nb_cubes_x-rayon-1.00001));
        y = floor(ofRandom(rayon+2., nb_cubes_y-rayon-1.00001));
        amplitude = 7.*(float)(midi_amplitude)/127.;
        break;

    case CLAP :
        type_impact = HV_IMPACT;
        rayon = floor( (float)(nb_max_cubes_impact)/2);
        x = floor(ofRandom(rayon+2., nb_cubes_x-rayon-1.00001));
        y = floor(ofRandom(rayon+2., nb_cubes_y-rayon-1.00001));
        amplitude = 8.*(float)(midi_amplitude)/127.;
        break;



    case HIHAT1 :
        type_impact = HV_IMPACT;
        rayon = 1;
        x = floor(ofRandom(rayon+2., nb_cubes_x-rayon-1.00001));
        y = 2 + ceil(ofRandom(0.,(float)(nb_cubes_y))/10. );
        amplitude = -4.*(float)(midi_amplitude)/127.;
        break;

    case HIHAT2 :
        type_impact = HV_IMPACT;
        rayon = 1;
        x = floor(ofRandom(rayon+2., nb_cubes_x-rayon-1.00001));
        y = 2 + ceil(ofRandom(0.,(float)(nb_cubes_y))/10. );
        amplitude = 4.*(float)(midi_amplitude)/127.;
        break;



    case TOM1 :
        type_impact = HV_VIBRATION;
        rayon = floor( (float)(nb_max_cubes_impact)/2.2);
        x = 3+ ceil(ofRandom((float)(rayon), (float)(rayon)*1.5));
        y = nb_cubes_y -2 - floor(ofRandom((float)(rayon), (float)(rayon)*1.5));
        amplitude = (float)(midi_amplitude)/127.;
        break;

    case TOM2 :
        type_impact = HV_VIBRATION;
        rayon = ceil( (float)(nb_max_cubes_impact)/1.6);
        x = floor(ofRandom((float)(rayon)*3., (float)(rayon)*3.5));
        y = nb_cubes_y -2 - floor(ofRandom((float)(rayon), (float)(rayon)*1.5));
        amplitude = (float)(midi_amplitude)/127.;
        break;

    case TOM3 :
        type_impact = HV_VIBRATION;
        rayon = ceil( (float)(nb_max_cubes_impact)/1.2);
        x = nb_cubes_x -2 - floor(ofRandom((float)(rayon), (float)(rayon)*1.5));
        y = nb_cubes_y -2 - floor(ofRandom((float)(rayon), (float)(rayon)*1.5));
        amplitude = (float)(midi_amplitude)/127.;
        break;
    }

}

/* Initialisation dégueulasse des variables statiques... */
int PercImpact::nb_max_cubes_impact = 0;
int PercImpact::nb_cubes_x = 0;
int PercImpact::nb_cubes_y = 0;
