



#include "new_pins.h"
#include "httpserver/new_http.h"
#include "logging/logging.h"
#include "new_cmd.h"


//According to your need to modify the constants.
#define BTN_TICKS_INTERVAL    5	//ms
#define BTN_DEBOUNCE_TICKS    3	//MAX 8
#define BTN_SHORT_TICKS       (300 /BTN_TICKS_INTERVAL)
#define BTN_LONG_TICKS        (1000 /BTN_TICKS_INTERVAL)

typedef enum {
	BTN_PRESS_DOWN = 0,
	BTN_PRESS_UP,
	BTN_PRESS_REPEAT,
	BTN_SINGLE_CLICK,
	BTN_DOUBLE_CLICK,
	BTN_LONG_RRESS_START,
	BTN_LONG_PRESS_HOLD,
	BTN_number_of_event,
	BTN_NONE_PRESS
}BTN_PRESS_EVT;


typedef void (*new_btn_callback)(void*);

typedef struct pinButton_ {
	uint16_t ticks;
	uint8_t  repeat : 4;
	uint8_t  event : 4;
	uint8_t  state : 3;
	uint8_t  debounce_cnt : 3; 
	uint8_t  active_level : 1;
	uint8_t  button_level : 1;
	
	uint8_t  (*hal_button_Level)(void *self);
	new_btn_callback  cb[BTN_number_of_event];
}pinButton_s;


#if WINDOWS

#elif PLATFORM_XR809

// XR809 sysinfo is used to save configuration to flash
#include "common/framework/sysinfo.h"
#include "driver/chip/hal_gpio.h"

#define os_memcpy memcpy

#else
#include "flash_config/flash_config.h"
#include <gpio_pub.h>

#include "../../beken378/func/include/net_param_pub.h"
#include "../../beken378/func/user_driver/BkDriverPwm.h"
#include "../../beken378/func/user_driver/BkDriverI2c.h"
#include "../../beken378/driver/i2c/i2c1.h"
#include "../../beken378/driver/gpio/gpio.h"
#undef PR_DEBUG
#undef PR_NOTICE


typedef struct item_pins_config
{
	INFO_ITEM_ST head;
	pinsState_t pins;
}ITEM_PINS_CONFIG,*ITEM_PINS_CONFIG_PTR;

void testI2C()
{
	bk_i2c_device_t def;

	def.address = 0xAA;
	def.address_width = I2C_ADDRESS_WIDTH_7BIT;
	def.speed_mode = I2C_STANDARD_SPEED_MODE;

	BkI2cInitialize(&def);


}
int TC74_readTemp_method1(int adr)
{
	bk_i2c_device_t def;
	byte buffer[9];
	int t;


	def.address = adr;
	def.address_width = I2C_ADDRESS_WIDTH_7BIT;
	def.speed_mode = I2C_STANDARD_SPEED_MODE;

	BkI2cInitialize(&def);

 // I2C1_start();
  //I2C1_Wr(adr);  // Status = 0 if got an ACK
 // I2C1_Wr(0x00);  // Status = 0 if got an ACK
  //I2C1_Repeated_Start();  // issue I2C signal repeated start
 // I2C1_Wr(adr+1);  // Status = 0 if got an ACK
 // delay_ms(10);
 // t = I2C1_Rd(1);
 // I2C1_stop();

	return t;

}


///*/*/*int TC74_readTemp_method2(int adr)
//{
//	byte buffer[9];
//	int t;
//    uint32_t status;
//	uint32_t oflag;
//    oflag = I2C_DEF_DIV;
//
//	//i2c1_init();
//	i2c_hdl = ddev_open("i2c1", &status, oflag);
//    if(DD_HANDLE_UNVALID == i2c_hdl){
//		addLogAdv(LOG_INFO, LOG_FEATURE_I2C,"TC74_readTemp_method2 ddev_open failed, status %i!\n",status);
//		return -1;
//	}
//
//		addLogAdv(LOG_INFO, LOG_FEATURE_I2C,"TC74_readTemp_method2 ddev_open OK!\n");
//
//	//I2C1_start();
//	//i2c1_open(0);
//	//I2C1_Wr(adr);  // Status = 0 if got an ACK
//	//I2C1_Wr(0x00);  // Status = 0 if got an ACK
//	buffer[0] = adr;
//	buffer[1] = 0x00;
//	camera_intf_sccb_write(adr,0x00);
//	//ddev_write(i2c_hdl,buffer,2,0);
//	//i2c1_write((char*)buffer, 2, 0);
//	//I2C1_Repeated_Start();  // issue I2C signal repeated start
//	// ????
//	//I2C1_Wr(adr+1);  // Status = 0 if got an ACK
//	buffer[0] = adr+1;
//	//ddev_write(i2c_hdl,buffer,1,0);
//	//delay_ms(10);
//	//t = I2C1_Rd(1);
//	//ddev_read(i2c_hdl,buffer,1,0);
//	//i2c1_read((char*)buffer, 1, 0);
//	camera_intf_sccb_read(adr,&buffer[0]);
//	t = buffer[0];
//	//I2C1_stop();
//	//i2c1_close();
//
//	ddev_close(i2c_hdl);
//	return t;
//
//}*/*/*/
	int attempt;

	
// dac i2c interface 
#define I2C_CLK 26
#define I2C_DAT 27

typedef struct Codec_RegCfg_s
{
	unsigned int map;
	unsigned char reg;
}Codec_ES8374_RegCfg_t;

