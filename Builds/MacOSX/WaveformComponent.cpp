//
//  WaveformComponent.cpp
//  Sampler - App
//
//  Created by Klay Anthony Clarke on 12/21/22.
//

#include "WaveformComponent.hpp"
WaveformComponent::WaveformComponent(int sourceSamplesPerThumbnailSample, juce::AudioFormatManager &formatManager, juce::AudioThumbnailCache &thumbnailCache) : thumbnail(sourceSamplesPerThumbnailSample, formatManager, thumbnailCache)
{
    thumbnail.addChangeListener(this);
}

WaveformComponent::~WaveformComponent()
{
    
}

void WaveformComponent::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    if (source==&thumbnail) thumbnailChanged();
}

void WaveformComponent::setFile(const juce::File &file)
{
    thumbnail.setSource(new juce::FileInputSource(file));
}

void WaveformComponent::thumbnailChanged()
{
    repaint();
}

void WaveformComponent::paint(juce::Graphics &g)
{
    if (thumbnail.getNumChannels() == 0)
        paintIfNoFileAdded(g);
    else
        paintIfFileAdded(g);
}

void WaveformComponent::paintIfNoFileAdded(juce::Graphics &g)
{
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(getLocalBounds());
    g.setColour(juce::Colours::white);
    g.drawFittedText("No File Loaded", getLocalBounds(), juce::Justification::centred, 1);
}

void WaveformComponent::paintIfFileAdded(juce::Graphics &g)
{
    g.setColour(juce::Colours::white);
    g.fillRect(getLocalBounds());
    g.setColour(juce::Colours::red);
    thumbnail.drawChannels(g, getLocalBounds(), 0.0, thumbnail.getTotalLength(), 1.0f);
}
