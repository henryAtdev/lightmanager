#include "output_processing.h"


 
void setBlinkingLed(const BlinkingState blinkingState){
  // dir==1 --> setzt den rechten Blinker
  if (blinkingState.dir == 1){
    if(blinkingState.ledbits>0){
      digitalWrite(led_pins.blinker_hr_innen, HIGH);
      digitalWrite(led_pins.blinker_vl, LOW);
      digitalWrite(led_pins.blinker_hl_innen, LOW);
      digitalWrite(led_pins.blinker_hl_mitte, LOW);
      digitalWrite(led_pins.blinker_hl_aussen, LOW);
    }
    if(blinkingState.ledbits>1){
      digitalWrite(led_pins.blinker_hr_mitte, HIGH);
      digitalWrite(led_pins.blinker_vr, HIGH);
    }
    if(blinkingState.ledbits>2){
      digitalWrite(led_pins.blinker_hr_aussen, HIGH);
    }
    if (blinkingState.ledbits==0){
      digitalWrite(led_pins.blinker_hr_innen, LOW);
      digitalWrite(led_pins.blinker_hr_mitte, LOW);
      digitalWrite(led_pins.blinker_hr_aussen, LOW);
      digitalWrite(led_pins.blinker_hl_innen, LOW);
      digitalWrite(led_pins.blinker_hl_mitte, LOW);
      digitalWrite(led_pins.blinker_hl_aussen, LOW);
      digitalWrite(led_pins.blinker_vr, LOW);
      digitalWrite(led_pins.blinker_vl, LOW);
    }
  }
  // dir == 2 --> setzt den linken Blinker
  if (blinkingState.dir == 2){
    if(blinkingState.ledbits>0){
      digitalWrite(led_pins.blinker_hl_innen, HIGH);
      digitalWrite(led_pins.blinker_vr, LOW);
      digitalWrite(led_pins.blinker_hr_innen, LOW);
      digitalWrite(led_pins.blinker_hr_mitte, LOW);
      digitalWrite(led_pins.blinker_hr_aussen, LOW);
    }
    if(blinkingState.ledbits>1){
      digitalWrite(led_pins.blinker_hl_mitte, HIGH);
      digitalWrite(led_pins.blinker_vl, HIGH);

    }
    if(blinkingState.ledbits>2){
      digitalWrite(led_pins.blinker_hl_aussen, HIGH);
    }
    if(blinkingState.ledbits==0){
      digitalWrite(led_pins.blinker_hl_innen, LOW);
      digitalWrite(led_pins.blinker_hl_mitte, LOW);
      digitalWrite(led_pins.blinker_hl_aussen, LOW);
      digitalWrite(led_pins.blinker_hr_innen, LOW);
      digitalWrite(led_pins.blinker_hr_mitte, LOW);
      digitalWrite(led_pins.blinker_hr_aussen, LOW);
      digitalWrite(led_pins.blinker_vr, LOW);
      digitalWrite(led_pins.blinker_vl, LOW);
    }
  }
  // dir == 3 --> setzt beide Blinker (Warnblinker)
  if(blinkingState.dir == 3){
    if(blinkingState.ledbits>0){
      digitalWrite(led_pins.blinker_hr_innen, HIGH);
      digitalWrite(led_pins.blinker_vl, LOW);
      digitalWrite(led_pins.blinker_vr, LOW);
      digitalWrite(led_pins.blinker_hl_innen, HIGH);
      digitalWrite(led_pins.blinker_hl_mitte, LOW);
      digitalWrite(led_pins.blinker_hl_aussen, LOW);
      digitalWrite(led_pins.blinker_hr_mitte, LOW);
      digitalWrite(led_pins.blinker_hr_aussen, LOW);
    }
    if(blinkingState.ledbits>1){
      digitalWrite(led_pins.blinker_hr_mitte, HIGH);
      digitalWrite(led_pins.blinker_hl_mitte, HIGH);
      digitalWrite(led_pins.blinker_vr, HIGH);
      digitalWrite(led_pins.blinker_vl, HIGH);
    }
    if(blinkingState.ledbits>2){
      digitalWrite(led_pins.blinker_hr_aussen, HIGH);
      digitalWrite(led_pins.blinker_hl_aussen, HIGH);
    }
    if (blinkingState.ledbits==0){
      digitalWrite(led_pins.blinker_hr_innen, LOW);
      digitalWrite(led_pins.blinker_hr_mitte, LOW);
      digitalWrite(led_pins.blinker_hr_aussen, LOW);
      digitalWrite(led_pins.blinker_hl_innen, LOW);
      digitalWrite(led_pins.blinker_hl_mitte, LOW);
      digitalWrite(led_pins.blinker_hl_aussen, LOW);
      digitalWrite(led_pins.blinker_vr, LOW);
      digitalWrite(led_pins.blinker_vl, LOW);
    }
  }
  // dir == 0 --> macht blinker aus
  if(blinkingState.dir == 0){
    digitalWrite(led_pins.blinker_hr_innen, LOW);
    digitalWrite(led_pins.blinker_hr_mitte, LOW);
    digitalWrite(led_pins.blinker_hr_aussen, LOW);
    digitalWrite(led_pins.blinker_hl_innen, LOW);
    digitalWrite(led_pins.blinker_hl_mitte, LOW);
    digitalWrite(led_pins.blinker_hl_aussen, LOW);
    digitalWrite(led_pins.blinker_vr, LOW);
    digitalWrite(led_pins.blinker_vl, LOW);

  }
}
// Turns on the blinker Led on the Pannel
void setBlinkingPanel(const BlinkingState blinkingState){
  static uint8_t last_send = 0;
  uint8_t blinker_rechts_on[] =  {0b00010000, 0b00010001 ,0b00000000};
  uint8_t blinker_links_on[] = {0b00010000, 0b00010100 ,0b00000000};
  uint8_t blinker_off[] =       {0b00010000, 0b00101010, 0b00000000};
  if(blinkingState.ledbits>=2&&last_send==0){
      if(blinkingState.dir == 1){
        sendData (blinker_links_on, MAXBYTES);
      }
      else if(blinkingState.dir == 2){
        sendData (blinker_rechts_on, MAXBYTES);
      }
      else if(blinkingState.dir == 3){
        sendData (blinker_rechts_on, MAXBYTES);
        sendData (blinker_links_on, MAXBYTES);
      }
      last_send=1;    
  }
  else if (blinkingState.ledbits<2&&last_send!=0){
    last_send=0;
    sendData (blinker_off, 3);
  }
}
// turns on the breaking LED dependent on their brightness
// alle weiteren set-Funktionen rufen jeweils eine Funktion auf welche sich um die Ausgabe des entsprechenden Lichts kümmert
void setBreakingLed (const BreakingState breakingstate){
  static long time = millis();
  if((breakingstate.brightness != 0) || (breakingstate.brightness == 0 && time+500<=millis())){
    if (breakingstate.brightness == 0){
      analogWrite(led_pins.rucklicht_links, 0);
      analogWrite(led_pins.rucklicht_rechts, 0);
    }
    else if (breakingstate.brightness == 50){
      analogWrite(led_pins.rucklicht_links, 127);
      analogWrite(led_pins.rucklicht_rechts, 127);
    }
    else if (breakingstate.brightness==100){
      analogWrite (led_pins.rucklicht_links, 255);
      analogWrite (led_pins.rucklicht_rechts, 255);
    }
    time = millis();
    recived_data[0] = 0b00000000;
  }
}

