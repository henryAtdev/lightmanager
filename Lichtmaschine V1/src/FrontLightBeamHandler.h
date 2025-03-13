#ifndef  FRONT_LIGHT_BEAM_HANDLER_H
#define FRONT_LIGHT_BEAM_HANDLER_H
#include "LightRequester.h"
#include "light_control.h" //zuletzt hinzugefügt, evt falsch
#include <set>

enum Side{
    SideLeft = 0,
    SideRight = 1
};

enum Position{
    PositionInner = 0,
    PositionOuter = 1
};

class Led{
    public:
        Led(Side dir, Position pos);
        void setBrightness(uint8_t brightness);
        uint8_t getBrightness();
        Position getPosition();
        Side getSide();
    private:
        Side dir_;
        Position pos_;
        uint8_t brightness_;
};
class FrontLightBeamHandler{
    public:
        std::set<Led*> leds_;   
        FrontLightBeamHandler(LightRequestCollector& collector) : ourLightRequestCollector{collector} {}
    private:
        LightRequestCollector& ourLightRequestCollector;
};

class FixedFrontLightHandler : FrontLightBeamHandler{
    public:
        void compute(){
            setAdaptiveLowBeamLight(OpsOff);
            setLowBeamLight(OpsOn);
        }
};

class AdaptiveFrontLightHandler : FrontLightBeamHandler{
    public:
        void compute(){
            setLowBeamLight(OpsOff);
            setAdaptiveLowBeamLight(OpsOn);
        }
};



#endif