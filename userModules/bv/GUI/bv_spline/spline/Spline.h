#pragma once

#include <bv_spline/common/Common.h>

namespace bav::spline
{
struct Spline : SerializableData
{
    void updatePoints (const juce::Rectangle< float >& bounds);

    float getPoint (int index) const;

    void resize (int newNumPoints);

    Knots  knots;
    Points points;

private:
    void serialize (TreeReflector& ref) final;
};

}  // namespace bav::spline
