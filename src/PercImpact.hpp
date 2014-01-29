#ifndef PERCIMPACT_HPP_INCLUDED
#define PERCIMPACT_HPP_INCLUDED

// Types d'impacts

#define HV_IMPACT 0
#define KICK 0
#define SNARE 1
#define CLAP 2
#define HIHAT1 3
#define HIHAT2 4

#define HV_VIBRATION 1
#define TOM1 101
#define TOM2 102
#define TOM3 103
#define TOM4 104

/** Classe de stockage des messages OSC de percussions venant de M4L **/
class PercImpact {

    public :
        PercImpact(unsigned long long timestamp, int type, int x, int y, int rayon, float amplitude, int type_impact);
        PercImpact(unsigned long long timestamp, int type, int midi_amplitude);

        unsigned long long timestamp; // sera en ms
        int type;
        int type_impact;
        int x,y; // coordonnées de l'éventuel impact
        int rayon;
        float amplitude;

        // Sauvegardes du moteur de programme
        static int nb_max_cubes_impact;
        static int nb_cubes_x, nb_cubes_y;

};


#endif // PERCIMPACT_HPP_INCLUDED
