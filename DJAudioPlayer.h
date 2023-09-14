/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 5 Feb 2023 11:33:37am
    Author:  yc

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer :public juce::AudioSource {
public:

    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(juce::URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);

    void start();
    void stop();

    /**get the relative position of the playhead*/
    void setLooping(const bool shouldLoop);
    double getPositionRelative();
    double getLengthInSeconds();

private:

    juce::AudioFormatManager &formatManager;

    //will have better memory management using unique pointer & can instantiate pointer on fly
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource,false,2 };
};
