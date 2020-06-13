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
    //initialize all wavetables to be unipolar for use as lfo
    for (int i=0; i<1024; ++i) {
        mSinTable[i] = ((sin((float)i*2.0f*M_PI/1024.0f))/2.0)+0.5;
        mSawTable[i] = (float)i/1024.0;
        float val = (2.0f*((float)i/1024.0f))-1.0f;
        if(val<0.0) val = -val;
        mTriTable[i] = val;
        mSqTable[i] = i < 512 ? 1.0 : 0.0;
    }
    
    mIncr = 0;
    mReadIndex =0.0;
    mPhaseReadindex =256.0;
}

WTOsc::~WTOsc()
{
}

void WTOsc::oscillate(float* value, float* offsetValue)
{
    //values to output from function
    float sample = 0;
    float offsetSample = 0;
    
    //get integer part for lin interp
    int intReadIndex = (int)mReadIndex;
    int intPhaseReadIndex = (int)mPhaseReadindex;
    //fractional part
    float frac = mReadIndex - intReadIndex;
    
    //wrap around table skipping last sample
    int nextReadIndex = intReadIndex + 1 > 1023 ? 0 : intReadIndex + 1;
    int nextPhaseReadIndex = intPhaseReadIndex + 1 > 1023 ? 0 : intPhaseReadIndex + 1;
    
    switch (lfoType) {
            //get linear interpolated value of lfo for each read index
        case SINE:
            sample = mSinTable[intReadIndex]*(1.0f-frac) + mSinTable[nextReadIndex]*frac;
            offsetSample = mSinTable[intPhaseReadIndex]*(1.0f-frac) + mSinTable[nextPhaseReadIndex]*frac;
            break;
        case SAW:
            sample = mSawTable[intReadIndex]*(1.0f-frac) + mSawTable[nextReadIndex]*frac;
            offsetSample = mSawTable[intPhaseReadIndex]*(1.0f-frac) + mSawTable[nextPhaseReadIndex]*frac;
            break;
        case TRIANGLE:
            sample = mTriTable[intReadIndex]*(1.0f-frac) + mTriTable[nextReadIndex]*frac;
            offsetSample = mTriTable[intPhaseReadIndex]*(1.0f-frac) + mTriTable[nextPhaseReadIndex]*frac;
            break;
        case SQUARE:
            sample = mSqTable[intReadIndex]*(1.0f-frac) + mSqTable[nextReadIndex]*frac;
            offsetSample = mSqTable[intPhaseReadIndex]*(1.0f-frac) + mSqTable[nextPhaseReadIndex]*frac;
            break;
            
        default:
            sample = mSinTable[intReadIndex]*(1.0f-frac) + mSinTable[nextReadIndex]*frac;
            offsetSample = mSinTable[intPhaseReadIndex]*(1.0f-frac) + mSinTable[nextPhaseReadIndex]*frac;
            break;
    }
    //wrap both read pointers around wavetable
    mReadIndex += mIncr;
    mPhaseReadindex += mIncr;
    if(mReadIndex>=1024)
        mReadIndex -= 1024;
    if(mPhaseReadindex>=1024)
        mPhaseReadindex -= 1024;
    
    //update output value parameters
    *value = sample;
    *offsetValue = offsetSample;
}


