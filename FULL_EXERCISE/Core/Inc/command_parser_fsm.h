#ifndef __COMMAND_PARSER_FSM_H
#define __COMMAND_PARSER_FSM_H

#include "main.h"
#include <stdint.h>

extern int command_flag;    // 0: none, 1: RST, 2: OK

void command_parser_fsm(uint8_t *buffer);

#endif /* __COMMAND_PARSER_FSM_H */
