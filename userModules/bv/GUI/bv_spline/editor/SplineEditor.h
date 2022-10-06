#pragma once


namespace bav::spline
{
class Editor : public juce::Component
{
public:
    using Color = juce::Colour;

    juce::Rectangle< float > getAdjustedBounds() const;

    void setBorderColour (Color c);
    void setPointColour (Color c);
    void setCurveColour (Color c);
    void setSelectionColour (Color c);
    void setBackgroundColour (Color c);
    void setBorderThickness (float pxl);
    void setPointThickness (float pxl);
    void setCurveThickness (float pxl);
    void setSelectionThickness (float pxl);

private:
    struct Select
    {
        Select (Editor& e);

        void setX (float xP) noexcept;

        void addWidth (float w, float scrollSensitivity = 0.2f);

        void paint (juce::Graphics& g);

        juce::Range< float > getRange() const noexcept;

        void mouseExited();

    private:
        Editor& editor;
        float   x {-1.f}, width {.1f};
    };

    struct Attribute
    {
        Color color;
        float thickness {0.f};
    };

    struct Attributes
    {
        Attributes();

        float getThickestThickness() const;

        Attribute border, point, curve, selection, background;

    private:
        std::vector< Attribute* > pntrs;
    };

    void mouseWheelMove (const juce::MouseEvent& evt, const juce::MouseWheelDetails& wheel) final;
    void mouseMove (const juce::MouseEvent& evt) final;
    void mouseDown (const juce::MouseEvent& evt) final;
    void mouseDrag (const juce::MouseEvent& evt) final;
    void mouseUp (const juce::MouseEvent& evt) final;
    void mouseExit (const juce::MouseEvent&) final;

    void paint (juce::Graphics& g) final;
    void drawPoint (const Point& knot, const Attribute& attribute, juce::Graphics& g);

    void resized() final;

    void timerCallback();

    Point normalizePoint (const Point& position) const noexcept;

    void updateCurve();

    Spline spline;
    Select select {*this};
    float  addTolerance {0.1f};

    Attributes attributes;

    events::TimerCallback t {[&]
                             { timerCallback(); }};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};

}  // namespace bav::spline
