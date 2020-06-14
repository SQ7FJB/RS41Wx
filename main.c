// STM32F100 and SI4032 APRS TRACKER with WX
// released under GPL v.2 by SQ7FJB SQ5RWU SQ9MDD
// enjoy and have a nice day
// ver 1.5a
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_spi.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_adc.h>
#include <stm32f10x_rcc.h>
#include "stdlib.h"
#include <stdio.h>
#include <string.h>
#include <misc.h>
#include "init.h"
#include "config.h"
#include "radio.h"
#include "ublox.h"
#include "delay.h"
#include "aprs.h"
#include "Wx.h"
///////////////////////////// test mode /////////////
static __IO uint32_t DelayCounter;
volatile uint32_t last_sended = 0;
volatile interval = APRS_INTERVAL;
volatile int last_course = 0;
volatile int last_fix = 0;
volatile int current_fix = 0;

//int8_t WxTx, newWxTx;


//const unsigned char test = 0; // 0 - normal, 1 - short frame only cunter, height, flag

#define GREEN  GPIO_Pin_7
#define RED  GPIO_Pin_8

unsigned int send_cun;        //frame counter

char status[2] = {'N'};
int voltage;
int Ubat;
volatile char flaga = 0;

volatile unsigned int cun = 10;
volatile unsigned char tx_on = 0;
volatile unsigned int off_delay;
volatile unsigned char tx_enable = 0;
volatile unsigned int stat_sended = 0;

volatile uint16_t button_pressed = 0;
volatile uint8_t disable_armed = 0;
volatile unsigned int flexible_delay = 60;
#define OFF	GPIO_Pin_12
int Button=0;
volatile uint8_t Key_ok = 0;



//Power off
void power_off(void){
	 if (ALLOW_DISABLE_BY_BUTTON && Key_ok)
	  	  	  {


	  	  		if ((ADCVal[1] - Button ) >= 200)
	  	  					{
	  	  					if (++button_pressed >= 3)
	  	  						{
	  	  						disable_armed = 1;
	  	  						GPIO_SetBits(GPIOB, RED);
	  	  					    GPIO_SetBits(GPIOB, GREEN);
	  	  					    button_pressed = 2;
	  	  						}
	  	  					}
	  	  				else
	  	  					{
	  	  					Button = ADCVal[1];
	  	  				  	button_pressed = 0;
	  	  					if (disable_armed)
	  	  						{
	  	  					    disable_armed = 1;
	  	  				        GPIO_SetBits(GPIOA, OFF);

	  	  						}

	  	  					}
	  	  	   }
	  		}



/**
 * GPS data processing
 */
void USART1_IRQHandler(void) {
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
    ublox_handle_incoming_byte((uint8_t) USART_ReceiveData(USART1));
      }else if (USART_GetITStatus(USART1, USART_IT_ORE) != RESET) {
    USART_ReceiveData(USART1);
  } else {
    USART_ReceiveData(USART1);
  }
}


void USART3_IRQHandler(void) {
  if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {


	Wx_handle_incoming_byte((uint8_t) USART_ReceiveData(USART3));
      }else if (USART_GetITStatus(USART3, USART_IT_ORE) != RESET) {
    USART_ReceiveData(USART3);
  } else {
    USART_ReceiveData(USART3);
  }
}


void TIM2_IRQHandler(void) {
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    if (aprs_is_active()){
      aprs_timer_handler();
    }
    if (++off_delay >= 150)
        {
        	off_delay = 0;
           	power_off();
        }
  }
}

