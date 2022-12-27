//
//  PositionLineComponent.hpp
//  Sampler - App
//
//  Created by Klay Anthony Clarke on 12/21/22.
//

#ifndef PositionLineComponent_hpp
#define PositionLineComponent_hpp

#include <stdio.h>
#include "JuceHeader.h"

class PositionLineComponent : public juce::Component, private juce::Timer
{
public:
    PositionLineComponent(juce::AudioTransportSource &transportSource);
    ~PositionLineComponent();
    
    void timerCallback() override;
    void paint(juce::Graphics &g) override;
private:
    juce::AudioTransportSource &transportSource;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PositionLineComponent);
};
#endif /* PositionLineComponent_hpp */
