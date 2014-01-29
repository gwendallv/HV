#pragma once

#include "ofMain.h"

// Extensions core
#include "ofxOsc.h"
#include "ofxAssimpModelLoader.h"

// Ext. externes
#include "ofxPostProcessing.h"

#include "MainApp/defines.h"

#include "PercImpact.hpp"


class MainApp : public ofBaseApp {
	public:

        // Méthodes du moteur du programme
		void setup();
            void setupReal(int _width = -1, int _nb_cubes_x = -1, bool first_init=false);
		void update();
		void draw();
            void computePercImpact(list<PercImpact>::iterator it, float elapsed_time);

        // Méthodes héritées de OF
        void keyPressed(int key);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        // Utilitaires
		int getNbCubesX();
		int getNbCubesY();


    protected:

        /// Paramètres de communication
		ofxOscReceiver receiver;


        /// Paramètres d'affichage
        int width, height;
        float screen_ratio;
        ofTrueTypeFont Courier_New;

        // Instance de la caméra OF OpenGL
        ofEasyCam cam;
        float cam_z;

        // Maillages 3D
        ofMesh back_cube;
        float back_cubes_size, back_cubes_spacing;
        int nb_cubes_x, nb_cubes_y;
        float bc_d_theta; // radians

        ofxAssimpModelLoader front_model;

        // Lumières
        ofLight main_light;
        float color_factor;

        // Effets
        ofxPostProcessing post;
        bool compute_glow;
        float glow_amount;
        bool compute_bloom;

        // Fond
        ofImage image_fond;



        /// Paramètres d'animation
        long long frame_begin_time;
        list< PercImpact > ElecDrumsImpacts;

        int back_anim_type;
        int tau; // en ms
        float disto_max;
        int nb_max_cubes_impact;

        float tempo;
        float volume;

        float rotate_y_general;
        vector< vector<float> > translate_z;
        vector< vector<float> > rotate_x; // rad
        vector< vector<float> > rotate_y; // rad
        vector< vector<float> > size_xyz;

};


