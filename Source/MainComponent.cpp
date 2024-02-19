#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() {
    // Make sure you set the size of the component after
    // you add any child components.
    setSize(1000, 600);

    // Some platforms require permissions to open input channels so request that
    // here
    if (RuntimePermissions::isRequired(RuntimePermissions::recordAudio) &&
        !RuntimePermissions::isGranted(RuntimePermissions::recordAudio)) {
        RuntimePermissions::request(RuntimePermissions::recordAudio,
                                    [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    } else {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(2, 2);
    }

    // Register file formats enabled by JUCE
    formatManager.registerBasicFormats();

    // Add application components and make them visible
    addAndMakeVisible(deckGUILeft);
    addAndMakeVisible(deckGUIRight);
    addAndMakeVisible(playlistComponent);

    // Add Labels and customize visuals for labels
    addAndMakeVisible(waveformLabel);
    waveformLabel.setText("Waveforms", juce::dontSendNotification);
    waveformLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
    waveformLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(posLabel);
    posLabel.setText("Playback", juce::dontSendNotification);
    posLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
    posLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(widgetLabel);
    widgetLabel.setText("Widgets Controls", juce::dontSendNotification);
    widgetLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
    widgetLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(playlistLabel);
    playlistLabel.setText("Drag Files here to add to Library", juce::dontSendNotification);
    playlistLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
    playlistLabel.setJustificationType(juce::Justification::centred);
}

MainComponent::~MainComponent() {
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    playerLeft.prepareToPlay(samplesPerBlockExpected, sampleRate);
    playerRight.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&playerLeft, false);
    mixerSource.addInputSource(&playerRight, false);
}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) {
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources() {
    playerLeft.releaseResources();
    playerRight.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint(Graphics &g) {
    g.fillAll (juce::Colours::lightsteelblue);
}

void MainComponent::resized() {
    deckGUILeft.setBounds(0, 0, getWidth() / 2, getHeight() / 2);
    deckGUIRight.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() / 2);
    playlistComponent.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);
}