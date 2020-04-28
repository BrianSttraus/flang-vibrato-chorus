/*
  ==============================================================================

    WTOsc.h
    Created: 16 Apr 2020 4:25:00pm
    Author:  Brian Roman 

  ==============================================================================
*/

#pragma once
enum OSCTYPE{SINE,SAW,TRIANGLE,SQUARE};

class WTOsc {
    float mSinTable[1024];
    float mSawTable[1024];
    float mTriTable[1024];
    float mSqTable[1024];
    
    float mIncr;
    float mReadIndex{0};
    
    int lfoType{0};
public:
    WTOsc();
    ~WTOsc();
    
    void updateInc(float freq, float srate){mIncr = 1024.0f * freq / srate;};
    void resetIndex(){mReadIndex=0.0;};
    void oscillate(float* value);
    void setOscType(int type){lfoType = type;};
};