void setHighBeamLight(const OperationState opsState){
  if (opsState == OpsOn){
    digitalWrite (led_pins.fernlicht_l, HIGH);
    digitalWrite (led_pins.fernlicht_r, HIGH);
  }
  else if (opsState == OpsOff){
    digitalWrite (led_pins.fernlicht_l, LOW);
    digitalWrite (led_pins.fernlicht_r, LOW);

  }
}

void setHighBeamPanel(const OperationState opsState){
  uint8_t high_beam_panel_on[] =  {0b00010000,0b01000000,0b00000000};
  uint8_t high_beam_panel_off[] = {0b00010000,0b10000000,0b00000000};
  if (opsState == OpsOn){
    sendData(high_beam_panel_on, MAXBYTES);
  }
  else if (opsState == OpsOff){
    sendData(high_beam_panel_off, MAXBYTES);
  }
}

void setFogFrontLight(const OperationState opsState){
  if (opsState == OpsOn){
    digitalWrite (led_pins.nebelscheinwerfer_vorne, HIGH);
    analogWrite(led_pins.abblendlicht_l, 255);
    analogWrite(led_pins.abblendlicht_r, 255);
  }
  else if (opsState == OpsOff){
    digitalWrite (led_pins.nebelscheinwerfer_vorne, LOW);
    analogWrite(led_pins.abblendlicht_r, 0);
    analogWrite(led_pins.abblendlicht_r, 0);
  }
}

