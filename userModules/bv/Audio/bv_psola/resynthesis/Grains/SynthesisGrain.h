
#pragma once

namespace bav::dsp::psola
{
template < typename SampleType >
class SynthesisGrain
{
public:
    using Storage = AnalysisGrainStorage< SampleType >;

    SynthesisGrain (const Storage& storageToUse);

    bool isActive() const;

    void startNewGrain (int start, int length);

    SampleType getNextSample();

private:
    const Storage& storage;

    bool active {false};

    int startIndex {0};
    int grainLength {0};

    int currentTick {0};
};

}  // namespace bav::dsp::psola
