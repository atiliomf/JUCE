
#pragma once


namespace bav::dsp
{
class BasicProcessorBase : public juce::AudioProcessor
{
public:
    BasicProcessorBase (juce::AudioProcessor::BusesProperties busesLayout = BusesProperties()
                                                                                .withInput (TRANS ("Input"), juce::AudioChannelSet::stereo(), true)
                                                                                .withOutput (TRANS ("Output"), juce::AudioChannelSet::stereo(), true));

    void repaintEditor();

private:
    void prepareToPlay (double samplerate, int blocksize) override;
    void releaseResources() override;

    void processBlock (juce::AudioBuffer< float >&, MidiBuffer&) override { }

    double getTailLengthSeconds() const override;

    void getStateInformation (juce::MemoryBlock&) override { }
    void setStateInformation (const void*, int) override { }

    int          getNumPrograms() override;
    int          getCurrentProgram() override;
    void         setCurrentProgram (int) override;
    const String getProgramName (int) override;
    void         changeProgramName (int, const String&) override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool supportsMPE() const override;
    bool isMidiEffect() const override;

    const String getName() const override;

    bool                        hasEditor() const override;
    juce::AudioProcessorEditor* createEditor() override;

    bool isBusesLayoutSupported (const BusesLayout& layout) const override;
};


}  // namespace bav::dsp
