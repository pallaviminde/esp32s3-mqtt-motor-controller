#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <stdint.h>

void temp_sensor_init(void);
uint8_t read_temperature(void);

#endif