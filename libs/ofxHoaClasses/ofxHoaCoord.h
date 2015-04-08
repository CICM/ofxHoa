/*
 // Copyright (c) 2012-2015 Eliott Paris, Julien Colafrancesco, Pierre Guillot & Claudio Cabral, CICM, Universite Paris 8.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */
#ifndef __OFX_HOA_COORD__
#define __OFX_HOA_COORD__

#include "../libs/HoaLibrary-Light/Sources/Hoa.hpp"
#include "ofMain.h"

namespace hoa {
template <Dimension D, typename T> class ofxHoaCoord{

private:

    PolarLines<D, T> _line;
    ofVec3f _ambisonicCenter;
    ofVec3f _lookingAt;
    T * _lineBuffer;
    int _numberOfSources;

public:
    
    ofxHoaCoord<D, T>(int numberOfSources);
    ~ofxHoaCoord<D, T>();
    
    inline void setSourcePosition(int index, ofVec3f position);
    inline void setSourcePosition(int index, float x, float y, float z);
    inline void setSourcePositionDirect(int index, ofVec3f position);
    inline void setSourcePositionDirect(int index, float x, float y, float z);
    void setAmbisonicCenter(ofVec3f position);
    void setAmbisonicRadius(float radius);

    void setRamp(int samples);
    void setRamp(int milliseconds, int sampleRate);
    
    inline T getAzimuth(int index);
    inline T getElevation(int index);
    inline T getRadius(int index);
    
    inline void process();
  
    
};


template <typename T> class ofxHoaCoord<Hoa2d, T>{

private:

    PolarLines<Hoa2d, T> _line;
    ofVec3f  _ambisonicCenter;
    float _ambisonicRadius;
    T * _lineBuffer;
    int _numberOfSources;
    
public:
    
    ofxHoaCoord<Hoa2d, T>(int numberOfSources) : _line(numberOfSources){
        _lineBuffer = new T[numberOfSources*2];
        memset(_lineBuffer, 0, sizeof(T)*numberOfSources*2);
        _numberOfSources = numberOfSources;
    }
    
    ~ofxHoaCoord<Hoa2d, T>(){
        if (_lineBuffer) {
            delete [] _lineBuffer;
        }
    }
    
    inline void setSourcePosition(int index, ofVec3f position){

        _line.setRadius(index, Math<T>::radius(position.x - _ambisonicCenter.x,
                    (ofGetHeight() - position.y) - _ambisonicCenter.y) * 1.0/_ambisonicRadius);
        
        _line.setAzimuth(index, Math<T>::azimuth(position.x - _ambisonicCenter.x,
                                          (ofGetHeight() - position.y) - _ambisonicCenter.y));
    }

    inline void setSourcePosition(int index, float x, float y, float z  = 0.){
    
        _line.setRadius(index, Math<T>::radius(x - _ambisonicCenter.x,
                                               (ofGetHeight() - y) - _ambisonicCenter.y) * 1.0/_ambisonicRadius);
        
        _line.setAzimuth(index, Math<T>::azimuth(x - _ambisonicCenter.x,
                                                 (ofGetHeight() - y) - _ambisonicCenter.y));
    
    }
    
    inline void setSourcePositionDirect(int index, ofVec3f position){
        _line.setRadiusDirect(index, Math<T>::radius(position.x - _ambisonicCenter.x,
                                               (ofGetHeight() - position.y) - _ambisonicCenter.y) * 1.0/_ambisonicRadius);
        
        _line.setAzimuthDirect(index, Math<T>::azimuth(position.x - _ambisonicCenter.x,
                                                 (ofGetHeight() - position.y) - _ambisonicCenter.y));
    }
    
    inline void setSourcePositionDirect(int index, float x, float y, float z = 0.){
        
        _line.setRadiusDirect(index, Math<T>::radius(x - _ambisonicCenter.x,
                                               (ofGetHeight() - y) - _ambisonicCenter.y) * 1.0/_ambisonicRadius);
        
        _line.setAzimuthDirect(index, Math<T>::azimuth(x - _ambisonicCenter.x,
                                                 (ofGetHeight() - y) - _ambisonicCenter.y));
    }
    
    void setAmbisonicCenter(ofVec3f position){
        _ambisonicCenter = position;
    }
    
    void setAmbisonicRadius(float radius){
        _ambisonicRadius = radius;
    }
    
    void setRamp(int samples){
        _line.setRamp(samples);
    }
    void setRamp(int milliseconds, int sampleRate){
        _line.setRamp(sampleRate/1000 * milliseconds);
    }
    
    inline T getRadius(int index){
        return _lineBuffer[index];
    }
    
    inline T getAzimuth(int index){
        return _lineBuffer[index + _numberOfSources];
    }
    
    inline void process(){
        _line.process(_lineBuffer);
    }


};

template <typename T> class ofxHoaCoord<Hoa3d, T>{

private:
    
    PolarLines<Hoa3d, T> _line;
    ofVec3f  _ambisonicCenter;
    float _ambisonicRadius;
    T * _lineBuffer;
    int _numberOfSources;
    
public:
    
    ofxHoaCoord<Hoa3d, T>(int numberOfSources) : _line(numberOfSources){
        _lineBuffer = new T[numberOfSources*3];
        memset(_lineBuffer, 0, sizeof(T)*numberOfSources*3);
        _numberOfSources = numberOfSources;
    }
    
    ~ofxHoaCoord<Hoa3d, T>(){
        if (_lineBuffer) {
            delete [] _lineBuffer;
        }
    }
    
    inline void setSourcePosition(int index, ofVec3f position){
        
        _line.setRadius(index, Math<T>::radius(position.x - _ambisonicCenter.x,
                                               (ofGetHeight() - position.y) - _ambisonicCenter.y,
                                               _ambisonicCenter.z - position.z) * 1.0/_ambisonicRadius);
        
        _line.setAzimuth(index, Math<T>::azimuth(position.x - _ambisonicCenter.x,
                                                 _ambisonicCenter.z - position.z,
                                                 (ofGetHeight() - position.y) - _ambisonicCenter.y));
        
        _line.setElevation(index, Math<T>::elevation(position.x - _ambisonicCenter.x,
                                                 _ambisonicCenter.z - position.z,
                                                 (ofGetHeight() - position.y) - _ambisonicCenter.y));
    }
    
    inline void setSourcePosition(int index, float x, float y, float z){
        
        _line.setRadius(index, Math<T>::radius(x - _ambisonicCenter.x,
                                               (ofGetHeight() - y) - _ambisonicCenter.y,
                                               _ambisonicCenter.z - z) * 1.0/_ambisonicRadius);
        
        _line.setAzimuth(index, Math<T>::azimuth(x - _ambisonicCenter.x,
                                                 _ambisonicCenter.z - z,
                                                 (ofGetHeight() - y) - _ambisonicCenter.y));
        
        _line.setElevation(index, Math<T>::elevation(x - _ambisonicCenter.x,
                                                     _ambisonicCenter.z - z,
                                                     (ofGetHeight() - y) - _ambisonicCenter.y));
    }
    
    inline void setSourcePositionDirect(int index, ofVec3f position){
        
        _line.setRadiusDirect(index, Math<T>::radius(position.x - _ambisonicCenter.x,
                                               (ofGetHeight() - position.y) - _ambisonicCenter.y,
                                               _ambisonicCenter.z - position.z) * 1.0/_ambisonicRadius);
        
        _line.setAzimuthDirect(index, Math<T>::azimuth(position.x - _ambisonicCenter.x,
                                                 _ambisonicCenter.z - position.z,
                                                 (ofGetHeight() - position.y) - _ambisonicCenter.y));
        
        _line.setElevationDirect(index, Math<T>::elevation(position.x - _ambisonicCenter.x,
                                                     _ambisonicCenter.z - position.z,
                                                     (ofGetHeight() - position.y) - _ambisonicCenter.y));
    }
    
    inline void setSourcePositionDirect(int index, float x, float y, float z){
        
        _line.setRadiusDirect(index, Math<T>::radius(x - _ambisonicCenter.x,
                                               (ofGetHeight() - y) - _ambisonicCenter.y,
                                               _ambisonicCenter.z - z) * 1.0/_ambisonicRadius);
        
        _line.setAzimuthDirect(index, Math<T>::azimuth(x - _ambisonicCenter.x,
                                                 _ambisonicCenter.z - z,
                                                 (ofGetHeight() - y) - _ambisonicCenter.y));
        
        _line.setElevationDirect(index, Math<T>::elevation(x - _ambisonicCenter.x,
                                                     _ambisonicCenter.z - z,
                                                     (ofGetHeight() - y) - _ambisonicCenter.y));
    }
    
    void setAmbisonicCenter(ofVec3f position){
        _ambisonicCenter = position;
    }
    
    void setAmbisonicRadius(float radius){
        _ambisonicRadius = radius;
    }
    
    void setRamp(int samples){
        _line.setRamp(samples);
    }
    void setRamp(int milliseconds, int sampleRate){
        _line.setRamp(sampleRate/1000 * milliseconds);
    }
    
    inline T getRadius(int index){
        return _lineBuffer[index];
    }
    
    inline T getAzimuth(int index){
        return _lineBuffer[index + _numberOfSources];
    }
    
    inline T getElevation(int index){
        return _lineBuffer[index + _numberOfSources*2];
    }
    
    inline void process(){
        _line.process(_lineBuffer);
    }
    
};
}


#endif


