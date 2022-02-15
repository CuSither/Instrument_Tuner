#include "PitchDetector.h"

PitchDetector::PitchDetector(double sampleRate) {
    sRate = sampleRate;
    std::pair<double, float> tempPair;
    tempPair.first = 440;
    tempPair.second = 1;
    
    currentFreq = tempPair;
}

void PitchDetector::setSampleRate(double sampleRate) {
    sRate = sampleRate;
}

/*
 * Parses an audio buffer and adds detected frequencies to freqVector in
 * linear time.
 */
void PitchDetector::getBufferFrequency(const float* buffer, int bufferSize) {
    if (sRate == 0) {
        return;
    }
    
    int currentSign;
    double curCycle;
    double cycleFrequency;
    
    // initialize previousSign
    if (!previousSign) {
        if (buffer[0] > 0) {previousSign = 1;} else {previousSign = -1;}
    }
    
    for (int i = 0; i < bufferSize; i++) {
        if (buffer[i] > 0) {currentSign = 1;} else {currentSign = -1;}
        
        // This identifies a zero crossing
        if (currentSign != previousSign) {
            if (crossCounter == -1) {
                startingSample = i;
                crossCounter = 0;
            }
            else if (crossCounter == 0) {
                curCycle = (i - startingSample);
                startingSample = i;
                
                if (prevCycle == reset) {
                    // cycle has been reset
                    prevCycle = curCycle;
                }
                else if (curCycle < prevCycle*(1+cycleMargin) && curCycle > prevCycle*(1-cycleMargin)) {
                    // found recurrence!
                    cycleFrequency = sRate / ((curCycle + prevCycle) / 2.0f);

                    addFrequency(cycleFrequency);
                    
                    // The cycle is reset every time a recurrence is found, meaning it needs to find
                    // the number of zero crossings per cycle every time. There is probably a better way
                    // to do this.
                    zoom = 2;
                    prevCycle = reset;
                }
                else {
                    // Every time a recurrence is not found the cycle is reset. This could probably
                    // be improved by adding a zero crossing to the existing cycle as opposed to starting
                    // over with a higher zoom.
                    prevCycle = reset;
                    
                    if (zoom < maxZoom) {
                        zoom += 1;
                        startingSample = i;
                    }
                    else {
                        zoom = 2;
                    }
                }
            }
            
            // Effectively, the number of expected zero crossings in the cycle (up to maxZoom) is incremented
            // until two cycles in a row are found to have the same length, within a margin of error of cycleMargin
            crossCounter = (crossCounter + 1) % zoom;
        }
        
        previousSign = currentSign;
    }
    
    //prepare for wraparound
    startingSample = startingSample - bufferSize;
}


/*
 * Adds a frequency to a vector containing pairs of frequency values
 * and relative weight of that frequency. As the frequency is repeatedly
 * added its weight goes up, and with each addition, every other frequency
 * in the vector has its weight reduced. Negative weighted frequencies
 * are removed from the vector.
 */
void PitchDetector::addFrequency(double frequency) {
    int i = 0;
    bool foundFreq = false;
    
    while (i < freqVector.size()) {
        
        if (!foundFreq) {
            double octaveChecker = 0.5; // checks if the entry is one half and twice of frequency to account for octave jumping
            
            for (int j = 0; j < 3; j++) {
                // find if the frequency has already been added to freqVector
                if (frequency * octaveChecker < freqVector[i]->first*(1+freqMargin) && frequency * octaveChecker > freqVector[i]->first*(1-freqMargin)) {
                    // The frequency is freqVector is adjusted to account for slight changes in pitch over time
                    freqVector[i]->first = (freqVector[i]->first * freqVector[i]->second + (frequency * octaveChecker)) / ((freqVector[i]->second+1) * 1.0f);
                    
                    if (freqVector[i]->second < maxFreqWeight) {
                        freqVector[i]->second += 2;
                    }
                    else {
                        // Once the frequencies weight has reached maxFreqWeight, the current pitch is
                        // decidedly freqVector[i], and the weight is reset.
                        currentFreq = *freqVector[i];
                        freqVector[i]->second = 2;
                    }
                    
                    foundFreq = true;
                    break;
                }
                
                if (j == 0) {
                    octaveChecker = 1;
                }
                else if (j == 1) {
                    octaveChecker = 2;
                }
            }
        }
        
        freqVector[i]->second -= 0.5; // reduces weight of every momorized frequency, so as to remove old readings
        
        if (freqVector[i]->second <= 0) {
            free(freqVector[i]);
            freqVector.erase(freqVector.begin() + i);
            
            i--;
        }
        
        i++;
    }
    
    if (!foundFreq) {
        std::pair<double, float>* newEntry = new std::pair<double, float>;
        newEntry->first = frequency;
        newEntry->second = 1;
        freqVector.push_back(newEntry);
    }
    
}


/*
 * Converts currentFreq to a pair containing a note name and
 * cents deviance from that note in equal temperament tuning.
 */
std::pair<std::string, double> PitchDetector::getNote() {
    std::string noteName;
    double noteNum;
    
    noteNum = std::fmod(12 * log2((currentFreq).first/55), 12);
    
    switch((int)round(noteNum)) {
        case 0:
            noteName = "A";
            break;
        case 1:
            noteName = "A#";
            break;
        case 2:
            noteName = "B";
            break;
        case 3:
            noteName = "C";
            break;
        case 4:
            noteName = "C#";
            break;
        case 5:
            noteName = "D";
            break;
        case 6:
            noteName = "D#";
            break;
        case 7:
            noteName = "E";
            break;
        case 8:
            noteName = "F";
            break;
        case 9:
            noteName = "F#";
            break;
        case 10:
            noteName = "G";
            break;
        case 11:
            noteName = "G#";
            break;
        case 12:
            noteName = "A";
            break;
    }
    
    std::pair<std::string, double> note;
    note.first = noteName;
    note.second = (noteNum - round(noteNum)) * 100; // cents deviance from note
    
    return note;
}
