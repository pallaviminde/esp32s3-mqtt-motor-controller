#ifndef CAN_HANDLER_H
#define CAN_HANDLER_H

#include "driver/twai.h"
#include <stdbool.h>
#include <stdint.h>

void can_init(void);
bool can_receive(uint8_t *temp, uint8_t *fault);
#endif