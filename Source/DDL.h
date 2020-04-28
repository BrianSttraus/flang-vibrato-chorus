/*
  ==============================================================================

    DDL.h
    Created: 24 Mar 2020 10:31:34am
    Author:  Brian Roman 

  ==============================================================================
*/

#pragma once

class Ddl
{
public:
    Ddl();
    ~Ddl();
    
    void calculateVariables();
    void setSize(double srate);
    void resetDelay();
    
    int getDelayBufferSize(){return mDelayBufferSize;};
    void delayLineProcessor(float* sample);
    
    float feedbackRawValue, wetRawValue, delayMs;
private:
    float mDelayInSamples, mFeedback, mWetLevel;
    int mReadindex, mWriteIndex;
    float* mDelayBuffer;
    int mDelayBufferSize;
    float mSampleRate;
    float interp;
    float mDelayTimeSmoothed;
    
};
