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

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
.loopear:
    ; To Infinity And Beyond!!
    ;mov eax, %1
    ;push eax
    mov edi, %1
    call atender_int
    mov eax, 0xFFF2
    mov ebx, 0xFFF2
    mov ecx, 0xFFF2
    mov edx, 0xFFF2
    jmp $
%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
reloj_numero:           dd 0x00000000
reloj:                  db '|/-\'


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

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

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
INT_0_len equ    $ - INT_0
INT_1: db 'Interrupcion reservada. (Error 1)', 0
INT_2: db 'Interrupcion no enmascarable. (Error 2)', 0
INT_3: db 'Interrupcion BreakPoint. (Error 3)', 0
INT_4: db 'Interrupcion de overflow. (Error 4)', 0
INT_5: db 'Interrupcion BOUND Range Exceeded. (Error 5)', 0
INT_6: db 'Interrupcion Invalid Opcode. (Error 6)' , 0
INT_7: db 'Interrupcion Coprocesador No Disponible. (Error 7)', 0
INT_8: db 'Interrupcion Doble Fault. (Error 8)', 0
INT_9: db 'Interrupcion Coprocessor Segment Overrun (reservada). (Error 9)', 0
INT_10: db 'Interrupcion TSS Invalido. (Error 10)', 0
INT_11: db 'Interrupcion Segmento no Presente. (Error 11)', 0
INT_12: db 'Interrupcion de Falta en el Stack Segment. (Error 12)', 0
INT_13: db 'Interrupcion de General Protection. (Error 13)', 0
INT_14: db 'Interrupcion de Page Fault. (Error 14)', 0
INT_15: db 'Interrupcion Reservada por Intel. (Error 15)', 0
INT_16: db 'Interrupcion X-87 FPU Error de punto flotante. (Error 16)', 0
INT_17: db 'Interrupcion de Alignment Check Fault. (Error 17)', 0
INT_18: db 'Interrupcion de Machine Check abort. (Error 18)', 0
INT_19: db 'Interrupcion SIMD Floating Point. (Error 19)', 0