//
//  WaveformComponent.hpp
//  Sampler - App
//
//  Created by Klay Anthony Clarke on 12/21/22.
//

#ifndef WaveformComponent_hpp
#define WaveformComponent_hpp

#include <stdio.h>
#include "JuceHeader.h"
#include <PositionLineComponent.hpp>

class WaveformComponent : juce::Component, juce::ChangeListener
{
public:
    WaveformComponent();
    ~WaveformComponent();
    
    void changeListenerCallback(juce::ChangeBroadcaster *source) override;
    void paintIfNoFileAdded(juce::Graphics &g, const juce::Rectangle<int> &thumbnailBounds);
    void paintIfFileAdded(juce::Graphics &g, const juce::Rectangle<int> &thumbnailBounds, juce::AudioTransportSource &transportSource);
    
    juce::AudioThumbnail thumbnail;
private:
    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioFormatManager formatManager;
    
    PositionLineComponent positionLine;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformComponent);
};

#endif /* WaveformComponent_hpp */
