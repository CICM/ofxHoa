#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    nOutputs = 2;
    nInputs = 0;
    sampleRate = 44100;
    nBuffers = (nInputs+nOutputs)*2;
    
    input = 0;

    myOsc.setup(sampleRate, OF_SQUARE_WAVE);
    myOsc.setFrequency(330);
    myEnv.setup(sampleRate, OF_SAWTOOTH_WAVE);
    myEnv.setFrequency(-2);
    

    harmonicMatrix = new float * [order * 2+1];
    
    for (int i = 0; i< order*2+1;++i) harmonicMatrix[i] = new float[bufferSize];
    
    outputMatrix = new float * [2];
    outputMatrix[0] = new float[bufferSize];
    outputMatrix[1] = new float[bufferSize];
    hoaDecoder.setCropSize(256);
 
    hoaDecoder.computeRendering(bufferSize);

    circleCenter = ofVec3f(ofGetWidth()/2,ofGetHeight()/2);
    circleRadius = 100;
    
    hoaCoord.setAmbisonicCenter(circleCenter);
    hoaCoord.setAmbisonicRadius(circleRadius);
    

    hoaCoord.setRamp(50, sampleRate);
    
    hoaCoord.setSourcePositionDirect(0, ofVec3f(10000,10000));
    
    ofSetCircleResolution(50);
    
    soundStream.setup(this, nOutputs, nInputs, sampleRate, bufferSize, nBuffers);
}

//--------------------------------------------------------------
void ofApp::update(){
    // CHANGE SOURCE POSITION
    sourcePosition = ofVec3f(mouseX, mouseY);
    hoaCoord.setSourcePosition(0, sourcePosition);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackgroundGradient(ofColor::gold, ofColor::black);
    ofSetColor(ofColor::crimson);
    ofFill();
    ofCircle(sourcePosition, 10);
    ofNoFill();
    ofCircle(circleCenter, circleRadius);
    
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

void ofApp::audioOut( float * output, int bufferSize, int nChannels){
    
    for (int i = 0; i<bufferSize; i++) {
        
        hoaCoord.process();
        input = myOsc.tick()*(myEnv.tick()+1)*0.05;
        hoaEncoder.setRadius(hoaCoord.getRadius(0));
        hoaEncoder.setAzimuth(hoaCoord.getAzimuth(0));
        hoaEncoder.process(&input, harmonicsBuffer);
        hoaOptim.process(harmonicsBuffer, harmonicsBuffer);
        
        for (int j = 0; j<order*2+1; j++) {
            harmonicMatrix[j][i] = harmonicsBuffer[j];
        }
    }
    hoaDecoder.processBlock(const_cast<const float **>(harmonicMatrix), outputMatrix);

    for (int i = 0; i<bufferSize; ++i) {
        output[i*nChannels] = outputMatrix[0][i];
        output[i*nChannels+1] = outputMatrix[1][i];
    }
    
    
    
    
}

void ofApp::exit(){
    // close soundStream
    soundStream.close();
    
    delete [] outputMatrix[0];
    delete [] outputMatrix[1];
    delete [] outputMatrix;
    
    for (int i = 0; i<order*2 + 1; ++i) delete[] harmonicMatrix[i];
    delete[] harmonicMatrix;

}
