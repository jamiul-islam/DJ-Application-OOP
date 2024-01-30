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

/**
 * @class DeckGUI
 * @brief Represents the graphical user interface for controlling an audio deck.
 *
 * This class inherits from Component and implements Button::Listener, Slider::Listener,
 * TableListBoxModel, and Timer interfaces. It provides buttons for play, stop, and load,
 * sliders for volume, speed, and position, and displays information about the playlist and waveform.
 */
class DeckGUI : public Component,
                public Button::Listener,
                public Slider::Listener,
                public TableListBoxModel,
                public Timer {
public:
    /**
     * @brief Constructor.
     * @param player Pointer to the DJAudioPlayer associated with the deck.
     * @param playlistComponent Pointer to the PlaylistComponent associated with the deck.
     * @param formatManagerToUse Reference to the audio format manager.
     * @param cacheToUse Reference to the audio thumbnail cache.
     * @param channelToUse Channel of the deck (0 for left, 1 for right).
     */
    DeckGUI(DJAudioPlayer* player, PlaylistComponent* playlistComponent, AudioFormatManager& formatManagerToUse, AudioThumbnailCache& cacheToUse, int channelToUse);

    /** Destructor. */
    ~DeckGUI();

    /** @internal */
    void paint(Graphics &) override;
    /** @internal */
    void resized() override;

    /** @internal */
    void buttonClicked(Button *) override;
    /** @internal */
    void sliderValueChanged(Slider *slider) override;

    /**
     * @brief Get the number of rows in the current table.
     * @return The number of rows in the table.
     */
    int getNumRows() override;

    /**
     * @brief Paint the background of each row in the table.
     * @param g The Graphics context.
     * @param rowNumber The row number.
     * @param width The width of the row.
     * @param height The height of the row.
     * @param rowIsSelected Whether the row is selected.
     */
    void paintRowBackground(Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;

    /**
     * @brief Paint the cells in the table.
     * @param g The Graphics context.
     * @param rowNumber The row number.
     * @param columnId The column ID.
     * @param width The width of the cell.
     * @param height The height of the cell.
     * @param rowIsSelected Whether the row is selected.
     */
    void paintCell(Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    /** @internal */
    void timerCallback() override;

private:
    // Buttons for play, stop, next
    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "PAUSE" };
    TextButton nextButton{ "LOAD" };

    // Sliders for volume, speed, position
    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;

    // Labels for volume and speed
    Label volLabel;
    Label speedLabel;

    // Visual theme
    LookAndFeel_V4 lookandfeel;

    // Variables for player, playlist, waveform
    DJAudioPlayer* player;
    PlaylistComponent* playlistComponent;
    WaveformDisplay waveformDisplay;
    TableListBox upNext;

    // Variable for channel (0=left, 1=right)
    int channel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
