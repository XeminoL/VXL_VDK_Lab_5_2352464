#include "command_parser_fsm.h"
#include "string.h"

int command_flag = 0;

void command_parser_fsm(uint8_t *buffer){
  if (strstr((char*)buffer, "!RST#")){
    command_flag = 1;
  }
  else if (strstr((char*)buffer, "!OK#")){
    command_flag = 2;
  }
}
