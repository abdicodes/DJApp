/*
  ==============================================================================

    PlayListComponent.h
    Created: 28 Aug 2022 8:46:34pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PlayListComponent  : public juce::Component
{
public:
    PlayListComponent();
    ~PlayListComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayListComponent)
};
