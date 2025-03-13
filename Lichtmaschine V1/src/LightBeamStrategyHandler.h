#ifndef _LIGHT_BEAM_STRATEGY_HANDLER_H
#define _LIGHT_BEAM_STRATEGY_HANDLER_H
#include "LightRequester.h"
#include "FrontLightBeamHandler.h"


class LightBeamStrategyHandler{
    public:
        LightBeamStrategyHandler(LightRequester& requester);
        void switchFrontLightStrategy();
    private:
        LightRequester &lightRequester_;
        FrontLightBeamHandler* frontLightHandler_[2];
        int frontLightStrategy = 0;
};



#endif