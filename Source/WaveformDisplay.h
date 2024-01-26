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
    void changeListenerCallback (ChangeBroadcaster *source);
    void loadURL(URL audioURL);

/** set the relative position of the playHead*/
void setPositionRelative(double pos);

private:
    AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
