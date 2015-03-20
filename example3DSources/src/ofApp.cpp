#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // INITIALIZE AUDIO VARIABLES
//    soundStream.setDeviceID(2);
    nOutputs = 2;
    nInputs = 0;
    sampleRate = 44100;
    bufferSize = 512;
    nBuffers = (nOutputs+nInputs)*2;
    
    // CHOOSE NUMBER OF SOURCES AND SET POSITION OF CENTRAL SPHERE
    numberOfSources = 3;
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
        frequency.push_back(ofRandom(100, 1000));
        oscillator.push_back(ofxHoaOsc());
        
    }
    
    /*INITIALIZE HOA VARIBLES. NOTE THAT FOR 3D SPACIALIZATION, THE NUMBER OF REQUIRED
    LOUDSPEAKERS EQUALS (ORDER+1)^2 AND LINE STORES 3 VALUES PER SOURCE */
    hoaOrder = 2;
    
    harmonic = new float[(hoaOrder+1)*(hoaOrder+1)];
    input = new float[numberOfSources];
    lineValue = new float[numberOfSources*3];
    
    encoder = new Encoder<Hoa3d, float>::Multi(hoaOrder, numberOfSources);
    
    // CHOOSE DECODER
//    decoder = new Decode r<Hoa3d, float>::Regular(hoaOrder, nOutputs);
    decoder = new Decoder<Hoa3d, float>::Binaural(hoaOrder);
    decoder->computeRendering(bufferSize);
    line = new PolarLines<Hoa3d, float>(numberOfSources);
    line->setRamp(44100/50);
    
    // SET CAM POSITION
    cam.setPosition(ofGetWidth()/2, ofGetHeight()/2, 1000);
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
        
        relativePosition = position[i] - centralSpherePosition;
        
        line->setAzimuth(i, Math<float>::azimuth(relativePosition.x, relativePosition.y,
                                                 relativePosition.z));
        line->setElevation(i, Math<float>::elevation(relativePosition.x, relativePosition.y, relativePosition.z));
        line->setRadius(i, Math<float>::radius(relativePosition.x, relativePosition.y)*0.01);
        
        
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
   
    for (int i = 0; i<bufferSize; i++) {
        line->process(lineValue);
        
        for (int j = 0; j<numberOfSources; j++) {
            input[j] = oscillator[j].triangle(frequency[j])/numberOfSources *0.4;
            encoder->setRadius(j, lineValue[j]);
            encoder->setAzimuth(j, lineValue[j+numberOfSources]);
            encoder->setElevation(j, lineValue[j+numberOfSources*2]);
        }
        encoder->process(input, harmonic);
        decoder->process(harmonic, output+i*nChannels);
    }
}

//--------------------------------------------------------------

void ofApp::exit(){
    
    // CLOSE AUDIO AND CLEAR OBJECTS WHEN CLOSING
    
    soundStream.close();
    
    delete [] harmonic;
    delete [] input;
    delete [] lineValue;
    delete line;
    delete encoder;
    delete decoder;
    sphere.clear();
    position.clear();
    velocity.clear();
    seed.clear();
    sphereColor.clear();
    frequency.clear();
    oscillator.clear();
    
}
