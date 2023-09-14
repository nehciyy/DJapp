/*
  ==============================================================================

    DeckGUI.cpp
    Created: 5 Feb 2023 12:20:33pm
    Author:  yc

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 juce::AudioFormatManager& formatManagerToUse,
                 juce::AudioThumbnailCache& cacheToUse): player(_player),
                                                         waveformDisplay(formatManagerToUse, cacheToUse)
{
    //add play,stop,load and loop button
    addAndMakeVisible(playButton); 
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(loopButton);

    //add volume slider and the label for volume slider
    addAndMakeVisible(volSlider);
    //make the volume slider such that it is a semi-circle shape slider where dragging the slider horizontally will change the slider value
    volSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    volSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    volLabel.setText("Volume", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(volLabel);

    //add speed slider and label for speed slider
    addAndMakeVisible(speedSlider);
    //make the speed slider such that it is a semi-circle shape slider where dragging the slider horizontally will change the slider value
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    speedLabel.setText("Speed", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(speedLabel);

    //add position slider and label for position slider
    addAndMakeVisible(posSlider);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    posLabel.setText("Position", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(posLabel);

    //displaying the waveform of the song
    addAndMakeVisible(waveformDisplay);

    //button listeners when 4 buttons are clicked
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    loopButton.addListener(this);

    //slider listeners when slider values change
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    //setting the boundaries for slider values
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 10.0);
    posSlider.setRange(0.0, 1.0);

    startTimer(500);

};
DeckGUI::~DeckGUI()
{
    stopTimer();
};

void DeckGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(138, 121, 175));//purple background
    g.setColour(juce::Colour::fromRGB(211, 140, 173));//pink outline
    g.drawRect(getLocalBounds(), 3); //draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    
    //setting button colours
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(255, 255, 193)); //yellow button
    playButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);//black text
    
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(255, 255, 193)); //yellow button
    stopButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);//black text

    loadButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(255, 255, 193)); //yellow button
    loadButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);//black text

    //setting slider colours
    volSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour::fromRGB(255, 210, 165)); //orange fill slider
    volSlider.setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(255, 210, 165)); //orange thumb colour
    volLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(255, 255, 193)); //yellow text

    speedSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour::fromRGB(255, 210, 165)); //orange fill slider
    speedSlider.setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(255, 210, 165)); //orange thumb colour
    speedLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(255, 255, 193)); //yellow text

    posSlider.setColour(juce::Slider::trackColourId, juce::Colour::fromRGB(255, 210, 165)); //orange fill slider
    posSlider.setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(255, 210, 165)); //orange thumb colour
    posLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(255, 255, 193)); //yellow text
};
void DeckGUI::resized()
{
    double rowH = getHeight() / 10;
    //waveform displaying at the top of the GUI
    waveformDisplay.setBounds(0, 0, getWidth(), rowH*3);

   //sliders are displayed below the waveform with the position slider at the top of volume and speed slider
    posSlider.setBounds(60, rowH * 5, getWidth() - 60, rowH);
    posLabel.setBounds(0, rowH * 5, 60, rowH);

    //volume and speed sliders are displayed side by side
    volSlider.setBounds(getWidth() / 2, rowH * 6, getWidth() / 2, rowH * 3);
    volLabel.setBounds(getWidth() / 2 + 70, rowH * 6 + 50, getWidth() / 2, rowH*3);

    speedSlider.setBounds(0, rowH * 6, getWidth()/2, rowH*3);
    speedLabel.setBounds(70, rowH * 6 + 50, getWidth() / 2, rowH * 3);

    //button displays from left to right order: play,stop,loop,load
    playButton.setBounds(0, rowH*3, getWidth()/4, rowH*2);
    stopButton.setBounds(getWidth() / 4, rowH * 3, getWidth() / 4, rowH * 2);
    loopButton.setBounds(getWidth()/2, rowH * 3, getWidth()/4, rowH*2);
    loadButton.setBounds((getWidth() / 4) * 3, rowH * 3, getWidth() / 4, rowH * 2);
};
void DeckGUI::buttonClicked(juce::Button* button)
{
      if (button == &playButton) //when play button is clicked, song will start
      {
          std::cout << "Play Button was clicked" << std::endl;
          player -> start();
      }
      if (button == &stopButton)//when stop button is clicked, song will stop
      {
          std::cout << "Stop button was clicked" << std::endl;
          player -> stop();
      }
      if (button == &loopButton) //when loop button is clicked, song will loop
      {
          if (!isLooping) // if isLooping is true colour of button is changed, else it will be of same colour as other buttons
          {
              isLooping = true;
              loopButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(255, 210, 165));//orange button
              loopButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black); //black fonts
          }
          else
          {
              isLooping = false;
              loopButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(255, 255, 193));//yellow button
          }

          player->setLooping(isLooping);
          
      }
      if(button == &loadButton) // when load button is clicked, user is able to choose a song and load the song into the DJApp
      {
          auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
              fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser &chooser)
              {
                  juce::File chosenFile = chooser.getResult();
                  player -> loadURL(juce::URL{ chosenFile });
                  waveformDisplay.loadURL(juce::URL{ chosenFile });
              });
      }
      
}

// running the respective functions when the slider changes value in DJAudioPlayer.cpp
void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)
     {
         player -> setGain(slider -> getValue());
     }
     if (slider == &speedSlider)
     {
         player -> setSpeed(slider -> getValue());
     }
     if (slider == &posSlider)
     {
         player -> setPositionRelative(slider -> getValue());
     }
     
}
//allow dragging and dropping of song tracks into the waveform
bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
};
void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    std::cout << "DeckGUI::fileDropped" << std::endl;
    if (files.size() == 1) {
        player -> loadURL(juce::URL{ juce::File{ files[0] } });
        waveformDisplay.loadURL(juce::URL{ juce::File{ files[0] } });
    }
};
//when value of position slider is changed, waveform can find the position the slider value is at
void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());
};
//load file
void DeckGUI::loadFile(juce::URL audioURL)
{
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
};