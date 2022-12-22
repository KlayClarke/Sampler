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

class PositionLineComponent : juce::Component
{
public:
    PositionLineComponent();
    ~PositionLineComponent();
    
    void draw(juce::Graphics &g, const juce::Rectangle<int> &thumbnailBounds, juce::AudioTransportSource &transportSource, float audioLength);
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PositionLineComponent);
};
#endif /* PositionLineComponent_hpp */
