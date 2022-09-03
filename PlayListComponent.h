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

//==============================================================================
/*
*/
class PlayListComponent  :  public juce::Component,
                            public TableListBoxModel,
                            public Button::Listener,
                            public FileDragAndDropTarget
{
public:
    PlayListComponent(DeckGUI* , DeckGUI*);
    ~PlayListComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /** returns number of rows in a play list */
    int getNumRows () override;
    
    /** paints the background of a single row */
    void paintRowBackground (Graphics &, int rowNumber, int width, int height, bool rowIsSelected) override;
    
    /** to draw inside the row cell e.g. text  */
    void paintCell (Graphics &, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    Component *  refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;
    
    /** need to write */
    void buttonClicked(Button* ) override ;
    
    /** need to write */
    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override;
    
private:
    TableListBox tableComponent;
    TextButton loadButton{"IMPORT PLAYLIST"};
    TextButton saveButton{"SAVE PLAYLIST"};
    FileChooser fChooser{"Select a file..."};
    Array<File> playlist;
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    std::ofstream myfile;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayListComponent)
};
