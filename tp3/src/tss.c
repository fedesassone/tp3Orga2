/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "gdt.h"
#include "mmu.h"  

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

//la vamos a llamar con cr3_inicial = 0x30000
void tss_iniciarTareas(){
	//en cada ciclo inicializamos un navio y una bandera
	unsigned int cr3_paCadaTarea;	
	int i;
	for(i = 0; i < 16; i= i+2){
		tss* tss_nueva = (tss*) mmu_proxima_pagina_fisica_libre();	
		//unsigned int cr3_paCadaTarea = cr3_inicial; //aca creo que estarian coincidiendo tss_nueva y cr3_paCadaTarea
		
		cr3_paCadaTarea = (unsigned int) mmu_inicializar_dir_tarea(i); //le pasamos el id

		//inicio navio
		tss_nueva->esp0 = (unsigned int) mmu_proxima_pagina_fisica_libre() + 0x1000; //le tiramos un cacho de memoria +1000 para q recorra hacia abajo
		unsigned pila_cero_tarea = tss_nueva->esp0;
		tss_nueva->ss0 = (GDT_IDX_DATA_0 << 3) | 3; // creo que va (GDT_IDX_DATA_0 << 3) | 3 
	    tss_nueva->cr3 = cr3_paCadaTarea;
	    tss_nueva->eip = DIR_VIRTUAL_TAREA;
	    tss_nueva->esp = 0x40001c00; // 0x40001C00
	    tss_nueva->ebp = 0x40001c00; //0x40001C00
	    tss_nueva->eflags = 0x202;
	    tss_nueva->es = (GDT_IDX_DATA_3 << 3) | 3;
	    tss_nueva->cs = (GDT_IDX_CODE_3 << 3) | 3;//aca creo que va (GDT_IDX_CODE_3 << 3) | 3 ( RPL = 3 )
	    tss_nueva->ss = (GDT_IDX_DATA_3 << 3) | 3;
	    tss_nueva->ds = (GDT_IDX_DATA_3 << 3) | 3;
	    tss_nueva->fs = (GDT_IDX_DATA_3 << 3) | 3;// no seria << en vez de |?
	    tss_nueva->gs = (GDT_IDX_DATA_3 << 3) | 3;//chequear esto
		//navio 1 ready
	    //inicio bandera 
		tss_nueva = (tss*) mmu_proxima_pagina_fisica_libre();			
		
		cr3_paCadaTarea = (unsigned int) mmu_inicializar_dir_tarea(i+1); //chequear que hace con el ID, le estamos pasando nums del 0 al 15

		//tss_nueva->esp0 = mmu_proxima_pagina_fisica_libre() + 0x1000; //le tiramos un cacho de memoria +1000 para q recorra hacia abajo
		tss_nueva->esp0 = pila_cero_tarea - 0x500;
		tss_nueva->ss0 = (GDT_IDX_DATA_0 << 3) | 3;  // creo que va (GDT_IDX_DATA_0 << 3) | 3 
	    tss_nueva->cr3 = cr3_paCadaTarea;
	    tss_nueva->eip = DIR_VIRTUAL_TAREA;
	    tss_nueva->esp = 0x40001FFC; // 0x40001FFC
	    tss_nueva->ebp = 0x40001FFC; // 0x40001FFC
	    tss_nueva->eflags = 0x202;
	    tss_nueva->es = (GDT_IDX_DATA_3 << 3) | 3;
	    tss_nueva->cs = (GDT_IDX_CODE_3 << 3) | 3;//aca creo que va (GDT_IDX_CODE_3 << 3) | 3 ( RPL = 3 )
	    tss_nueva->ss = (GDT_IDX_DATA_3 << 3) | 3;
	    tss_nueva->ds = (GDT_IDX_DATA_3 << 3) | 3;
	    tss_nueva->fs = (GDT_IDX_DATA_3 << 3) | 3;
	    tss_nueva->gs = (GDT_IDX_DATA_3 << 3) | 3;//chequear esto
		//mismo contexto que su navio (bandera)

	}


}

