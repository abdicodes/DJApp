/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
           ) : player(_player), 
               waveformDisplay(formatManagerToUse, cacheToUse)
{

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    
    speedSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 1, 1);
    volSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 1, 1);
    volSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);

    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(1);
    
    speedSlider.setRange(0.0, 2.0);
    speedSlider.setValue(1.0);
    
    posSlider.setRange(0.0, 1.0);
    

    startTimer(500);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
//    addAndMakeVisible(loadButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);


}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("DeckGUI", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    
    double rotarySliderWidth = getWidth()/2;
    double rotarySliderHeight = getHeight()/2;
    double rowH = getHeight() / 8;
    double colW = getWidth() / 3 ;
    playButton.setBounds(0, 0, colW, rowH);
    stopButton.setBounds(colW, 0, colW, rowH);
    
    posSlider.setBounds(0, rowH * 1, getWidth(), rowH);
    speedSlider.setBounds(getWidth() -  rotarySliderWidth, rowH * 2, rotarySliderWidth, rotarySliderHeight);
    volSlider.setBounds(0, rowH * 2, rotarySliderWidth, rotarySliderHeight);
    
    waveformDisplay.setBounds(0, rowH * 6, getWidth(), rowH * 2);
//    loadButton.setBounds(0, rowH * 7, getWidth(), rowH);

}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
     if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();

    }
       if (button == &loadButton)
    {
        auto fileChooserFlags =
        FileBrowserComponent::canSelectFiles;
        
        auto fileChooserFlags2 = FileBrowserComponent::canSelectMultipleItems;
        
        
        fChooser.launchAsync(fileChooserFlags2 | fileChooserFlags , [this](const FileChooser& chooser)
        {
            player->loadURL(URL{chooser.getResult()});
            std::cout << "Stop button was clicked " << std::endl;
            
            // and now the waveformDisplay as well
            waveformDisplay.loadURL(URL{chooser.getResult()}); 
        });
    }
    // if (button == &loadButton)
    // {
    //     FileChooser chooser{"Select a file..."};
    //     if (chooser.browseForFileToOpen())
    //     {
    //         player->loadURL(URL{chooser.getResult()});
    //         waveformDisplay.loadURL(URL{chooser.getResult()});
            
    //     }


    // }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
      player->loadURL(URL{File{files[0]}});
      waveformDisplay.loadURL(URL{{File{files[0]}}});
  }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(
    player->getPositionRelative());
    
    if    (!isnan(player->getPositionRelative())) posSlider.setValue(player->getPositionRelative());
        
    
//    posSlider.setValue(player->getPositionRelative());
}



/** need to write */
void DeckGUI::playFromList(File  file)
{
    player->loadURL(URL{file});
    waveformDisplay.loadURL(URL{file});
}

void DeckGUI::setPos(double val)

{
    posSlider.setValue(val);
}
    

