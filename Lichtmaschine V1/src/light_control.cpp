#include "light_control.h"
#include "LightControlManeger.h"
//#include"LightRequester.h"

long lasttime;

void buttonCbk(const PanelState state){
  static PanelState last_panel;
  static OperationState opstate_fog_light;
  static OperationState opstate_fog_light_rear;
  
  LightControlManager* light_control_manager_instance = LightControlManager::getInstance();
  LightRequester light_requester_instance = light_control_manager_instance->getLightRequester();
  

  if (last_panel.actual_button_state != state.actual_button_state){
    last_panel.actual_button_state = state.actual_button_state;
    switch(state.actual_button_state){
      case ButtonNotApplicable:
        Serial.println("ButtonNotApplicable");
        break;
      case ButtonIgnitionOn: 
        Serial.println("ButtonIgnitionOn");
        light_requester_instance.requestIgnitionOn();
        break;
      case ButtonIgnitionOff:
        Serial.println("ButtonIgnitionOff");
        light_requester_instance.requestIgnitionOff();
        break;
      case ButtonWarningOn: 
        Serial.println("ButtonWarningOn");
        break;
      case ButtonWarningOff: 
        Serial.println("ButtonWarningOff");
        break;
      case ButtonFogFrontOn: 
        Serial.println("ButtonFogFrontOn");
        light_requester_instance.requestFogFrontOn();
        opstate_fog_light = OpsOn;
        setFogFrontLight(opstate_fog_light);
        setFogFrontPanel(opstate_fog_light);
        break;
      case ButtonFogFrontOff:
        light_requester_instance.requestFogFrontOff();
        Serial.println("ButtonFogFrontOff");
        opstate_fog_light = OpsOff;
        setFogFrontLight(opstate_fog_light);
        setFogFrontPanel(opstate_fog_light);
        switchCbk(state);
        break;
      case ButtonFogRearOn:
        light_requester_instance.requestFogRearOn(); 
        Serial.println("ButtonFogRearOn");
        opstate_fog_light_rear = OpsOn;
        setFogRearLight(opstate_fog_light_rear);
        setFogRearPanel(opstate_fog_light_rear);
        break;
      case ButtonFogRearOff: 
        light_requester_instance.requestFogRearOff();
        Serial.println("ButtonFogRearOff");
        opstate_fog_light_rear = OpsOff;
        setFogRearLight(opstate_fog_light_rear);
        setFogRearPanel(opstate_fog_light_rear);
      default:;
    }
  }
}

// Wenn sich etwas am Stickstate ändert --> die entsprechenden Leds setzen
void stickCbk(const PanelState state){
  static bool last_stick_flashingstate;
  static bool last_stick_high_beam;
  //LightControlManager* light_control_manager_instance = LightControlManager::getInstance();
  //LightRequester ligh_requester_instance = light_control_manager_instance->getLightRequester();
  if (state.actual_stick_state.StickHome){
    Serial.println("StickHome");
    if (last_stick_flashingstate){
      setHighBeamLight(OpsOff);
      setHighBeamPanel(OpsOff);
      last_stick_flashingstate = false;
    }
  }
  else if (state.actual_stick_state.StickUp){
    Serial.println("StickUp");
  }
  else if (state.actual_stick_state.StickDown){
    Serial.println("StickDown");
  }
  else if (state.actual_stick_state.StickHighBeam){
    Serial.println("StickHighBeam");
      if (!last_stick_high_beam){
        setHighBeamLight(OpsOn);
        setHighBeamPanel(OpsOn);
        last_stick_high_beam = true;
      }
      else if(last_stick_high_beam){
        setHighBeamLight(OpsOff);
        setHighBeamPanel(OpsOff);
        last_stick_high_beam = false;
      }
  }
  else if (state.actual_stick_state.StickFlashingState){
    Serial.println("StickFlashingState");
    setHighBeamLight(OpsOn);
    setHighBeamPanel(OpsOn);
    last_stick_flashingstate = true;
  }
}

