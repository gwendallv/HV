#include "../MainApp.h"


/* Fonction qui permet de refaire un setup complet, avec d'autres paramètres,
 * pendant le déroulement du programme
 *
 * Si les paramètres ne sont pas transmis, ou sont transmis à -1, on utilise
 * les paramètres actuels.
 */
void MainApp::setupReal(int _width, int _nb_cubes_x, bool first_init ){

    /// Paramètres d'entrée
    if (_width!=-1)
        width = _width;
    if (_nb_cubes_x!=-1)
        nb_cubes_x = _nb_cubes_x;


    /// Préparation de l'écran
    screen_ratio = (float)(ofGetScreenWidth())/ofGetScreenHeight();
    height = ceil(width/screen_ratio);
    cout << "Ratio d'ecran : " << screen_ratio << endl;
    cout << "Taille de la fenetre initiale (et du fond 3D pour z=0) : " << width << "px par " << height << "px" << endl;
    ofSetWindowShape(width, height);
    ofSetFrameRate(60);
#ifdef HV_DEBUG
	ofSetFrameRate(60);
#endif // HV_DEBUG
	ofSetWindowTitle("Harmonie visuelle");

	/// OSC
	cout << "Ecoute du port " << PORT << " pour l'OSC" << endl;
	receiver.setup(PORT);

	/// Graine du générateur aléatoire
	srand(time(NULL));


    /// Trucs graphiques
	ofBackground(0);
	image_fond.loadImage("fond_psyche.jpg");
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
    cout << "Fov : " << cam.getFov() << "deg" << endl;

    /* Création du mesh avant animé
     * /!\ recopie débile de l'exemple donné
     */
    ofDisableArbTex(); // we need GL_TEXTURE_2D for our models coords.
    front_model.loadModel("bidon.dae", true);
    front_model.setPosition(0,0 , 200);
    front_model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    front_model.playAllAnimations();


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
        post.createPass<EdgePass>()->setEnabled(false);
        post.createPass<ToonPass>()->setEnabled(false);
        post.createPass<GodRaysPass>()->setEnabled(false);
        post.createPass<LimbDarkeningPass>()->setEnabled(false);

        glow_amount = 0.0006; // = 0.001953125; par défaut
        itg::BloomPass::last_bloom_instance->setBlurSize(glow_amount);
    }

    /// Paramètres d'animation
    back_anim_type = ANIM_ALL;
    tau = 250;

    disto_max = 0.6;
    nb_max_cubes_impact = ceil( (float)(nb_cubes_x)/6. );
        PercImpact::nb_max_cubes_impact = nb_max_cubes_impact;

    tempo = 120.;
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
