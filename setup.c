/* 
 * File:   setup.c
 * Author: Pablo Caal
 *
 * Created on 4 de abril de 2022, 08:35 PM
 */

#include <xc.h>

void setup(void){
    
    ANSEL = 0;
    ANSELH = 0;                     // Habilitación de puertos como I/O digitales
    TRISA = 0x00;                   // Habilitación de PORTA como salida
    TRISB = 0x03;                   // Habilitación de PORTB, RB0 Y RB1 como entradas
    TRISC = 0x00;                   // Habilitación de PORTC como salida
    TRISD = 0x00;                   // Habilitación de PORTD como salida
    PORTA = 0x00;                   // Limpieza del PORTA
    PORTB = 0x00;                   // Limpieza del PORTB
    PORTC = 0x00;                   // Limpieza del PORTC
    PORTD = 0x00;                   // Limpieza del PORTD
    
    // Activación de bits de interrupciones a utilizar
    INTCONbits.GIE = 1;             // Habilitación de interrupciones globales
    INTCONbits.T0IE = 1;            // Habilitación de interrupciones del TIMER0
    INTCONbits.RBIE = 1;            // Habilitación de interrupciones del PORTB
    INTCONbits.T0IF = 0;            // Limpieza bandera de interrupción del TIMER0
    INTCONbits.RBIF = 0;            // Limpieza bandera de interrupción del PORTB
    
    // Configuración de interrupciones por cambio de estado para PORTB
    IOCBbits.IOCB0 = 1;             // Habilitación de interrupción por cambio de estado para RB0
    IOCBbits.IOCB1 = 1;             // Habilitación de interrupción por cambio de estado para RB1
    OPTION_REGbits.nRBPU = 0;       // Habilitación de resistencias de pull-up del PORTB
    WPUBbits.WPUB0 = 1;             // Habilitación de resistencia de pull-up de RB0
    WPUBbits.WPUB1 = 1;             // Habilitación de resistencia de pull-up de RB1
    
    // Configuración de TIMER0
    OPTION_REGbits.T0CS = 0;        // Configuración del TIMER0 como temporizador
    OPTION_REGbits.PSA = 0;         // Configuración del Prescaler para el TIMER0 y no para el Watchdog timer
    OPTION_REGbits.PS = 0b110;      // Prescaler de 1:128 PS<2:0> = 110
            
	/* Cálculo del valor a ingresar al TIMER0 para que tenga retardo de 100 ms
	; N = 256 - (Temp/(4 x Tosc x Presc))
	; N = 256 - (100 ms/(4 x (1/500 kHz) x 128))
	; N = 158 
     * Para el laboratorio
    */
    
    /* Cálculo del valor a ingresar al TIMER0 para que tenga retardo de 2 ms
	; N = 256 - (Temp/(4 x Tosc x Presc))
	; N = 256 - (2 ms/(4 x (1/500 kHz) x 128))
	; N = 254
     * Para el post laboratorio
    */
    
    TMR0 = 254;
    
    // Configuración del oscilador interno del PIC
    OSCCONbits.IRCF = 0b011;        // Configuración de frecuencia de oscilador a 500kHz
    OSCCONbits.SCS = 1;             // Oscilador interno
}