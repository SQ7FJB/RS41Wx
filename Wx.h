#ifndef STM32_Wx_H
#define STM32_Wx_H


#include "ublox.h"

#define send_status   '0'
#define send_position '1'
#define send_aprs	  '2'

typedef struct  __attribute__((packed)){
  uint8_t sc1; // 'W'
  uint8_t sc2; // 'x'
  uint8_t sc3; // ':'
  uint8_t messageType; //0-get status
  	  	  	  	  	   //1-get position
  	  	  	  	  	   //2-send data by APRS
  uint8_t sc4; //':'
} WxHeader;

#define WxDataSize 100

typedef struct {
  int8_t new_frame;
  int8_t order;
  int8_t frame[WxDataSize];
  } WxEntry;




typedef struct __attribute__((packed)){
  WxHeader header;
  //char newPacet;
  char data[WxDataSize];
} WxPacket;

#ifdef __cplusplus
extern "C" {
#endif

void Wx_handle_incoming_byte(uint8_t data);
void Wx_handle_packet(WxPacket *ptk);

void send_Wx(char *data);
void Wx_get_last_data(WxEntry * wxEntry);
void Wx_send_status(int Uz);
void Wx_send_position(GPSEntry *gpsData);

//void Wx_send_aprs(char *data);

#ifdef __cplusplus
};
#endif


#endif //STM32_Wx_H
