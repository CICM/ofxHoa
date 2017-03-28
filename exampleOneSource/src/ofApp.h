#pragma once

#include "ofMain.h"
#include "ofxHoa.h"
#include <memory>


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

        hoa::ofxHoaOscillator<float> myOsc, myEnv;

        // most hoa classes require arguments for initilization, so we create
        // them as pointers
        unique_ptr<hoa::Encoder<hoa::Hoa2d, float>::DC> hoaEncoder;
        unique_ptr<hoa::Decoder<hoa::Hoa2d, float> > hoaDecoder;
        unique_ptr<hoa::Optim<hoa::Hoa2d, float> > hoaOptim;
        unique_ptr<hoa::ofxHoaCoord<hoa::Hoa2d, float> > hoaCoord;
        ofSoundStream soundStream;

        float input;
        vector<float> harmonicsBuffer;


        int order;
        int nInputs, nOutputs;
        int bufferSize;
        int sampleRate;
        int nBuffers;

        float circleRadius;
        ofVec3f circleCenter;
        ofVec3f sourcePosition;
};
