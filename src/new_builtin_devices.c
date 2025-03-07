
// I know this is not the best way to do this, and we can easily support config-strings like Tasmota
// but for now let's use that
#include "new_common.h"
#include "new_pins.h"

void Setup_Device_Empty() {
	PIN_ClearPins();

	PIN_SaveToFlash();

}

// https://www.elektroda.pl/rtvforum/topic3804553.html
// SmartSwitch Tuya WL-SW01_16 16A
void Setup_Device_TuyaWL_SW01_16A() {
	PIN_ClearPins();

	PIN_SetPinRoleForPinIndex(7, IOR_Relay);
	PIN_SetPinChannelForPinIndex(7, 1);

	PIN_SetPinRoleForPinIndex(26, IOR_Button);
	PIN_SetPinChannelForPinIndex(26, 1);

	PIN_SaveToFlash();
}
// https://www.elektroda.pl/rtvforum/topic3822484.html
//  WiFi Tuya SmartLife 4CH 10A
void Setup_Device_TuyaSmartLife4CH10A() {
	PIN_ClearPins();

	PIN_SetPinRoleForPinIndex(7, IOR_Button);
	PIN_SetPinChannelForPinIndex(7, 1);
	PIN_SetPinRoleForPinIndex(8, IOR_Button);
	PIN_SetPinChannelForPinIndex(8, 2);
	PIN_SetPinRoleForPinIndex(9, IOR_Button);
	PIN_SetPinChannelForPinIndex(9, 3);
	PIN_SetPinRoleForPinIndex(1, IOR_Button);
	PIN_SetPinChannelForPinIndex(1, 4);

	PIN_SetPinRoleForPinIndex(14, IOR_Relay);
	PIN_SetPinChannelForPinIndex(14, 1);
	PIN_SetPinRoleForPinIndex(6, IOR_Relay);
	PIN_SetPinChannelForPinIndex(6, 2);
	PIN_SetPinRoleForPinIndex(24, IOR_Relay);
	PIN_SetPinChannelForPinIndex(24, 3);
	PIN_SetPinRoleForPinIndex(26, IOR_Relay);
	PIN_SetPinChannelForPinIndex(26, 4);

	PIN_SaveToFlash();
}
// Tuya "12W" smart light bulb
// "Tuya Wifi Smart Life Light Bulb Lamp E27 LED RGBCW Dimmable For Alexa/Google 18W
// See this topic: https://www.elektroda.pl/rtvforum/viewtopic.php?t=3880540&highlight=
void Setup_Device_BK7231N_TuyaLightBulb_RGBCW_5PWMs() {
	PIN_ClearPins();

	// RGBCW, in that order
	// Raw PWMS (no I2C)

	// P26 - red
	PIN_SetPinRoleForPinIndex(26, IOR_PWM);
	PIN_SetPinChannelForPinIndex(26, 1);

	// P8 - green
	PIN_SetPinRoleForPinIndex(8, IOR_PWM);
	PIN_SetPinChannelForPinIndex(8, 2);

	// P7 - blue
	PIN_SetPinRoleForPinIndex(7, IOR_PWM);
	PIN_SetPinChannelForPinIndex(7, 3);

	// P9 - cold white
	PIN_SetPinRoleForPinIndex(9, IOR_PWM);
	PIN_SetPinChannelForPinIndex(9, 4);

	// P6 - warm white
	PIN_SetPinRoleForPinIndex(6, IOR_PWM);
	PIN_SetPinChannelForPinIndex(6, 5);

	PIN_SaveToFlash();
}
// https://www.elektroda.pl/rtvforum/viewtopic.php?p=19743751#19743751
void Setup_Device_IntelligentLife_NF101A() {
	PIN_ClearPins();

	// TODO: LED

	PIN_SetPinRoleForPinIndex(24, IOR_Relay);
	PIN_SetPinChannelForPinIndex(24, 1);

	PIN_SetPinRoleForPinIndex(6, IOR_Button);
	PIN_SetPinChannelForPinIndex(6, 1);


	PIN_SaveToFlash();
}	
// https://www.elektroda.pl/rtvforum/topic3798114.html
void Setup_Device_TuyaLEDDimmerSingleChannel() {
	PIN_ClearPins();

	// pin 8 has PWM 
	PIN_SetPinRoleForPinIndex(8, IOR_Relay);
	PIN_SetPinChannelForPinIndex(8, 1);

	// button is on RXD2, which is a debug uart..
	PIN_SetPinRoleForPinIndex(1, IOR_Button);
	PIN_SetPinChannelForPinIndex(1, 1);


	PIN_SaveToFlash();
}


