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
	scheduler.tareas[0].viva = 1;
	scheduler.tareas[1].tss_selector = (GDT_TAREA_2 << 3) | 3;
	scheduler.tareas[1].id = 1;
	scheduler.tareas[1].viva = 1;
	scheduler.tareas[2].tss_selector = (GDT_TAREA_3 << 3) | 3;
	scheduler.tareas[2].id = 2;
	scheduler.tareas[2].viva = 1;
	scheduler.tareas[3].tss_selector = (GDT_TAREA_4 << 3) | 3;
	scheduler.tareas[3].id = 3;
	scheduler.tareas[3].viva = 1;
	scheduler.tareas[4].tss_selector = (GDT_TAREA_5 << 3) | 3;
	scheduler.tareas[4].id = 4;
	scheduler.tareas[4].viva = 1;
	scheduler.tareas[5].tss_selector = (GDT_TAREA_6 << 3) | 3;
	scheduler.tareas[5].id = 5;
	scheduler.tareas[5].viva = 1;
	scheduler.tareas[6].tss_selector = (GDT_TAREA_7 << 3) | 3;
	scheduler.tareas[6].id = 6;
	scheduler.tareas[6].viva = 1;
	scheduler.tareas[7].tss_selector = (GDT_TAREA_8 << 3) | 3;
	scheduler.tareas[7].id = 7;
	scheduler.tareas[7].viva = 1;
	scheduler.banderas[0].tss_selector = (GDT_TAREA_1_BANDERA << 3) | 3;
	scheduler.banderas[0].id = 0;
	scheduler.banderas[0].viva = 1;
	scheduler.banderas[1].tss_selector = (GDT_TAREA_2_BANDERA << 3) | 3;
	scheduler.banderas[1].id = 1;
	scheduler.banderas[1].viva = 1;
	scheduler.banderas[2].tss_selector = (GDT_TAREA_3_BANDERA << 3) | 3;
	scheduler.banderas[2].id = 2;
	scheduler.banderas[2].viva = 1;
	scheduler.banderas[3].tss_selector = (GDT_TAREA_4_BANDERA << 3) | 3;
	scheduler.banderas[3].id = 3;
	scheduler.banderas[3].viva = 1;
	scheduler.banderas[4].tss_selector = (GDT_TAREA_5_BANDERA << 3) | 3;
	scheduler.banderas[4].id = 4;
	scheduler.banderas[4].viva = 1;
	scheduler.banderas[5].tss_selector = (GDT_TAREA_6_BANDERA << 3) | 3;
	scheduler.banderas[5].id = 5;
	scheduler.banderas[5].viva = 1;
	scheduler.banderas[6].tss_selector = (GDT_TAREA_7_BANDERA << 3) | 3;
	scheduler.banderas[6].id = 6;
	scheduler.banderas[6].viva = 1;
	scheduler.banderas[7].tss_selector = (GDT_TAREA_8_BANDERA << 3) | 3;
	scheduler.banderas[7].id = 7;
	scheduler.banderas[7].viva = 1;
}


unsigned short sched_proximo_indice() {

	scheduler.tarea_actual = scheduler.tarea_actual + 1 ;
	if (scheduler.tarea_actual == 8) 
	{
		scheduler.tarea_actual = 0;
	}

	while ( scheduler.tareas[scheduler.tarea_actual].viva == 0 )
	{
		scheduler.tarea_actual = scheduler.tarea_actual + 1 ;

	}// salgo del while con la proxima tarea viva
    
    return scheduler.tareas[scheduler.tarea_actual].tss_selector;
}

unsigned short sched_proxima_bandera() {

	scheduler.bandera_actual = scheduler.bandera_actual + 1 ;
	if (scheduler.bandera_actual == 8) 
	{
		scheduler.bandera_actual = 0;
	}

	while ( scheduler.banderas[scheduler.bandera_actual].viva == 0 )
	{
		scheduler.bandera_actual = scheduler.bandera_actual + 1 ;

	}// salgo del while con la proxima bandera viva
    
    return scheduler.banderas[scheduler.bandera_actual].tss_selector;
}

