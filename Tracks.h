/*
  ==============================================================================

    Tracks.h
    Created: 24 Feb 2023 12:49:00am
    Author:  yc

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Tracks
{
public:
    Tracks(juce::File _file);
    
    juce::File file;
    juce::URL URL;
    juce::String songTitle;
    juce::String songDuration;

    bool operator == (const juce::String& otherSongs)const;


};
