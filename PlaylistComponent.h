/*
  ==============================================================================

    PlaylistComponent.h
    Created: 5 Feb 2023 5:59:01pm
    Author:  yc

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include <fstream>
#include "Tracks.h"
#include "DeckGUI.h"
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener,
                           public juce::FileDragAndDropTarget
{
public:
    PlaylistComponent(DeckGUI* _deckGUI1, DeckGUI* _deckGUI2, DJAudioPlayer* _playerForParsingData);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;

    void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;

    void paintCell(juce::Graphics&, int rowNumber, int coloumId, int width, int height, bool rowIsSelected) override;

    juce::Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* exsitingComponenetToUpdate) override;

    void buttonClicked(juce::Button* button) override;
    
    void loadInDeck(DeckGUI* deckGUI);
    void loadPlaylist(DeckGUI* deckGUI);
    void importToPlayer();
    bool isInTrack(juce::String fileNameWOExtension);

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;


    void searchTrack(juce::String searchSong);
    int songPos(juce::String searchSong);
    void deleteTrack(int id);
    void saveTracks();
    void loadTracks();

private:
    juce::TableListBox tableComponent;
    
    juce::TextButton deckGUI1Button{ "Load track to left deck" };
    juce::TextButton deckGUI2Button{ "Load track to right deck" };
    juce::TextButton importTrack{ "IMPORT TRACK LIBRARY" };
    juce::TextEditor findTrack;

    std::vector<Tracks>trackTitle;

    juce::AudioFormatManager formatManager;
    DJAudioPlayer djaudioPlayer{ formatManager };
    DJAudioPlayer* playerForParsingData;

    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;

    double durationOfSong;

    juce::String getDuration(juce::URL audioURL);
    juce::String secondsToMinutes(double seconds);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
