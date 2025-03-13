#include "FogLightHandler.h"
#include "light_control.h"

void FogLightHandler::loopChanged(const uint64_t)
{
}

void FogLightHandler::compute()
{
        if (lightRequestCollector_.isFogFrontOn() == false){
            setFogFrontLight(OpsOff);
            setFogFrontPanel(OpsOff);
        }
        else{
            setFogFrontLight(OpsOn);
            setFogFrontPanel(OpsOn);
        }
        if (lightRequestCollector_.isFogRearOn() == false){
            setFogRearLight(OpsOff);
            setFogRearPanel(OpsOff);
        }
        else{
            setFogRearLight(OpsOn);
            setFogRearPanel(OpsOn);
        }
    }
