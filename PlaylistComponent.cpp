/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 5 Feb 2023 5:59:01pm
    Author:  yc

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1,
                                     DeckGUI* _deckGUI2,
                                     DJAudioPlayer* _playerForParsingData) : deckGUI1(_deckGUI1),
                                                                             deckGUI2(_deckGUI2),
                                                                             playerForParsingData(_playerForParsingData)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    tableComponent.getHeader().addColumn("Track title", 1, 400);
    tableComponent.getHeader().addColumn("Duration", 2, 100);
    tableComponent.getHeader().addColumn("Delete Song", 3, 100);
    tableComponent.getHeader().addColumn("Load to Left Deck", 4, 100);
    tableComponent.getHeader().addColumn("Load to Right Deck", 5, 100);
    tableComponent.setModel(this);

    //display tableComponent
    addAndMakeVisible(tableComponent);

    //display import track button and button listener
    addAndMakeVisible(importTrack);
    importTrack.addListener(this);

    //display search song search bar and text editor listener
    addAndMakeVisible(findTrack);
    findTrack.addListener(this);
    findTrack.setTextToShowWhenEmpty("Find your song! (Press ENTER to search)", juce::Colours::black);
    findTrack.setJustification(juce::Justification::centred);
    findTrack.onReturnKey = [this] {searchTrack(findTrack.getText()); };

    //display left and right decks
    addAndMakeVisible(deckGUI1Button);
    deckGUI1Button.addListener(this);

    addAndMakeVisible(deckGUI2Button);
    deckGUI2Button.addListener(this);

    //call the load track function
    loadTracks();
    

};

PlaylistComponent::~PlaylistComponent()
{
    saveTracks(); //allow tracks to be saved even after GUI is closed and reopened
};
void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.setColour (juce::Colours::grey);
    g.drawRect(getLocalBounds(), 3); //draw an outline around the component

    g.setFont (14.0f);
    //setting colours of GUI for playlist component, left and right deck, the button to import track and the search bar to find track
    tableComponent.setColour(juce::TableListBox::backgroundColourId, juce::Colour::fromRGB(64, 53, 89)); // dark purple 

    deckGUI1Button.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(255, 210, 165)); //orange button
    deckGUI1Button.setColour(juce::TextButton::textColourOffId, juce::Colours::black); //black font

    deckGUI2Button.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(255, 210, 165)); //orange button
    deckGUI2Button.setColour(juce::TextButton::textColourOffId, juce::Colours::black); //black font

    importTrack.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(255, 210, 165));//orange button
    importTrack.setColour(juce::TextButton::textColourOffId, juce::Colours::black); //black font

    findTrack.setColour(juce::TextEditor::backgroundColourId, juce::Colour::fromRGB(255, 210, 165));//orange button
    findTrack.applyColourToAllText(juce::Colours::black); //black font
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    //setting the look and where the position should be displayed for respective components
    importTrack.setBounds(0, 0, getWidth(), getHeight()/10);
    findTrack.setBounds(0, getHeight()/10, getWidth(), getHeight() / 10);
    tableComponent.setBounds(0, getHeight() / 5, getWidth(), (getHeight() / 10)*8);
    deckGUI1Button.setBounds((getWidth() / 8) * 6, getHeight() / 5 + 25, getWidth() / 8, (getHeight() / 10) * 8 - 31) ;
    deckGUI2Button.setBounds((getWidth() / 8) * 7, getHeight() / 5 + 25, getWidth() / 8, (getHeight() / 10) * 8 - 31);

}
int PlaylistComponent::getNumRows()
{
    return trackTitle.size(); // get total number of tracks in the vector
};

//changing the colour of the rows when the playlist which has song imported is selected with the cursor
void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colour::fromRGB(255, 255, 193)); //yellow highlight when selected
    }
    else
    {
        g.fillAll(juce::Colour::fromRGB(138, 121, 175)); //purple background
    }
};

//coloumId == 1 will display song title while coloumId == 2 will show the song duration
void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int coloumId, int width, int height, bool rowIsSelected)
{
   if (coloumId == 1) 
   {
        g.drawText(trackTitle[rowNumber].songTitle, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
   }
   if (coloumId == 2)
   {
        g.drawText(trackTitle[rowNumber].songDuration, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
   }
    
    
};

//when coloumId == 3 delete button is displayed
juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* exsitingComponentToUpdate)
{
    if (columnId == 3)
    {
        if (exsitingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton{ "x" };
            btn->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(255, 255, 193));
            btn->setColour(juce::TextButton::textColourOffId, juce::Colours::black);
            juce::String id{ std::to_string(rowNumber) };

            btn -> setComponentID(id);
            btn -> addListener(this);

            exsitingComponentToUpdate = btn;
        }
    }
    return exsitingComponentToUpdate;
};
//functions that will be called when respective buttons are clicked
void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button == &importTrack) // import tracks from file explorer
    {
        importToPlayer();
        tableComponent.updateContent();
    }
    else if (button == &deckGUI1Button) //when load into left deck is clicked, song will be loaded to left deck.
    {
        loadInDeck(deckGUI1);
    }
    else if (button == &deckGUI2Button)//when load into right deck is clicked, song will be loaded to right deck.
    {
        loadInDeck(deckGUI2);
    }
    else 
    {
        //when delete button is clicked, song will be deleted from tableComponent
        int id = std::stoi(button->getComponentID().toStdString());
        deleteTrack(id);
        tableComponent.updateContent();
    }
};

