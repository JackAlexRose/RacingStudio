//
//  DebugTools.h
//  racing
//
//  Created by Jack Rose on 05/03/2019.
//

#ifndef DebugTools_h
#define DebugTools_h

#include "MainComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../../../common/FMODHeaders.h"

using namespace FMOD::Studio;

class DebugTools : public Component, SliderListener, ButtonListener
{
public:
    
    //Tremolo toggle, required in other classes
    bool tyresTremoloOn = true;
    
    DebugTools()
    {
        //create sliders for VCAs
        addAndMakeVisible (&carVolumeSlider);
        carVolumeSlider.setRange (0.0, 1.0, 0.01);
        carVolumeSlider.addListener (this);
        addAndMakeVisible (carVolumeLabel);
        carVolumeLabel.setText ("Car VCA", dontSendNotification);
        carVolumeLabel.attachToComponent (&carVolumeSlider, true);
        
        addAndMakeVisible (&engineVolumeSlider);
        engineVolumeSlider.setRange (0.0, 1.0, 0.01);
        engineVolumeSlider.addListener (this);
        addAndMakeVisible (engineVolumeLabel);
        engineVolumeLabel.setText ("Engine Bus", dontSendNotification);
        engineVolumeLabel.attachToComponent (&engineVolumeSlider, true);
        
        addAndMakeVisible (&crashVolumeSlider);
        crashVolumeSlider.setRange (0.0, 1.0, 0.01);
        crashVolumeSlider.addListener (this);
        addAndMakeVisible (crashVolumeLabel);
        crashVolumeLabel.setText ("Crash Bus", dontSendNotification);
        crashVolumeLabel.attachToComponent (&crashVolumeSlider, true);
        
        addAndMakeVisible (&gearVolumeSlider);
        gearVolumeSlider.setRange (0.0, 2, 0.01);
        gearVolumeSlider.addListener (this);
        addAndMakeVisible (gearVolumeLabel);
        gearVolumeLabel.setText ("Gear Bus", dontSendNotification);
        gearVolumeLabel.attachToComponent (&gearVolumeSlider, true);
        
        addAndMakeVisible (&skidVolumeSlider);
        skidVolumeSlider.setRange (0.0, 1.0, 0.01);
        skidVolumeSlider.addListener (this);
        addAndMakeVisible (skidVolumeLabel);
        skidVolumeLabel.setText ("Skid Bus", dontSendNotification);
        skidVolumeLabel.attachToComponent (&skidVolumeSlider, true);
        
        addAndMakeVisible (&tyresVolumeSlider);
        tyresVolumeSlider.setRange (0.0, 3, 0.01);
        tyresVolumeSlider.addListener (this);
        addAndMakeVisible (tyresVolumeLabel);
        tyresVolumeLabel.setText ("Tyres Bus", dontSendNotification);
        tyresVolumeLabel.attachToComponent (&tyresVolumeSlider, true);
        
        addAndMakeVisible (&environmentVolumeSlider);
        environmentVolumeSlider.setRange (0.0, 1.0, 0.01);
        environmentVolumeSlider.addListener (this);
        addAndMakeVisible (environmentVolumeLabel);
        environmentVolumeLabel.setText ("Environment VCA", dontSendNotification);
        environmentVolumeLabel.attachToComponent (&environmentVolumeSlider, true);
        
        //set initial values for all sliders
        carVolumeSlider.setValue(1, dontSendNotification);
        environmentVolumeSlider.setValue(1, dontSendNotification);
        engineVolumeSlider.setValue(0.5, dontSendNotification);
        gearVolumeSlider.setValue(2, dontSendNotification);
        tyresVolumeSlider.setValue(2, dontSendNotification);
        crashVolumeSlider.setValue(0.6, dontSendNotification);
        skidVolumeSlider.setValue(1, dontSendNotification);
        
        //create buttons for logging enabling/disabling
        vectorLoggingButton.setButtonText ("Vector Logging");
        addAndMakeVisible (&vectorLoggingButton);
        vectorLoggingButton.addListener (this);
        
        parameterLoggingButton.setButtonText ("Parameter Logging");
        addAndMakeVisible (&parameterLoggingButton);
        parameterLoggingButton.addListener (this);
        
        collisionLoggingButton.setButtonText ("Collision Logging");
        addAndMakeVisible (&collisionLoggingButton);
        collisionLoggingButton.addListener (this);
        
        tyresTremoloButton.setButtonText ("Tyres Tremolo");
        addAndMakeVisible (&tyresTremoloButton);
        tyresTremoloButton.addListener (this);
        
        //set initial value for tremolo toggle
        tyresTremoloButton.setToggleState(1, dontSendNotification);
    }
    
    void setVCAs( Studio::System* newSystem)
    {
        //receive system and get VCA and bus references, set initial values
        system = newSystem;
        system->getVCA("vca:/Car", &carVCA);
        system->getVCA("vca:/Environment", &environmentVCA);
        
        system->getBus("bus:/gear", &gearBus);
        gearBus->setVolume(2);
        
        system->getBus("bus:/engine", &engineBus);
        engineBus->setVolume(0.5);
        
        system->getBus("bus:/tyres", &tyreBus);
        tyreBus->setVolume(2);
        
        system->getBus("bus:/crash", &crashBus);
        crashBus->setVolume(0.6);
        
        system->getBus("bus:/skid", &skidBus);
    }
    
