/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 12 Jan 2024 3:31:32pm
    Author:  JAMIUL ISLAM

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent() {

    // dummy data
    trackTitles.push_back("Track 1");
    trackTitles.push_back("Track 2");
    trackTitles.push_back("Track 3");
    trackTitles.push_back("Track 4");
    trackTitles.push_back("Track 5");
    trackTitles.push_back("Track 6");

    tableComponent.getHeader().addColumn("Track title", 1, 500);
    tableComponent.getHeader().addColumn("", 2, 100);

    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);
}

PlaylistComponent::~PlaylistComponent() {
}

void PlaylistComponent::paint(juce::Graphics &g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText("PlaylistComponent", getLocalBounds(),
               juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized() {
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows() {
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics &g, int rowNumber, int width, int height, bool rowIsSelected) {
    if (rowIsSelected) {
        g.fillAll(Colours::orange);
    } else {
        g.fillAll(Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, true);
}

Component *PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) {
    if (columnId == 2) {
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton{"play"};

            String id{std::to_string(rowNumber)};
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }

    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button *button) {
    int id = std::stoi(button->getComponentID().toStdString());
    std::cout << "PlaylistComponent::buttonClicked " << trackTitles[id] << std::endl;
}