/*
  ==============================================================================

    DeckGUI.cpp
    Created: 11 Jan 2024 9:26:59pm
    Author:  JAMIUL ISLAM

 * Tasks:
 * 
 * 1. Add buttons and sliders for GUI items - DONE
 * 2. Position sliders for each GUI - DONE
 * 3. Set the range, styles, and default values for sliders - DONE
 * 4. Add listeners to buttons and sliders - DONE
 * 5. Implement the paint method for GUI components - DONE
 * 6. Resize and position GUI components based on the layout - DONE
 * 7. Handle button clicks for play, stop, and next - DONE
 * 8. Handle slider value changes for volume, speed, and position - DONE
 * 9. Update the content of the upNext table - DONE
 * 10. Implement paint methods for row background and cell in upNext table - DONE
 * 11. Implement the timer callback to update waveform display position - DONE
 *

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer *_player, PlaylistComponent *_playlistComponent, AudioFormatManager &formatManagerToUse,
                 AudioThumbnailCache &cacheToUse, int channelToUse
) : player(_player), playlistComponent(_playlistComponent), waveformDisplay(formatManagerToUse, cacheToUse),
    channel(channelToUse) {

    // add buttons for each GUI items
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(nextButton);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(upNext);

    // add listeners to buttons and sliders
    playButton.addListener(this);
    stopButton.addListener(this);
    nextButton.addListener(this);
    posSlider.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);

    // position slider for each GUI
    posSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    posSlider.setRange(0.0, 1.0);
    posSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

    // volume slider for each GUI
    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(0.5); //default volume half of max vol
    volSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    volSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

    // label for volume slider
    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.attachToComponent(&volSlider, false);
    volLabel.setJustificationType(juce::Justification::centred);

    // speed slider for each GUI
    speedSlider.setRange(0.5, 2, 0);
    speedSlider.setValue(1);
    speedSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
    speedSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    speedSlider.setMouseDragSensitivity(80);

    // label for speed slider
    speedLabel.setText("Speed X", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, false);
    speedLabel.setJustificationType(juce::Justification::centred);

    // set colour to sliders
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::cornflowerblue); //dial
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::lightslategrey); //body
    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::lightslategrey); //body

    upNext.getHeader().addColumn("Up Next", 1, 100);
    upNext.setModel(this);

    startTimer(100);
}

DeckGUI::~DeckGUI() {
    stopTimer();
}

void DeckGUI::paint(Graphics &g) {}

void DeckGUI::resized() {
    double rowH = getHeight() / 6;
    double colW = getWidth() / 4;

    waveformDisplay.setBounds(0, 0, getWidth(), rowH * 2);

    posSlider.setBounds(0, rowH * 2, getWidth(), rowH);
    volSlider.setBounds(0, rowH * 3 + 20, colW, rowH * 3 - 30);
    speedSlider.setBounds(colW, rowH * 3 + 20, colW * 1.5, rowH * 2 - 30);

    upNext.setBounds(colW * 2.5, rowH * 3, colW * 1.5 - 20, rowH * 2);

    playButton.setBounds(colW + 10, rowH * 5 + 10, colW - 20, rowH - 20);
    stopButton.setBounds(colW * 2 + 10, rowH * 5 + 10, colW - 20, rowH - 20);
    nextButton.setBounds(colW * 3 + 10, rowH * 5 + 10, colW - 20, rowH - 20);

}

void DeckGUI::buttonClicked(Button *button) {
    if (button == &playButton) {
        player->start(); // start playing
    }
    if (button == &stopButton) {
        player->stop(); // stop playing
    }
    if (button == &nextButton) {
        if (channel == 0 && playlistComponent->playListL.size() > 0) { // if left deck and playlist is not empty
            // load the first song in the playlist
            URL fileURL = URL{File{playlistComponent->playListL[0]}};
            // load the waveform display
            player->loadURL(fileURL);
            // load the waveform display
            waveformDisplay.loadURL(fileURL);
            // remove the first song from the playlist
            playlistComponent->playListL.erase(playlistComponent->playListL.begin());
        }
        if (channel == 1 && playlistComponent->playListR.size() > 0) { // if right deck and playlist is not empty
            // do the same like left deck ...
            URL fileURL = URL{File{playlistComponent->playListR[0]}};
            player->loadURL(fileURL);
            waveformDisplay.loadURL(fileURL);
            playlistComponent->playListR.erase(playlistComponent->playListR.begin());
        }

        if (nextButton.getButtonText() == "LOAD") {
            nextButton.setButtonText("NEXT");
        } else {
            player->start(); // start playing when next button is clicked
        }
    }

    // at last, update the content of the upNext table
    upNext.updateContent();
}

void DeckGUI::sliderValueChanged(Slider *slider) {
    if (slider == &volSlider) {
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider) {
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider) {
        player->setPositionRelative(slider->getValue());
    }
}

int DeckGUI::getNumRows() {
    if (channel == 0) {
        return playlistComponent->playListL.size(); // left deck
    }
    if (channel == 1) {
        return playlistComponent->playListR.size(); // right deck
    }
}

void DeckGUI::paintRowBackground(Graphics &g, int rowNumber, int width, int height, bool rowIsSelected) {
    g.fillAll(juce::Colours::cornflowerblue);
}

void DeckGUI::paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    std::string filepath = "";

    if (channel == 0) {
        filepath = playlistComponent->playListL[rowNumber];
    }
    if (channel == 1) {
        filepath = playlistComponent->playListR[rowNumber];
    }

    std::size_t startFilePos = filepath.find_last_of("\\");
    std::size_t startExtPos = filepath.find_last_of(".");
    std::string extn = filepath.substr(startExtPos + 1, filepath.length() - startExtPos);
    std::string file = filepath.substr(startFilePos + 1, filepath.length() - startFilePos - extn.size() - 2);

    g.drawText(file, 1, rowNumber, width - 4, height, Justification::centredLeft, true);
}

void DeckGUI::timerCallback() {
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}