#pragma once

namespace bav::dsp::filters
{
template < typename T >
struct Coefs : juce::Array< T >
{
    Coefs();
    Coefs& operator= (std::initializer_list< T > list);
};

/*--------------------------------------------------------------------------------------------------------------*/

template < typename NumericType >
struct Coefficients
{
    Coefficients();

    void makeFirstOrderLowPass (double sampleRate, NumericType frequency);
    void makeFirstOrderHighPass (double sampleRate, NumericType frequency);
    void makeFirstOrderAllPass (double sampleRate, NumericType frequency);

    void makeLowPass (double sampleRate, NumericType frequency, NumericType Q = inverseRootTwo);
    void makeHighPass (double sampleRate, NumericType frequency, NumericType Q = inverseRootTwo);
    void makeBandPass (double sampleRate, NumericType frequency, NumericType Q = inverseRootTwo);
    void makeNotch (double sampleRate, NumericType frequency, NumericType Q = inverseRootTwo);
    void makeAllPass (double sampleRate, NumericType frequency, NumericType Q = inverseRootTwo);

    //==============================================================================
    /** Returns the coefficients for a low-pass shelf filter with variable Q and gain.
     The gain is a scale factor that the low frequencies are multiplied by, so values
     greater than 1.0 will boost the low frequencies, values less than 1.0 will
     attenuate them.
     */
    void makeLowShelf (double      sampleRate,
                       NumericType cutOffFrequency,
                       NumericType Q          = inverseRootTwo,
                       NumericType gainFactor = (NumericType) 1);

    /** Returns the coefficients for a high-pass shelf filter with variable Q and gain.
     The gain is a scale factor that the high frequencies are multiplied by, so values
     greater than 1.0 will boost the high frequencies, values less than 1.0 will
     attenuate them.
     */
    void makeHighShelf (double      sampleRate,
                        NumericType cutOffFrequency,
                        NumericType Q          = inverseRootTwo,
                        NumericType gainFactor = (NumericType) 1);

    /** Returns the coefficients for a peak filter centred around a
     given frequency, with a variable Q and gain.
     The gain is a scale factor that the centre frequencies are multiplied by, so
     values greater than 1.0 will boost the centre frequencies, values less than
     1.0 will attenuate them.
     */
    void makePeakFilter (double      sampleRate,
                         NumericType centreFrequency,
                         NumericType Q          = inverseRootTwo,
                         NumericType gainFactor = (NumericType) 1);

    //==============================================================================
    /** Returns the filter order associated with the coefficients */
    int getFilterOrder() const noexcept;

    NumericType*       getRawCoefficients() noexcept;
    const NumericType* getRawCoefficients() const noexcept;

    //==============================================================================
    /** The raw coefficients.
     You should leave these numbers alone unless you really know what you're doing.
     */
    Coefs< NumericType > coefficients;

private:
    static constexpr NumericType inverseRootTwo =
        static_cast< NumericType > (0.70710678118654752440L);
};

}  // namespace bav::dsp::filters
