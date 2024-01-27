/*
  ==============================================================================

    DeckGUI.h
    Created: 11 Jan 2024 9:26:59pm
    Author:  JAMIUL ISLAM

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"

using namespace juce;

class DeckGUI : public Component,
                public Button::Listener,
                public Slider::Listener,
                public TableListBoxModel,
                public Timer {
public:
    DeckGUI(DJAudioPlayer* player, PlaylistComponent* playlistComponent, AudioFormatManager& formatManagerToUse, AudioThumbnailCache& cacheToUse, int channelToUse);
    ~DeckGUI();

    void paint(Graphics &) override;
    void resized() override;

    /** implement Button::Listener */
    void buttonClicked(Button *) override;
    /** implement Slider::Listener */
    void sliderValueChanged(Slider *slider) override;

    /** returns row numbers in current table */
    int getNumRows() override;

    void paintRowBackground(Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    void timerCallback() override;

private:

    // buttons for play, stop, next
    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "PAUSE" };
    TextButton nextButton{ "LOAD" };

    // sliders for volume, speed, position
    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;

    // labels for volume and speed
    Label volLabel;
    Label speedLabel;

    // visual theme
    LookAndFeel_V4 lookandfeel;


    DJAudioPlayer* player;
    PlaylistComponent* playlistComponent;
    WaveformDisplay waveformDisplay;
    TableListBox upNext;

    // variable for channel (0=left, 1=right)
    int channel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};