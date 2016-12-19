/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "colors.h"
#include "screen.h"
#include "defines.h"
#include "sched.h"

#define BANDERA_BUFFER  0x40001000

/*str para guardarme info de registros*/

struct {
  unsigned int eax;
  
  char *error;
  unsigned int error_len;
  unsigned int tareaId;
  
  unsigned int ebx;
  unsigned int ecx;
  unsigned int edx;
  unsigned int esi;
  unsigned int edi;
  unsigned int ebp;
  unsigned int esp;
  unsigned int eip;
  unsigned int cr0;
  unsigned int cr2;
  unsigned int cr3;
  unsigned int cr4;

  unsigned int cs;
  unsigned int ds;
  unsigned int es;
  unsigned int fs;
  unsigned int gs;
  unsigned int ss;
  
  unsigned int eflags;
  
} __attribute__((__packed__)) debug_info;

unsigned char relojito[4] = {'\\','|','/','-'};
unsigned char ciclito = 0;
unsigned char banderasIniciadas[8] = {0,0,0,0,0,0,0,0};

ca primero;

void print(unsigned int dest, const char * text, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) dest;
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

void print_hex(unsigned int dest, unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) dest;
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

void print_int(unsigned int dest, unsigned int n, unsigned int x, unsigned int y, unsigned short attr){
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) dest;
    if( n > 9 ) {
      int a = n / 10;
      n -= 10 * a;
      print_int(dest,a,x-1,y,attr);
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
    const char* a = " // Pabellon de Aragon -1 // F. Sassone - G. Teren";
    print(BUFFER_ESTADO,a,0,0,(C_FG_WHITE | C_BG_BLUE)); 

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
//inicio buffers bandera
    ca (*estado)       [VIDEO_COLS] = (ca (*)[VIDEO_COLS]) BUFFER_ESTADO;
    ca (*bufferbandera)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) BANDERA_BUFFER;
    //inicializo vars
    //int x = 2+(id_Bandera* 12);
    x = 2;
    int j;
    int cont;
    for (i = 0; i < 8; ++i){
        
        if(i<4){
            y=2;
            x = 2 + (i*12);    
        }
        if(i>3){
            y=9;
            x = 2 + ((i-4)*12);    
        }
        print    (BUFFER_ESTADO,"NAVIO ",x,y,(C_FG_WHITE | C_BG_LIGHT_GREY));
        print_int(BUFFER_ESTADO,i+1, x+6, y, (C_FG_WHITE | C_BG_LIGHT_GREY));
        y++;
        cont = 0;
        for (j = 0; j < 50; ++j){
           estado[y][x].c = bufferbandera[y][x].c;
           estado[y][x].a = bufferbandera[y][x].a;
           x++;
           cont++;
           if(cont==10){
                x = 2 + (i*12);
                if(i>3)x = 2 + ((i-4)*12); 
                cont = 0;
                y++;
             }
        }
    }


//registros derecha
    x=51;
    y=2;
    const char *regs[20] ={"EAX","EBX","ECX","EDX","ESI","EDI","EBP","ESP","EIP",
    "CR0","CR2","CR3","CR4","CS","DS","ES","FS","GS","SS","EFLAGS"};
    for (i = 0; i < 19; ++i){
        print(BUFFER_ESTADO,regs[i],x,y,(C_BG_BLACK|C_FG_WHITE));
        y++;
        if(y==15){
            y=2;
            x=66;
        }
    }
    y=y+2;
    print(BUFFER_ESTADO,regs[19],x,y,(C_BG_BLACK|C_FG_WHITE));

//estados  

    y = 16;
    for (i = 0; i < 8; ++i){
        //numero
        print_int(BUFFER_ESTADO,i+1, 1, y, (C_FG_BLACK|C_BG_CYAN));
        //caract
        print(BUFFER_ESTADO, "P1:",3 ,y,(C_BG_CYAN | C_FG_BLACK ));    
        print(BUFFER_ESTADO, "P2:",15,y,(C_BG_CYAN | C_FG_BLACK ));    
        print(BUFFER_ESTADO, "P3:",29,y,(C_BG_CYAN | C_FG_BLACK ));

        print_hex(BUFFER_ESTADO,scheduler.paginasTareas[i].p1 ,8,6 ,y,(C_BG_BLACK|C_FG_WHITE));    
        print_hex(BUFFER_ESTADO,scheduler.paginasTareas[i].p2 ,8,18,y,(C_BG_BLACK|C_FG_WHITE));    
        print_hex(BUFFER_ESTADO,scheduler.paginasTareas[i].p3 ,8,32,y,(C_BG_BLACK|C_FG_WHITE)); 

        y++;
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
}

