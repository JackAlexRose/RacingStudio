//
//  Reverbs.h
//  racing
//
//  Created by Jack Rose on 05/03/2019.
//

#ifndef Reverbs_h
#define Reverbs_h

#include "MainComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../../../common/FMODHeaders.h"

class Reverbs
{
public:
    
    //setup reverb events and start events
    void setup(Studio::System* system, int gameObjectInstanceID)
    {
        if (gameObjectInstanceID == 2508) {
            ERRCHECK (system->getEvent ("event:/reverbs/overbridge", &eventDescription));
            ERRCHECK (eventDescription->createInstance(&overbridge));
            ERRCHECK(overbridge->start());
        }
        else if (gameObjectInstanceID == 3156) {
            ERRCHECK (system->getEvent ("event:/reverbs/missioncontrol", &eventDescription));
            ERRCHECK (eventDescription->createInstance(&missioncontrol));
            ERRCHECK(missioncontrol->start());
        }
        else if (gameObjectInstanceID == 2768) {
            ERRCHECK (system->getEvent ("event:/reverbs/tunnel", &eventDescription));
            ERRCHECK (eventDescription->createInstance(&tunnel));
            ERRCHECK(tunnel->start());
        }
        else if (gameObjectInstanceID == 2930) {
            ERRCHECK (system->getEvent ("event:/reverbs/underbridge", &eventDescription));
            ERRCHECK (eventDescription->createInstance(&underbridge));
            ERRCHECK(underbridge->start());
        }
    }
    
    //Set vectors of reverb objects
    void vectorPlace(int gameObjectInstanceID, String const& param, const Vector3* vector)
    {
        if (gameObjectInstanceID == 2508) overbridge->get3DAttributes(&attr3d);
        else if (gameObjectInstanceID == 3156) missioncontrol->get3DAttributes(&attr3d);
        else if (gameObjectInstanceID == 2768) tunnel->get3DAttributes(&attr3d);
        else if (gameObjectInstanceID == 2930) underbridge->get3DAttributes(&attr3d);
        
        if( param == "pos" ) attr3d.position = *vector;
        else if( param == "dir" ) attr3d.forward = *vector;
        else if( param == "up" ) attr3d.up = *vector;
        else if( param == "vel" ) attr3d.velocity = *vector;
        
        else if (gameObjectInstanceID == 2508) overbridge->set3DAttributes(&attr3d);
        else if (gameObjectInstanceID == 3156) missioncontrol->set3DAttributes(&attr3d);
        else if (gameObjectInstanceID == 2768) tunnel->set3DAttributes(&attr3d);
        else if (gameObjectInstanceID == 2930) underbridge->set3DAttributes(&attr3d);
    }
private:
    Studio::EventInstance* missioncontrol;
    Studio::EventInstance* overbridge;
    Studio::EventInstance* tunnel;
    Studio::EventInstance* underbridge;
    
    FMOD_3D_ATTRIBUTES attr3d;
    
    Studio::EventDescription* eventDescription;
};

#endif /* Reverbs_h */
