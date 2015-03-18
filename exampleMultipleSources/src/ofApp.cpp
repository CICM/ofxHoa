#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

//    soundStream.setDeviceID(2);
//    ofSoundStreamListDevices();
    // CHOOSE NUMBER OF PARTICLES; EACH ONE WILL HAVE IT'S OWN COLOR AND FREQUENCY
    numberOfParticles = 10;
    
    // CREATE VARIABLES OF THE CENTER CIRCLE
    circleCenter = ofVec3f(ofGetWidth()/2,ofGetHeight()/2);
    circleRadius = ofGetWidth()/4;
    circleMin = circleCenter - circleRadius;
    circleMax = circleCenter + circleRadius;
    
    // CONFIGURE AUDIO
    nOutputs = 2;
    nInputs = 0;
    sampleRate = 44100;
    bufferSize = 512;
    nBuffers = (nOutputs + nInputs)*2;
    
    // CHOOSE HOA ORDER
    order = 3;
    
    // CREATE ALL THE ARRAYS WE'LL NEED
    position = new ofVec3f[numberOfParticles];
    velocity = new ofVec3f[numberOfParticles];
    noise = new ofVec3f[numberOfParticles];
    lineValues = new float[numberOfParticles*2];
    frequencies = new float[numberOfParticles];
    inputBuffer = new float[numberOfParticles*bufferSize];
    harmonicsBuffer = new float[order*2+1];
    myOsc = new ofxHoaOsc[numberOfParticles];
    
    // THE ENCODERMULTI ALLOWS TO DISTRIBUTE MULTIPLE SOURCES IN SPACE
    encoderMulti = new EncoderMulti<Hoa2d, float>(order,numberOfParticles);

    // CHOSE DECODER TYPE
//    decoder = new Decoder<Hoa2d, float>::Binaural(order);
        decoder = new Decoder<Hoa2d, float>::Regular(order,nOutputs);
    // COMPUE MATRIX OF SPEAKERS
    decoder->computeRendering(bufferSize);
    
    line = new PolarLines<Hoa2d, float>(numberOfParticles);
    
    // SET THE RAMP FOR SMOOTHING THE VALUES
    line->setRamp(sampleRate/100);

    // SETUP MESH
    mesh.setMode(OF_PRIMITIVE_POINTS);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(20);
    
    // MAXIMUM VALUE FOR THE VELOCITY OF THE PARTICLES
    velocityMax = 5;
    
    // INITIALIZE ALL ARRAYS AND ADD VERTICES TO MESH
    for (int i = 0; i<numberOfParticles; i++) {
        myOsc[i].setup(sampleRate);
        frequencies[i] = ofRandom(100, 1000);
        position[i] = ofVec3f(ofRandom(circleMin.x, circleMax.x),ofRandom(circleMin.y, circleMax.y));
        velocity[i] = ofVec3f(ofRandom(-velocityMax,velocityMax),ofRandom(-velocityMax,velocityMax));
        noise[i] = ofVec3f(ofRandom(10000),ofRandom(10000));
        mesh.addVertex(position[i]);
        mesh.addColor(ofFloatColor(abs(ofRandomf()),abs(ofRandomf()),abs(ofRandomf())));
        
        line->setRadiusDirect(i, ofMap(position[i].distance(circleCenter), 0.0, circleMax.x, 0.0, 10.0));
        line->setAzimuthDirect(i, Math<float>::azimuth(position[i].x, position[i].y));
        
        lineValues[i] = line->getRadius(i);
        lineValues[i+numberOfParticles] = line->getAzimuth(i);
    }

    soundStream.setup(this, nOutputs, nInputs, sampleRate, bufferSize, nBuffers);
    
    // MAKE A PRETIER CIRCLE
    ofSetCircleResolution(50);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // CHECK FOR WALL COLLISION
    for (int i = 0; i<numberOfParticles; i++) {
        
        if (position[i].x <= circleMin.x || position[i].x >= circleMax.x) {
            velocity[i].x*= -1;
        }
        if (position[i].y <= circleMin.y || position[i].y >= circleMax.y) {
            velocity[i].y*= -1;
        }
        
        relativePosition.x = position[i].x - circleCenter.x;
        relativePosition.y = (ofGetHeight() - position[i].y) - circleCenter.y;

        // CALCULATE NEW POSITION USING VELOCITY AND PERLIN NOISE
        noise[i]+=0.01;
        position[i]+=ofNoise(noise[i].x,noise[i].y)*velocity[i];
        mesh.setVertex(i, position[i]);
        
        // SET RADIUS AND AZIMUTH IN RELATION TO NEW POSITION
        line->setRadius(i, Math<float>::radius(relativePosition.x,
                                               relativePosition.y)* 1/circleRadius);
        line->setAzimuth(i, Math<float>::azimuth(relativePosition.x, relativePosition.y));
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // DRAW EVERYTHING
    ofBackgroundGradient(ofColor::gold, ofColor::black);
    ofNoFill();
    ofSetColor(ofColor::paleVioletRed);
    for (int i = 0; i<10; i++){
    ofCircle(circleCenter, circleRadius+i-5);
    }
    ofSetColor(255);
    mesh.draw();

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
void ofApp::audioOut(float *output, int bufferSize, int nChannels){

    // BEGIN AUDIO LOOP
    for (int i = 0; i<bufferSize; i++) {

    // SMOOTH VALUES
    line->process(lineValues);
        
        // SET CURRENT RADIUS AND AZIMUTH FOR EACH PARTICLE
        for (int j = 0; j<numberOfParticles; j++) {
            inputBuffer[j] = (myOsc[j].triangle(frequencies[j])/numberOfParticles)*0.5;

            encoderMulti->setRadius(j, lineValues[j]);
            encoderMulti->setAzimuth(j, lineValues[j+numberOfParticles]);
        }

        // PROCESS ONE SAMPLE OF EACH PARTICLE AND OUTPUT THE CORRESPONDING AUDIO
        encoderMulti->process(inputBuffer,harmonicsBuffer);
        decoder->process(harmonicsBuffer, output+i*nChannels);

    }
}

//--------------------------------------------------------------
void  ofApp::exit(){
    
    // CLEAR AUDIO AND DELETE OBJECTS WHEN CLOSING
    soundStream.close();
    
    delete [] position;
    delete [] velocity;
    delete [] noise;
    delete [] lineValues;
    delete [] frequencies;
    delete [] inputBuffer;
    delete [] harmonicsBuffer;
    delete [] myOsc;
    
    delete encoderMulti;
    delete decoder;
    delete line;
}