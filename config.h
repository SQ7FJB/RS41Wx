//
// Created by SQ5RWU on 2016-12-24.
// Modified by SQ9MDD on 2019.05.09-14
//
// **************************** config ***************************
// callsign icon and comment
#define APRS_CALLSIGN 					"SQ7FJB" 					// put your APRS callsign here, 6 characters. If your callsign is shorter add spaces
#define APRS_SSID 						'B' 						// put your APRS SSID here allowed numbers are from 0-F (SSID 0-15)
#define APRS_TABL 						"/"							// icon table look at: http://www.aprs.org/symbols/symbolsX.txt
#define APRS_ICON						"["							// icon symbol
#define APRS_COMMENT 					" testuje trakera"			// comment
//
// tactical name icon and comment
#define APRS_USE_TACTICAL_CALLSIGN		1							// enable/disable tactical callsign insted callsign, remember you sended this using primary callsign and SSID
#define APRS_TACTICAL_CALLSIGN			"ABRA"						// optional tactical name for your item up 9 characters (This is an ITEM from APRS spec. in this case my pet dog's name)
#define APRS_TACTICAL_TBL				"/"							// icon table for tactical call sign look at: http://www.aprs.org/symbols/symbolsX.txt
#define APRS_TACTICAL_ICON				"p"							// icon symbol for tacti
#define APRS_TACTICAL_COMMENT			"pies na spacerze"			// comment for tactical
//
// path and TX intervals
#define APRS_PATH						""							// APRS DIGI PATH use WIDE1-1 or WIDE1-1,WIDE2-1 or RFONLY, whatever you want
#define APRS_PROPORTIONAL_PATH 			0							// enable proportional path algorythm this option override main path see: http://www.aprs.org/newN/ProportionalPathing.txt
#define APRS_ALT_REDUCE_PATH			0							// reduce path on altitude, below normal on the ground NONE above APRS_ALT_LIMIT
#define APRS_ALT_LIMIT					1000						// reduce path above this parameter (in meters)
#define APRS_INTERVAL					10							// number of seconds between packets, DO NOT SETUP LESS THAN 60s
//
// sending optional info
#define APRS_SEND_ALT					0							// enable/disable sending altitude (extending APRS data)
#define APRS_SEND_SPD					1							// enable/disable sending course and speed (extending APRS data)
#define APRS_SEND_SAT					0							// enable/disable sending SAT condition (@ end of the comment)
//
// smart bikon section
#define APRS_SMARTBIKON					1							// enable/disable smart bikoning option
#define APRS_SB_FAST_RATE				30							// seconds default 30s
#define APRS_SB_LOW_RATE				600							// seconds default 10m
#define APRS_SB_FAST_SPEED				90							// km/h (max speed = fast rate)
#define APRS_SB_LOW_SPEED				5							// km/h (min speed = low rate)
#define APRS_SB_TURN_ANGLE				30							// turn angle treshold
//
// radio setup
#define ENABLE_TX						1							// enable/disable TX
#define APRS_FREQUENCY  				432.500f 					// Mhz middle frequency usually 432.500MHz
#define TX_POWER  						0 							// PWR 0...7 0- MIN ... 7 - MAX
#define ALLOW_DISABLE_BY_BUTTON 		1 							// 1- enable power off button


// communication with SQ9MDD Wx-station
// WxEnable 0 -> RS41 work like APRS tracker
// WxEnable 1 -> RS41 send via radio (ax25) dates received via Com3. Command : Wx:2:xxxxx\n\r xxxx- dates to send via rs41-> example "!5200.01N/02011.58E_167/000g002t056h71b10230V115oDtz Dust: 15.7 ug/m3 WXDomoPy"
// Com setup: 19200 8 n 1													   Wx:0:\n\r - get status
//																			   Wx:1:\n\r - get actual position from  internal GPS receiver
#define WxEnable 1
#define APRS_Wx_CALLSIGN 					"SR7WX " 					// put your APRS callsign here, 6 characters. If your callsign is shorter add spaces
#define APRS_Wx_SSID 						'1' 						// put your APRS SSID here allowed numbers are from 0-F (SSID 0-15)



// !!!!! PLEASE FOLLOW THE RULES !!!!!

// SSID:
// 0 --> Your primary station usually fixed and message capable
// 1 --> generic additional station, digi, mobile, wx, etc.
// 2 --> generic additional station, digi, mobile, wx, etc.
// 3 --> generic additional station, digi, mobile, wx, etc.
// 4 --> generic additional station, digi, mobile, wx, etc.
// 5 --> Other network source s (Dstar, Iphones, Blackberry's etc)
// 6 --> Special activity, Satellite ops, camping or 6 meters, etc.
// 7 --> walkie talkies, HT's or other human portable
// 8 --> boats, sailboats, RV's or second main mobile
// 9 --> Primary Mobile (usually message capable)
// A (10) --> internet, Igates, echolink, winlink, AVRS, APRN, etc.
// B (11) --> balloons, aircraft, spacecraft, etc.
// C (12) --> APRStt, DTMF, RFID, devices, one-way trackers*, etc.
// D (13) --> Weather stations
// E (14) --> Truckers or generally full time drivers
// F (15) --> generic additional station, digi, mobile, wx, etc.
//
// APRS DIGI recommended path:
// mobile: WIDE1-1,WIDE2-1
// fixed: WIDE2-1
// flying objects: no path leave: ""
//
// Power levels:
// 0 --> -1dBm
// 1 --> 2dBm
// 2 --> 5dBm
// 3 --> 8dBm
// 4 --> 11dBm
// 5 --> 14dBm
// 6 --> 17dBm
// 7 --> 20dBm
