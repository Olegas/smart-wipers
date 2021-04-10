//
// Created by Oleg Elifantiev on 14.02.2021.
//

#include "vehicle_state.h"
#include "config.h"
#include "tim.h"
#include "usart.h"
#include "vehicle_wipers.h"
#include <stdio.h>
#include <string.h>

char *statusStrOff = "off";
char *statusStrRunning = "running";
char *statusStrUnknown = "unknown";

char *wipersWasUsed = "was used";
char *wipersWasNotUsed = "was not used";

double current_temp = -100.0;
uint8_t wipers_was_used = 0;
uint8_t engine_status = ENGINE_STATUS_UNKNOWN;

void report_vehicle_status_to_uart() {
#ifndef DEBUG_REPORT_VEHICLE_STATE
  return;
#endif
  char buffer[255];
  char *strStatus;
  switch (engine_status) {
    case ENGINE_STATUS_RUNNING:
      strStatus = statusStrRunning;
      break;
    case ENGINE_STATUS_OFF:
      strStatus = statusStrOff;
      break;
    default:
      strStatus = statusStrUnknown;
  }

  sprintf(buffer,
          "Temp: %02.2f oC, engine: %s, wipers: %s\n",
          current_temp,
          strStatus,
          wipers_was_used > 0 ? wipersWasUsed : wipersWasNotUsed);
  HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 100);
}

void set_engine_status_led() {
    if (engine_status == ENGINE_STATUS_OFF || engine_status == ENGINE_STATUS_UNKNOWN) {
        HAL_GPIO_WritePin(LED_ACC_GPIO_Port, LED_ACC_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(LED_ACC_GPIO_Port, LED_ACC_Pin, GPIO_PIN_RESET);
    }
}

void toggle_engine_led() {
    HAL_GPIO_TogglePin(LED_ACC_GPIO_Port, LED_ACC_Pin);
}

void set_current_temp_led() {
    if (current_temp < MAX_OUT_TEMP) {
        HAL_GPIO_WritePin(LED_LOWT_GPIO_Port, LED_LOWT_Pin, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(LED_LOWT_GPIO_Port, LED_LOWT_Pin, GPIO_PIN_SET);
    }
}

void update_current_temp(double new_temp) {
    if (current_temp != new_temp) {
        current_temp = new_temp;
        set_current_temp_led();
    }
}

void react_on_steering_wheel_switches(uint8_t const *data) {
  if (data[1] > 0) {
    if (engine_status == ENGINE_STATUS_RUNNING) {
      // 0 - не использовались
      // 1 - махнули один раз (напрмиер вернули с парковки)
      // 2 - пользовались дворниками (дальше не увеличиваем - нет нужды знать точное кол-во)
      if (wipers_was_used < 2) {
        wipers_was_used++;
      }
    }
  }
}

void update_engine_status(uint8_t new_status) {
    if (engine_status != new_status) {
        if (engine_status == ENGINE_STATUS_RUNNING && new_status == ENGINE_STATUS_OFF) {
          // Если дворниками пользовались...
          if (wipers_was_used > 0) {
            // Два раз и более - паркуем
            if (wipers_was_used == 2) {
              request_park();
            }
            // В любом случае сбрасываем счетчик при выключении двигателя
            wipers_was_used = 0;
          }
        }
        engine_status = new_status;
        set_engine_status_led();
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM1) {
        toggle_engine_led();
    } else if (htim->Instance == TIM2) {
        report_vehicle_status_to_uart();
    }
}


