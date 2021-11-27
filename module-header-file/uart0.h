/*
 * uart0.h
 *
 * Created: 2021-08-17 오전 10:26:36
 *  Author: kcci
 */ 


#ifndef UART0_H_
#define UART0_H_

void UART0_init(void);
void UART0_transmit(char data);
unsigned char UART0_receive(void);
void UART0_print_string(char *str);
void UART0_tx_rx_test(void);
uint8_t isRxString();
uint8_t *getRxString();

#endif /* UART0_H_ */