// 0-output&pullup, 1--input
static void gpio_config_c( UINT32 index, UINT32 mode ) 
{
	volatile UINT32 *gpio_cfg_addr;
	UINT32 val;
	UINT32 tmp;
	if(index >GPIONUM)
		return;
	gpio_cfg_addr = (volatile UINT32 *)(REG_GPIO_CFG_BASE_ADDR + index * 4);
	tmp =mode;
	mode &= 0xff;
	
	if(mode == 1)
		val =0x0c;
	else
	{
		val = 0x30;
		if(tmp>>8)
			val |= 1<<1;
	}
	REG_WRITE(gpio_cfg_addr, val);
}



static void gpio_output_c(UINT32 index, UINT32 val)
{
    UINT32 reg_val;
    volatile UINT32 *gpio_cfg_addr;

    gpio_cfg_addr = (volatile UINT32 *)(REG_GPIO_CFG_BASE_ADDR + index * 4);
    reg_val = REG_READ(gpio_cfg_addr);

    reg_val &= ~GCFG_OUTPUT_BIT;
    reg_val |= (val & 0x01) << GCFG_OUTPUT_POS;
    REG_WRITE(gpio_cfg_addr, reg_val);
}

static UINT32 DATA_INPUT(void)
{
    UINT32 val = 0;
    volatile UINT32 *gpio_cfg_addr;

    gpio_cfg_addr = (volatile UINT32 *)(REG_GPIO_CFG_BASE_ADDR + I2C_DAT * 4);
    val = REG_READ(gpio_cfg_addr);

    return (val & 1);
}


static void CLK_OUTPUT_HIGH(void)
{
	gpio_output_c(I2C_CLK,1);
}
static void CLK_OUTPUT_LOW(void)
{
	gpio_output_c(I2C_CLK,0);
}

static void DATA_OUTPUT_HIGH(void)
{
	gpio_output_c(I2C_DAT,1);
}

static void DATA_OUTPUT_LOW(void)
{
	gpio_output_c(I2C_DAT,0);
}

static void SET_DATA_INPUT(void)
{
	gpio_config_c(I2C_DAT,1);
}

static void set_data_output(void)
{
	UINT32 mode;
	UINT32 val = 	DATA_INPUT();
	mode = (val<<8) ;
	gpio_config_c(I2C_DAT,mode);
}

/*1 equals about 5 us*/
static void es8374_codec_i2c_delay(int us)
{
    volatile int i, j;
    for(i = 0; i < us; i++)
    {
        j = 50;
        while(j--);
    }	
}

static void es8374_codec_i2c_start(void)
{
    // start bit
	CLK_OUTPUT_HIGH();
	DATA_OUTPUT_HIGH();
    es8374_codec_i2c_delay(1); 
	DATA_OUTPUT_LOW();
    es8374_codec_i2c_delay(1); 
}

static void es8374_codec_i2c_stop(void)
{
	//stop bit
	CLK_OUTPUT_LOW();
	es8374_codec_i2c_delay(1); 
	CLK_OUTPUT_HIGH();
	es8374_codec_i2c_delay(1); 
	DATA_OUTPUT_HIGH();
	es8374_codec_i2c_delay(1); 
}


static void es8374_codec_i2c_write_byte(unsigned char data)
{
	int i;

	//data 0-7bit
	for(i = 7; i >= 0; i--)
	{
		CLK_OUTPUT_LOW();
		if(data & (0x1 << i)) // msb first, rising change and falling lock in codec
			DATA_OUTPUT_HIGH();
		else
			DATA_OUTPUT_LOW();
		es8374_codec_i2c_delay(1);
		CLK_OUTPUT_HIGH();
		es8374_codec_i2c_delay(1);      
	}
	//receive ack, bit9
	CLK_OUTPUT_LOW();
	SET_DATA_INPUT();

	es8374_codec_i2c_delay(1);          

	CLK_OUTPUT_HIGH();
	es8374_codec_i2c_delay(1);     
	set_data_output();
}
int TC74_readTemp_method3(int adr)
{
  int t;


  es8374_codec_i2c_start();
  es8374_codec_i2c_write_byte(adr);  // Status = 0 if got an ACK
  es8374_codec_i2c_write_byte(0x00);  // Status = 0 if got an ACK
  es8374_codec_i2c_start();  // issue I2C signal repeated start
  es8374_codec_i2c_write_byte(adr+1);  // Status = 0 if got an ACK
  delay_ms(10);
  t = I2C1_Rd(1);
  es8374_codec_i2c_stop();
      return t;

}
static void I2CWRNBYTE_CODEC(unsigned char reg, unsigned char val)
{
    UINT8 i2c_address = 0x20;

    es8374_codec_i2c_start();
    es8374_codec_i2c_write_byte(i2c_address); 	
    es8374_codec_i2c_write_byte(reg);
    es8374_codec_i2c_write_byte(val);
    es8374_codec_i2c_stop();
}


void testI2C_method1()
{
	char buffer[8];
	

	i2c1_open(0);
	
	i2c1_write(buffer, 2, 0);
	i2c1_close();

}

#endif



/*
// from BkDriverPwm.h"
OSStatus bk_pwm_start(bk_pwm_t pwm);

OSStatus bk_pwm_update_param(bk_pwm_t pwm, uint32_t frequency, uint32_t duty_cycle);

OSStatus bk_pwm_stop(bk_pwm_t pwm);
*/
//char g_pwmIndexForPinIndexMapping []
//{
//	// pin 0
//
//
//};