void iniciarBufferMapa(){
    int i;
    ca (*mapa)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) BUFFER_MAPA;

///////imprimoTierra
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
    
    //int MarInicioX = 16;
    //int MarInicioY = 3;

///////lleno el mar
    for(i=0; i<1664;i++){

        mapa[y][x].c =  ' '; //character
        mapa[y][x].a =  C_BG_CYAN; //atributo; fondo blue, letras white
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }

//////pongo pags 1 y 2 de tareas en mar
    int j;
    for (j=0;j<8;j++)
    {
    	 /*x = damePosX(MarInicioX,MarInicioY,scheduler.paginasTareas[j].p1);
    	 y = damePosY(MarInicioX,MarInicioY,scheduler.paginasTareas[j].p1);
         print_int(BUFFER_MAPA,j+1,x,y,C_FG_WHITE | C_BG_RED); 
     
         x = damePosX(MarInicioX,MarInicioY,scheduler.paginasTareas[j].p2);
    	 y = damePosY(MarInicioX,MarInicioY,scheduler.paginasTareas[j].p2);
     
         print_int(BUFFER_MAPA,j+1,x,y,C_FG_WHITE | C_BG_RED);*/
         print_int(BUFFER_MAPA,j+1,((scheduler.paginasTareas[j].p1)/0x1000)%80,((scheduler.paginasTareas[j].p1)/0x1000)/80,C_FG_WHITE | C_BG_RED);
         print_int(BUFFER_MAPA,j+1,((scheduler.paginasTareas[j].p2)/0x1000)%80,((scheduler.paginasTareas[j].p2)/0x1000)/80,C_FG_WHITE | C_BG_RED);      
         
    }

///////pongo anclas de tareas en tierra
        //estan todas en 0, no es nec
        char * p = "x";
        print(BUFFER_MAPA,p,0,0,C_FG_WHITE | C_BG_RED);     
}

int damePosX(int x,int y,unsigned int pagina )
{
		int i;
	    int pos = (pagina - 0x10000)/0x1000;
        for(i=0; i<pos;i++)
        {
        	x++;
	        if (x == VIDEO_COLS) {
	            x = 0;
	            y++;
	        }
    	}
    	return x;
    
}

int damePosY(int x,int y,unsigned int pagina )
{
		int i;
	    int pos = (pagina - 0x10000)/0x1000;
        for(i=0; i<pos;i++)
        {
        	x++;
	        if (x == VIDEO_COLS) {
	            x = 0;
	            y++;
	        }
    	}
    	return y;
    
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

void actualizarBufferEstado_Bandera_i(unsigned int dir_bandera_buffer){
    ////inicio vars
    int i = scheduler.bandera_actual; //un id 0..7
    ca (*estado)       [VIDEO_COLS] = (ca (*)[VIDEO_COLS]) BUFFER_ESTADO;
    ca (*bufferbandera)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) dir_bandera_buffer;
    int x;
    int y;
    int j;
    int cont;
    if(i<4){
        y=2;
        x = 2 + (i*12);    
    }
    if(i>3){
        y=9;
        x = 2 + ((i-4)*12);    
    }
    y++;
    cont = 0;
    ////caso primera corrida bandera
    if(banderasIniciadas[i] == 0){
        banderasIniciadas[i] =1;
        for (j = 0; j < 50; ++j){
           estado[y][x].c = bufferbandera[0][j].c;
           estado[y][x].a = bufferbandera[0][j].a;
           x++;
           cont++;
           if(cont==10){
                x = 2 + (i*12);
                if(i>3)x = 2 + ((i-4)*12); 
                cont = 0;
                y++;
             }
        }
        return; ////copie bandera del buffer y salgo
    }
    ////caso entro a una bandera y la actualizo
    primero.c = estado[y][x].c;
    primero.a = estado[y][x].a;
    for (j = 0; j < 50; ++j){
       estado[y][x].c = estado[y][x+1].c;
       estado[y][x].a = estado[y][x+1].a;
       x++;
       cont++;
       if(cont==10){
            if(i<4){
                estado[y][x-1].c = estado[y+1][2 + (i*12)].c;
                estado[y][x-1].a = estado[y+1][2 + (i*12)].a;
                x = 2 + (i*12);
            }
            if(i>3){
                estado[y][x-1].c = estado[y+1][2 + ((i-4)*12)].c;
                estado[y][x-1].a = estado[y+1][2 + ((i-4)*12)].a;
                x = 2 + ((i-4)*12);
            }
            cont = 0;
            y++;
         }
    }
    estado[y-1][x+9].c = primero.c;
    estado[y-1][x+9].a = primero.a;

} 


