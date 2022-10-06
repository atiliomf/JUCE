

namespace bav::gui
{
static inline juce::DrawablePath createStopIcon (juce::Colour colour)
{
    juce::Path squarePath;
    squarePath.addRectangle (100.0f, 100.0f, 100.0f, 100.0f);

    juce::DrawablePath squareImage;
    squareImage.setFill (colour);
    squareImage.setPath (squarePath);

    return squareImage;
}

static inline juce::DrawablePath createPlayIcon (juce::Colour colour)
{
    juce::Path trianglePath;
    trianglePath.addTriangle (0.0f, 0.0f, 0.0f, 100.0f, 100.0f, 50.0f);

    juce::DrawablePath triangleImage;
    triangleImage.setFill (colour);
    triangleImage.setPath (trianglePath);

    return triangleImage;
}

static inline juce::DrawablePath createPauseIcon (juce::Colour colour)
{
    juce::Path pausePath;
    pausePath.addRectangle (0.0f, 0.0f, 20.0f, 100.0f);
    pausePath.addRectangle (60.0f, 0.0f, 20.0f, 100.0f);

    juce::DrawablePath pauseImage;
    pauseImage.setFill (colour);
    pauseImage.setPath (pausePath);

    return pauseImage;
}

static inline juce::DrawablePath createNextIcon (juce::Colour colour)
{
    juce::Path p;
    p.addTriangle (0.0f, 0.0f, 0.0f, 100.0f, 90.0f, 50.0f);
    p.addRectangle (90, 0, 10, 100);

    juce::DrawablePath drawablePath;
    drawablePath.setFill (colour);
    drawablePath.setPath (p);

    return drawablePath;
}

static inline juce::DrawablePath createPreviousIcon (juce::Colour colour)
{
    juce::Path p;
    p.addTriangle (100.0f, 100.0f, 100.0f, 0.0f, 10.0f, 50.0f);
    p.addRectangle (0, 0, 10, 100);

    juce::DrawablePath drawablePath;
    drawablePath.setFill (colour);
    drawablePath.setPath (p);

    return drawablePath;
}

static inline juce::DrawablePath createShuffleForwardIcon (juce::Colour colour)
{
    juce::Path p;
    p.addTriangle (0.0f, 0.0f, 0.0f, 100.0f, 50.0f, 50.0f);
    p.addTriangle (50.0f, 0.0f, 50.0f, 100.0f, 100.0f, 50.0f);

    juce::DrawablePath drawablePath;
    drawablePath.setFill (colour);
    drawablePath.setPath (p);

    return drawablePath;
}

static inline juce::DrawablePath createShuffleBackIcon (juce::Colour colour)
{
    juce::Path p;
    p.addTriangle (0.0f, 50.0f, 75.0f, 0.0f, 75.0f, 100.0f);
    p.addTriangle (75.0f, 50.0f, 150.0f, 0.0f, 150.0f, 100.0f);

    juce::DrawablePath drawablePath;
    drawablePath.setFill (colour);
    drawablePath.setPath (p);

    return drawablePath;
}

static inline juce::DrawablePath createEjectIcon (juce::Colour colour)
{
    juce::Path p;
    p.addTriangle (0.0f, 50.0f, 75.0f, 0.0f, 75.0f, 100.0f);
    p.addTriangle (75.0f, 50.0f, 150.0f, 0.0f, 150.0f, 100.0f);

    juce::DrawablePath drawablePath;
    drawablePath.setFill (colour);
    drawablePath.setPath (p);

    return drawablePath;
}

static inline juce::DrawablePath createAddIcon (juce::Colour colour)
{
    juce::Path p;
    p.startNewSubPath (50.0f, 0.0f);
    p.lineTo (50.0f, 100.0f);
    p.startNewSubPath (0.0f, 50.0f);
    p.lineTo (100.0f, 50.0f);

    juce::DrawablePath drawablePath;
    drawablePath.setFill (juce::Colours::white.withAlpha (0.0f));
    drawablePath.setStrokeFill (colour);
    drawablePath.setStrokeThickness (15);
    drawablePath.setPath (p);

    return drawablePath;
}

static inline juce::DrawablePath createSearchIcon (juce::Colour colour)
{
    juce::Path p;
    p.addEllipse (20, 0, 80, 80);
    p.startNewSubPath (0.0f, 100.0f);
    p.lineTo (35.0f, 65.0f);

    juce::DrawablePath drawablePath;
    drawablePath.setFill (juce::Colours::white.withAlpha (0.0f));
    drawablePath.setStrokeFill (colour);
    drawablePath.setStrokeThickness (15);
    drawablePath.setPath (p);

    return drawablePath;
}

static inline juce::DrawablePath createPowerIcon (juce::Colour colour)
{
    juce::Path p;
    p.addArc (0.0f,
              20.0f,
              100.0f,
              100.0f,
              0.18f * juce::MathConstants< float >::pi,
              2.0f * juce::MathConstants< float >::pi
                  - (0.18f * juce::MathConstants< float >::pi),
              true);
    p.startNewSubPath (50.0f, 0.0f);
    p.lineTo (50.0f, 70.0f);

    juce::DrawablePath drawablePath;
    drawablePath.setFill (juce::Colours::white.withAlpha (0.0f));
    drawablePath.setStrokeFill (colour);
    drawablePath.setStrokeThickness (10.0f);
    drawablePath.setPath (p);

    return drawablePath;
}

static inline juce::DrawablePath createBypassIcon (juce::Colour colour)
{
    juce::Path p;
    p.startNewSubPath (50.0f, 0.0f);
    p.lineTo (50.0f, 30.0f);
    p.lineTo (80.0f, 70.0f);
    p.startNewSubPath (50.0f, 70.0f);
    p.lineTo (50.0f, 100.0f);

    juce::DrawablePath drawablePath;
    drawablePath.setFill (juce::Colours::white.withAlpha (0.0f));
    drawablePath.setStrokeFill (colour);
    drawablePath.setStrokeThickness (10);
    drawablePath.setPath (p);

    return drawablePath;
}

static inline juce::DrawablePath createInfoIcon (juce::Colour colour)
{
    juce::Path circlePath;
    circlePath.addEllipse (0.0f, 0.0f, 100.0f, 100.0f);

    juce::DrawablePath circleImage;
    circleImage.setFill (colour);
    circleImage.setPath (circlePath);

    return circleImage;
}

static inline juce::DrawablePath createLoopIcon (juce::Colour colour)
{
    juce::Path loopPath;
    loopPath.addRoundedRectangle (0.0f, 0.0f, 150.0f, 50.0f, 25.0f);
    loopPath.addTriangle (100.0f, -10.0f, 100.0f, 10.0f, 110.0f, 0.0f);
    loopPath.addTriangle (40.0f, 50.0f, 50.0f, 60.0f, 50.0f, 40.0f);

    juce::DrawablePath loopImage;
    loopImage.setFill (juce::Colours::white.withAlpha (0.0f));
    loopImage.setStrokeFill (colour);
    loopImage.setStrokeThickness (15.0f);
    loopImage.setPath (loopPath);

    return loopImage;
}

static inline juce::DrawablePath createSpeakerIcon (juce::Colour colour)
{
    juce::Path p;
    p.addRoundedRectangle (0.0f, 33.0f, 33.0f, 33.0f, 2.0f);
    p.addTriangle (7.5f, 50.0f, 55.0f, 6.5f, 55.0f, 93.5f);
    p.addArc (60.0f,
              30.0f,
              12.0f,
              40.0f,
              juce::MathConstants< float >::pi * 0.15f,
              juce::MathConstants< float >::pi * 0.85f,
              true);
    p.addArc (70.0f,
              20.0f,
              16.0f,
              60.0f,
              juce::MathConstants< float >::pi,
              juce::MathConstants< float >::pi * 0.85f,
              true);
    p.addArc (80.0f,
              10.0f,
              20.0f,
              80.0f,
              juce::MathConstants< float >::pi * 0.15f,
              juce::MathConstants< float >::pi * 0.85f,
              true);

    juce::DrawablePath dp;
    dp.setFill (colour);
    dp.setStrokeFill (colour);
    dp.setStrokeThickness (5.0f);
    dp.setPath (p);

    return dp;
}

static inline juce::DrawablePath createMutedSpeakerIcon (juce::Colour colour)
{
    juce::Path p;
    p.addRoundedRectangle (0.0f, 33.0f, 33.0f, 33.0f, 2.0f);
    p.addTriangle (7.5f, 50.0f, 55.0f, 6.5f, 55.0f, 93.5f);
    p.scaleToFit (0.0f, 0.0f, 100.0f, 100.0f, true);

    juce::DrawablePath dp;
    dp.setFill (colour);
    dp.setStrokeFill (colour);
    dp.setStrokeThickness (5.0f);
    dp.setPath (p);

    return dp;
}


juce::DrawablePath createIcon (IconType icon, juce::Colour colour)
{
    switch (icon)
    {
        case Stop : return createStopIcon (colour);
        case Play : return createPlayIcon (colour);
        case Pause : return createPauseIcon (colour);
        case Next : return createNextIcon (colour);
        case Previous : return createPreviousIcon (colour);
        case ShuffleForward : return createShuffleForwardIcon (colour);
        case ShuffleBack : return createShuffleBackIcon (colour);
        case Eject : return createEjectIcon (colour);
        case Add : return createAddIcon (colour);
        case Search : return createSearchIcon (colour);
        case Power : return createPowerIcon (colour);
        case Bypass : return createBypassIcon (colour);
        case Info : return createInfoIcon (colour);
        case Loop : return createLoopIcon (colour);
        case Speaker : return createSpeakerIcon (colour);
        case MutedSpeaker : return createMutedSpeakerIcon (colour);
        default : return {};
    }
}

}  // namespace bav::gui
