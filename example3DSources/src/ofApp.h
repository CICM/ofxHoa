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
    
    ofCamera cam;
    ofLight light;
    int numberOfSources;
    ofSpherePrimitive centralSphere, sphereOfSpeakers;
    ofVec3f centralSpherePosition;
    int centralSphereRadius;
    vector<ofSpherePrimitive> sphere;
    vector<ofVec3f> position;
    vector<ofVec3f> velocity;
    vector<ofVec3f> seed;
    ofVec3f relativePosition;
    vector<ofColor> sphereColor;

    float velMax;
    
    ofSoundStream soundStream;
    
    int nOutputs, nInputs, sampleRate, bufferSize, nBuffers;
    
    int hoaOrder;
    
    vector<ofxHoaOscillator<float> > oscillator;

    PolarLines<Hoa3d, float> * line;
    
    float * lineValue;
    float * harmonic;
    float * input;

    Encoder<Hoa3d, float>::Multi * encoder;
    Decoder<Hoa3d, float> * decoder;
    Optim<Hoa3d, float> * optim;
    
		
};
