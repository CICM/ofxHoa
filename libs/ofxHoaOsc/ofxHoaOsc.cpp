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
}

//SETUP
void ofxHoaOsc::setup(int sampleRate)
{
    mySampleRate = sampleRate;
    count = 0;
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


double ofxHoaOsc::sawtooth(double freq)
{
    //phase entre -1 et 1//    
    return (phasor(freq)*2-1);
}


// SIGNAL CARRÉ
double ofxHoaOsc::square(double freq)
{

    if (phasor(freq) < 0.5)
        return -1;
    else
        return 1;
}

// SIGNAL TRIANGULAIRE
double ofxHoaOsc::triangle(double freq)
{
    if (phasor(freq) < 0.5)
        return (2.0*phasor(freq));
    else
        return (2.0 - 2.0*phasor(freq));
}

// SIGNAL SINUS
double ofxHoaOsc::sine(double freq)
{
    return cos(HOA_2PI*phasor(freq));
}