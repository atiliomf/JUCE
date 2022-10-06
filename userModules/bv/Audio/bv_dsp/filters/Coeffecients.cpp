
namespace bav::dsp::filters
{
template < typename T >
Coefs< T >::Coefs()
{
    this->ensureStorageAllocated (8);
}

template < typename T >
Coefs< T >& Coefs< T >::operator= (std::initializer_list< T > list)
{
    int a0Index = 2;

    if (list.size() == 6)
        a0Index = 3;
    else if (list.size() == 8)
        a0Index = 4;

    this->clearQuick();

    int index = 0;

    T a0inv = (T) 0;

    for (auto& element : list)
    {
        if (index != a0Index)
            this->add (element);
        else
            a0inv = (T) 1 / element;

        ++index;
    }

    for (auto& c : *this)
        c *= a0inv;

    return *this;
}

template struct Coefs< float >;
template struct Coefs< double >;

/*--------------------------------------------------------------------------------------------------------------*/

template < typename NumericType >
Coefficients< NumericType >::Coefficients()
{
    coefficients.ensureStorageAllocated (8);
    coefficients.fill ((NumericType) 0);
}

template < typename NumericType >
void Coefficients< NumericType >::makeFirstOrderLowPass (double      sampleRate,
                                                         NumericType frequency)
{
    jassert (sampleRate > 0.0);
    jassert (frequency > 0 && frequency <= static_cast< float > (sampleRate * 0.5));

    const auto n = std::tan (juce::MathConstants< NumericType >::pi * frequency
                             / static_cast< NumericType > (sampleRate));

    coefficients = {n, n, n + 1, n - 1};
}

template < typename NumericType >
void Coefficients< NumericType >::makeFirstOrderHighPass (double      sampleRate,
                                                          NumericType frequency)
{
    jassert (sampleRate > 0.0);
    jassert (frequency > 0 && frequency <= static_cast< float > (sampleRate * 0.5));

    const auto n = std::tan (juce::MathConstants< NumericType >::pi * frequency
                             / static_cast< NumericType > (sampleRate));

    coefficients = {1, -1, n + 1, n - 1};
}

template < typename NumericType >
void Coefficients< NumericType >::makeFirstOrderAllPass (double      sampleRate,
                                                         NumericType frequency)
{
    jassert (sampleRate > 0.0);
    jassert (frequency > 0 && frequency <= static_cast< float > (sampleRate * 0.5));

    const auto n = std::tan (juce::MathConstants< NumericType >::pi * frequency
                             / static_cast< NumericType > (sampleRate));

    coefficients = {n - 1, n + 1, n + 1, n - 1};
}

template < typename NumericType >
void Coefficients< NumericType >::makeLowPass (double      sampleRate,
                                               NumericType frequency,
                                               NumericType Q)
{
    const auto n = 1
                 / std::tan (juce::MathConstants< NumericType >::pi * frequency
                             / static_cast< NumericType > (sampleRate));
    const auto nSquared = n * n;
    const auto invQ     = (NumericType) 1 / Q;
    const auto c1       = 1 / (1 + invQ * n + nSquared);

    coefficients = {
        c1, c1 * 2, c1, 1, c1 * 2 * (1 - nSquared), c1 * (1 - invQ * n + nSquared)};
}

template < typename NumericType >
void Coefficients< NumericType >::makeHighPass (double      sampleRate,
                                                NumericType frequency,
                                                NumericType Q)
{
    const auto n        = std::tan (juce::MathConstants< NumericType >::pi * frequency
                             / static_cast< NumericType > (sampleRate));
    const auto nSquared = n * n;
    const auto invQ     = (NumericType) 1 / Q;
    const auto c1       = 1 / (1 + invQ * n + nSquared);

    coefficients = {
        c1, c1 * -2, c1, 1, c1 * 2 * (nSquared - 1), c1 * (1 - invQ * n + nSquared)};
}

template < typename NumericType >
void Coefficients< NumericType >::makeBandPass (double      sampleRate,
                                                NumericType frequency,
                                                NumericType Q)
{
    jassert (sampleRate > 0.0);
    jassert (frequency > 0 && frequency <= static_cast< float > (sampleRate * 0.5));
    jassert (Q > 0.0);

    const auto n = 1
                 / std::tan (juce::MathConstants< NumericType >::pi * frequency
                             / static_cast< NumericType > (sampleRate));
    const auto nSquared = n * n;
    const auto invQ     = 1 / Q;
    const auto c1       = 1 / (1 + invQ * n + nSquared);

    coefficients = {c1 * n * invQ,
                    0,
                    -c1 * n * invQ,
                    1,
                    c1 * 2 * (1 - nSquared),
                    c1 * (1 - invQ * n + nSquared)};
}

template < typename NumericType >
void Coefficients< NumericType >::makeNotch (double      sampleRate,
                                             NumericType frequency,
                                             NumericType Q)
{
    jassert (sampleRate > 0.0);
    jassert (frequency > 0 && frequency <= static_cast< float > (sampleRate * 0.5));
    jassert (Q > 0.0);

    const auto n = 1
                 / std::tan (juce::MathConstants< NumericType >::pi * frequency
                             / static_cast< NumericType > (sampleRate));
    const auto nSquared = n * n;
    const auto invQ     = 1 / Q;
    const auto c1       = 1 / (1 + n * invQ + nSquared);
    const auto b0       = c1 * (1 + nSquared);
    const auto b1       = 2 * c1 * (1 - nSquared);

    coefficients = {b0, b1, b0, 1, b1, c1 * (1 - n * invQ + nSquared)};
}

template < typename NumericType >
void Coefficients< NumericType >::makeAllPass (double      sampleRate,
                                               NumericType frequency,
                                               NumericType Q)
{
    jassert (sampleRate > 0);
    jassert (frequency > 0 && frequency <= sampleRate * 0.5);
    jassert (Q > 0);

    const auto n = 1
                 / std::tan (juce::MathConstants< NumericType >::pi * frequency
                             / static_cast< NumericType > (sampleRate));
    const auto nSquared = n * n;
    const auto invQ     = 1 / Q;
    const auto c1       = 1 / (1 + invQ * n + nSquared);
    const auto b0       = c1 * (1 - n * invQ + nSquared);
    const auto b1       = c1 * 2 * (1 - nSquared);

    coefficients = {b0, b1, 1, 1, b1, b0};
}

template < typename NumericType >
void Coefficients< NumericType >::makeLowShelf (double      sampleRate,
                                                NumericType cutOffFrequency,
                                                NumericType Q,
                                                NumericType gainFactor)
{
    jassert (sampleRate > 0.0);
    jassert (cutOffFrequency > 0.0 && cutOffFrequency <= sampleRate * 0.5);
    jassert (Q > 0.0);

    const auto A       = juce::jmax (static_cast< NumericType > (0.0), std::sqrt (gainFactor));
    const auto aminus1 = A - 1;
    const auto aplus1  = A + 1;
    const auto omega   = (2 * juce::MathConstants< NumericType >::pi
                        * juce::jmax (cutOffFrequency, static_cast< NumericType > (2.0)))
                     / static_cast< NumericType > (sampleRate);
    const auto coso             = std::cos (omega);
    const auto beta             = std::sin (omega) * std::sqrt (A) / Q;
    const auto aminus1TimesCoso = aminus1 * coso;

    coefficients = {A * (aplus1 - aminus1TimesCoso + beta),
                    A * 2 * (aminus1 - aplus1 * coso),
                    A * (aplus1 - aminus1TimesCoso - beta),
                    aplus1 + aminus1TimesCoso + beta,
                    -2 * (aminus1 + aplus1 * coso),
                    aplus1 + aminus1TimesCoso - beta};
}

template < typename NumericType >
void Coefficients< NumericType >::makeHighShelf (double      sampleRate,
                                                 NumericType cutOffFrequency,
                                                 NumericType Q,
                                                 NumericType gainFactor)
{
    jassert (sampleRate > 0);
    jassert (cutOffFrequency > 0
             && cutOffFrequency <= static_cast< NumericType > (sampleRate * 0.5));
    jassert (Q > 0);

    const auto A       = juce::jmax (static_cast< NumericType > (0.0), std::sqrt (gainFactor));
    const auto aminus1 = A - 1;
    const auto aplus1  = A + 1;
    const auto omega   = (2 * juce::MathConstants< NumericType >::pi
                        * juce::jmax (cutOffFrequency, static_cast< NumericType > (2.0)))
                     / static_cast< NumericType > (sampleRate);
    const auto coso             = std::cos (omega);
    const auto beta             = std::sin (omega) * std::sqrt (A) / Q;
    const auto aminus1TimesCoso = aminus1 * coso;

    coefficients = {A * (aplus1 + aminus1TimesCoso + beta),
                    A * -2 * (aminus1 + aplus1 * coso),
                    A * (aplus1 + aminus1TimesCoso - beta),
                    aplus1 - aminus1TimesCoso + beta,
                    2 * (aminus1 - aplus1 * coso),
                    aplus1 - aminus1TimesCoso - beta};
}

template < typename NumericType >
void Coefficients< NumericType >::makePeakFilter (double      sampleRate,
                                                  NumericType frequency,
                                                  NumericType Q,
                                                  NumericType gainFactor)
{
    jassert (sampleRate > 0);
    jassert (frequency > 0
             && frequency <= static_cast< NumericType > (sampleRate * 0.5));
    jassert (Q > 0);
    jassert (gainFactor > 0);

    const auto A     = juce::jmax (static_cast< NumericType > (0.0), std::sqrt (gainFactor));
    const auto omega = (2 * juce::MathConstants< NumericType >::pi
                        * juce::jmax (frequency, static_cast< NumericType > (2.0)))
                     / static_cast< NumericType > (sampleRate);
    const auto alpha       = std::sin (omega) / (Q * 2);
    const auto c2          = -2 * std::cos (omega);
    const auto alphaTimesA = alpha * A;
    const auto alphaOverA  = alpha / A;

    coefficients = {
        1 + alphaTimesA, c2, 1 - alphaTimesA, 1 + alphaOverA, c2, 1 - alphaOverA};
}

template < typename NumericType >
int Coefficients< NumericType >::getFilterOrder() const noexcept
{
    return (coefficients.size() - 1) / 2;
}

template < typename NumericType >
NumericType* Coefficients< NumericType >::getRawCoefficients() noexcept
{
    return coefficients.data();
}

template < typename NumericType >
const NumericType* Coefficients< NumericType >::getRawCoefficients() const noexcept
{
    return coefficients.data();
}

template struct Coefficients< float >;
template struct Coefficients< double >;

}  // namespace bav::dsp::filters
