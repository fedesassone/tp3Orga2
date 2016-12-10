; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32


;; PIC
extern fin_intr_pic1
extern atender_int
extern atender_sched
;; SYSCALL ; estan en sched.c (fue la forma que encontre que me las acepte)
extern llamada
extern llamoTarea
extern matar_tarea
;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1
global _isr32
global _isr33
global _isr80
global _isr102

_isr%1:
.loopear:
    ; To Infinity And Beyond!!
    ;mov eax, %1
    ;push eax
    pushfd
    pushad
    imprimir_texto_mp INT_%1, INT_len_%1, 0x07, 1, 0
    call matar_tarea ; mata a la tarea que provoco la interrupcion y a su bandera o viceversa y devuelve en ax el selector
    mov [tss_selector], ax
    jmp far [tss_offset] ;paso a la proxima tarea o bandera segun corresponda
    popad
    popfd
%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
reloj_numero:           dd 0x00000000
reloj:                  db '|/-\'

; Tareas
tss_selector: dw 0x0
tss_offset:   dd 0x0 


;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14

ISR 16
ISR 17
ISR 18
ISR 19

;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
_isr32:
  pushad
  pushfd

  call fin_intr_pic1
  ;muestra reloj
  call proximo_reloj
  ;schedulizar
  ;call atender_sched ;esto me devuelve un selector tss
  ;mov [tss_selector], ax
  ;jmp far [tss_offset]
  ; listo?
  popfd
  popad

  iret
;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
_isr33:
  pushad
  pushfd

  call fin_intr_pic1


  ;ACA HACER LA INT DEL TECLADO
  
  popfd
  popad
  
  iret
;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
_isr80:
        push ecx
        push edx
        push ebx
        push esp
        push ebp
        push esi
        push edi
        pushfd
        cmp eax,0x83a
        jne cont
        xor esi,esi
        mov esi,97
  ciclo:mov dl,[ecx];MUEVO EL BYTE A COPIAR EN DL // VER LO DE QUE ES RELATIVA
        mov [ebx],dl; muevo el byte a la pagina destino
        inc ecx
        inc ebx
        dec esi
        cmp esi,0
        jne ciclo
   cont: push ecx
         push ebx
         push eax

        call llamada
        add esp,12
        
        jmp  0xc0:0x0 ;saltamos a la tarea idle

        popfd
        pop edi
        pop esi
        pop ebp
        pop esp
        pop ebx
        pop edx
        pop ecx
        
        iret

  _isr102:
  push ecx
  push edx
  push ebx
  push esp
  push ebp
  push esi
  push edi
  pushfd
  
  call llamoTarea ; me fijo si la llamó una tarea. Si es así, borro la tarea y a su bandera.Ademas pongo corriendoBandera en 0
 
  jmp  0xc0:0x0 ;saltamos a la tarea idle

  popfd
  pop edi
  pop esi
  pop ebp
  pop esp
  pop ebx
  pop edx
  pop ecx
  
  iret
;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
proximo_reloj:
    pushad

    inc DWORD [reloj_numero]
    mov ebx, [reloj]
    cmp ebx, 0x4
    jl .ok
        mov DWORD [reloj_numero], 0x0
        mov ebx, 0
    .ok:
        add ebx, reloj
        imprimir_texto_mp ebx, 1, 0x0f, 24, 79

    popad
    ret

INT_0: db 'Interrupcion de division por 0. (Error 0)', 0
INT_len_0 equ    $ - INT_0
INT_1: db 'Interrupcion reservada. (Error 1)', 0
INT_len_1 equ    $ - INT_1
INT_2: db 'Interrupcion no enmascarable. (Error 2)', 0
INT_len_2 equ    $ - INT_2
INT_3: db 'Interrupcion BreakPoint. (Error 3)', 0
INT_len_3 equ    $ - INT_3
INT_4: db 'Interrupcion de overflow. (Error 4)', 0
INT_len_4 equ    $ - INT_4
INT_5: db 'Interrupcion BOUND Range Exceeded. (Error 5)', 0
INT_len_5 equ    $ - INT_5
INT_6: db 'Interrupcion Invalid Opcode. (Error 6)' , 0
INT_len_6 equ    $ - INT_6
INT_7: db 'Interrupcion Coprocesador No Disponible. (Error 7)', 0
INT_len_7 equ    $ - INT_7
INT_8: db 'Interrupcion Doble Fault. (Error 8)', 0
INT_len_8 equ    $ - INT_8
INT_9: db 'Interrupcion Coprocessor Segment Overrun (reservada). (Error 9)', 0
INT_len_9 equ    $ - INT_9
INT_10: db 'Interrupcion TSS Invalido. (Error 10)', 0
INT_len_10 equ    $ - INT_10
INT_11: db 'Interrupcion Segmento no Presente. (Error 11)', 0
INT_len_11 equ    $ - INT_11
INT_12: db 'Interrupcion de Falta en el Stack Segment. (Error 12)', 0
INT_len_12 equ    $ - INT_12
INT_13: db 'Interrupcion de General Protection. (Error 13)', 0
INT_len_13 equ    $ - INT_13
INT_14: db 'Interrupcion de Page Fault. (Error 14)', 0
INT_len_14 equ    $ - INT_14
INT_15: db 'Interrupcion Reservada por Intel. (Error 15)', 0
INT_len_15 equ    $ - INT_15
INT_16: db 'Interrupcion X-87 FPU Error de punto flotante. (Error 16)', 0
INT_len_16 equ    $ - INT_16
INT_17: db 'Interrupcion de Alignment Check Fault. (Error 17)', 0
INT_len_17 equ    $ - INT_17
INT_18: db 'Interrupcion de Machine Check abort. (Error 18)', 0
INT_len_18 equ    $ - INT_18
INT_19: db 'Interrupcion SIMD Floating Point. (Error 19)', 0
INT_len_19 equ    $ - INT_19