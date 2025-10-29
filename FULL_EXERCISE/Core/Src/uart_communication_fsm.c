#include "uart_communication_fsm.h"
#include "stdio.h"
#include "string.h"

extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;
extern int command_flag;

int uart_state = 0;
uint32_t last_time = 0;
uint32_t ADC_value;
char str[50];

void uart_communication_fsm(void){
    switch(uart_state){
        case 0: // IDLE
            if(command_flag == 1){ // !RST#
                command_flag = 0;
HAL_ADC_Start(&hadc1);
HAL_ADC_PollForConversion(&hadc1, 10);
ADC_value = HAL_ADC_GetValue(&hadc1);
HAL_ADC_Stop(&hadc1);
sprintf(str, "!ADC=%lu#\r\n", ADC_value);
HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), 100);

                last_time = HAL_GetTick();
                uart_state = 1;
            }
            break;

        case 1: // WAIT_OK
            if(command_flag == 2){ // !OK#
                command_flag = 0;
                uart_state = 0;
            }
            else if (HAL_GetTick() - last_time > 3000){ // timeout
                HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), 100);
                last_time = HAL_GetTick();
            }
            break;
    }
}