// pwm mqtt
// https://community.home-assistant.io/t/shelly-dimmer-with-mqtt/149871
// https://community.home-assistant.io/t/mqtt-light-problems-with-dimmer-devices/120822
// https://community.home-assistant.io/t/mqtt-dimming/96447/2
// https://community.home-assistant.io/t/mqtt-dimmer-switch-doesnt-display-brightness-slider/15471/2

int PIN_GetPWMIndexForPinIndex(int pin) {
	if(pin == 6)
		return 0;
	if(pin == 7)
		return 1;
	if(pin == 8)
		return 2;
	if(pin == 9)
		return 3;
	if(pin == 24)
		return 4;
	if(pin == 26)
		return 5;
	return -1;
}

#if PLATFORM_XR809
typedef struct xr809pin_s {
	const char *name;
	int port;
	int pin;
} xr809pin_t;

// https://developer.tuya.com/en/docs/iot/xr3-datasheet?id=K98s9168qi49g

xr809pin_t g_xrPins[] = {
	{ "PA19", GPIO_PORT_A, GPIO_PIN_19 },
	{ "PB03", GPIO_PORT_B, GPIO_PIN_3 },
	{ "PA12", GPIO_PORT_A, GPIO_PIN_12 },
	{ "PA14", GPIO_PORT_A, GPIO_PIN_14 },
	{ "PA15", GPIO_PORT_A, GPIO_PIN_15 },
	{ "PA06", GPIO_PORT_A, GPIO_PIN_6 },
	{ "PA07", GPIO_PORT_A, GPIO_PIN_7 },
	{ "PA16", GPIO_PORT_A, GPIO_PIN_16 },
	{ "PA20", GPIO_PORT_A, GPIO_PIN_20 },
	{ "PA21", GPIO_PORT_A, GPIO_PIN_21 },
	{ "PA22", GPIO_PORT_A, GPIO_PIN_22 },
	{ "PB02", GPIO_PORT_B, GPIO_PIN_2 },
	{ "PA08", GPIO_PORT_A, GPIO_PIN_8 },
};
int g_numXRPins = sizeof(g_xrPins) / sizeof(g_xrPins[0]);

void PIN_XR809_GetPortPinForIndex(int index, int *xr_port, int *xr_pin) {
	if(index < 0 || index >= g_numXRPins) {
		*xr_port = 0;
		*xr_pin = 0;
		return;
	}
	*xr_port = g_xrPins[index].port;
	*xr_pin = g_xrPins[index].pin;
}

#endif
// it was nice to have it as bits but now that we support PWM...
//int g_channelStates;
int g_channelValues[GPIO_MAX] = { 0 };
// channel content types, mostly not required to set
int g_channelTypes[GPIO_MAX] = { 0 };


pinButton_s g_buttons[GPIO_MAX];

#ifdef WINDOWS

#elif PLATFORM_XR809

#else

#endif
pinsState_t g_pins;

void (*g_channelChangeCallback)(int idx, int iVal) = 0;
void (*g_doubleClickCallback)(int pinIndex) = 0;


void PIN_SaveToFlash() {
#if WINDOWS

#elif PLATFORM_XR809
	sysinfo_t *inf;
	int res;
	inf = sysinfo_get();
	if(inf == 0) {
		printf("PIN_SaveToFlash: sysinfo_get returned 0!\n\r");
		return;
	}
	os_memcpy(&inf->pins, &g_pins, sizeof(inf->pins));
	
	if(sizeof(inf->pins) != sizeof(g_pins)) {
		printf("PIN_SaveToFlash: FATAL ERROR - pin structures size mismatch! %i vs %i\r\n",sizeof(inf->pins),sizeof(g_pins));
	}

	res = sysinfo_save_wrapper();
	if(res != 0) {
		printf("PIN_SaveToFlash: sysinfo_save error - %i!\n\r",res);
	}

#else
	ITEM_PINS_CONFIG pins;
	os_memcpy(&pins.pins, &g_pins, sizeof(pins.pins));
	CONFIG_INIT_ITEM(CONFIG_TYPE_PINS, &pins);
	config_save_item(&pins);
	// delete old if it exists
	config_delete_item(OLD_PINS_CONFIG);
#endif
}
void PIN_LoadFromFlash() {
	int i;
#if WINDOWS

#elif PLATFORM_XR809
	sysinfo_t *inf;
#else
	int res;
	ITEM_PINS_CONFIG pins;
#endif


	addLogAdv(LOG_INFO, LOG_FEATURE_GENERAL,"PIN_LoadFromFlash called - going to load pins.\r\n");
	addLogAdv(LOG_INFO, LOG_FEATURE_GENERAL,"UART log breaking after that means that you changed the role of TX pin to digital IO or smth else.\r\n");

#if WINDOWS

#elif PLATFORM_XR809
	inf = sysinfo_get();
	if(inf == 0) {
		printf("PIN_LoadFromFlash: sysinfo_get returned 0!\n\r");
		return;
	}
	os_memcpy(&g_pins, &inf->pins, sizeof(g_pins));
	if(sizeof(inf->pins) != sizeof(g_pins)) {
		printf("PIN_LoadFromFlash: FATAL ERROR - pin structures size mismatch! %i vs %i\r\n",sizeof(inf->pins),sizeof(g_pins));
	}
#else
	CONFIG_INIT_ITEM(CONFIG_TYPE_PINS, &pins);
	res = config_get_item(&pins);
	if (res){
			os_memcpy(&g_pins, &pins.pins, sizeof(g_pins));
	}
#endif
	for(i = 0; i < GPIO_MAX; i++) {
		PIN_SetPinRoleForPinIndex(i,g_pins.roles[i]);
	}
	addLogAdv(LOG_INFO, LOG_FEATURE_GENERAL,"PIN_LoadFromFlash pins have been set up.\r\n");
}
void PIN_ClearPins() {
	memset(&g_pins,0,sizeof(g_pins));
}
int PIN_GetPinRoleForPinIndex(int index) {
	return g_pins.roles[index];
}
int PIN_GetPinChannelForPinIndex(int index) {
	return g_pins.channels[index];
}
int PIN_GetPinChannel2ForPinIndex(int index) {
	return g_pins.channels2[index];
}
void RAW_SetPinValue(int index, int iVal){
#if WINDOWS

#elif PLATFORM_XR809
	int xr_port; // eg GPIO_PORT_A
	int xr_pin; // eg. GPIO_PIN_20

	PIN_XR809_GetPortPinForIndex(index, &xr_port, &xr_pin);

	HAL_GPIO_WritePin(xr_port, xr_pin, iVal);
#else
    bk_gpio_output(index, iVal);
#endif
}
void Button_OnShortClick(int index)
{
	addLogAdv(LOG_INFO, LOG_FEATURE_GENERAL,"%i key_short_press\r\n", index);
	if(g_pins.roles[index] == IOR_Button_ToggleAll || g_pins.roles[index] == IOR_Button_ToggleAll_n)
	{
		CHANNEL_DoSpecialToggleAll();
		return;
	}
	// first click toggles FIRST CHANNEL linked to this button
	CHANNEL_Toggle(g_pins.channels[index]);
}
void Button_OnDoubleClick(int index)
{
	addLogAdv(LOG_INFO, LOG_FEATURE_GENERAL,"%i key_double_press\r\n", index);
	if(g_pins.roles[index] == IOR_Button_ToggleAll || g_pins.roles[index] == IOR_Button_ToggleAll_n)
	{
		CHANNEL_DoSpecialToggleAll();
		return;
	}
	// double click toggles SECOND CHANNEL linked to this button
	CHANNEL_Toggle(g_pins.channels2[index]);

	if(g_doubleClickCallback!=0) {
		g_doubleClickCallback(index);
	}
}
void Button_OnLongPressHold(int index)
{
	addLogAdv(LOG_INFO, LOG_FEATURE_GENERAL,"%i key_long_press_hold\r\n", index);
}