    void resized() override
    {
        //set sizes of items on debug menu
        carVolumeSlider.setBounds (100, 20, getWidth()-110, 20);
        environmentVolumeSlider.setBounds(100, 200, getWidth()-110, 20);
        engineVolumeSlider.setBounds(100, 50, getWidth()-110, 20);
        crashVolumeSlider.setBounds(100, 80, getWidth()-110, 20);
        gearVolumeSlider.setBounds(100, 110, getWidth()-110, 20);
        skidVolumeSlider.setBounds(100, 140, getWidth()-110, 20);
        tyresVolumeSlider.setBounds(100, 170, getWidth()-110, 20);
        vectorLoggingButton.setBounds (10, 230, getWidth()-20, 20);
        parameterLoggingButton.setBounds (10, 260, getWidth()-20, 20);
        collisionLoggingButton.setBounds (10, 290, getWidth()-20, 20);
        tyresTremoloButton.setBounds (10, 320, getWidth()-20, 20);
    }
    
    void sliderValueChanged(Slider* slider) override
    {
        //if the system is loaded, listen for slider values and apply to buses and VCAs
        if (system != nullptr) {
            if (&carVolumeSlider == slider) carVCA->setVolume((float)carVolumeSlider.getValue());
            if (&engineVolumeSlider == slider) engineBus->setVolume((float)engineVolumeSlider.getValue());
            if (&crashVolumeSlider == slider) crashBus->setVolume((float)crashVolumeSlider.getValue());
            if (&gearVolumeSlider == slider) gearBus->setVolume((float)gearVolumeSlider.getValue());
            if (&skidVolumeSlider == slider) skidBus->setVolume((float)skidVolumeSlider.getValue());
            if (&tyresVolumeSlider == slider) tyreBus->setVolume((float)tyresVolumeSlider.getValue());
            if (&environmentVolumeSlider == slider) environmentVCA->setVolume((float)environmentVolumeSlider.getValue());
            }
    }
    
    void buttonClicked(Button* button) override
    {
        //toggle logging and tremolo when buttons are clicked
        if(button == &vectorLoggingButton) vectorLoggingOn = vectorLoggingButton.getToggleState();
        if (&parameterLoggingButton == button) parameterLoggingOn = parameterLoggingButton.getToggleState();
        if (&collisionLoggingButton == button) collisionLoggingOn = collisionLoggingButton.getToggleState();
        if (&tyresTremoloButton == button) tyresTremoloOn = tyresTremoloButton.getToggleState();

    }
    
    void vectorLogging (String const& name, int gameObjectInstanceID, String const& param, const Vector3* vector)
    {
        //log vector messages in the console if enabled
        if(vectorLoggingOn)
        {
            Logger::outputDebugString (String ("Vector: ")
                                       + name
                                       + String (" [id = ")
                                       + String (gameObjectInstanceID)
                                       + String ("]")
                                       + String (" param: ")
                                       + param);
        }
    }
    
    void parameterLogging (String const& name, int gameObjectInstanceID, String const& param, double value)
    {
        //log parameter messages in the console if enabled
        if(parameterLoggingOn)
        {
            Logger::outputDebugString (String ("Parameter: ")
                                       + name
                                       + String (" [id = ")
                                       + String (gameObjectInstanceID)
                                       + String ("]")
                                       + String (" param: ")
                                       + param
                                       + String (" value = ")
                                       + String (value));
        }
    }
    void collisionLogging (String const& name, int gameObjectInstanceID, Collision const& collision)
    {
        //log collision messages in the console if enabled
        if(collisionLoggingOn)
        {
            Logger::outputDebugString (String ("Collision: ")
                                       + name
                                       + String (" [id = ")
                                       + String (gameObjectInstanceID)
                                       + String ("]")
                                       + String (" other: ")
                                       + collision.otherName
                                       + String (" velocity = ")
                                       + String (collision.velocity));
        }
    }
    
    void createLogging (String const& name, int gameObjectInstanceID)
    {
        //log handle create messages in the console
        Logger::outputDebugString (String ("Created: ")
                                   + name
                                   + String(" [id = ")
                                   + String(gameObjectInstanceID)
                                   + String ("]"));
    }

    
private:

    Studio::System* system;
    
    //JUCE Objects
    Slider carVolumeSlider;
    Label carVolumeLabel;
    Slider environmentVolumeSlider;
    Label environmentVolumeLabel;
    Slider engineVolumeSlider;
    Label engineVolumeLabel;
    Slider gearVolumeSlider;
    Label gearVolumeLabel;
    Slider crashVolumeSlider;
    Label crashVolumeLabel;
    Slider skidVolumeSlider;
    Label skidVolumeLabel;
    Slider tyresVolumeSlider;
    Label tyresVolumeLabel;
    ToggleButton vectorLoggingButton;
    ToggleButton parameterLoggingButton;
    ToggleButton collisionLoggingButton;
    ToggleButton tyresTremoloButton;
    
    //FMOD Objects
    VCA* carVCA;
    VCA* environmentVCA;
    Bus* gearBus;
    Bus* engineBus;
    Bus* tyreBus;
    Bus* crashBus;
    Bus* skidBus;
    
    //Toggle bools for enabling logging
    bool vectorLoggingOn = false;
    bool parameterLoggingOn = false;
    bool collisionLoggingOn = false;
};
#endif /* DebugTools_h */