void actualizarBufferEstado_UltimoProblema(){
    unsigned int i;
    unsigned int x = 50;
    unsigned int y = 1;

    char *error = debug_info.error;
    //ca (*d)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) BUFFER_ESTADO;
    //error
    print(BUFFER_ESTADO, error,x,y,(C_BG_CYAN | C_FG_BLACK ));
    
    //print(unsigned int dest, const char * text, unsigned int x, unsigned int y, unsigned short attr)
    print(BUFFER_ESTADO, "NAVIO ", 71, 1,(C_BG_CYAN | C_FG_BLACK ) );
    //print_int(unsigned int dest, unsigned int n, unsigned int x, unsigned int y, unsigned short attr)


    //registros der
    x=55;
    y=2;
    unsigned int nums[20] ={debug_info.eax,debug_info.ebx, debug_info.ecx, debug_info.edx, 
        debug_info.esi, debug_info.edi, debug_info.ebp, debug_info.esp, debug_info.eip, 
        debug_info.cr0, debug_info.cr2, debug_info.cr3, debug_info.cr4, 
        debug_info.cs, debug_info.ds, debug_info.es, debug_info.fs, 
        debug_info.gs, debug_info.ss, debug_info.eflags} ;
    for (i = 0; i < 19; ++i){
        print_hex(BUFFER_ESTADO,nums[i],8,x,y,(C_BG_BLACK|C_FG_WHITE));
        y++;
        if(y==15){
            y=2;
            x=69;
        }
    }
    y=y+3;
    print_hex(BUFFER_ESTADO,nums[19],8,x,y,(C_BG_BLACK|C_FG_WHITE));
    unsigned char a = fueErrorBandera();
    if(a == 1){
        print_int(BUFFER_ESTADO, scheduler.bandera_actual+1,77,1,(C_BG_CYAN | C_FG_BLACK ));
        print(BUFFER_ESTADO, error,55,16+scheduler.bandera_actual,(C_FG_WHITE | C_BG_RED)); //falta escribir el error arriba
    }
    if(a!=1){
    print_int(BUFFER_ESTADO, scheduler.tarea_actual+1,77,1,(C_BG_CYAN | C_FG_BLACK ));
    }
}

void actualizarBufferEstado_Paginas(){ 

    //actualiza en buffer las direcciones de pag de la tarea act

    //int i;
    unsigned int y = 16 + scheduler.tarea_actual;
    //for (i = 0; i < 8; ++i){
        //pags
        print_hex(BUFFER_ESTADO,scheduler.paginasTareas[scheduler.tarea_actual].p1 ,8,6 ,y,(C_BG_BLACK|C_FG_WHITE));    
        print_hex(BUFFER_ESTADO,scheduler.paginasTareas[scheduler.tarea_actual].p2 ,8,18,y,(C_BG_BLACK|C_FG_WHITE));    
        print_hex(BUFFER_ESTADO,scheduler.paginasTareas[scheduler.tarea_actual].p3 ,8,32,y,(C_BG_BLACK|C_FG_WHITE));    

      //  y++;
    //}
    
}

