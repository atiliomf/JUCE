#pragma once

#include <juce_graphics/juce_graphics.h>
#include <bv_serializing/bv_serializing.h>


namespace bav::spline
{
using Point = juce::Point< float >;

using Points = std::vector< float >;
}  // namespace bav::spline

#include "Knots.h"
#include "Interpolation.h"
