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
PlaylistComponent::PlaylistComponent(AudioFormatManager &_formatManager) : formatManager(_formatManager) {

    //set up playlist library table
    tableComponent.getHeader().addColumn("Song Title", 1, 250);
    tableComponent.getHeader().addColumn("Duration", 2, 100);
    tableComponent.getHeader().addColumn("+ to Left", 3, 100);
    tableComponent.getHeader().addColumn("+ to Right", 4, 100);
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);

    //add search bar and listener
    addAndMakeVisible(searchBar);
    searchBar.addListener(this);
    //add label for search bar
    addAndMakeVisible(searchLabel);
    searchLabel.setText("Find Song: ", juce::dontSendNotification);
}

PlaylistComponent::~PlaylistComponent() {}

void PlaylistComponent::paint(juce::Graphics &g) {}

void PlaylistComponent::resized() {
    double rowH = getHeight() / 8;
    double colW = getWidth() / 6;

    searchLabel.setBounds(0, 0, colW, rowH);
    searchBar.setBounds(colW, 0, colW * 5, rowH);
    tableComponent.setBounds(0, rowH, getWidth(), rowH * 7);
}

int PlaylistComponent::getNumRows() {
    return interestedTitle.size();
}

void PlaylistComponent::paintRowBackground(Graphics &g, int rowNumber, int width, int height, bool rowIsSelected) {
    if (rowIsSelected) {
        g.fillAll(Colours::orange);
    } else {
        g.fillAll(Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    // song title in the first column
    if (columnId == 1) {
        g.drawText(interestedTitle[rowNumber], 1, rowNumber, width - 4, height, Justification::centredLeft, true);
    }

    // song duration in the second column
    if (columnId == 2) {
        g.drawText(std::to_string(interestedDuration[rowNumber]) + "s", 1, rowNumber, width - 4, height,
                   Justification::centredLeft, true);
    }
}

Component *PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected,
                                                      Component *existingComponentToUpdate) {
    // buttons for left deck
    if (columnId == 3) {
        if (existingComponentToUpdate == nullptr) {
            TextButton *btn = new TextButton{"+ to Left"};
            String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
            btn->setColour(TextButton::buttonColourId, juce::Colours::darkslategrey);
        }
    }

    // buttons for right deck
    if (columnId == 4) {
        if (existingComponentToUpdate == nullptr) {
            TextButton *btn = new TextButton{"+ to Right"};
            String id{std::to_string(rowNumber + 1000)};
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
            btn->setColour(TextButton::buttonColourId, juce::Colours::darkslategrey);
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {}

void PlaylistComponent::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) {}

void PlaylistComponent::releaseResources() {}

void PlaylistComponent::buttonClicked(Button *button) {
    int id = std::stoi(button->getComponentID().toStdString());
    if (id < 1000) {
        addToChannelList(interestedFiles[id], 0);
    } else {
        addToChannelList(interestedFiles[id - 1000], 1);
    }
}

bool PlaylistComponent::isInterestedInFileDrag(const StringArray &files) {
    return true;
}

void PlaylistComponent::filesDropped(const StringArray &files, int x, int y) {
    for (String filename: files) {
        std::string songPath = String(filename).toStdString();
        std::size_t audioFilePosStart = songPath.find_last_of("\\");
        std::size_t audioFilePosEnd = songPath.find_last_of(".");
        std::string extn = songPath.substr(audioFilePosEnd + 1, songPath.length() - audioFilePosEnd);
        std::string file = songPath.substr(audioFilePosStart + 1,
                                           songPath.length() - audioFilePosStart - extn.size() - 2);

        inputFiles.push_back(songPath);
        trackTitles.push_back(file);

        getAudioLength(URL{File{songPath}});
    }

    interestedTitle = trackTitles;
    interestedFiles = inputFiles;

    // update playlist table
    tableComponent.updateContent();
}

void PlaylistComponent::textEditorTextChanged(TextEditor &textEditor) {
    // clear table on change
    interestedTitle.clear();
    interestedDuration.clear();
    interestedFiles.clear();

    int pos = 0;
    for (std::string track: trackTitles) {
        // check substring of the song name
        if (track.find(searchBar.getText().toStdString()) != std::string::npos) {
            interestedTitle.push_back(trackTitles[pos]);
            interestedDuration.push_back(trackDurations[pos]);
            interestedFiles.push_back(inputFiles[pos]);
        }

        ++pos;
    }

    // update lists after iterating through all songs
    tableComponent.updateContent();
}

void PlaylistComponent::addToChannelList(std::string filepath, int channel) {
    if (channel == 0) {
        playListL.push_back(filepath); // add to left channel
    } else {
        playListR.push_back(filepath); // add to right channel
    }
}

void PlaylistComponent::getAudioLength(URL audioURL) {
    double songLength = 0.0;
    auto *reader = formatManager.createReaderFor(audioURL.createInputStream(false));

    if (reader != nullptr) // good file!
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
        double songLength = transportSource.getLengthInSeconds();
        trackDurations.push_back(songLength);
    }

    interestedDuration = trackDurations;
}