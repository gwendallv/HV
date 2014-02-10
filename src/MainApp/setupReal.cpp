#include "../MainApp.h"

#include <fstream>


/* Fonction qui permet de refaire un setup complet, avec d'autres paramètres,
 * pendant le déroulement du programme
 *
 * Si les paramètres ne sont pas transmis, ou sont transmis à -1, on utilise
 * les paramètres actuels.
 *
 * On lit également le fichier texte setup.hv qui va contenir certains paramètres.
 * Tout les paramètres seront normalement initialisés et ceux présents dans ce
 * fichier seront actualisés.
 */
void MainApp::setupReal(bool first_init){

    /// Lecture des paramètres - setup.hv
    float front_model_size;
    string front_model_name;

    ifstream setup_file("hvsetup.txt", ios::in );
    if ( !setup_file )
        cerr << "Impossible d'ouvrir setup.hv" << endl;
    else {

        string temp_setup;

        while( !setup_file.eof() ) {
            setup_file >> temp_setup;
            if ( !temp_setup.empty() ) {
                if ( temp_setup[0] != '#' ) {

                    // Général
                    if ( temp_setup.compare("width") == 0 )
                        setup_file >> width;
                    else if ( temp_setup.compare("osc_port") == 0 )
                        setup_file >> osc_port;
                    else if ( temp_setup.compare("release_framerate") == 0 )
                        setup_file >> release_framerate;
                    else if ( temp_setup.compare("debug_framerate") == 0 )
                        setup_file >> debug_framerate;

                    // Image de fond
                    else if ( temp_setup.compare("path_image_fond") == 0 )
                        setup_file >> path_image_fond;

                    // Cubes de fond
                    else if ( temp_setup.compare("nb_cubes_x") == 0 )
                        setup_file >> nb_cubes_x;
                    else if ( temp_setup.compare("ratio_nb_max_cubes_impact") == 0 ) {
                        float ratio_nb_max_cubes_impact;
                        setup_file >> ratio_nb_max_cubes_impact;
                        nb_max_cubes_impact = ceil( (float)(nb_cubes_x)/ratio_nb_max_cubes_impact );
                    }
                    else if ( temp_setup.compare("tau") == 0 )
                        setup_file >> tau;
                    else if ( temp_setup.compare("disto_max") == 0 )
                        setup_file >> disto_max;

                    // Effets shaders
                    else if ( temp_setup.compare("glow_amount") == 0 )
                        setup_file >> glow_amount; // = 0.001953125; par défaut

                    // Front model
                    else if ( temp_setup.compare("front_model_name") == 0 )
                        setup_file >> front_model_name;
                    else if ( temp_setup.compare("front_model_size") == 0 )
                        setup_file >> front_model_size;
                    else if ( temp_setup.compare("front_model_pos") == 0 )
                        setup_file >> front_model_pos.x >> front_model_pos.y >> front_model_pos.z ;

                }
            }
        }

        setup_file.close();
    }




    /// Préparation de l'écran
    if (ofGetWindowMode()==OF_FULLSCREEN)
        width = ofGetWindowWidth();
    screen_ratio = (float)(ofGetScreenWidth())/ofGetScreenHeight();
    height = ceil(width/screen_ratio);
    cout << "Ratio d'ecran : " << screen_ratio << endl;
    cout << "Taille de la fenetre initiale (et du fond 3D pour z=0) : " << width << "px par " << height << "px" << endl;
    ofSetWindowShape(width, height);
    ofSetFrameRate(release_framerate);
#ifdef HV_DEBUG
	ofSetFrameRate(debug_framerate);
#endif // HV_DEBUG
	ofSetWindowTitle("Harmonie visuelle");

	/// OSC
	receiver.setup(osc_port);

	/// Graine du générateur aléatoire
	srand(time(NULL));


    /// Trucs graphiques
	ofBackground(0);
	image_fond.loadImage(path_image_fond);
	Courier_New.loadFont("cour.ttf", 15);

    /* Conventions ofEasyCam
     * z sera la profondeur (positifs "de notre côté de l'écran")
     * x vers la droite, y vers le haut
     * Les coordonnées pour le placement 2D seront alors , dans le repère d'origine de la caméra :
     *      x in [ -width/2 ; +width/2 ]
     *      y in [ -height/2 ; +height/2 ]
     *      z = 0
     * Ceci est dû au Fov de 60° par défaut : (largeur à z=0) == (éloignement z de la caméra à l'origine)
     *
     * MAIS il se passe des choses bizarres en plein écran... Donc on a 2 ratios entre la "distance caméra"
     * qui est une valeur étrange, et la hauteur/largeur.
     * hauteur/distance_cam = 1.1547 pour écran 1920*1080
     * largeur/distance_cam = 2.0528 pour écran 1920*1080   (== 2.00000 ?????)
     */
    cam.setDistance((float)(width)/2.0528);
    cam.reset();



    /// Meshes
    // Création du mesh de cube arrière
    // Nombre de cubes fixé en largeur
    PercImpact::nb_cubes_x = nb_cubes_x;
    back_cubes_size = (float)(width)/(nb_cubes_x) /2.; // Taille moyenne du cube (moitié de taille max.)
    nb_cubes_y = floor((float)(height) * nb_cubes_x / (float)(width))  + 2; // Pour qu'il y en ait encore dehors
    nb_cubes_x += 2; // pour supprimer erreurs segmentation avec déplacement rotY
    if ( (nb_cubes_y%2) == 0)
        nb_cubes_y++; // Pour avoir un nb impair
    PercImpact::nb_cubes_y = nb_cubes_y;

    back_cube = ofMesh::box(back_cubes_size, back_cubes_size, back_cubes_size);

    back_cubes_spacing = back_cubes_size * 2;

    bc_d_theta = (cam.getFov()/(nb_cubes_x+2)) *M_PI /180.;

    /* Création du mesh avant animé
     * /!\ recopie débile de l'exemple donné
     */
    ofDisableArbTex(); // we need GL_TEXTURE_2D for our models coords.
    front_model.loadModel(front_model_name, true);
    front_model.setScale(front_model_size, front_model_size, front_model_size);
    front_model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    front_model.playAllAnimations();
    cout << front_model.getAnimationCount() << " animations chargees pour front_model" << endl;




    /// Lumières
    main_light.setPointLight();
    main_light.setAmbientColor( ofFloatColor(0.1, 0.1, 0.1) );
    main_light.setPosition(-width/3, width/4, width);
    main_light.lookAt( ofVec3f(0.,0.,0.) );

    main_light.setAttenuation();

    color_factor = 0.55; // compensation du bloom



    /// Effets
    // Setup post-processing chain
    post.init(width, height);
    if ( first_init ) {
        post.createPass<FxaaPass>()->setEnabled(false);
        post.createPass<BloomPass>()->setEnabled(true);
        post.createPass<DofPass>()->setEnabled(false);
        post.createPass<KaleidoscopePass>()->setEnabled(false);
        post.createPass<NoiseWarpPass>()->setEnabled(false);
        post.createPass<PixelatePass>()->setEnabled(false);
#ifdef HV_DEBUG
        post.createPass<EdgePass>()->setEnabled(false);
#else
        post.createPass<EdgePass>()->setEnabled(true);
#endif // HV_DEBUG
        post.createPass<ToonPass>()->setEnabled(false);
        post.createPass<GodRaysPass>()->setEnabled(false);
        post.createPass<LimbDarkeningPass>()->setEnabled(false);
    }
    itg::BloomPass::last_bloom_instance->setBlurSize(glow_amount);

    /// Paramètres d'animation
    back_anim_type = ANIM_ALL;
    PercImpact::nb_max_cubes_impact = nb_max_cubes_impact;

    tempo = 0.;
    rotate_y_general = 0.;

    translate_z.resize(nb_cubes_x);
    rotate_x.resize(nb_cubes_x);
    rotate_y.resize(nb_cubes_x);
    size_xyz.resize(nb_cubes_x);
    for (int k=0; k<nb_cubes_x ; k++) {
        translate_z[k].resize(nb_cubes_y);
        rotate_x[k].resize(nb_cubes_y);
        rotate_y[k].resize(nb_cubes_y);
        size_xyz[k].resize(nb_cubes_y, 1);
    }







    cout << "===== Fin d'initialisation Harmonie Visuelle =====" << endl << endl << endl << endl;
}
