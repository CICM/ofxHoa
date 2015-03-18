//
//  ofxHoaOsc.h
//  exampleOneSource
//
//  Created by Claudio Cabral on 18/02/15.
//
// THIS IS AN EXTREMELLY NON-OPTIMIZED OSCILATOR, IT'S INCLUDED FOR TESTING AND EXAMPLE PURPOSES

#ifndef __exampleOneSource__ofxHoaOsc__
#define __exampleOneSource__ofxHoaOsc__

#include <stdio.h>
#include "../HoaLibrary-light/Sources/Defs.hpp"

namespace hoa {
    
class ofxHoaOsc {
    
private:
    double ramp, oldValue, newValue;
    double freq;//fréquence du signal carré//

    double phase;
    
    double phaseStep;

    double totalPhase;
    
    double decimalPhase;
    
    double phasor(double phase);
    double counter();
    
    int count, mySampleRate;
    
public:
    
    ofxHoaOsc();
    
    void setup(int sampleRate);
    
    double sine(double freq);
    double square(double freq);
    double triangle(double freq);
    double sawtooth(double freq);
    double envelope();
    
};
}
#endif /* defined(__exampleOneSource__ofxHoaOsc__) */
