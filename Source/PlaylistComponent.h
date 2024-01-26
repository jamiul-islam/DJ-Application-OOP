/*
  ==============================================================================

    PlaylistComponent.h
    Created: 12 Jan 2024 3:31:32pm
    Author:  JAMIUL ISLAM

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

class PlaylistComponent : public juce::Component, public TableListBoxModel, public Button::Listener {
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint(juce::Graphics &) override;
    void resized() override;

    // functions for playlist
    int getNumRows() override;
    void paintRowBackground(Graphics &, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics &, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;

    void buttonClicked(Button* button) override;
private:
    TableListBox tableComponent;
    std::vector<std::string> trackTitles;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
