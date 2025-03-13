#ifndef _LIGHT_REQUESER
#define _LIGHT_REQUESER
#include <mutex>
#include "LightRequestRegistry.h"


class LightRequestCollector : public LightRequestRegistry {
  protected:
    bool ignitionOn_;
    bool blinkerLeftOn_;
    bool blinkerRightOn_;
    bool blinkerWarningOn_;
    bool autoLowBeamOn_;
    bool manualLowBeam_;
    bool highBeamOn_;
    bool fogFrontOn_;
    bool fogRearOn_;
    bool parkingOnsideOn_;
    bool parkAllOn_;
  public:
    bool isIgnitionOn(){
      return ignitionOn_;
    };
    bool isBlinkingLeft(){
      return blinkerLeftOn_;
    };
    bool isBlinkingRight(){
      return blinkerRightOn_;
    };
    bool isBlinkerWarningOn(){
      return blinkerWarningOn_;
    };
    bool isAutoLowBeamOn(){
      return autoLowBeamOn_;
    };
    bool isManualLowBeamOn(){
      return manualLowBeam_;
    };
    bool isHighBeamOn(){
      return highBeamOn_;
    };
    bool isFogFrontOn(){
      return fogFrontOn_;
    };
    bool isFogRearOn(){
      return fogRearOn_;
    };
    bool isParkingOnSideOn(){
      return parkingOnsideOn_;
    };
    bool isParkingAllOn(){
      return parkAllOn_;
    };
};


class LightRequester : public LightRequestCollector{
  public:
    void requestIgnitionOn(){
      this->ignitionOn_ = true;
      fireStateChanged();
    };
    void requestIgnitionOff(){
      this->ignitionOn_ = false;
      fireStateChanged();
    };
    void requestBlinkingRightOn(){
      this->blinkerRightOn_ = true;
      fireStateChanged();
    };
    void requestBlinkingRightOff(){
      this->blinkerRightOn_ = false;
      fireStateChanged();
    };
    void requestBlinkingLeftOn(){
      this->blinkerLeftOn_ = true;
      fireStateChanged();
    };
    void requestBlinkingLeftOff(){
      this->blinkerLeftOn_ =false;
      fireStateChanged();
    };
    void requestBlinkingWarningOn(){
      this->blinkerWarningOn_ = true;
      fireStateChanged();
    };
    void requestBlinkingWarningOff(){
      this->blinkerWarningOn_ = false;
      fireStateChanged();
    };
    void requestAutoLowBeamOn(){
      this->autoLowBeamOn_ = true;
      fireStateChanged();
    };
    void requestAutoLowBeamOff(){
      this->autoLowBeamOn_ = false;
      fireStateChanged();
    };
    void requestManualLowBeamOn(){
      this->manualLowBeam_ = true;
      fireStateChanged();
    };
    void requestManualLowBeamOff(){
      this->manualLowBeam_ = false;
      fireStateChanged();
    };
    void requestHighBeamOn(){
      this->highBeamOn_ = true;
      fireStateChanged();
    };
    void requestHighBeamOff(){
      this->highBeamOn_ = false;
      fireStateChanged();
    };
    void requestFogFrontOn(){
      this->fogFrontOn_ = true;
      fireStateChanged();
    };
    void requestFogFrontOff(){
      this->fogFrontOn_ = false;
      fireStateChanged();
    };
    void requestFogRearOn(){
      this->fogRearOn_ = true;
      fireStateChanged();
    };
    void requestFogRearOff(){
      this->fogRearOn_ = false;
      fireStateChanged();
    };
    void requestParkingAllOn(){
      this->parkingOnsideOn_ = true;
      fireStateChanged();
    };
    void requestParkingAllOff(){
      this->parkingOnsideOn_ = false;
      fireStateChanged();
    };
    void requestParkingOnSideOn(){
      this->parkingOnsideOn_ = true;
      fireStateChanged();
    };
    void requestParkingOnSideOff(){
      this->parkingOnsideOn_ = false;
      fireStateChanged();
    };
    void requestNextLoop(const uint64_t timeDiff){
      fireLoopChanged(timeDiff);
    };
};
#endif