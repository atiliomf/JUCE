
namespace bav::dsp
{
template class ReorderableEffect< float >;
template class ReorderableEffect< double >;


template < typename SampleType >
int ReorderableFxChain< SampleType >::addEffect (Effect* effect,
                                                 int     numberInChain,
                                                 bool    addAsBypassed)
{
    jassert (effect != nullptr);

    const auto newNumber = assignNewEffectNumber (numberInChain);

    jassert (newNumber >= 0);

    addEffectWithNumber (effect, newNumber, addAsBypassed);

    return newNumber;
}

template < typename SampleType >
int ReorderableFxChain< SampleType >::addEffectBetweenTwoOthers (Effect* effectToAdd,
                                                                 Effect* slotBeforeNew,
                                                                 Effect* slotAfterNew,
                                                                 bool    addEffectAsBypassed)
{
    jassert (effectToAdd != nullptr);

    if (slotBeforeNew == nullptr || slotAfterNew == nullptr) return -1;

    if (slotBeforeNew->effectNumber == slotAfterNew->effectNumber) return -1;

    if (slotBeforeNew->effectNumber > slotAfterNew->effectNumber)
    {
        auto* temp    = slotBeforeNew;
        slotBeforeNew = slotAfterNew;
        slotAfterNew  = temp;
    }

    for (int i = slotBeforeNew->effectNumber + 1; i < slotAfterNew->effectNumber;
         ++i)
    {
        if (isEffectNumberAvailable (i))
        {
            addEffectWithNumber (effectToAdd, i, addEffectAsBypassed);
            return i;
        }
    }

    const auto newNumber = slotBeforeNew->effectNumber + 1;

    for (int i = newNumber; i < highestCurrentEffectNumber(); ++i)
        if (auto* fct = getEffect (i)) fct->effectNumber++;

    addEffectWithNumber (effectToAdd, newNumber, addEffectAsBypassed);
    return newNumber;
}

template < typename SampleType >
void ReorderableFxChain< SampleType >::removeEffect (Effect* effect)
{
    effects.removeObject (effect, true);
}

template < typename SampleType >
void ReorderableFxChain< SampleType >::removeEffect (int numberInChain)
{
    removeEffect (getEffect (numberInChain));
}

template < typename SampleType >
void ReorderableFxChain< SampleType >::removeAllEffects()
{
    effects.clear();
}


template < typename SampleType >
ReorderableEffect< SampleType >* ReorderableFxChain< SampleType >::getEffect (int numberInChain)
{
    if (numberInChain < 0) return nullptr;

    for (auto* effect : effects)
        if (effect->effectNumber == numberInChain) return effect;

    return nullptr;
}

template < typename SampleType >
bool ReorderableFxChain< SampleType >::swapTwoEffects (Effect* first, Effect* second)
{
    if (first == nullptr || second == nullptr) return false;

    const auto initNum   = first->effectNumber;
    first->effectNumber  = second->effectNumber;
    second->effectNumber = initNum;
    return true;
}

template < typename SampleType >
bool ReorderableFxChain< SampleType >::swapTwoEffects (int firstNumInChain, int secondNumInChain)
{
    return swapTwoEffects (getEffect (firstNumInChain),
                           getEffect (secondNumInChain));
}

template < typename SampleType >
bool ReorderableFxChain< SampleType >::moveEffectForwardInChain (Effect* effect)
{
    if (effect == nullptr) return false;
    return swapTwoEffects (effect, getEffect (effect->effectNumber - 1));
}

template < typename SampleType >
bool ReorderableFxChain< SampleType >::moveEffectForwardInChain (int numInChain)
{
    return moveEffectForwardInChain (getEffect (numInChain));
}

template < typename SampleType >
bool ReorderableFxChain< SampleType >::moveEffectBackwardInChain (Effect* effect)
{
    if (effect == nullptr) return false;
    return swapTwoEffects (effect, getEffect (effect->effectNumber + 1));
}

template < typename SampleType >
bool ReorderableFxChain< SampleType >::moveEffectBackwardInChain (int numInChain)
{
    return moveEffectBackwardInChain (getEffect (numInChain));
}

template < typename SampleType >
bool ReorderableFxChain< SampleType >::setEffectBypass (Effect* effect, bool shouldBeBypassed)
{
    if (effect == nullptr) return false;

    effect->isBypassed = shouldBeBypassed;
    return true;
}

template < typename SampleType >
bool ReorderableFxChain< SampleType >::setEffectBypass (int numberInChain, bool shouldBeBypassed)
{
    return setEffectBypass (getEffect (numberInChain), shouldBeBypassed);
}

template < typename SampleType >
int ReorderableFxChain< SampleType >::numEffects() const noexcept
{
    return effects.size();
}

template < typename SampleType >
int ReorderableFxChain< SampleType >::numActiveEffects() const noexcept
{
    int numActive = 0;

    for (auto* effect : effects)
        if (! effect->isBypassed) ++numActive;

    return numActive;
}

template < typename SampleType >
void ReorderableFxChain< SampleType >::bypassAll()
{
    for (auto* effect : effects)
        effect->isBypassed = true;
}

template < typename SampleType >
void ReorderableFxChain< SampleType >::unBypassAll()
{
    for (auto* effect : effects)
        effect->isBypassed = false;
}

template < typename SampleType >
void ReorderableFxChain< SampleType >::prepare (double samplerate, int blocksize)
{
    jassert (samplerate > 0.0 && blocksize > 0);

    lastSamplerate = samplerate;
    lastBlocksize  = blocksize;

    for (auto* effect : effects)
        effect->fxChain_prepare (samplerate, blocksize);
}

template < typename SampleType >
void ReorderableFxChain< SampleType >::process (AudioBuffer& audio)
{
    jassert (! effects.isEmpty());
    jassert (lastSamplerate > 0.0 && lastBlocksize > 0);

    const auto numSamples = audio.getNumSamples();

    audio.clear();

    for (int i = 0; i < effects.size(); ++i)
    {
        if (auto* effect = getEffect (i))
        {
            if (effect->isBypassed)
                effect->fxChain_bypassedBlock (numSamples);
            else
                effect->fxChain_process (audio);
        }
    }
}

template < typename SampleType >
void ReorderableFxChain< SampleType >::bypassedBlock (int numSamples)
{
    for (auto* effect : effects)
        effect->fxChain_bypassedBlock (numSamples);
}

template < typename SampleType >
int ReorderableFxChain< SampleType >::assignNewEffectNumber (int requestedNumber)
{
    if (effects.isEmpty()) return requestedNumber;

    if (requestedNumber < 0) requestedNumber = 0;

    if (isEffectNumberAvailable (requestedNumber)) return requestedNumber;

    const auto maxNumber = highestCurrentEffectNumber();

    for (int num = 0, p = 1, m = -1; num <= maxNumber; ++num, ++p, --m)
    {
        const auto lower = requestedNumber + m;

        if (lower >= 0 && isEffectNumberAvailable (lower)) return lower;

        const auto higher = requestedNumber + p;

        if (higher >= maxNumber && isEffectNumberAvailable (higher))
            return higher;
    }

    return maxNumber + 1;
}

template < typename SampleType >
bool ReorderableFxChain< SampleType >::isEffectNumberAvailable (int numberInChain)
{
    for (auto* effect : effects)
        if (effect->effectNumber == numberInChain) return false;

    return true;
}

template < typename SampleType >
void ReorderableFxChain< SampleType >::addEffectWithNumber (Effect* effect, int newNumber, bool addAsBypassed)
{
    jassert (isEffectNumberAvailable (newNumber));

    effect->effectNumber = newNumber;

    effect->isBypassed = addAsBypassed;

    effects.add (effect);

    jassert (getEffect (newNumber) != nullptr);

    if (lastSamplerate > 0.0 && lastBlocksize > 0)
        getEffect (newNumber)->fxChain_prepare (lastSamplerate, lastBlocksize);
}

template < typename SampleType >
int ReorderableFxChain< SampleType >::highestCurrentEffectNumber()
{
    int maxNumber = -1;

    for (auto* effect : effects)
        if (effect->effectNumber > maxNumber) maxNumber = effect->effectNumber;

    return maxNumber;
}


template class ReorderableFxChain< float >;
template class ReorderableFxChain< double >;

}  // namespace bav::dsp
