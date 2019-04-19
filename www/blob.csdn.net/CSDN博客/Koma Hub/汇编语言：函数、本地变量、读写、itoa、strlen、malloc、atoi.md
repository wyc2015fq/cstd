# 汇编语言：函数、本地变量、读写、itoa、strlen、malloc、atoi - Koma Hub - CSDN博客
2019年03月21日 21:43:57[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：12
个人分类：[Assembly																[汇编](https://blog.csdn.net/Rong_Toa/article/category/8750815)](https://blog.csdn.net/Rong_Toa/article/category/8750816)
**Table of Contents**
[function.asm](#function.asm)
[build.sh](#build.sh)
[local-variables.asm](#local-variables.asm)
[read-input.asm](#read-input.asm)
[itoa.asm](#itoa.asm)
[strlen.asm](#strlen.asm)
[malloc.asm](#malloc.asm)
[strcat.asm](#strcat.asm)
[atoi.asm](#atoi.asm)
# function.asm
```
; Simon Whitehead, 2015
; ---------------------
;
; 2. Function calls
;
;    This program prints "Hello world!" to stdout
;    by calling two functions, then exits. One function
;    call uses the stack, the other uses registers
section .data
    msg1 db "Hello ", 0		; The first string to print,
    msg1.len equ $-msg1		; Length of the first string
    msg2 db "world!", 10, 0	; The second string to print
    msg2.len equ $-msg2		; Length of the second string
    sys_write equ 4		; long sys_write(unsigned int fd, const char __user *buf, size_t count);
    sys_exit equ 1
    stdout equ 1
section .text
global _start
_start:
    mov rdi,msg1
    mov rsi,msg1.len
    call write
    mov rdi,msg2
    mov rsi,msg2.len
    call write
    mov rax,sys_exit
    mov rbx,0
    int 0x80
; Args: (rdi: char*, rsi: int)
write:
    ; This is straight forward
    mov rax,sys_write
    mov rbx,stdout
    mov rcx,rdi
    mov rdx,rsi
    int 0x80
    ret
```
# build.sh
```bash
#!/bin/bash
nasm -f elf64 2.function-call.s
ld 2.function-call.o -o 2.function-call
```
# local-variables.asm
```
; Simon Whitehead, 2015
; ---------------------
;
; 3. Local variables
;
;    This program prints "Hello world!" to stdout
;    by pushing a global pointer onto the stack
;    of a called function. I am not sure if using
;    the stack pointer directly like this is okay..
;    - I assume its fine. I can't find documentation
;    that says it isn't.
section .data
    msg db "Hello world!", 10, 0	; Message to print
    msg.len equ $-msg
    sys_write equ 4
    sys_exit equ 1
    stdout equ 1
section .text
global _start
_start:
    mov rdi,msg		; Move msg into rdi
    mov rsi,msg.len	; Move the length of msg into rsi
    call print
    mov rax,sys_exit
    mov rbx,0
    int 0x80
print:
    push rbp
    mov rbp,rsp
    sub rsp,16		; Allocate space for two local variables (2*8, one pointer and one integer)
    mov [rbp-8],rdi	; Store our string pointer
    mov [rbp-16],rsi	; Store our integer
    mov rax,sys_write
    mov rbx,stdout
    mov rcx,[rbp-8]		; Local string pointer
    mov rdx,[rbp-16]		; Local string length integer
    int 0x80
    leave
    ret
```
# read-input.asm
```
; Simon Whitehead, 2015
; ---------------------
;
; 4. Read input
;
;    This program echos what the user types into
;    the console back to them.
section .data
    sys_exit equ 1
    sys_read equ 3
    sys_write equ 4
    stdin equ 0
    stdout equ 1
    BUFFER_SIZE equ 1024
section .bss
    buffer resb BUFFER_SIZE	; max of 1024 characters
section .text
global _start
_start:
    ; Call sys_read and store the result in buffer
    mov rax,sys_read
    mov rbx,stdin
    mov rcx,buffer
    mov rdx,BUFFER_SIZE
    int 0x80
    ; Call sys_write and print the buffer
    mov rax,sys_write
    mov rbx,stdout
    mov rcx,buffer
    mov rdx,BUFFER_SIZE
    int 0x80
    ; Exit
    mov rax,sys_exit
    mov rbx,0
    int 0x80
```
# itoa.asm
```
; Simon Whitehead, 2015
; ---------------------
;
; 5. itoa
;
;    This program converts numbers into ASCII characters and
;    prints them to stdout.
section .data
    newline db 10
    number dd 1234567890	; The number to print
    ; Syscall information
    sys_exit equ 1
    sys_write equ 4
    ; Streams
    stdout equ 1
    ; Constants
    BUFFER_SIZE equ 10
section .bss
    numbuf resb BUFFER_SIZE		; A buffer to store our string of numbers in
section .text
global _start
_start:
    mov rdi,[number]	; Move the number (123456789) into rax
    call itoa		; call the function
    ; Write the string returned in rax out to stdout
    mov rdi,rax		; The string pointer is returned in rax - move it to rdi for the function call
    mov rsi,rcx
    call print
    ; Write the newline character to stdout
    mov rdi,newline
    mov rsi,1
    call print
    
   ; Exit
    mov rax,sys_exit
    mov rbx,0
    int 0x80
; Args: (rdi: char*, rsi: int)
print:
    
    mov rax,sys_write
    mov rbx,stdout
    mov rcx,rdi
    mov rdx,rsi
    int 0x80
    ret
itoa:
    push rbp		
    mov rbp,rsp
    sub rsp,4		; allocate 4 bytes for our local string length counter
    mov rax,rdi		; Move the passed in argument to rax
    lea rdi,[numbuf+10]	; load the end address of the buffer (past the very end)
    mov rcx,10		; divisor
    mov [rbp-4],dword 0	; rbp-4 will contain 4 bytes representing the length of the string - start at zero
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
```
# strlen.asm
```
; Simon Whitehead, 2015
; ---------------------
;
; 6. strlen
;
;    This program prints "Hello world!" but it figures out the length
;    of the string all by itself.
section .data
    msg db "Hello world!", 10, 0	; The input string - including a newline character
    ; Syscall information
    sys_write equ 4
    sys_exit equ 1
    stdout equ 1
section .text
global _start
_start:
    mov rdi,msg		; Move the string into rdi
    call strlen
    ; print it to stdout
    mov rdx,rax		; strlen returns the length in rax
    mov rax,sys_write
    mov rbx,stdout
    mov rcx,msg
    int 0x80
    ; exit
    mov rax,sys_exit
    mov rbx,0
    int 0x80
strlen:
    xor rax,rax		; Set the value that scasb will search for. In this case it is zero (the null terminator byte)
    mov rcx,-1		; Store -1 in rcx so that scasb runs forever (or until it finds a null terminator). scasb DECREMENTS rcx each iteration
    cld			; Clear the direction flag so scasb iterates forward through the input string
    repne scasb		; Execute the scasb instruction. This leaves rdi pointing at the base of the null terminator.
    not rcx		; Invert the value of rcx so that we get the two's complement value of the count. E.g, a count of -25 results in 24.
    mov rax,rcx		; Move the length of the string into rax
    ret
```
# malloc.asm
```
; Simon Whitehead, 2015
; ---------------------
;
; 7. malloc
;
;    This program prints "Hello World!" but it
;    points to a string that is allocated in the
;    virtual memory of the process at runtime. It
;    does this by calling sys_brk to ask the OS
;    to allocate some more memory for the process
;    and return a pointer to the memory. It does not
;    handle any potential errors from sys_brk.
section .data
    sys_exit	 equ 1
    sys_write	 equ 4
    sys_brk	 equ 45
    
    stdout equ 1
section .text
global _start
_start:
    ; Allocate 16 bytes
    mov rdi,16
    call malloc
    call create_string	; helper function to create the string in the buffer located at rax
    ; Write the dynamically allocated string to stdout
    mov rcx,rax
    mov rax,sys_write
    mov rbx,stdout
    mov rdx,13		; Note, could call the strlen function defined in #6 here
    int 0x80
    ; Exit
    mov rax,sys_exit
    mov rbx,0
    int 0x80
malloc:
    ; Find the end of the data segment (passing zero returns the current location)
    mov rax,sys_brk
    xor rbx,rbx
    
    int 0x80
    ; Allocate the specified number of bytes passed in
    add rax,rdi
    
    mov rbx,rax
    mov rax,sys_brk
    int 0x80
    sub rax,rdi		; rax points to the HIGHEST address available. We need to subtract the original number of bytes so we point back at the start of the new region
    ret
create_string:
    mov [rax],byte 72	; H
    inc rax
    mov [rax],byte 101	; e
    inc rax
    mov [rax],byte 108	; l
    inc rax
    mov [rax],byte 108	; l
    inc rax
    mov [rax],byte 111	; o
    inc rax
    mov [rax],byte 32	; (space)
    inc rax
    mov [rax],byte 87	; W
    inc rax
    mov [rax],byte 111	; o
    inc rax
    mov [rax],byte 114	; r
    inc rax
    mov [rax],byte 108	; l
    inc rax
    mov [rax],byte 100	; d
    inc rax
    mov [rax],byte 33	; !
    inc rax
    mov [rax],byte 10	; NewLine
    inc rax
    mov [rax],byte 0	; Null terminator
    sub rax,13
    ret
```
# strcat.asm
```
; Simon Whitehead, 2015
; ---------------------
;
; 8. strcat
;
;    This program prints "Hello world!" by concatenating the
;    strings "Hello" and "World!" together. It does so by 
;    asking the operating system for some free memory to
;    place the result in.
section .data
    hello db "Hello ", 0	; First string, with newline and null terminator
    world db "World!", 0	; Second string, with newline and null terminator
    newline db 10		; Newline character
    sys_exit equ 1
    sys_write equ 4
    sys_brk equ 45
    stdout equ 1
section .text
global _start
_start:
    push rbp
    mov rbp,rsp
    sub rsp,4		; Local int32 variable
    mov rdi,16384
    call malloc		; Allocate 16kb
    mov [rbp-4],rax	; Move the base pointer of the malloc'd buffer to r9
    mov rdi,[rbp-4]	; Pass the base pointer in
    mov rsi,hello 	; Pass the string "Hello "
    mov rdx,world	; Pass the string "World!"
    call strcat		; Call strcat and have it combine the second and third arguments into the buffer passed in as the first argument
    mov rdi,[rbp-4]	; Move the pointer to the buffer into rdi
    call strlen		; have strlen computer the length of the string in the buffer
    mov rdx,rax		; Move the result (the length of the string) to rdx
    mov rax,sys_write
    mov rbx,stdout 
    mov rcx,[rbp-4]
    int 0x80
    ; Write the newline character
    mov rax,sys_write
    mov rbx,stdout
    mov rcx,newline
    mov rdx,1
    int 0x80
    mov rax,sys_exit
    mov rbx,0
    int 0x80
    leave
    ret
strcat:
    push rbp
    mov rbp,rsp
    sub rsp,8		; Room for a local counter
    push rdi		; Push rdi on to the stack for safe keeping
    mov rdi,rsi		; Move the second argument into rdi then call strlen
    call strlen
    mov rcx,rax		; Store the length in rcx
    mov [rbp-8],rax	; Also store the length in the local variable
    ; Restore the destination
    pop rdi
    
    cld			; Clear the direction flag
    repnz movsb		; Copy the string across
    push rdi		; Save rdi
    mov rdi,rdx		; Move the third argument into rdi and call strlen
    call strlen
    mov rcx,rax		; Store the length in rcx
    add [rbp-8],rax	; Add this length to the previous length to get the total characters
    ; Restore the destination and set the source as the third argument
    pop rdi
    mov rsi,rdx
    repnz movsb
    leave
    ret 
strlen:
    xor rax,rax		; Set the value that scasb will search for. In this case it is zero (the null terminator byte)
    mov rcx,-1		; Store -1 in rcx so that scasb runs forever (or until it finds a null terminator). scasb DECREMENTS rcx each iteration
    cld			; Clear the direction flag so scasb iterates forward through the input string
    repne scasb		; Execute the scasb instruction. This goes up to and includes the null terminator plus another decrement of rcx. The length is rcx-2.
    not rcx		; Invert the value of rcx so that we get the two's complement value of the count. E.g, a count of -25 results in 24.
    
    dec rcx
    mov rax,rcx
    ret
malloc:
    ; Find the end of the data segment (passing zero returns the current location)
    mov rax,sys_brk
    xor rbx,rbx
    
    int 0x80
    ; Allocate the specified number of bytes passed in
    add rax,rdi
    
    mov rbx,rax
    mov rax,sys_brk
    int 0x80
    sub rax,rdi	; rax points to the HIGHEST address available. We need to subtract the original number of bytes so we point back at the start of the new region
    ret
```
# atoi.asm
```
; Simon Whitehead, 2015
; ---------------------
;
; 9. atoi
;
;    This program prints "Equal" or "Not equal"
;    depending on whether the numerical value
;    contained in a string adds up to a predetermined
;    sum.
section .data
    equal db "Equal", 10, 0
    equal.len equ $-equal
    notequal db "Not equal", 10, 0
    notequal.len equ $-notequal
    sum equ 123
    base equ 100
    test_one db "23",0		; Should pass
    test_two db "32",0		; Should fail
    sys_write equ 4
    sys_exit equ 1
    
    stdout equ 1
section .text
global _start
_start:
    mov rdi,test_one	; Move our first test string into rdi
    call atoi
    add rax,base	; Add 100 to the result
    cmp rax,sum		; Should equal 123
    jne .notequal
    mov rax,sys_write
    mov rbx,stdout
    mov rcx,equal
    mov rdx,equal.len
    int 0x80
    mov rdi,test_two
    call atoi
    add rax,base	; Add 100 to the result
    cmp rax,sum		; Will equal 132 .. but not 123
    jne .notequal
    ; Add the "equal" code anyway .. just to make sure (but this second print should never be "Equal")
    mov rax,sys_write
    mov rbx,stdout
    mov rcx,equal
    mov rdx,equal.len
    int 0x80
    mov rax,sys_exit
    mov rbx,0
    int 0x80
    
.notequal:
    mov rax,sys_write
    mov rbx,stdout
    mov rcx,notequal
    mov rdx,notequal.len
    int 0x80
    mov rax,sys_exit
    mov rbx,0
    int 0x80
atoi:
    push rbp
    mov rbp,rsp
    sub rsp,16
    xor rcx,rcx		; Zero out our counter
    mov rbx,10		; Setup the multiplier
.multiplyLoop:
    mov dl,[rdi+rcx]	; Select the character
    sub dl,0x30		; Subtract ASCII 48 from the character so that it equals its actual value
    mov [rbp-8],rdx	; Preserve rdx
    mul rbx		; Multiply the current result by 10
    mov rdx,[rbp-8]	; Bring rdx back
    add al,dl		; Add the result to the value in al
    
    inc rcx		; Increase the counter
    cmp byte [rdi+rcx],0 ; Have we reached a null terminator?
    jne .multiplyLoop	 ; If not.. jump back and continue on
    ; The result will be in in rax
    leave
    ret
```
