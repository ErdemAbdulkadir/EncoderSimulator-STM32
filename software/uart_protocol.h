#ifndef UART_PROTOCOL_H
#define UART_PROTOCOL_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define UART_SOP  0xAE
#define BAUD_RATE 115200

typedef enum UartReqPacKey_e{
    UartReqPack_EncoderConfig = 0,
    UartReqPack_EncoderDepth,
    UartReqPack_EncoderStatus,
    UartReqPack_Debug,
}UartReqPackKey_t;

typedef enum UartResPackKey_e{
    UartResPack_EncoderConfig = 0,
    UartResPack_EncoderDepth,
    UartResPack_EncoderStatus,
    UartResPack_Debug,
}UartResPackKey_t;

typedef enum encoderSpeed_s{
    encoderSpeedNone = 0,
    encoderSpeedLow = 88,
    encoderSpeedMedium = 44,
    encoderSpeedHigh = 22,
}encoderSpeed_t;

typedef enum encoderDirection_s{
    encoderDirectionNone = 0,
    encoderDirectionUp,
    encoderDirectionDown,
}encoderDirection_t;

typedef enum encoderStatus_s{
    encoderStatusStart,
    encoderStatusStop,
}encoderStatus_t;

// UartReqPacket
#pragma pack(push, 1)
typedef struct ReqPackEncoderConfig_s{
    uint8_t  sop;
    uint8_t  key;
    uint8_t  seq;
    uint16_t resolution;
    uint8_t  direction;
    uint8_t  encoderSpeed;
    uint8_t  reserved[23];
    uint16_t crc;
}ReqPackEncoderConfig_t;

typedef struct ReqPackEncoderDepth_s{
    uint8_t  sop;
    uint8_t  key;
    uint8_t  seq;
    uint32_t depth;
    uint16_t speed;
    uint8_t  reserved[21];
    uint16_t crc;
}ReqPackEncoderDepth_t;

typedef struct ReqPackEncoderStatus_s{
    uint8_t  sop;
    uint8_t  key;
    uint8_t  seq;
    uint8_t  encoderStatus;
    uint8_t  reserved[26];
    uint16_t crc;
}ReqPackEncoderStatus_t;

typedef struct ReqPackDebug_s{
    uint8_t sop;
    uint8_t key;
    uint8_t seq;
    uint8_t msgLen;
    uint8_t msg[24];
    uint8_t reserved[2];
    uint16_t crc;
}ReqPackDebug_t;

// UartResPacket

typedef struct ResPackEncoderConfig_s{
    uint8_t  sop;
    uint8_t  key;
    uint8_t  seq;
    uint16_t resolution;
    uint8_t  direction;
    uint8_t  encoderSpeed;
    uint8_t  reserved[23];
    uint16_t crc;
}ResPackEncoderConfig_t;

typedef struct ResPackEncoderDepth_s{
    uint8_t  sop;
    uint8_t  key;
    uint8_t  seq;
    int32_t  depth;
    uint16_t speed;
    uint8_t  reserved[21];
    uint16_t crc;
}ResPackEncoderDepth_t;

typedef struct ResPackEncoderStatus_s{
    uint8_t  sop;
    uint8_t  key;
    uint8_t  seq;
    uint8_t  encoderStatus;
    uint8_t  reserved[26];
    uint16_t crc;
}ResPackEncoderStatus_t;

typedef struct ResPackDebug_s{
    uint8_t sop;
    uint8_t key;
    uint8_t seq;
    uint8_t msgLen;
    uint8_t msg[24];
    uint8_t reserved[2];
    uint16_t crc;
}ResPackDebug_t;

typedef struct ResPackFlag_s{
    uint8_t resEncoderConfigFlag;
    uint8_t resEncoderDepthFlag;
    uint8_t resEncoderStatusFlag;
}ResPackFlag_t;

//typedef struct ReqPackFlag_s{
//	uint8_t reqEncoderConfigFlag;
//	uint8_t reqEncoderDepthFlag;
//	uint8_t reqEncoderStatusFlag;
//
//};

typedef struct uart_s{
    ReqPackEncoderConfig_t ReqPackEncoderConfig;
    ReqPackEncoderDepth_t  ReqPackEncoderDepth;
    ReqPackEncoderStatus_t ReqPackEncoderStatus;
    ReqPackDebug_t         ReqPackDebug;
    ResPackEncoderConfig_t ResPackEncoderConfig;
    ResPackEncoderDepth_t  ResPackEncoderDepth;
    ResPackEncoderStatus_t ResPackEncoderStatus;
    ResPackDebug_t         ResPackDebug;
}uart_t;

#pragma pack(pop)
uint16_t crc16_modbus(const uint8_t* data, uint32_t len);
void crc_fill(uint8_t* data, uint32_t len);
uint8_t crc_is_valid(uint8_t* data, uint32_t len);
uint8_t* uartReqPackKey(UartReqPackKey_t key);
uint8_t* uartResPackKey(UartResPackKey_t key);
size_t  sizeOfUartReqPack(UartReqPackKey_t key);
size_t  sizeOfUartResPack(UartResPackKey_t key);
extern  ResPackFlag_t  resPackFlag;
extern  uart_t Uart;
#endif // UART_PROTOCOL_H
