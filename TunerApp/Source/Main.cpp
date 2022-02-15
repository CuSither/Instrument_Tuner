#include <JuceHeader.h>
#include "PitchDetectionComponent.h"
#include <iostream>

//==============================================================================
class TunerAppApplication  : public juce::JUCEApplication {
 public:
    //==============================================================================
    TunerAppApplication() {}

    const juce::String getApplicationName() override {
        return ProjectInfo::projectName;
    }
    const juce::String getApplicationVersion() override {
        return ProjectInfo::versionString;
    }
    bool moreThanOneInstanceAllowed() override {
        return true;
    }

    //==============================================================================
    void initialise(const juce::String& commandLine) override {
        mainWindow.reset (new MainWindow ("Tuner",
                                          new PitchDetectionComponent));
    }

    void shutdown() override {
        mainWindow = nullptr;   // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override {
        quit();
    }

    void anotherInstanceStarted(const juce::String& commandLine) override {
        // When another instance of the app is launched while this one is
        // running, this method is invoked, and the commandLine parameter
        // tells you what the other instance's command-line arguments were.
    }

    class MainWindow    : public juce::DocumentWindow {
     public:
        MainWindow(juce::String name, juce::Component* c)
            : DocumentWindow(name,
                              juce::Desktop::getInstance().getDefaultLookAndFeel()
                                                          .findColour(juce::ResizableWindow::backgroundColourId),
                              DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(c, true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen(true);
           #else
            setResizable(true, true);
            setResizeLimits(300, 250, 10000, 10000);
            centreWithSize(getWidth(), getHeight());
           #endif

            setVisible(true);
        }

        void closeButtonPressed() override {
            // This is called when the user tries to close this window.
            // Here, we'll just ask the app to quit when this happens,
            // but you can change this to do whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

     private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

 private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(TunerAppApplication)
