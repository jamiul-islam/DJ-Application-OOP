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

/**
 * @class DJAudioPlayer
 * @brief Audio player for handling audio playback.
 *
 * This class represents an audio player that utilizes JUCE's AudioSource interface.
 * It provides methods for preparing to play, getting the next audio block, releasing resources,
 * loading audio from a URL, setting gain and speed, and controlling playback.
 */
class DJAudioPlayer : public juce::AudioSource {
public:
    /** Constructor.
     *  @param _formatManager The audio format manager reference.
     */
    DJAudioPlayer(AudioFormatManager& _formatManager);

    /** Destructor. */
    ~DJAudioPlayer();

    /**
     * @brief Prepare to play audio.
     * @param samplesPerBlockExpected The number of samples per block expected.
     * @param sampleRate The sample rate of the audio.
     */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    /**
     * @brief Get the next audio block for playback.
     * @param bufferToFill The buffer to fill with audio data.
     */
    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;

    /** Release audio resources. */
    void releaseResources() override;

    /**
     * @brief Load audio from a URL into the transport source.
     * @param audioURL The URL of the audio file.
     */
    void loadURL(URL audioURL);

    /**
     * @brief Set the gain of the transport source.
     * @param gain The gain value (0 to 1).
     */
    void setGain(double gain);

    /**
     * @brief Set the speed of the transport source.
     * @param ratio The speed ratio (0 to 100).
     */
    void setSpeed(double ratio);

    /**
     * @brief Set the position of the playHead.
     * @param posInSecs The position in seconds.
     */
    void setPosition(double posInSecs);

    /**
     * @brief Set the position of the playHead relative to the length of the audio.
     * @param pos The relative position (0 to 1).
     */
    void setPositionRelative(double pos);

    /**
     * @brief Get the relative position of the playHead.
     * @return The relative position of the playHead.
     */
    double getPositionRelative();

    /** Start playback. */
    void start();

    /** Stop playback. */
    void stop();

private:
    AudioFormatManager& formatManager; /**< Reference to the formatManager. */
    std::unique_ptr<AudioFormatReaderSource> readerSource; /**< Pointer to the readerSource. */
    AudioTransportSource transportSource; /**< TransportSource initialization. */
    ResamplingAudioSource resamplingSource{&transportSource, false, 2}; /**< ResamplingSource initialization. */
};
