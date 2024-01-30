/*
  ==============================================================================

    WaveformDisplay.h
    Created: 12 Jan 2024 11:18:28am
    Author:  JAMIUL ISLAM

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

/**
 * @class WaveformDisplay
 * @brief A component for displaying the waveform of an audio file and managing playhead position.
 *
 * This class provides functionality to load and display the waveform of an audio file.
 * It includes features to dynamically update the playhead position and display the name
 * of the currently playing song.
 */
class WaveformDisplay : public juce::Component, public ChangeListener {
public:
    /**
     * @brief Constructor.
     * @param formatManagerToUse The audio format manager to use.
     * @param cacheToUse The audio thumbnail cache to use.
     */
    WaveformDisplay(AudioFormatManager &formatManagerToUse, AudioThumbnailCache &cacheToUse);

    /** Destructor. */
    ~WaveformDisplay();

    /**
     * @brief Paint method for rendering the component.
     * @param g The Graphics object used for rendering.
     */
    void paint(juce::Graphics &g) override;

    /**
     * @brief Resized method called when the component is resized.
     */
    void resized() override;

    /**
     * @brief Load an audio file from a given URL.
     * @param audioURL The URL of the audio file to load.
     */
    void loadURL(URL audioURL);

    /**
     * @brief Callback method for change events.
     * @param source The ChangeBroadcaster triggering the change.
     */
    void changeListenerCallback(ChangeBroadcaster *source) override;

    /**
     * @brief Set the relative position of the playhead.
     * @param pos The relative position (0.0 to 1.0) of the playhead.
     */
    void setPositionRelative(double pos);

private:
    AudioThumbnail audioThumb; /**< Audio thumbnail for waveform display. */
    bool fileLoaded; /**< Flag indicating whether an audio file is loaded. */
    double position; /**< Relative position of the playhead. */
    std::string currentlyPlaying; /**< Name of the currently playing song. */

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
