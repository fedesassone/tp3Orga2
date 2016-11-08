/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/
#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"
#include "i386.h"
#include "tss.h"
#include "game.h"

typedef struct str_pde_entry {
    unsigned char   present:1;
    unsigned char   rw:1;
    unsigned char   us:1;
    unsigned char   pwt:1;
    unsigned char   pcd:1;
    unsigned char   a:1;
    unsigned char   d:1;
    unsigned char   pat:1;
    unsigned char   g:1;
    unsigned short  disponible:3;
    unsigned int    base:20;
} __attribute__((__packed__)) pde_entry;


typedef struct str_pte_entry {
    unsigned char   present:1;
    unsigned char   rw:1;
    unsigned char   us:1;
    unsigned char   pwt:1;
    unsigned char   pcd:1;
    unsigned char   a:1;
    unsigned char   d:1;
    unsigned char   pat:1;
    unsigned char   g:1;
    unsigned short  disponible:3;
    unsigned int    base:20;
} __attribute__((__packed__)) pte_entry;


void mmu_inicializar();
void mmu_inicializar_dir_kernel();
void mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica, unsigned char us);
unsigned int mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3);
unsigned int mmu_proxima_pagina_fisica_libre();
unsigned int mmu_inicializar_dir_tarea(unsigned int* codigo, posicion pos);
unsigned int game_dame_fisica_de_posicion(posicion pos);


#endif	/* !__MMU_H__ */