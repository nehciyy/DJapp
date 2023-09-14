/*
  ==============================================================================

    Tracks.cpp
    Created: 24 Feb 2023 12:49:00am
    Author:  yc

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Tracks.h"
#include <filesystem>

//==============================================================================
Tracks::Tracks(juce::File _file) : file(_file),
                                   songTitle(_file.getFileNameWithoutExtension()),
                                   URL(juce::URL {_file})
{

};

bool Tracks::operator == (const juce::String& otherSongs)const
{
    return songTitle == otherSongs;
};