const char *PIN_GetPinNameAlias(int index) {
#if PLATFORM_XR809
	if(index < 0 || index >= g_numXRPins) {
		return "bad_index";
	}
	return g_xrPins[index].name;
#else
	return "not_implemented_here";
#endif
}
bool BTN_ShouldInvert(int index)
{
	if(g_pins.roles[index] == IOR_Button_n || g_pins.roles[index] == IOR_Button_ToggleAll_n|| g_pins.roles[index] == IOR_DigitalInput_n) {
		return true;
	}
	return false;
}
unsigned char PIN_ReadDigitalInputValue_WithInversionIncluded(int index) {
#if WINDOWS
	return 0;
#elif PLATFORM_XR809
	int xr_port; // eg GPIO_PORT_A
	int xr_pin; // eg. GPIO_PIN_20

	PIN_XR809_GetPortPinForIndex(index, &xr_port, &xr_pin);

	if(BTN_ShouldInvert(index)) {
		if (HAL_GPIO_ReadPin(xr_port, xr_pin) == GPIO_PIN_LOW)
			return 1;//0;
		return 0;//1;
	}
	if (HAL_GPIO_ReadPin(xr_port, xr_pin) == GPIO_PIN_LOW)
		return 0;
	return 1;
#else
	// support inverted button
	if(BTN_ShouldInvert(index)) {
		return !bk_gpio_input(index);
	}
	return bk_gpio_input(index);
#endif
}
unsigned char button_generic_get_gpio_value(void *param)
{
#if WINDOWS
	return 0;
#else
	int index;
	index = ((pinButton_s*)param) - g_buttons;

	return PIN_ReadDigitalInputValue_WithInversionIncluded(index);
#endif
}
#define PIN_UART1_RXD 10
#define PIN_UART1_TXD 11
#define PIN_UART2_RXD 1
#define PIN_UART2_TXD 0

