//
//  WaveformComponent.cpp
//  Sampler - App
//
//  Created by Klay Anthony Clarke on 12/21/22.
//

#include "WaveformComponent.hpp"
WaveformComponent::WaveformComponent() : thumbnail(512, formatManager, thumbnailCache), thumbnailCache(5)
{
    formatManager.registerBasicFormats();
    thumbnail.addChangeListener(this);
}

WaveformComponent::~WaveformComponent()
{
    
}

void WaveformComponent::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    
}

void WaveformComponent::paintIfNoFileAdded(juce::Graphics &g, const juce::Rectangle<int> &thumbnailBounds)
{
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(thumbnailBounds);
    g.setColour(juce::Colours::white);
    g.drawFittedText("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
}

void WaveformComponent::paintIfFileAdded(juce::Graphics &g, const juce::Rectangle<int> &thumbnailBounds, juce::AudioTransportSource &transportSource)
{
    // waveform style drawing
    g.setColour(juce::Colours::white);
    g.fillRect(thumbnailBounds);
    g.setColour(juce::Colours::red);
    
    float audioLength = (float) thumbnail.getTotalLength();
    thumbnail.drawChannels(g, thumbnailBounds, 0.0, thumbnail.getTotalLength(), 1.0f);
    
    // playback position line style drawing
    positionLine.draw(g, thumbnailBounds, transportSource, audioLength);
}
