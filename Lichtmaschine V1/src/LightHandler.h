#ifndef LIGHT_HANDLER_H
#define LIGHT_HANDLER_H
#include "LightRequester.h"

class LightHandler: public LightRequestListener{
protected: 
    LightRequestCollector &lightRequestCollector_; 
    virtual void compute() = 0;
public:
    LightHandler(LightRequestCollector &lightRequestCollector):lightRequestCollector_(lightRequestCollector){
        lightRequestCollector_.addLightRequestListener(this);
    }
    void stateChanged(){
        //compute(); ansonsten entsthen Querempfindlichkeiten zwischen Abblendlicht und Nebelleuchtenn
        }
    virtual void loopChanged(const uint64_t) = 0;
};

#endif