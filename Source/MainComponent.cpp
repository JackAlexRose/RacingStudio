#include "MainComponent.h"


MainContentComponent::MainContentComponent()
{
    
    FMOD_Debug_Initialize (FMOD_DEBUG_LEVEL_NONE, FMOD_DEBUG_MODE_TTY, nullptr, nullptr);
    
    //add debug menu as a JUCE component
    addAndMakeVisible(debugTools);
    
    //set size of JUCE window
    setSize (600, 350);
    
    //launch the other application
    launchGame();
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::initFMODStudio()
{
    //Find the resources path
    File resourcesPath = getResourcesPath();
    
    //Get banks
    File bankPath = resourcesPath.getChildFile("Master Bank.bank");
    File stringsPath = resourcesPath.getChildFile("Master Bank.strings.bank");

    //Open FMOD system
    ERRCHECK (Studio::System::create (&system));
    ERRCHECK (system->initialize (64,
                                  FMOD_STUDIO_INIT_LIVEUPDATE | FMOD_STUDIO_INIT_ALLOW_MISSING_PLUGINS,
                                  FMOD_INIT_PROFILE_ENABLE, 0));
    
    //Loading the bank into the system
    Studio::Bank* bank;
    ERRCHECK (system->loadBankFile (bankPath.getFullPathName().toUTF8(),
                                    FMOD_STUDIO_LOAD_BANK_NORMAL,
                                    &bank));
    
    //Loading the strings bank
    Studio::Bank* stringsBank;
    ERRCHECK (system->loadBankFile (stringsPath.getFullPathName().toUTF8(),
                                    FMOD_STUDIO_LOAD_BANK_NORMAL,
                                    &stringsBank));
    
    //initialise car crash cooldown
    carCrashCooldown[0] = 0;
    carCrashCooldown[1] = 0;
}

void MainContentComponent::shutdownFMODStudio()
{
    //release the system on exit
    if (system != nullptr)
        ERRCHECK (system->release());
}

void MainContentComponent::resized()
{
    //resize the component to fit the window
    Rectangle<int> mainComponent = getBounds();
    debugTools.setBounds(mainComponent);
}

void MainContentComponent::handleConnect()
{
    initFMODStudio();
    
    //send the system to the debug menu so that the VCAs can be mapped
    debugTools.setVCAs( system );
}

void MainContentComponent::handleDisconnect()
{
    //shutdown the FMOD Studio connection
    shutdownFMODStudio();
    JUCEApplication::getInstance()->systemRequestedQuit();
}

void MainContentComponent::tick()
{
    //update the FMOD system every tick
    if (system != nullptr) {
        if (system->isValid()) ERRCHECK (system->update());
    }
    
    //decrement car crash cooldown if there is one
    if (carCrashCooldown[0] > 0) carCrashCooldown[0]--;
    if (carCrashCooldown[1] > 0) carCrashCooldown[1]--;
}

void MainContentComponent::handleCreate (String const& name, int gameObjectInstanceID)
{
    //log the creation of objects
    debugTools.createLogging(name, gameObjectInstanceID);
    
    //car creating, environment also gets created when car1 gets created
    if (name.contains("car")) {
        float var = 0;
        if( name == "car" ) {
            carSelector = 0;
            var = 0;
            environment.setup(system);
        }
        if( name == "car2" ) {
            carSelector = 1;
            var = 0.54;
        }
        cars[carSelector].setup(system, var);
    }
    
    //creating other elements
    else if(gameObjectInstanceID == 2672) wires[0].setup(system);
    else if(gameObjectInstanceID == 2674) wires[1].setup(system);
    else if(gameObjectInstanceID == 2694) wires[2].setup(system);
    else reverbs.setup(system, gameObjectInstanceID);
}

void MainContentComponent::handleDestroy (String const& name, int gameObjectInstanceID)
{
    
}


void MainContentComponent::handleVector (String const& name, int gameObjectInstanceID, String const& param, const Vector3* vector)
{
    //log incoming vector messages
    debugTools.vectorLogging(name, gameObjectInstanceID, param, vector);
    
    //set vector position of all events including the listener
    if (name == "camera")
    {
        system->getListenerAttributes(FMOD_MAIN_LISTENER, &attr3d);
        
        if( param == "pos" ) attr3d.position = *vector;
        else if( param == "dir" ) attr3d.forward = *vector;
        else if( param == "up" ) attr3d.up = *vector;
        else if( param == "vel" ) attr3d.velocity = *vector;
        
        system->setListenerAttributes(FMOD_MAIN_LISTENER, &attr3d);
    }
    else if (name.contains("car"))
    {
        if (name == "car") carSelector = 0;
        if (name == "car2") carSelector = 1;
        cars[carSelector].vectorPlace(param, vector);
    }
    else if (name == "wires")
    {
        if(gameObjectInstanceID == 2672) {
            wires[0].vectorPlace(param, vector);
            environment.vectorPlace(param, vector);
        }
        else if(gameObjectInstanceID == 2674) wires[1].vectorPlace(param, vector);
        else if(gameObjectInstanceID == 2694) wires[2].vectorPlace(param, vector);
    }
    else reverbs.vectorPlace(gameObjectInstanceID, param, vector);
}

void MainContentComponent::handleHit (String const& name, int gameObjectInstanceID, Collision const& collision)
{
    //if the crash is above a threshold, trigger crash and log the collision
    if (collision.velocity > 0.2)
    {
        debugTools.collisionLogging(name, gameObjectInstanceID, collision);
        
        String const& param = "crash";
        
        if ( name.contains("car")) {
            if (name == "car") carSelector = 0;
            else if (name == "car2") carSelector = 1;
            
            if (carCrashCooldown[carSelector] == 0) {
                cars[carSelector].paramChange(param, collision.velocity);
                carCrashCooldown[carSelector] = 10;
            }
        }
    }
}

void MainContentComponent::handleBool (String const& name, int gameObjectInstanceID, String const& param, bool flag)
{

}

void MainContentComponent::handleInt (String const& name, int gameObjectInstanceID, String const& param, int value)
{
    //log the parameter changes
    debugTools.parameterLogging(name, gameObjectInstanceID, param, value);
    
    //send parameter to relevant car object
    if (name == "car") carSelector = 0;
    else if (name == "car2") carSelector = 1;
    
    cars[carSelector].paramChange(param, value);
}

void MainContentComponent::handleReal (String const& name, int gameObjectInstanceID, String const& param, double value)
{
    //log the parameter changes
    debugTools.parameterLogging(name, gameObjectInstanceID, param, value);
    
    //send parameter to relevant car object
    if (name == "car") carSelector = 0;
    else if (name == "car2") carSelector = 1;
    
    cars[carSelector].paramChange(param, value);
    
    //send tyre speed values to tremolo
    if (param == "speed") {
        cars[carSelector].paramChange("tremoloDepth", debugTools.tyresTremoloOn);
        cars[carSelector].paramChange("tremoloFrequency", (value / 250) * 20);
    }
}

void MainContentComponent::handleString (String const& name, int gameObjectInstanceID, String const& param, String const& content)
{
   
}

void MainContentComponent::handleOther (String const& name, String const& t, String const& value)
{
    
}
