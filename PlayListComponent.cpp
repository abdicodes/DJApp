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
PlayListComponent::PlayListComponent(DJAudioPlayer* _player, DeckGUI* _deckGUI1,DeckGUI* _deckGUI2 ): deckGUI1(_deckGUI1),deckGUI2(_deckGUI2), player(_player)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    tableComponent.getHeader().addColumn("Track Title", 1, int (150));
    tableComponent.getHeader().addColumn("File Path", 2, int (150));
    tableComponent.getHeader().addColumn("Duration", 3, int (100));
    tableComponent.getHeader().addColumn(" ", 4, int (110));
    tableComponent.getHeader().addColumn(" ", 5, int (110));
    tableComponent.getHeader().addColumn(" ", 6, int (100));
    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(saveButton);
    addAndMakeVisible(searchBar);
   
    loadButton.setName("loadButton");
    saveButton.setName("saveButton");
    loadButton.addListener(this);
    saveButton.addListener(this);
    searchBar.addListener(this);
    searchBar.setTextToShowWhenEmpty("Search tracks...", juce::Colours::grey);
    searchBar.onReturnKey = [this] { search(searchBar.getText()); };
    
    auto filePath = File::getSpecialLocation (File::SpecialLocationType::userHomeDirectory).getChildFile ("playlist.txt");
    std::string line;
    
    
    std::ifstream playListFile(filePath.File::getFullPathName().toRawUTF8());
    
    //defensive coding just in case there is issue opening file stream
    if ( playListFile.is_open())
    {
        while (getline(playListFile,line))
        {
            playlist.add(File(line));
        }
        playListFile.close();
    }
    
    else std::cout << " ERROR: playlist file can't be opened!" << std::endl;
    
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
    g.setFont (12.0f);
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
    if (columnId == 1)  g.drawText(playlist[rowNumber].getFileName(), 4, 0, width - 4, height, Justification::centredLeft, true);
//    g.drawText(playlist[rowNumber].getFullPathName(), 4, 0, width - 4, height, Justification::centredLeft, true);
    
   
    if ( columnId == 2) g.drawText(playlist[rowNumber].getFullPathName(), 4, 0, width - 4, height, Justification::centredLeft, true);
    
    if ( columnId == 3)
    {
        g.drawText(getDuration(playlist[rowNumber]), 4, 0, width - 4, height, Justification::centredLeft, true);
    }
   
    
    
}

Component *  PlayListComponent::refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate)
{
    if (rowNumber == playlist.size()) return nullptr;
    
    if (columnId == 4)
    {
        
        if (existingComponentToUpdate == nullptr){
            TextButton* btn = new TextButton{"play in Deck1"};
            String id{std::to_string(rowNumber + 1000)};
            btn->setComponentID(id);
            btn-> addListener(this);
           
            existingComponentToUpdate = btn;
        }
    }
    
    if (columnId == 5)
    {
        if (existingComponentToUpdate == nullptr){
            TextButton* btn = new TextButton{"play in Deck2"};
            String id{std::to_string(rowNumber+ 2000)};
            btn->setComponentID(id);
            btn-> addListener(this);
           
            existingComponentToUpdate = btn;
        }
    }
    
    if (columnId == 6)
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
        
        if (myfile.is_open()) {
            for (const File& file:playlist )
            {
                std::cout << file.getFullPathName() << std::endl;
                myfile <<file.getFullPathName() << std::endl;
            }
            
            myfile.close();
        }
        
        else std::cout << "ERROR playlist file can't be written on! check your access rules!";
  
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
    double buttonWidth = getWidth() / 8;
    double componentHight = getHeight() / 8;
    loadButton.setBounds(0, 0, buttonWidth, componentHight);
    saveButton.setBounds(getWidth() - 100, 0, 100, 25);
    searchBar.setBounds(buttonWidth + 2, 0, buttonWidth * 6 - 4, componentHight);
    
    tableComponent.setBounds(0, componentHight, getWidth(), getHeight() - componentHight);
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


std::string PlayListComponent::getDuration(File file)

{
    if (file.exists())
    {
        
        player->loadURL(URL(file));
        
        std::string output = player->getDuration();
        std::cout << output << std::endl;
        return output;
    }
    else return "";
}
void PlayListComponent::search(String val){
    if (val != "") {
        for (int i = 0;i < playlist.size();i++) {
            if (playlist[i].getFileName().contains(val)) {
                tableComponent.selectRow(i);
            }
        }
    }
    else {
        tableComponent.deselectAllRows();
    }
}
