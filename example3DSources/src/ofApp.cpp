#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    soundStream.setDeviceID(2);
    nOutputs = 2;
    nInputs = 0;
    sampleRate = 44100;
    bufferSize = 512;
    nBuffers = (nOutputs+nInputs)*2;
    
    numberOfSources = 5;
    centralSphereRadius = 100;
    centralSpherePosition = ofVec3f(ofGetWidth()/2, ofGetHeight()/2, 0);
    centralSphere = ofSpherePrimitive(centralSphereRadius, 50);
    centralSphere.setPosition(centralSpherePosition);
    
    velMax = 10;
    
    for (int i = 0; i<numberOfSources; i++) {
        
        sphere.push_back(ofSpherePrimitive(30,50));
        position.push_back(ofVec3f(ofRandomWidth(), ofRandomHeight(),0));
        velocity.push_back(ofVec3f(ofRandomf()*velMax, ofRandomf()*velMax,ofRandomf()*velMax));
        seed.push_back(ofVec3f(ofRandom(10000),ofRandom(10000),ofRandom(10000)));
        sphereColor.push_back(ofFloatColor(ofRandomuf(),ofRandomuf(),ofRandomuf()));
        frequency.push_back(ofRandom(100, 1000));
        oscillator.push_back(ofxHoaOsc());
        
    }
    
    hoaOrder = 7;
    
    harmonic = new float[hoaOrder*2+1];
    input = new float[numberOfSources];
    lineValue = new float[numberOfSources*2];
    
//    encoder = new EncoderMulti<Hoa3d, float>(hoaOrder, numberOfSources);
//    decoder = new Decoder<Hoa3d, float>(hoaOrder, nOutputs);
//    line = new PolarLines<Hoa3d, float>(numberOfSources);
    
    cam.setPosition(ofGetWidth()/2, ofGetHeight()/2, 1000);
    cam.setVFlip(true);
    cam.lookAt(ofVec3f(ofGetWidth()/2,ofGetHeight()/2,0));

    light.setDiffuseColor(ofColor(255));
    light.setPosition(0,0,10);
    light.lookAt(ofVec3f(ofGetWidth()/2, ofGetHeight(), 0));
    soundStream.setup(this, nOutputs, nInputs, sampleRate, bufferSize, nBuffers);
}

//--------------------------------------------------------------
void ofApp::update(){
    for (int i = 0; i<numberOfSources; i++) {
        seed[i]+=0.01;
        position[i]+=ofNoise(seed[i].x,seed[i].y,seed[i].z)*velocity[i];
        sphere[i].setPosition(position[i]);
    
        if (position[i].distance(centralSpherePosition)<centralSphereRadius+30) {
            velocity[i]*= -1;
        }
        
        if (position[i].x < 0 || position[i].x > ofGetWidth()) {
            velocity[i].x*= -1;
        }
        if (position[i].y < 0 || position[i].y > ofGetHeight()) {
            velocity[i].y*= -1;
        }
        if (position[i].z < -500 || position[i].z > 500) {
            velocity[i].z*= -1;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetupScreen();
    ofBackground(0);
    ofEnableDepthTest();

    cam.begin();
    ofEnableLighting();
    light.enable();
    ofSetColor(255, 0, 0);

    centralSphere.draw();

    for (int i = 0; i<numberOfSources; i++) {

        ofSetColor(sphereColor[i]);

        sphere[i].draw();
    }
    cam.end();


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
    
}

//--------------------------------------------------------------

void ofApp::exit(){
    
    delete [] harmonic;
    delete [] input;
    delete [] lineValue;
//    delete [] line;
//    delete [] encoder;
//    delete [] decoder;
    sphere.clear();
    position.clear();
    velocity.clear();
    seed.clear();
    sphereColor.clear();
    frequency.clear();
    oscillator.clear();
    
}
