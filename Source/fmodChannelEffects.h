//
//  fmodChannelEffects.h
//  fmodChannelEffects
//
//  Philip Phelps 08 Aug 2018
//
//

#ifndef fmodChannelEffects_h
#define fmodChannelEffects_h

#include "../../../common/FMODHeaders.h"

class DSPUtils {

public:
    
    void addTremoloDSP(Studio::System* system, Studio::EventInstance* evtInstance )
    {
        // get access to the low level channel group for the event
        // channelgroups aredis part of the low level api, NOT the studio api
        FMOD::ChannelGroup* channelGroup;
        
        // calling getChannelGroup() may fail if our event is still being set up
        // so we first call system->flushCommands()
        ERRCHECK ( system->flushCommands() );
        ERRCHECK ( evtInstance->getChannelGroup(&channelGroup) );
        
        // also get access to the low level FMOD system (below the studio system)
        FMOD::System* lowLevelSystem;
        ERRCHECK( system->getLowLevelSystem(&lowLevelSystem) );
        
        // create a DSP unit for a tremolo
        FMOD_DSP_TYPE tremoloType = FMOD_DSP_TYPE_TREMOLO;
        ERRCHECK( lowLevelSystem->createDSPByType(tremoloType, &tremoloDSP) );
        
        // find out how many dsp units are in our channel group
        int numDSPUnits;
        ERRCHECK ( channelGroup->getNumDSPs(&numDSPUnits) );
        
        // add the flanger DSP at the index just before the "end" of the signal chain
        ERRCHECK( channelGroup->addDSP(numDSPUnits - 1, tremoloDSP) );
    }
    
    void changeDSPParams(int paramNumber, float value)
    {
        //0 is freq, 1 is depth
        tremoloDSP->setParameterFloat(paramNumber, value);
    }

private:
    FMOD::DSP* tremoloDSP;
};




#endif /* fmodChannelEffects_h */
