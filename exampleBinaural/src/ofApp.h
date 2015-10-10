#pragma once

#include "ofMain.h"
#include "ofxHoa.h"

/*
    The binaural decoder works in a slightly different manner for optimization purposes
 */

using namespace hoa;

class ofApp : public ofBaseApp{
    
    
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void audioOut(float * output, int bufferSize, int nChannels);
    void exit();
    
    ofxHoaOscillator<float> myOsc, myEnv;
   
    static const int order = 3;
    int nInputs, nOutputs;
    static const int bufferSize = 512;
    int sampleRate;
    int nBuffers;
    
    //MOST HOA CLASSES REQUIRE ARGUMENTS FOR INITILIZATION, SO WE CREATE THEM AS POINTERS
    Encoder<Hoa2d, float>::DC hoaEncoder = Encoder<Hoa2d, float>::DC(order);
    Decoder<Hoa2d, float>::Binaural hoaDecoder = Decoder<Hoa2d, float>::Binaural(order);
    Optim<Hoa2d, float>::Basic hoaOptim = Optim<Hoa2d, float>::Basic(order);
    ofxHoaCoord<Hoa2d, float> hoaCoord = ofxHoaCoord<Hoa2d, float>(1);
    
    ofSoundStream soundStream;
    
    float input;
    float harmonicsBuffer[order*2+1];
    
    float ** const harmonicMatrix;
    float ** const outputMatrix;
    
    float circleRadius;
    ofVec3f circleCenter;
    ofVec3f sourcePosition;
};