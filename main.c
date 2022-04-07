/* 
 * File:   main.c
 * Author: Pablo Caal
 *
 * Laboratorio 07
 * 
 * Created on 4 de abril de 2022, 08:37 PM
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include "setup.h"

#define _XTAL_FREQ 500000

/*------------------------------------------------------------------------------
 * VARIABLES 
 ------------------------------------------------------------------------------*/
uint8_t numero = 123, contador = 0;               // Declaramos la variable con el número de 3 dígitos a mostrar en los displays
int num[3] = {0, 0, 0};             // En este arreglo se dividirá el número de 3 dígitos en unidades, decenas y centenas
uint8_t Bandera_multiplexado = 0;   // Utilizamos una variable como bandera para el multiplexado de los displays

/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/
void setup(void);                           // Prototipo de función de configuración
void separacion(uint8_t a, int num[]);      // Prototipo de función de separación del número de 3 dígitos
void mostrar(int num[]);                    // Prototipo de función para mostrar valores en displays
uint8_t tabla(int numero);                  // Prototipo de función de tabla de conversión para displays

/*------------------------------------------------------------------------------
 * INTERRUPCIONES 
 ------------------------------------------------------------------------------*/
void __interrupt() isr (void){
    if(INTCONbits.RBIF){                    // Verificación de interrupción del PORTB
        if(!PORTBbits.RB0){                 // Verificación de interrupción de RB0
            //while(!PORTBbits.RB0);        // ANTIREBOTE (no funciona))
            contador++;                        // Incremento del PORTA (INCF PORTA)                      
        } else if(!PORTBbits.RB1){          // Verificación de interrupción de RB1
            //while(!PORTBbits.RB1);        // ANTIREBOTE (no funciona)) 
            contador--;                        // Decremento del PORTA (DECF PORTA)
        }            
        INTCONbits.RBIF = 0;                // Limpieza de bandera de interrupción del PORTB
    }
    else if(INTCONbits.T0IF){               // Verificación de interrupción del TIMER0 
        mostrar(num);                       // Llamado a función para mostrar en displays
        Bandera_multiplexado++;             // Incremento de variable de bandera para multiplexado
        if(Bandera_multiplexado>2){         // Si la bandera es mayor a 2 reiniciar
            Bandera_multiplexado = 0;
        }        
        INTCONbits.T0IF = 0;                // Limpieza de bandera de interrupción del TIMER0
        TMR0 = 254;                         // Ingreso de número correspondiente para retardo del TIMER0
    }
    return;
}

/*------------------------------------------------------------------------------
 * CICLO PRINCIPAL
 ------------------------------------------------------------------------------*/
void main(void) {
    setup();                                // Llamado de función de configuración general
    separacion(numero, num);                // Llamado de función de separación de dígitos del número
    contador = 0;
    while(1){                               // Ciclo principal
        PORTA = contador;
    }
    return;
}

/*------------------------------------------------------------------------------
 * FUNCIONES 
 ------------------------------------------------------------------------------*/
// Función de separación de un número en unidades, decenas y centenas
void separacion(uint8_t a, int num[]){      
    num[1] = a/100;                             // Centenas
    num[2] = (a-(100*(num[1])))/10;             // Decenas
    num[3] = (a-(100*(num[1])+10*(num[2])));    // Unidades
    return;
}

// Función para mostrar los valores en los distintos displays
void mostrar(int num[]){
    PORTD = 0x00;
    PORTC = 0x00;
    switch(Bandera_multiplexado){
        case 0:                                 // Display de centenas
            PORTDbits.RD2 = 1;                  // Activación de bit RD2
            PORTC = tabla(num[1]);              // Conversión de dígito de centenas
            break;
        case 1:                                 // Display de decenas
            PORTDbits.RD1 = 1;                  // Activación de bit RD1
            PORTC = tabla(num[2]);              // Conversión de dígito de decenas 
            break;  
        case 2:                                 // Display de unidades
            PORTDbits.RD0 = 1;                  // Activación de bit RD0
            PORTC = tabla(num[3]);              // Conversión de dígito de unidades
            break;
        default:
            break;
    }
    return;
}

// Función de tabla para conversión de números para mostrar en display de 7 segmentos
uint8_t tabla(int numero){
    uint8_t valor = 0;
    switch(numero){
        case 0:                 // Número = 0
            valor = 0x3F;       // Valor para mostrar 0 en display de 7 segmentos
            break;
        case 1:                 // Número = 1
            valor = 0x06;       // Valor para mostrar 1 en display de 7 segmentos
            break;
        case 2:                 // Número = 2
            valor = 0x5B;       // Valor para mostrar 2 en display de 7 segmentos
            break;
        case 3:                 // Número = 3
            valor = 0x4F;       // Valor para mostrar 3 en display de 7 segmentos
            break;
        case 4:                 // Número = 4
            valor = 0x66;       // Valor para mostrar 4 en display de 7 segmentos
            break;
        case 5:                 // Número = 5
            valor = 0x6D;       // Valor para mostrar 5 en display de 7 segmentos
            break;
        case 6:                 // Número = 6
            valor = 0x7D;       // Valor para mostrar 6 en display de 7 segmentos
            break;
        case 7:                 // Número = 7
            valor = 0x07;       // Valor para mostrar 7 en display de 7 segmentos
            break;
        case 8:                 // Número = 8
            valor = 0x7F;       // Valor para mostrar 8 en display de 7 segmentos
            break;
        case 9:                 // Número = 9
            valor = 0x6F;       // Valor para mostrar 9 en display de 7 segmentos
            break;
    }
    return valor;               // Retorno de valor ingresador para mostrar en display
}