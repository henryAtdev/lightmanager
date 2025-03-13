#include "LightRequestRegistry.h"

void LightRequestRegistry::fireStateChanged()
{
    for (LightRequestListener* listener: registry_){
        listener->stateChanged();
    }
}

void LightRequestRegistry::fireLoopChanged(uint64_t timeDiff)
{
    for (LightRequestListener* listener: registry_){
        listener->loopChanged(timeDiff);
    }
}
