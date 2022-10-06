
#pragma once

namespace bav::dsp::FX
{
/*
        Class that wraps juce::Reverb into a usable interface, and includes some other functionality as well: hi- and lo-pass filters, and a sidechain-able compressor.
    */

class Reverb
{
public:
    Reverb();
    virtual ~Reverb() = default;

    void prepare (int blocksize, double samplerate, int numChannels);
    void reset();

    void setRoomSize (float newRoomSize);
    void setDamping (float newDampingAmount);
    void setWidth (float newWidth);
    void setDryWet (int wetMixPercent);
    void setDuckAmount (int newDuckAmount);
    void setLoCutFrequency (float freq);
    void setHiCutFrequency (float freq);

    //  process input with no external compressor sidechain
    void process (juce::AudioBuffer< double >& input, double* reverbLevel = nullptr);
    void process (juce::AudioBuffer< float >& input, float* reverbLevel = nullptr);

    //  process input with an external signal as the compressor's sidechain
    void process (juce::AudioBuffer< double >& input,
                  juce::AudioBuffer< double >& compressorSidechain,
                  double*                      reverbLevel = nullptr);

    void process (juce::AudioBuffer< float >& input,
                  juce::AudioBuffer< float >& compressorSidechain,
                  float*                      reverbLevel = nullptr);

private:
    juce::Reverb reverb;

    juce::Reverb::Parameters params;

    juce::AudioBuffer< float > conversionBuffer;
    juce::AudioBuffer< float > workingBuffer;
    juce::AudioBuffer< float > sidechainBuffer;

    bool                isDucking;
    Compressor< float > compressor;

    dsp::filters::MultiFilter< float > loCut, hiCut;
    float                              loCutFreq = 80.0f, hiCutFreq = 5500.0f;

    double sampleRate = 0.0;

    SmoothedGain< float, 2 > dryGain, wetGain;
};

}  // namespace bav::dsp::FX
