#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //    UNCOMMENT THIS LINE TO PRINT AVALIABLE AUDIO DEVICES
//    ofSoundStreamListDevices();
    
    // USE THIS FUNCTION TO SET THE AUDIO DEVICE
    soundStream.setDeviceID(3);
    
    nOutputs = 2;
    nInputs = 0;
    sampleRate = 44100;
    bufferSize = 512;
    nBuffers = (nInputs+nOutputs)*2;
    
    //INITIALIZE SOUNDSTREAM
    soundStream.setup(this, nOutputs, nInputs, sampleRate, bufferSize, nBuffers);
    
    // CREATE BUFFER FOR SOUND INPUT
    inputBuffer = new float[bufferSize];
    for (int i = 0; i<bufferSize; i++) {
        inputBuffer[i] = 0;
    }
    
    // SETUP TEST OSCILATOR
    myOsc.setup(sampleRate);
    
    // SETUP HOA
    order = 3;
    harmonicsBuffer = new float[order*2+1];
    
    // THE ENCODER CREATES THE SPHERICAL HARMONICS
    hoaEncoder = new EncoderDC<Hoa2d, float>(order);
    
    // THE DECODER TRANSLATES THE HARMONICS INTO AUDIO SIGNALS FOR OUTPUT
    // NUMBER OF MINIMUM OUPUT CHANNELS  = ORDER*2+1
    hoaDecoder = new Decoder<Hoa2d, float>(order, nOutputs);
    
    // LINE USED TO SMOOTH RADIUS AND AZIMUTH VALUES
    line = new PolarLines<Hoa2d, float>(1);
    line->setRamp(100);
    smoothValues = new float[2];
    smoothValues[0] = 0.0;
    smoothValues[1] = 0.0;
    azimuth = 0;
    distanceFromCenter = 0;
    
    // FUNCTIONS TO SET THE ANGLE AND THE DISTANCE OF THE ENCODED SOUND SOURCE
    line->setRadiusDirect(0, distanceFromCenter);
    line->setAzimuthDirect(0, azimuth);
    hoaEncoder->setAzimuth(azimuth);
    hoaEncoder->setRadius(distanceFromCenter);
    
    
    circleCenter = ofPoint(ofGetWidth()/2,ofGetHeight()/2);
    circleRadius = ofGetWidth()/4;
    
    sourcePosition = circleCenter;
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    ofSetColor(255);
    ofCircle(circleCenter,circleRadius);
    ofSetColor(255,0,0);
    ofCircle(sourcePosition, 10);
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

    // CALCULATE AND SET SOURCE POSITION IN RELATION TO THE CENTER
    ofPoint currentPosition;
    sourcePosition = ofPoint(x,y);
    currentPosition = sourcePosition-circleCenter;
    
    // SMOOTH VALUES USING hoa::PolarLines
    line->setRadius(0, ofMap(sourcePosition.distance(circleCenter),0.0,circleRadius, 0.0,1.0));
    line->setAzimuth(0, converter.azimuth(currentPosition.x, currentPosition.y));

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

void ofApp::audioOut( float * output, int bufferSize, int nChannels){
    
    line->process(smoothValues);
    
    for (int i = 0; i<bufferSize; i++) {

        
        // CREATE AUDIO INPUT
        inputBuffer[i] = myOsc.sine(330)*0.1;
        
        // SET SMOOTHED CURRENT RADIUS AND AZIMUTH
        hoaEncoder->setRadius(smoothValues[0]);
        hoaEncoder->setAzimuth(smoothValues[1]);

        // CREATE THE SPHERICAL HARMONICS
        hoaEncoder->process(&inputBuffer[i], harmonicsBuffer);
        
        // DECODE THE HARMONICS; AUDIO TREATEMENTS ARE POSSIBLE IN BETWEEN THESE STEPS
        hoaDecoder->process(harmonicsBuffer, &output[i*nChannels]);

    }
}

void ofApp::exit(){
    soundStream.close();
}
