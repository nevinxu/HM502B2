#ifndef BLE_SEND_H_
#define BLE_SEND_H_

#include "hal_types.h"

#define MAXNUM          16
#define PACKSIZE        2

typedef struct 
{
  uint8 state[3];
  uint8 data[PACKSIZE][MAXNUM];
}STRSEND;

typedef struct 
{
  uint8 len;
  STRSEND strsenddata;
}STRREV;

extern void Rev_data_analy(uint8* data);

#endif
