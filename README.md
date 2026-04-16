# 🔬 Microprocessors & Microcontrollers — Lab 5

> **STM32F103C6 — UART Communication, ADC Reading & Command Protocol FSM**

| | |
|---|---|
| **Student** | Dang Thai Khang — 2352464 |
| **Course** | C03010 — CC02 |
| **Institution** | Ho Chi Minh University of Technology (HCMUT) |
| **Lecturer** | Phan Van Sy |
| **Semester** | 2025 |

---

## 📌 Overview

This lab implements a **UART-based command-response protocol** on the STM32F103C6. The MCU reads an **ADC sensor value** and communicates it over **UART** using a structured message format. The system uses two FSMs: one for **parsing incoming commands** and one for **managing the communication flow** (with retry/timeout logic).

### Tech Stack

- **MCU:** STM32F103C6 (ARM Cortex-M3)
- **IDE:** STM32CubeIDE (HAL Library)
- **Simulation:** Proteus 8
- **Language:** C
- **Peripherals:** USART2 (9600 baud), ADC1 (Channel 0), GPIO

---

## 📂 Repository Structure

```
VXL_VDK_Lab_5_2352464/
└── FULL_EXERCISE/
    ├── Lab_5.c                        # Main program (UART ISR, ADC init, main loop)
    ├── Lab_5.hex                      # Compiled firmware for Proteus
    ├── Lab_5.pdsprj                   # Proteus schematic & simulation
    └── Core/
        ├── Inc/
        │   ├── command_parser_fsm.h   # Command parser header
        │   └── uart_communication_fsm.h # UART comm FSM header
        └── Src/
            ├── command_parser_fsm.c   # Parses "!RST#" and "!OK#" commands
            └── uart_communication_fsm.c # UART state machine with retry logic
```

---

## 🧪 System Architecture

### Communication Protocol

The system uses a simple **request-response protocol** over UART:

```
PC → STM32:  "!RST#"          (Request ADC reading)
STM32 → PC:  "!ADC=<value>#"  (Respond with ADC value)
PC → STM32:  "!OK#"           (Acknowledge receipt)
```

### Message Format
- All messages start with `!` and end with `#`
- Commands: `!RST#` (request), `!OK#` (acknowledge)
- Response: `!ADC=XXXX#` where XXXX is the raw 12-bit ADC value (0–4095)

---

## 🔄 FSM Diagrams

### Command Parser FSM (`command_parser_fsm.c`)
```
Incoming buffer
    ├── contains "!RST#"  →  command_flag = 1 (request)
    └── contains "!OK#"   →  command_flag = 2 (acknowledge)
```

### UART Communication FSM (`uart_communication_fsm.c`)
```
State 0: IDLE
    │  command_flag == 1 (RST received)
    │  → Read ADC, send "!ADC=value#", start timeout
    ▼
State 1: WAIT_ACK
    ├── command_flag == 2 (OK received) → back to State 0
    └── timeout (3 seconds) → retransmit same ADC value, reset timeout
```

### UART Receive (Interrupt-driven)
```
HAL_UART_RxCpltCallback()
    → Append byte to buffer
    → If byte == '#' → set buffer_flag = 1
    → Re-enable interrupt for next byte
```

---

## 📝 Module Descriptions

### `Lab_5.c` — Main Program
- Initializes UART2 (9600/8N1), ADC1 (Channel 0, continuous), GPIO
- UART RX interrupt fills a 30-byte circular buffer
- Main loop: LED heartbeat (500ms), command parsing, UART FSM

### `command_parser_fsm.c` — Command Parser
- Uses `strstr()` to detect `!RST#` and `!OK#` in the receive buffer
- Sets `command_flag` for the communication FSM to act on

### `uart_communication_fsm.c` — Communication Controller
- **State 0 (IDLE):** Waits for RST command, then reads ADC and sends response
- **State 1 (WAIT_ACK):** Waits for OK acknowledgment, retransmits after 3s timeout

---

## ⚙️ How to Run

### Proteus Simulation
1. Open `FULL_EXERCISE/Lab_5.pdsprj` in **Proteus 8+**
2. Load `Lab_5.hex` into the STM32 component
3. Click **Run** — use the Virtual Terminal to send `!RST#` and `!OK#`

### Flash to Hardware
1. Create STM32F103C6 project in STM32CubeIDE
2. Enable USART2 (9600 baud, TX/RX), ADC1 (Channel 0), GPIO PA5 output
3. Copy source files and build
4. Connect via USB-UART adapter, open serial terminal at 9600 baud

---

## 📝 UART Configuration

| Parameter | Value |
|-----------|-------|
| Baud Rate | 9600 |
| Word Length | 8 bits |
| Stop Bits | 1 |
| Parity | None |
| Flow Control | None |
| Mode | TX + RX (interrupt-driven RX) |

---

## 📄 Report

The full lab report (PDF) with schematics, protocol diagrams, and source code explanations is available in the course submission.

---

## 📜 License

This project is developed for educational purposes as part of the Microprocessors & Microcontrollers course at HCMUT.
