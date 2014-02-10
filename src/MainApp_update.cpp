#include "MainApp.h"


void MainApp::update(){
    ofxOscMessage m;

	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		receiver.getNextMessage(&m);


        if ( m.getAddress() ==  "/drums/snare" ) {
            ElecDrumsImpacts.push_back( PercImpact( ofGetElapsedTimeMillis(), SNARE, m.getArgAsFloat(0) ) );
		}
		else if ( m.getAddress() == "/drums/kick") {
            ElecDrumsImpacts.push_back( PercImpact( ofGetElapsedTimeMillis(), KICK, m.getArgAsFloat(0) ) );
		}
		else if ( m.getAddress() == "/drums/clap") {
            ElecDrumsImpacts.push_back( PercImpact( ofGetElapsedTimeMillis(), CLAP, m.getArgAsFloat(0) ) );
		}
		else if ( m.getAddress() == "/drums/hihat1") {
            ElecDrumsImpacts.push_back( PercImpact( ofGetElapsedTimeMillis(), HIHAT1, m.getArgAsFloat(0) ) );
		}
		else if ( m.getAddress() == "/drums/hihat2") {
            ElecDrumsImpacts.push_back( PercImpact( ofGetElapsedTimeMillis(), HIHAT2, m.getArgAsFloat(0) ) );
		}


		else if ( m.getAddress() == "/drums/tom1") {
            ElecDrumsImpacts.push_back( PercImpact( ofGetElapsedTimeMillis(), TOM1, m.getArgAsFloat(0) ) );
		}
		else if ( m.getAddress() == "/drums/tom2") {
            ElecDrumsImpacts.push_back( PercImpact( ofGetElapsedTimeMillis(), TOM2, m.getArgAsFloat(0) ) );
		}
		else if ( m.getAddress() == "/drums/tom3") {
            ElecDrumsImpacts.push_back( PercImpact( ofGetElapsedTimeMillis(), TOM3, m.getArgAsFloat(0) ) );
		}


		else if ( m.getAddress() == "/tempo") {
            tempo = m.getArgAsFloat(0);
		}
		else if ( m.getAddress() == "/peaks/master") {
            volume = m.getArgAsFloat(0);
		}


		else if (m.getAddress() == "/is_playing") {
            int is_playing = m.getArgAsInt32(0);
            if ( is_playing )
                post[IDX_EDGE_PASS]->setEnabled(false);
            else
                post[IDX_EDGE_PASS]->setEnabled(true);
		}

	}
}
