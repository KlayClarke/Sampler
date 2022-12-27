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

class WaveformComponent : public juce::Component, public juce::ChangeListener
{
public:
    WaveformComponent(int sourceSamplesPerThumbnailSample, juce::AudioFormatManager &formatManager, juce::AudioThumbnailCache &thumbnailCache);
    ~WaveformComponent();
    
    void changeListenerCallback(juce::ChangeBroadcaster *source) override;
    void setFile(const juce::File &file);
    void thumbnailChanged();
    void paint(juce::Graphics &g) override;
    void paintIfNoFileAdded(juce::Graphics &g);
    void paintIfFileAdded(juce::Graphics &g);
    
private:
    juce::AudioThumbnail thumbnail;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformComponent);
};

#endif /* WaveformComponent_hpp */
