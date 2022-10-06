
namespace bav::dsp::psola
{
template < typename SampleType >
SynthesisGrain< SampleType >::SynthesisGrain (const Storage& storageToUse)
    : storage (storageToUse)
{
}

template < typename SampleType >
bool SynthesisGrain< SampleType >::isActive() const
{
    return active;
}

template < typename SampleType >
void SynthesisGrain< SampleType >::startNewGrain (int start, int length)
{
    active      = true;
    startIndex  = start;
    grainLength = length;
    currentTick = 0;
}

/*-------------------------------------------------------------*/
/* Hann window function */

template < typename FloatType >
static inline FloatType ncos (int order, int i, int size) noexcept
{
    return std::cos (static_cast< FloatType > (order * i)
                     * juce::MathConstants< FloatType >::pi / static_cast< FloatType > (size - 1));
}
template float  ncos (int, int, int) noexcept;
template double ncos (int, int, int) noexcept;

template < typename SampleType >
static inline SampleType getWindowValue (int index, int windowLength) noexcept
{
    return static_cast< SampleType > (0.5 - 0.5 * ncos< SampleType > (2, index, windowLength));
}
template float  getWindowValue (int, int) noexcept;
template double getWindowValue (int, int) noexcept;

/*-------------------------------------------------------------*/

template < typename SampleType >
SampleType SynthesisGrain< SampleType >::getNextSample()
{
    jassert (grainLength > 0);

    const auto sample = storage.getSample (startIndex, currentTick) * getWindowValue< SampleType > (currentTick, grainLength);

    currentTick++;

    if (currentTick > grainLength)
        active = false;

    return sample;
}

template class SynthesisGrain< float >;
template class SynthesisGrain< double >;

}  // namespace bav::dsp::psola
