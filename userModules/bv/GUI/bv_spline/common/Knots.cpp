
#include "Interpolation.h"

namespace bav::spline
{
Knot::Knot (float xToUse, float yToUse)
    : location (xToUse, yToUse)
{
}

Knot& Knot::operator= (const Knot& other)
{
    location  = other.location;
    dragStart = other.dragStart;
    selected  = other.selected;

    return *this;
}

void Knot::serialize (TreeReflector& ref)
{
    ref.add ("Location", location);
    ref.add ("DragStart", dragStart);
    ref.add ("Selected", selected);
}

void Knot::select() noexcept
{
    dragStart = location;
    selected  = true;
}

void Knot::deselect()
{
    selected = false;
}

bool Knot::isSelected() const
{
    return selected;
}

bool Knot::drag (const Point& p) noexcept
{
    if (! selected) return false;

    location.x = dragStart.x + p.x;
    location.y = dragStart.y + p.y;

    return true;
}

Point Knot::getDenormalizedPoint (const juce::Rectangle< float >& bounds) const
{
    return {
        bounds.getX() + bounds.getWidth() * location.x,
        bounds.getY() + bounds.getHeight() * location.y};
}


Knots::Knots()
{
    add ({0, 0});
    add ({1, 1});
}

const Knot& Knots::getKnot (int index) const
{
    const auto idx = static_cast< size_type > (index);
    jassert (index >= 0 && idx < size());
    return (*this)[idx];
}

void Knots::serialize (TreeReflector& ref)
{
    ref.add ("Knot", *(static_cast< std::vector< Knot >* > (this)));
}

void Knots::add (const Point& p)
{
    push_back ({p.x, p.y});
}

void Knots::sort()
{
    std::sort (begin(), end(),
               [] (const Knot& a, const Knot& b)
               { return a.location.x < b.location.x; });
}

void Knots::removeOffLimits()
{
    std::remove_if (begin(), end(),
                    [] (const Knot& knot)
                    {
                        if (! knot.isSelected()) return false;
                        return knot.location.x <= 0.f || knot.location.x >= 1.f || knot.location.y <= 0.f || knot.location.y >= 1.f;
                    });
}

void Knots::remove (const juce::Range< float >& range)
{
    for (auto knot = begin(); knot != end(); ++knot)
    {
        if (range.getEnd() < knot->location.x) break;

        if (range.getStart() < knot->location.x)
            erase (knot);
    }
}

void Knots::select (const juce::Range< float >& range) noexcept
{
    for (auto& knot : *this)
    {
        if (knot.location.x > range.getEnd()) return;

        if (knot.location.x >= range.getStart())
            knot.select();
    }
}

bool Knots::drag (const Point& drag) noexcept
{
    bool changed = false;

    for (auto& knot : *this)
        if (knot.drag (drag))
            changed = true;

    return changed;
}

void Knots::deselect()
{
    for (auto& knot : *this)
        knot.deselect();
}

void Knots::makeSpline (Points& spline) const
{
    const auto inc              = 1.f / static_cast< float > (spline.size());
    const auto smallestDistance = inc * 2.f;
    auto       x                = 0.f;
    int        kIdx             = 1;

    for (auto& point : spline)
    {
        if (x >= getKnot (kIdx).location.x)
        {
            ++kIdx;
            kIdx %= size();
        }

        point = juce::jlimit (0.f, 1.f,
                              interpolation::hermitCubic2 (*this, x, smallestDistance, kIdx - 1));

        x += inc;
    }
}

}  // namespace bav::spline