void matarEnBuffer(){
    ca (*d)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) BUFFER_ESTADO;
    int i;
    int x=2;
    int y=16+scheduler.tarea_actual;
    //pone en rojo
    for (i = 2; i < 79; ++i)
    {
        d[y][x].a = (C_FG_WHITE | C_BG_RED);
        x++;
    }
    //imprime mensaje
    x=50;
    print(BUFFER_ESTADO,debug_info.error,55,y,(C_FG_WHITE | C_BG_RED));
    print(BUFFER_ESTADO,"Tarea  :",46,y,(C_FG_WHITE | C_BG_RED));


    //mata su bandera


    ////mata bandera (pone todo rojo)
    i = scheduler.tarea_actual;

    if(i<4){
        y=2;
        x = 2 + (i*12);    
    }
    if(i>3){
        y=9;
        x = 2 + ((i-4)*12);    
    }
    y++;
    int cont = 0;
    int j;
    for (j = 0; j < 50; ++j){
       d[y][x].c = 'X';
       d[y][x].a = (C_FG_WHITE | C_BG_RED);
       x++;
       cont++;
       if(cont==10){
            x = 2 + (i*12);
            if(i>3)x = 2 + ((i-4)*12); 
            cont = 0;
            y++;
         }
    }

}

void matarBanderaEnBuffer(){
    ////pone roja la tarea
    ca (*d)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) BUFFER_ESTADO;
    int i;
    int x=2;
    int y=16+scheduler.bandera_actual;
    //pone en rojo
    for (i = 2; i < 79; ++i)
    {
        d[y][x].a = (C_FG_WHITE | C_BG_RED);
        x++;
    }
    ////imprime mensaje de error en paginas 
    print(BUFFER_ESTADO,"Bandera: ",46,y,(C_FG_WHITE | C_BG_RED));    
    //print(BUFFER_ESTADO,debug_info.error,55,y,(C_FG_WHITE | C_BG_RED));   

    ////mata bandera (pone todo rojo)
    i = scheduler.bandera_actual;

    if(i<4){
        y=2;
        x = 2 + (i*12);    
    }
    if(i>3){
        y=9;
        x = 2 + ((i-4)*12);    
    }
    y++;
    int cont = 0;
    int j;
    for (j = 0; j < 50; ++j){
       d[y][x].c = 'X';
       d[y][x].a = (C_FG_WHITE | C_BG_RED);
       x++;
       cont++;
       if(cont==10){
            x = 2 + (i*12);
            if(i>3)x = 2 + ((i-4)*12); 
            cont = 0;
            y++;
         }
    }
    actualizarBufferEstado_UltimoProblema();


}



void matarBanderaEnBuffer_porInt50(){
    matarBanderaEnBuffer();
    actualizarBufferEstado_UltimoProblema();
    int y=16+scheduler.bandera_actual;
    print(BUFFER_ESTADO,"Syscall 50",55,y,(C_FG_WHITE | C_BG_RED)); 
    print(BUFFER_ESTADO,"Syscall 50",50,1,(C_BG_CYAN | C_FG_BLACK ));
    print_int(BUFFER_ESTADO, scheduler.bandera_actual+1,77,1,(C_BG_CYAN | C_FG_BLACK ));  

}

