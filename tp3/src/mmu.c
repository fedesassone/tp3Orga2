/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "defines.h"

void mmu_inicializar_dir_kernel()
{
	uint* dir_paginas = (uint*)PAGE_DIR_ADDR;
	dir_paginas[0] = PAGE_TABLE_ADDR | 0x3;
	dir_paginas[1] = PAGE_TABLE2_ADDR | 0x3;
	uint i;
    for(i = 2; i<1024; i++)
    {
        directory[i] = 0x00000000;
    }
}

void mmu_inicializar_table_kernel()
{
    uint *tabla_paginas = (uint*)(0x28000);

    uint i;
    for(i = 0; i<1024; i++)
    {

        tabla_paginas[i] = (i<<12) | 0x3;
    }

    uint *tabla2_paginas = (uint*)(0x29000);
    for(i = 0; i<895; i++) // 77f-400 = 895(decimal)
    {

        tabla_paginas[i] = (i<<12) | 0x3; // por ej con i = 1 este es 0x001003
        tabla_paginas[i] = tabla_paginas[i] + 0x400000; // le sumo 0x400000 y queda 0x401003

    }

}

void mmu_inicializar() {
}

prox_pag_libre = 0x30000

void* dame_pagina_libre()
{
    prox_pag_libre += 0x1000;
    return prox_pag_libre;
}

void mmu_inicializar_dir_tarea(uint* dir_paginas, uint* tabla_paginas, uint* tabla2_paginas, uint* codigo_tarea)
{
    //HAGO IDENTITY MAPPING
    dir_paginas[0] = tabla_paginas | 0x3;
    dir_paginas[1] = tabla2_paginas | 0x3;
    uint i;
    for(i = 2; i<1024; i++)
    {
        directory[i] = 0x00000000;
    }

    uint j;
    for(j = 0; j<1024; j++)
    {

        tabla_paginas[j] = (j<<12) | 0x3;
    }

    uint *tabla2_paginas = (uint*)(0x29000);
    for(j = 0; j<895; j++) // 77f-400 = 895(decimal)
    {

        tabla_paginas[j] = (j<<12) | 0x3; // por ej con i = 1 este es 0x001003
        tabla_paginas[j] = tabla_paginas[j] + 0x400000; // le sumo 0x400000 y queda 0x401003

    }

    copiar_tarea(codigo_tarea,0x40000000)
    mmu_mapear_pagina(codigo_tarea, dir_paginas,0x40000000)

}


void mmu_mapear_pagina(uint virtual, uint cr3,uint fisica)
{
    //del cr3 agarro el directorio y lo guardo en pd
    
    uint pd_index = virtual >> 22;
    uint pt_index = virtual >> 12;//ver como borrar la parte de adelante
    pde = pd[pd_index]; //ver como hacer que sea una pde
    uint* pt = pde >> 12 // del pde me quedo con los de arriba que son la direccion de la tabla
    pte = pt[pt_index]; //ver como hacer que sea una pte
    //pte -> address = fisica;
    tlbflush();

}