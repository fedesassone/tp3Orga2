; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"



global start

%macro limpiar_screen 0
   mov ecx, 0x7cf 
   mov ebx, 0x7d0 
   limpar:    
       mov word [fs:ecx+ ebx], 0x007F 
       dec ebx
       loop limpar
    mov ebx, 0x0f9e
    sub ebx, 0xa0 
    mov ecx, 0xa0
    linea2:
        mov word [fs:ebx+ecx], 0x000f ; pongo fondo negro
        dec ecx
        loop linea2

    ;mov ebx, 0xa0
    mov ecx, 0xa0
    lineas:
        dec ecx 
        mov word [fs:ecx ], 0x000f ; pongo fondo negro
        loop lineas

%endmacro
;; GDT
extern GDT_DESC

;; IDT
extern IDT_DESC
extern idt_inicializar

;; PIC
extern resetear_pic
extern habilitar_pic

;; SCREEN 
extern limpiar_screen
extern print

;; MMU
extern mmu_inicializar
extern mmu_inicializar_dir_kernel
;extern mmu_inicializar_table_kernel
%define BASE_PAGE_DIRECTORY 0x27000 

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg



;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; xchg bx, bx

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0


    ; habilitar A20
    call habilitar_A20
    ; cargar la GDT
    lgdt [GDT_DESC]
    
    ; setear el bit PE del registro CR0
  
    mov  eax, cr0       
    or eax, 1
    mov cr0, eax
 
    ; pasar a modo protegido
    jmp 0xa8:modoprotegido
    ;10101|000 =a8 
BITS 32    
   modoprotegido: 
    ;xchg bx, bx

   
    ; acomodar los segmentos
    xor eax, eax
    ; 10001000 
    mov ax, 0x98
    ;;10011|000 =98     
    mov ss, ax  

    mov ds, ax
    ;xchg bx,bx
    mov es, ax
    ;xchg bx,bx
  
    mov gs, ax
    ;xchg bx, bx
    ;index = 0000000001111=gdt15 video|0gdt|00rpl
    mov ax, 0xb8
    ;10111000
    mov fs, ax 
   ;video = 23 = 10111|000 = b8
    ; setear la pila
    mov esp, 0x27000

    ; pintar pantalla, todos los colores, que bonito!
  limpiar_screen
  
; Imprimir mensaje de bienvenida
    ;imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 0, 0
    


    ; inicializar el manejador de memoria
    call mmu_inicializar
    ; inicializar el directorio de paginas
    call mmu_inicializar_dir_kernel
    ;call mmu_inicializar_table_kernel
    ; inicializar memoria de tareas

    ;habilitar paginacion
    mov eax, BASE_PAGE_DIRECTORY
    mov cr3, eax
        xchg bx,bx

    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    
    ; inicializar tarea idle

    ; inicializar todas las tsss

    ; inicializar entradas de la gdt de las tsss

    ; inicializar el scheduler

    ; inicializar la IDT
        xchg bx,bx
    call idt_inicializar
    
    lidt[IDT_DESC]



    ; configurar controlador de interrupciones
    

    ; cargar la tarea inicial

    ; saltar a la primer tarea

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
