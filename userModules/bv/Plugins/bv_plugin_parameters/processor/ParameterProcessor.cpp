
namespace bav::plugin
{
template < typename SampleType >
ParameterProcessorBase< SampleType >::ParameterProcessorBase (ParameterList& listToUse)
    : list (listToUse)
{
}

template < typename SampleType >
void ParameterProcessorBase< SampleType >::handleMidiMessage (const MidiMessage& m)
{
    list.processMidiMessage (m);
}

template class ParameterProcessorBase< float >;
template class ParameterProcessorBase< double >;

}  // namespace bav::plugin
