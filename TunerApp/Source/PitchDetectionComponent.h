#pragma once

#include <array>
#include <vector>
#include <utility>
#include <cmath>
#include <math.h>
#include "PitchDetector.h"
#include "JuceHeader.h"

class PitchDetectionComponent : public juce::AudioAppComponent, juce::Timer {
public:
    PitchDetectionComponent();
    ~PitchDetectionComponent();
    
    void timerCallback() override;
    
    void prepareToPlay (int, double) override;
    void releaseResources() override {};
    
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    
    juce::Colour convertCentsToColour(double deviance);
    
    void paint (juce::Graphics& g) override;
    
private:
    PitchDetector* detector;
};

