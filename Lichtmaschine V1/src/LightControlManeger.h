#ifndef _LIGHT_CONTROL_MANEGER
#define _LIGHT_CONTROL_MANEGER
#include "LightRequester.h"
#include "FogLightHandler.h"
#include "FrontLightBeamHandler.h"
#include "LightBeamStrategyHandler.h"

class LightControlManager {
public: 
  static LightControlManager* getInstance(){
    if (lightControlInstance == nullptr){
      lightControlInstance = new LightControlManager();
    }
    return lightControlInstance;
  }
  LightRequester& getLightRequester(){
    return lightRequester_; 
  }
  void switchFrontLightStrategie(){
    static bool act_adaptive = true;
    if (act_adaptive == false){
      setAdaptiveLowBeamLight(OpsOff);
      setLowBeamLight(OpsOn);
      act_adaptive = true;
    }
    else if (act_adaptive == true){
      setLowBeamLight(OpsOff);
      setAdaptiveLowBeamLight(OpsOn);
      act_adaptive = false;
      }

  };
  void testLightHandler(){
    fogLightHandler_.compute();
  }

private:
  static LightControlManager* lightControlInstance;

  LightRequester lightRequester_; 
  FogLightHandler fogLightHandler_ {lightRequester_};
  FrontLightBeamHandler frontLightBeamHandler_ {lightRequester_};
  
  LightControlManager(){}

};
#endif