void Setup_Device_CalexLEDDimmerFiveChannel() {

	// pins are:
	// red - PWM2 = P7
	// green - PWM3 = P8
	// blue - PWM1 = P6
	// warm white - PWM5 = P26
	// cold white - PWM4 = P24

	PIN_ClearPins();

	// red
	PIN_SetPinChannelForPinIndex(7, 1);
	PIN_SetPinRoleForPinIndex(7, IOR_PWM);
	// green
	PIN_SetPinChannelForPinIndex(8, 2);
	PIN_SetPinRoleForPinIndex(8, IOR_PWM);
	// blue
	PIN_SetPinChannelForPinIndex(6, 3);
	PIN_SetPinRoleForPinIndex(6, IOR_PWM);
	// cold white
	PIN_SetPinChannelForPinIndex(24, 4);
	PIN_SetPinRoleForPinIndex(24, IOR_PWM);
	// warm white
	PIN_SetPinChannelForPinIndex(26, 5);
	PIN_SetPinRoleForPinIndex(26, IOR_PWM);

	PIN_SaveToFlash();
}

void Setup_Device_CalexPowerStrip_900018_1v1_0UK() {

	// pins are:
	// red - PWM2 = P7
	// green - PWM3 = P8
	// blue - PWM1 = P6
	// warm white - PWM5 = P26
	// cold white - PWM4 = P24

	PIN_ClearPins();

	// relays - 4 sockets + 1 USB
	PIN_SetPinChannelForPinIndex(6, 5);
	PIN_SetPinRoleForPinIndex(6, IOR_Relay);
	PIN_SetPinChannelForPinIndex(7, 2);
	PIN_SetPinRoleForPinIndex(7, IOR_Relay);
	PIN_SetPinChannelForPinIndex(8, 3);
	PIN_SetPinRoleForPinIndex(8, IOR_Relay);
	PIN_SetPinChannelForPinIndex(9, 1);
	PIN_SetPinRoleForPinIndex(9, IOR_Relay);
	PIN_SetPinChannelForPinIndex(26, 4);
	PIN_SetPinRoleForPinIndex(26, IOR_Relay);

	// button
	PIN_SetPinChannelForPinIndex(14, 1);
	PIN_SetPinRoleForPinIndex(14, IOR_Button);

	// 2 x LEDs
	// wifi stat
	PIN_SetPinChannelForPinIndex(10, 1);
	PIN_SetPinRoleForPinIndex(10, IOR_LED);
	// power stat
	PIN_SetPinChannelForPinIndex(24, 2);
	PIN_SetPinRoleForPinIndex(24, IOR_LED);

	PIN_SaveToFlash();
}

// https://www.bunnings.com.au/arlec-grid-connect-smart-9w-cct-led-downlight_p0168694
void Setup_Device_ArlecCCTDownlight() {

	// WB3L
	// pins are:
	// cold white - PWM1 = P6
	// warm white - PWM2 = P24

	PIN_ClearPins();

	// cold white
	PIN_SetPinChannelForPinIndex(6, 1);
	PIN_SetPinRoleForPinIndex(6, IOR_PWM);
	// warm white
	PIN_SetPinChannelForPinIndex(24, 2);
	PIN_SetPinRoleForPinIndex(24, IOR_PWM);

	PIN_SaveToFlash();
}

// https://www.elektroda.pl/rtvforum/topic3804553.html
// SmartSwitch Nedis WIFIPO120FWT
void Setup_Device_NedisWIFIPO120FWT_16A() {

	// WB2S
	// Pins are:
	// Led - PWM0 - P6
	// BL0937-CF - PWM1 - P7
	// BL0937-CF1- PWM2 - P8
	// Button - RX1  - P10
	// BL0937-SEL - PWM4 - P24
	// Relay - PWM5 - P26
	

	PIN_ClearPins();
	// LEd
	PIN_SetPinRoleForPinIndex(6, IOR_LED);
	PIN_SetPinChannelForPinIndex(6, 1);
	// Button
	PIN_SetPinRoleForPinIndex(10, IOR_Button);
	PIN_SetPinChannelForPinIndex(10, 1);
	// Relay
	PIN_SetPinRoleForPinIndex(26, IOR_Relay_n);
	PIN_SetPinChannelForPinIndex(26, 1);

	PIN_SaveToFlash();
}

