/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/
#ifndef __ISR_H__
#define __ISR_H__
#include "i386.h"
#include "screen.h"
#include <stdio.h>
#include <stdlib.h>

void _isr0();
void _isr1();
void _isr2();
void _isr3();
void _isr4();
void _isr5();
void _isr6();
void _isr7();
void _isr8();
void _isr9();
void _isr10();

void _isr11();
void _isr12();
void _isr13();
void _isr14();
//void _isr15();
void _isr16();
void _isr17();
void _isr18();
void _isr19();
void _isr32();
void _isr33();
void _isr80();
void _isr102();


void handler_teclado(unsigned char scan_code)
{

    switch (scan_code)
    {
        case 0x02:
        	unsigned short attr = (C_FG_WHITE | C_BG_LIGHT_GREY);
        	print_int(1,79,0,attr)
            break;

        case 0x03:
 			unsigned short attr = (C_FG_WHITE | C_BG_BROWN );
        	print_int(2,79, unsigned int 0, unsigned short attr)
            break;

        case 0x04:
            unsigned short attr = (C_FG_WHITE | C_BG_MAGENTA);
        	print_int(3,79,0, unsigned short attr)

            break;

        case 0x05:	
            unsigned short attr = (C_FG_WHITE | C_BG_RED);
        	print_int(4,79,0, unsigned short attr)
            break;


        case 0x06:
            unsigned short attr = (C_FG_WHITE | C_BG_CYAN);
        	print_int(5,79,0, unsigned short attr)
            break;

        case 0x07:
            unsigned short attr = (C_FG_WHITE | C_BG_GREEN);
        	print_int(6,79,0, unsigned short attr)

            break;

        case 0x08:	
            unsigned short attr = (C_FG_WHITE | C_BG_BLUE);
        	print_int(7,79,0, unsigned short attr)
            break;


        case 0x09:
            unsigned short attr = (C_FG_WHITE | C_BG_LIGHT_GREY);
        	print_int(8,79,0, unsigned short attr)

            break;

        case 0x0a:	
            unsigned short attr = (C_FG_WHITE | C_BG_RED   );
        	print_int(9,79,0, unsigned short attr)
            break;
        case 0x0b:	
            unsigned short attr = (C_FG_WHITE | C_BG_BROWN);
        	print_int(0,79,0, unsigned short attr)
            break;
        case 0x32:	//M
        	if (!MuestroMapa){
        		cargarBufferMapa();
        		MuestroMapa = 1;
        	}

        
        case 0x12://	E
        	if (MuestroMapa)
        	{	
        		MuestroMapa = 0;
        		cargarBufferEstado();
        	}
            
             break;
    }
}

#endif  /* !__ISR_H__ */
