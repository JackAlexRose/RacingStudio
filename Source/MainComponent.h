#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../../common/FMODHeaders.h"
#include "Car.h"
#include "Environment.h"
#include "Wires.h"
#include "Reverbs.h"
#include "DebugTools.h"
#include "fmodChannelEffects.h"

using namespace FMOD::Studio;

class MainContentComponent  :   public Component,
                                public GameEngineServer

{
public:
    MainContentComponent();
    ~MainContentComponent();
	
    void initFMODStudio();
    void shutdownFMODStudio();
	
    void resized() override;
    
    void handleConnect() override;
    void handleDisconnect() override;
    void tick() override;
    
    void handleCreate (String const& name, int gameObjectInstanceID) override;
    void handleDestroy (String const& name, int gameObjectInstanceID) override;
    void handleVector (String const& name, int gameObjectInstanceID, String const& param, const Vector3* vector) override;
    void vectorPlace (String const& name, int gameObjectInstanceID, String const& param, const Vector3* vector);
    void handleHit (String const& name, int gameObjectInstanceID, Collision const& collision) override;
    void handleBool (String const& name, int gameObjectInstanceID, String const& param, bool flag) override;
    void handleInt (String const& name, int gameObjectInstanceID, String const& param, int value) override;
    void handleReal (String const& name, int gameObjectInstanceID, String const& param, double value) override;
    void handleString (String const& name, int gameObjectInstanceID, String const& param, String const& content) override;
    void handleOther (String const& name, String const& t, String const& value) override;
    
private:
    //initialise system
    Studio::System* system = nullptr;
    
    //car crash cooldown to prevent multiple crashes
    int carCrashCooldown[2];
    
    //FMOD vectors
    FMOD_3D_ATTRIBUTES attr3d;
    
    //references to classes required in the main component
    Car cars[2];
    Environment environment;
    Wires wires[3];
    Reverbs reverbs;
    DebugTools debugTools;
    
    //private variables required for selecting instances
    int carSelector;
    int wireSelector;
};

#endif  // MAINCOMPONENT_H_INCLUDED


