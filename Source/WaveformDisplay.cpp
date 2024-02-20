/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 12 Jan 2024 11:18:28am
    Author:  JAMIUL ISLAM

 * Tasks:
 *
 * 1. Initialize the WaveformDisplay with an AudioThumbnail - DONE
 * 2. Implement the paint method to display waveform, playhead, and track name - DONE
 * 3. Implement the resized method - DONE
 * 4. Load an audio file from the provided URL - DONE
 * 5. Handle changeListenerCallback to repaint on changes - DONE
 * 6. Set the relative position of the playhead - DONE
 *

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager &formatManagerToUse, AudioThumbnailCache &cacheToUse) :
        audioThumb(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0) {

    // audioThumb.addChangeListener(this);
    audioThumb.addChangeListener(reinterpret_cast<ChangeListener *>(this));
}

WaveformDisplay::~WaveformDisplay() {}

// ***********************************************
// *********** SELF WRITTEN CODE START ***********
// ********** slight modification on UI **********
// ***********************************************
void WaveformDisplay::paint(Graphics &g) {
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    if (fileLoaded) {
        g.setColour(Colours::orange);
        audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);

        //draw the playHead
        g.setColour(juce::Colours::orangered);
        g.fillRect(position * getWidth(), 0, 2, getHeight());

        //display name of currently playing track on the waveform in white
        g.setColour(juce::Colours::floralwhite);
        g.setFont(16.0f);
        g.drawText(currentlyPlaying, getLocalBounds(), juce::Justification::centred, true);
    } else {
        g.setColour(juce::Colours::cornflowerblue);
        g.setFont(20.0f);
        g.drawText("load a file", getLocalBounds(), Justification::centred, true);
    }
}
// ***********************************************
// *********** SELF WRITTEN CODE END *************
// ***********************************************

void WaveformDisplay::resized() {}

// ***********************************************
// *********** SELF WRITTEN CODE START ***********
// ***********************************************
void WaveformDisplay::loadURL(URL audioURL) {
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
    if (fileLoaded) {
        std::string audioFile = audioURL.toString(false).toStdString();
        std::size_t audioFilePosStart = audioFile.find_last_of("/");
        std::size_t audioFilePosEnd = audioFile.find_last_of(".");
        std::string extn = audioFile.substr(audioFilePosEnd + 1, audioFile.length() - audioFilePosEnd);
        std::string file = audioFile.substr(audioFilePosStart + 1, audioFile.length() - audioFilePosStart - extn.size() - 2);

        currentlyPlaying = file;
        repaint();
    } else {
        std::cout << "file not loaded yet! " << std::endl;
    }
}
// ***********************************************
// *********** SELF WRITTEN CODE END ***********
// ***********************************************

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster *source) {
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos) {
    if (pos != position && pos > 0) {
        position = pos; // first update the position
        repaint(); // then perform repaint
    }
}
