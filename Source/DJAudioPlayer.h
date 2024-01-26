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
    void loadURL(URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);

    // positioning
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    /** get the relative position of the playHead */
    double getPositionRelative();

    // controls
    void start();
    void stop();

private:
    /** First Layer */
    AudioFormatManager& formatManager;

    /**
     * Second Layer, instantiate the playlist, ready for user to choose file,
     * Use smart pointer: better memory management
     * */
    std::unique_ptr<AudioFormatReaderSource> readerSource;

    /** Control the audio playback, go back and forward, stop and play */
    AudioTransportSource transportSource;

    /** resampling audio */
    ResamplingAudioSource resamplingSource{&transportSource, false, 2};
};