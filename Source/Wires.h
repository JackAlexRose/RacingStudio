//
//  Wires.h
//  racing
//
//  Created by Jack Rose on 05/03/2019.
//

#ifndef Wires_h
#define Wires_h

#include "MainComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../../../common/FMODHeaders.h"

class Wires
{
public:
    
    //setup wire events and start events
    void setup(Studio::System* system)
    {
        ERRCHECK(system->getEvent("event:/environment/electric-wires", &eventDescription));
        
            ERRCHECK (eventDescription->createInstance(&wires));
            ERRCHECK(wires->start());
        }
    
    //Set vectors of wire objects
    void vectorPlace(String const& param, const Vector3* vector)
    {
        wires->get3DAttributes(&attr3d);
        
        if( param == "pos" ) attr3d.position = *vector;
        else if( param == "dir" ) attr3d.forward = *vector;
        else if( param == "up" ) attr3d.up = *vector;
        else if( param == "vel" ) attr3d.velocity = *vector;
        
        wires->set3DAttributes(&attr3d);
    }
private:
    Studio::EventInstance* wires;
    
    FMOD_3D_ATTRIBUTES attr3d;
    
    Studio::EventDescription* eventDescription;
};

#endif /* Wires_h */
