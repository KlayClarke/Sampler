#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : state (Stopped), thumbnailCache(5), thumbnail(512, formatManager, thumbnailCache)
{
    // currentTime label
    addAndMakeVisible(&currentTimeLabel);
    currentTimeLabel.setText("00:00", juce::dontSendNotification);
    currentTimeLabel.setJustificationType(juce::Justification::centred);
    
    // open button
    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open...");
    openButton.onClick=[this]{openButtonClicked();};
    
    // play button
    addAndMakeVisible(&playButton);
    playButton.setButtonText("Play");
    playButton.onClick = [this]{playButtonClicked();};
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    playButton.setEnabled(false);
    
    // stop button
    addAndMakeVisible(&stopButton);
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this]{stopButtonClicked();};
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    stopButton.setEnabled(false);
    
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (300, 200);

    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);
    thumbnail.addChangeListener(this); // AudioThumbnail is a type of ChangeBroadcaster class -> we can register as a listener for changes (accounts for updates of our waveform)
    
    MainComponent::startTimer(20);
    
    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
//![prepareToPlay]
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    // For more details, see the help for AudioProcessor::prepareToPlay()
}
//![prepareToPlay]

//![getNextAudioBlock]
void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!
    if (MainComponent::readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    
    transportSource.getNextAudioBlock(bufferToFill);
    
    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    //bufferToFill.clearActiveBufferRegion();
}
//![getNextAudioBlock]

//![releaseResources]
void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    MainComponent::transportSource.releaseResources();

    // For more details, see the help for AudioProcessor::releaseResources()
}
//![releaseResources]

//![openButtonClicked]
void MainComponent::openButtonClicked()
{
    MainComponent::chooser = std::make_unique<juce::FileChooser>("Select a .wav file to play...",
                                                                 juce::File{},
                                                                 "*.wav;*.mp3;*.AIFF");
    auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
    
    MainComponent::chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc){
        auto file = fc.getResult();
        
        if (file != juce::File{})
        {
            auto* reader = formatManager.createReaderFor(file);
            
            if (reader != nullptr)
            {
                auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
                
                transportSource.setSource(newSource.get(),0,nullptr,reader->sampleRate);
                
                playButton.setEnabled(true);
                
                thumbnail.setSource(new juce::FileInputSource(file));
                
                readerSource.reset(newSource.release());
            }
        }
    });
}
//![openButtonClicked]

//![playButtonClicked]
void MainComponent::playButtonClicked()
{
    if ((state==Stopped)||(state==Paused))
    {
        changeState(Starting);
    } else if(state==Playing)
    {
        changeState(Pausing);
    }
}
//![playButtonClicked]

//![stopButtonClicked]
void MainComponent::stopButtonClicked()
{
    if (state ==Paused)
    {
        changeState(Stopped);
    }
    else
    {
        changeState(Stopping);
    }
}
//![stopButtonClicked]

//![changeState]
void MainComponent::changeState(TransportState newState)
{
   if (state != newState)
   {
       state = newState;
       switch (state) {
           case Stopped:
               playButton.setButtonText("Play");
               stopButton.setButtonText("Stop");
               stopButton.setEnabled(false);
               transportSource.setPosition(0.0);
               break;
           case Starting:
               transportSource.start();
               break;
           case Playing:
               playButton.setButtonText("Pause");
               stopButton.setButtonText("Stop");
               stopButton.setEnabled(true);
               break;
           case Pausing:
               transportSource.stop();
               break;
           case Paused:
               playButton.setButtonText("Resume");
               stopButton.setButtonText("Return to Zero");
               break;
           case Stopping:
               transportSource.stop();
               break;
       }
   }
}
//![changeState]

//![thumbnailChanged]
void MainComponent::thumbnailChanged()
{
    repaint(); // to repaint waveform if audio file changes ???
}
//![thumbnailChanged]

//==============================================================================
//![paintIfNoFileLoaded]
void MainComponent::paintIfNoFileLoaded(juce::Graphics &g, const juce::Rectangle<int> &thumbnailBounds)
{
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(thumbnailBounds);
    g.setColour(juce::Colours::white);
    g.drawFittedText("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
}
//![paintIfNoFileLoaded]

//![paintIfFileLoaded]
void MainComponent::paintIfFileLoaded(juce::Graphics &g, const juce::Rectangle<int> &thumbnailBounds)
{
    g.setColour(juce::Colours::white);
    g.fillRect(thumbnailBounds);
    g.setColour(juce::Colours::red);
    thumbnail.drawChannels(g, thumbnailBounds, 0.0, thumbnail.getTotalLength(), 1.0f);
}
//![paintIfFileLoaded]

//![paint]
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    // You can add your drawing code here!
    g.setColour(juce::Colours::white);
    
    // waveform thumbnail styling code below
    juce::Rectangle<int> thumbnailBounds (10, 130, getWidth()-20, getHeight()-120);
    if (thumbnail.getNumChannels()==0)
        // add paint for no file loaded (standard "No file loaded" text)
        paintIfNoFileLoaded(g, thumbnailBounds);
    else
        // add paint for file loaded (waveform of currently loaded audio file)
        paintIfFileLoaded(g, thumbnailBounds);
}
//![paint]

//![resized]
void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    currentTimeLabel.setBounds(10, 10, getWidth()-20, 20);
    openButton.setBounds(10, 40, getWidth()-20, 20);
    playButton.setBounds(10, 70, getWidth()-20, 20);
    stopButton.setBounds(10, 100, getWidth()-20, 20);
}
//![resized]

//![changeListenerCallback]
void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
   if (source==&transportSource)
   {
       if (transportSource.isPlaying())
       {
           changeState(Playing);
       } else if ((state == Stopping) || (state == Playing))
       {
           changeState(Stopped);
       } else if (Pausing == state)
       {
           changeState(Paused);
       }
   }
    if (source==&thumbnail) thumbnailChanged();
}
//![changeListenerCallback]

//![timerCallback]
void MainComponent::timerCallback()
{
    juce::RelativeTime currentPosition (transportSource.getCurrentPosition());
    juce::RelativeTime totalTime (transportSource.getLengthInSeconds());
    
    auto currentMinutes = ((int) currentPosition.inMinutes()) % 60;
    auto currentSeconds = ((int) currentPosition.inSeconds()) % 60;
    
    auto totalMinutes = ((int) totalTime.inMinutes()) % 60;
    auto totalSeconds = ((int) totalTime.inSeconds()) % 60;
    
    auto positionString = juce::String::formatted("%02d:%02d / %02d:%02d", currentMinutes, currentSeconds, totalMinutes, totalSeconds);
    
    currentTimeLabel.setText(positionString, juce::dontSendNotification);
}
//![timerCallback]
