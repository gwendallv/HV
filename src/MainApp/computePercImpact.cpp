#include "../MainApp.h"

 void MainApp::computePercImpact(list< PercImpact >::iterator it, float elapsed_time ) {

    int nb_cubes_impact = (*it).rayon; // " rayon discret " de l'impact
    float r_impact = (nb_cubes_impact+0.3) * back_cubes_spacing; // le + 0.3 pour meilleur effet graphique (aucun cube "seul à la tangente")
    float decay_factor = exp(-elapsed_time/tau);



    /// === IMPACT EN PROFONDEUR === ///
    if ((*it).type_impact == HV_IMPACT) {
        /// Paramètres
        // splines pour les impacts
        float z_max = ((*it).amplitude)*back_cubes_spacing;
        float a=2*z_max/pow(r_impact,3);
        float b=-3*z_max/pow(r_impact,2);
        float c=0;
        float d=z_max;

        // temporaires
        float cur_r = 0.;
        float derivative=0.;

        /// Calcul des translations et rotations
        int delta_j = 0;
        for (int i=-nb_cubes_impact ; i<=nb_cubes_impact ; i++) {
            // Calcul des valeurs de j à parcourir à l'intérieur du cercle de rayon désiré
            delta_j = floor(sqrt(r_impact*r_impact - pow( abs(i)*back_cubes_spacing ,2)) /back_cubes_spacing);
            for (int j=-delta_j ; j<=delta_j ; j++) {
                cur_r = back_cubes_spacing * sqrt(i*i+j*j);
                translate_z[i+(*it).x][j+(*it).y] += (a*pow(cur_r,3)+b*pow(cur_r,2)+d) * decay_factor ;
                // /!\ PROBLEME AVEC LE CALCUL DE ATAN !!!! Imprécision trop forte du calcul de la dérivée (passe dans les négatifs au lieu positifs, ou inverse)
                derivative = 3.*a*cur_r*cur_r + 2.*b*cur_r;
                if ( abs(derivative) < 0.001 )
                    derivative=0.;
                rotate_x[i+(*it).x][j+(*it).y] += (-j* atan(derivative) /nb_cubes_impact) * decay_factor;
                rotate_y[i+(*it).x][j+(*it).y] += (-i* atan(derivative) /nb_cubes_impact) * decay_factor;
            }
        }

    }



    /// === VIBRATION === ///
    else if ((*it).type_impact == HV_VIBRATION) {

        // variables temporaires
        float cur_r=0.;
        float angle = 2*M_PI* elapsed_time * 0.010; // angle en rad, fréquence en kHz (car temps en ms)

        /// Calcul des rotations de vibration
        int delta_j = 0;
        for (int i=-nb_cubes_impact ; i<=nb_cubes_impact ; i++) {
            // Calcul des valeurs de j à parcourir à l'intérieur du cercle de rayon désiré
            delta_j = floor(sqrt(r_impact*r_impact - pow( abs(i)*back_cubes_spacing ,2)) /back_cubes_spacing);
            for (int j=-delta_j ; j<=delta_j ; j++) {
                cur_r = back_cubes_spacing * sqrt(i*i+j*j);

                rotate_x[i+(*it).x][j+(*it).y] += ((r_impact*1.1 -cur_r)/r_impact) * pow(-1, (abs(i))%3) * decay_factor * (*it).amplitude
                                                    * (M_PI/4) * sin(angle + (float)(i) + 100*(float)(j));
                rotate_y[i+(*it).x][j+(*it).y] += ((r_impact*1.1 -cur_r)/r_impact) * pow(-1, (abs(j))%3) * decay_factor * (*it).amplitude
                                                    * (M_PI/4) * sin(angle + (float)(j) + 100*(float)(i));;
            }
        }

    }

 }


