//
//  ofxHoaOscillator.h
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
//  Created by Claudio Cabral on 21/03/15.
//


#ifndef __OFX_HOA_OSCILLATOR__
#define __OFX_HOA_OSCILLATOR__

#define OSC_BUFFER_SIZE 4096

#include "../HoaLibrary-light/Sources/Hoa.hpp"


namespace hoa {

    enum oscType {  OF_SINE_WAVE = 0,
                    OF_TRIANGLE_WAVE = 1,
                    OF_SAWTOOTH_WAVE = 2,
                    OF_SQUARE_WAVE = 3,
                    OF_PHASOR_WAVE = 4 };
    
template <typename T> class ofxHoaOscillator {
    
private:
    
    T _freq;

    T _phase;
    
    T _step;
    
    int _index;
    
    T _buffer[OSC_BUFFER_SIZE];
    
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
    
    _phase = 0;
    _index = 0;
    _freq = 440;
    memset(_buffer, 0, sizeof(T)*OSC_BUFFER_SIZE);
    _step = OSC_BUFFER_SIZE/44100.0;
    

}
template <typename T>
ofxHoaOscillator<T>::~ofxHoaOscillator(){

}
//SETUP
template <typename T>
void ofxHoaOscillator<T>::setup(int sampleRate, oscType waveform, bool normalize)
{

    _step = sampleRate;
    _step = OSC_BUFFER_SIZE/_step;
    
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

                for (float j = 1; j<=20; j++){

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
    
    if (_phase<0) _phase+=OSC_BUFFER_SIZE;
    
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
    float factor = 1.0;
    
    for (int i = 0; i<OSC_BUFFER_SIZE;i++){
        if (abs(_buffer[i])>factor) factor = abs(_buffer[i]);
    }
    for (int i = 0; i<OSC_BUFFER_SIZE;i++){
        _buffer[i]/=factor;
    }
}

#endif
