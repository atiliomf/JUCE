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

#include "../jucer_Headers.h"
#include "../jucer_Application.h"

#include "jucer_StartPageComponent.h"
#include "jucer_StartPageTreeHolder.h"
#include "jucer_NewProjectTemplates.h"
#include "jucer_ContentComponents.h"

//==============================================================================
struct ContentComponent final : public Component
{
    ContentComponent()
    {
        setTitle ("Content");
        setFocusContainerType (FocusContainerType::focusContainer);
    }

    void resized() override
    {
        if (content != nullptr)
            content->setBounds (getLocalBounds());
    }

    void setContent (std::unique_ptr<Component>&& newContent)
    {
        if (content.get() != newContent.get())
        {
            content = std::move (newContent);
            addAndMakeVisible (content.get());
            resized();
        }
    }

private:
    std::unique_ptr<Component> content;

    //==============================================================================
    JUCE_LEAK_DETECTOR (ContentComponent)
};

//==============================================================================
static File findExampleFile (int dirIndex, int index)
{
    auto dir = ProjucerApplication::getSortedExampleDirectories()[dirIndex];
    return ProjucerApplication::getSortedExampleFilesInDirectory (dir)[index];
}

static std::unique_ptr<Component> createExampleProjectsTab (ContentComponent& content, std::function<void (const File&)> cb)
{
    StringArray exampleCategories;
    std::vector<StringArray> examples;

    for (auto& dir : ProjucerApplication::getSortedExampleDirectories())
    {
        exampleCategories.add (dir.getFileName());

        StringArray ex;
        for (auto& f : ProjucerApplication::getSortedExampleFilesInDirectory (dir))
            ex.add (f.getFileNameWithoutExtension());

        examples.push_back (ex);
    }

    if (exampleCategories.isEmpty())
        return nullptr;

    auto selectedCallback = [&, cb] (int category, int index) mutable
    {
        content.setContent (std::make_unique<ExampleComponent> (findExampleFile (category, index), cb));
    };

    return std::make_unique<StartPageTreeHolder> ("Examples",
                                                  exampleCategories,
                                                  examples,
                                                  std::move (selectedCallback),
                                                  StartPageTreeHolder::Open::no);
}

//==============================================================================
static StringArray getAllTemplateCategoryStrings()
{
    StringArray categories;

    for (auto& t : NewProjectTemplates::getAllTemplates())
        categories.addIfNotAlreadyThere (NewProjectTemplates::getProjectCategoryString (t.category));

    return categories;
}

static std::vector<NewProjectTemplates::ProjectTemplate> getTemplatesInCategory (const String& category)
{
    std::vector<NewProjectTemplates::ProjectTemplate> templates;

    for (auto& t : NewProjectTemplates::getAllTemplates())
        if (NewProjectTemplates::getProjectCategoryString (t.category) == category)
            templates.push_back (t);

    return templates;
}

static StringArray getAllTemplateNamesForCategory (const String& category)
{
    StringArray types;

    for (auto& t : getTemplatesInCategory (category))
        types.add (t.displayName);

    return types;
}

static std::unique_ptr<Component> createProjectTemplatesTab (ContentComponent& content,
                                                             std::function<void (std::unique_ptr<Project>&&)>&& cb)
{
    auto categories = getAllTemplateCategoryStrings();

    std::vector<StringArray> templateNames;

    for (auto& c : categories)
        templateNames.push_back (getAllTemplateNamesForCategory (c));

    auto selectedCallback = [&, cb] (int category, int index)
    {
        auto categoryString = getAllTemplateCategoryStrings()[category];
        auto templates = getTemplatesInCategory (categoryString);

        content.setContent (std::make_unique<TemplateComponent> (templates[(size_t) index], std::move (cb)));
    };

    auto holder = std::make_unique<StartPageTreeHolder> ("Templates",
                                                         categories,
                                                         templateNames,
                                                         std::move (selectedCallback),
                                                         StartPageTreeHolder::Open::yes);
    holder->setSelectedItem (categories[0], 1);

    JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wredundant-move")
    return std::move (holder);
    JUCE_END_IGNORE_WARNINGS_GCC_LIKE
}

