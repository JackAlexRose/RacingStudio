//
//  Car.h
//  racing
//
//  Created by Jack Rose on 24/02/2019.
//

#ifndef Car_h
#define Car_h
#include "MainComponent.h"
#include "fmodChannelEffects.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../../../common/FMODHeaders.h"

class Car
{
public:
    
    //setup the cars, create game instances and start them where required
    void setup(Studio::System* system, float var)
    {
        ERRCHECK (system->getEvent ("event:/car/engine", &eventDescription));
        ERRCHECK (eventDescription->createInstance(&engine));
        ERRCHECK(engine->start());
        engine->setParameterValue("variation", var);
        ERRCHECK (system->getEvent ("event:/car/crash", &eventDescription));
        ERRCHECK (eventDescription->createInstance(&crash));
        ERRCHECK (system->getEvent ("event:/car/gear", &eventDescription));
        ERRCHECK (eventDescription->createInstance(&gear));
        ERRCHECK (system->getEvent ("event:/car/skid", &eventDescription));
        ERRCHECK (eventDescription->createInstance(&skid));
        ERRCHECK(skid->start());
        ERRCHECK (system->getEvent ("event:/car/tyres", &eventDescription));
        ERRCHECK (eventDescription->createInstance(&tyres));
        ERRCHECK(tyres->start());
        
        //add the tremolo to the tyres
        util.addTremoloDSP(system, tyres);
    }
    
    //if a parameter changes, map to FMOD parameters
    void paramChange(String const& param, double value)
    {
        if (param == "gear") {
            gear->start();
            gear->setParameterValue("blowoff", value / 10 + 0.6);
        }
        else if (param == "rpm") engine->setParameterValue("rpm", value);
        else if (param == "torque") engine->setParameterValue("load", value / 2000);
        else if (param == "speed") tyres->setParameterValue("speed", value);
        else if (param == "suspension") tyres->setParameterValue("force", value * 1.5);
        else if (param == "skid")
        {
            if (value < 0.1) value = 0;
            skid->setParameterValue("amount", value);
        }
        else if(param == "crash") {
            if ( crash != nullptr) {
                ERRCHECK(crash->start());
                crash->setParameterValue("intensity", value);
            }
        }
        else if(param == "tremoloDepth") util.changeDSPParams(1, value * 41);
        else if(param == "tremoloFrequency") util.changeDSPParams(0, value);
    }
    
    //Set vectors of car objects
    void vectorPlace(String const& param, const Vector3* vector)
    {
        engine->get3DAttributes(&attr3d);
        crash->get3DAttributes(&attr3d);
        gear->get3DAttributes(&attr3d);
        tyres->get3DAttributes(&attr3d);
        skid->get3DAttributes(&attr3d);
        
        if( param == "pos" ) attr3d.position = *vector;
        else if( param == "dir" ) attr3d.forward = *vector;
        else if( param == "up" ) attr3d.up = *vector;
        else if( param == "vel" ) attr3d.velocity = *vector;
        
        engine->set3DAttributes(&attr3d);
        crash->set3DAttributes(&attr3d);
        gear->set3DAttributes(&attr3d);
        tyres->set3DAttributes(&attr3d);
        skid->set3DAttributes(&attr3d);
    }
private:
    Studio::EventInstance* engine;
    Studio::EventInstance* crash;
    Studio::EventInstance* gear;
    Studio::EventInstance* skid;
    Studio::EventInstance* tyres;
    
    FMOD_3D_ATTRIBUTES attr3d;
    
    Studio::EventDescription* eventDescription;
    DSPUtils util;
};


#endif /* Car_h */
