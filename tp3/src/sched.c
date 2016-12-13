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
#include "screen.h"



unsigned int tareasRestantes;
unsigned short corriendoTareas;
unsigned short corriendoBandera;
unsigned short muestroMapa;
sched_t scheduler;

unsigned int tarea_actual(){
	return (unsigned int) scheduler.tarea_actual;
}


void handler_teclado(unsigned char scan_code){
    
    unsigned short attr;

    switch (scan_code)
    {
        case 0x02:
            attr = (C_FG_WHITE | C_BG_LIGHT_GREY);
            print_int(VIDEO,1,79,0,attr);
            break;

        case 0x03:
            attr = (C_FG_WHITE | C_BG_BROWN);
            print_int(VIDEO,2,79, 0, attr);
            break;

        case 0x04:
            attr = (C_FG_WHITE | C_BG_MAGENTA);
            print_int(VIDEO,3,79,0, attr);

            break;

        case 0x05:  
            attr = (C_FG_WHITE | C_BG_RED);
            print_int(VIDEO,4,79,0, attr);
            break;


        case 0x06:
            attr = (C_FG_WHITE | C_BG_CYAN);
            print_int(VIDEO,5,79,0, attr);
            break;

        case 0x07:
            attr = (C_FG_WHITE | C_BG_GREEN);
            print_int(VIDEO,6,79,0, attr);

            break;

        case 0x08:  
            attr = (C_FG_WHITE | C_BG_BLUE);
            print_int(VIDEO,7,79,0, attr);
            break;


        case 0x09:
            attr = (C_FG_WHITE | C_BG_LIGHT_GREY);
            print_int(VIDEO,8,79,0, attr);
            break;

        case 0x0a:  
            attr = (C_FG_WHITE | C_BG_RED);
            print_int(VIDEO,9,79,0, attr);
            break;
        case 0x0b:  
            attr = (C_FG_WHITE | C_BG_BROWN);
            print_int(VIDEO,0,79,0, attr);
            break;
        case 0x32:  //M
            if (!muestroMapa){
                cargarBufferMapa();
                muestroMapa = 1;
            }
        case 0x12://    E
            if (muestroMapa)
            {   
                muestroMapa = 0;
                cargarBufferEstado();
            }
            
             break;
    }
}


void llamada (unsigned int eax,unsigned int ebx, unsigned int ecx)
{
	//scheduler.paginas.idTarea = scheduler.tarea_actual +1;
	if ( eax == SYS_FONDEAR) //cambia ancla, actualiza pag, hay que llamar a buffer
	{
		//syscall_fondear(ebx);
		unsigned int directorio_tareas = rcr3(); //rcr3 creo que devuelve la dir fisica del cr3 actual
		mmu_mapear_pagina(TASK_ANCLA,directorio_tareas,ebx,1,0);
		//scheduler.paginas.p3=(unsigned int)ebx;
	}
	
	if ( eax == SYS_NAVEGAR)// actualiza pag, hay que llamar a buffer
	{
		//syscall_navegar(ebx,ecx);
		
		copiarCodigo(0x10000 + (0x1000*scheduler.tarea_actual), ebx); //copia la primera pagina de codigo a ebx
		copiarCodigo(0x10000 + (0x1000*scheduler.tarea_actual) + 0x1000, ecx); //copia la segunda pagina de codigo a ecx
		//scheduler.paginas.p1=(unsigned int)ebx;
		//scheduler.paginas.p2=(unsigned int)ecx;
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
	//actualizarBufferEstado_Paginas();

}


unsigned short atender_int66(){
	if(corriendoBandera==1){
		//scheduler.bandera_actual = scheduler.bandera_actual+1;
		//if(scheduler.bandera_actual ==)
		corriendoBandera = 0;
		return (GDT_TAREA_IDLE<<3);
	}	
	if ( corriendoTareas == 1){
		return matar_tarea();
	}
	return 0; //nunca llega acá
}



void sched_inicializar() {
	scheduler.tarea_actual = 7;
	scheduler.bandera_actual = 7;
	scheduler.banderasVivas=8;
	tareasRestantes = 3;
	corriendoTareas = 1;
	corriendoBandera = 0;
	muestroMapa =0;
	unsigned short i = 0x0;
	for(i=0x0; i< 0x8; i = i + 0x1){
		scheduler.tareas[i].tss_selector = ((GDT_TAREA_1 + i) << 3) | 3;
		scheduler.tareas[i].id = 0;
		scheduler.tareas[i].viva = 1;
	}
	for(i=0x0; i<0x8;i=i+0x1){
		scheduler.banderas[i].tss_selector = 
		((GDT_TAREA_1_BANDERA + i) << 3) | 3;
		scheduler.banderas[i].id = 0;
		scheduler.banderas[i].viva = 1;
	}
	//inicializo las paginas de las tareas
	for ( i=0;i<8;i++)
	{
		scheduler.paginasTareas[i].p1 = 0x10000 + (0x1000*i*0x2);
		scheduler.paginasTareas[i].p2 = 0x10000 + (0x1000*i*0x2) + 0x1000;
		scheduler.paginasTareas[i].p3 = 0x0;

	}
	//scheduler.paginasTareas[1].p1 = 0x12000;
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
	actualizarBufferEstado_Bandera_i(scheduler.bandera_actual);
	if(scheduler.banderasPorCiclar==0){
		corriendoTareas = 1;
		tareasRestantes = 3;
		corriendoBandera = 0;
		return (GDT_TAREA_IDLE<<3);
	}
	scheduler.bandera_actual = scheduler.bandera_actual + 1 ;
	if (scheduler.bandera_actual == 8) 
	{
		scheduler.bandera_actual = 0;
	}
	while ( scheduler.banderas[scheduler.bandera_actual].viva == 0 )
	{
		scheduler.bandera_actual = scheduler.bandera_actual + 1 ;

	}// salgo del while con la proxima bandera viva
    //fijarse si es la ultima
    scheduler.banderasPorCiclar--;
    return scheduler.banderas[scheduler.bandera_actual].tss_selector;
}


unsigned short atender_reloj(){
	if(corriendoTareas==1){
		if(tareasRestantes == 0){	//si me quede sin tareas
			corriendoTareas = 0;
			corriendoBandera = 1;	
			scheduler.bandera_actual = 7;
			scheduler.banderasPorCiclar = scheduler.banderasVivas;
			return sched_proxima_bandera();	//corro primer bandera
		}
		tareasRestantes--;
		return sched_proximo_indice(); //sigo corriendo tarea
	}
	//corriendo banderas
	if ( corriendoBandera == 1){
		corriendoBandera = 0;
		return matar_tarea(); //estaba corriendo bandera y me corto el clock
	}
	//si estoy en 0,0; vengo de la idle y estoy ciclando banderas
	corriendoBandera = 1;
	return sched_proxima_bandera(); //no estoy ni corriendo banderas, ni tareas => vengo de idle y ciclo bander
}

unsigned short matar_tarea()
{
	scheduler.tareas[scheduler.tarea_actual].viva = 0; //mato tarea
	scheduler.banderas[scheduler.tarea_actual].viva = 0;//mato bandera
	scheduler.banderasVivas--;
	//return atender_sched();
	//tenemos que saltar a la idle desde acá, 
	//matarEnBuffer();
	return 0xc0; //selector de idle
}
