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
#include "tss.h"



unsigned int tareasRestantes;
unsigned short corriendoTareas;
unsigned short corriendoBandera;
unsigned short muestroMapa;
unsigned int ultimoMisil = 2;//arranco con un valor cualquiera para saber si es el primer cañon que tiran
sched_t scheduler;

unsigned char fueErrorBandera(){
	if(corriendoBandera==1)return 1;
	return 0;
}

unsigned int tarea_actual(){
	return (unsigned int) scheduler.tarea_actual;
}
unsigned int tarea_o_bandera_actual()
{
	if ( corriendoBandera == 1)
	{
		return scheduler.bandera_actual;
	}
	if ( corriendoTareas == 1)
	{
		return scheduler.tarea_actual;
	}
	return 0;//nunca llega aca 
}

unsigned short matar()
{
	if ( corriendoBandera == 1 )
	{
		return matar_bandera();
	}
	if ( corriendoTareas == 1 )
	{
		return matar_tarea();
	}
	return 0;//nunca llega aca 
}


void handler_teclado(unsigned char scan_code){
    unsigned short attr;
    const char *m = (const char*)'M';
    const char *e = (const char*)'E';
    //CASO ESTOY EN ESTADO
	if(scheduler.mostrarEstado == 1){
	    switch (scan_code)
	    {
	        case 0x02:
	            attr = (C_FG_WHITE | C_BG_LIGHT_GREY);
	            print_int(BUFFER_ESTADO,1,79,0,attr);
	            break;

	        case 0x03:
	            attr = (C_FG_WHITE | C_BG_BROWN);
	            print_int(BUFFER_ESTADO,2,79, 0, attr);
	            break;

	        case 0x04:
	            attr = (C_FG_WHITE | C_BG_MAGENTA);
	            print_int(BUFFER_ESTADO,3,79,0, attr);

	            break;

	        case 0x05:  
	            attr = (C_FG_WHITE | C_BG_RED);
	            print_int(BUFFER_ESTADO,4,79,0, attr);
	            break;


	        case 0x06:
	            attr = (C_FG_WHITE | C_BG_CYAN);
	            print_int(BUFFER_ESTADO,5,79,0, attr);
	            break;

	        case 0x07:
	            attr = (C_FG_WHITE | C_BG_GREEN);
	            print_int(BUFFER_ESTADO,6,79,0, attr);

	            break;

	        case 0x08:  
	            attr = (C_FG_WHITE | C_BG_BLUE);
	            print_int(BUFFER_ESTADO,7,79,0, attr);
	            break;


	        case 0x09:
	            attr = (C_FG_WHITE | C_BG_LIGHT_GREY);
	            print_int(BUFFER_ESTADO,8,79,0, attr);
	            break;

	        case 0x0a:  
	            attr = (C_FG_WHITE | C_BG_RED);
	            print_int(BUFFER_ESTADO,9,79,0, attr);
	            break;
	        case 0x0b:  
	            attr = (C_FG_WHITE | C_BG_BROWN);
	            print_int(BUFFER_ESTADO,0,79,0, attr);
	            break;
	        case 0x32:  //M
	        		attr = (C_FG_WHITE | C_BG_BROWN);
	         		print(BUFFER_MAPA,m,79,0, attr);
	                scheduler.mostrarEstado = 0;
	                cargarBufferMapa();
	            return;
	        case 0x12://    E
	            	attr = (C_FG_WHITE | C_BG_BROWN);
	         		print(BUFFER_ESTADO,e,79,0, attr);
	             break;
	    }
	    cargarBufferEstado();
	    return;
	}

//CASO ESTOY EN MAPA
	if(scheduler.mostrarEstado == 0){
		    switch (scan_code)
		    {
		        case 0x02:
		            attr = (C_FG_WHITE | C_BG_LIGHT_GREY);
		            print_int(BUFFER_MAPA,1,79,0,attr);
		            break;

		        case 0x03:
		            attr = (C_FG_WHITE | C_BG_BROWN);
		            print_int(BUFFER_MAPA,2,79, 0, attr);
		            break;

		        case 0x04:
		            attr = (C_FG_WHITE | C_BG_MAGENTA);
		            print_int(BUFFER_MAPA,3,79,0, attr);

		            break;
		        case 0x05:  
		            attr = (C_FG_WHITE | C_BG_RED);
		            print_int(BUFFER_MAPA,4,79,0, attr);
		            break;


		        case 0x06:
		            attr = (C_FG_WHITE | C_BG_CYAN);
		            print_int(BUFFER_MAPA,5,79,0, attr);
		            break;

		        case 0x07:
		            attr = (C_FG_WHITE | C_BG_GREEN);
		            print_int(BUFFER_MAPA,6,79,0, attr);

		            break;

		        case 0x08:  
		            attr = (C_FG_WHITE | C_BG_BLUE);
		            print_int(BUFFER_MAPA,7,79,0, attr);
		            break;


		        case 0x09:
		            attr = (C_FG_WHITE | C_BG_LIGHT_GREY);
		            print_int(BUFFER_MAPA,8,79,0, attr);
		            break;

		        case 0x0a:  
		            attr = (C_FG_WHITE | C_BG_RED);
		            print_int(BUFFER_MAPA,9,79,0, attr);
		            break;
		        case 0x0b:  
		            attr = (C_FG_WHITE | C_BG_BROWN);
		            print_int(BUFFER_MAPA,0,79,0, attr);
		            break;
		        case 0x32:  //M
		        	attr = (C_FG_WHITE | C_BG_BROWN);
		         	print(BUFFER_MAPA,m,79,0, attr);
		            break;
		        case 0x12://    E
	         		attr = (C_FG_WHITE | C_BG_BROWN);
	         		print(BUFFER_ESTADO,e,79,0, attr);
	                scheduler.mostrarEstado = 1;
	                cargarBufferEstado();
	                return;
		    }
		    cargarBufferMapa();
		    return;
		}

}


