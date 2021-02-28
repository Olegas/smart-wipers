//
// Created by Oleg Elifantiev on 28.02.2021.
//

#include "vehicle_util.h"
#include "can.h"

void beep(uint8_t wait, uint8_t count, uint8_t length) {
  uint8_t data[] = {0x70, 0x05, wait, count, length};
  LSCAN_SendData(0x280, data, 5);
}