// https://www.elektroda.pl/rtvforum/topic3804553.html
// SmartSwitch Nedis WIFIP130FWT
void Setup_Device_NedisWIFIP130FWT_10A() {

	// WB2S
	// Pins are:
	// Led - PWM0 - P6 
	// Button - RX1  - P10
	// Relay - PWM5 - P26
	

	PIN_ClearPins();
	// Led
	PIN_SetPinRoleForPinIndex(6, IOR_LED);
	PIN_SetPinChannelForPinIndex(6, 1);
	// Button
	PIN_SetPinRoleForPinIndex(10, IOR_Button);
	PIN_SetPinChannelForPinIndex(10, 1);
	// Relay
	PIN_SetPinRoleForPinIndex(26, IOR_Relay);
	PIN_SetPinChannelForPinIndex(26, 1);

	PIN_SaveToFlash();
}

// https://www.elektroda.com/rtvforum/topic3819498.html
// 
void Setup_Device_TH06_LCD_RTCC_WB3S() {

}

// https://www.elektroda.pl/rtvforum/topic3804553.html
// SmartSwitch Emax Home EDU8774 16A 
void Setup_Device_EmaxHome_EDU8774() {

	// WB2S
	// Pins are:
	// BL0937-CF - PWM0 - P6 
	// BL0937-CF1 - PWM1 - P7
	// BL0937-SEL - PWM2 - P8
	// Button - RX1 - P10
	// Relay - PWM4 - P24
	// Led - PWM5 - P26
	

	PIN_ClearPins();
	// Button
	PIN_SetPinRoleForPinIndex(10, IOR_Button);
	PIN_SetPinChannelForPinIndex(10, 1);
	// Relay
	PIN_SetPinRoleForPinIndex(24, IOR_LED_n);
	PIN_SetPinChannelForPinIndex(24, 1);
	// Led
	PIN_SetPinRoleForPinIndex(26, IOR_Relay);
	PIN_SetPinChannelForPinIndex(26, 1);

	PIN_SaveToFlash();
}


// QiachipSmartSwitch
// See teardown article here:
// https://www.elektroda.pl/rtvforum/viewtopic.php?t=3874289&highlight=
void Setup_Device_BK7231N_CB2S_QiachipSmartSwitch() {



	PIN_ClearPins();
	// Button
	PIN_SetPinRoleForPinIndex(7, IOR_Button);
	PIN_SetPinChannelForPinIndex(7, 1);
	// Relay
	PIN_SetPinRoleForPinIndex(8, IOR_Relay);
	PIN_SetPinChannelForPinIndex(8, 1);
	// Led

	PIN_SaveToFlash();
}
void Setup_Device_BK7231T_WB2S_QiachipSmartSwitch() {
       PIN_ClearPins();
       // Button
       PIN_SetPinRoleForPinIndex(7, IOR_Button);
       PIN_SetPinChannelForPinIndex(7, 1);
       // Relay
       PIN_SetPinRoleForPinIndex(6, IOR_Relay_n);
       PIN_SetPinChannelForPinIndex(6, 1);
       // Led
       PIN_SetPinRoleForPinIndex(10, IOR_LED);
       PIN_SetPinChannelForPinIndex(10, 1);

       PIN_SaveToFlash();
}



