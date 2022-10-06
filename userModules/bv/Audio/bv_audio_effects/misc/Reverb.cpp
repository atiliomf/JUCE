
namespace bav::dsp::FX
{
Reverb::Reverb()
{
    params.roomSize   = 0.5f;
    params.damping    = 0.35f;
    params.wetLevel   = 1.0f;
    params.dryLevel   = 0.0f;
    params.width      = 1.0f;
    params.freezeMode = 0.2f;

    compressor.setAttack (15.0f);
    compressor.setRelease (35.0f);
}

void Reverb::prepare (int blocksize, double samplerate, int numChannels)
{
    jassert (numChannels <= 2);
    jassert (samplerate > 0 && blocksize > 0 && numChannels > 0);

    reverb.setSampleRate (samplerate);
    reverb.setParameters (params);

    compressor.prepare (samplerate, blocksize);

    sampleRate = samplerate;

    loCut.coefs.makeLowPass (
        samplerate, loCutFreq);
    loCut.prepare();

    hiCut.coefs.makeHighPass (
        samplerate, hiCutFreq);
    hiCut.prepare();

    sidechainBuffer.setSize (numChannels, blocksize, true, true, true);
    workingBuffer.setSize (numChannels, blocksize, true, true, true);
    conversionBuffer.setSize (numChannels, blocksize, true, true, true);

    dryGain.prepare (samplerate, blocksize);
    wetGain.prepare (samplerate, blocksize);
}

void Reverb::reset()
{
    reverb.reset();
    compressor.reset();
    workingBuffer.clear();
    sidechainBuffer.clear();
    conversionBuffer.clear();

    dryGain.reset();
    wetGain.reset();
}

void Reverb::setRoomSize (float newRoomSize)
{
    params.roomSize = newRoomSize;
    reverb.setParameters (params);
}

void Reverb::setDamping (float newDampingAmount)
{
    params.damping = newDampingAmount;
    reverb.setParameters (params);
}

void Reverb::setWidth (float newWidth)
{
    params.width = newWidth;
    reverb.setParameters (params);
}

void Reverb::setDryWet (int wetMixPercent)
{
    const auto wet = static_cast< float > (wetMixPercent) * 0.01f;
    wetGain.setGain (wet);
    dryGain.setGain (1.0f - wet);
}

void Reverb::setDuckAmount (int newDuckAmount)
{
    isDucking = newDuckAmount > 50;

    const auto duck = static_cast< float > (newDuckAmount) * 0.01f;
    compressor.setThreshold (juce::jmap (duck, 0.0f, -20.0f));
    compressor.setRatio (juce::jmap (duck, 1.0f, 10.0f));
}

void Reverb::setLoCutFrequency (float freq)
{
    loCutFreq = freq;
    loCut.coefs.makeLowPass (
        sampleRate, loCutFreq);
    loCut.reset();
}

void Reverb::setHiCutFrequency (float freq)
{
    hiCutFreq = freq;
    hiCut.coefs.makeHighPass (
        sampleRate, hiCutFreq);
    hiCut.reset();
}


void Reverb::process (juce::AudioBuffer< double >& input, double* reverbLevel)
{
    process (input, input, reverbLevel);
}

void Reverb::process (juce::AudioBuffer< float >& input, float* reverbLevel)
{
    process (input, input, reverbLevel);
}


void Reverb::process (juce::AudioBuffer< double >& input,
                      juce::AudioBuffer< double >& compressorSidechain,
                      double*                      reverbLevel)
{
    conversionBuffer.makeCopyOf (input, true);
    sidechainBuffer.makeCopyOf (compressorSidechain, true);

    float level;

    process (conversionBuffer, sidechainBuffer, &level);

    input.makeCopyOf (conversionBuffer, true);

    if (reverbLevel != nullptr) *reverbLevel = static_cast< double > (level);
}


void Reverb::process (juce::AudioBuffer< float >& input,
                      juce::AudioBuffer< float >& compressorSidechain,
                      float*                      reverbLevel)
{
    const auto numSamples  = input.getNumSamples();
    const auto numChannels = std::min (2, input.getNumChannels());

    jassert (numSamples == compressorSidechain.getNumSamples());
    jassert (numChannels == compressorSidechain.getNumChannels());

    dsp::buffers::copy (compressorSidechain, sidechainBuffer);
    dsp::buffers::copy (input, workingBuffer);

    // reverb
    switch (numChannels)
    {
        case (0) : return;

        case (1) :
            reverb.processMono (workingBuffer.getWritePointer (0), numSamples);

        default :
            reverb.processStereo (workingBuffer.getWritePointer (0),
                                  workingBuffer.getWritePointer (1),
                                  numSamples);
    }

    if (reverbLevel != nullptr)
        *reverbLevel = workingBuffer.getMagnitude (0, numSamples);

    // filters
    loCut.process (workingBuffer);
    hiCut.process (workingBuffer);

    // sidechain compressor
    if (isDucking) compressor.process (sidechainBuffer, workingBuffer);

    input.clear();

    dryGain.process (sidechainBuffer);
    wetGain.process (workingBuffer);

    for (int chan = 0; chan < numChannels; ++chan)
    {
        // add & write result to workingBuffer
        vecops::addV (workingBuffer.getWritePointer (chan),
                      sidechainBuffer.getReadPointer (chan),
                      numSamples);

        dsp::buffers::copy (workingBuffer, input);
    }
}

}  // namespace bav::dsp::FX
