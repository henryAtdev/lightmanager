#ifndef _FogLightHandler_H
#define _FogLightHandler_H
#include <stdlib.h>
#include "LightRequester.h"
// #include "LightControlManeger.h"
#include "LightHandler.h"


class FogLightHandler : public LightHandler{

public:
    FogLightHandler (LightRequestCollector& collector) : LightHandler(collector){}
    void loopChanged(const uint64_t);
    void compute();
};

#endif