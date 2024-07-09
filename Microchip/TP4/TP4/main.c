#include <avr/io.h>
#include <avr/interrupt.h> // Librer?a para el manejo de interrupciones en AVR

#include "PWM.h"
#include "pot.h"
#include "serialPort.h"


#define F_CPU 16000000UL // Especifico la frecuencia de reloj del MCU en 16MHz
#include <util/delay.h> // Retardos por software ? Macros: depende de F_CPU

#define BR9600 (0x67)	// 0x67=103 configura BAUDRATE=9600@16MHz

char msg1[] = "Control de color de un RGB\r\n";
char msgR[] = "Modificando color rojo\r\n";
char msgG[] = "Modificando color verde\r\n";
char msgB[] = "Modificando color azul\r\n";
char msgF[] = "Fin de modificacion\r\n";

uint8_t active = 0;
uint8_t CAct;

int main(void)
{
	uint8_t cant = 0;
	RGB color;
	
	PWM_Init();
	ADC_Init();
	SerialPort_Init(BR9600); 		// Inicializo formato 8N1 y BAUDRATE = 9600bps
	SerialPort_TX_Enable();			// Activo el Transmisor del Puerto Serie
	SerialPort_RX_Enable();			// Activo el Receptor del Puerto Serie
	SerialPort_Send_String(msg1);   // Envío el mensaje de Bienvenida
	SerialPort_RX_Interrupt_Enable();	// Activo Interrupción de recepcion.
	sei();
	
	color.R = 0;
	color.G = 0;
	color.B = 0;
    while (1) 
    {
		if (active){
			cant = getCant();
			switch (CAct){
				case 'R':
					SetCT_Red(cant);
				break;
				case 'G':
					SetCT_Green(cant);
				break;
				case 'B':
					SetCT_Blue(cant);
				break;
			}
// 
// 			SerialPort_Send_uint8_t(cant);
// 			SerialPort_Send_String("\r\n");
		}
    }
}

// Rutina de Servicio de Interrupción de Byte Recibido
ISR(USART_RX_vect){
	char RX_Buffer = UDR0; //la lectura del UDR borra flag RXC
	// Si se presiona 'f' se alterna la transmision de datos
	if((RX_Buffer == 'f')||(RX_Buffer == 'F')){ // Si se presiono una F
		active = 0; // Se finaliza la modificacion
		SerialPort_Send_String(msgF);   // Envío el mensaje de Bienvenida
	}
	// Si se presiona 'R' se alterna la transmision de datos
	if((RX_Buffer == 'r')||(RX_Buffer == 'R')){ // Si se presiono una R y no se esta modificando otro color
		active = 1; // Se activa la modificacion de color
		CAct = 'R';
		SerialPort_Send_String(msgR);   // Envío el mensaje de Bienvenida
	}
	// Si se presiona 'G' se alterna la transmision de datos
	if((RX_Buffer == 'g')||(RX_Buffer == 'G')){ // Si se presiono una G
		active = 1; // Se activa la modificacion de color
		CAct = 'G';
		SerialPort_Send_String(msgG);   // Envío el mensaje de Bienvenida
	}
	// Si se presiona 'B' se alterna la transmision de datos
	if((RX_Buffer == 'b')||(RX_Buffer == 'B')){ // Si se presiono una B
		active = 1; // Se activa la modificacion de color
		CAct = 'B';
		SerialPort_Send_String(msgB);   // Envío el mensaje de Bienvenida
	}
	RX_Buffer=0;
}
