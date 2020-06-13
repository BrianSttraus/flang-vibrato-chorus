/*
  ==============================================================================

    WTOsc.h
    Created: 16 Apr 2020 4:25:00pm
    Author:  Brian Roman 

  ==============================================================================
*/

#pragma once
enum OSCTYPE{TRIANGLE,SINE,SAW,SQUARE};
//Unipolar wavetable Oscillator for use as LFO
class WTOsc {
    float mSinTable[1024];
    float mSawTable[1024];
    float mTriTable[1024];
    float mSqTable[1024];
    
    float mIncr;
    float mReadIndex,mPhaseReadindex;
    
    int lfoType{0};
public:
    WTOsc();
    ~WTOsc();
    
    void updateInc(float freq, float srate){mIncr = 1024.0f * freq / srate;}; //calculates read increment of table
    void resetIndex(){mReadIndex=0.0; mPhaseReadindex=32.0;}; //read offset of 32 samples
    void oscillate(float* value,float* offsetValue);
    void setOscType(int type){lfoType = type;};
};
