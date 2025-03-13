#ifndef _LIGHT_CONTROL_H
#define _LIGHT_CONTROL_H

#include "input_processing.h"
#include "output_processing.h"


#define BLINKLENGTH 500


extern long safe_time;
extern uint8_t recived_data[MAXBYTES];


BreakingState computeBreakingBrightness (const uint16_t speed);
BlinkingState computeBlinkingImpulse(const SensorState sensorstate, const long timediff, PanelState panel);
BlinkingState computeBlinkingRunning(const SensorState sensorstate, const long timediff, PanelState panel);
void firePanelEvent(SwitchState act_switch, StickState act_stick, ButtonState act_button, FrontLightStrategyState act_strategy);
PanelState panelstateBuilder();
long computeTimeDiff();
uint8_t getSteeringAngle();
uint8_t getLightConditions();
void computeNextLoop(uint64_t timediff);

void buttonCbk(const PanelState state);
void stickCbk(const PanelState state);
void switchCbk(const PanelState state);
void strategyCbk(const PanelState state);
void computeNextLoop(uint64_t timediff);


#endif