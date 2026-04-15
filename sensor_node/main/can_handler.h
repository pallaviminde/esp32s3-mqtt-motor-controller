#ifndef CAN_HANDLER_H
#define CAN_HANDLER_H

#include "driver/twai.h"
#include <stdint.h>

void can_init(void);
void can_send(uint8_t temp, uint8_t fault);

#endif