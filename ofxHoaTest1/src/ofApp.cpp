#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

//    UNCOMMENT THIS LINE TO PRINT AVALIABLE AUDIO DEVICES
    ofSoundStreamListDevices();
    
    soundStream.setDeviceID(3);
    
    nOutputs = 2;
    nInputs = 0;
    sampleRate = 44100;
    bufferSize = 512;
    nBuffers = (nInputs+nOutputs)*2;
    soundStream.setup(this, nOutputs, nInputs, sampleRate, bufferSize, nBuffers);
    
    inputBuffer = new float[bufferSize];
    
    order = 3;
    harmonicsBuffer = new float[order*2+1];
    hoaEncoder = new hoa::EncoderDC<hoa::Hoa2d, float>(order);
    hoaDecoder = new hoa::Decoder<hoa::Hoa2d, float>(order, nOutputs);
    
    hoaEncoder->setAzimuth(1.5);
    hoaEncoder->setRadius(1.0);
    
    oldValue = newValue = 1.5;
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key) {
        case ' ':
            
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    newValue = ofMap(x, 0, ofGetWidth(), 0, (HOA_2PI));
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
//    newValue = xValue;
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

void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    for (int i = 0; i<bufferSize; i++) {

    }
}

void ofApp::audioOut( float * output, int bufferSize, int nChannels){
    

    for (int i = 0; i<bufferSize; i++) {
    myValue = ofLerp(oldValue, newValue, 0.01);
        inputBuffer[i] = sine.square(110)*0.001;

        hoaEncoder->process(&inputBuffer[i], harmonicsBuffer);
        hoaDecoder->process(harmonicsBuffer, &output[i*nChannels]);
         hoaEncoder->setAzimuth(myValue);
    }
    
    oldValue = myValue;

}

void ofApp::exit(){
    ofSoundStreamClose();
}