//when row is selected in the tableComponent and a load to deck button is clicked, the track will be retrieved and loaded from trackTitle
void PlaylistComponent::loadInDeck(DeckGUI* deckGUI)
{
    int rowSelected = {tableComponent.getSelectedRow() };

    if (rowSelected != -1)
    {
        deckGUI->loadFile(trackTitle[rowSelected].URL);
    }
};

//load the songs that were not deleted before closing the GUI, when the GUI is restarted
void PlaylistComponent::loadPlaylist(DeckGUI* deckGUI)
{
    int selectedRow{ tableComponent.getSelectedRow() };
    if (selectedRow != -1)
    {
        deckGUI->loadFile(trackTitle[selectedRow].URL);
    }
};
//when the import track button is clicked, multiple tracks will be allowed to load into the tableComponent 
void PlaylistComponent::importToPlayer()
{
    juce::FileChooser fchooser{ "Please select your files" };
    if (fchooser.browseForMultipleFilesToOpen())
    {
        for (const juce::File& file : fchooser.getResults())
        {
            juce::String fileNameWOExtension{ file.getFileNameWithoutExtension() };
            if (!isInTrack(fileNameWOExtension))
            {
                Tracks newTrack{file};
                juce::URL audioURL{file};
                newTrack.songDuration = getDuration(audioURL);
                trackTitle.push_back(newTrack);
            }
        }
    }
};
bool PlaylistComponent::isInTrack(juce::String fileNameWOExtension)
{
    return (std::find(trackTitle.begin(), trackTitle.end(), fileNameWOExtension) != trackTitle.end());
};
//processing the duration of each tracks
juce::String PlaylistComponent::getDuration(juce::URL audioURL)
{
    playerForParsingData->loadURL(audioURL);
    double seconds{ playerForParsingData->getLengthInSeconds() };
    juce::String minutes{ secondsToMinutes(seconds) };
    return minutes;
};
//allow multiple file dragging and dropping into tableComponent
bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    DBG("PlaylistComponent::isInterestedInFileDrag");
    return true;
};

void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y)
{
    DBG("DeckGUI::fileDropped");
    
        for (const juce::File& file : files)
        {
            juce::String fileName = { file.getFileNameWithoutExtension() };
            if (!isInTrack(fileName))
            {
                Tracks newTrack{ file };
                juce::URL audioURL{ file };
                newTrack.songDuration = getDuration(audioURL);
                trackTitle.push_back(newTrack);
            }
        }
    
    tableComponent.updateContent();
};
//converting the seconds to minutes
juce::String PlaylistComponent::secondsToMinutes(double seconds)
{
    int secondsInt{ int(std::round(seconds)) };
    juce::String sec{ std::to_string(secondsInt % 60) };
    juce::String min{ std::to_string(secondsInt / 60) };
    
    if (sec.length() < 2) {
        sec = sec.paddedLeft('0', 2);
    }
    return juce::String{ min + ":" + sec }; 
};

// when the track is found in trackTitle, the row will be selected in tableComponent
void PlaylistComponent::searchTrack(juce::String searchSong)
{
    if (searchSong != " ") 
    {
        int rowNo = songPos(searchSong);
        tableComponent.selectRow(rowNo);
    }
    else
    {
        tableComponent.deselectAllRows();
    }
};
//searching for the song in the trackTitle
int PlaylistComponent::songPos(juce::String searchSong)
{
    auto pos = find_if(trackTitle.begin(), trackTitle.end(), 
               [&searchSong](const Tracks& foundSong) {return foundSong.songTitle.contains(searchSong); 
               });
    int i = -1;

    if(pos != trackTitle.end())
    {
        i = std::distance(trackTitle.begin(), pos);
    }

    return i;
};
//deleting track
void PlaylistComponent::deleteTrack(int id)
{
    trackTitle.erase(trackTitle.begin() + id);
    
};
//save track so that tracks will remain the same after restarting
void PlaylistComponent::saveTracks()
{
    std::ofstream allTracks("allTracks.csv");

    for (Tracks& e : trackTitle) {
        allTracks << e.file.getFullPathName() << "," << e.songDuration << "\n";
    }
};
//loading previous tracks again after restarting GUI
void PlaylistComponent::loadTracks()
{
    std::ifstream allTracks("allTracks.csv");
    std::string filePath;
    std::string duration;

    if(allTracks.is_open())
    {
        while (std::getline(allTracks, filePath, ','))
        {
            juce::File file{ filePath };
            Tracks newTrackSongs{ file };

            std::getline(allTracks, duration);
            newTrackSongs.songDuration = duration;
            trackTitle.push_back(newTrackSongs);

        }
    }
    allTracks.close();
};

