/*
  ==============================================================================

    PlayListComponent.h
    Created: 28 Aug 2022 8:46:34pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include "DeckGUI.h"
#include <iostream>
#include <fstream>
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class PlayListComponent  :  public juce::Component,
                            public TableListBoxModel,
                            public Button::Listener,
                            public FileDragAndDropTarget
{
public:
    PlayListComponent(DJAudioPlayer*, DeckGUI* , DeckGUI*);
    ~PlayListComponent() override;

    // functions responsible for drawing of the UI
    void paint (juce::Graphics&) override;
    void resized() override;
    
    /** returns number of rows in a play list */
    int getNumRows () override;
    
    /** paints the background of a single row */
    void paintRowBackground (Graphics &, int rowNumber, int width, int height, bool rowIsSelected) override;
    
    /** to draw inside the row cell e.g. text  */
    void paintCell (Graphics &, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    Component *  refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;
    
    /** event listener that listens to user clicks within the playlist GUI */
    void buttonClicked(Button* ) override ;
    
    /** adding the functionality of draging and dropping files into the playlist*/
    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override;
    
    std::string getDuration(File);

    
private:
    AudioFormatManager formatManager;
    DJAudioPlayer tempPlayer{formatManager};
    TableListBox tableComponent;
    TextButton loadButton{"IMPORT PLAYLIST"};  // button that imports playlist from the local drive
    TextButton saveButton{"SAVE PLAYLIST"};  // save the current playlist to be retrieved upon the launch of the file
    FileChooser fChooser{"Select a file..."};
    Array<File> playlist;
    DeckGUI* deckGUI1; // pointer to deckGUI1 instance
    DeckGUI* deckGUI2; // pointer to deckGUI2 instance
    DJAudioPlayer* player;
    std::ofstream myfile; // to save playlist in text file
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayListComponent)
};
