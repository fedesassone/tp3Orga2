/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#include "defines.h"

sched_t scheduler;


void sched_inicializar() {
	scheduler.tarea_actual = 0;
	scheduler.tareas[0].tss_selector = (GDT_TAREA_1 << 3) | 3;
	scheduler.tareas[0].id = 0;
	scheduler.tareas[1].tss_selector = (GDT_TAREA_2 << 3) | 3;
	scheduler.tareas[1].id = 1;
	scheduler.tareas[2].tss_selector = (GDT_TAREA_3 << 3) | 3;
	scheduler.tareas[2].id = 2;
	scheduler.tareas[3].tss_selector = (GDT_TAREA_4 << 3) | 3;
	scheduler.tareas[3].id = 3;
	scheduler.tareas[4].tss_selector = (GDT_TAREA_5 << 3) | 3;
	scheduler.tareas[4].id = 4;
	scheduler.tareas[5].tss_selector = (GDT_TAREA_6 << 3) | 3;
	scheduler.tareas[5].id = 5;
	scheduler.tareas[6].tss_selector = (GDT_TAREA_7 << 3) | 3;
	scheduler.tareas[6].id = 6;
	scheduler.tareas[7].tss_selector = (GDT_TAREA_8 << 3) | 3;
	scheduler.tareas[7].id = 7;
	scheduler.banderas[0].tss_selector = (GDT_TAREA_1_BANDERA << 3) | 3;
	scheduler.banderas[0].id = 0;
	scheduler.banderas[1].tss_selector = (GDT_TAREA_2_BANDERA << 3) | 3;
	scheduler.banderas[1].id = 1;
	scheduler.banderas[2].tss_selector = (GDT_TAREA_3_BANDERA << 3) | 3;
	scheduler.banderas[2].id = 2;
	scheduler.banderas[3].tss_selector = (GDT_TAREA_4_BANDERA << 3) | 3;
	scheduler.banderas[3].id = 3;
	scheduler.banderas[4].tss_selector = (GDT_TAREA_5_BANDERA << 3) | 3;
	scheduler.banderas[4].id = 4;
	scheduler.banderas[5].tss_selector = (GDT_TAREA_6_BANDERA << 3) | 3;
	scheduler.banderas[5].id = 5;
	scheduler.banderas[6].tss_selector = (GDT_TAREA_7_BANDERA << 3) | 3;
	scheduler.banderas[6].id = 6;
	scheduler.banderas[7].tss_selector = (GDT_TAREA_8_BANDERA << 3) | 3;
	scheduler.banderas[7].id = 7;
}

/*
unsigned short sched_proximo_indice() {

	if (scheduler.tarea_actual == 7)
	{
		scheduler.tarea_actual = 0;
	}
    
}*/

