/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }  

    addAndMakeVisible(deckGUI1); 
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playListComponent);
    formatManager.registerBasicFormats();
    
    getLookAndFeel().setColour(TextEditor::textColourId,Colours::black);
    getLookAndFeel().setColour(TextEditor::backgroundColourId,Colours::pink);
    getLookAndFeel().setColour(TextButton::buttonColourId,Colour::fromRGB(255, 210, 210));
    getLookAndFeel().setColour(ListBox::backgroundColourId, Colours::pink);
    getLookAndFeel().setColour(TextButton::textColourOffId, Colours::black);
    getLookAndFeel().setColour(ResizableWindow::backgroundColourId, Colours::pink);
    getLookAndFeel().setColour(Slider::backgroundColourId, Colours::hotpink);
    getLookAndFeel().setColour(Slider::thumbColourId, Colours::palevioletred);
    getLookAndFeel().setColour(Slider::trackColourId, Colours::mediumvioletred);
    getLookAndFeel().setColour(Slider::rotarySliderFillColourId, Colours::mediumvioletred);
    getLookAndFeel().setColour(Slider::rotarySliderOutlineColourId, Colours::hotpink);
    
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    mixerSource.addInputSource(&player3, false);

 }
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    player3.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    double deckHight = getHeight() - getHeight()/2.5;
    double playlistHight = getHeight()  - deckHight;
    deckGUI1.setBounds(0, 0, getWidth()/2, deckHight);
    deckGUI2.setBounds(getWidth()/2, 0, getWidth()/2, deckHight);
    playListComponent.setBounds(0, deckHight, getWidth(), playlistHight);
    

}

