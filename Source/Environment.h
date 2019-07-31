//
//  Environment.h
//  racing
//
//  Created by Jack Rose on 05/03/2019.
//

#ifndef Environment_h
#define Environment_h
#include "MainComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../../../common/FMODHeaders.h"

class Environment
{
public:
    
    //setup environment events and start events
    void setup(Studio::System* system)
    {
        ERRCHECK (system->getEvent ("event:/environment/atmos", &eventDescription));
        ERRCHECK (eventDescription->createInstance(&atmos));
        ERRCHECK(atmos->start());
        ERRCHECK (system->getEvent ("event:/environment/crowd", &eventDescription));
        ERRCHECK (eventDescription->createInstance(&crowd));
        ERRCHECK(crowd->start());
    }
    
    //Set vectors of crowd object
    void vectorPlace(String const& param, const Vector3* vector)
    {
        crowd->get3DAttributes(&attr3d);
        
        if( param == "pos" ) attr3d.position = *vector;
        else if( param == "dir" ) attr3d.forward = *vector;
        else if( param == "up" ) attr3d.up = *vector;
        else if( param == "vel" ) attr3d.velocity = *vector;
        
        crowd->set3DAttributes(&attr3d);
    }
private:
    Studio::EventInstance* atmos;
    Studio::EventInstance* crowd;
    
    FMOD_3D_ATTRIBUTES attr3d;
    
    Studio::EventDescription* eventDescription;
};


#endif /* Environment_h */
