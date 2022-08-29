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
PlayListComponent::PlayListComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    tableComponent.getHeader().addColumn("Hello there", 1, int (200));
    addAndMakeVisible(tableComponent);
    tableComponent.setModel(this);
    trackTitles.push_back("hello1");
    trackTitles.push_back("hello2");
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
    return int (trackTitles.size());
}
void PlayListComponent::paintRowBackground (Graphics & g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(Colours::orange);
    }
    else g.fillAll(Colours::darkgrey);
}
void PlayListComponent::paintCell (Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    g.drawText(trackTitles[rowNumber], 4, 0, width - 4, height, Justification::centredLeft, true);
}


void PlayListComponent::resized()
{
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
