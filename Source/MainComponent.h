#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

/**
 * @class MainComponent
 * @brief The main component of the application, hosting controls and content.
 *
 * This component lives inside the main window and contains controls and content
 * for the application. It includes audio players, playlist components, and GUI elements.
 */
class MainComponent : public AudioAppComponent {
public:
    /** Constructor. */
    MainComponent();

    /** Destructor. */
    ~MainComponent() override;

    /**
     * @brief Prepare to play audio.
     * @param samplesPerBlockExpected The number of samples per block expected.
     * @param sampleRate The sample rate of the audio.
     */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    /**
     * @brief Get the next audio block for playback.
     * @param bufferToFill The buffer to fill with audio data.
     */
    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;

    /** Release audio resources. */
    void releaseResources() override;

    /**
     * @brief Paint method for rendering the component.
     * @param g The Graphics object used for rendering.
     */
    void paint(Graphics &g) override;

    /** Resized method called when the component is resized. */
    void resized() override;

private:
    AudioFormatManager formatManager; /**< Audio format manager for handling audio file formats. */
    AudioThumbnailCache thumbCache{ 100 }; /**< Cache for up to 100 audio thumbnails. */

    int channelL = 0; /**< Left channel index. */
    int channelR = 1; /**< Right channel index. */

    PlaylistComponent playlistComponent{ formatManager }; /**< Playlist component. */
    DJAudioPlayer playerLeft{ formatManager }; /**< Left audio player. */
    DeckGUI deckGUILeft{ &playerLeft, &playlistComponent, formatManager, thumbCache, channelL }; /**< Left deck GUI. */

    DJAudioPlayer playerRight{ formatManager }; /**< Right audio player. */
    DeckGUI deckGUIRight{ &playerRight, &playlistComponent, formatManager, thumbCache, channelR }; /**< Right deck GUI. */

    // Labels of the GUI
    Label waveformLabel; /**< Label for the waveform display. */
    Label posLabel; /**< Label for position information. */
    Label widgetLabel; /**< Label for widget information. */
    Label playlistLabel; /**< Label for the playlist. */

    MixerAudioSource mixerSource; /**< Mixer audio source for combining left and right channels. */

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
