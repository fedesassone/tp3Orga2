/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 25
#define VIDEO_COLS 80
#define VIDEO_SCREEN 0xB8000

#define BUFFER_MAPA				0x0002E000
#define BUFFER_ESTADO			0x0002D000

#include "colors.h"

/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    unsigned char c;
    unsigned char a;
} ca;

typedef struct pn_s {
  unsigned int idTarea;  
  unsigned int p1;
  unsigned int p2;
  unsigned int p3;
  
} __attribute__((__packed__)) paginasNavios;


void print(unsigned int dest, const char * text, unsigned int x, unsigned int y, unsigned short attr);

void print_hex(unsigned int dest, unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr);

void print_int(unsigned int dest, unsigned int n, unsigned int x, unsigned int y, unsigned short attr);

void iniciarBufferMapa(); 
void cargarBufferMapa();
void iniciarBufferEstado();
void cargarBufferEstado();

void actualizarBufferEstado_Bandera_i(char id_Bandera);
void actualizarBufferEstado_UltimoProblema();
void actualizarBufferEstado_Paginas();

#endif  /* !__SCREEN_H__ */
