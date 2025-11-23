# Encoder Simulator – STM32F429 + Qt Interface
## Overview

Testing an encoder-driven motor in an office environment generates significant mechanical noise, which can disturb people working nearby.

This project was developed to generate encoder signals electronically and silently, allowing hardware and software verification without operating a physical motor.

The STM32F429-Discovery board produces the encoder signals using PWM, while the Qt-based GUI displays depth, speed, direction, and debug messages in real time.


## Features
### STM32 (Firmware)

- ⚙️ Quadrature encoder signal generation
- 🔼 TIM1 → Phase A (Up)
- 🔽 TIM8 → Phase B (Down)
- 🔄 90° phase shift between signals
- 🎚️ PWM-based signal generation
- ⏱️ Depth & speed sent every 100 ms
- 🔌 USART2 communication (115200 baud)
- ✔️ CRC16-validated packet structure
- 🛠️ GUI-configurable: resolution, direction, speed
- 📝 Debug messages with [STM] prefix


## Timer and PWM Structure

### Timer Clock
TIM1 and TIM8: 180 MHz 

### PWM Settings

ARR (Auto-Reload): 999 (0–999 = 1000 steps)

Preload: Enabled

Duty Cycle: 50%

Prescaler: Selected through the GUI

### Speed (Prescaler) Profiles
typedef enum encoderSpeed_s { 

    encoderSpeedNone   = 0, 

    encoderSpeedLow    = 88, 

    encoderSpeedMedium = 44, 

    encoderSpeedHigh   = 22, 

} encoderSpeed_t; 


| Profile | PSC Value |
|---------|-----------|
| Low     | 88        |
| Medium  | 44        |
| High    | 22        |


### Encoder Signal Pins

| Signal        | Timer | Channel | Pin  |
|---------------|--------|---------|------|
| Phase A (Up)   | TIM1  | CH1     | PE9  |
| Phase B (Down) | TIM8  | CH1     | PC6  |


Both signals operate in PWM mode and have a 90° phase shift between them.
  

### UART (USART2) Pins

| Function | Pin  |
|----------|------|
| TX       | PD5  |
| RX       | PD6  |


Serial Port Settings

| Setting      | Value                 |
|--------------|------------------------|
| Baud Rate    | 115200                |
| Data Bits    | 8                     |
| Parity       | None                  |
| Stop Bits    | 1                     |
| Frame Format | 8N1                   |



Data Sent from STM32 to Qt Every 100 ms

Depth

Speed

## Encoder Configuration (GUI)

Encoder Simulator GUI Image
   
<img width="572" height="440" alt="guı" src="https://github.com/user-attachments/assets/f0f22892-b158-4ccc-8667-f0296bef86cd" />


               
## The Following Settings Can Be Adjusted in the Qt Application


- 🎚️ Encoder resolution can be entered
- 🔄 Direction can be selected: **Up / Down**
- ⚡ Speed profile: **Low / Medium / High**
- ▶️⏸️ Start / Stop controls the encoder
- ♻️ **Encoder Depth Reset** resets depth on STM32
- 📝 Log panel shows real-time debug messages
- 📊 Speed field shows the real-time encoder speed
- 🔌 Port selection + **Connect / Disconnect**

## Encoder Signal Output (Logic Analyzer)
![encoderSımGuı](https://github.com/user-attachments/assets/4764ed32-0987-4cb0-b02f-0333473d3fcf)

  
### Debug Messages
[STM] : Encoder started

[QT]  : Connected

### Future Improvements

GUI installer package
