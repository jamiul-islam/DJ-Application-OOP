/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 9 Jan 2024 2:41:38pm
    Author:  JAMIUL ISLAM

 * Progress:
 *
 * 1. Prepare the transportSource and resamplingSource - DONE
 * 2. Get the next audio block from the resamplingSource - DONE
 * 3. Release the resources of the transportSource and resamplingSource - DONE
 * 4. Load the audio file from the provided URL - DONE
 * 5. Set the gain for the audio player - DONE
 * 6. Set the speed/resampling ratio for the audio player - DONE
 * 7. Set the position of the playhead in seconds - DONE
 * 8. Set the relative position of the playhead - DONE
 * 9. Start playing the audio - DONE
 * 10. Stop/pause the audio - DONE
 * 11. Get the relative position of the playhead - DONE
 *

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager &_formatManager) : formatManager(_formatManager) {}

DJAudioPlayer::~DJAudioPlayer() {}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    // prepare the transportSource and resamplingSource
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resamplingSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) {
    // get the next audio block from the resamplingSource
    resamplingSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources() {
    // release the resources of the transportSource and resamplingSource
    transportSource.releaseResources();
    resamplingSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL) {
    // create a reader for the audioURL that was passed in the parameter
    auto *reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        // create a new readerSource that will be used to load the reader
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
        // set the song source of the transportSource to the newSource
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        // set the song source of the resamplingSource to the transportSource
        readerSource.reset(newSource.release());
    }
}

void DJAudioPlayer::setGain(double gain) {
    if (gain < 0 || gain > 1.0) {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    } else {
        transportSource.setGain(gain);
    }
}

void DJAudioPlayer::setSpeed(double ratio) {
    if (ratio < 0 || ratio > 100.0) {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
    } else {
        resamplingSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setPosition(double posInSecs) {
    // set the position of the transportSource
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos) {
    if (pos < 0 || pos > 1.0) {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    } else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::start() {
    transportSource.start(); // start the song
}

void DJAudioPlayer::stop() {
    transportSource.stop(); // pause the song
}

double DJAudioPlayer::getPositionRelative() {
    // return the relative position of the playHead so that it can be used in the slider
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}