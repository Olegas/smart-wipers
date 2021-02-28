//
// Created by Oleg Elifantiev on 28.02.2021.
//

#include "vehicle_wipers.h"
#include "usart.h"
#include "can.h"
#include "vehicle_util.h"
#include <string.h>

char *parkingWipersStr = "Parking wipers...";
uint8_t need_park = 0;
uint8_t pkt626_1[] = {0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t pkt625_1[] = {0x00, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t pkt175_1[] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t pkt625_2[] = {0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t pkt175_2[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void request_park() {
  need_park = 1;
}

void park_wipers_if_needed() {
  if (need_park == 0) {
    return;
  }
  beep(1, 1, 50);
  need_park = 0;
#ifdef DEBUG_REPORT_VEHICLE_STATE
  HAL_UART_Transmit(&huart1, (uint8_t *) parkingWipersStr, strlen(parkingWipersStr), 100);
#endif

#ifdef ENABLE_PARKING
  LSCAN_SendData(0x626, pkt626_1, 8);
  HAL_Delay(10);

  LSCAN_SendData(0x625, pkt625_1, 8);
  HAL_Delay(10);
  LSCAN_SendData(0x175, pkt175_1, 8);
  HAL_Delay(10);

  for (uint8_t i = 0; i < 17; i++) {
    LSCAN_SendData(0x625, pkt625_2, 8);
    HAL_Delay(100);
  }

  LSCAN_SendData(0x625, pkt625_1, 8);
  HAL_Delay(10);
  LSCAN_SendData(0x175, pkt175_2, 8);
  HAL_Delay(10);

#endif
}