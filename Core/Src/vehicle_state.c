//
// Created by Oleg Elifantiev on 14.02.2021.
//

#include "vehicle_state.h"
#include "tim.h"
#include "usart.h"

double current_temp = -100.0;
uint8_t engine_status = ENGINE_STATUS_UNKNOWN;

void report_vehicle_status_to_uart() {
    // Transmit engine temp
    // Transmit engine status
}

void set_engine_status_led() {
    if (engine_status == ENGINE_STATUS_IGNITION) {
        HAL_TIM_Base_Start_IT(&htim1);
    } else {
        HAL_TIM_Base_Stop_IT(&htim1);
        if (engine_status == ENGINE_STATUS_OFF || engine_status == ENGINE_STATUS_UNKNOWN) {
            HAL_GPIO_WritePin(LED_ACC_GPIO_Port, LED_ACC_Pin, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(LED_ACC_GPIO_Port, LED_ACC_Pin, GPIO_PIN_RESET);
        }
    }
}

void toggle_engine_led() {
    HAL_GPIO_TogglePin(LED_ACC_GPIO_Port, LED_ACC_Pin);
}

void set_current_temp_led() {
    if (current_temp < 4.0) {
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

void update_engine_status(uint8_t new_status) {
    if (engine_status != new_status) {
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

