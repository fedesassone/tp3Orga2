/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tarea_inicial;
tss tarea_idle;

tss tss_navios[CANT_TAREAS];
tss tss_banderas[CANT_TAREAS];

void tss_inicializar() {

		// IDLE
	tarea_idle.esp 		= 0x0002A000;
	tarea_idle.ebp 		= 0x0002A000;
	tarea_idle.eip  	= 0x40000000;
	tarea_idle.cr3 		= 0x27000;//KERNEL PAGE DIRECTORY
	tarea_idle.es 		= 0x98;//seg datos 0
	tarea_idle.cs 		= 0xa8;//SEG Codigo 0
	tarea_idle.ss 		= 0x98;
	tarea_idle.ds 		= 0x98;
	tarea_idle.fs 		= 0x98;
	tarea_idle.gs 		= 0x98;
	tarea_idle.eflags 	= 0x00000202;
	tarea_idle.iomap 	= 0xFFFF;
	tarea_idle.esp0     = TASK_IDLE_STACK_RING_0; // aca no se si va esto o TASK_IDLE_STACK o 0x0002a000
	tarea_idle.ss0      = 0xa8;

}

