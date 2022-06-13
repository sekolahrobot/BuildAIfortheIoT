/*
 * sensor.c
 *
 *  Created on: Jun 9, 2022
 *      Author: surovotic
 */
// MachineLearningLoadIdentifier program for Infineon PSoC 62S2 Wi-Fi BT Pioneer Kit and VY8CKIT-028-SENSE
// Build AI for the IoT contest @Hackster.io
// Alessandro Felicetti 2022

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "cJSON.h"
#include "cy_rgb_led.h"
#include "mtb_ssd1306.h"
#include "GUI.h"
#include <utilities.h>
#include "kb.h"
#include "kb_debug.h"

// VARIABLES AND OBJECTS
#define CURRENT_CHANNEL (P10_5)
#define SENSOR_BATCH_SIZE  1
#define SENSOR_SCAN_RATE	2000
int transmit_flag = 0;
static char str_buffer[2048];
static uint8_t fv_arr[MAX_VECTOR_SIZE];
int32_t calibrated0 = 0;
int ret = 0;
int f = 0;
uint32_t read_len = 0;
float adcTGS = 0.0f;
cy_rslt_t result;
cyhal_adc_t adc_obj;
cyhal_adc_channel_t adc_chan_0_obj;
const cyhal_adc_config_t adc_config = {
		.continuous_scanning=false, // Continuous Scanning is disabled
		.average_count=1,           // Average count disabled
		.vref=CYHAL_ADC_REF_VDDA   ,   // VREF for Single ended channel set to VDDA
		.vneg=CYHAL_ADC_VNEG_VSSA,  // VNEG for Single ended channel set to VSSA
		.resolution = 12u,          // 12-bit resolution
		.ext_vref = NC,             // No connection
		.bypass_pin = NC };       // No connection
cyhal_i2c_t i2c_obj;
cyhal_i2c_cfg_t i2c_cfg = {
		.is_slave = false,
		.address = 0,
		.frequencyhal_hz = 400000
};

// READ ADC CHANNEL AND SEND DATA TO SENSIML
void captureADC(void){
	if(transmit_flag == 1){
		int32_t adc_result_0 = 0;
		adc_result_0 = cyhal_adc_read_uv(&adc_chan_0_obj);
		if(adc_result_0 < 0){
			adc_result_0 = 0;
		}
		if(adc_result_0 > 2999999){
			adc_result_0 = 2999999;
		}
		int16_t tx_buf[10];
		size_t tx_length = 2;
		tx_buf[0] = (unsigned long)adc_result_0 / 100;
		cyhal_uart_write(&cy_retarget_io_uart_obj, (void*)tx_buf, &tx_length);
	}
	else
	{
		print_sensor_config_json();
		cyhal_system_delay_ms(500);
	}
}

// READ ADC CHANNEL AND RUN LOCAL INTERFERENCE
void recognitionADC(void){
	int32_t adc_result_0 = 0;
	adc_result_0 = cyhal_adc_read_uv(&adc_chan_0_obj);
	if(adc_result_0 < 0){
		adc_result_0 = 0;
	}
	if(adc_result_0 > 2999999){
		adc_result_0 = 2999999;
	}
	int16_t tx_buf[4];
	tx_buf[0] = (unsigned long)adc_result_0 / 100;
	SENSOR_DATA_T* pData = (SENSOR_DATA_T*) &tx_buf[0];
	ret = kb_run_model((SENSOR_DATA_T*)pData, 1, 0);
	if (ret >= 0){
		kb_print_model_result(0, ret, str_buffer, 1, fv_arr);
		printf("%s\r\n", str_buffer);
		f = ret;
		kb_reset_model(0);
	}
}

// UPDATE IO/SENSORS
void updateIO(void){
	mtb_ssd1306_free();
	mtb_ssd1306_init_i2c(&i2c_obj);
	int32_t adc_result_0 = 0;
	adc_result_0 = cyhal_adc_read_uv(&adc_chan_0_obj);

	adcTGS = adc_result_0/100;
	printf("adcTGS  = %4.2f\r\n", adcTGS);

}

