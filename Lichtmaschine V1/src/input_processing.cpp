#include "input_processing.h"


Element* addElement(Element* root, const uint32_t value){
  Element *newElement = (Element*)malloc(sizeof(Element));
  Command *newCommand = (Command*)malloc(sizeof(Command));
  newElement->next = NULL;
  newCommand->value=value;
  newElement->command_pointer= newCommand; // In den letzten 3 Zeilen wird das neue Element mit werten erstellt
  if (root==NULL){    //Wenn die Liste (noch) keine Elemente hat, wird in root die adresse vom ersten element gespeichert 
    root=newElement;
  }
  else{         //es wird die Adresse des letzten Elements ermittelt, dort wird die adresse des neuen elements gespeichert
    Element* adress_next_variable;
    Element next_variable = *root;
    if (next_variable.next==NULL){
      adress_next_variable = root;
    }
    while (next_variable.next != NULL){
      adress_next_variable = next_variable.next;
      next_variable = *next_variable.next;
    }
    adress_next_variable->next = newElement;
  }
  return root;    //Adresse des ersten elements wird zurückgegeben
} 

uint16_t getSize(Element* root){    //liefert die rückgabe, wieviele Elemente sich in einer Liste befinden
  uint16_t size = 1;
  if (root == NULL){
    size=0;
  }
  else{
    Element next_variable = *root;
    while (next_variable.next != NULL){
      size =size + 1;
      next_variable = *next_variable.next;
    }
  }
  return size;
}

Element* removeSequence(Element* root, const uint16_t size){    //löscht Anzahl size an Elementen, vom Anfang der Liste
  uint16_t size_of_list = getSize (root);
  uint16_t new_size;
  Element next_variable;
  Element* adress_of_next_variable;
  if(size_of_list<size){    //checken, das maximal so viele elemente wie die liste hat gelöscht werden
    new_size= size_of_list;
    Serial.println("Die einegebene Size war zu groß, daher wurden alle Elemente gelöscht");
  }
  else{
    new_size=size;
  }
  for (uint16_t i = new_size; i>0; i--){
    next_variable=*root;
    adress_of_next_variable=next_variable.next;
    free(root);
    root = adress_of_next_variable;
  }
  if(size_of_list == new_size){
    root=NULL;
  }
  return root;
}

Status getValue(Element* root, const uint16_t pos, uint32_t* value){    //das nullte Element ist an Position 0!
  uint16_t size_of_list = getSize (root);                               //Funktion speichert unter der Adresse value, den Wert der an position pos der Liste steht,
  Status value_status;                                                  //Rückgabewert Status gibt an ob operation erfolgreich war oder fehlgeschlagen ist
  Element next_variable;
  Element* adress_of_next_variable;
  Command data_value;
  uint32_t command_value;
  if(size_of_list<pos){   //Checken, ob die Liste weniger Elemente als die angefragte position hat, Status dementsprechend zurückgeben
    value_status = E_NOK;
  }
  else{
    value_status = E_OK;
    for (uint16_t i = pos; i>0; i--)    //in root wird Zeiger auf das Element der Liste gespeichert, von dem der Value erfragt ist
    {
      next_variable=*root;
      root = next_variable.next;
    }
    next_variable = *root;
    data_value = *next_variable.command_pointer;
    command_value = data_value.value;
    *value = command_value;
  }
  return value_status;
}

void printList(Element* root){    //Printet alle Werte inc Position dieser Werte im Seriellen Monitor aus
  Element next_variable;
  Command data_value;
  uint16_t size =getSize(root);
  for(int16_t i=1; i<=size; i++){
    Serial.print("Position: ");
    Serial.print(i-1);
    next_variable=*root;
    root = next_variable.next;
    data_value=*next_variable.command_pointer;
    Serial.print("   Value: ");
    Serial.println(data_value.value);
  }
}

void printValue(Element* root, const uint16_t pos){    //Printet den Wert an der Stelle pos der Liste aus
  uint32_t value;
  uint32_t *ptr_value = &value;
  Status status_value = getValue(root, pos, ptr_value);
  if (status_value == E_OK){
    Serial.println(value);
  }
  else{
    Serial.println("Die Liste hat nicht soviele Stellen wie angefragt!");
  }
}

//get the data and safe it in the global variable recived_data
void receiveDataCbk(UInt8Ptr data, size_t length){
  // insert here what happens with new data;
  uint32_t value=0;
  for (int i=0; i<MAXBYTES; i++){
    value= value <<8;
    value = value | data[i];
  }

  mutex.lock();
  commandlist = addElement(commandlist, value);
  mutex.unlock();
}
//returns the decoded actual speed
uint16_t get_speed(const uint8_t bytes[], const uint8_t counter){
  uint16_t speed;
  uint8_t byte2 = bytes[1] & 0b00111111;
  uint8_t byte3 = bytes[2];
  speed= byte3 + (256*byte2);
  return speed; 
}

// returns the Stickstate in a struct
StickState get_stickstate(const uint8_t bytes[], const uint8_t counter){
  StickState act_stickstate = {0,0,0,0,0};
  switch(bytes[1]){
  case 0b00000001: 
  act_stickstate.StickHome = 1;
  break;
  case 0b00000010:
  act_stickstate.StickUp = 1;
  break;
  case 0b00000100:
  act_stickstate.StickDown = 1;
  break;
  case 0b00001000:
  act_stickstate.StickHighBeam = 1;
  break;
  case 0b00010000:
  act_stickstate.StickFlashingState = 1;
  break;
  case 0b00100000:
  act_stickstate.StickFlashingState = 0;
  break;
  default:
  act_stickstate={0,0,0,0,0};
  }
  return (act_stickstate);
}

FrontLightStrategyState getStrategyButtonState(){
 bool buttonState = digitalRead(BUTTONPIN);
 FrontLightStrategyState actFrontLightStrategyState;
 if(!buttonState){
  actFrontLightStrategyState = StrategyNotApplicable;
 }
 else {
  actFrontLightStrategyState = StrategyChanged;
 }
 return actFrontLightStrategyState;
}