void llamada (unsigned int eax,unsigned int ebx, unsigned int ecx)
{
	unsigned int cuantas;
	if (corriendoBandera == 1)//si una bandera llama a la syscall 0x50, se muere esa bandera y su tarea
	{
			corriendoBandera = 0;
			//scheduler.tareas[scheduler.tarea_actual].viva = 0; //mato tarea
			//scheduler.banderas[scheduler.tarea_actual].viva = 0;//mato bandera
			//scheduler.banderasVivas--;
			matar_bandera_porInt50();
			return;
	}
	else
	{
			//scheduler.paginas.idTarea = scheduler.tarea_actual +1;
			if ( eax == SYS_FONDEAR) //cambia ancla, actualiza pag, hay que llamar a buffer
			{
				unsigned int posnueva = ebx;
				//unsigned int posvieja1;
				//syscall_fondear(ebx);
				unsigned int directorio_tareas = rcr3(); //rcr3 creo que devuelve la dir fisica del cr3 actual
				mmu_mapear_pagina(TASK_ANCLA,directorio_tareas,posnueva,1,0);
				unsigned int posvieja1 = scheduler.paginasTareas[scheduler.tarea_actual].p3;
				scheduler.paginasTareas[scheduler.tarea_actual].p3 = posnueva;
				cuantas = cuantasMeApuntan(posnueva);
				if(cuantas == 1)//si no tenia ninguna apuntando, pongo tarea_actual
				{
					    /*x = damePosX(16,3,scheduler.paginasTareas[scheduler.tarea_actual].p1);
		    	 		y = damePosY(16,3,scheduler.paginasTareas[scheduler.tarea_actual].p1);
		           		print_int(BUFFER_MAPA,scheduler.tarea_actual+1,x,y,C_FG_WHITE | C_BG_RED);*/
		           		print_int(BUFFER_MAPA,scheduler.tarea_actual+1,((scheduler.paginasTareas[scheduler.tarea_actual].p3)/0x1000)%80,((scheduler.paginasTareas[scheduler.tarea_actual].p3)/0x1000)/80,C_FG_WHITE | C_BG_RED);
					    
				}
				if (cuantas == 2)// si tenia una apuntando, pongo la X
				{
						/*x = damePosX(16,3,scheduler.paginasTareas[scheduler.tarea_actual].p1);
		    	 		y = damePosY(16,3,scheduler.paginasTareas[scheduler.tarea_actual].p1);*/
		    	 		char * p = "x";
		           		print(BUFFER_MAPA,p,((scheduler.paginasTareas[scheduler.tarea_actual].p3)/0x1000)%80,((scheduler.paginasTareas[scheduler.tarea_actual].p3)/0x1000)/80,C_FG_WHITE | C_BG_BROWN);
				}
				//si tenia mas de una, ya tiene una X asi que no tengo que hacer nada
				
				//sacar de donde apuntaba
				//ya la saqué entonces si me queda 
				//si a esa pos apuntaban dos nada mas, poner el numero de la otra que apuntaba.
				//si a esa pos apuntaba solo la actual, poner en azul esa pos
				//si apuntaban mas de dos, no hacerle nada
				//cuantas = cuantas;
				unsigned int cuantas = cuantasMeApuntan(posvieja1);
				if (cuantas == 0)//si no le queda ninguna apuntando la pongo en azul
				{
						/*x = damePosX(16,3,posvieja1);
		    	 		y = damePosY(16,3,posvieja1);*/
		    	 		char * p = " ";
		           		print(BUFFER_MAPA,p,(posvieja1/0x1000)%80,(posvieja1/0x1000)/80,C_BG_GREEN);

				}
				if ( cuantas == 1)//si tenia una,pongo ese indice
				{
					int ind = dameIndTareaEnPos(posvieja1);
					/*x = damePosX(16,3,posvieja1);
			 		y = damePosY(16,3,posvieja1);*/
		           	print_int(BUFFER_MAPA,ind,(posvieja1/0x1000)%80,(posvieja1/0x1000)/80,C_FG_WHITE | C_BG_RED); 

				}
				//si le quedan dos o mas va a seguir con una X				    
				}
			
			
			if ( eax == SYS_NAVEGAR)// actualiza pag, hay que llamar a buffer
			{
				//syscall_navegar(ebx,ecx);
				unsigned int pagina1nueva = ebx;
				unsigned int pagina2nueva = ecx;
				unsigned int prueba = rcr3();
				prueba = prueba;
				//copiarCodigo(0x10000 + (0x1000*scheduler.tarea_actual*2), pagina1nueva); //copia la primera pagina de codigo a ebx
				//copiarCodigo(0x10000 + (0x1000*scheduler.tarea_actual*2) + 0x1000, pagina2nueva); //copia la segunda pagina de codigo a ecx
	 			copiarCodigo(scheduler.paginasTareas[scheduler.tarea_actual].p1, pagina1nueva); //copia la primera pagina de codigo a ebx
	 			copiarCodigo(scheduler.paginasTareas[scheduler.tarea_actual].p2, pagina2nueva); //copia la segunda pagina de codigo a ecx
	 			//Remapeo 
	 			//if (scheduler.tarea_actual != 7)
	 			//{
	    		mmu_mapear_pagina( DIR_VIRTUAL_TAREA, rcr3(), pagina1nueva,1,1);
	    		mmu_mapear_pagina( DIR_VIRTUAL_TAREA + 0x1000, rcr3(), pagina2nueva,1,1);
	    		//}
				//scheduler.paginas.p1=(unsigned int)ebx;
				//scheduler.paginas.p2=(unsigned int)ecx;
				//scheduler.paginas.p3=(unsigned int)ebx;
				//sacar de donde apuntaba
				//si a esa pos apuntaban dos nada mas, poner el numero de la otra que apuntaba.
				//si a esa pos apuntaba solo la actual, poner en azul esa pos
				//si apuntaban mas de dos, no hacerle nada
				unsigned int posvieja1 = scheduler.paginasTareas[scheduler.tarea_actual].p1;//me guardo las posiciones viejas
				unsigned int posvieja2 = scheduler.paginasTareas[scheduler.tarea_actual].p2;
				//actualizo las paginas
				scheduler.paginasTareas[scheduler.tarea_actual].p1 = pagina1nueva;
				scheduler.paginasTareas[scheduler.tarea_actual].p2 = pagina2nueva;


				
				int cuantas;
				//primero meto la nueva
				//PRIMERA PAGINA
				//int x;
				//int y;
				cuantas = cuantasMeApuntan(pagina1nueva);
				if(cuantas == 1)//si no tenia ninguna apuntando, pongo tarea_actual
				{
					    /*x = damePosX(16,3,scheduler.paginasTareas[scheduler.tarea_actual].p1);
		    	 		y = damePosY(16,3,scheduler.paginasTareas[scheduler.tarea_actual].p1);
		           		print_int(BUFFER_MAPA,scheduler.tarea_actual+1,x,y,C_FG_WHITE | C_BG_RED);*/
		           		print_int(BUFFER_MAPA,scheduler.tarea_actual+1,((scheduler.paginasTareas[scheduler.tarea_actual].p1)/0x1000)%80,((scheduler.paginasTareas[scheduler.tarea_actual].p1)/0x1000)/80,C_FG_WHITE | C_BG_RED);
					    
				}
				if (cuantas == 2)// si tenia una apuntando, pongo la X
				{
						/*x = damePosX(16,3,scheduler.paginasTareas[scheduler.tarea_actual].p1);
		    	 		y = damePosY(16,3,scheduler.paginasTareas[scheduler.tarea_actual].p1);*/
		    	 		char * p = "x";
		           		print(BUFFER_MAPA,p,((scheduler.paginasTareas[scheduler.tarea_actual].p1)/0x1000)%80,((scheduler.paginasTareas[scheduler.tarea_actual].p1)/0x1000)/80,C_FG_WHITE | C_BG_BROWN);
				}
				//si tenia mas de una, ya tiene una X asi que no tengo que hacer nada
				//sacar de donde apuntaba
				//ya la saqué entonces si me queda 
				//si a esa pos apuntaban dos nada mas, poner el numero de la otra que apuntaba.
				//si a esa pos apuntaba solo la actual, poner en azul esa pos
				//si apuntaban mas de dos, no hacerle nada
				cuantas = cuantasMeApuntan(posvieja1);
				if (cuantas == 0)//si no le queda ninguna apuntando la pongo en azul
				{
						/*x = damePosX(16,3,posvieja1);
		    	 		y = damePosY(16,3,posvieja1);*/
		    	 		char * p = " ";
		           		print(BUFFER_MAPA,p,(posvieja1/0x1000)%80,(posvieja1/0x1000)/80,C_BG_CYAN);

				}
				if ( cuantas == 1)//si tenia una,pongo ese indice
				{
					int ind = dameIndTareaEnPos(posvieja1);
					/*x = damePosX(16,3,posvieja1);
			 		y = damePosY(16,3,posvieja1);*/
		           	print_int(BUFFER_MAPA,ind+1,(posvieja1/0x1000)%80,(posvieja1/0x1000)/80,C_FG_WHITE | C_BG_RED); 

				}
				//si le quedan dos o mas va a seguir con una X
				//SEGUNDA PAGINA
				
							//int x;
				//int y;
				cuantas = cuantasMeApuntan(pagina2nueva);
				if(cuantas == 1)//si no tenia ninguna apuntando, pongo tarea_actual
				{
					    /*x = damePosX(16,3,scheduler.paginasTareas[scheduler.tarea_actual].p1);
		    	 		y = damePosY(16,3,scheduler.paginasTareas[scheduler.tarea_actual].p1);
		           		print_int(BUFFER_MAPA,scheduler.tarea_actual+1,x,y,C_FG_WHITE | C_BG_RED);*/
		           		print_int(BUFFER_MAPA,scheduler.tarea_actual+1,((scheduler.paginasTareas[scheduler.tarea_actual].p2)/0x1000)%80,((scheduler.paginasTareas[scheduler.tarea_actual].p2)/0x1000)/80,C_FG_WHITE | C_BG_RED);
					    
				}
				if (cuantas == 2)// si tenia una apuntando, pongo la X
				{
						/*x = damePosX(16,3,scheduler.paginasTareas[scheduler.tarea_actual].p1);
		    	 		y = damePosY(16,3,scheduler.paginasTareas[scheduler.tarea_actual].p1);*/
		    	 		char * p = "x";
		           		print(BUFFER_MAPA,p,((scheduler.paginasTareas[scheduler.tarea_actual].p2)/0x1000)%80,((scheduler.paginasTareas[scheduler.tarea_actual].p2)/0x1000)/80,C_FG_WHITE | C_BG_BROWN);
				}
				//si tenia mas de una, ya tiene una X asi que no tengo que hacer nada

				//sacar de donde apuntaba
				//ya la saqué entonces si me queda 
				//si a esa pos apuntaban dos nada mas, poner el numero de la otra que apuntaba.
				//si a esa pos apuntaba solo la actual, poner en azul esa pos
				//si apuntaban mas de dos, no hacerle nada
				cuantas = cuantasMeApuntan(posvieja2);
				if (cuantas == 0)//si no le queda ninguna apuntando la pongo en azul
				{
						/*x = damePosX(16,3,posvieja1);
		    	 		y = damePosY(16,3,posvieja1);*/
		    	 		char * p = " ";
		           		print(BUFFER_MAPA,p,(posvieja2/0x1000)%80,(posvieja2/0x1000)/80,C_BG_CYAN);

				}
				if ( cuantas == 1)//si tenia una,pongo ese indice
				{
					int ind = dameIndTareaEnPos(posvieja2);
					/*x = damePosX(16,3,posvieja1);
			 		y = damePosY(16,3,posvieja1);*/
		           	print_int(BUFFER_MAPA,ind+1,(posvieja2/0x1000)%80,(posvieja2/0x1000)/80,C_FG_WHITE | C_BG_RED); 

				}
				//si le quedan dos o mas va a seguir con una X
				actualizarBufferEstado_Paginas();

			}
			if ( eax == SYS_CANONEAR )
			{
				//syscall_canonear(ebx,ecx);//ver lo de que es relativa
				if ( ultimoMisil == 2)//si es el primer misil que tiran
				{
					char * p = " ";
					ultimoMisil = ebx;
					print(BUFFER_MAPA,p,(ultimoMisil/0x1000)%80,(ultimoMisil/0x1000)/80,C_BG_MAGENTA );

				}
				else
				{
					char * p= " ";
					print(BUFFER_MAPA,p,(ultimoMisil/0x1000)%80,(ultimoMisil/0x1000)/80,C_BG_CYAN );//pinto la direccion anterior como mar
					ultimoMisil = ebx;
					print(BUFFER_MAPA,p,(ultimoMisil/0x1000)%80,(ultimoMisil/0x1000)/80,C_BG_MAGENTA );

				}
				//unsigned int dir_absoluta = 0x40000000 + ecx;
				unsigned int dir_absoluta = ecx;
				unsigned int i;
			    for(i = 0; i<97; i+=1)
			    {
			        *((unsigned char*) (ebx + i)) =  *((unsigned char *) (dir_absoluta + i));
			    }
			}
	}
		actualizarBufferEstado_Paginas();
}

