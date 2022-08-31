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
    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(saveButton);
   
    loadButton.setName("loadButton");
    saveButton.setName("saveButton");
    loadButton.addListener(this);
    saveButton.addListener(this);
    
    auto filePath = File::getSpecialLocation (File::SpecialLocationType::userHomeDirectory).getChildFile ("playlist.txt");
    std::string line;
    
    
    std::ifstream MyReadFile(filePath.File::getFullPathName().toRawUTF8());
    while (getline(MyReadFile,line))
    {
        playlist.add(File(line));
    }
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
        int songID = std::stoi(button->getComponentID().toStdString()) - 1000;
        deckGUI1->playFromList(playlist[songID]);
        
    }
    
   else if (button->getName() == "play in Deck2")
    {
        int songID =  std::stoi(button->getComponentID().toStdString()) - 2000;
        deckGUI2->playFromList(playlist[songID]);
    }
    
    else if ( button ->getName() == "saveButton")
    {
        auto filePath = File::getSpecialLocation (File::SpecialLocationType::userHomeDirectory).getChildFile ("playlist.txt");
        myfile.open(filePath.File::getFullPathName().toRawUTF8());
        
        for (const File& file:playlist )
        {
            std::cout << file.getFullPathName() << std::endl;
            myfile <<file.getFullPathName() << std::endl;
        }
      
        myfile.close();
    }
    
   else   if (button->getName() == "Remove")
    {
        tableComponent.deselectAllRows();
        int songID =  std::stoi(button->getComponentID().toStdString()) - 3000;
        playlist.remove(songID);
        tableComponent.updateContent();
    }
   else NULL;
}

void PlayListComponent::resized()
{
    
    loadButton.setBounds(0, 0, 200, 50);
    saveButton.setBounds(getWidth() - 200, 0, 200, 50);
    
    tableComponent.setBounds(0, 50, getWidth(), getHeight());
    // This method is where you should set the bounds of any child
    // components that your component contains..
}


bool PlayListComponent::isInterestedInFileDrag (const StringArray &files)
{
  return true;
}

void PlayListComponent::filesDropped (const StringArray &files, int x, int y)
{
    for (const String & file :  files)
    {
        
        //convert to File then add to playList
        playlist.add(File{file});
    }
    tableComponent.updateContent();
}