void NEW_button_init(pinButton_s* handle, uint8_t(*pin_level)(void *self), uint8_t active_level)
{
	memset(handle, sizeof(pinButton_s), 0);
	
	handle->event = (uint8_t)BTN_NONE_PRESS;
	handle->hal_button_Level = pin_level;
	handle->button_level = handle->hal_button_Level(handle);
	handle->active_level = active_level;
}
void CHANNEL_SetType(int ch, int type) {

	g_channelTypes[ch] = type;
}
int CHANNEL_GetType(int ch) {
	return g_channelTypes[ch];
}
bool CHANNEL_IsInUse(int ch) {
	int i;
	for(i = 0; i < GPIO_MAX; i++){
		if(g_pins.channels[i] == ch) {
			switch(g_pins.roles[i])
			{
			case IOR_LED:
			case IOR_LED_n:
			case IOR_Relay:
			case IOR_Relay_n:
			case IOR_PWM:
				return true;
				break;

			default:
				break;
			}
		}
	}
	return false;
}
void CHANNEL_SetAll(int iVal) {
	int i;


	for(i = 0; i < GPIO_MAX; i++) {	
		switch(g_pins.roles[i])
		{
		case IOR_Button:
		case IOR_Button_n:
		case IOR_Button_ToggleAll:
		case IOR_Button_ToggleAll_n:
			{

			}
			break;
		case IOR_LED:
		case IOR_LED_n:
		case IOR_Relay:
		case IOR_Relay_n:
			CHANNEL_Set(g_pins.channels[i],iVal,1);
			break;
		case IOR_PWM:
			CHANNEL_Set(g_pins.channels[i],iVal,1);
			break;

		default:
			break;
		}
	}
}
void CHANNEL_SetStateOnly(int iVal) {
	int i;

	if(iVal == 0) {
		CHANNEL_SetAll(0);
		return;
	}

	// is it already on on some channel?
	for(i = 0; i < GPIO_MAX; i++) {
		if(CHANNEL_IsInUse(i)) {
			if(CHANNEL_Get(i) > 0) {
				return;
			}
		}
	}
	CHANNEL_SetAll(iVal);


}
void CHANNEL_DoSpecialToggleAll() {
	int anyEnabled, i;

	anyEnabled = 0;

	for(i = 0; i < CHANNEL_MAX; i++) {	
		if(CHANNEL_IsInUse(i)==false)
			continue;
		if(g_channelValues[i] > 0) {
			anyEnabled++;
		}	
	}
	if(anyEnabled)
		CHANNEL_SetAll(0);
	else
		CHANNEL_SetAll(255);

}
void PIN_SetPinRoleForPinIndex(int index, int role) {
#if 0
	if(index == PIN_UART1_RXD) {
		// default role to None in order to fix broken config
		role = IOR_None;
	}
	if(index == PIN_UART1_TXD) {
		// default role to None in order to fix broken config
		role = IOR_None;
	}
	if(index == PIN_UART2_RXD) {
		// default role to None in order to fix broken config
		role = IOR_None;
	}
	if(index == PIN_UART2_TXD) {
		// default role to None in order to fix broken config
		role = IOR_None;
	}
#endif
	switch(g_pins.roles[index])
	{
	case IOR_Button:
	case IOR_Button_n:
	case IOR_Button_ToggleAll:
	case IOR_Button_ToggleAll_n:
		{
			//pinButton_s *bt = &g_buttons[index];
			// TODO: disable button
#if WINDOWS
	
#elif PLATFORM_XR809

#else
#endif
		}
		break;
	case IOR_LED:
	case IOR_LED_n:
	case IOR_Relay:
	case IOR_Relay_n:
#if WINDOWS
	
#elif PLATFORM_XR809

#else
		// TODO: disable?
#endif
		break;
		// Disable PWM for previous pin role
	case IOR_PWM:
		{
			int pwmIndex;
			//int channelIndex;

			pwmIndex = PIN_GetPWMIndexForPinIndex(index);
			// is this pin capable of PWM?
			if(pwmIndex != -1) {
#if WINDOWS
	
#elif PLATFORM_XR809

#elif PLATFORM_BK7231N
				bk_pwm_stop(pwmIndex);
#elif PLATFORM_BK7231T
				bk_pwm_stop(pwmIndex);
#else
#error "Unknown platform"
#endif
			}
		}
		break;

	default:
		break;
	}
	// set new role
	g_pins.roles[index] = role;
	// init new role
	switch(role)
	{
	case IOR_Button:
	case IOR_Button_n:
	case IOR_Button_ToggleAll:
	case IOR_Button_ToggleAll_n:
		{
			pinButton_s *bt = &g_buttons[index];
#if WINDOWS
	
#elif PLATFORM_XR809
			{
				int xr_port; // eg GPIO_PORT_A
				int xr_pin; // eg. GPIO_PIN_20
				GPIO_InitParam param;

				PIN_XR809_GetPortPinForIndex(index, &xr_port, &xr_pin);

				param.driving = GPIO_DRIVING_LEVEL_1;
				param.mode = GPIOx_Pn_F0_INPUT;
				param.pull = GPIO_PULL_UP;
				HAL_GPIO_Init(xr_port, xr_pin, &param);
			}
#else
			bk_gpio_config_input_pup(index);
#endif
			// init button after initializing pin role
			NEW_button_init(bt, button_generic_get_gpio_value, 0);
		/*	button_attach(bt, BTN_SINGLE_CLICK,     Button_OnShortClick);
			button_attach(bt, BTN_DOUBLE_CLICK,     Button_OnDoubleClick);
			button_attach(bt, BTN_LONG_PRESS_HOLD,  Button_OnLongPressHold);
			button_start(bt);*/
		}
		break;
	case IOR_DigitalInput:
	case IOR_DigitalInput_n:
#if WINDOWS
	
#elif PLATFORM_XR809
			{
				int xr_port; // eg GPIO_PORT_A
				int xr_pin; // eg. GPIO_PIN_20
				GPIO_InitParam param;

				PIN_XR809_GetPortPinForIndex(index, &xr_port, &xr_pin);

				param.driving = GPIO_DRIVING_LEVEL_1;
				param.mode = GPIOx_Pn_F0_INPUT;
				param.pull = GPIO_PULL_UP;
				HAL_GPIO_Init(xr_port, xr_pin, &param);
			}
#else
			bk_gpio_config_input_pup(index);
#endif
		break;
	case IOR_LED:
	case IOR_LED_n:
	case IOR_Relay:
	case IOR_Relay_n:
	{
#if WINDOWS
	
#elif PLATFORM_XR809
		GPIO_InitParam param;
		int xr_port; // eg GPIO_PORT_A
		int xr_pin; // eg. GPIO_PIN_20

		PIN_XR809_GetPortPinForIndex(index, &xr_port, &xr_pin);

		/*set pin driver capability*/
		param.driving = GPIO_DRIVING_LEVEL_1;
		param.mode = GPIOx_Pn_F1_OUTPUT;
		param.pull = GPIO_PULL_NONE;
		HAL_GPIO_Init(xr_port, xr_pin, &param);
#else
		bk_gpio_config_output(index);
		bk_gpio_output(index, 0);
#endif
	}
		break;
	case IOR_PWM:
		{
			int pwmIndex;
			int channelIndex;
			float f;

			pwmIndex = PIN_GetPWMIndexForPinIndex(index);
			// is this pin capable of PWM?
			if(pwmIndex != -1) {
				channelIndex = PIN_GetPinChannelForPinIndex(index);
#if WINDOWS
	
#elif PLATFORM_XR809

#elif PLATFORM_BK7231N
				// OSStatus bk_pwm_initialize(bk_pwm_t pwm, uint32_t frequency, uint32_t duty_cycle);
				bk_pwm_initialize(pwmIndex, 1000, 0, 0, 0);

				bk_pwm_start(pwmIndex);
				f = g_channelValues[channelIndex] * 0.01f;
				// OSStatus bk_pwm_update_param(bk_pwm_t pwm, uint32_t frequency, uint32_t duty_cycle1, uint32_t duty_cycle2, uint32_t duty_cycle3)
				bk_pwm_update_param(pwmIndex, 1000, f * 1000.0f,0,0);

#else
				// OSStatus bk_pwm_initialize(bk_pwm_t pwm, uint32_t frequency, uint32_t duty_cycle);
				bk_pwm_initialize(pwmIndex, 1000, 0);

				bk_pwm_start(pwmIndex);
				// they are using 1kHz PWM
				// See: https://www.elektroda.pl/rtvforum/topic3798114.html
			//	bk_pwm_update_param(pwmIndex, 1000, g_channelValues[channelIndex]);
				f = g_channelValues[channelIndex] * 0.01f;
				// OSStatus bk_pwm_update_param(bk_pwm_t pwm, uint32_t frequency, uint32_t duty_cycle)
				bk_pwm_update_param(pwmIndex, 1000, f * 1000.0f);

#endif
			}
		}
		break;

	default:
		break;
	}
}
void PIN_SetPinChannelForPinIndex(int index, int ch) {
	g_pins.channels[index] = ch;
}
void PIN_SetPinChannel2ForPinIndex(int index, int ch) {
	g_pins.channels2[index] = ch;
}
void PIN_SetGenericDoubleClickCallback(void (*cb)(int pinIndex)) {
	g_doubleClickCallback = cb;
}
void CHANNEL_SetChangeCallback(void (*cb)(int idx, int iVal)) {
	g_channelChangeCallback = cb;
}
void Channel_OnChanged(int ch) {
	int i;
	int iVal;
	int bOn;
	int pwmIndex;


	//bOn = BIT_CHECK(g_channelStates,ch);
	iVal = g_channelValues[ch];
	bOn = iVal > 0;

	I2C_OnChannelChanged(ch,iVal);

	for(i = 0; i < GPIO_MAX; i++) {
		if(g_pins.channels[i] == ch) {
			if(g_pins.roles[i] == IOR_Relay || g_pins.roles[i] == IOR_LED) {
				RAW_SetPinValue(i,bOn);
				if(g_channelChangeCallback != 0) {
					g_channelChangeCallback(ch,bOn);
				}
			}
			if(g_pins.roles[i] == IOR_Relay_n || g_pins.roles[i] == IOR_LED_n) {
				RAW_SetPinValue(i,!bOn);
				if(g_channelChangeCallback != 0) {
					g_channelChangeCallback(ch,bOn);
				}
			}
			if(g_pins.roles[i] == IOR_DigitalInput) {
				if(g_channelChangeCallback != 0) {
					g_channelChangeCallback(ch,iVal);
				}
			}
			if(g_pins.roles[i] == IOR_PWM) {
				if(g_channelChangeCallback != 0) {
					g_channelChangeCallback(ch,iVal);
				}
				pwmIndex = PIN_GetPWMIndexForPinIndex(i);
				// is this pin capable of PWM?
				if(pwmIndex != -1) {

#if WINDOWS
	
#elif PLATFORM_XR809

#elif PLATFORM_BK7231N
					bk_pwm_update_param(pwmIndex, 1000, iVal * 10.0f,0,0); // Duty cycle 0...100 * 10.0 = 0...1000

#else
					// they are using 1kHz PWM
					// See: https://www.elektroda.pl/rtvforum/topic3798114.html
					bk_pwm_update_param(pwmIndex, 1000, iVal * 10.0f); // Duty cycle 0...100 * 10.0 = 0...1000
#endif
				}
			}
			
		}
	}

}
int CHANNEL_Get(int ch) {
	if(ch < 0 || ch >= GPIO_MAX) {
		addLogAdv(LOG_ERROR, LOG_FEATURE_GENERAL,"CHANNEL_Get: Channel index %i is out of range <0,%i)\n\r",ch,GPIO_MAX);
		return 0;
	}
	return g_channelValues[ch];
}

