#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // ENABLE DEPTH TEST FOR 3D DRAWING WITH Z-AXIS
    ofEnableDepthTest();

//  soundStream.listDevices();
//  soundStream.setDeviceID(5);
    // INITIALIZE AUDIO VARIABLES

    nOutputs = 2;
    nInputs = 0;
    sampleRate = 44100;
    bufferSize = 512;
    nBuffers = (nOutputs+nInputs)*2;
    
    // CHOOSE NUMBER OF SOURCES AND SET POSITION OF CENTRAL SPHERE
    numberOfSources = 5;
    centralSphereRadius = 100;
    centralSpherePosition = ofVec3f(ofGetWidth()/2, ofGetHeight()/2, 0);
    centralSphere = ofSpherePrimitive(centralSphereRadius, 50);
    centralSphere.setPosition(centralSpherePosition);

    velMax = 10;
    
    // CREATE PARTICLES AND ASSOCIATED OSCILLATOR
    for (int i = 0; i<numberOfSources; i++) {
        
        sphere.push_back(ofSpherePrimitive(30,50));
        position.push_back(ofVec3f(ofRandomWidth(), ofRandomHeight(),0));
        velocity.push_back(ofVec3f(ofRandomf()*velMax, ofRandomf()*velMax,ofRandomf()*velMax));
        seed.push_back(ofVec3f(ofRandom(10000),ofRandom(10000),ofRandom(10000)));
        sphereColor.push_back(ofFloatColor(ofRandomuf(),ofRandomuf(),ofRandomuf()));
        
        oscillator.push_back(ofxHoaOscillator<float>());
        oscillator.back().setup(sampleRate, oscType(floor(ofRandom(4.9))));
        oscillator.back().setFrequency(ofRandom(100, 1000));
        
    }
    
    /*INITIALIZE HOA VARIBLES. NOTE THAT FOR 3D SPACIALIZATION, THE NUMBER OF REQUIRED
    LOUDSPEAKERS EQUALS (ORDER+1)^2 AND LINE STORES 3 VALUES PER SOURCE */
    hoaOrder = 3;
    
    harmonic = new float[(hoaOrder+1)*(hoaOrder+1)];
    input = new float[numberOfSources];
    memset(harmonic, 0, sizeof(float)*(hoaOrder+1)*(hoaOrder+1));
    memset(input, 0, sizeof(float)*numberOfSources);
    
    encoder = new Encoder<Hoa3d, float>::Multi(hoaOrder, numberOfSources);
    
    // CHOOSE DECODER
//    decoder = new Decoder<Hoa3d, float>::Regular(hoaOrder, nOutputs);
    decoder = new Decoder<Hoa3d, float>::Binaural(hoaOrder);
    decoder->computeRendering(bufferSize);
    
    // CHOOSE OPTIM
    optim = new Optim<Hoa3d, float>::Basic(hoaOrder);
//        optim = new Optim<Hoa3d, float>::InPhase(hoaOrder);

    // CREATE HOACOORD AND SET RAMP IN MILLISECONDS
    hoaCoord = new ofxHoaCoord<Hoa3d, float>(numberOfSources);
    hoaCoord->setRamp(50, sampleRate);
   
    // SET SPATIAL REFERENCE
    hoaCoord->setAmbisonicRadius(centralSphereRadius*4);
    hoaCoord->setAmbisonicCenter(centralSpherePosition);
    
    
    // INTIALIZE VALUES TO A DISTANT POINT TO AVOID CLICKS IN THE BEGINNING
    for (int i = 0; i<numberOfSources; i++) {
        hoaCoord->setSourcePositionDirect(i, ofVec3f(300,300,300));
    }
    
    // SET CAM POSITION
    cam.setPosition(ofGetWidth()/2, ofGetHeight()/2, 640);
    cam.setVFlip(true);
    cam.lookAt(ofVec3f(ofGetWidth()/2,ofGetHeight()/2,0));

    // SET LIGHT
    light.setDiffuseColor(ofColor(255));
    light.setPosition(0,0,10);
    light.lookAt(ofVec3f(ofGetWidth()/2, ofGetHeight(), 0));

    // INITIALIZE AUDIO
    soundStream.setup(this, nOutputs, nInputs, sampleRate, bufferSize, nBuffers);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    for (int i = 0; i<numberOfSources; i++) {
        
        seed[i]+=0.01;
        position[i]+=ofNoise(seed[i].x,seed[i].y,seed[i].z)*velocity[i];
        sphere[i].setPosition(position[i]);
        
        hoaCoord->setSourcePosition(i, position[i]);
        
        if (position[i].distance(centralSpherePosition) < centralSphereRadius +
                                                            sphere[i].getRadius()) {
            velocity[i].z *= -1;
        }
                
        if (position[i].x < 0 || position[i].x > ofGetWidth()) {
            velocity[i].x*= -1;
        }
        if (position[i].y < 0 || position[i].y > ofGetHeight()) {
            velocity[i].y*= -1;
        }
        if (position[i].z < -640 || position[i].z > 640) {
            velocity[i].z*= -1;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(0);

    cam.begin();
    light.enable();

    ofSetColor(ofColor::papayaWhip);

    centralSphere.draw();
    for (int i = 0; i<numberOfSources; i++) {

        ofSetColor(sphereColor[i]);

        sphere[i].draw();
    }

    cam.end();
    ofSetColor(ofColor::white);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels){

    for (int i = 0; i<bufferSize; i++) {
        
        hoaCoord->process();
        
        for (int j = 0; j<numberOfSources; j++) {
            input[j] = oscillator[j].tick()/numberOfSources * 0.5;
            
            encoder->setRadius(j, hoaCoord->getRadius(j));
            encoder->setAzimuth(j, hoaCoord->getAzimuth(j));
            encoder->setElevation(j, hoaCoord->getElevation(j));
        }

        encoder->process(input, harmonic);
        optim->process(harmonic, harmonic);
        decoder->process(harmonic, output+i*nChannels);
    }
}

//--------------------------------------------------------------

void ofApp::exit(){
    
    // CLOSE AUDIO AND CLEAR OBJECTS WHEN CLOSING
    
    soundStream.close();
    
    delete [] harmonic;
    delete [] input;
    delete hoaCoord;
    delete encoder;
    delete decoder;
    delete optim;
}
