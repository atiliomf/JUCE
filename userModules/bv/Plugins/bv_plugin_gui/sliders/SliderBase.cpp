
namespace bav::gui
{
SliderBase::SliderBase (plugin::Parameter& paramToUse)
    : param (paramToUse)
{
    Slider::setName (param.parameterNameVerbose);
    Slider::setComponentID (param.parameterNameVerbose);

    auto range = param.getNormalisableRange();

    auto convertFrom0To1Function = [range] (double currentRangeStart,
                                            double currentRangeEnd,
                                            double normalisedValue) mutable
    {
        range.start = (float) currentRangeStart;
        range.end   = (float) currentRangeEnd;
        return (double) range.convertFrom0to1 ((float) normalisedValue);
    };

    auto convertTo0To1Function = [range] (double currentRangeStart,
                                          double currentRangeEnd,
                                          double mappedValue) mutable
    {
        range.start = (float) currentRangeStart;
        range.end   = (float) currentRangeEnd;
        return (double) range.convertTo0to1 ((float) mappedValue);
    };

    auto snapToLegalValueFunction = [range] (double currentRangeStart,
                                             double currentRangeEnd,
                                             double mappedValue) mutable
    {
        range.start = (float) currentRangeStart;
        range.end   = (float) currentRangeEnd;
        return (double) range.snapToLegalValue ((float) mappedValue);
    };

    juce::NormalisableRange< double > newRange {(double) range.start,
                                                (double) range.end,
                                                std::move (convertFrom0To1Function),
                                                std::move (convertTo0To1Function),
                                                std::move (snapToLegalValueFunction)};

    newRange.interval      = range.interval;
    newRange.skew          = range.skew;
    newRange.symmetricSkew = range.symmetricSkew;

    Slider::setNormalisableRange (newRange);

    Slider::setTextBoxIsEditable (true);
    Slider::setTextValueSuffix (param.getLabel());

    Slider::setTooltip (param.parameterNameShort);

    Slider::setPopupDisplayEnabled (true, false, this);
    Slider::setPopupMenuEnabled (true);
}

void SliderBase::startedDragging()
{
    param.beginGesture();
}

void SliderBase::stoppedDragging()
{
    param.endGesture();
}

}  // namespace bav::gui
