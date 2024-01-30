/*
  ==============================================================================

    PlaylistComponent.h
    Created: 12 Jan 2024 3:31:32pm
    Author:  JAMIUL ISLAM

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>

using namespace juce;

class PlaylistComponent :
        public juce::Component,
        public TableListBoxModel,
        public Button::Listener,
        public FileDragAndDropTarget,
        public AudioSource,
        public TextEditor::Listener {
public:
    PlaylistComponent(AudioFormatManager& formatManager);
    ~PlaylistComponent() override;

    void paint(juce::Graphics &) override;
    void resized() override;

    // functions for playlist
    int getNumRows() override;
    void paintRowBackground(Graphics &, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics &, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;

    /** audio source */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
    void releaseResources() override;

    /** Button::button listener */
    void buttonClicked(Button* button) override;

    /** drag and drop feature */
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;
    void textEditorTextChanged(TextEditor&) override;

    /** stores songs in the left player */
    std::vector<std::string> playListL;
    /** stores songs in the right player */
    std::vector<std::string> playListR;

private:

    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;

    // playlist displayed as a table list
    TableListBox tableComponent;

    // for storing music files
    std::vector<std::string> inputSongs; // stores all the songs
    std::vector<std::string> interestedSongs; // stores songs that are interested in dropping
    std::vector<std::string> songTitles; // stores song titles
    std::vector<std::string> interestedSongTitles; // stores song titles that are interested in dropping
    std::vector<int> songDurations; // stores song durations
    std::vector<int> interestedSongDuration; // stores song durations that are interested in dropping

    /** search bar and search label */
    TextEditor searchBar;
    Label searchLabel;

    /** adds songs to the playlist */
    void addToDecklList(std::string filepath, int channel);
    /** gets the duration of the audio file */
    void getAudioLen(URL audioURL);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
