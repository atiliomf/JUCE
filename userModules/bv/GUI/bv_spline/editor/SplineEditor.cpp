
namespace bav::spline
{
Editor::Select::Select (Editor& e) : editor (e)
{
}

void Editor::Select::setX (float xP) noexcept
{
    const auto bounds = editor.getAdjustedBounds();
    x                 = (xP - bounds.getX()) / bounds.getWidth();
}

void Editor::Select::mouseExited()
{
    x = -1;
}

void Editor::Select::addWidth (float w, float scrollSensitivity)
{
    width = juce::jlimit (scrollSensitivity, 1.f, width + (w * scrollSensitivity));
}

juce::Range< float > Editor::Select::getRange() const noexcept
{
    return {
        juce::jlimit (0.f, 1.f, x - width),
        juce::jlimit (0.f, 1.f, x + width)};
}

void Editor::Select::paint (juce::Graphics& g)
{
    if (x == -1.f) return;
    g.setColour (editor.attributes.selection.color);

    const auto bounds = editor.getAdjustedBounds();

    const auto w       = bounds.getWidth();
    const auto boundsX = bounds.getX();

    auto xN     = juce::jlimit (0.f, 1.f, x - width) * w + boundsX;
    auto rightN = juce::jlimit (0.f, 1.f, x + width) * w + boundsX;

    juce::Rectangle< float > area (xN,
                                   bounds.getY(),
                                   rightN - xN,
                                   bounds.getHeight());

    g.fillRect (area);
}

Editor::Attributes::Attributes()
{
    border.thickness    = 2.f;
    point.thickness     = 3.f;
    curve.thickness     = 1.5f;
    selection.thickness = 5.f;

    border.color     = juce::Colours::cadetblue;
    point.color      = juce::Colours::cadetblue;
    curve.color      = juce::Colours::yellowgreen;
    selection.color  = juce::Colours::yellowgreen.withAlpha (.2f);
    background.color = juce::Colours::cadetblue.darker (2);

    pntrs.push_back ({&border});
    pntrs.push_back ({&point});
    pntrs.push_back ({&curve});
    pntrs.push_back ({&selection});
    pntrs.push_back ({&background});
}

float Editor::Attributes::getThickestThickness() const
{
    auto max = border.thickness;

    for (auto& pntr : pntrs)
        if (pntr->thickness > max)
            max = pntr->thickness;

    return max;
}


void Editor::setBorderColour (Color c)
{
    attributes.border.color = c;
    repaint();
}

void Editor::setPointColour (Color c)
{
    attributes.point.color = c;
    repaint();
}

void Editor::setCurveColour (Color c)
{
    attributes.curve.color = c;
    repaint();
}

void Editor::setSelectionColour (Color c)
{
    attributes.selection.color = c;
    repaint();
}

void Editor::setBackgroundColour (Color c)
{
    attributes.background.color = c;
    repaint();
}

void Editor::setBorderThickness (float pxl)
{
    attributes.border.thickness = pxl;
    resized();
}

void Editor::setPointThickness (float pxl)
{
    attributes.point.thickness = pxl;
    resized();
}

void Editor::setCurveThickness (float pxl)
{
    attributes.curve.thickness = pxl;
    resized();
}

void Editor::setSelectionThickness (float pxl)
{
    attributes.selection.thickness = pxl;
    resized();
}

void Editor::paint (juce::Graphics& g)
{
    g.setImageResamplingQuality (juce::Graphics::lowResamplingQuality);

    const auto bounds = getAdjustedBounds();

    g.setColour (attributes.background.color);
    g.fillRect (bounds);

    g.setColour (attributes.border.color);
    g.drawRect (bounds, attributes.border.thickness);

    g.setColour (attributes.curve.color);

    const auto x = bounds.getX();

    auto yStart = spline.getPoint (0);

    for (auto i = 1; i < static_cast< decltype (i) > (spline.points.size()); ++i)
    {
        const auto yEnd   = spline.getPoint (i);
        const auto xEnd   = i + x;
        const auto xStart = xEnd - 1;

        juce::Line< float > cLine (xStart, yStart, xEnd, yEnd);

        g.drawLine (cLine, attributes.curve.thickness);
        yStart = yEnd;
    }

    select.paint (g);

    g.setColour (attributes.point.color);

    for (const auto& knot : spline.knots)
    {
        const auto  point     = knot.getDenormalizedPoint (bounds);
        const auto& attribute = knot.isSelected() ? attributes.selection : attributes.point;

        drawPoint (point, attribute, g);
    }
}

void Editor::drawPoint (const Point&     knot,
                        const Attribute& attribute,
                        juce::Graphics&  g)
{
    const auto width = attribute.thickness * 2.f + 1.f;

    g.fillEllipse (knot.x - attribute.thickness,
                   knot.y - attribute.thickness,
                   width, width);
}

Point Editor::normalizePoint (const Point& position) const noexcept
{
    const auto bounds = getAdjustedBounds();

    return {
        (position.x - bounds.getX()) / bounds.getWidth(),
        (position.y - bounds.getY()) / bounds.getHeight()};
}

juce::Rectangle< float > Editor::getAdjustedBounds() const
{
    return getLocalBounds().toFloat().reduced (attributes.getThickestThickness());
}

void Editor::resized()
{
    spline.resize (juce::roundToInt (getAdjustedBounds().getWidth()));
    updateCurve();
}

void Editor::timerCallback()
{
    spline.knots.sort();
    updateCurve();
}

void Editor::mouseWheelMove (const juce::MouseEvent&, const juce::MouseWheelDetails& wheel)
{
    const auto direc = wheel.deltaY > 0 ? 1.f : -1.f;
    select.addWidth (direc);
    repaint();
}

void Editor::mouseMove (const juce::MouseEvent& evt)
{
    select.setX (evt.position.x);
    repaint();
}

void Editor::mouseDown (const juce::MouseEvent&)
{
    spline.knots.select (select.getRange());
    repaint();
}

void Editor::mouseDrag (const juce::MouseEvent& evt)
{
    select.setX (evt.position.x);

    if (spline.knots.drag (normalizePoint (evt.getOffsetFromDragStart().toFloat())))
        updateCurve();
}

void Editor::mouseUp (const juce::MouseEvent& evt)
{
    if (evt.mouseWasDraggedSinceMouseDown())
    {
        spline.knots.removeOffLimits();
    }
    else
    {
        const auto pos = normalizePoint (evt.position);

        if (evt.mods.isLeftButtonDown())
        {
            if (pos.x > 0.f && pos.x < 1.f)
                spline.knots.add (pos);
        }
        else
        {
            spline.knots.remove (select.getRange());
        }
    }

    spline.knots.deselect();
    updateCurve();
}

void Editor::mouseExit (const juce::MouseEvent&)
{
    select.mouseExited();
    repaint();
}

void Editor::updateCurve()
{
    spline.updatePoints (getAdjustedBounds());
    repaint();
}

}  // namespace bav::spline
