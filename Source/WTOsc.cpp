/*
  ==============================================================================

    WTOsc.cpp
    Created: 16 Apr 2020 4:25:00pm
    Author:  Brian Roman 

  ==============================================================================
*/

#include "WTOsc.h"
#include <math.h>


WTOsc::WTOsc()
{
    for (int i=0; i<1024; ++i) {
        mSinTable[i] = ((sin((float)i*2.0f*M_PI/1024.0f))/2.0)+0.5;
        mSawTable[i] = (float)i/1024.0;
        float val = (2.0f*((float)i/1024.0f))-1.0f;
        if(val<0.0) val = -val;
        mTriTable[i] = val;
        mSqTable[i] = i < 512 ? 1.0 : 0.0;
    }
    
    mIncr = 0;
}

WTOsc::~WTOsc()
{
}

void WTOsc::oscillate(float* value)
{
    float sample = 0;
    int nReadIndex = (int)mReadIndex;
    float frac = mReadIndex - nReadIndex;
    
    int nextReadIndex = nReadIndex + 1 > 1023 ? 0 : nReadIndex + 1;
    
    switch (lfoType) {
        case SINE:
            sample = mSinTable[nReadIndex]*(1.0f-frac) + mSinTable[nextReadIndex]*frac;
            break;
        case SAW:
            sample = mSawTable[nReadIndex]*(1.0f-frac) + mSawTable[nextReadIndex]*frac;
            break;
        case TRIANGLE:
            sample = mTriTable[nReadIndex]*(1.0f-frac) + mTriTable[nextReadIndex]*frac;
            break;
        case SQUARE:
            sample = mSqTable[nReadIndex]*(1.0f-frac) + mSqTable[nextReadIndex]*frac;
            break;
            
        default:
            sample = mSinTable[nReadIndex]*(1.0f-frac) + mSinTable[nextReadIndex]*frac;
            break;
    }
    
    mReadIndex += mIncr;
    if(mReadIndex>=1024)
        mReadIndex -= 1024;
    
    *value = sample;
}


