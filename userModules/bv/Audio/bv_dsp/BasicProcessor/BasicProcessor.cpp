
namespace bav::dsp
{
BasicProcessorBase::BasicProcessorBase (juce::AudioProcessor::BusesProperties busesLayout)
    : AudioProcessor (busesLayout)
{
}

void BasicProcessorBase::prepareToPlay (double, int) { }
void BasicProcessorBase::releaseResources() { }

double BasicProcessorBase::getTailLengthSeconds() const { return 0.0; }

int          BasicProcessorBase::getNumPrograms() { return 1; }
int          BasicProcessorBase::getCurrentProgram() { return 0; }
void         BasicProcessorBase::setCurrentProgram (int) { }
const String BasicProcessorBase::getProgramName (int) { return {}; }
void         BasicProcessorBase::changeProgramName (int, const String&) { }

bool BasicProcessorBase::acceptsMidi() const { return true; }
bool BasicProcessorBase::producesMidi() const { return true; }
bool BasicProcessorBase::supportsMPE() const { return false; }
bool BasicProcessorBase::isMidiEffect() const { return false; }

const String BasicProcessorBase::getName() const { return "ProcessorBase"; }

bool BasicProcessorBase::hasEditor() const { return false; }

juce::AudioProcessorEditor* BasicProcessorBase::createEditor() { return nullptr; }

static inline bool isChannelsetValid (const juce::AudioChannelSet& set)
{
    return set != juce::AudioChannelSet::disabled();
}

bool BasicProcessorBase::isBusesLayoutSupported (const BusesLayout& layout) const
{
    return isChannelsetValid (layout.getMainInputChannelSet()) && isChannelsetValid (layout.getMainOutputChannelSet());
}

void BasicProcessorBase::repaintEditor()
{
    if (auto* editor = getActiveEditor())
        editor->repaint();
}

}  // namespace bav::dsp
