#pragma once

#include "ofMain.h"
#include "ofxHoa.h"
#include "ofxMaxim.h"
#include "ofxStk.h"

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
        void audioIn(float * input, int bufferSize, int nChannels);
        void exit();

    ofSoundStream soundStream;
    
    maxiOsc sine;
    
    float newValue, oldValue, xValue, myValue;
    
    float *inputBuffer;
    float *harmonicsBuffer;
    float azymuth;
    float radius;
    
    int order;
    int nInputs, nOutputs;
    int bufferSize;
    int sampleRate;
    int nBuffers;
    
private:
    
    hoa::EncoderDC<hoa::Hoa2d, float> *hoaEncoder;
    
    hoa::Decoder<hoa::Hoa2d, float> *hoaDecoder;
    
    stk::DelayL *delays;




    
};
