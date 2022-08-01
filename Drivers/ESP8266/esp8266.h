/*
 * esp8266.h
 *
 *  Created on: Jul 24, 2022
 *      Author: BGD
 */

#ifndef ESP8266_ESP8266_H_
#define ESP8266_ESP8266_H_
#include "main.h"
#include "usart.h"

#define success 1
#define fail 0
#define ESPhuart huart6
#define USART8266 USART6

void ESP8266_STA_init(void);
uint8_t isRespones(char*respones);
void sendNoAck(char* cmd);
uint8_t sendCmd(char *cmd,char* respones,uint16_t timeout);


#endif /* ESP8266_ESP8266_H_ */
