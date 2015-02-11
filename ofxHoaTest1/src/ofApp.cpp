#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    soundStream.setDeviceID(5);
    
//    ofSoundStreamListDevices();
    
    soundStream.setup(this, 8, 0, 44100, 512, 16);
    
//    ofSoundStreamSetup(2, 2, 44100, 512, 8);
    
    
    myEncoder = new hoa::EncoderDC<hoa::Hoa2d, float>(3);
    myDecoder = new hoa::Decoder<hoa::Hoa2d, float>(3, 8);
    
    myEncoder->setAzimuth(1.5);
    myEncoder->setRadius(1.0);
    
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
    newValue = ofMap(x, 0, ofGetWidth(), 0, (3.14*4));
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
//        myBuffer[i] = input[i*nChannels];
    }
}

void ofApp::audioOut( float * output, int bufferSize, int nChannels){
    
    float harmonics[7];
    float outs[8];

    for (int i = 0; i<bufferSize; i++) {
    myValue = ofLerp(oldValue, newValue, 0.01);
        myBuffer[i] = sine.square(110)*0.001;
        
        myEncoder->process(&myBuffer[i], harmonics);
        myDecoder->process(harmonics, &output[i*nChannels]);
         myEncoder->setAzimuth(myValue);
    }
    
    oldValue = myValue;

}

void ofApp::exit(){
    ofSoundStreamClose();
}
