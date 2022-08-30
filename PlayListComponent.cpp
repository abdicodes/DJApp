/*
  ==============================================================================

    PlayListComponent.cpp
    Created: 28 Aug 2022 8:46:34pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlayListComponent.h"

//==============================================================================
PlayListComponent::PlayListComponent(DeckGUI* _deckGUI1,DeckGUI* _deckGUI2 ): deckGUI1(_deckGUI1),deckGUI2(_deckGUI2)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    tableComponent.getHeader().addColumn("Audio Title", 1, int (400));
    tableComponent.getHeader().addColumn(" Deck 1", 2, int (150));
    tableComponent.getHeader().addColumn(" Deck2", 3, int (150));
    tableComponent.getHeader().addColumn(" Delete", 4, int (150));



    addAndMakeVisible(tableComponent);
    addAndMakeVisible(loadButton);
    
    tableComponent.setModel(this);
    
    trackTitles.push_back("hello1");
    trackTitles.push_back("hello2");
    loadButton.setName("loadButton");
    loadButton.addListener(this);
    
    
}

PlayListComponent::~PlayListComponent()
{
}

void PlayListComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlayListComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

int PlayListComponent::getNumRows ()
{
    return int (playlist.size());
}
void PlayListComponent::paintRowBackground (Graphics & g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(Colours::orange);
    }
    else g.fillAll(Colours::darkgoldenrod);
}
void PlayListComponent::paintCell (Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    std::cout << " PlayListComponent::paintCell row: " << rowNumber << std::endl;
   
//    g.drawText(trackTitles[rowNumber], 4, 0, width - 4, height, Justification::centredLeft, true);
    g.drawText(playlist[rowNumber].getFileName(), 4, 0, width - 4, height, Justification::centredLeft, true);
}

Component *  PlayListComponent::refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate)
{
    if (rowNumber == playlist.size()) return nullptr;
    if (columnId == 2)
    {
        
        if (existingComponentToUpdate == nullptr){
            TextButton* btn = new TextButton{"play in Deck1"};
            String id{std::to_string(rowNumber + 1000)};
            btn->setComponentID(id);
            btn-> addListener(this);
           
            existingComponentToUpdate = btn;
        }
    }
    
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr){
            TextButton* btn = new TextButton{"play in Deck2"};
            String id{std::to_string(rowNumber+ 2000)};
            btn->setComponentID(id);
            btn-> addListener(this);
           
            existingComponentToUpdate = btn;
        }
    }
    
    if (columnId == 4)
    {
        if (existingComponentToUpdate == nullptr){
            TextButton* btn = new TextButton{"Remove"};
            String id{std::to_string(rowNumber + 3000)};
            btn->setComponentID(id);
            btn-> addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}


void PlayListComponent::buttonClicked (Button * button)
{
    if (button->getName() == "loadButton")
    {
         auto fileChooserFlags1 =
        FileBrowserComponent::canSelectFiles;
        
        auto fileChooserFlags2 = FileBrowserComponent::canSelectMultipleItems;
        
        fChooser.launchAsync(fileChooserFlags1 | fileChooserFlags2 , [this](const FileChooser& chooser)
        {
            
            for (File file : chooser.getResults())
            {
                playlist.add(file);
            }
            tableComponent.updateContent();
        });
    }
    
   else if (button->getName() == "play in Deck1")
    {
        std::cout << "play1 button is clicked" << std::endl;
        int songID = std::stoi(button->getComponentID().toStdString()) - 1000;
         std::cout << songID<< std::endl;
        deckGUI1->playFromList(playlist[songID]);
        
    }
    
   else if (button->getName() == "play in Deck2")
    {
        std::cout << "play2 button is clicked" << std::endl;
        int songID =  std::stoi(button->getComponentID().toStdString()) - 2000;
         std::cout << songID<< std::endl;
        deckGUI2->playFromList(playlist[songID]);
    }
    
   else   if (button->getName() == "Remove")
    {
        tableComponent.deselectAllRows();
        std::cout << "remove button is clicked" << std::endl;
        int songID =  std::stoi(button->getComponentID().toStdString()) - 3000;
        playlist.remove(songID);
        tableComponent.updateContent();
    }
   else NULL;
}

void PlayListComponent::resized()
{
    
    loadButton.setBounds(0, 0, 50, 50);
    
    tableComponent.setBounds(0, 50, getWidth(), getHeight());
    // This method is where you should set the bounds of any child
    // components that your component contains..
}


bool PlayListComponent::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true;
}

void PlayListComponent::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
    for (const String & file :  files)
    {
        
        //convert to File then add to playList
        playlist.add(File{file});
    }
    tableComponent.updateContent();
}

