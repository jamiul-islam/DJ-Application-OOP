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

class WaveformDisplay : public juce::Component, public ChangeListener {
public:
    WaveformDisplay(AudioFormatManager &formatManagerToUse, AudioThumbnailCache &cacheToUse);
    ~WaveformDisplay();

    void paint(juce::Graphics &) override;
    void resized() override;
    void loadURL(URL audioURL);

    /** listener to repaint whenever there has been a change to enable moving playhead*/
    void changeListenerCallback (ChangeBroadcaster *source);
    /** set the relative position of the playHead*/
    void setPositionRelative(double pos);

private:
    AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;
    
    /** name of the song being played*/
    std::string currentlyPlaying;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
