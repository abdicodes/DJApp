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
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 1, 1);
    

    startTimer(500);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
//    addAndMakeVisible(loadButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    
    addAndMakeVisible(elapsedTimeButton);
    addAndMakeVisible(remainingTimeButton);


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
    double margin = getWidth()/ 6;
    playButton.setBounds(0, 0, colW, rowH);
    stopButton.setBounds(colW, 0, colW, rowH);
    
    posSlider.setBounds(0 + margin, rowH * 1 , getWidth() - margin * 2, rowH);
    speedSlider.setBounds(getWidth() -  rotarySliderWidth, rowH * 2, rotarySliderWidth, rotarySliderHeight);
    volSlider.setBounds(0, rowH * 2, rotarySliderWidth, rotarySliderHeight);
    
    waveformDisplay.setBounds(0, rowH * 6, getWidth(), rowH * 2);
//    loadButton.setBounds(0, rowH * 7, getWidth(), rowH);
    
    elapsedTimeButton.setBounds(5, rowH, margin , rowH);
    remainingTimeButton.setBounds(getWidth() - margin - 5, rowH, margin, rowH);

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
    waveformDisplay.setPositionRelative(player->getPositionRelative());
   
    if (!isnan(player->getPositionRelative())) posSlider.setValue(player->getPositionRelative());
    
    elapsedTimeButton.setButtonText(getElapsedTime());
    remainingTimeButton.setButtonText(getRemainingTime());
                                    
    
}



/** need to write */
void DeckGUI::playFromList(File  file)
{
    player->loadURL(URL{file});
    waveformDisplay.loadURL(URL{file});
}

std::string DeckGUI::getElapsedTime()

{
    int current = 0;
    if (!isnan(player->getTotalLength()))
        {
            current += std::floor(player->getCurrentPosition());
        }
    
    // minutes and seconds from start of stream
    int sec = current % 60;
    int min = current / 60;
    std::string minutes =std::to_string(min) ;
    std::string seconds =std::to_string(sec) ;
    
    if ( min < 10) minutes = "0"+minutes;
    if ( sec < 10) seconds = "0"+seconds;
    
    return minutes + " : " + seconds;
}

std::string DeckGUI::getRemainingTime()

{
    int remainingSeconds = 0;
    if (!isnan(player->getTotalLength()))
        {
            remainingSeconds = std::floor(player->getTotalLength() -  player->getCurrentPosition());
        }
    
    
    // minutes and seconds from start of stream
    int sec = remainingSeconds % 60;
    int min = remainingSeconds / 60;
    
    std::string minutes =std::to_string(min) ;
    std::string seconds =std::to_string(sec) ;
    
    if ( min < 10) minutes = "0"+minutes;
    if ( sec < 10) seconds = "0"+seconds;
    
    
    
    return minutes + " : " + seconds;
}
    