void calibrateADC(void){
	int32_t adc_result_0 = 0;
	int32_t temp = 0;
	for(int i = 0; i < 10; ++i){
		adc_result_0 = cyhal_adc_read_uv(&adc_chan_0_obj);
		temp = temp + adc_result_0;
		cyhal_system_delay_us(333);
	}
	calibrated0 = temp / 10;
	printf("Calibrated ADC zeropoint: %lu\r\n", calibrated0);
}
// MAIN INIT FUNCTION
void initIO(void){
	// INIT SERIAL INTERFACE
	cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);
	printf("\033[3J");
	printf("Loading...\r\n");
	// INIT GPIO AND ADC
	cy_rgb_led_init(CYBSP_LED_RGB_RED, CYBSP_LED_RGB_GREEN, CYBSP_LED_RGB_BLUE, CY_RGB_LED_ACTIVE_LOW);
	cy_rgb_led_off();
	cyhal_gpio_init(P13_5, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
	cyhal_gpio_write(P13_5, true);
	cyhal_adc_init(&adc_obj, CURRENT_CHANNEL, NULL);
	const cyhal_adc_channel_config_t channel_config = {
			.enable_averaging = false,
			.min_acquisition_ns = 1000,
			.enabled = true };
	cyhal_adc_channel_init_diff(&adc_chan_0_obj, &adc_obj, CURRENT_CHANNEL, CYHAL_ADC_VNEG, &channel_config);
	cyhal_adc_configure(&adc_obj, &adc_config);
	// INIT DISPLAY SSD1306
	cyhal_i2c_init(&i2c_obj, CYBSP_I2C_SDA, CYBSP_I2C_SCL, NULL);
	cyhal_i2c_configure(&i2c_obj, &i2c_cfg);
	mtb_ssd1306_init_i2c(&i2c_obj);
	GUI_Init();
	GUI_SetFont(GUI_FONT_8X16);
	GUI_SetBkColor(GUI_BLACK);
	GUI_SetColor(GUI_WHITE);
	GUI_SetTextMode(GUI_TM_NORMAL);
	GUI_Clear();
#if (APPLICATION_RUNNING_MODE == DATA_CAPTURE_RUNNING_MODE)
	GUI_DispStringAt("*CAPTURE MODE*", 2, 20);
#endif
#if (APPLICATION_RUNNING_MODE == RECOGNITION_RUNNING_MODE)
	GUI_DispStringAt("*REC. MODE*", 2, 20);
	kb_model_init();
	calibrateADC();
#endif
	cyhal_system_delay_ms(100);
}

// READ INCOMING SERIAL MESSAGES
void serialInt(){
	read_len = cyhal_uart_readable(&cy_retarget_io_uart_obj);
	if(read_len > 0)
	{
		char read_buffer[20];
		memset(read_buffer, 0, 20);
		cyhal_uart_read(&cy_retarget_io_uart_obj, read_buffer, &read_len);
		if(strcmp("connect",read_buffer) == 0)
		{
			transmit_flag = 1;
			cy_rgb_led_on(CY_RGB_LED_COLOR_BLUE, 10u);
		}
		else if(strcmp("disconnect",read_buffer) == 0)
		{
			transmit_flag = 0;
			print_sensor_config_json();
			cy_rgb_led_off();
			cyhal_system_delay_ms(500);
		}

	}
}

// SEND SENSOR CONFIGURATION TO SENSIML
void print_sensor_config_json(void){
	char *string = NULL;
	cJSON *sample_rate = NULL;
	cJSON *version = NULL;
	cJSON *samples_per_packet = NULL;
	cJSON *column_location = NULL;
	cJSON *Current = NULL;

	cJSON *config_json = cJSON_CreateObject();
	if (config_json == NULL)
	{
		goto end;
	}

	/* Create sample rate */
	sample_rate = cJSON_CreateNumber((unsigned int) SENSOR_SCAN_RATE);
	if (sample_rate == NULL)
	{
		goto end;
	}
	cJSON_AddItemToObject(config_json, "sample_rate", sample_rate);

	/* Create version (Version-1) */
	version = cJSON_CreateNumber(1);
	if (version == NULL)
	{
		goto end;
	}
	cJSON_AddItemToObject(config_json, "version", version);

	/* Create samples per packet */
	samples_per_packet = cJSON_CreateNumber((unsigned int) SENSOR_BATCH_SIZE);
	if (samples_per_packet == NULL)
	{
		goto end;
	}
	cJSON_AddItemToObject(config_json, "samples_per_packet", samples_per_packet);

	/* Create Column location array */
	column_location = cJSON_CreateObject();
	if (column_location == NULL)
	{
		goto end;
	}
	cJSON_AddItemToObject(config_json, "column_location", column_location);


	/* Create arrays */
	Current = cJSON_CreateNumber((unsigned int) 0);
	if (Current == NULL)
	{
		goto end;
	}
	cJSON_AddItemToObject(column_location, "ADCCurrent", Current);


	// string = cJSON_Print(config_json);
	string = cJSON_PrintUnformatted(config_json);
	if (string == NULL)
	{
		printf( "Failed to print monitor.\n");
	}

	end:
	cJSON_Delete(config_json);
	printf(string);
	printf("\r\n");
}
