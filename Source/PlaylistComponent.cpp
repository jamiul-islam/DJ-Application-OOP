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

    // Set up playlist library table
    tableComponent.getHeader().addColumn("Song Title", 1, 250);
    tableComponent.getHeader().addColumn("Duration", 2, 100);
    tableComponent.getHeader().addColumn("+ to Left", 3, 100);
    tableComponent.getHeader().addColumn("+ to Right", 4, 100);
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);

    // Add search bar and listener
    addAndMakeVisible(searchBar);
    searchBar.addListener(this);

    // Add label for search bar
    addAndMakeVisible(searchLabel);
    searchLabel.setText("Find Song: ", juce::dontSendNotification);
}

PlaylistComponent::~PlaylistComponent() {}

void PlaylistComponent::paint(juce::Graphics &g) {}

void PlaylistComponent::resized() {
    // Adjust the layout of components when the size of the PlaylistComponent changes
    double rowH = getHeight() / 8;
    double colW = getWidth() / 6;

    searchLabel.setBounds(0, 0, colW, rowH);
    searchBar.setBounds(colW, 0, colW * 5, rowH);
    tableComponent.setBounds(0, rowH, getWidth(), rowH * 7);
}

int PlaylistComponent::getNumRows() {
    return interestedSongTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics &g, int rowNumber, int width, int height, bool rowIsSelected) {
    // Paint the background of each row in the playlist
    if (rowIsSelected) {
        g.fillAll(Colours::orange);
    } else {
        g.fillAll(Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    // Paint the cells with song titles and durations in the playlist
    if (columnId == 1) {
        g.drawText(interestedSongTitles[rowNumber], 1, rowNumber, width - 4, height, Justification::centredLeft, true);
    }

    if (columnId == 2) {
        g.drawText(std::to_string(interestedSongDuration[rowNumber]) + "s", 1, rowNumber, width - 4, height, Justification::centredLeft, true);
    }
}

Component *PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) {
    // Refresh components for the "Add to Left" and "Add to Right" buttons in the playlist
    if (columnId == 3 || columnId == 4) {
        if (existingComponentToUpdate == nullptr) {
            TextButton *btn = new TextButton{columnId == 3 ? "+ to Left" : "+ to Right"};
            String id{std::to_string(columnId == 3 ? rowNumber : rowNumber + 1000)};
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
    // Handle button clicks for adding songs to the left or right player
    int id = std::stoi(button->getComponentID().toStdString());
    if (id < 1000) {
        addToDecklList(interestedSongs[id], 0);
    } else {
        addToDecklList(interestedSongs[id - 1000], 1);
    }
}

bool PlaylistComponent::isInterestedInFileDrag(const StringArray &files) {
    // Allow the application to accept file drag and drop
    return true;
}

void PlaylistComponent::filesDropped(const StringArray &files, int x, int y) {
    // Handle files dropped into the playlist
    for (String filename: files) {
        // Extract file information and update the playlist
        std::string songPath = String(filename).toStdString(); // convert song path name to std::string
        std::size_t audioFilePosStart = songPath.find_last_of("\\"); // find the last occurrence of "\"
        std::size_t audioFilePosEnd = songPath.find_last_of("."); // find the last occurrence of "."
        std::string extn = songPath.substr(audioFilePosEnd + 1, songPath.length() - audioFilePosEnd); // get the file extension
        std::string file = songPath.substr(audioFilePosStart + 1, songPath.length() - audioFilePosStart - extn.size() - 2); // get the file name

        inputSongs.push_back(songPath); // add to input songs
        songTitles.push_back(file); // add to song titles

        getAudioLen(URL{File{songPath}}); // get the duration of the audio file
    }

    // Update playlist table with the new content
    interestedSongTitles = songTitles;
    interestedSongs = inputSongs;
    tableComponent.updateContent();
}

void PlaylistComponent::textEditorTextChanged(TextEditor &textEditor) {
    // Handle changes in the search bar text
    interestedSongTitles.clear(); // clear the interested song titles
    interestedSongDuration.clear(); // clear the interested song durations
    interestedSongs.clear(); // clear the interested songs

    int pos = 0;
    for (std::string track: songTitles) {
        // Check substring of the song name against the search bar text
        if (track.find(searchBar.getText().toStdString()) != std::string::npos) {
            interestedSongTitles.push_back(songTitles[pos]); // add to interested song titles
            interestedSongDuration.push_back(songDurations[pos]); // add to interested song durations
            interestedSongs.push_back(inputSongs[pos]); // add to interested songs
        }

        ++pos;
    }

    // Update playlist table based on search results
    tableComponent.updateContent();
}

void PlaylistComponent::addToDecklList(std::string filepath, int channel) {
    // Add selected song to the left or right player playlist
    if (channel == 0) {
        playListL.push_back(filepath); // add to left deck
    } else {
        playListR.push_back(filepath); // add to right deck
    }
}

void PlaylistComponent::getAudioLen(URL audioURL) {
    // Retrieve and store the duration of the audio file
    double songLength = 0.0;
    auto *reader = formatManager.createReaderFor(audioURL.createInputStream(false));

    if (reader != nullptr) {
        // Create a new reader source and transport source
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
        // Set the source of the transport source
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        // Prepare the transport source
        readerSource.reset(newSource.release());
        // Get the length of the audio file
        double songLength = transportSource.getLengthInSeconds();
        // Store the duration of the audio file
        songDurations.push_back(songLength);
    }

    interestedSongDuration = songDurations; // update the interested song durations
}
