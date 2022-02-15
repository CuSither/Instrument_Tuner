#include <stdio.h>
#include <utility>
#include <vector>
//#include <cmath>
#include <math.h>
#include "JuceHeader.h"

class PitchDetector {
public:
    PitchDetector(double sampleRate);
    void setSampleRate(double sampleRate);
    
    void getBufferFrequency(const float* buffer, int bufferSize);
    
    void addFrequency(double frequency);
    std::pair<std::string, double> getNote();
    
private:
    double sRate;

    int crossCounter = -1;
    int startingSample;
    int previousSign = 0;
    double prevCycle = -666;
    int zoom = 2;   // number of zero crossings per cycle
    
    //constants
    const float cycleMargin = 0.02;
    const int reset = -666;
    const int maxZoom = 8;
    
    const float freqMargin = 0.1;
    const int maxFreqWeight = 10;
    
    std::vector<std::pair<double, float>*> freqVector; // contains pairs representing a frequency its weight
    std::pair<double, float> currentFreq;
};
