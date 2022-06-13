/*
 * utilities.h
 *
 *  Created on: Jun 9, 2022
 *      Author: surovotic
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

// Running Modes
// 1 = DATA CAPTURE => Use this mode for collecting data and use the Data Caputre Lab
// 2 = RECOGNITION => Use this mode for running a Knowledge pack from the sensor
#define DATA_CAPTURE_RUNNING_MODE 1
#define RECOGNITION_RUNNING_MODE 2

// Change the below to either DATA_CAPTURE_RUNNING_MODE (or) RECOGNITION_RUNNING_MODE
#define APPLICATION_RUNNING_MODE RECOGNITION_RUNNING_MODE

// FUNCTIONS
void updateDisplay(void);

// GLOBAL VARIABLES
extern int f;
extern int transmit_flag;
extern float adcTGS;



#endif /* UTILITIES_H_ */
