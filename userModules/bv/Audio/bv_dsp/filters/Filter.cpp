
#include <juce_dsp/juce_dsp.h>

namespace bav::dsp::filters
{
template < typename SampleType >
void Filter< SampleType >::reset (SampleType resetToValue)
{
    order = coefs.getFilterOrder();
    state.ensureStorageAllocated (order);
    state.fill (resetToValue);
}

template < typename SampleType >
void Filter< SampleType >::prepare() noexcept
{
    reset();
}

template < typename SampleType >
void Filter< SampleType >::snapToZero() noexcept
{
    for (auto& e : state)
        juce::dsp::util::snapToZero (e);
}

template < typename SampleType >
void Filter< SampleType >::process (SampleType* buffer, int numSamples)
{
    if (order != coefs.getFilterOrder())
        reset();

    auto src    = buffer;
    auto dst    = buffer;
    auto coeffs = coefs.getRawCoefficients();

    switch (order)
    {
        case 1 :
        {
            auto b0 = coeffs[0];
            auto b1 = coeffs[1];
            auto a1 = coeffs[2];

            auto lv1 = state[0];

            for (int i = 0; i < numSamples; ++i)
            {
                auto input  = src[i];
                auto output = input * b0 + lv1;

                dst[i] = output;

                lv1 = (input * b1) - (output * a1);
            }

            juce::dsp::util::snapToZero (lv1);
            state.set (0, lv1);
        }
        break;

        case 2 :
        {
            auto b0 = coeffs[0];
            auto b1 = coeffs[1];
            auto b2 = coeffs[2];
            auto a1 = coeffs[3];
            auto a2 = coeffs[4];

            auto lv1 = state[0];
            auto lv2 = state[1];

            for (int i = 0; i < numSamples; ++i)
            {
                auto input  = src[i];
                auto output = (input * b0) + lv1;
                dst[i]      = output;

                lv1 = (input * b1) - (output * a1) + lv2;
                lv2 = (input * b2) - (output * a2);
            }

            juce::dsp::util::snapToZero (lv1);
            state.set (0, lv1);
            juce::dsp::util::snapToZero (lv2);
            state.set (1, lv2);
        }
        break;

        case 3 :
        {
            auto b0 = coeffs[0];
            auto b1 = coeffs[1];
            auto b2 = coeffs[2];
            auto b3 = coeffs[3];
            auto a1 = coeffs[4];
            auto a2 = coeffs[5];
            auto a3 = coeffs[6];

            auto lv1 = state[0];
            auto lv2 = state[1];
            auto lv3 = state[2];

            for (int i = 0; i < numSamples; ++i)
            {
                auto input  = src[i];
                auto output = (input * b0) + lv1;
                dst[i]      = output;

                lv1 = (input * b1) - (output * a1) + lv2;
                lv2 = (input * b2) - (output * a2) + lv3;
                lv3 = (input * b3) - (output * a3);
            }

            juce::dsp::util::snapToZero (lv1);
            state.set (0, lv1);
            juce::dsp::util::snapToZero (lv2);
            state.set (1, lv2);
            juce::dsp::util::snapToZero (lv3);
            state.set (2, lv3);
        }
        break;

        default :
        {
            for (int i = 0; i < numSamples; ++i)
            {
                auto input  = src[i];
                auto output = (input * coeffs[0]) + state[0];
                dst[i]      = output;

                for (int j = 0; j < order - 1; ++j)
                    state.set (j, (input * coeffs[j + 1])
                                      - (output * coeffs[order + j + 1]) + state[j + 1]);

                state.set (order - 1,
                           (input * coeffs[order]) - (output * coeffs[order * 2]));
            }

            snapToZero();
        }
    }
}

template class Filter< float >;
template class Filter< double >;


/*--------------------------------------------------------------------------------------------------------------*/

template < typename SampleType, size_t numChannels >
MultiFilter< SampleType, numChannels >::MultiFilter()
{
    while ((size_t) filters.size() < numChannels)
        filters.add (new Filter< SampleType >);
}

template < typename SampleType, size_t numChannels >
void MultiFilter< SampleType, numChannels >::prepare() noexcept
{
    for (auto& filter : filters)
        filter->prepare();
}

template < typename SampleType, size_t numChannels >
void MultiFilter< SampleType, numChannels >::reset (SampleType resetToValue)
{
    for (auto& filter : filters)
        filter->reset (resetToValue);
}

template < typename SampleType, size_t numChannels >
void MultiFilter< SampleType, numChannels >::process (AudioBuffer& buffer) noexcept
{
    for (auto& filter : filters)
        filter->coefs = coefs;

    for (int channel = 0;
         channel < std::min (buffer.getNumChannels(), (int) numChannels);
         ++channel)
    {
        filters[channel]->process (buffer.getWritePointer (channel),
                                   buffer.getNumSamples());
    }
}

template < typename SampleType, size_t numChannels >
void MultiFilter< SampleType, numChannels >::processChannel (int channel, SampleType* audio, int numSamples) noexcept
{
    if (channel < filters.size())
    {
        filters[channel]->coefs = coefs;

        filters[channel]->process (audio, numSamples);
    }
}

template struct MultiFilter< float, 1 >;
template struct MultiFilter< float, 2 >;
template struct MultiFilter< double, 1 >;
template struct MultiFilter< double, 2 >;

}  // namespace bav::dsp::filters
