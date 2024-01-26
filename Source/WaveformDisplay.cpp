/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 12 Jan 2024 11:18:28am
    Author:  JAMIUL ISLAM

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

WaveformDisplay::~WaveformDisplay() {
}

void WaveformDisplay::paint(Graphics &g) {
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(Colours::orange);
    if (fileLoaded) {
        audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);
        g.setColour(Colours::lightgreen);
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
    } else {
        g.setFont(20.0f);
        g.drawText("load a file", getLocalBounds(), Justification::centred, true);
    }
}

void WaveformDisplay::resized() {

}

void WaveformDisplay::loadURL(URL audioURL) {
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
    if (fileLoaded) {
        std::cout << "wfd: loaded! " << std::endl;
        repaint();
    } else {
        std::cout << "wfd: not loaded! " << std::endl;
    }
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster *source) {
    std::cout << "wfd: change received! " << std::endl;
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos) {
    if (pos != position && pos > 0) {
        position = pos;
        repaint();
    }
}