# Full Exercise — UART Command Protocol with ADC Reading

## Overview
Implements a request-response protocol over UART with ADC sensor reading and automatic retransmission on timeout.

## Module Map
```
Lab_5.c (main)
├── UART RX ISR ── buffer accumulation, '#' detection
│
└── Main Loop ──┬── LED heartbeat (500ms toggle)
                ├── command_parser_fsm()    [command_parser_fsm.c]
                │   └── Detect "!RST#" / "!OK#" in buffer
                └── uart_communication_fsm() [uart_communication_fsm.c]
                    ├── State 0: IDLE → wait for RST, read ADC, send response
                    └── State 1: WAIT_ACK → wait for OK or retransmit after 3s
```

## Hardware Connections
| Component | Pin | Description |
|-----------|-----|-------------|
| LED | PA5 | Heartbeat indicator (500ms blink) |
| UART TX | PA2 | USART2 transmit |
| UART RX | PA3 | USART2 receive |
| ADC Input | PA0 | ADC1 Channel 0 (sensor/potentiometer) |

## Protocol Example
```
Terminal → STM32:  !RST#
STM32 → Terminal:  !ADC=2048#
Terminal → STM32:  !OK#
(ready for next request)
```

## Files
| File | Description |
|------|-------------|
| `Lab_5.c` | Main program with UART ISR and peripheral init |
| `Lab_5.hex` | Pre-compiled firmware |
| `Lab_5.pdsprj` | Proteus simulation |
| `Core/Src/command_parser_fsm.c` | Command string parser |
| `Core/Src/uart_communication_fsm.c` | UART communication state machine |
