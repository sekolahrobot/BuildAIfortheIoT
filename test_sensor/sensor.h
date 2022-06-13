/*
 * sensor.h
 *
 *  Created on: Jun 9, 2022
 *      Author: surovotic
 */

#ifndef SENSOR_H_
#define SENSOR_H_


#include "utilities.h"

// FUNCTIONS
void initIO(void);
void updateIO(void);
void captureADC(void);
void recognitionADC(void);
void calibrateADC(void);
void serialInt(void);
void print_sensor_config_json(void);


#endif /* SENSOR_H_ */