int cuantasMeApuntan(unsigned int direccion)
{
	int i;
	int res = 0;
	for (i=0;i<8;i++)
	{
		if(scheduler.paginasTareas[i].p1 == direccion)
		{
			res = res + 1;
		}
		if(scheduler.paginasTareas[i].p2 == direccion)
		{
			res = res + 1;
		}
		if(scheduler.paginasTareas[i].p3 == direccion)
		{
			res = res + 1;
		}
	}
	return res;
}
int dameIndTareaEnPos(unsigned int direccion)
{
	int i;
	int res = 0;
	for (i=0;i<8;i++)
	{
		if(scheduler.paginasTareas[i].p1 == direccion)
		{
			res = i;
		}
		if(scheduler.paginasTareas[i].p2 == direccion)
		{
			res = i;
		}
		if(scheduler.paginasTareas[i].p3 == direccion)
		{
			res = i;
		}
	}
	return res;
}
unsigned short atender_int66(unsigned int dir_bandera_buffer){
	unsigned int buff = dir_bandera_buffer;
	if(corriendoBandera==1){
		actualizarBufferEstado_Bandera_i(buff);
		int id_bandera = 0x1000 *(scheduler.bandera_actual);
		//REINICIO LAS BANDERAS
		
			unsigned int *dir_bandera= (unsigned int *)(0x10000 + id_bandera + 0x1FFC);
			tss_banderas[scheduler.bandera_actual].eip = 0x40000000 + *(dir_bandera);
	    
	    	tss_banderas[scheduler.bandera_actual].esp = 0x40001FFC; // 0x40001FFC
	    	tss_banderas[scheduler.bandera_actual].ebp = 0x40001FFC; // 0x40001FFC
	    	tss_banderas[scheduler.bandera_actual].esp0 = pilas_cero_bandera[scheduler.bandera_actual];
	    	tss_banderas[scheduler.bandera_actual].eflags = 0x202;
		//scheduler.bandera_actual = scheduler.bandera_actual+1;
		//if(scheduler.bandera_actual ==)
		//actualizarBufferEstado_Bandera_i(dir_bandera_buffer);
		//actualizarBufferEstado_Bandera_i(dir_bandera_buffer);
		//if(scheduler.mostrarEstado ==1)cargarBufferEstado();
		corriendoBandera = 0;
		//corriendoBandera = 0;
		return (GDT_TAREA_IDLE<<3);
	}	
	if ( corriendoTareas == 1){
		return matar_tarea_porInt66(); //agregar que debe matar la bandera corresp
	}
	return 0; //nunca llega acá
}



