//
//  ofxHoaOsc.cpp
//  exampleOneSource
//
//  Created by Claudio Cabral on 18/02/15.
//
//

#include "ofxHoaOsc.h"
using namespace hoa;

ofxHoaOsc::ofxHoaOsc(){

    mySampleRate = 44100;
    ramp = 1.0/(100.0*mySampleRate/1000.0);
    oldValue = 0.0-ramp;
    newValue = 1.0;
    count = 0;
}

//SETUP
void ofxHoaOsc::setup(int sampleRate)
{
    mySampleRate = sampleRate;

}

// PHASOR
double ofxHoaOsc::phasor(double freq)
{
    phaseStep = freq/(double)mySampleRate;
    
    phase = count*phaseStep;
    decimalPhase = phase -floor(phase);

        count++;

    return (decimalPhase);
}

double ofxHoaOsc::envelope(){
    
    if (oldValue<=newValue) {
        oldValue+=ramp;
        return oldValue;
    }

    else {
        return (double) 1.0;
    }
}

double ofxHoaOsc::sawtooth(double freq)
{
    //phase entre -1 et 1//    
    return (phasor(freq)*2-1);
}


// SQUARE WAVE
double ofxHoaOsc::square(double freq)
{

    if (phasor(freq) < 0.5)
        return -1;
    else
        return 1;
}

// TRIANGLE WAVE
double ofxHoaOsc::triangle(double freq)
{
    const double currentPhase = fmod(phasor(freq)+0.25, 1.);
    
    if (currentPhase<=0.5) {
        return currentPhase * 4. -1.;
    }
    else{
        return fmod(1-currentPhase,0.5) *4. -1.;
        }
}

// SIGNAL SINUS
double ofxHoaOsc::sine(double freq)
{
    return sin(HOA_2PI*phasor(freq));
}