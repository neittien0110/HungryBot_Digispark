#include "Conmon.h"
void read_eeprom(int s_address,int length,byte *value){
    EEPROM.begin(512);
    for(int i = s_address;i<s_address+length;i++){
      value[i-s_address] = EEPROM.read(i);
    }
}
void write_eeprom(int s_address,int length,byte *value){
  EEPROM.begin(512);
  Serial.printf("size: %d\n",length);
  for(int i=0;i<length;i++){
    EEPROM.write(s_address+i,value[i]);
  }
  EEPROM.commit();
  
}
