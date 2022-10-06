
namespace bav::spline
{
void Spline::serialize (TreeReflector& ref)
{
    ref.add ("Knots", knots);
    ref.add ("Point", points);
}

void Spline::updatePoints (const juce::Rectangle< float >& bounds)
{
    knots.makeSpline (points);

    const auto height = bounds.getHeight();
    const auto y      = bounds.getY();

    for (auto& point : points)
        point = point * height + y;
}

void Spline::resize (int newNumPoints)
{
    points.resize (static_cast< Points::size_type > (newNumPoints));
}

float Spline::getPoint (int index) const
{
    return points[static_cast< Points::size_type > (index)];
}

}  // namespace bav::spline
