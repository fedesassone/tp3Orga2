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

    uint *tabla2_paginas = (uint*)(0x30000);
    for(i = 0; i<895; i++) // 77f-400 = 895(decimal)
    {

        tabla_paginas[i] = (i<<12) | 0x3; // por ej con i = 1 este es 0x001003
        tabla_paginas[i] = tabla_paginas[i] + 0x400000; // le sumo 0x400000 y queda 0x401003

    }

}

void mmu_inicializar() {
}

