#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"


class FMODApplication  : public JUCEApplication
{
public:
    FMODApplication() {}

    const String getApplicationName()       { return ProjectInfo::projectName; }
    const String getApplicationVersion()    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed()       { return true; }

    void initialise (const String&)
    {
        mainWindow = new MainWindow();
    }

    void shutdown()
    {
        mainWindow = nullptr;
    }

    void systemRequestedQuit()
    {
        quit();
    }

    void anotherInstanceStarted (const String&) { }

    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow()
        :   DocumentWindow ("MainWindow",
                            Colours::lightgrey,
                            DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainContentComponent(), true);

            setBounds (30, 30, getWidth(), getHeight());
            setVisible (true);
            setResizable (true, false);
        }

        void closeButtonPressed()
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    ScopedPointer<MainWindow> mainWindow;
};

START_JUCE_APPLICATION (FMODApplication)