//Wenn Sich etwas am switchState etwas verändert hat wird diese Funktion ausgeführt,
void switchCbk(const PanelState state){
  LightControlManager* light_control_manager_instance = LightControlManager::getInstance();
  LightRequester light_requester_instance = light_control_manager_instance->getLightRequester();
  FrontLightStrategyState strategyState = getStrategyButtonState();
  static bool act_manual = false; 

  switch (state.actual_switch_state){
    case SwitchNotApplicable:
      Serial.println("SwichNotApplicable");
      setParkingLight(OpsOff);
      setLowBeamLight(OpsOff);
      setLowBeamPanel(OpsOff);
      setParkingAllPanel(OpsOff);
    break;
    case SwitchParkingOnSide:
      
      Serial.println("SwitchParkingOnSide");
      light_requester_instance.requestParkingOnSideOn();
      light_requester_instance.requestParkingAllOff();
      light_requester_instance.requestAutoLowBeamOff();
      light_requester_instance.requestManualLowBeamOff();
      setLowBeamLight(OpsOff);
      setLowBeamPanel(OpsOff);
      setParkingAllPanel(OpsOff);
      setParkingLight(OpsOn);
    break;
    case SwitchParkingAll:
      Serial.println("SwitchParkingAll");
      light_requester_instance.requestParkingOnSideOff();
      light_requester_instance.requestParkingAllOn();
      light_requester_instance.requestAutoLowBeamOff();
      light_requester_instance.requestManualLowBeamOff();
      setParkingLight(OpsOff);
      setLowBeamPanel(OpsOff);
      setLowBeamLight(OpsOn);
      setParkingAllPanel(OpsOn);
    break;
    case SwitchAuto:
      Serial.println("SwitchAuto");
      light_requester_instance.requestParkingOnSideOff();
      light_requester_instance.requestParkingAllOff();
      light_requester_instance.requestAutoLowBeamOn();
      light_requester_instance.requestManualLowBeamOff();
      setParkingLight(OpsOff);
      setLowBeamLight(OpsOff);
      setLowBeamPanel(OpsOff);
      setParkingAllPanel(OpsOff);
      act_manual = false;
    break;
    case SwitchManually:
      light_requester_instance.requestParkingOnSideOff();
      light_requester_instance.requestParkingAllOff();
      light_requester_instance.requestAutoLowBeamOff();
      light_requester_instance.requestManualLowBeamOn();
      setParkingLight(OpsOff);
      setParkingAllPanel(OpsOff);
      setLowBeamLight(OpsOn);
      setLowBeamPanel(OpsOn);
      act_manual = true;
    break;
    default:;
  }
}

void strategyCbk(const PanelState state){
 LightControlManager* light_control_manager_instance = LightControlManager::getInstance();
 //LightRequester light_requester_instance = light_control_manager_instance->getLightRequester();
  light_control_manager_instance->switchFrontLightStrategie();
  
}

long computeTimeDiff(){
  long lasttime = safe_time;
  safe_time = millis();
  long timediff = safe_time-lasttime;
  return(timediff); 
}
BreakingState computeBreakingBrightness (const uint16_t speed){
  static int16_t last_speed=0;
  static BreakingState light_brightness;
  int16_t speed_diff;
  speed_diff = last_speed - speed; 
    if (speed_diff <= 10){
      light_brightness.brightness=0;
    }
  else if (speed_diff <= 30 && speed_diff > 10){
      light_brightness.brightness=50;
    }
  else if (30 < speed_diff){
      light_brightness.brightness=100;
    }
    last_speed=speed;
  return light_brightness;
}
BlinkingState computeBlinkingImpulse(const SensorState sensorstate, const long timediff, PanelState panel){
  static BlinkingState act_blinkingstate;
  // Warnblinker testen
  if(panel.actual_button_state == ButtonWarningOn){
    act_blinkingstate.dir =3;
  }
  // Normalen Blinker setzen
  else if (sensorstate.stickstate.StickUp && act_blinkingstate.dir!=1){
    act_blinkingstate.dir = 1;
  }
  else if (sensorstate.stickstate.StickDown && act_blinkingstate.dir!=2){
    act_blinkingstate.dir = 2;
  }
  else if ((sensorstate.stickstate.StickUp && act_blinkingstate.dir==1)|| 
           (sensorstate.stickstate.StickDown && act_blinkingstate.dir==2)||
           (panel.actual_button_state == ButtonWarningOff && act_blinkingstate.dir==3))
           {
    act_blinkingstate.dir = 0;
  }
  return (act_blinkingstate);
}
// returns in ledbits which led has to glow as number from 0-4
BlinkingState computeBlinkingRunning(const SensorState sensorstate, const long timediff, PanelState panel){
  static BlinkingState last_blinkstate;
  static long last_time=0;
  last_time = last_time+timediff;
  BlinkingState act_blinkingstate = computeBlinkingImpulse(sensorstate, timediff, panel);
  last_blinkstate.dir=act_blinkingstate.dir;
  if(act_blinkingstate.dir == 0){
    last_time = 0;
    act_blinkingstate.ledbits = 0;
    last_blinkstate.ledbits = 0;
  }
  else{
    if (last_blinkstate.ledbits<1 && last_time>BLINKLENGTH&&last_time<BLINKLENGTH*2){
      act_blinkingstate.ledbits = 1;
      last_blinkstate.ledbits = 1;
    }
    else if (last_blinkstate.ledbits<2 && last_time>=BLINKLENGTH*2&&last_time<BLINKLENGTH*3){
      act_blinkingstate.ledbits = 2;
      last_blinkstate.ledbits = 2;
    }
    else if (last_blinkstate.ledbits<4 && last_time>=BLINKLENGTH*3&&last_time<BLINKLENGTH*4){
      act_blinkingstate.ledbits = 4;
      last_blinkstate.ledbits = 4;
    }
    else if (act_blinkingstate.ledbits<4&& last_time>BLINKLENGTH*4){
      act_blinkingstate.ledbits = 0;
      last_blinkstate.ledbits = 0;
      last_time = 0;
    }
    
  }
  return last_blinkstate;
}
// wenn sich etwas seit dem letzten Aufruf auf dem Pannel verändert hat --> ausführen der entsprechnenden Funktion der Leds
// Liste mit den Funktionsadressen erstellen. --> jede Cbk Funktion hat in der Liste ihre Adresse gespeichert
void (*cbkPanelRegistration[])(const PanelState state)= {&switchCbk, &stickCbk, &buttonCbk, &strategyCbk};
// In der main wird firePanelEvent aufgerufen. Dieses kennt die LightControl- bzw Outputfunktionen nicht, sondern ruft über 
// Funktionsarray cbkPanelRegistration die jeweiligen Funktionen indirekt über einen Callback (Adresse) auf.
void firePanelEvent(SwitchState act_switch, StickState act_stick, ButtonState act_button, FrontLightStrategyState act_strategy){  
  static SwitchState last_switch;
  static StickState last_stick;
  static ButtonState last_button;
  static FrontLightStrategyState last_strategy;
  static bool lastPoint;
  PanelState act_panel_state;
  act_panel_state.actual_button_state = act_button;
  act_panel_state.actual_stick_state = act_stick;
  act_panel_state.actual_switch_state = act_switch; 
  act_panel_state.actual_strategy = act_strategy;
  if(last_switch != act_switch){
    cbkPanelRegistration[0] (act_panel_state);
  }
  if(last_button != act_button){
    cbkPanelRegistration[2](act_panel_state);
  }
  // Hier muss jede Möglichkeit einzeln verglichen werden, da Stickstate ein struct ist.
  if((last_stick.StickDown != act_stick.StickDown) ||
    (last_stick.StickFlashingState != act_stick.StickFlashingState) ||
    (last_stick.StickHighBeam != act_stick.StickHighBeam) ||
    (last_stick.StickHome != act_stick.StickHome) ||
    (last_stick.StickUp != act_stick.StickUp)){
    cbkPanelRegistration[1](act_panel_state);
  }
  if (act_switch == SwitchManually && act_strategy != last_strategy && act_strategy == StrategyChanged){
    cbkPanelRegistration[3](act_panel_state);
  }
  last_switch = act_switch;
  last_stick = act_stick;
  last_button = act_button;
  last_strategy = act_strategy;

}

