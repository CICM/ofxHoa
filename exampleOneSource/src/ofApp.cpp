#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

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

    // INITIALIZE INPUT TO 0
    input = 0;

    // SETUP TEST OSCILATOR
    myOsc.setup(sampleRate, OF_SQUARE_WAVE);
    myOsc.setFrequency(330);
    myEnv.setup(sampleRate, OF_SAWTOOTH_WAVE);
    myEnv.setFrequency(-2);

    // SETUP HOA
    order = 3;
    //CREATE THE SPHERICAL HARMONICS BUFFER, IT MUST HAVE ORDER*2+1 VALUES FOR 2 DIMENSIONS
    harmonicsBuffer = new float[order*2+1];

    // THE ENCODER CALCULATES THE SPHERICAL HARMONICS
    hoaEncoder = new Encoder<Hoa2d, float>::DC(order);

    /* THE DECODER TRANSLATES THE HARMONICS INTO AUDIO SIGNALS FOR OUTPUT.
       THE NUMBER OF MINIMUM OUPUT CHANNELS FOR REGULAR MODE = ORDER*2+1
       SMALLER VALUES MAY BE USED, BUT THE RESULTING SOUND WON'T BE AS EXPECTED
       FOR SMALL DIFFERENCES ( 5 OR 6 INSTEAD OF 7 SPEAKERS) IRREGULAR MODE MAY BE USED */
    hoaDecoder = new Decoder<Hoa2d, float>::Regular(order, nOutputs);

    // BINAURAL MODE SET FOR USE WITH HEADPHONES
    //    hoaDecoder = new Decoder<Hoa2d, float>::Binaural(order);

    /* RENDERING IS COMPUTED IN RELATION TO THE SPEAKER'S ANGLES
       THEY MAYBE SET WITH THE FUNCTION hoaDecoder->setPlanewaveAzimuth(const ulong index,
       const float azimuth); */
    hoaDecoder->computeRendering(bufferSize);

    /*THE OPTIM ALLOWS TO ACOUNT FOR DISPLACEMENTS IN IDEAL SPEAKER POSITION
      "Basic" WORKS AS A BYPASS.
      "InPhase" AND "MaxRe" SHOULD BE USED IF THE AMBSIONICS CIRCLE/SPHERE IS NOT PERFECT */
    hoaOptim = new Optim<Hoa2d, float>::Basic(order);
    //    hoaOptim = new Optim<Hoa2d, float>::InPhase(order);

    // ofxHoaCoord USED TO SET SOURCE POSITION AND VOID CLICKS IN AUDIO
    hoaCoord = new ofxHoaCoord<Hoa2d, float>(1);

    // SET THE POSITION IN SCREEN THAT'LL REPRESENT THE CENTER OF THE SPEAKER CIRCLE
    // AND IT'S RADIUS
    circleCenter = ofVec3f(ofGetWidth()/2,ofGetHeight()/2);
    circleRadius = 100;
    hoaCoord->setAmbisonicCenter(circleCenter);
    hoaCoord->setAmbisonicRadius(circleRadius);

    // SET THE RAMP FOR INTERPOLATION IN MILLISECONDS
    hoaCoord->setRamp(50, sampleRate);

    // FUNCTIONS TO SET THE POSITION OF THE ENCODED SOUND SOURCE
    // FAR AWAY TO AVOID CLICKS IN THE BEGINING
    hoaCoord->setSourcePositionDirect(0, ofVec3f(10000,10000));


    // MAKE A PRETTIER CIRCLE
    ofSetCircleResolution(50);

    //INITIALIZE SOUNDSTREAM
    soundStream.setup(this, nOutputs, nInputs, sampleRate, bufferSize, nBuffers);
}

//--------------------------------------------------------------
void ofApp::update(){
    // CHANGE SOURCE POSITION
    sourcePosition = ofVec3f(mouseX, mouseY);
    hoaCoord->setSourcePosition(0, sourcePosition);
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

        // CALCULATE SMOOTHED VALUES
        hoaCoord->process();

        // CREATE AUDIO INPUT. THE LAST MULTIPLICATION IS THE VOLUME (SHOULD BE BETWEEN 0 AND 1)
        //        inputBuffer[i] = myOsc.tick()*(myEnv.tick()+1)*0.05;
        input = myOsc.tick()*(myEnv.tick()+1)*0.05;
        // SET SMOOTHED CURRENT RADIUS AND AZIMUTH
        hoaEncoder->setRadius(hoaCoord->getRadius(0));
        hoaEncoder->setAzimuth(hoaCoord->getAzimuth(0));

        // CREATE THE SPHERICAL HARMONICS
        hoaEncoder->process(&input, harmonicsBuffer);

        // PROCESS THE HARMONICS WITH OPTIM
        hoaOptim->process(harmonicsBuffer, harmonicsBuffer);

        // DECODE THE HARMONICS; AUDIO TREATEMENTS ARE POSSIBLE IN BETWEEN THESE STEPS
        hoaDecoder->process(harmonicsBuffer, output+i*nChannels);
    }
}

void ofApp::exit(){
    //CLOSE SOUND STREAM AND THEN DESTROY ALL OBJECTS CREATED TO AVOID MEMORY LEAKS
    soundStream.close();

    delete hoaEncoder;
    delete hoaDecoder;
    delete hoaOptim;
    delete hoaCoord;
    delete [] harmonicsBuffer;
}