//==============================================================================
struct ProjectTemplatesAndExamples final : public TabbedComponent
{
    ProjectTemplatesAndExamples (ContentComponent& c,
                                 std::function<void (std::unique_ptr<Project>&&)>&& newProjectCb,
                                 std::function<void (const File&)>&& exampleCb)
        : TabbedComponent (TabbedButtonBar::Orientation::TabsAtTop),
          content (c),
          exampleSelectedCallback (std::move (exampleCb))
    {
        setTitle ("Templates and Examples");
        setFocusContainerType (FocusContainerType::focusContainer);

        addTab ("New Project",
                Colours::transparentBlack,
                createProjectTemplatesTab (content, std::move (newProjectCb)).release(),
                true);

        refreshExamplesTab();
    }

    void refreshExamplesTab()
    {
        auto wasOpen = (getCurrentTabIndex() == 1);

        removeTab (1);

        auto exampleTabs = createExampleProjectsTab (content, exampleSelectedCallback);

        addTab ("Open Example",
                Colours::transparentBlack,
                exampleTabs == nullptr ? new SetJUCEPathComponent (*this) : exampleTabs.release(),
                true);

        if (wasOpen)
            setCurrentTabIndex (1);
    }

private:
    //==============================================================================
    struct SetJUCEPathComponent final : public Component,
                                        private ChangeListener
    {
        explicit SetJUCEPathComponent (ProjectTemplatesAndExamples& o)
            : owner (o)
        {
            getGlobalProperties().addChangeListener (this);

            setPathButton.setButtonText ("Set path to JUCE...");
            setPathButton.onClick = [] { ProjucerApplication::getApp().showPathsWindow (true); };

            addAndMakeVisible (setPathButton);
        }

        ~SetJUCEPathComponent() override
        {
            getGlobalProperties().removeChangeListener (this);
        }

        void paint (Graphics& g) override
        {
            g.fillAll (findColour (secondaryBackgroundColourId));
        }

        void resized() override
        {
            auto bounds = getLocalBounds().reduced (5);
            bounds.removeFromTop (25);

            setPathButton.setBounds (bounds.removeFromTop (25));
        }

    private:
        void changeListenerCallback (ChangeBroadcaster*) override
        {
            if (isValidJUCEExamplesDirectory (ProjucerApplication::getJUCEExamplesDirectoryPathFromGlobal()))
                owner.refreshExamplesTab();
        }

        ProjectTemplatesAndExamples& owner;
        TextButton setPathButton;
    };

    ContentComponent& content;
    std::function<void (const File&)> exampleSelectedCallback;
};

//==============================================================================
StartPageComponent::StartPageComponent (std::function<void (std::unique_ptr<Project>&&)>&& newProjectCb,
                                        std::function<void (const File&)>&& exampleCb)
    : content (std::make_unique<ContentComponent>()),
      tabs (std::make_unique<ProjectTemplatesAndExamples> (*content, std::move (newProjectCb), std::move (exampleCb)))
{
    tabs->setOutline (0);
    addAndMakeVisible (*tabs);

    addAndMakeVisible (openExistingButton);
    openExistingButton.setCommandToTrigger (&ProjucerApplication::getCommandManager(), CommandIDs::open, true);

    addAndMakeVisible (openMostRecentButton);
    openMostRecentButton.onClick = [&]()
    {
        ProjucerApplication::getApp().openFile (getAppSettings().recentFiles.getFile (0), nullptr);
    };
    
    addAndMakeVisible (*content);

    setSize (900, 600);
}

void StartPageComponent::paint (Graphics& g)
{
    g.fillAll (findColour (backgroundColourId));
}

void StartPageComponent::resized()
{
    auto bounds = getLocalBounds().reduced (10);

    auto tabBounds = bounds.removeFromLeft (bounds.getWidth() / 3);

    openExistingButton.setBounds (tabBounds.removeFromBottom (30).reduced (10, 0));
    tabBounds.removeFromBottom (5);
    
    openMostRecentButton.setBounds (tabBounds.removeFromBottom (30).reduced (10, 0));
    tabBounds.removeFromBottom (5);

    tabs->setBounds (tabBounds);
    bounds.removeFromLeft (10);

    content->setBounds (bounds);
}
