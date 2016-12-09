/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__
  
typedef struct str_sched_tarea
{
	unsigned short tss_selector;
	unsigned int id;
	unsigned int viva;
}__attribute__((__packed__)) sched_tarea;

typedef struct str_sched_t
{
	unsigned char tarea_actual;
	unsigned char bandera_actual;
	sched_tarea tareas[8];
	sched_tarea banderas[8];
}__attribute__((__packed__)) sched_t;



void sched_inicializar();
unsigned short sched_proximo_indice();


#endif	/* !__SCHED_H__ */