void actualizarRelojes(){
    //void print(unsigned int dest, const char * text, unsigned int x, unsigned int y, unsigned short attr
    //escribe abajo a la der el reloj de clock, abajo los de tareas y a la izq copia el de idle
    ca (*D_EST)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) BUFFER_ESTADO;
    ca (*D_MAP)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) BUFFER_MAPA;
    ca (*S_VID)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;

    D_EST[24][79].a = (C_BG_BLACK|C_FG_WHITE) ;
    D_EST[24][79].c = relojito[ciclito];
    D_EST[24][0].a  = S_VID[24][0].a;
    D_EST[24][0].c  = S_VID[24][0].c;

    D_MAP[24][79].a =(C_BG_BLACK|C_FG_WHITE) ;
    D_MAP[24][79].c = relojito[ciclito];
    D_MAP[24][0].a  = S_VID[24][0].a;
    D_MAP[24][0].c  = S_VID[24][0].c;

    int i;
    int x=4;
    char nums[8]={'1','2','3','4','5','6','7','8'};
    for (i = 0; i < 8; ++i)
    {
        if(scheduler.tareas[i].viva ==1 ){

            D_EST[24][x+(i*3)].a =(C_BG_LIGHT_GREY|C_FG_BLACK) ;
            D_EST[24][x+(i*3)].c = nums[i];
            D_MAP[24][x+(i*3)].a =(C_BG_LIGHT_GREY|C_FG_BLACK) ;
            D_MAP[24][x+(i*3)].c = nums[i];

            D_EST[24][(x+1)+(i*3)].a =(C_BG_LIGHT_GREY|C_FG_BLACK) ;
            D_EST[24][(x+1)+(i*3)].c = relojito[ciclito];
            D_MAP[24][(x+1)+(i*3)].a =(C_BG_LIGHT_GREY|C_FG_BLACK) ;
            D_MAP[24][(x+1)+(i*3)].c = relojito[ciclito];


        }
        if(scheduler.tareas[i].viva ==0 ){

            D_EST[24][x+(i*3)].a =(C_BG_RED|C_FG_WHITE) ;
            D_EST[24][x+(i*3)].c = nums[i];
            D_MAP[24][x+(i*3)].a =(C_BG_RED|C_FG_WHITE) ;
            D_MAP[24][x+(i*3)].c = nums[i];

            D_EST[24][(x+1)+(i*3)].a =(C_BG_RED|C_FG_WHITE) ;
            //D_EST[24][(x+1)+(i*3)].c = relojito[ciclito];
            D_MAP[24][(x+1)+(i*3)].a =(C_BG_RED|C_FG_WHITE) ;
            //D_MAP[24][(x+1)+(i*3)].c = relojito[ciclito];


        }


        if(scheduler.banderas[i].viva ==1 ){

            D_EST[24][(x+28)+(i*3)].a =(C_BG_MAGENTA|C_FG_BLACK) ;
            D_EST[24][(x+28)+(i*3)].c = nums[i];
            D_MAP[24][(x+28)+(i*3)].a =(C_BG_MAGENTA|C_FG_BLACK) ;
            D_MAP[24][(x+28)+(i*3)].c = nums[i];

            D_EST[24][(x+29)+(i*3)].a =(C_BG_MAGENTA|C_FG_BLACK) ;
            D_EST[24][(x+29)+(i*3)].c = relojito[ciclito];
            D_MAP[24][(x+29)+(i*3)].a =(C_BG_MAGENTA|C_FG_BLACK) ;
            D_MAP[24][(x+29)+(i*3)].c = relojito[ciclito];
           
        }   

        if(scheduler.banderas[i].viva ==0 ){

            D_EST[24][(x+28)+(i*3)].a =(C_BG_RED|C_FG_WHITE) ;
            D_EST[24][(x+28)+(i*3)].c = nums[i];
            D_MAP[24][(x+28)+(i*3)].a =(C_BG_RED|C_FG_WHITE) ;
            D_MAP[24][(x+28)+(i*3)].c = nums[i];

            D_EST[24][(x+29)+(i*3)].a =(C_BG_RED|C_FG_WHITE) ;
            D_EST[24][(x+29)+(i*3)].c = ' ';
            D_MAP[24][(x+29)+(i*3)].a =(C_BG_RED|C_FG_WHITE) ;
            D_MAP[24][(x+29)+(i*3)].c = ' ';
       
        }   


    }

    ciclito++;
    if(ciclito ==4)ciclito=0;

}

void matarEnBuffer_porInt66(){
    matarEnBuffer();
    actualizarBufferEstado_UltimoProblema();
    int y=16+scheduler.tarea_actual;
    print(BUFFER_ESTADO,"Syscall 66    ",55,y,(C_FG_WHITE | C_BG_RED));

    print(BUFFER_ESTADO,"Syscall 66    ",50,1,(C_BG_CYAN | C_FG_BLACK ));
    print_int(BUFFER_ESTADO, scheduler.tarea_actual+1,77,1,(C_BG_CYAN | C_FG_BLACK ));

}






