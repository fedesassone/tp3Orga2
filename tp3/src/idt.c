/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "idt.h"
  #include "colors.h"
  #include "isr.h"
#include "screen.h"
idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (unsigned int) &idt
};


/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);

        ...
    }
*/


#define IDT_ENTRY(numero)                                                                                        \
    idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
    idt[numero].segsel = (unsigned short) 0x00a8;                                                                  \
    idt[numero].attr = (unsigned short) 0x8e00;                                                                  \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);

/*void atender_int(unsigned short n){
    unsigned int x = 10;
    unsigned int y = 10;
    unsigned int attr = ((C_FG_WHITE | C_BG_BLACK ));
    char l = ' ';
    char *c = &l;
    if(n==0)c= "Divide Error";
    if(n==1)c= "RESERVED";
    if(n==2)c= "NMI Interrupt";
    if(n==3)c= "Breakpoint";
    if(n==4)c= "Overflow";
    if(n==5)c= "BOUND Range Exceeded";
    if(n==6)c= "Invalid Opcode (Undefined Opcode)";
    if(n==7)c= "Device Not Available (No Math Coprocessor";
    if(n==8)c= "Double Fault";
    if(n==9)c= "Coprocessor Segment Overrun (reserved)";
    if(n==10)c= "Invalid TSS";
    if(n==11)c=  "Segment Not Present";
    if(n==12)c= "Stack-Segment Fault";
    if(n==13)c= "General Protection";
    if(n==14)c= "Page Fault";
    //if(n==15)c= "Page Fault";
    if(n==16)c= "x87 FPU Floating-Point Error (Math Fault)";
    if(n==17)c=  "Alignment Check";
    if(n==18)c= "Machine Check";
    if(n==19)c= "SIMD Floating-Point Exception";

    print_int(n,x,y,attr);
}
*/

void idt_inicializar() {
        IDT_ENTRY(0);
        IDT_ENTRY(1);
        IDT_ENTRY(2);
        IDT_ENTRY(3);
        IDT_ENTRY(4);
        IDT_ENTRY(5);
        IDT_ENTRY(6);
        IDT_ENTRY(7);
        IDT_ENTRY(8);
        IDT_ENTRY(9);
        IDT_ENTRY(10);
        IDT_ENTRY(11);
        IDT_ENTRY(12);
        IDT_ENTRY(13);
        IDT_ENTRY(14);
        //IDT_ENTRY(15);
        IDT_ENTRY(16);
        IDT_ENTRY(17);
        IDT_ENTRY(18);
        IDT_ENTRY(19);
        IDT_ENTRY(32);//RELOJ
        IDT_ENTRY(33);//TECLADO
        idt[80].offset_0_15 = (unsigned short) ((unsigned int)(&_isr80) & (unsigned int) 0xFFFF);        
        idt[80].segsel = (unsigned short) 0x00a8;                                                                  
        idt[80].attr = (unsigned short) 0xee00;// LE PONGO DPL 3 YA QUE VA A SER USADA POR UN USUARIO                                                                  
        idt[80].offset_16_31 = (unsigned short) ((unsigned int)(&_isr80) >> 16 & (unsigned int) 0xFFFF);
        idt[102].offset_0_15 = (unsigned short) ((unsigned int)(&_isr102) & (unsigned int) 0xFFFF);        
        idt[102].segsel = (unsigned short) 0x00a8;                                                                  
        idt[102].attr = (unsigned short) 0xee00;// LE PONGO DPL 3 YA QUE VA A SER USADA POR UN USUARIO                                                                  
        idt[102].offset_16_31 = (unsigned short) ((unsigned int)(&_isr102) >> 16 & (unsigned int) 0xFFFF);

    }


