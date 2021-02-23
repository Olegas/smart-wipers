//
// Created by Oleg Elifantiev on 14.02.2021.
//

#ifndef SMART_WIPERS_VEHICLE_STATE_H
#define SMART_WIPERS_VEHICLE_STATE_H

#include "main.h"

#define TEMP_MSG_ID (0x350)
#define ENGINE_STATUS_MSG_ID (0x145)

#define ENGINE_STATUS_UNKNOWN (255)
#define ENGINE_STATUS_RUNNING (2)
#define ENGINE_STATUS_IGNITION (1)
#define ENGINE_STATUS_OFF (0)

void update_current_temp(double new_temp);
void update_engine_status(uint8_t new_status);

#endif //SMART_WIPERS_VEHICLE_STATE_H
