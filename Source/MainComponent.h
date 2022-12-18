#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
                       public juce::ChangeListener,
                       private juce::Timer
{
public:
    //![TransportState]
    enum TransportState
    {
        Starting,
        Playing,
        Stopping,
        Stopped,
        Pausing,
        Paused
    };
    //![TransportState]
    //==============================================================================
    MainComponent();
    ~MainComponent() override;
    
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void changeState(TransportState newState);
    void thumbnailChanged();
    void openButtonClicked();
    void playButtonClicked();
    void stopButtonClicked();
    //==============================================================================
    void paintIfNoFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);
    void paintIfFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);
    void paint (juce::Graphics& g) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void timerCallback() override;
private:
    //==============================================================================
    //![Labels]
    juce::Label currentTimeLabel;
    //![Labels]
    
    //![Buttons]
    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    //![Buttons]
    
    //![Variables]
    std::unique_ptr<juce::FileChooser> chooser;

    juce::AudioFormatManager formatManager; // contains a list of audio formats (wav, aiff, mp3) and can create suitable objects for reading audio data from these formats
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource; // allows us to read audio in a consistent format
    juce::AudioTransportSource transportSource; // can control playback of an audioformatreadersource object

    TransportState state; // the current state of audio playback (starting, playing, stopping, stopped)
    
    juce::AudioThumbnailCache thumbnailCache; // to cache low res version of audio file
    juce::AudioThumbnail thumbnail;
    //![Variables]
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

