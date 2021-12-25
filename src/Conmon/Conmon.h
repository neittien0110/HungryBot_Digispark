#ifndef _CONMON_H_
#define _CONMON_H_
#include<Arduino.h>
#include <EEPROM.h>
/**
 * @brief read/write data form/to EEPROM
 * 
 * @param s_address start address in EEPROM
 * @param length number of byte readed/write
 * @param value  byte array readed/write form/to EEPROM
 */
void read_eeprom(int s_address,int length,byte *value);
void write_eeprom(int s_address,int length,byte *value);
#endif
