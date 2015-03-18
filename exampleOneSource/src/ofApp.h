#pragma once

#include "ofMain.h"
#include "ofxHoa.h"

// DON'T FORGET TO USE THE NAMESPACE HOA
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
    
    ofxHoaOsc myOsc, myEnv;
    
    //MOST HOA CLASSES REQUIRE ARGUMENTS FOR INITILIZATION, SO WE CREATE THEM AS POINTERS
    EncoderDC<Hoa2d, float> * hoaEncoder;
    Decoder<Hoa2d, float> * hoaDecoder;
    PolarLines<Hoa2d, float> * line;
    
    ofSoundStream soundStream;
    
    float * inputBuffer;
    float * harmonicsBuffer;
    float * smoothValues;
    float azimuth;
    float distanceFromCenter;
    
    int order;
    int nInputs, nOutputs;
    int bufferSize;
    int sampleRate;
    int nBuffers;
    
    float circleRadius;
    ofVec3f circleCenter;
    ofVec3f sourcePosition;
    ofVec3f relativePosition;
};