#include "PitchDetectionComponent.h"
#include <stdlib.h>
#include <iostream>
#include <array>

/*
 * The PitchDetectionComponent class will act as
 * the primary component for the tuner app, and is
 * designed to be agnostic as to what pitch detection
 * algorithm it uses.
 */
PitchDetectionComponent::PitchDetectionComponent() {
    setOpaque(true);
    setAudioChannels(1, 0);
    startTimerHz(30);
    setSize(700, 500);
}

PitchDetectionComponent::~PitchDetectionComponent() {
    shutdownAudio();
    free(detector);
}

void PitchDetectionComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    std::cout<<"samples per block: "<<samplesPerBlockExpected<<std::endl;
    std::cout<<"sample rate: "<<sampleRate<<std::endl;

    detector = new PitchDetector(sampleRate);
}

/*
 * timeCallBack is called repeatedly by
 * PitchDetectionComponent's base class. The frequency
 * of the callback is determined by the value passed
 * to the startTimerHz function in the class constructor.
 */
void PitchDetectionComponent::timerCallback() {
   repaint();
}

/*
 * This function recieves packets of audio from the device's
 * audio source.
 */
void PitchDetectionComponent::getNextAudioBlock(
        const juce::AudioSourceChannelInfo& bufferToFill) {
    if (bufferToFill.buffer->getNumChannels() > 0) {
        auto* channelData = bufferToFill.buffer->
            getReadPointer(0, bufferToFill.startSample);
        
        detector->getBufferFrequency(channelData, bufferToFill.numSamples);
    }
}

/*
* Produces a juce colour object based on the cents deviance from the closest pitch.
* Since 100 cents eqals one half step, deviance exists in the range -50 to 50, and the
* returned color ranges from red to yellow to green, depending on how out of tune
* the note is.
*/
juce::Colour PitchDetectionComponent::convertCentsToColour(double deviance) {
    double absDeviance = abs(deviance);
    int red;
    int green;
    
    if (absDeviance < 25) {
        red = round(absDeviance * 10);
        green = 250;
    }
    else {
        green = round((50 - absDeviance) * 10);
        red = 250;
    }
    
    return juce::Colour(red, green, 0);
}


/*
 * Paints entire GUI, including text and color gradient.
 */
void PitchDetectionComponent::paint(juce::Graphics& g) {
    std::pair<std::string, double> note = detector->getNote();
    std::string noteName = note.first;
    double pitchDeviance = note.second;

    g.setOpacity(1.0f);
    
    juce::Colour gradientColour = convertCentsToColour(pitchDeviance);
    juce::ColourGradient gradient(juce::Colours::white, 0, getHeight() * 0.382, gradientColour, 0, getHeight() * 0.764, false);
    
    g.setGradientFill(gradient);
    g.fillAll();
    
    std::string sharpFlat;
    
    if (abs(pitchDeviance) < 4) {
        sharpFlat = "just right";
    }
    else if (pitchDeviance < 0) {
        sharpFlat = "flat";
    }
    else{
        sharpFlat = "sharp";
    }
    
    g.setColour(juce::Colours::black);
    g.setFont(juce::Font("Proxima Nova", 80.0f, juce::Font::bold));
    g.drawText(noteName, 0, 0, getWidth(), getHeight() * 0.382, juce::Justification::centred, true);
    
    g.setFont(juce::Font("Proxima Nova", 20.0f, juce::Font::plain));
    g.drawText(sharpFlat, 0, 100, getWidth(), getHeight() * 0.382, juce::Justification::centred, true);
}
