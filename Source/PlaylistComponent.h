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

/**
 * @class PlaylistComponent
 * @brief A component for managing and displaying a playlist of audio files.
 *
 * This class provides features for managing a playlist of audio files, allowing
 * users to add songs, display song details, and interact with the playlist.
 */
class PlaylistComponent :
        public juce::Component,
        public TableListBoxModel,
        public Button::Listener,
        public FileDragAndDropTarget,
        public AudioSource,
        public TextEditor::Listener {
public:
    /**
     * @brief Constructor.
     * @param formatManager The audio format manager to use.
     */
    PlaylistComponent(AudioFormatManager& formatManager);

    /** Destructor. */
    ~PlaylistComponent() override;

    /**
     * @brief Paint method for rendering the component.
     * @param g The Graphics object used for rendering.
     */
    void paint(juce::Graphics &) override;

    /**
     * @brief Resized method called when the component is resized.
     */
    void resized() override;

    // Functions for playlist
    /**
     * @brief Get the number of rows in the playlist.
     * @return The number of rows in the playlist.
     */
    int getNumRows() override;

    /**
     * @brief Paint the background of each row in the playlist.
     * @param g The Graphics object used for rendering.
     * @param rowNumber The index of the row.
     * @param width The width of the row.
     * @param height The height of the row.
     * @param rowIsSelected Flag indicating if the row is selected.
     */
    void paintRowBackground(Graphics &, int rowNumber, int width, int height, bool rowIsSelected) override;

    /**
     * @brief Paint the cells with song titles and durations in the playlist.
     * @param g The Graphics object used for rendering.
     * @param rowNumber The index of the row.
     * @param columnId The index of the column.
     * @param width The width of the cell.
     * @param height The height of the cell.
     * @param rowIsSelected Flag indicating if the row is selected.
     */
    void paintCell(Graphics &, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    /**
     * @brief Refresh components for the "Add to Left" and "Add to Right" buttons in the playlist.
     * @param rowNumber The index of the row.
     * @param columnId The index of the column.
     * @param isRowSelected Flag indicating if the row is selected.
     * @param existingComponentToUpdate The existing component to update.
     * @return The updated component.
     */
    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

    // Audio source
    /**
     * @brief Prepare the audio source to play.
     * @param samplesPerBlockExpected The number of samples per block expected.
     * @param sampleRate The sample rate of the audio source.
     */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    /**
     * @brief Get the next audio block from the audio source.
     * @param bufferToFill The buffer to fill with audio data.
     */
    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;

    /** Release audio resources. */
    void releaseResources() override;

    // Button::button listener
    /**
     * @brief Handle button clicks for adding songs to the left or right player.
     * @param button The button that was clicked.
     */
    void buttonClicked(Button* button) override;

    // Drag and drop feature
    /**
     * @brief Check if the application is interested in file drag and drop.
     * @param files The array of file paths being dragged.
     * @return True if interested; otherwise, false.
     */
    bool isInterestedInFileDrag(const StringArray& files) override;

    /**
     * @brief Handle files dropped into the playlist.
     * @param files The array of file paths dropped.
     * @param x The x-coordinate of the drop location.
     * @param y The y-coordinate of the drop location.
     */
    void filesDropped(const StringArray& files, int x, int y) override;

    /**
     * @brief Handle changes in the search bar text.
     * @param textEditor The TextEditor triggering the change.
     */
    void textEditorTextChanged(TextEditor&) override;

    /** Vector storing songs in the left player playlist. */
    std::vector<std::string> playListL;

    /** Vector storing songs in the right player playlist. */
    std::vector<std::string> playListR;

private:
    AudioFormatManager& formatManager; /**< Audio format manager to handle audio file formats. */
    std::unique_ptr<AudioFormatReaderSource> readerSource; /**< Reader source for audio format reading. */
    AudioTransportSource transportSource; /**< Audio transport source for playback. */

    // Playlist displayed as a table list
    TableListBox tableComponent; /**< Table component for displaying the playlist. */

    // For storing music files
    std::vector<std::string> inputSongs; /**< Vector storing all the songs. */
    std::vector<std::string> interestedSongs; /**< Vector storing songs that are interested in dropping. */
    std::vector<std::string> songTitles; /**< Vector storing song titles. */
    std::vector<std::string> interestedSongTitles; /**< Vector storing song titles that are interested in dropping. */
    std::vector<int> songDurations; /**< Vector storing song durations. */
    std::vector<int> interestedSongDuration; /**< Vector storing song durations that are interested in dropping. */

    // Search bar and search label
    TextEditor searchBar; /**< TextEditor for searching songs. */
    Label searchLabel; /**< Label for search bar. */

    /**
     * @brief Add selected song to the left or right player playlist.
     * @param filepath The file path of the selected song.
     * @param channel The channel (left or right) to add the song to.
     */
    void addToDecklList(std::string filepath, int channel);

    /**
     * @brief Get the duration of the audio file.
     * @param audioURL The URL of the audio file.
     */
    void getAudioLen(URL audioURL);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
