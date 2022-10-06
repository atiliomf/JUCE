#pragma once

namespace bav::spline::interpolation
{
Point lin (const Point& a, const Point& b, float x) noexcept;

Point nurbs (const Knots& data, float t, const int* k) noexcept;

float hermitCubic (const Knots& data, float x) noexcept;

float hermitCubic2 (const Knots& data, float x, float tolerance, int i1) noexcept;

}  // namespace bav::spline::interpolation
