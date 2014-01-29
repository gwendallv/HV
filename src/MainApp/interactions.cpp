#include "../MainApp.h"

void MainApp::keyPressed(int key){
    switch( key ) {

        /*case (0 +320) : // offset de 320 entre l'index de touche et le chiffre inscrit
            back_anim_type = ANIM_FULL_SCREEN;
            break;

        case (1 +320) :
            back_anim_type = ANIM_IMPACT;
            break;

        case (2 +320) :
            back_anim_type = ANIM_ALL;
            break;*/

        case 98 : // B
            break;

        case 102 : // F
            if ( ofGetWindowMode()==OF_WINDOW ) {
                ofSetFullscreen(1);
                setupReal(ofGetScreenWidth());
            }
            else if ( ofGetWindowMode()==OF_FULLSCREEN ) {
                ofSetFullscreen(0);
                setupReal(WINDOW_WIDTH);
            }
            break;

        case 116 : // T
            cout << "Entrez la nouvelle valeur de tau [en ms] ; " << endl;
            cin >> tau; cout << endl;
            break;

        case 113 : // Q
            this->exit();
            std::exit(0);
            break;

        case 115 : // S
            setup();
            break;

        case OF_KEY_UP :
            glow_amount += 0.0001953125;
            itg::BloomPass::last_bloom_instance->setBlurSize(glow_amount);
            cout << "Rayon de glow : " << glow_amount << endl;
            /*color_factor += 0.05;
            cout << "Facteur de couleur : " << color_factor << endl;*/
            break;

        case OF_KEY_DOWN :
            glow_amount -= 0.0001953125;
            itg::BloomPass::last_bloom_instance->setBlurSize(glow_amount);
            cout << "Rayon de glow : " << glow_amount << endl;
            /*color_factor -= 0.05;
            cout << "Facteur de couleur : " << color_factor << endl;*/
            break;
    }

/// Choix de l'effet post processing
unsigned idx = key - 320;
if (idx < post.size()) post[idx]->setEnabled(!post[idx]->getEnabled());

}



//--------------------------------------------------------------
void MainApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void MainApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void MainApp::dragEvent(ofDragInfo dragInfo){

}
