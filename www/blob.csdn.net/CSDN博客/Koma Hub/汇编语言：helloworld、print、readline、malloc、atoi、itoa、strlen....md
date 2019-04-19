# 汇编语言：helloworld、print、readline、malloc、atoi、itoa、strlen... - Koma Hub - CSDN博客
2019年03月21日 21:32:41[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：13
个人分类：[Assembly																[汇编](https://blog.csdn.net/Rong_Toa/article/category/8750815)](https://blog.csdn.net/Rong_Toa/article/category/8750816)
unistd_64.h：[https://blog.csdn.net/Rong_Toa/article/details/88726424](https://blog.csdn.net/Rong_Toa/article/details/88726424)
**Table of Contents**
[hello.asm](#hello.asm)
[io.asm](#io.asm)
[malloc.asm](#malloc.asm)
[strings.asm](#strings.asm)
[include.asm](#include.asm)
# hello.asm
```
; Simon Whitehead, 2015
; ---------------------
;
; 1. Hello world
;
;    This program prints "Hello world!" to stdout
;    then exits
section .data
    msg db "Hello world!", 10, 0	; The string to print
    msg.len equ $-msg			; Length of the string
    sys_write equ 4			; long sys_write(unsigned int fd, const char __user *buf, size_t count);
    sys_exit equ 1
    stdout equ 1
section .text
global _start
_start:
    mov rax,sys_write	; syscall
    mov rbx,stdout	; fd
    mov rcx,msg		; buf
    mov rdx,msg.len    	; count
    int 0x80 ;中断0x80的处理函数system_call,it was used to handle system_call
    mov rax,sys_exit
    int 0x80 ;中断0x80的处理函数system_call,it was used to handle system_call
```
# io.asm
```
; Simon Whitehead, 2015
; ---------------------
;
; 10. includes
;
;     This application demonstrates separating code
;     into multiple files for clarity/organisation.
;     Functionally, this application will ask the user
;     for two numbers, add them, then print them to
;     stdout.
section .data
    stdin equ 0
    stdout equ 1
	
    sys_read equ 3
    sys_write equ 4
section .bss
    readline_buffer resb 1024	; A buffer for readline to store its result
section .text
global _print, _readline
_print:
    ; Call the sys_write syscall
    mov rax,sys_write
    mov rbx,stdout
    mov rcx,rdi
    mov rdx,rsi
    int 0x80
    ret
_readline:
    ; Read a line of input from stdin into a 1024 byte buffer
    mov rax,sys_read
    mov rbx,stdin
    mov rcx,readline_buffer
    mov rdx,1024
    int 0x80
    mov rax,readline_buffer
    ret
```
# malloc.asm
```
; Simon Whitehead, 2015
; ---------------------
;
; 10. includes
;
;     This application demonstrates separating code
;     into multiple files for clarity/organisation.
;     Functionally, this application will ask the user
;     for two numbers, add them, then print them to
;     stdout.
section .data
    sys_brk equ 45
section .text
global _malloc
_malloc:
    ; Find the end of the data segment (passing zero returns the current location)
    mov rax,sys_brk ;#define __NR_brk		 45
    xor rbx,rbx
    
    int 0x80
    ; Allocate the specified number of bytes passed in
    add rax,rdi
    
    mov rbx,rax
    mov rax,sys_brk ;#define __NR_brk		 45
    int 0x80
    sub rax,rdi	; rax points to the HIGHEST address available. 
                ; We need to subtract the original number of bytes 
                ; so we point back at the start of the new region
    ret
```
# strings.asm
```
; Simon Whitehead, 2015
; ---------------------
;
; 10. includes
;
;     This application demonstrates separating code
;     into multiple files for clarity/organisation.
;     Functionally, this application will ask the user
;     for two numbers, add them, then print them to
;     stdout.
section .data
section .bss
    itoa_buffer resb 10
section .text
global _atoi, _itoa, _strlen
_atoi:
    push rbp
    mov rbp,rsp
    sub rsp,16
    xor rcx,rcx		; Zero out our counter
    xor rax,rax
    mov rbx,10		; Setup the multiplier
.multiplyLoop:
    mov dl,[rdi+rcx]	; Select the character
    sub dl,0x30		; Subtract ASCII 48 from the character 
                    ; so that it equals its actual value
    mov [rbp-8],rdx	; Preserve rdx
    mul rbx		; Multiply the current result by 10
    mov rdx,[rbp-8]	; Bring rdx back
    add al,dl		; Add the result to the value in al
    
    inc rcx		; Increase the counter
    cmp byte [rdi+rcx],32 ; Have we reached a non-printable character?
    jge .multiplyLoop	 ; If not.. jump back and continue on
    ; The result will be in in rax
    leave
    ret
_itoa:
    ; allocate 4 bytes for our local string length counter
    push rbp
    mov rbp,rsp
    sub rsp,4
    lea rdi,[itoa_buffer+10]	; load the end address of the buffer (past the very end)
    mov byte [rdi], 0x00	; Store a null terminator
    mov rcx,10		; divisor
    mov [rbp-4],dword 0	; rbp-4 will contain 4 bytes representing the
                        ; length of the string - start at zero
.divloop:
    xor rdx,rdx		; Zero out rdx (where our remainder goes after idiv)
    idiv rcx		; divide rax (the number) by 10 (the remainder is placed in rdx)
    add rdx,0x30	; add 0x30 to the remainder so we get the correct ASCII value
    dec rdi		; move the pointer backwards in the buffer
    mov byte [rdi],dl	; move the character into the buffer
    inc dword [rbp-4]	; increase the length
    
    cmp rax,0		; was the result zero?
    jnz .divloop	; no it wasn't, keep looping
    mov rax,rdi		; rdi now points to the beginning of the string - move it into rax
    mov rcx,[rbp-4]	; rbp-4 contains the length - move it into rcx
    leave		; clean up our stack
    ret
_strlen:
    enter 0,0		; Initialize the stack frame nicely for us
    xor rax,rax		; Set the value that scasb will search for. 
                    ; In this case it is zero (the null terminator byte)
    mov rcx,-1		; Store -1 in rcx so that scasb runs forever 
                    ; (or until it finds a null terminator). scasb DECREMENTS rcx each iteration
    cld			; Clear the direction flag so scasb iterates 
                ; forward through the input string
    repne scasb		; Execute the scasb instruction. 
                    ; This goes up to and includes the null terminator 
                    ; plus another decrement of rcx. The length is rcx-2.
    not rcx		; Invert the value of rcx so that we get the two's 
                ; complement value of the count. E.g, a count of -25 results in 24.
    lea rax,[rcx-1]	; The above inversion includes the null terminator, 
                    ;so return the count-1 to strip it
    leave
    ret
_strcat:
    enter 8,0
    push rdi		; Push rdi on to the stack for safe keeping
    mov rdi,rsi		; Move the second argument into rdi then call strlen
    call _strlen
    mov rcx,rax		; Store the length in rcx
    mov [rbp-8],rax	; Also store the length in the local variable
    ; Restore the destination
    pop rdi
    
    cld			; Clear the direction flag
    repnz movsb		; Copy the string across
    push rdi		; Save rdi
    mov rdi,rdx		; Move the third argument into rdi and call strlen
    call _strlen
    mov rcx,rax		; Store the length in rcx
    add [rbp-8],rax	; Add this length to the previous length to get the total characters
    ; Restore the destination and set the source as the third argument
    pop rdi
    mov rsi,rdx
    repnz movsb
    leave
    ret
```
# include.asm
```
; Simon Whitehead, 2015
; ---------------------
;
; 10. includes
;
;     This application demonstrates separating code
;     into multiple files for clarity/organisation.
;     Functionally, this application will ask the user
;     for two numbers, add them, then print them to
;     stdout.
%include "io.asm"
%include "strings.asm"
%include "malloc.asm"
section .data
    sys_exit equ 1
    ; Set up some constants we will use throughout the application
    welcome_msg db "This program will add two numbers together.", 10
    welcome_msg.len equ $-welcome_msg
    newline_spacer db 10
    ; Prompts for the user
    prompt_one db "Enter your first number: ", 0
    prompt_one.len equ $-prompt_one
    prompt_two db "Enter your second number: ", 0
    prompt_two.len equ $-prompt_two
    ; The response
    answer db "The sum of the numbers is: ", 0
section .text
global _start
_start:
    ; Allocate 32 bytes of stack space
    push rbp
    mov rbp,rsp
    sub rsp,32
    call _welcome		; Call the welcome function
    mov rdi,prompt_one		; Push our prompts onto registers and call the prompt function
    mov rsi,prompt_one.len
    call _prompt
    mov [rbp-8],rax		; Store what the user entered in our local variable
    mov rdi,prompt_two		; Prompt for the second number
    mov rsi,prompt_two.len
    call _prompt
    add rax,[rbp-8]		; Add the first result to the result of the second prompt
    mov rdi,rax			; Call itoa to convert the number to a string
    call _itoa
    mov [rbp-16],rax		; Store this for later
    ; allocate 16kb of memory
    mov rdi,16384
    call _malloc
    mov [rbp-24],rax	; Store its base
    ; Concatenate the answer prompt with the sum of the entered numbers
    mov rdi,[rbp-24]
    mov rsi,answer
    mov rdx,[rbp-16]
    call _strcat
    ; Calculate the total length of the resulting string
    mov rdi,[rbp-24]
    call _strlen
    ; Print the result to the screen
    mov rdi,[rbp-24]
    mov rsi,rax
    call _print
    ; Add two spaces between the answer and the end
    mov rdi,newline_spacer
    mov rsi,1
    call _print
    call _print
    ; Exit
    call _exit
    leave		; Restore the stack
    ret
_welcome:
    ; Print the welcome message to the screen
    mov rdi,welcome_msg
    mov rsi,welcome_msg.len
    call _print
    ; Add a space underneath
    mov rdi,newline_spacer
    mov rsi,1
    call _print
    ret
_prompt:
    call _print		; Print the prompt that was passed in
    call _readline	; Read the user's input
    
    ; Call atoi to convert the users input to a number and return the number in rax
    mov rdi,rax
    call _atoi
    ret
_exit:
    ; execute the exit interrupt
    mov rax,sys_exit
    mov rbx,0
    int 0x80
    ret
```
