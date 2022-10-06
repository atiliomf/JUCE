#pragma once

namespace bav::dsp::FX
{
template < typename SampleType >
class Manager
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    virtual ~Manager() = default;

    void prepare (double samplerate, int blocksize);

    void process (const AudioBuffer& input);

    const AudioBuffer& getProcessedSignal();

protected:
    void add (AudioEffect< SampleType >& effect);

    template < typename... Args >
    void add (AudioEffect< SampleType >& first, Args... rest)
    {
        add (first);
        add (std::forward< Args > (rest)...);
    }

private:
    AudioBuffer storage;
    AudioBuffer storageAlias;

    juce::Array< AudioEffect< SampleType >* > effects;

    int lastBlocksize {0};
};

}  // namespace bav::dsp::FX