void sched_inicializar() {
	scheduler.tarea_actual = 7;
	scheduler.bandera_actual = 7;
	scheduler.banderasVivas=8;
	scheduler.mostrarEstado = 1;
	tareasRestantes = 3;
	corriendoTareas = 1;
	corriendoBandera = 0;
	muestroMapa =0;
	unsigned short i = 0x0;
	for(i=0; i< 8; i++){
		scheduler.tareas[i].tss_selector = ((GDT_TAREA_1 + 2*i) << 3) | 0; //consultar 
		scheduler.tareas[i].id = 0;
		scheduler.tareas[i].viva = 1;

		scheduler.banderas[i].tss_selector = ((GDT_TAREA_1_BANDERA + 2*i) << 3) | 0;
		scheduler.banderas[i].id = 0;
		scheduler.banderas[i].viva = 1;


	}
	//inicializo las paginas de las tareas
	for ( i=0;i<8;i++)
	{
		scheduler.paginasTareas[i].p1 = 0x100000 + (0x1000*i*0x2);
		scheduler.paginasTareas[i].p2 = 0x100000 + (0x1000*i*0x2) + 0x1000;
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

void reiniciarBanderas()
{
	int i;
		int id_tarea = 0x0000;
		//REINICIO LAS BANDERAS
		for (i=0;i<8;i++)
		{
			unsigned int *dir_bandera= (unsigned int *)(0x10000 + id_tarea + 0x1FFC);
			tss_banderas[i].eip = 0x40000000 + *(dir_bandera);
	    
	    	tss_banderas[i].esp = 0x40001FFC; // 0x40001FFC
	    	tss_banderas[i].ebp = 0x40001FFC; // 0x40001FFC
	    	tss_banderas[i].esp0 = pilas_cero_bandera[i];
	    	tss_banderas[i].eflags = 0x202;
	    	tss_banderas[i].es = (GDT_IDX_DATA_3 << 3) | 3;
	    	tss_banderas[i].cs = (GDT_IDX_CODE_3 << 3) | 3;//aca creo que va (GDT_IDX_CODE_3 << 3) | 3 ( RPL = 3 )
	    	tss_banderas[i].ss = (GDT_IDX_DATA_3 << 3) | 3;
	    	tss_banderas[i].ds = (GDT_IDX_DATA_3 << 3) | 3;
	    	tss_banderas[i].fs = (GDT_IDX_DATA_3 << 3) | 3;// no seria << en vez de |?
	    	tss_banderas[i].gs = (GDT_IDX_DATA_3 << 3) | 3;//chequear esto
	    	tss_banderas[i].	ptl     =0x0;
	        tss_banderas[i].	unused0	=0x0;
	        tss_banderas[i].	unused1	=0x0;
	        tss_banderas[i]. esp1	=0x0;
	        tss_banderas[i].	ss1		=0x0;
	        tss_banderas[i].	unused2	=0x0;
	        tss_banderas[i]. esp2	=0x0;
	        tss_banderas[i].	ss2		=0x0;
	        tss_banderas[i].	unused3	=0x0;
	        tss_banderas[i]. eax		=0x0;
	        tss_banderas[i]. ecx		=0x0;
	        tss_banderas[i]. edx		=0x0;
	        tss_banderas[i]. ebx		=0x0;
	        //tss_banderas[i]. ebp		=0x0;
	        tss_banderas[i]. esi		=0x0;
	        tss_banderas[i]. edi		=0x0;
	        tss_banderas[i].	unused4	=0x0;
	        tss_banderas[i].	unused5	=0x0;
	        tss_banderas[i].	unused6	=0x0;        
	        tss_banderas[i].	unused7	=0x0;        
	        tss_banderas[i]. unused8	=0x0;        
	        tss_banderas[i]. unused9	=0x0;
	        tss_banderas[i]. ldt		=0x0;
	        tss_banderas[i]. unused10=0x0;
	        tss_banderas[i]. dtrap	=0x0;
	    	id_tarea = id_tarea + 0x2000;
	 
		}
}

unsigned short sched_proxima_bandera() {
	int a = 2;
	a=a;
	reiniciarBanderas();
	//LAS REINICIO EN INT66
			/*int i;
		int id_tarea = 0x0000;
		//REINICIO LAS BANDERAS
		for (i=0;i<8;i++)
		{
			unsigned int *dir_bandera= (unsigned int *)(0x10000 + id_tarea + 0x1FFC);
			tss_banderas[i].eip = 0x40000000 + *(dir_bandera);
	    
	    	tss_banderas[i].esp = 0x40001FFC; // 0x40001FFC
	    	tss_banderas[i].ebp = 0x40001FFC; // 0x40001FFC
	    	tss_banderas[i].esp0 = pilas_cero_bandera[i];
	    	tss_banderas[i].eflags = 0x202;
	    	tss_banderas[i].es = (GDT_IDX_DATA_3 << 3) | 3;
	    	tss_banderas[i].cs = (GDT_IDX_CODE_3 << 3) | 3;//aca creo que va (GDT_IDX_CODE_3 << 3) | 3 ( RPL = 3 )
	    	tss_banderas[i].ss = (GDT_IDX_DATA_3 << 3) | 3;
	    	tss_banderas[i].ds = (GDT_IDX_DATA_3 << 3) | 3;
	    	tss_banderas[i].fs = (GDT_IDX_DATA_3 << 3) | 3;// no seria << en vez de |?
	    	tss_banderas[i].gs = (GDT_IDX_DATA_3 << 3) | 3;//chequear esto
	    	tss_banderas[i].	ptl     =0x0;
	        tss_banderas[i].	unused0	=0x0;
	        tss_banderas[i].	unused1	=0x0;
	        tss_banderas[i]. esp1	=0x0;
	        tss_banderas[i].	ss1		=0x0;
	        tss_banderas[i].	unused2	=0x0;
	        tss_banderas[i]. esp2	=0x0;
	        tss_banderas[i].	ss2		=0x0;
	        tss_banderas[i].	unused3	=0x0;
	        tss_banderas[i]. eax		=0x0;
	        tss_banderas[i]. ecx		=0x0;
	        tss_banderas[i]. edx		=0x0;
	        tss_banderas[i]. ebx		=0x0;
	        //tss_banderas[i]. ebp		=0x0;
	        tss_banderas[i]. esi		=0x0;
	        tss_banderas[i]. edi		=0x0;
	        tss_banderas[i].	unused4	=0x0;
	        tss_banderas[i].	unused5	=0x0;
	        tss_banderas[i].	unused6	=0x0;        
	        tss_banderas[i].	unused7	=0x0;        
	        tss_banderas[i]. unused8	=0x0;        
	        tss_banderas[i]. unused9	=0x0;
	        tss_banderas[i]. ldt		=0x0;
	        tss_banderas[i]. unused10=0x0;
	        tss_banderas[i]. dtrap	=0x0;
	    	id_tarea = id_tarea + 0x2000;
	 
		}*/


	if(scheduler.banderasPorCiclar==0){
		corriendoTareas = 1;
		//tareasRestantes = 3;
		tareasRestantes = 2;
		corriendoBandera = 0;
		//return (GDT_TAREA_IDLE<<3); vengo de la idle,ya habiendo corrido la ultima bandera, tengo que saltar a la proxima tarea
		return sched_proximo_indice();
	}   
	scheduler.bandera_actual = scheduler.bandera_actual + 1 ;
	if (scheduler.bandera_actual == 8 )//si estaba en la ultima viva, paso a la primera yreinicio banderas
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
	if(corriendoTareas==1){			//si vengo de la idle, estoy corriendo tareas
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
		return matar_bandera(); //estaba corriendo bandera y me corto el clock
	}
	//si estoy en 0,0; vengo de la idle y estoy ciclando banderas
	corriendoBandera = 1;
	return sched_proxima_bandera(); //no estoy ni corriendo banderas, ni tareas => vengo de idle y ciclo bander
}

unsigned short matar_tarea()
{
	//debo matar la bandera corresp tambien
	scheduler.tareas[scheduler.tarea_actual].viva = 0; //mato tarea
	scheduler.banderas[scheduler.tarea_actual].viva = 0;//mato bandera
	scheduler.banderasVivas--;
	//return atender_sched();
	//tenemos que saltar a la idle desde acá, 
	matarEnBuffer();
	return 0xc0; //selector de idle
}

unsigned short matar_bandera(){

	scheduler.tareas[scheduler.bandera_actual].viva = 0; //mato tarea
	scheduler.banderas[scheduler.bandera_actual].viva = 0;//mato bandera
	scheduler.banderasVivas--;
	corriendoBandera = 0;
	//return atender_sched();
	//tenemos que saltar a la idle desde acá, 
	matarBanderaEnBuffer();
	return 0xc0;
}

unsigned short matar_bandera_porInt50(){

	scheduler.tareas[scheduler.bandera_actual].viva = 0; //mato tarea
	scheduler.banderas[scheduler.bandera_actual].viva = 0;//mato bandera
	scheduler.banderasVivas--;
	corriendoBandera = 0;
	//return atender_sched();
	//tenemos que saltar a la idle desde acá, 
	matarBanderaEnBuffer_porInt50();
	return 0xc0;
}



void actualizarPantalla(){
	if(scheduler.mostrarEstado == 1){
		cargarBufferEstado();
	}
	if(scheduler.mostrarEstado == 0){
		cargarBufferMapa();
	}
}

unsigned short matar_tarea_porInt66()
{
	//debo matar la bandera corresp tambien
	scheduler.tareas[scheduler.tarea_actual].viva = 0; //mato tarea
	scheduler.banderas[scheduler.tarea_actual].viva = 0;//mato bandera
	scheduler.banderasVivas--;
	//return atender_sched();
	//tenemos que saltar a la idle desde acá, 
	matarEnBuffer_porInt66();
	return 0xc0; //selector de idle
}