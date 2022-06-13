/*
 * utilities.c
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
#include "mtb_ssd1306.h"
#include "GUI.h"
#include "cy_rgb_led.h"
#include <utilities.h>

// UPDATE THE DISPLAY
void updateDisplay(){
	GUI_Clear();
	char c[10];
		GUI_DispStringAt("ODOR DETECTOR", 2, 2);
		GUI_DispStringAt("Data:", 2, 20);
		sprintf(c, "%3.0f", adcTGS);
		GUI_DispStringAt(c, 50, 20);
		GUI_DispStringAt("Normall", 2, 40);

//	// Print recognition result
//	if(f == 5){
//		// NOTHING
//		GUI_DispStringAt("NO LOAD", 2, 2);
//	}
//	if(f == 3){
//		// VACUUM
//		GUI_DispStringAt("VACUUM", 2, 2);
//	}
//	if(f == 1){
//		// PC
//		GUI_DispStringAt("PC", 2, 2);
//	}
//	if(f == 2){
//		// LOW POWER LOAD
//		GUI_DispStringAt("LOW POWER", 2, 2);
//	}
//	if(f == 4){
//		// HEATER
//		GUI_DispStringAt("HEATER", 2, 2);
//	}
}




