#ifndef MQTT_H
#define MQTT_H

#include <stdint.h>

void mqtt_init(void);
void mqtt_publish_data(uint8_t temp, uint8_t fault);

#endif