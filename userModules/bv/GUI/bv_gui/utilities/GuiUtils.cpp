
namespace bav::gui
{
ScopedCursor::ScopedCursor (
    const juce::MouseCursor& cursor,
    juce::MouseCursor        cursorToResetTo)
    : resetCursor (std::move (cursorToResetTo))
{
    juce::Desktop::getInstance().getMainMouseSource().showMouseCursor (cursor);
}

ScopedCursor::~ScopedCursor()
{
    juce::Desktop::getInstance().getMainMouseSource().showMouseCursor (
        resetCursor);
}

ScopedWaitCursor::ScopedWaitCursor()
    : ScopedCursor (juce::MouseCursor::WaitCursor)
{
}

void addAndMakeVisible (juce::Component* parent, juce::Component& child)
{
    jassert (parent != nullptr);
    parent->addAndMakeVisible (child);
}

juce::Button::ButtonState boolToButtonState (const bool isOn) noexcept
{
    if (isOn) return juce::Button::ButtonState::buttonDown;

    return juce::Button::ButtonState::buttonNormal;
}

bool buttonStateToBool (juce::Button::ButtonState state) noexcept
{
    if (state == juce::Button::ButtonState::buttonDown)
        return true;

    return false;
}

static inline juce::Rectangle< float > scaleRect (const juce::Rectangle< float >& source,
                                                  const juce::Rectangle< float >& ratio)
{
    auto newWidth  = source.getWidth() * ratio.getWidth();
    auto newHeight = source.getHeight() * ratio.getHeight();
    auto newX      = source.getX() + (ratio.getX() * source.getWidth());
    auto newY      = source.getY() + (ratio.getY() * source.getHeight());

    return {newX, newY, newWidth, newHeight};
}

static inline juce::AffineTransform getRectTransformFor (const juce::Rectangle< float >& source,
                                                         const juce::Rectangle< float >& target)
{
    auto newX = target.getX();
    auto newY = target.getY();

    auto scaleX = target.getWidth() / source.getWidth();
    auto scaleY = target.getHeight() / source.getHeight();

    return juce::AffineTransform::translation (-source.getX(), -source.getY())
        .scaled (scaleX, scaleY)
        .translated (newX, newY);
}

void scale (juce::Component& component, const juce::Rectangle< float >& boundsRatio)
{
    if (boundsRatio.isEmpty())
        return;

    if (auto* parent = component.getParentComponent())
    {
        auto scaledBounds = scaleRect (parent->getLocalBounds().toFloat(),
                                       boundsRatio);

        auto compBounds = scaledBounds.getSmallestIntegerContainer();
        auto transform  = getRectTransformFor (compBounds.toFloat(), scaledBounds);

        component.setTransform (juce::AffineTransform());
        component.setBounds (compBounds);
        component.setTransform (transform);
    }
}

}  // namespace bav::gui