// Strigona donation
// Teardown article: https://www.elektroda.pl/rtvforum/viewtopic.php?p=19906670#19906670
// https://obrazki.elektroda.pl/6606464600_1642467157.jpg
// NOTE: It used to be ESP-based https://templates.blakadder.com/prime_CCWFIO232PK.html
void Setup_Device_BK7231T_Raw_PrimeWiFiSmartOutletsOutdoor_CCWFIO232PK() {



	PIN_ClearPins();
	// Relay
	PIN_SetPinRoleForPinIndex(6, IOR_Relay);
	PIN_SetPinChannelForPinIndex(6, 1);
	// Relay
	PIN_SetPinRoleForPinIndex(7, IOR_Relay);
	PIN_SetPinChannelForPinIndex(7, 2);
	// Led
	PIN_SetPinRoleForPinIndex(10, IOR_LED);
	PIN_SetPinChannelForPinIndex(10, 1);
	// Led
	PIN_SetPinRoleForPinIndex(26, IOR_LED);
	PIN_SetPinChannelForPinIndex(26, 2);

	// Single button
	PIN_SetPinRoleForPinIndex(24, IOR_Button);
	PIN_SetPinChannelForPinIndex(24, 1);


	PIN_SaveToFlash();
}


// https://www.tokmanni.fi/alypistorasia-home-connect-ip20-6419860720456
// Marked as Smart-PFW02-G
// Relay (with npn-transistor) at PWM4 P24
// Button PWM5 P26
// LED PWM1 P7
void Setup_Device_TuyaSmartPFW02G() {
	PIN_ClearPins();

	PIN_SetPinRoleForPinIndex(24, IOR_Relay_n);
	PIN_SetPinChannelForPinIndex(24, 1);

	PIN_SetPinRoleForPinIndex(26, IOR_Button);
	PIN_SetPinChannelForPinIndex(26, 1);

	PIN_SetPinRoleForPinIndex(7, IOR_LED);
	PIN_SetPinChannelForPinIndex(7, 1);


	PIN_SaveToFlash();
}


void Setup_Device_AvatarASL04() {

	// pins are:
	// red - PWM1 = P24
	// green - PWM2 = P6
	// blue - PWM3 = P8

	// buttons
	// music - P7
	// color - P9
	// on/off - P14

	// IR - P14

	// audio input ???? - most likely P23/ADC?

	PIN_ClearPins();

	// red
	PIN_SetPinChannelForPinIndex(24, 1);
	PIN_SetPinRoleForPinIndex(24, IOR_PWM);
	// green
	PIN_SetPinChannelForPinIndex(8, 2);
	PIN_SetPinRoleForPinIndex(8, IOR_PWM);
	// blue
	PIN_SetPinChannelForPinIndex(6, 3);
	PIN_SetPinRoleForPinIndex(6, IOR_PWM);


	// just set to buttons 1/2/3 for the moment
	PIN_SetPinRoleForPinIndex(7, IOR_Button);
	PIN_SetPinChannelForPinIndex(7, 1);

	PIN_SetPinRoleForPinIndex(9, IOR_Button);
	PIN_SetPinChannelForPinIndex(9, 2);

	PIN_SetPinRoleForPinIndex(14, IOR_Button);
	PIN_SetPinChannelForPinIndex(14, 3);


	PIN_SaveToFlash();
}


void Setup_Device_TuyaSmartWIFISwith_4Gang_CB3S(){
	PIN_ClearPins();

	PIN_SetPinRoleForPinIndex(24, IOR_Button);
	PIN_SetPinChannelForPinIndex(24, 1);
	PIN_SetPinRoleForPinIndex(20, IOR_Button);
	PIN_SetPinChannelForPinIndex(20, 2);
	PIN_SetPinRoleForPinIndex(7, IOR_Button);
	PIN_SetPinChannelForPinIndex(7, 3);
	PIN_SetPinRoleForPinIndex(14, IOR_Button);
	PIN_SetPinChannelForPinIndex(14, 4);

	PIN_SetPinRoleForPinIndex(6, IOR_Relay);
	PIN_SetPinChannelForPinIndex(6, 1);
	PIN_SetPinRoleForPinIndex(8, IOR_Relay);
	PIN_SetPinChannelForPinIndex(8, 2);
	PIN_SetPinRoleForPinIndex(9, IOR_Relay);
	PIN_SetPinChannelForPinIndex(9, 3);
	PIN_SetPinRoleForPinIndex(26, IOR_Relay);
	PIN_SetPinChannelForPinIndex(26, 4);

	PIN_SetPinRoleForPinIndex(22, IOR_LED_WIFI);
	PIN_SetPinChannelForPinIndex(22, 1);

	PIN_SaveToFlash();
}
