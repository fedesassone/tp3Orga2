/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"
#include "colors.h"
#include "defines.h"

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    int i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (unsigned char) text[i];
        p[y][x].a = (unsigned char) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}//imprime puntero a char

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}// imprime en hexa

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    if( n > 9 ) {
      int a = n / 10;
      n -= 10 * a;
      print_int(a,x-1,y,attr);
    }
    p[y][x].c = '0'+n;
    p[y][x].a = attr;
}//imprime un short
void iniciarBufferEstado(){
    //linea fondo negro y letras blancas de 80pix
    unsigned int i;
    unsigned int x = 0;
    unsigned int y = 0;
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) BUFFER_ESTADO;
//primer linea 
    for(i=0; i<80; i++){
            p[y][x].c = ' ';
            p[y][x].a = (C_FG_WHITE | C_BG_BLACK);
            x++;
            if(x == VIDEO_COLS){
                x=0;
                y++;
            }
    }
//fondo gris
    x=0;
    y=1;
    for(i=0; i<1200 ; i++){
        p[y][x].c = ' ';
        p[y][x].a = (C_FG_BLACK | C_BG_LIGHT_GREY);
        x++;
        if(x == VIDEO_COLS){
            x=0;
            y++;
        }
    }
//linea celeste der
    x=50;
    y=1;
    for(i=0; i<28 ; i++){
        p[y][x].c = ' ';
        p[y][x].a = (C_FG_BLACK | C_BG_CYAN);
        x++;
        if(x == 78){
            x=50;
            y++;
        }
    }
//panel derecha, 
    x=50;
    y=2;
    for(i=0; i<364 ; i++){
        p[y][x].c = ' ';
        p[y][x].a = (C_FG_WHITE | C_BG_BLACK);
        x++;
        if(x == 78){
            x=50;
            y++;
        }
    }
//estados  
    x=1;
    y=16;
    for(i=0; i<624 ; i++){
        p[y][x].c = ' ';
        p[y][x].a = (C_FG_BLACK | C_BG_CYAN);
        x++;
        if(x == 79){
            x=1;
            y++;
        }
    }
}

void cargarBufferEstado(){
    unsigned int i;
    ca (*s)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) BUFFER_ESTADO;
    ca (*d)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    unsigned int x = 0;
    unsigned int y = 0;
    for(i=0; i<2000;i++){
        d[y][x].c = s[y][x].c;
        d[y][x].a = s[y][x].a;
        x++;
        if(x == VIDEO_COLS){
            x=0;
            y++;
        }
    }
    const char* a = " // Pabellon de Aragon -1 // F. Sassone - G. Teren";
    print(a,0,0,(C_FG_WHITE | C_BG_BLUE));    
}

void iniciarBufferMapa(){
    int i;
    ca (*mapa)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) BUFFER_MAPA;
    //imprimoTierra
    int x = 0;
    int y = 0;
    for(i=0; i<256; i++){
        mapa[y][x].c = ' '; //character
        mapa[y][x].a =  (C_BG_GREEN); //atributo; fondo green, letras white
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
    //agua
    for(i=0; i<1664;i++){
        mapa[y][x].c =  ' '; //character
        mapa[y][x].a =  C_BG_CYAN; //atributo; fondo blue, letras white
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
    x = 1;
    y = 24;
    //panel inferior
    for(i=0;i<78;i++){
        mapa[y][x].c = ' '; //character
        mapa[y][x].a =  (C_BG_BLACK); //atributo; fondo black, letras white
        x++;
    }
}

void cargarBufferMapa(){
    int i;
    int x=0;
    int y=0;
    ca(*d)[VIDEO_COLS] = (ca(*)[VIDEO_COLS]) VIDEO_SCREEN;
    ca(*s)[VIDEO_COLS] = (ca(*)[VIDEO_COLS]) BUFFER_MAPA;
    for(i=0;i<2000;i++){
        d[y][x].c = s[y][x].c;
        d[y][x].a = s[y][x].a;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}






