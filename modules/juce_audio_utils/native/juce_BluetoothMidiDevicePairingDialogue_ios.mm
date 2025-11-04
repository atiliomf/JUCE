/*
  ==============================================================================

   This file is part of the JUCE framework.
   Copyright (c) Raw Material Software Limited

   JUCE is an open source framework subject to commercial or open source
   licensing.

   By downloading, installing, or using the JUCE framework, or combining the
   JUCE framework with any other source code, object code, content or any other
   copyrightable work, you agree to the terms of the JUCE End User Licence
   Agreement, and all incorporated terms including the JUCE Privacy Policy and
   the JUCE Website Terms of Service, as applicable, which will bind you. If you
   do not agree to the terms of these agreements, we will not license the JUCE
   framework to you, and you must discontinue the installation or download
   process and cease use of the JUCE framework.

   JUCE End User Licence Agreement: https://juce.com/legal/juce-8-licence/
   JUCE Privacy Policy: https://juce.com/juce-privacy-policy
   JUCE Website Terms of Service: https://juce.com/juce-website-terms-of-service/

   Or:

   You may also use this code under the terms of the AGPLv3:
   https://www.gnu.org/licenses/agpl-3.0.en.html

   THE JUCE FRAMEWORK IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL
   WARRANTIES, WHETHER EXPRESSED OR IMPLIED, INCLUDING WARRANTY OF
   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

#if ! TARGET_IPHONE_SIMULATOR

namespace juce
{

//==============================================================================
class BluetoothMidiSelectorOverlay final : public Component
{
public:
    BluetoothMidiSelectorOverlay (ModalComponentManager::Callback* exitCallbackToUse,
                                  const Rectangle<int>& boundsToUse,
                                  bool advertiseAlsPeriferal)
        : bounds (boundsToUse)
    {
        std::unique_ptr<ModalComponentManager::Callback> exitCallback (exitCallbackToUse);

        update(); // sets bounds

        kioskModeComponent = Desktop::getInstance().getKioskModeComponent();

        if (kioskModeComponent != nullptr)
            kioskModeComponent->addChildComponent (this);
        else
            addToDesktop (ComponentPeer::windowHasDropShadow);

        toFront (true);
        setAlwaysOnTop (true);
        setVisible (true);

        usePeripheralController = advertiseAlsPeriferal;

        if (usePeripheralController)
        {
            peripheralController = [[CABTMIDILocalPeripheralViewController alloc] init];
            nativeSelectorComponent.setView ([peripheralController view]);
        }
        else
        {
            centralController = [[CABTMIDICentralViewController alloc] init];
            nativeSelectorComponent.setView ([centralController view]);
        }

        addAndMakeVisible (nativeSelectorComponent);

        enterModalState (true, exitCallback.release(), true);
    }

    ~BluetoothMidiSelectorOverlay() override
    {
        nativeSelectorComponent.setView (nullptr);
        
        if (usePeripheralController)
            [peripheralController release];
        else
            [centralController release];
    }

    void paint (Graphics& g) override
    {
//        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId).withAlpha (0.7f));
//        g.fillAll (Colours::black.withAlpha (0.25f));
    }

    void inputAttemptWhenModal() override           { close(); }
    void mouseDrag (const MouseEvent&) override     {}
    void mouseDown (const MouseEvent&) override     { close(); }
    void resized() override                         { close(); }
    void parentSizeChanged() override               { close(); }

private:
    void update()
    {
        if (bounds.isEmpty())
        {
            setBounds (0, 0, getParentWidth(), getParentHeight());
            
            const int pw = getParentWidth();
            const int ph = getParentHeight();

            setBounds (0, 0, getParentWidth(), getParentHeight());

            nativeSelectorComponent.setBounds (Rectangle<int> (pw, ph)
                                                 .withSizeKeepingCentre (jmin (400, pw),
                                                                         jmin (450, ph - 40)));
        }
        else
        {
            setBounds (bounds);
            nativeSelectorComponent.setBounds (bounds.withZeroOrigin());
        }
    }

    void close()
    {
        exitModalState (0);
        setVisible (false);
    }

    bool usePeripheralController = false;
    CABTMIDICentralViewController* centralController;
    CABTMIDILocalPeripheralViewController* peripheralController;
    UIViewComponent nativeSelectorComponent;
    Rectangle<int> bounds;
    Component* kioskModeComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BluetoothMidiSelectorOverlay)
};

bool BluetoothMidiDevicePairingDialogue::open (ModalComponentManager::Callback* exitCallback,
                                               Rectangle<int>* btBounds,
                                               bool advertiseAlsPeriferal)
{
    std::unique_ptr<ModalComponentManager::Callback> cb (exitCallback);
    auto boundsToUse = (btBounds != nullptr ? *btBounds : Rectangle<int> {});

    if (isAvailable())
    {
        new BluetoothMidiSelectorOverlay (cb.release(), boundsToUse, advertiseAlsPeriferal);
        return true;
    }

    return false;
}

bool BluetoothMidiDevicePairingDialogue::isAvailable()
{
    return NSClassFromString (@"CABTMIDICentralViewController") != nil;
}

} // namespace juce

//==============================================================================
#else

namespace juce
{
    bool BluetoothMidiDevicePairingDialogue::open (ModalComponentManager::Callback* exitCallback,
                                                   Rectangle<int>*)
    {
        std::unique_ptr<ModalComponentManager::Callback> cb (exitCallback);
        return false;
    }

    bool BluetoothMidiDevicePairingDialogue::isAvailable()  { return false; }
}

#endif