void CHANNEL_Set(int ch, int iVal, int bForce) {
	if(ch < 0 || ch >= GPIO_MAX) {
		addLogAdv(LOG_ERROR, LOG_FEATURE_GENERAL,"CHANNEL_Set: Channel index %i is out of range <0,%i)\n\r",ch,GPIO_MAX);
		return;
	}
	if(bForce == 0) {
		int prevVal;

		//prevVal = BIT_CHECK(g_channelStates, ch);
		prevVal = g_channelValues[ch];
		if(prevVal == iVal) {
			addLogAdv(LOG_INFO, LOG_FEATURE_GENERAL,"No change in channel %i - ignoring\n\r",ch);
			return;
		}
	}
	addLogAdv(LOG_INFO, LOG_FEATURE_GENERAL,"CHANNEL_Set channel %i has changed to %i\n\r",ch,iVal);
	//if(iVal) {
	//	BIT_SET(g_channelStates,ch);
	//} else {
	//	BIT_CLEAR(g_channelStates,ch);
	//}
	g_channelValues[ch] = iVal;

	Channel_OnChanged(ch);
}
void CHANNEL_Toggle(int ch) {
	if(ch < 0 || ch >= GPIO_MAX) {
		addLogAdv(LOG_ERROR, LOG_FEATURE_GENERAL,"CHANNEL_Toggle: Channel index %i is out of range <0,%i)\n\r",ch,GPIO_MAX);
		return;
	}
	//BIT_TGL(g_channelStates,ch);
	if(g_channelValues[ch] == 0)
		g_channelValues[ch] = 100;
	else
		g_channelValues[ch] = 0;

	Channel_OnChanged(ch);
}
bool CHANNEL_Check(int ch) {
	if(ch < 0 || ch >= GPIO_MAX) {
		addLogAdv(LOG_ERROR, LOG_FEATURE_GENERAL,"CHANNEL_Check: Channel index %i is out of range <0,%i)\n\r",ch,GPIO_MAX);
		return 0;
	}
	//return BIT_CHECK(g_channelStates,ch);
	if (g_channelValues[ch] > 0)
		return 1;
	return 0;
}


