/*
  ==============================================================================

    DDL.cpp
    Created: 24 Mar 2020 10:31:34am
    Author:  Brian Roman 

  ==============================================================================
*/

#include "DDL.h"
#include <cstring>

Ddl::Ddl()
{
    mDelayInSamples = 0;
    mFeedback = 0;
    mWetLevel = 0;
    wetRawValue = 0;
    feedbackRawValue = 0;
    delayMs = 0;
    mDelayBufferSize = 0;
    mReadindex = 0;
    mWriteIndex = 0;
    mDelayBuffer = nullptr;
    interp = 0.0;
}

Ddl::~Ddl()
{
    //relese memory allocated buffer
   if(mDelayBuffer)
       delete [] mDelayBuffer;
}

void Ddl::setSize(double srate)
{
    mSampleRate =  (float) srate;
    //Set delay buffer size and allocate memory. Fixed maximum of 2 sec
    mDelayBufferSize = int(2.0*mSampleRate);
    
    if(mDelayBuffer)
        delete [] mDelayBuffer;
    
    mDelayBuffer = new float[mDelayBufferSize];
}

void Ddl::resetDelay()
{
    //set delay samples to 0
    if(mDelayBuffer)
        std::memset(mDelayBuffer, 0,mDelayBufferSize*sizeof(float));
    //reset interpolation value
    interp = 0;
}

void Ddl::calculateVariables()
{
    //calculate variables from raw percentages and delay in msec
    mFeedback = feedbackRawValue/100.0;
    mWetLevel = wetRawValue/100.0;
    mDelayInSamples = delayMs * mSampleRate / 1000.0;
    
    //set read index and wrap around circular buffer
       mReadindex = mWriteIndex - (int)mDelayInSamples;
       if(mReadindex<0)
           mReadindex += getDelayBufferSize();
   
}

void Ddl::delayLineProcessor(float* sample)
{
    float xn = *sample;
    float yn = mDelayBuffer[mReadindex];
    //if delay < 1 sample, interpolate between x and x-1
    if(mReadindex == mWriteIndex && mDelayInSamples < 1.0)
        yn = xn;
    //get pointer to previous sample for interpolation
    int mReadIndex_1 = mReadindex - 1;
    if(mReadIndex_1 < 0) //wrap buffer
        mReadIndex_1 = mDelayBufferSize-1;
        
    float yn_1 = mDelayBuffer[mReadIndex_1];
    
    //first-order allpass interpolation
    //η * [x(n) - y(n-1)] + x(n-1)
    float allpassCoeff = (1.0-mDelayInSamples)/(1.0+mDelayInSamples);
    interp = allpassCoeff*(yn-interp)+yn_1;
    
    if (mDelayInSamples==0)
        yn = xn;
    else
        yn = interp;
    

    mDelayBuffer[mWriteIndex] = xn + mFeedback*yn;

    //update output sample from parameter value
    *sample = mWetLevel*yn + (1.0-mWetLevel)*xn;
    
    
    //wrap read and write head around delay buffer
    mWriteIndex++;
    if(mWriteIndex >= mDelayBufferSize)
        mWriteIndex = 0;
    mReadindex++;
    if(mReadindex >= mDelayBufferSize)
        mReadindex = 0;
}
