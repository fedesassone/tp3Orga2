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
		//syscall_fondear(ebx);
		unsigned int directorio_tareas = rcr3(); //rcr3 creo que devuelve la dir fisica del cr3 actual
		mmu_mapear_pagina(TASK_ANCLA,directorio_tareas,ebx,1,0);

	}
	
	if ( eax == SYS_NAVEGAR)
	{
		//syscall_navegar(ebx,ecx);
		
		copiarCodigo(0x10000 + (0x1000*scheduler.tarea_actual), ebx); //copia la primera pagina de codigo a ebx
		copiarCodigo(0x10000 + (0x1000*scheduler.tarea_actual) + 0x1000, ecx); //copia la segunda pagina de codigo a ecx

	}
	if ( eax == SYS_CANONEAR )
	{
		//syscall_canonear(ebx,ecx);//ver lo de que es relativa
		
		unsigned int dir_absoluta = 0x40000000 + ecx;
		unsigned int i;
	    for(i = 0; i<97; i+=1)
	    {
	        *((unsigned char*) (ebx + i)) =  *((unsigned char *) (dir_absoluta + i));
	    }
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
	unsigned short i = 0x0;
	for(i=0x0; i< 0x8; i = i + 0x1){
		scheduler.tareas[i].tss_selector = ((GDT_TAREA_1 + i) << 3) | 3;
		scheduler.tareas[i].id = 0;
		scheduler.tareas[i].viva = 1;
	}
	for(i=0x0; i<0x8;i=i+0x1){
		scheduler.banderas[i].tss_selector = ((GDT_TAREA_1_BANDERA + i) << 3) | 3;
		scheduler.banderas[i].id = 0;
		scheduler.banderas[i].viva = 1;
	}

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
