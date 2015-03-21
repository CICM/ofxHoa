//
//  ofxHoaOscillator.h
//
//  Created by Claudio Cabral on 21/03/15.
//


#ifndef __OFX_HOA_OSCILLATOR__
#define __OFX_HOA_OSCILLATOR__

#define OSC_BUFFER_SIZE 4096

//#include "ofMain.h"
#include <stdio.h>
#include "../HoaLibrary-light/Sources/Defs.hpp"

namespace hoa {

    enum oscType { OF_SINE_WAVE, OF_TRIANGLE_WAVE, OF_SAWTOOTH_WAVE, OF_SQUARE_WAVE, OF_NOISE_WAVE, OF_PHASOR_WAVE };
    
template <typename T> class ofxHoaOscillator {
    
private:
    
    T _freq;

    T _phase;
    
    T _step;
    
    T _buffer[OSC_BUFFER_SIZE];
    
    int _index, _sampleRate;
    
public:

    ofxHoaOscillator();
    ~ofxHoaOscillator();
    
    void setup(int sampleRate, oscType waveform);
    void setFrequency(T frequency);
    void setPhase(T phase);
    
    T tick();
    
};
}

using namespace hoa;

template <typename T>
ofxHoaOscillator<T>::ofxHoaOscillator(){
    
    _sampleRate = 44100;
    _phase = 0;
    _index = 0;
    _freq = 440;
    
    _step = _sampleRate;
    _step = OSC_BUFFER_SIZE/_step;
    

}
template <typename T>
ofxHoaOscillator<T>::~ofxHoaOscillator(){

}
//SETUP
template <typename T>
void ofxHoaOscillator<T>::setup(int sampleRate, oscType waveform)
{
    for (int i = 0; i<OSC_BUFFER_SIZE; i++) {
        
        switch (waveform) {
                
            case OF_SINE_WAVE:
                
                _buffer[i] = sin (i * HOA_2PI/OSC_BUFFER_SIZE);
                
                break;
                
            case OF_TRIANGLE_WAVE:
                
                if (i <= OSC_BUFFER_SIZE/2 ) {
                    _buffer[i] = -1.0 + i * (4.0/OSC_BUFFER_SIZE) ;
                    
                } else {
                    _buffer[i] =1.0 -  (i-OSC_BUFFER_SIZE/2) * (4.0/OSC_BUFFER_SIZE);
                }
                break;
                
            case OF_SAWTOOTH_WAVE:
                
                _buffer[i] = -1 + i * 2.0/OSC_BUFFER_SIZE;
                break;
                
            case OF_SQUARE_WAVE:
                
                if (i<=OSC_BUFFER_SIZE/2) _buffer[i]=-1;
                
                else  _buffer[i]=1;
                
                break;
                
            case OF_NOISE_WAVE:
                
                //                _buffer[i] = ofRandomf();
                
                break;
                
            default:
                _buffer[i] = 0;
                break;
        }
        
    }
    
}

template <typename T>
T ofxHoaOscillator<T>::tick() {
    
    _phase += _step * _freq;
    
    if (_phase>=OSC_BUFFER_SIZE) _phase-=OSC_BUFFER_SIZE;
    
    _index = floor(_phase);
    
    if (_index < OSC_BUFFER_SIZE-1) {
        return _buffer[_index] * (1.0 - (_phase - _index)) + _buffer[_index+1] * (_phase - _index);
    }
    else if (_index == OSC_BUFFER_SIZE-1) {
        return _buffer[_index] * (1.0 - (_phase - _index)) + _buffer[0] * (_phase - _index);
    }
    
    else { return 0;}
    
}

template <typename T>
void ofxHoaOscillator<T>::setFrequency(T frequency) {
    
    _freq = frequency;
}

template <typename T>
void ofxHoaOscillator<T>::setPhase(T phase) {
    
    _phase = phase * OSC_BUFFER_SIZE;
}

#endif
