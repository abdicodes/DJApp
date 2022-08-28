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

//==============================================================================
/*
*/
class PlayListComponent  : public juce::Component,
                            public TableListBoxModel
{
public:
    PlayListComponent();
    ~PlayListComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    int getNumRows () override;
    void paintRowBackground (Graphics &, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell (Graphics &, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

private:
    TableListBox tableComponent;
    std::vector<std::string> trackTitles;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayListComponent)
};
