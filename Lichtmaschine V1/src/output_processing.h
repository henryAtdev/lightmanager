#ifndef _OUTPUT_PROCESSING_H
#define _OUTPUT_PROCESSING_H

#include "input_processing.h"

//int rearLightBrightness;

typedef struct {
  uint16_t brightness;
}BreakingState;
typedef struct {
  uint8_t dir;
  uint8_t ledbits;
}BlinkingState;
enum OperationState{
  OpsOn = 0,
  OpsOff = 1
};


typedef const struct{
  uint8_t rucklicht_links;
  uint8_t rucklicht_rechts;
  uint8_t blinker_hl_innen;
  uint8_t blinker_hl_mitte;
  uint8_t blinker_hl_aussen;
  uint8_t blinker_hr_innen;
  uint8_t blinker_hr_mitte;
  uint8_t blinker_hr_aussen;
  uint8_t blinker_vr;
  uint8_t blinker_vl;
  uint8_t nebelschlussleuchte;
  uint8_t abblendlicht_r;
  uint8_t abblendlicht_l;
  uint8_t fernlicht_r;
  uint8_t fernlicht_l;
  uint8_t nebelscheinwerfer_vorne; //Zwecks mangelnder LEDs leuchtet hier ebenfalls die hintere weiße "Nebelschlussleuchte"
}LedPins;

LedPins led_pins = {4, 19, 0, 2, 15, 21, 22, 23, 33, 12, 18, 25, 14, 27, 26, 14};
extern uint8_t recived_data[MAXBYTES];


void setBlinkingLed(const BlinkingState blinkingState);
void setBlinkingPanel(const BlinkingState blinkingState);
void setBreakingLed (const BreakingState breakingstate);
void setHighBeamLight(const OperationState opsState);
void setHighBeamPanel(const OperationState opsState);
void setFogFrontLight(const OperationState opsState);
void setFogFrontPanel(const OperationState opsState);
void setFogRearLight(const OperationState opsState);
void setFogRearPanel(const OperationState opsState);
void setParkingLight(const OperationState opsState);
void setParkingAllPanel(OperationState opsState);
void setLowBeamLight(const OperationState opsState);
void setLowBeamPanel(const OperationState opsState);
void setAdaptiveLowBeamLight(const OperationState opsState);

#endif