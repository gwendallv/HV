#include "MainApp.h"


//--------------------------------------------------------------
void MainApp::draw(){
    frame_begin_time = ofGetElapsedTimeMillis();

    // Paramètres pour la taille
    float initial_size = 1.1;
    float expansion = 0.5;

    // Paramètres pour toutes les anim
    int i,j;
    float elapsed_time=0.;
    // Remises à zéro
    for (int k=0; k<nb_cubes_x ; k++) {
        fill( (translate_z[k]).begin(), (translate_z[k]).end(), 0);
        fill( rotate_x[k].begin(), rotate_x[k].end(), 0);
        fill( rotate_y[k].begin(), rotate_y[k].end(), 0);
        fill( size_xyz[k].begin(), size_xyz[k].end(), 1);
    }

    /// Calcul des transformations sur les cubes de fond
    // Déplacement : rotation générale
    rotate_y_general += bc_d_theta * ( tempo/2.  /60.) * ofGetLastFrameTime() * volume; // 1 cycle tous les 2 temps
    bool bc_increment = false;
    if ( rotate_y_general >= bc_d_theta ) {
        rotate_y_general = fmodf(rotate_y_general, bc_d_theta);
        bc_increment = true;
    }

    // Impacts
    list<PercImpact>::iterator it = ElecDrumsImpacts.begin();
    while ( it != ElecDrumsImpacts.end() ) {

        if ( (frame_begin_time - (*it).timestamp) > (5*tau) ) {
            it = ElecDrumsImpacts.erase(it);
        }

        else { // Le trigger est toujours valable, on le traite
            if ( bc_increment )
                (*it).x = (*it).x -1;

            elapsed_time = (float)(frame_begin_time-(*it).timestamp);

            // ANIMATION EN IMPACT
            computePercImpact(it, elapsed_time);


            it++; // on ne fait it++ que là au cas (pas dans le cas où l'on supprime un trigger)
        }
    }


    /** Phase de dessin
     **/

    // copy enable part of gl state
    glPushAttrib(GL_ENABLE_BIT);

    // setup gl state
    glEnable(GL_DEPTH_TEST); //     ofEnableDepthTest();
    glEnable(GL_CULL_FACE);
    main_light.enable();


    ofClear(0,0,0, 255);

    post.begin(cam);

    /// Fond d'écran
    ofSetColor(120, 255);
    ofPushMatrix();
    glDisable(GL_DEPTH_TEST);
    ofTranslate(-width/2., -height/2.,0);
    //ofRect(0, 0, width, height);
    image_fond.draw(0,0, width, height);
    glEnable(GL_DEPTH_TEST);
    ofPopMatrix();

/// Axes
#ifdef HV_DEBUG
    ofDrawAxis(height);
#endif // HV_DEBUG



    /// Cubes de fond
    // Couleur pour commencer
    /*if ( post[1]->getEnabled() ) // Bloom à l'index 1
        ofSetColor( 220*color_factor, 169*color_factor, 0, 255 );
    else*/
        ofSetColor( 200, 149, 0, 255 );

    ofPushMatrix(); // origine sur caméra
    ofTranslate(0,0, width);

    int i_c,j_c; // compteurs centrés sur zéro (nb_cubes_x/y sont impairs)
    for ( i=0 ; i<nb_cubes_x ; i++) {
        i_c = i - (nb_cubes_x-1)/2;
        for ( j=0; j<nb_cubes_y; j++) {
            j_c = j - (nb_cubes_y-1)/2;
            ofPushMatrix(); // origine au centre du mini-cube + modifs impacts

            // déplacement = translation simple
            //ofTranslate( back_cubes_spacing*(i+0.5), back_cubes_spacing*(j+0.5), 0 );

            // Déplacement des cubes sur une sphère
            ofRotateX( -(float)(j_c)*bc_d_theta*180./M_PI);
            //ofRotate((float)(i_c)*bc_d_theta*180./M_PI, 0, cos((float)(j_c)*bc_d_theta), -sin((float)(j_c)*bc_d_theta));
            ofRotateY( (-(float)(i_c)*bc_d_theta + rotate_y_general )*180./M_PI);
            ofTranslate(0,0,-width);

            if ( (back_anim_type==ANIM_IMPACT) || (back_anim_type==ANIM_ALL) ) { // Impact
                ofTranslate( 0, 0, translate_z[i][j] );
                ofRotateX(rotate_x[i][j]*180./M_PI);
                ofRotate(rotate_y[i][j]*180./M_PI, 0, cos(rotate_x[i][j]), -sin(rotate_x[i][j]));
            }
            if ( (back_anim_type==ANIM_FULL_SCREEN) || (back_anim_type==ANIM_ALL) ) {// Taille de tous les cubes
                size_xyz[i][j] = initial_size;
                if ( size_xyz[i][j] > (initial_size+expansion) ) // saturation +
                    size_xyz[i][j] = initial_size+expansion;
                else if ( size_xyz[i][j] < (initial_size-expansion) ) // saturation -
                    size_xyz[i][j] = initial_size-expansion;
                ofScale(size_xyz[i][j], size_xyz[i][j], size_xyz[i][j]);
            }

            back_cube.draw();
            ofPopMatrix();  // origine au centre du mini-cube + modifs impacts
        }
    }

    ofPopMatrix(); // origine sur caméra


    /// Modèle avant
    ofPushMatrix(); // repère type blender
    ofTranslate(front_model_pos);
    ofRotateX(90.0);

    (front_model.getAnimation(0)).setPosition( (front_model.getAnimation(0)).getPosition() +0.005 );
    front_model.update();
    front_model.drawFaces();

    ofPopMatrix(); // repère type blender





    // set gl state back to original
    glPopAttrib();
    post.end(); // end scene and draw


    glPushAttrib(GL_ENABLE_BIT);

    /// Infos diverses
#ifdef HV_DEBUG
    ofSetColor(255, 255, 255, 255);
    Courier_New.drawString(ofToString(ofGetFrameRate()) + " fps",   15, height-30);
#endif // HV_DEBUG

    glPopAttrib();





}

