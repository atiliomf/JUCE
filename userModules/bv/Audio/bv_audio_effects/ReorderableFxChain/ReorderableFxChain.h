
#pragma once


namespace bav::dsp
{
/// forward declaration...
template < typename SampleType >
class ReorderableFxChain;


/*
     This base class defines the basic interface any processor must use to be a member of a reorderable FX chain.
     The FX chain owns and manages a collection of these objects, and will delete them when they are removed.
     */
template < typename SampleType >
class ReorderableEffect
{
    using AudioBuffer = juce::AudioBuffer< SampleType >;

public:
    ReorderableEffect()          = default;
    virtual ~ReorderableEffect() = default;

protected:
    /*
            These are the virtual functions you should override to implement the effect's functionality. 
            These purposefully have unique names so that you can easily add the ReorderableEffect interface to an existing effect or processor class; usually you can just reroute these functions to your previously implemented process(), prepare(), etc.
        */

    // called in the subclass to apply the effect to the input audio. Output should be rendered in place.
    virtual void fxChain_process (AudioBuffer& audio) = 0;

    virtual void fxChain_bypassedBlock (const int /*numSamples*/) { }

    virtual void fxChain_prepare (double /*samplerate*/, int /*blocksize*/) { }

private:
    friend class ReorderableFxChain< SampleType >;

    int  effectNumber;
    bool isBypassed = false;
};


/*
     Base class for an engine object that owns and manages a series of individual effects processors, and can reorder them dynamically.
     */
template < typename SampleType >
class ReorderableFxChain
{
    using Effect      = ReorderableEffect< SampleType >;
    using AudioBuffer = juce::AudioBuffer< SampleType >;

public:
    ReorderableFxChain()          = default;
    virtual ~ReorderableFxChain() = default;

    // adds an effect to the chain. Returns the actual number in the chain that the effect was placed in. (Doesn't move anything in the chain around.)
    int addEffect (Effect* effect,
                   int     numberInChain,
                   bool    addAsBypassed = false);

    // adds an effect to the chain between two others in the chain. If it needs to, this function will move everything in the chain after the new node up by one position number to fit the new effect in.
    // -1 indicates an error, and that the new effect was not added to the chain.
    // to add an effect in a much more 'friendly' way, use the above addEffect() function -- the effect is guarunteed to be added to the chain, and it doesn't move any of the existing effects around.
    int addEffectBetweenTwoOthers (Effect* effectToAdd,
                                   Effect* slotBeforeNew,
                                   Effect* slotAfterNew,
                                   bool    addEffectAsBypassed = false);

    void removeEffect (Effect* effect);
    void removeEffect (int numberInChain);
    void removeAllEffects();

    Effect* getEffect (int numberInChain);

    bool swapTwoEffects (Effect* first, Effect* second);
    bool swapTwoEffects (int firstNumInChain, int secondNumInChain);

    bool moveEffectForwardInChain (Effect* effect);
    bool moveEffectForwardInChain (int numInChain);

    bool moveEffectBackwardInChain (Effect* effect);
    bool moveEffectBackwardInChain (int numInChain);

    bool setEffectBypass (Effect* effect, bool shouldBeBypassed);
    bool setEffectBypass (int numberInChain, bool shouldBeBypassed);

    int numEffects() const noexcept;
    int numActiveEffects() const noexcept;

    void bypassAll();
    void unBypassAll();

    void prepare (double samplerate, int blocksize);

    /*
            The top-level rendering callback that renders the entire effects chain in order.
            Audio will be output in place.
         */
    void process (AudioBuffer& audio);
    void bypassedBlock (int numSamples);

private:
    // if the current effect number is out of range or taken, attempts to return the closest availabe effect number to the passed number.
    int assignNewEffectNumber (int requestedNumber);

    // returns true if the effect number is not assigned to an effect currently in this chain.
    bool isEffectNumberAvailable (int numberInChain);

    void addEffectWithNumber (Effect* effect, int newNumber, bool addAsBypassed);

    int highestCurrentEffectNumber();

    /*
        */

    double lastSamplerate = 0.0;
    int    lastBlocksize  = 0;

    juce::OwnedArray< Effect > effects;
};

}  // namespace bav::dsp
