#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetCircleResolution(50);
    //    UNCOMMENT THIS LINE TO PRINT AVALIABLE AUDIO DEVICES
//    ofSoundStreamListDevices();
    
    // USE THIS FUNCTION TO SET THE AUDIO DEVICE IF NECESSARY
//    soundStream.setDeviceID(5);

    /*ASSIGN AUDIO PARAMETERS
    NUMBER OF OUTPUTS MUST BE >= ORDER*2+1 FOR DECODER ON REGULAR MODE*/
    nOutputs = 2;
    nInputs = 0;
    sampleRate = 44100;
    bufferSize = 512;
    nBuffers = (nInputs+nOutputs)*2;
    
    // CREATE BUFFER FOR SOUND INPUT
    inputBuffer = new float[bufferSize];
    
    // SETUP TEST OSCILATOR
    myOsc.setup(sampleRate);
    myEnv.setup(sampleRate);
    
    // SETUP HOA
    order = 3;
    //CREATE THE SPHERICAL HARMONICS BUFFER, IT MUST HAVE ORDER*2+1 VALUES FOR 2 DIMENSIONS
    harmonicsBuffer = new float[order*2+1];
    
    // THE ENCODER CALCULATES THE SPHERICAL HARMONICS
    hoaEncoder = new EncoderDC<Hoa2d, float>(order);
    
    /* THE DECODER TRANSLATES THE HARMONICS INTO AUDIO SIGNALS FOR OUTPUT.
     THE NUMBER OF MINIMUM OUPUT CHANNELS FOR REGULAR MODE = ORDER*2+1
     SMALLER VALUES MAY BE USED, BUT THE RESULTING SOUND WON'T BE AS EXPECTED 
     FOR SMALL DIFFERENCES ( 5 OR 6 INSTEAD OF 7 SPEAKERS) IRREGULAR MODE MAY BE USED */

//        hoaDecoder = new Decoder<Hoa2d, float>::Regular(order, nOutputs);
    
    // BINAURAL MODE SET FOR USE WITH HEADPHONES
    hoaDecoder = new Decoder<Hoa2d, float>::Binaural(order);
    
    /* RENDERING IS COMPUTED IN RELATION TO THE SPEAKER'S ANGLES
     THEY MAYBE SET WITH THE FUNCTION
     hoaDecoder->setPlanewaveAzimuth(const ulong index, const float azimuth); */

    hoaDecoder->computeRendering(bufferSize);
    // LINE USED TO SMOOTH RADIUS AND AZIMUTH VALUES
    line = new PolarLines<Hoa2d, float>(1);
    line->setRamp(round(50 * sampleRate/1000.0));
    smoothValues = new float[2];
    smoothValues[0] = 0.0;
    smoothValues[1] = 0.0;
    azimuth = 0;
    distanceFromCenter = 0;
    
    // FUNCTIONS TO SET THE ANGLE AND THE DISTANCE OF THE ENCODED SOUND SOURCE
    line->setRadiusDirect(0, 10000.);
    line->setAzimuthDirect(0, azimuth);
    hoaEncoder->setAzimuth(azimuth);
    hoaEncoder->setRadius(100000.);
    
    
    circleCenter = ofVec3f(ofGetWidth()/2,ofGetHeight()/2);
    circleRadius = 100;
    
    sourcePosition = circleCenter;
    
    //INITIALIZE SOUNDSTREAM
    soundStream.setup(this, nOutputs, nInputs, sampleRate, bufferSize, nBuffers);
}

//--------------------------------------------------------------
void ofApp::update(){
    // CHANGE SOURCE POSITION
    sourcePosition = ofVec3f(mouseX, mouseY);
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
    
    // CALCULATE SOURCE POSITION IN RELATION TO THE CENTER
    relativePosition = ofVec3f(mouseX,ofGetHeight() - mouseY)-circleCenter;
    
    // SET NEW RADIUS AND ANGLE USING HOA MATH CLASS

    line->setRadius(0, Math<float>::radius(relativePosition.x, relativePosition.y)*1.0/circleRadius);

    line->setAzimuth(0, Math<float>::azimuth(relativePosition.x, relativePosition.y));
    
    for (int i = 0; i<bufferSize; i++) {
        // CALCULATE SMOOTHED VALUES AND PUT THEM INTO THE ARRAY
        line->process(smoothValues);

        // CREATE AUDIO INPUT
        inputBuffer[i] = myOsc.triangle(330)*myEnv.sawtooth(2)*0.1;
        
        // SET SMOOTHED CURRENT RADIUS AND AZIMUTH
        hoaEncoder->setRadius(smoothValues[0]);
        hoaEncoder->setAzimuth(smoothValues[1]);

        // CREATE THE SPHERICAL HARMONICS
        hoaEncoder->process(inputBuffer+i, harmonicsBuffer);

        // DECODE THE HARMONICS; AUDIO TREATEMENTS ARE POSSIBLE IN BETWEEN THESE STEPS
        hoaDecoder->process(harmonicsBuffer, output+i*nChannels);
        }
}

void ofApp::exit(){
    //CLOSE SOUND STREAM AND THEN DESTROY ALL OBJECTS CREATED TO AVOID MEMORY LEAKS
    soundStream.close();
    
    delete hoaEncoder;
    delete hoaDecoder;
    delete line;
    delete [] inputBuffer;
    delete [] harmonicsBuffer;
    delete [] smoothValues;
}
