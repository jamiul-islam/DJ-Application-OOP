/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 9 Jan 2024 2:41:38pm
    Author:  JAMIUL ISLAM

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

class DJAudioPlayer : public juce::AudioSource {
public:

    // Constructor
    DJAudioPlayer(AudioFormatManager& _formatManager);
    // Destructor
    ~DJAudioPlayer();

    //inner layer
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
    void releaseResources() override;

    // outer layer
    /** load a URL into the transportSource */
    void loadURL(URL audioURL);
    /** set the gain of the transportSource */
    void setGain(double gain); // gain : 0 -> 1
    /** set the speed of the transportSource */
    void setSpeed(double ratio); // speed : 0 -> 100

    // positioning
    /** set the position of the playHead */
    void setPosition(double posInSecs);
    /** get the position of the playHead */
    void setPositionRelative(double pos);
    /** get the relative position of the playHead */
    double getPositionRelative();

    // controls
    void start();
    void stop();

private:

    AudioFormatManager& formatManager; // reference to the formatManager
    std::unique_ptr<AudioFormatReaderSource> readerSource; // pointer to the readerSource
    AudioTransportSource transportSource; // transportSource initialization
    ResamplingAudioSource resamplingSource{&transportSource, false, 2}; // resamplingSource initialization
};