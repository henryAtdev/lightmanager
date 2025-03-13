#ifndef _INPUT_PROCESSING_H
#define _INPUT_PROCESSING_H


#include <HardwareSerial.h>
#include "websocketconnection.h"

#include <mutex>

#define MAXBYTES 3
#define BUTTONPIN 35

extern std::mutex mutex;

// alle Funktionen die zu input_processing gehören;
typedef struct {
  bool StickHome;
  bool StickUp;
  bool StickDown;
  bool StickHighBeam;
  bool StickFlashingState;
} StickState;
typedef struct{
  uint16_t speed;
  StickState stickstate;
} SensorState;
typedef struct{
  uint32_t value;
}Command;
typedef struct _Element{
  _Element *next;
  Command *command_pointer;
}Element;
enum ButtonState{
  ButtonNotApplicable,
  ButtonIgnitionOn,
  ButtonIgnitionOff,
  ButtonWarningOn,
  ButtonWarningOff,
  ButtonFogFrontOn,
  ButtonFogFrontOff,
  ButtonFogRearOn,
  ButtonFogRearOff
};
enum SwitchState{
  SwitchNotApplicable,
  SwitchParkingOnSide,
  SwitchParkingAll,
  SwitchAuto,
  SwitchManually
};
enum Status {
  E_NOK = 0,
  E_OK = 1
   };
enum FrontLightStrategyState{
  StrategyNotApplicable = 0,
  StrategyChanged = 1
};
typedef struct {
  SwitchState actual_switch_state;
  StickState actual_stick_state;
  ButtonState actual_button_state;
  FrontLightStrategyState actual_strategy;
}PanelState;


Element* addElement(Element* root, const uint32_t value);
uint16_t getSize(Element* root);
Element* removeSequence(Element* root, const uint16_t size);
Status getValue(Element* root, const uint16_t pos, uint32_t* value);
void printList(Element* root);
void printValue(Element* root, const uint16_t pos);
uint16_t get_speed(const uint8_t bytes[], const uint8_t counter);
StickState get_stickstate(const uint8_t bytes[], const uint8_t counter);
void receiveDataCbk(UInt8Ptr data, size_t length);
FrontLightStrategyState getStrategyButtonState();

extern Element* commandlist;



#endif