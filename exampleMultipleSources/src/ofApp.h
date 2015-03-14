#pragma once

#include "ofMain.h"
#include "ofxHoa.h"

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
		
    ofMesh mesh;
    ofVec2f * position;
    ofVec2f * velocity;
    ofVec2f * noise;
    ofVec2f circleCenter;
    ofVec2f circleMin, circleMax;
    float circleRadius;
    float velocityMax;
    
    EncoderMulti<Hoa2d, float> * encoderMulti;
    Decoder<Hoa2d, float> * decoder;
    PolarLines<Hoa2d, float> * line;
    
    ofxHoaOsc * myOsc;

    float * inputBuffer;
    float * harmonicsBuffer;
    float * frequencies;
    float * lineValues;

    ofSoundStream soundStream;
    int nInputs, nOutputs, nChannels, nBuffers, sampleRate, bufferSize;
    
    int numberOfParticles;
    int order;
    
    
};
