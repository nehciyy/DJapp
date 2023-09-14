/*
  ==============================================================================

    DeckGUI.h
    Created: 5 Feb 2023 12:20:33pm
    Author:  yc

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

class DeckGUI : public juce::Component ,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::FileDragAndDropTarget,
    public juce::Timer
    

{
public:
    DeckGUI(DJAudioPlayer* player,
        juce::AudioFormatManager &formatManagerToUse, 
        juce::AudioThumbnailCache &cacheToUse); 

    ~DeckGUI();

    void paint(juce::Graphics&) override;
    void resized() override;

    /**implemet Button::Listener*/
    void buttonClicked(juce::Button*) override;
    void sliderValueChanged(juce::Slider* slider)override;
    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    void timerCallback() override;
    void loadFile(juce::URL audioURL);

private:
    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD" };
    juce::TextButton loopButton{ "LOOP" };

    juce::Slider volSlider;
    juce::Label volLabel;

    juce::Slider speedSlider;
    juce::Label speedLabel;
    
    juce::Label posLabel;
    juce::Slider posSlider;

    bool isLooping = false;

    juce::FileChooser fChooser{ "Select a file..." };

    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI);
};
