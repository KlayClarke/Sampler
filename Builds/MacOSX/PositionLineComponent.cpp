//
//  PositionLineComponent.cpp
//  Sampler - App
//
//  Created by Klay Anthony Clarke on 12/21/22.
//

#include "PositionLineComponent.hpp"

PositionLineComponent::PositionLineComponent()
{
    
}

PositionLineComponent::~PositionLineComponent()
{
    
}

// draw position line on waveform
void PositionLineComponent::draw(juce::Graphics &g, const juce::Rectangle<int> &thumbnailBounds, juce::AudioTransportSource &transportSource, float audioLength)
{
    g.setColour(juce::Colours::black);
    
    auto audioPosition = (float) transportSource.getCurrentPosition();
    auto drawPosition = (audioPosition / audioLength) * (float) thumbnailBounds.getWidth() + (float) thumbnailBounds.getX();
    
    g.drawLine(drawPosition, (float) thumbnailBounds.getY(), drawPosition, (float) thumbnailBounds.getBottom(), 2.0f);
}
