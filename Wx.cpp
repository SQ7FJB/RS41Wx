//
// Created by SQ7FJB 08-05-2020
//

#include <stm32f10x_usart.h>
#include <string.h>
#include "Wx.h"
#include "delay.h"
//#include "init.h"
#include "config.h"
#include "ublox.h"
#include "stdio.h"
#include "QAPRSBase.h"

char SendWxBuf[200];

WxEntry currentWxData;

QAPRSBase wxaprs;

void Wx_handle_incoming_byte(uint8_t data){
	//Wx frame Wx:R:data\n\r
	// R                     data
	// 0-get status            0
 	// 1-get position          0
    // 2-send data by APRS     >0

	 static uint8_t sync = 0;
	  static uint8_t buffer_pos = 0;
	  static uint8_t incoming_packet_buffer[sizeof(WxPacket) ];
	  static WxPacket * incoming_packet = (WxPacket *) incoming_packet_buffer;
	  if (!sync){
	    if (!buffer_pos && data == 'W'){
	      buffer_pos = 1;
	      incoming_packet->header.sc1 = data;
	    } else if (buffer_pos == 1 && data == 'x'){
	      //sync = 1;
	      buffer_pos = 2;
	      incoming_packet->header.sc2 = data;
	    } else if (buffer_pos == 2 && data == ':'){
		  //sync = 1;
		  buffer_pos = 3;
		  incoming_packet->header.sc3 = data;
	    } else if (buffer_pos == 3 ){
	     //sync = 1;
		  buffer_pos = 4;
		  incoming_packet->header.messageType = data;
	    } else if (buffer_pos == 4 && data == ':'){
	      sync = 1;
		  buffer_pos = 5;
		  incoming_packet->header.sc4 = data;
	    }else {
	      buffer_pos = 0;
	    }
	  } else {((uint8_t *)incoming_packet)[buffer_pos] = data;
	    if ((buffer_pos >= WxDataSize-1) || (data == '\r') || (data == '\n')){
	    	((uint8_t *)incoming_packet)[buffer_pos] = 0x0;
	    	Wx_handle_packet((WxPacket *) incoming_packet);

	      buffer_pos = 0;
	      sync = 0;
	    } else {
	      buffer_pos++;
	      if (buffer_pos >= sizeof(WxPacket) ) {
	        buffer_pos = 0;
	        sync = 0;
	      }
	    }
	  };
}


void Wx_handle_packet(WxPacket *ptk){
	//if(ptk->header == '0')
	currentWxData.new_frame = 1;
	currentWxData.order = ptk->header.messageType;
	memcpy(&currentWxData.frame, ptk->data, WxDataSize);
};


void _sendWxByte(uint8_t message) {
  while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) {
  }
   USART_SendData(USART3, message);
   while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) {
  }
}

void send_Wx(char *data) {
	while (*data !=0){
	_sendWxByte(*(data++));

	}

}
void Wx_get_last_data(WxEntry * wxEntry){
   __disable_irq();
   memcpy(wxEntry, &currentWxData, sizeof(currentWxData));
   currentWxData.new_frame =0;
   __enable_irq();
}

void Wx_send_status(int Uz){
	sprintf(SendWxBuf,"Status Uz:%d  ok",Uz);
	send_Wx(SendWxBuf);
}

void Wx_send_position(GPSEntry *gpsData){
	int32_t lat;
	int32_t lon;
	int32_t alt;
	int32_t speed;
	int course;
	int fix;
	//char buf[100];
	lat =gpsData->lat_raw/10;
	lon = gpsData->lon_raw/10;
	alt = (gpsData->alt_raw/1000) * 3280 / 1000;
	speed = (gpsData->speed_raw)*0.036;
	course = (gpsData->course)/100000;
	fix = gpsData->fix;
	sprintf(SendWxBuf,"GPS:%06ld,%06ld,%06ld,%03ld,%03ld,%d\n\r",lat,lon,alt,speed,course,fix);
	send_Wx(SendWxBuf);
}



//void Wx_send_aprs(char *data){
//	wxaprs.init(0, 0, (char *) APRS_CALLSIGN, (const uint8_t) APRS_SSID, (char *) "APZQAP", '0', (char *) "WIDE1-1,WIDE2-1");
//	wxaprs.sendData(data);
//}