int CHANNEL_GetRoleForOutputChannel(int ch){
	int i;
	for (i = 0; i < 32; i++){
		if (g_pins.channels[i] == ch){
			switch(g_pins.roles[i]){
				case IOR_Relay:
				case IOR_Relay_n:
				case IOR_LED:
				case IOR_LED_n:
				case IOR_PWM:
					return g_pins.roles[i];
					break;
				case IOR_Button:
				case IOR_Button_n:
				case IOR_LED_WIFI:
				case IOR_LED_WIFI_n:
					break;
			}
		}
	}
	return IOR_None;
}


#define EVENT_CB(ev)   if(handle->cb[ev])handle->cb[ev]((pinButton_s*)handle)

#define PIN_TMR_DURATION       5
#if WINDOWS

#elif PLATFORM_XR809

#else

beken_timer_t g_pin_timer;
#endif

void PIN_Input_Handler(int pinIndex)
{
	pinButton_s *handle;
	uint8_t read_gpio_level;
	
	handle = &g_buttons[pinIndex];
	//read_gpio_level = bk_gpio_input(pinIndex);
	read_gpio_level = button_generic_get_gpio_value(handle);

	//ticks counter working..
	if((handle->state) > 0)
		handle->ticks++;

	/*------------button debounce handle---------------*/
	if(read_gpio_level != handle->button_level) { //not equal to prev one
		//continue read 3 times same new level change
		if(++(handle->debounce_cnt) >= BTN_DEBOUNCE_TICKS) {
			handle->button_level = read_gpio_level;
			handle->debounce_cnt = 0;
		}
	} else { //leved not change ,counter reset.
		handle->debounce_cnt = 0;
	}

	/*-----------------State machine-------------------*/
	switch (handle->state) {
	case 0:
		if(handle->button_level == handle->active_level) {	//start press down
			handle->event = (uint8_t)BTN_PRESS_DOWN;
			EVENT_CB(BTN_PRESS_DOWN);
			handle->ticks = 0;
			handle->repeat = 1;
			handle->state = 1;
		} else {
			handle->event = (uint8_t)BTN_NONE_PRESS;
		}
		break;

	case 1:
		if(handle->button_level != handle->active_level) { //released press up
			handle->event = (uint8_t)BTN_PRESS_UP;
			EVENT_CB(BTN_PRESS_UP);
			handle->ticks = 0;
			handle->state = 2;

		} else if(handle->ticks > BTN_LONG_TICKS) {
			handle->event = (uint8_t)BTN_LONG_RRESS_START;
			EVENT_CB(BTN_LONG_RRESS_START);
			handle->state = 5;
		}
		break;

	case 2:
		if(handle->button_level == handle->active_level) { //press down again
			handle->event = (uint8_t)BTN_PRESS_DOWN;
			EVENT_CB(BTN_PRESS_DOWN);
			handle->repeat++;
			if(handle->repeat == 2) {
				EVENT_CB(BTN_DOUBLE_CLICK); // repeat hit
				Button_OnDoubleClick(pinIndex);
			} 
			EVENT_CB(BTN_PRESS_REPEAT); // repeat hit
			handle->ticks = 0;
			handle->state = 3;
		} else if(handle->ticks > BTN_SHORT_TICKS) { //released timeout
			if(handle->repeat == 1) {
				handle->event = (uint8_t)BTN_SINGLE_CLICK;
				EVENT_CB(BTN_SINGLE_CLICK);
				Button_OnShortClick(pinIndex);
			} else if(handle->repeat == 2) {
				handle->event = (uint8_t)BTN_DOUBLE_CLICK;
			}
			handle->state = 0;
		}
		break;

	case 3:
		if(handle->button_level != handle->active_level) { //released press up
			handle->event = (uint8_t)BTN_PRESS_UP;
			EVENT_CB(BTN_PRESS_UP);
			if(handle->ticks < BTN_SHORT_TICKS) {
				handle->ticks = 0;
				handle->state = 2; //repeat press
			} else {
				handle->state = 0;
			}
		}
		break;

	case 5:
		if(handle->button_level == handle->active_level) {
			//continue hold trigger
			handle->event = (uint8_t)BTN_LONG_PRESS_HOLD;
			EVENT_CB(BTN_LONG_PRESS_HOLD);

		} else { //releasd
			handle->event = (uint8_t)BTN_PRESS_UP;
			EVENT_CB(BTN_PRESS_UP);
			handle->state = 0; //reset
		}
		break;
	}
}


