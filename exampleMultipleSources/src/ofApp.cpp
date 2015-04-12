#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

//    soundStream.setDeviceID(5);
//    ofSoundStreamListDevices();
    
    // CHOOSE NUMBER OF PARTICLES; EACH ONE WILL HAVE IT'S OWN COLOR AND FREQUENCY
    numberOfParticles = 3;
    
    // CREATE VARIABLES OF THE SPEAKER CIRCLE
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
    inputBuffer = new float[numberOfParticles];
    harmonicsBuffer = new float[order*2+1];
    myOsc = new ofxHoaOscillator<float>[numberOfParticles];
    
    // THE ENCODERMULTI ALLOWS TO DISTRIBUTE MULTIPLE SOURCES IN SPACE
    encoderMulti = new Encoder<Hoa2d, float>::Multi(order,numberOfParticles);

    // CHOSE DECODER TYPE
//    decoder = new Decoder<Hoa2d, float>::Binaural(order);
        decoder = new Decoder<Hoa2d, float>::Regular(order,nOutputs);
    // COMPUE MATRIX OF SPEAKERS
    decoder->computeRendering(bufferSize);
    
    /*THE OPTIM ALLOWS TO ACOUNT FOR DISPLACEMENTS IN IDEAL SPEAKER POSITION
     "Basic" WORKS AS A BYPASS.
     "InPhase" AND "MaxRe" SHOULD BE USED IF THE AMBSIONICS CIRCLE/SPHERE IS NOT PERFECT */
//    optimizer = new Optim<Hoa2d, float>::Basic(order);
    optimizer = new Optim<Hoa2d, float>::InPhase(order);

    
    hoaCoord = new ofxHoaCoord<Hoa2d, float>(numberOfParticles);

    // SET THE RAMP FOR SMOOTHING THE VALUES
    hoaCoord->setRamp(50, sampleRate);

    // SETUP MESH, THIS IS JUST A CHEAP WAY OF MAKING BIG POINTS
    mesh.setMode(OF_PRIMITIVE_POINTS);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(20);
    
    // MAXIMUM VALUE FOR THE VELOCITY OF THE PARTICLES
    velocityMax = 5;
    
    // INITIALIZE ALL ARRAYS AND ADD VERTICES TO MESH
    for (int i = 0; i<numberOfParticles; i++) {
        
        myOsc[i].setup(sampleRate, OF_TRIANGLE_WAVE);
                       
                       //oscType(floor(ofRandom(4))));
        myOsc[i].setFrequency(ofRandom(100, 1000));
        
        position[i] = ofVec3f(ofRandom(circleMin.x, circleMax.x),ofRandom(circleMin.y, circleMax.y));
        velocity[i] = ofVec3f(ofRandom(-velocityMax,velocityMax),ofRandom(-velocityMax,velocityMax));
        noise[i] = ofVec3f(ofRandom(10000),ofRandom(10000));
        
        mesh.addVertex(position[i]);
        mesh.addColor(ofFloatColor(abs(ofRandomf()),abs(ofRandomf()),abs(ofRandomf())));
        
        hoaCoord->setAmbisonicCenter(circleCenter);
        hoaCoord->setAmbisonicRadius(circleRadius);
        hoaCoord->setSourcePositionDirect(i, position[i]);
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
        
        // SET SOURCE POSITION
        hoaCoord->setSourcePosition(i, position[i]);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // DRAW EVERYTHING
    ofBackgroundGradient(ofColor::gold, ofColor::black);
    ofNoFill();
    ofSetColor(ofColor::paleVioletRed);
    
    //MAKE A THICK CIRCLE
    for (int i = 0; i<10; i++){
    ofCircle(circleCenter, circleRadius+i-5);
    }
    
    //DRAW THE SOURCES
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
        hoaCoord->process();

        
        // SET CURRENT POSITION FOR EACH PARTICLE
        for (int j = 0; j<numberOfParticles; j++) {
            
            inputBuffer[j] = (myOsc[j].tick()/numberOfParticles)*0.3;

            //GET CALCULATED RADIUS AND AZIMUTH AND PASS THEM TO THE ENCODER
            encoderMulti->setRadius(j, hoaCoord->getRadius(j));
            encoderMulti->setAzimuth(j, hoaCoord->getAzimuth(j));
        }

        // PROCESS ONE SAMPLE OF EACH PARTICLE AND CREATE THE SPHERICAL HARMONICS
        encoderMulti->process(inputBuffer,harmonicsBuffer);
        
        // PROCESS THE HARMONICS WITH OPTIM
        optimizer->process(harmonicsBuffer, harmonicsBuffer);
        
        // TRANSFORM THE HARMONICS TO AUDIO SIGNALS
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
    delete [] inputBuffer;
    delete [] harmonicsBuffer;
    delete [] myOsc;
    
    delete encoderMulti;
    delete decoder;
    delete optimizer;
    delete hoaCoord;
}