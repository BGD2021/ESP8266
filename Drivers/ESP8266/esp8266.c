/*
 * esp8266.c
 *
 *  Created on: Jul 24, 2022
 *      Author: BGD
 */
#include "esp8266.h"
#include "main.h"
#include "string.h"

/*变量区*/
char receive_data[500];
uint8_t isRes=0;
/*重定向*/
#include "stdio.h"
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&ESPhuart, (uint8_t*)&ch,1,HAL_MAX_DELAY);
    return ch;
}


uint8_t Respones(char* respones,char* rxbuffer)
{//查找子串
	char *str_rx = rxbuffer;
	char *str_res = respones;
	char *current = rxbuffer;
	while(*current){
		str_rx = current;
		str_res = respones;
		while(*str_res && *str_rx && (*str_res == *str_rx)){
			str_res++;
			str_rx++;
		}
		if(*str_res == '\0'){
			return success;
		}
		current++;
	}
	return fail;
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if(huart->Instance == USART8266){
		HAL_UART_DMAStop(huart);
//		printf(receive_data);//调试使用
		isRes = Respones("OK", receive_data);

		memset(receive_data,0,sizeof(receive_data));//清空数组
		HAL_UARTEx_ReceiveToIdle_DMA(&huart6, (uint8_t*)receive_data, 500);
	}
}




uint8_t sendCmd(char *cmd,char* respones,uint16_t timeout)
{
	isRes=0;
	printf("%s\r\n",cmd);

	while(timeout>0 && !isRes){//未超时 未回应
		HAL_Delay(1);
		timeout--;
		if(!timeout){
			return fail;//超时
		}
	}
	return success;

}

void sendNoAck(char* cmd)
{
	printf("%s\r\n",cmd);
	HAL_Delay(50);
}

void quitTrans(void)
{
	HAL_UART_Transmit(&ESPhuart, (uint8_t *)"+++", 3, 1000);
	HAL_Delay(1000);
}

void ESP8266_STA_init(void)
{
//	quitTrans();
	sendNoAck("AT+RST");
	HAL_Delay(10000);
	sendCmd("ATE0", "OK", 10000);
	sendCmd("AT+CWMODE=1", "OK", 1000);
	sendCmd("AT+CWJAP=\"dxxy16-402-1\",\"dxxy16402\"","OK",6000);
	sendCmd("AT+CIPSTART=\"TCP\",\"192.168.0.189\",233", "OK", 6000);
	sendCmd("AT+CIPMODE=1","OK",1000);
	sendNoAck("AT+CIPSEND");//进入透传
	sendNoAck("初始化完成");

}