/**
  * @brief  background ticks, timer repeat invoking interval 5ms.
  * @param  None.
  * @retval None
  */
void PIN_ticks(void *param)
{
	int i;
	int value;

	for(i = 0; i < GPIO_MAX; i++) {
		if(g_pins.roles[i] == IOR_Button || g_pins.roles[i] == IOR_Button_n
			|| g_pins.roles[i] == IOR_Button_ToggleAll || g_pins.roles[i] == IOR_Button_ToggleAll_n) {
			//addLogAdv(LOG_INFO, LOG_FEATURE_GENERAL,"Test hold %i\r\n",i);
			PIN_Input_Handler(i);
		}		
		else if(g_pins.roles[i] == IOR_DigitalInput || g_pins.roles[i] == IOR_DigitalInput_n) {
			// read pin digital value (and already invert it if needed)
			value = PIN_ReadDigitalInputValue_WithInversionIncluded(i);
				// NOT NEEDED - INCLUDED in function above
			//if(g_pins.roles[i] == IOR_DigitalInput_n) {
//
			//}
			CHANNEL_Set(g_pins.channels[i], value,0);
		}
	}
}
int CHANNEL_ParseChannelType(const char *s) {
	if(!stricmp(s,"temperature"))
		return ChType_Temperature;
	if(!stricmp(s,"humidity") )
		return ChType_Humidity;
	if(!stricmp(s,"humidity_div10") )
		return ChType_Humidity_div10;
	if(!stricmp(s,"temperature_div10"))
		return ChType_Temperature_div10;
	if(!stricmp(s,"toggle"))
		return ChType_Toggle;
	if(!stricmp(s,"default") )
		return ChType_Default;
	return ChType_Error;
}
static int CMD_SetChannelType(const void *context, const char *cmd, const char *args){
	int channel;
	const char *type;
	int typeCode;

	Tokenizer_TokenizeString(args);

	if(Tokenizer_GetArgsCount() < 2) {
		addLogAdv(LOG_INFO, LOG_FEATURE_GENERAL,"This command requires 2 arguments");
		return 1;
	}
	channel = Tokenizer_GetArgInteger(0);
	type = Tokenizer_GetArg(1);

	typeCode = CHANNEL_ParseChannelType(type);
	if(typeCode == ChType_Error) {

		addLogAdv(LOG_INFO, LOG_FEATURE_GENERAL,"Channel %i type not set because %s is not a known type", channel,type);
		return 1;
	}

	CHANNEL_SetType(channel,typeCode);

	addLogAdv(LOG_INFO, LOG_FEATURE_GENERAL,"Channel %i type changed to %s", channel,type);
	return 0;
}

static int showgpi(const void *context, const char *cmd, const char *args){
	int i;
	unsigned int value = 0;

	for (i = 0; i < 32; i++) {
		int val = 0;
#ifdef WINDOWS		
#elif PLATFORM_XR809
		int xr_port; // eg GPIO_PORT_A
		int xr_pin; // eg. GPIO_PIN_20
		PIN_XR809_GetPortPinForIndex(i, &xr_port, &xr_pin);

		if (HAL_GPIO_ReadPin(xr_port, xr_pin) == GPIO_PIN_LOW)
			val = 0;
		else 
			val = 1;
#else
		val = bk_gpio_input(i);
		if (val){
			val = 1;
		} else {
			val = 0;
		}
#endif
		value |= ((val & 1)<<i);
	}
	addLogAdv(LOG_INFO, LOG_FEATURE_GENERAL,"GPIs are 0x%x", value);
	return 1;
}


void PIN_Init(void)
{
#if WINDOWS

#elif PLATFORM_XR809
	OS_Timer_t timer;

	OS_TimerSetInvalid(&timer);
	if (OS_TimerCreate(&timer, OS_TIMER_PERIODIC, PIN_ticks, NULL,
	                   PIN_TMR_DURATION) != OS_OK) {
		printf("PIN_Init timer create failed\n");
		return;
	}

	OS_TimerStart(&timer); /* start OS timer to feed watchdog */
#else

	OSStatus result;
	
    result = rtos_init_timer(&g_pin_timer,
                            PIN_TMR_DURATION,
                            PIN_ticks,
                            (void *)0);
    ASSERT(kNoErr == result);
	
    result = rtos_start_timer(&g_pin_timer);
    ASSERT(kNoErr == result);
#endif


	CMD_RegisterCommand("showgpi", NULL, showgpi, "log stat of all GPIs", NULL);
	CMD_RegisterCommand("setChannelType", NULL, CMD_SetChannelType, "qqqqqqqq", NULL);
}
void PIN_set_wifi_led(int value){
	int res = -1;
	int i;
	for ( i = 0; i < 32; i++){
		if ((g_pins.roles[i] == IOR_LED_WIFI) || (g_pins.roles[i] == IOR_LED_WIFI_n)){
			res = i;
			break;
		}
	}
	if (res >= 0){
		if (g_pins.roles[res] == IOR_LED_WIFI_n){
			value = !value;
		}
		RAW_SetPinValue(res, value & 1);
	}
}



