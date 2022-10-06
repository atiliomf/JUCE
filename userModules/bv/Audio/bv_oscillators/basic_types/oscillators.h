
#pragma once

#include <bv_dsp/bv_dsp.h>

namespace bav::dsp::osc
{
template < typename SampleType >
struct Phase
{
    void       resetPhase() noexcept;
    void       setFrequency (SampleType frequency, SampleType sampleRate);
    SampleType getIncrement() const;
    SampleType next (SampleType wrapLimit) noexcept;

private:
    SampleType phase = 0, increment = 0;
};

/*--------------------------------------------------------------------------------------------*/

template < typename SampleType >
struct Oscillator
{
    virtual ~Oscillator() = default;

    virtual void       resetPhase()                                               = 0;
    virtual void       setFrequency (SampleType frequency, SampleType sampleRate) = 0;
    virtual SampleType getSample()                                                = 0;

    void getSamples (SampleType* output, int numSamples);
};

/*--------------------------------------------------------------------------------------------*/

template < typename SampleType >
struct Sine : public Oscillator< SampleType >
{
    Sine();

    void       resetPhase() final;
    void       setFrequency (SampleType frequency, SampleType sampleRate) final;
    SampleType getSample() final;

private:
    Phase< SampleType >   phase;
    static constexpr auto twoPi = static_cast< SampleType > (3.141592653589793238 * 2.0);
};

/*--------------------------------------------------------------------------------------------*/

template < typename SampleType >
struct Saw : public Oscillator< SampleType >
{
    Saw();

    void       resetPhase();
    void       setFrequency (SampleType frequency, SampleType sampleRate) final;
    SampleType getSample() final;

private:
    Phase< SampleType > phase;
};

/*--------------------------------------------------------------------------------------------*/

template < typename SampleType >
struct Square : public Oscillator< SampleType >
{
    Square();

    void       resetPhase() final;
    void       setFrequency (SampleType frequency, SampleType sampleRate) final;
    SampleType getSample() final;
    SampleType getIncrement() const;

private:
    Phase< SampleType > phase;
};

/*--------------------------------------------------------------------------------------------*/

template < typename SampleType >
struct Triangle : public Oscillator< SampleType >
{
    Triangle();

    void       resetPhase() final;
    void       setFrequency (SampleType frequency, SampleType sampleRate) final;
    SampleType getSample() final;

private:
    Square< SampleType > square;
    SampleType           sum = 1;
};

}  // namespace bav::dsp::osc