void setFogFrontPanel(const OperationState opsState){
  uint8_t fog_front_panel_on[] =  {0b00010000,0b00000000,0b00000100};
  uint8_t fog_front_panel_off[] = {0b00010000,0b00000000,0b00001000};
  if (opsState == OpsOn){
    sendData(fog_front_panel_on, MAXBYTES);
  }
  else if (opsState == OpsOff){
    sendData(fog_front_panel_off, MAXBYTES);
  }
}

void setFogRearLight(const OperationState opsState){
  if (opsState == OpsOn){
    digitalWrite (led_pins.nebelschlussleuchte, HIGH);
  }
  else if (opsState == OpsOff){
    digitalWrite (led_pins.nebelschlussleuchte, LOW);
  }
}

void setFogRearPanel(const OperationState opsState){
  uint8_t light_fog_rear_on[] =  {0b00010000,0b00000000,0b00010000};
  uint8_t light_fog_rear_off[] = {0b00010000,0b00000000,0b00100000};
  if (opsState == OpsOn){
    sendData(light_fog_rear_on, MAXBYTES);
  }
  else if (opsState == OpsOff){
    sendData(light_fog_rear_off, MAXBYTES);
  }
}

void setParkingLight(const OperationState opsState){
  if (opsState == OpsOn){
    analogWrite (led_pins.rucklicht_links, 20);
    analogWrite (led_pins.abblendlicht_l, 20);
  }
  else if (opsState == OpsOff){
    analogWrite (led_pins.rucklicht_links, 0);
    analogWrite (led_pins.abblendlicht_l, 0);
  }
}

void setParkingAllPanel(OperationState opsState){
  uint8_t light_parking_all_on[] =  {0b00010000,0b00000000,0b01000000};
  uint8_t light_parking_all_off[] = {0b00010000,0b00000000,0b10000000};
  if (opsState == OpsOn){
    sendData(light_parking_all_on, MAXBYTES);
  }
  else if (opsState == OpsOff){
    sendData(light_parking_all_off, MAXBYTES);
  }
}

void setLowBeamLight(const OperationState opsState){
  if (opsState == OpsOn){
    analogWrite (led_pins.rucklicht_links, 255);
    analogWrite (led_pins.rucklicht_rechts, 255);
    analogWrite (led_pins.abblendlicht_l, 20);
    analogWrite (led_pins.abblendlicht_r, 20);
  }
  else if (opsState == OpsOff){
    analogWrite (led_pins.rucklicht_links, 0);
    analogWrite (led_pins.rucklicht_rechts, 0);
    analogWrite (led_pins.abblendlicht_l, 0);
    analogWrite (led_pins.abblendlicht_r, 0);
  }
}


void setLowBeamPanel(const OperationState opsState){

  uint8_t low_beam_panel_on[] =  {0b00010000,0b00000000,0b00000001};
  uint8_t low_beam_panel_off[] = {0b00010000,0b00000000,0b00000010};
  if (opsState == OpsOn){
    sendData(low_beam_panel_on, MAXBYTES);
  }
  else if (opsState == OpsOff){
    sendData(low_beam_panel_off, MAXBYTES);
  }
}

void setAdaptiveLowBeamLight(const OperationState opsState){
  if (opsState == OpsOn){
    analogWrite (led_pins.rucklicht_links, 255);
    analogWrite (led_pins.rucklicht_rechts, 255);
    analogWrite (led_pins.abblendlicht_l, 255);
    analogWrite (led_pins.abblendlicht_r, 255);
  }
  else if (opsState == OpsOff){
    analogWrite (led_pins.rucklicht_links, 0);
    analogWrite (led_pins.rucklicht_rechts, 0);
    analogWrite (led_pins.abblendlicht_l, 0);
    analogWrite (led_pins.abblendlicht_r, 0);
  }
}