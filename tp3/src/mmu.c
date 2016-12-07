/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "defines.h"


unsigned int proxima_pagina_libre;
void mmu_inicializar() {
	proxima_pagina_libre = 0x30000;
}

void mmu_inicializar_dir_kernel(){
	int* page_directory = (int*) 0x27000;  // PAGE_DIRECTORY_KERNEL = 0x27000
	page_directory[0] = 0x28000 + 0x3;		 // PAGE_TABLE_KERNEL_1 = 0x28000, seteo p=1 y r/w=1
	page_directory[1] = 0x29000 + 0x3;       // PAGE_TABLE_KERNEL_2 = 0x30000
 	
	int i;
	for (i = 2; i < 1024; ++i) { //pongo todo el resto de las posiciones en cero.
		page_directory[i]= 0x0;
	}

	int* page_table_1 = (int*) 0x28000;
	for (i = 0; i < 1024; ++i) {
		page_table_1[i] = ((i << 12) | 3);
	}

	int* page_table_2 = (int*) 0x29000;
	for (i = 0; i < 1024; ++i) {
		if(i<895){
			page_table_2[i] = (((i+1024) << 12) | 3);	
		}else{
			page_table_2[i] = 0x0;
		}
	}
}

