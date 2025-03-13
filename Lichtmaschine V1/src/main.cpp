#include <HardwareSerial.h>
#include "websocketconnection.h"
#include <mutex>

#include "input_processing.h"
#include "light_control.h"
#include "output_processing.h"

std::mutex mutex;

#define BLINKLENGTH 500
#define BREAKINGTIME 500
#define MAXBYTES 3

#define BUTTONPIN 35

const BOOL isLog = TRUE;

//individual access point configuration
const char* ssid = "ESP32-A";
const char* password = "87654321"; // min. 8 digits

// global variables
uint8_t recived_data[MAXBYTES] =  {0b00000000, 0b00000000, 0b00000000};
uint8_t data_to_send[] =  {0b00010000, 0b00000001, 0b00000000}; 
uint8_t data_to_send2[] = {0b00010000, 0b00000010 ,0b00000000};
Element* commandlist = NULL;

long safe_time=0;

int counter= 0;
uint8_t array[MAXBYTES];

// setup
void setup(){
  Serial.begin(115200);
  startConnection(ssid, password);
  // set all LEDs as output
  pinMode(led_pins.abblendlicht_l, OUTPUT);
  pinMode(led_pins.abblendlicht_r, OUTPUT);
  pinMode(led_pins.blinker_hl_aussen, OUTPUT);
  pinMode(led_pins.blinker_hl_innen, OUTPUT);
  pinMode(led_pins.blinker_hl_mitte, OUTPUT);
  pinMode(led_pins.blinker_hr_aussen, OUTPUT);
  pinMode(led_pins.blinker_hr_innen, OUTPUT);
  pinMode(led_pins.blinker_hr_mitte, OUTPUT);
  pinMode(led_pins.blinker_vl, OUTPUT);
  pinMode(led_pins.blinker_vr, OUTPUT);
  pinMode(led_pins.fernlicht_l, OUTPUT);
  pinMode(led_pins.fernlicht_r, OUTPUT);
  pinMode(led_pins.nebelschlussleuchte, OUTPUT);
  pinMode(led_pins.rucklicht_links, OUTPUT);
  pinMode(led_pins.rucklicht_rechts, OUTPUT);  
  pinMode(BUTTONPIN, INPUT);
}


// loop
void loop() {
  //auslesen der empfangenen Daten
  uint32_t final_value = 0;
  Status data_status;
  mutex.lock();
  uint16_t size = getSize(commandlist);
  if (size>0){                                    //Wert an pos 0 der Liste wird in final value gespeichert, pos 0 wird anschließend gelöscht
    data_status=getValue(commandlist, 0, &final_value);
    if(data_status==E_OK){
      commandlist=removeSequence(commandlist,1);
    }
  }
  mutex.unlock();
  
  for(int i=MAXBYTES-1; i>=0; i--){               //final Value wieder in 3 8bit Teile zerlegen und in received data speichern
    recived_data[i] = final_value & 0b11111111;
    final_value=final_value >> 8;
  }

  //Aufgabenblatt 1
  static long last_speed_change = 0;
  static bool breaking_led_status = 0;
  ws.cleanupClients();
  SensorState safed_sensor_state{0,{0,0,0,0,0}};
  // check, if data is available on serial interface
  if (Serial.available()) {
    String input= Serial.readStringUntil(' ');
    array[counter]= input.toInt();
    Serial.println(array[counter]);
    counter++;   
  }
  // if stickposition changes --> safe Stickposition
    if (recived_data[0]==0b00000100){
      StickState stickposition = get_stickstate(recived_data, MAXBYTES);
      safed_sensor_state.stickstate = stickposition;
    if (safed_sensor_state.stickstate.StickUp){
      Serial.println("rechter Blinker");
    }
    if (safed_sensor_state.stickstate.StickDown){
      Serial.println("linker Blinker");
    }
  }
  // if the speed changes
  if (recived_data[0] == 0b00001000 && (recived_data[1]&0b01000000) == 0b01000000){
    safed_sensor_state.speed = get_speed(recived_data, MAXBYTES);
    BreakingState act_breakingstate = computeBreakingBrightness(safed_sensor_state.speed);
    setBreakingLed(act_breakingstate);
    recived_data[0] = 0b00000000;
    last_speed_change = millis();
    breaking_led_status = 1;
  }
  // process the time of the breaking Led
  if (breaking_led_status&&last_speed_change+BREAKINGTIME <=millis()){
    BreakingState act_breakingstate;
    act_breakingstate.brightness=0;
    setBreakingLed(act_breakingstate);
    breaking_led_status = 0;
  }
  long timediff = computeTimeDiff();
  
  
  
  // Aufgabenblatt 4
  PanelState act_panelstate = panelstateBuilder();
  firePanelEvent(act_panelstate.actual_switch_state, 
                 act_panelstate.actual_stick_state,
                 act_panelstate.actual_button_state,
                 act_panelstate.actual_strategy); 

  BlinkingState blinker_state = computeBlinkingRunning(safed_sensor_state, timediff, act_panelstate);
  setBlinkingLed(blinker_state);
  setBlinkingPanel(blinker_state);
  // computeNextLoop();;
}