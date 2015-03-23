//
//  ofxHoaOscillator.h
//
//  Created by Claudio Cabral on 21/03/15.
//


#ifndef __OFX_HOA_OSCILLATOR__
#define __OFX_HOA_OSCILLATOR__

#define OSC_BUFFER_SIZE 4096

#include <stdio.h>
#include "../HoaLibrary-light/Sources/Defs.hpp"

//if c++11...
//#include <random>
//std::mt19937 engine(0);
//std::uniform_real_distribution<> dist;

namespace hoa {

    enum oscType { OF_SINE_WAVE, OF_TRIANGLE_WAVE, OF_SAWTOOTH_WAVE, OF_SQUARE_WAVE, OF_PHASOR_WAVE };
    
template <typename T> class ofxHoaOscillator {
    
private:
    
    T _freq;

    T _phase;
    
    T _step;
    
    T _buffer[OSC_BUFFER_SIZE];
    
    int _index, _sampleRate;
    
    void normalize();
    
public:

    ofxHoaOscillator();
    ~ofxHoaOscillator();
    
    void setup(int sampleRate, oscType waveform, bool normalize = true);
   inline void setFrequency(T frequency);
   inline void setPhase(T phase);
    
   inline T tick();
    
};
}

using namespace hoa;

template <typename T>
ofxHoaOscillator<T>::ofxHoaOscillator(){
    
    _sampleRate = 44100;
    _phase = 0;
    _index = 0;
    _freq = 440;
    memset(_buffer, 0, sizeof(T)*OSC_BUFFER_SIZE);
    _step = _sampleRate;
    _step = OSC_BUFFER_SIZE/_step;
    

}
template <typename T>
ofxHoaOscillator<T>::~ofxHoaOscillator(){

}
//SETUP
template <typename T>
void ofxHoaOscillator<T>::setup(int sampleRate, oscType waveform, bool normalize)
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

                for (float j = 1; j<=10; j++){

                _buffer[i] += sin (i * j * HOA_2PI/OSC_BUFFER_SIZE )/j;
                }

                break;
                
            case OF_SQUARE_WAVE:
                
                for (float j = 1; j<=20; j+=2){
                    
                    _buffer[i] += sin (i * j * HOA_2PI/OSC_BUFFER_SIZE )/j;
                }
                
                break;
                
            case OF_PHASOR_WAVE:
                
                _buffer[i] = i * 1.0/OSC_BUFFER_SIZE;
                break;
                
            default:
                _buffer[i] = 0;
                break;
        }
        
    }
    if (normalize) ofxHoaOscillator<T>::normalize();
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

template<typename T>
void ofxHoaOscillator<T>::normalize(){
    float factor = 0;
    
    for (int i = 0; i<OSC_BUFFER_SIZE;i++){
        if (_buffer[i]>factor) factor = _buffer[i];
    }
    for (int i = 0; i<OSC_BUFFER_SIZE;i++){
        _buffer[i]/=factor;
    }
}

#endif