int main(void) {
  RCC_Conf();
  NVIC_Conf();
  init_port();

  delay_init();
  ublox_init();

  GPIO_SetBits(GPIOB, RED);
  if(WxEnable){
   send_Wx("\n\rWx mode!!!\n\r");
  }else{
   send_Wx("\n\rTracker mode!!!\n\r");
  }
  radio_soft_reset();
  // setting TX frequency
  radio_set_tx_frequency(APRS_FREQUENCY);

  // setting TX power
  radio_rw_register(0x6D, 00 | (TX_POWER & 0x0007), 1);

  radio_rw_register(0x71, 0x00, 1);
  radio_rw_register(0x12, 0x20, 1);
  radio_rw_register(0x13, 0x00, 1);
  radio_rw_register(0x12, 0x00, 1);
  radio_rw_register(0x0f, 0x80, 1);
  tx_on = 0;
  tx_enable = 1;

  aprs_init();
  Button =ADCVal[1];
  Key_ok = 1;
  while (1) {
	  Ubat = srednia(ADCVal[0] * 600 / 4096);

	  // podciagamy dane z GPS-a
	     GPSEntry gpsData;
	     ublox_get_last_data(&gpsData);


	  //Sprawdzamy czy s¹ nowe dane z Wx
	  	  WxEntry WxData;
	  Wx_get_last_data(&WxData);
	  //send_Wx(WxData.new_frame+0x30);

	  if  (WxData.new_frame)
	  {

		  if(WxData.order == send_status){
			  Wx_send_status(Ubat);

		  }else if(WxData.order == send_position){
			  Wx_send_position(&gpsData);

		  }else if (WxData.order == send_aprs){
			  if(WxEnable){
			  send_Wx("Sending APRS\n\r");
			  send_Wx(WxData.frame);
			  USART_Cmd(USART1, DISABLE);
			  GPIO_SetBits(GPIOB, GREEN);
			  GPIO_ResetBits(GPIOB, RED);
			  if(ENABLE_TX == 1){
			   	radio_enable_tx();
			    }
			  //Wx_send_aprs(">NO FIX");//WxData.frame);
			  //Example: "!5215.01N/02055.58E_167/000g002t056h71b10230V115oDtz Dust: 15.7 ug/m3 WXDomoPy"
			  Wx_send_aprs(WxData.frame);
		  //aprs_send_status();
			  if(ENABLE_TX == 1){
			  	radio_disable_tx();
			  }

	          USART_Cmd(USART1, ENABLE);
	          GPIO_SetBits(GPIOB, RED);
	          GPIO_ResetBits(GPIOB, GREEN);
             _delay_ms(100);
			  }
			  else{
				  send_Wx("\n\rWx-mode disabled\n\r");
			  }

		  }else if (WxData.order == '4'){
			  send_Wx("Set PA7 i PB9\n\r");
			  GPIO_SetBits(GPIOB, GPIO_Pin_9);
			  GPIO_SetBits(GPIOA, GPIO_Pin_7);
		  }else if (WxData.order == '5'){
			  send_Wx("Reset PA7 i PB9\n\r");
	    	  GPIO_ResetBits(GPIOB, GPIO_Pin_9);
		      GPIO_ResetBits(GPIOA, GPIO_Pin_7);

		  } else{

		      send_Wx("ok\n\r");
		  }
	  }




    	int predkosc = (gpsData.speed_raw)*0.036;
        int current_course = (gpsData.course)/100000;
        current_fix = gpsData.fix;
       if(!WxEnable){
        // zmiana timingu w zaleznosci od wybranego trybu smartbikon or not
    	if(APRS_SMARTBIKON == 0){
    		interval = APRS_INTERVAL;	//sta³y timing
    	}
    	else{
            // zmiana czestotliwosci nadawania ramki na podstawie predkosci

            if(predkosc < APRS_SB_LOW_SPEED){
            	flexible_delay = APRS_SB_LOW_RATE;
            } else if(predkosc > APRS_SB_LOW_SPEED && predkosc < APRS_SB_FAST_SPEED){
            	// return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
            	// in  = APRS_SB_LOW_SPEED, APRS_SB_FAST_SPEED
            	// out = APRS_SB_LOW_RATE, APRS_SB_FAST_RATE
            	flexible_delay = (predkosc - APRS_SB_LOW_SPEED) * (APRS_SB_FAST_SPEED - APRS_SB_LOW_SPEED) / (APRS_SB_FAST_SPEED - APRS_SB_LOW_SPEED) + APRS_SB_LOW_RATE;
            } else if(predkosc > APRS_SB_FAST_SPEED){
            	flexible_delay = APRS_SB_FAST_RATE;
            }


    		interval = flexible_delay;	//((flexible_delay*1000)-1000);

        	// przeliczanie zmian kata kursu uwaga na przejscie 359<->0
        	// uzaleznic od minimalnej predkosci bo bedzie dryf na postoju

        	int delta_course = abs(last_course-current_course);
        	if(last_course > (359 - APRS_SB_TURN_ANGLE) && current_course < (0 + APRS_SB_TURN_ANGLE)){
        		delta_course = 359 - delta_course;
        	}
        	if(last_course  < (0 + APRS_SB_TURN_ANGLE) && current_course > (359 - APRS_SB_TURN_ANGLE)){
        		delta_course = 359 - delta_course;
        	}
        	// jesli zmiana kursu wieksza niz podana w konfigu wyslij ramke za 10sec
        	if(delta_course > APRS_SB_TURN_ANGLE && predkosc > APRS_SB_LOW_SPEED && interval > 10){
        		interval = 10;
        	}
        	last_course = current_course;
    	}

    	//odliczanie do wysylki jesli warunek spelniony wyslij ramke
        if( (gpsData.licznik_sekund > (last_sended+interval) ) || (last_fix == 0 && current_fix == 3)){
        last_fix = 1;
        last_sended = gpsData.licznik_sekund;


        USART_Cmd(USART1, DISABLE);

        GPIO_SetBits(GPIOB, GREEN);
        GPIO_ResetBits(GPIOB, RED);
        if(ENABLE_TX == 1){
        	radio_enable_tx();
        }

        //tutaj zlecamy wysylka ramki
        if(gpsData.fix < 3){	//jesli nie ma fixa i juz czas to wysylaj status
        	aprs_send_status();
        	stat_sended = 1;
        }else{
        	if(stat_sended == 1){ //jesli zlapal fixa a wczesniej wysylal status no fix to teraz wyslij status FIX OK a pozniej juz normalne ramki
        		aprs_send_status_ok();
        		stat_sended = 0;
        	}
        	aprs_send_position(gpsData);
        }
        if(ENABLE_TX == 1){
        	radio_disable_tx();
        }

        USART_Cmd(USART1, ENABLE);
        GPIO_SetBits(GPIOB, RED);
        GPIO_ResetBits(GPIOB, GREEN);
        _delay_ms(100);
    }
   }
  }
}

/*  od SQ6QV
 * void radio_enable_direct_mode(){

	GPIO_SetBits(GPIOC, radioNSELpin);
	radio_rw_register(0x6D, 0b00000111, 1);//power
	//Direct Mode using TX Data via SDI pin (only when nSEL is high)
	radio_rw_register(0x6e, 38, 1);//txdr
	radio_rw_register(0x6F, 28, 1); //txdr
	radio_rw_register(0x70, 28, 0b00100000);//modulation mode control //txdr scale

	radio_rw_register(0x72, 2, 1); //frequency deviation
	radio_rw_register(0x71, 0b10010011, 1);//modulation control GFSK

	//radio_rw_register(0x72, 5, 1); //frequency deviation
	//radio_rw_register(0x71, 0b00010010, 1);//modulation control FSK
 *
 *
 */

