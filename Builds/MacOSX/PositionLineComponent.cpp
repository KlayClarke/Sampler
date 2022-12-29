//
//  PositionLineComponent.cpp
//  Sampler - App
//
//  Created by Klay Anthony Clarke on 12/21/22.
//

#include "PositionLineComponent.hpp"

PositionLineComponent::PositionLineComponent(juce::AudioTransportSource &transportSource) : transportSource(transportSource)
{
    PositionLineComponent::startTimer(40);
}

PositionLineComponent::~PositionLineComponent()
{
    
}

void PositionLineComponent::timerCallback()
{
    repaint();
}

void PositionLineComponent::paint(juce::Graphics &g)
{
    auto duration = (float) transportSource.getLengthInSeconds();
    
    if (duration > 0.0)
    {
        auto audioPosition = (float) transportSource.getCurrentPosition();
        auto drawPosition = (audioPosition / duration) * (float) getWidth();
        
        g.setColour(juce::Colours::green);
        g.drawLine(drawPosition, 0.0f, drawPosition, (float) getHeight(), 2.0f);
    }
}

void PositionLineComponent::mouseDown(const juce::MouseEvent &event)
{
    auto duration = (float) transportSource.getLengthInSeconds();
    
    if (duration > 0.0)
    {
        auto clickPos = event.position.x;
        auto audioPos = (clickPos / (float) getWidth()) * duration;
        transportSource.setPosition((double) audioPos);
    }
}