PanelState panelstateBuilder(){
  static PanelState act_panelstate;
  // wenn sich der Buttonstate ändert
  if (recived_data[0] == 0b00000001){
    switch(recived_data[1]){
      case 0b00000001: 
      act_panelstate.actual_button_state = ButtonIgnitionOn;
      break;
      case 0b00000010: 
      act_panelstate.actual_button_state = ButtonIgnitionOff;
      break;
      case 0b00000100: 
      act_panelstate.actual_button_state = ButtonWarningOn;
      break;
      case 0b00001000:
      act_panelstate.actual_button_state = ButtonWarningOff;
      break;
      case 0b00010000:
      act_panelstate.actual_button_state = ButtonFogFrontOn;
      break;
      case 0b00100000:
      act_panelstate.actual_button_state = ButtonFogFrontOff;
      break;
      case 0b01000000:
      act_panelstate.actual_button_state = ButtonFogRearOn;
      break;
      case 0b10000000:
      act_panelstate.actual_button_state = ButtonFogRearOff;
      break;
      default:;
    }  
  }
  // wenn sich switchstate ändert
  else if (recived_data[0] == 0b00000010){
    switch(recived_data[1]){  
      case 0b00000001: 
      act_panelstate.actual_switch_state = SwitchParkingOnSide;
      break;
      case 0b00000010: 
      act_panelstate.actual_switch_state = SwitchParkingAll;
      break;
      case 0b00000100: 
      act_panelstate.actual_switch_state = SwitchAuto;
      break;
      case 0b00001000:
      act_panelstate.actual_switch_state = SwitchManually;
      break;
    }
  }
  // wenn sich Stickstate ändert
  else if (recived_data[0] == 0b00000100){
    switch(recived_data[1]){  
      case 0b00000001: 
      act_panelstate.actual_stick_state.StickHome = true;
      act_panelstate.actual_stick_state.StickHighBeam = false;
      act_panelstate.actual_stick_state.StickFlashingState = false;
      break;
      case 0b00001000: 
      act_panelstate.actual_stick_state.StickHighBeam = true;
      act_panelstate.actual_stick_state.StickHome = false;
      act_panelstate.actual_stick_state.StickFlashingState = false;
      break;
      case 0b00010000: 
      act_panelstate.actual_stick_state.StickFlashingState = true;
      act_panelstate.actual_stick_state.StickHome = false;
      act_panelstate.actual_stick_state.StickHighBeam = false;      
      break;
      default:;
    }
  }
  act_panelstate.actual_strategy = getStrategyButtonState();
  return act_panelstate;
}

void computeNextLoop(uint64_t timediff){
  LightControlManager* lightControlInstance = LightControlManager::getInstance();
  LightRequester lightRequesterInstance = lightControlInstance->getLightRequester();
  lightRequesterInstance.requestNextLoop(timediff); 
}