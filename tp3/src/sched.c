/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#include "defines.h"
#include "i386.h"
#include "mmu.h"
#include "syscall.h"

sched_t scheduler;
unsigned int tareasRestantes;
unsigned short corriendoTareas;
unsigned short corriendoBandera;
void llamada (unsigned int eax,unsigned int ebx, unsigned int ecx)
{
	if ( eax == SYS_FONDEAR)
	{
		syscall_fondear(ebx);
		//unsigned int directorio_tareas = rcr3(); //rcr3 creo que devuelve la dir fisica del cr3 actual
		//mmu_mapear_pagina(TASK_ANCLA,directorio_tareas,ebx,1,0);
	}
	
	if ( eax == SYS_NAVEGAR)
	{
		syscall_navegar(ebx,ecx);
		//copiarCodigo deberia copiar una sola pagina
		//copiarCodigo(0x10000 + (0x1000*scheduler.tarea_actual), ebx); //copia la primera pagina de codigo a ebx
		//copiarCodigo(0x10000 + (0x1000*scheduler.tarea_actual) + 0x1000, ecx); //copia la segunda pagina de codigo a ecx

	}
	if ( eax == SYS_CANONEAR )
	{
		syscall_canonear(ebx,ecx);//ver lo de que es relativa
	}
}
void llamoTarea()
{
	corriendoBandera = 0;
	if (corriendoTareas == 1)
	{
		scheduler.tareas[scheduler.tarea_actual].viva = 0; // si una tarea llama a int 66 se muere la tarea
		scheduler.banderas[scheduler.tarea_actual].viva = 0;// y su bandera
	}
}



void sched_inicializar() {
	scheduler.tarea_actual = 7;
	scheduler.bandera_actual = 7;
	tareasRestantes = 3;
	corriendoTareas = 1;
	corriendoBandera = 0;
	int i;
	for(i=0; i<8;i++){
		scheduler.tareas[i].tss_selector = ((GDT_TAREA_1 + i) << 3) | 3;
		scheduler.tareas[i].id = 0;
		scheduler.tareas[i].viva = 1;
	}
	for(i=0; i<8;i++){
		scheduler.banderas[i].tss_selector = ((GDT_TAREA_1_BANDERA + i) << 3) | 3;
		scheduler.banderas[i].id = 0;
		scheduler.banderas[i].viva = 1;
	}

	
	// scheduler.tareas[1].tss_selector = (GDT_TAREA_2 << 3) | 3;
	// scheduler.tareas[1].id = 1;
	// scheduler.tareas[1].viva = 1;
	// scheduler.tareas[2].tss_selector = (GDT_TAREA_3 << 3) | 3;
	// scheduler.tareas[2].id = 2;
	// scheduler.tareas[2].viva = 1;
	// scheduler.tareas[3].tss_selector = (GDT_TAREA_4 << 3) | 3;
	// scheduler.tareas[3].id = 3;
	// scheduler.tareas[3].viva = 1;
	// scheduler.tareas[4].tss_selector = (GDT_TAREA_5 << 3) | 3;
	// scheduler.tareas[4].id = 4;
	// scheduler.tareas[4].viva = 1;
	// scheduler.tareas[5].tss_selector = (GDT_TAREA_6 << 3) | 3;
	// scheduler.tareas[5].id = 5;
	// scheduler.tareas[5].viva = 1;
	// scheduler.tareas[6].tss_selector = (GDT_TAREA_7 << 3) | 3;
	// scheduler.tareas[6].id = 6;
	// scheduler.tareas[6].viva = 1;
	// scheduler.tareas[7].tss_selector = (GDT_TAREA_8 << 3) | 3;
	// scheduler.tareas[7].id = 7;
	// scheduler.tareas[7].viva = 1;

	// scheduler.banderas[1].tss_selector = (GDT_TAREA_2_BANDERA << 3) | 3;
	// scheduler.banderas[1].id = 1;
	// scheduler.banderas[1].viva = 1;
	// scheduler.banderas[2].tss_selector = (GDT_TAREA_3_BANDERA << 3) | 3;
	// scheduler.banderas[2].id = 2;
	// scheduler.banderas[2].viva = 1;
	// scheduler.banderas[3].tss_selector = (GDT_TAREA_4_BANDERA << 3) | 3;
	// scheduler.banderas[3].id = 3;
	// scheduler.banderas[3].viva = 1;
	// scheduler.banderas[4].tss_selector = (GDT_TAREA_5_BANDERA << 3) | 3;
	// scheduler.banderas[4].id = 4;
	// scheduler.banderas[4].viva = 1;
	// scheduler.banderas[5].tss_selector = (GDT_TAREA_6_BANDERA << 3) | 3;
	// scheduler.banderas[5].id = 5;
	// scheduler.banderas[5].viva = 1;
	// scheduler.banderas[6].tss_selector = (GDT_TAREA_7_BANDERA << 3) | 3;
	// scheduler.banderas[6].id = 6;
	// scheduler.banderas[6].viva = 1;
	// scheduler.banderas[7].tss_selector = (GDT_TAREA_8_BANDERA << 3) | 3;
	// scheduler.banderas[7].id = 7;
	// scheduler.banderas[7].viva = 1;


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


unsigned short atender_sched(){
	if(corriendoTareas==1){
		tareasRestantes--;
		if(tareasRestantes == 0){
			corriendoTareas = 0; //voy a correr banderas
			corriendoBandera = 1;
			scheduler.bandera_actual = 7;
			return sched_proxima_bandera();
		}
		return sched_proximo_indice();
	}
	//corriendo banderas
	if ( corriendoBandera == 1)
	{
		scheduler.tareas[scheduler.tarea_actual].viva = 0; // si cae una int de reloj mientras estaba corriendo una bandera se muere la bandera
		scheduler.banderas[scheduler.tarea_actual].viva = 0;// y su bandera
	}
	unsigned short ultimaViva = 0;
	unsigned int i;
	for(i=0; i<8;i++){
		if(scheduler.banderas[i].viva == 1) ultimaViva = i;
	}
	//si es la ultima, termino
	if(scheduler.bandera_actual == ultimaViva){
		corriendoTareas = 1;
		tareasRestantes = 3;
		return sched_proximo_indice();
	}
	//si no, paso bandera
	return sched_proxima_bandera();
}

unsigned short matar_tarea()
{
	scheduler.tareas[scheduler.tarea_actual].viva = 0; //mato tarea
	scheduler.banderas[scheduler.tarea_actual].viva = 0;//mato bandera
	return atender_sched();
}
