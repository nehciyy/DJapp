/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 5 Feb 2023 3:24:42pm
    Author:  yc

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"


WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                 juce::AudioThumbnailCache& cacheToUse):audioThumb(1000,formatManagerToUse,cacheToUse),
                                                                        fileLoaded(false),
                                                                        position(0)
{
    audioThumb.addChangeListener(this);
};
WaveformDisplay::~WaveformDisplay()
{

};

void WaveformDisplay::paint(juce::Graphics& g)
{
   // g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colour::fromRGB(138, 121, 175)); //purple
    //g.setColour(juce::Colour::fromRGB(255, 210, 165));
    g.setColour(juce::Colour::fromRGB(211, 140, 173)); //pink
    g.drawRect(getLocalBounds(), 3); //draw an outline

    g.setColour(juce::Colour::fromRGB(255, 255, 193));//yellow

    if (fileLoaded)
    {
        audioThumb.drawChannel(g,getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);
        g.setColour(juce::Colour::fromRGB(255, 210, 165)); //orange box
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight(),3.0);
        g.setFont(20.0f);
        g.setColour(juce::Colours::white);
        g.drawText(fileName, getLocalBounds(),juce::Justification::bottomRight,true);
    }
    else 
    {
        g.setFont(20.0f);
        g.drawText("File not loaded...", getLocalBounds(), juce::Justification::centred, true);
    }
   

};
void WaveformDisplay::resized()
{

};

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
    if (fileLoaded)
    {
        std::cout << "wfd: loadURL!" << std::endl;
        fileName = audioURL.getFileName();
        repaint();
    }
    else
    {
        std::cout << "wfd: not loadURL!" << std::endl;
    }
};

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    std::cout << "wfd: change received!" << std::endl;
    repaint();
};
void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint();
    }
   
};