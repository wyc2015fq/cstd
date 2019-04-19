# Linux X64下汇编学习：C语言调用汇编代码，汇编中调用C语言函数 - Koma Hub - CSDN博客
2019年03月12日 21:24:57[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：158
**Table of Contents**
[hello world](#hello%C2%A0world)
[hello.asm](#hello.asm)
[makefile](#makefile)
[float](#float)
[circle_fpu_87c.c](#circle_fpu_87c.c)
[circle_fpu_87.asm](#circle_fpu_87.asm)
[makefile](#makefile)
[stack](#stack)
[stack.asm](#stack.asm)
[makefile](#makefile)
[string](#string)
[reverse.asm](#reverse.asm)
[makefile](#makefile)
[sum](#sum)
[sum.asm](#sum.asm)
[makefile](#makefile)
[C语言与汇编之间调用](#C%E8%AF%AD%E8%A8%80%E4%B8%8E%E6%B1%87%E7%BC%96%E4%B9%8B%E9%97%B4%E8%B0%83%E7%94%A8)
[casm1 - call C function from asm](#casm1%20-%20call%20C%20function%20from%20asm)
[casm.c](#casm.c)
[casm.asm](#casm.asm)
[makefile](#makefile)
[casm2 - gcc inline assembly](#casm2%20-%20gcc%20inline%20assembly)
[casm.c](#casm.c)
[makefile](#makefile)
[casm3 - call asm function from C](#casm3%20-%20call%20asm%20function%20from%20C)
[casm.c](#casm.c)
[casm.asm](#casm.asm)
[makefile](#makefile)
# hello world
## hello.asm
```
section .data
	msg	db "hello, world!",`\n`
section .text
	global	_start
_start:
	;; write syscall
	mov	rax, 1
	;; file descriptor, standard output
	mov	rdi, 1
	;; message address
	mov	rsi, msg
	;; length of message
	mov	rdx, 14
	;; call write syscall
	syscall
	;; exit
	mov	rax, 60
	mov	rdi, 0
	syscall
```
## makefile
```
all:
	nasm -f elf64 -o hello.o hello.asm
	ld -o hello hello.o
clean:
	rm hello hello.o
```
# float
## circle_fpu_87c.c
```cpp
#include <stdio.h>
extern int printResult(double result);
int printResult(double result) {
	printf("Circle radius is - %f\n", result);
	return 0;
}
```
## circle_fpu_87.asm
```
extern printResult
section .data
	radius		dq  1.7
	result		dq  0
	SYS_EXIT	equ 60
	EXIT_CODE	equ 0
section .text
	global	_start
_start:
	fld	qword [radius]
	fld	qword [radius]
	fmul
	fldpi
	fmul
	fstp	qword [result]
	mov	rax, 0
	movq	xmm0, [result]
	call	printResult
	mov	rax, SYS_EXIT
	mov	rdi, EXIT_CODE
	syscall
```
## makefile
```
build:
	gcc  -g -c circle_fpu_87c.c -o c.o
	nasm -f elf64 circle_fpu_87.asm -o circle_fpu_87.o
	ld   -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc circle_fpu_87.o  c.o -o testFloat1
clean:
	rm -rf *.o
	rm -rf testFloat1
```
# stack
## stack.asm
```
section .data
	SYS_WRITE	equ 1
	STD_IN		equ 1
	SYS_EXIT	equ 60
	EXIT_CODE	equ 0
	NEW_LINE	db 0xa
	WRONG_ARGC	db "Must be two command line argument", 0xa
section .text
	global	_start
_start:
	;; rcx - argc
	pop	rcx
	;;
	;; Check argc
	;;
	cmp	rcx, 3
	jne	argcError
	;;
	;; start to sum arguments
	;;
	;; skip argv[0] - program name
	add	rsp, 8
	;; get argv[1]
	pop	rsi
	;; convert argv[1] str to int
	call	str_to_int
	;; put first num to r10
	mov	r10, rax
	;; get argv[2]
	pop	rsi
	;; convert argv[2] str to int
	call	str_to_int
	;; put second num to r10
	mov	r11, rax
	;; sum it
	add	r10, r11
	;;
	;; Convert to string
	;;
	mov	rax, r10
	;; number counter
	xor	r12, r12
	;; convert to string
	jmp	int_to_str
;;
;; Print argc error
;;
argcError:
	;; sys_write syscall
	mov	rax, 1
	;; file descritor, standard output
	mov	rdi, 1
	;; message address
	mov	rsi, WRONG_ARGC
	;; length of message
	mov	rdx, 34
	;; call write syscall
	syscall
	;; exit from program
	jmp	exit
;;
;; Convert int to string
;;
int_to_str:
	;; reminder from division
	mov	rdx, 0
	;; base
	mov	rbx, 10
	;; rax = rax / 10
	div	rbx
	;; add \0
	add	rdx, 48
	add	rdx, 0x0
	;; push reminder to stack
	push	rdx
	;; go next
	inc	r12
	;; check factor with 0
	cmp	rax, 0x0
	;; loop again
	jne	int_to_str
	;; print result
	jmp	print
;;
;; Convert string to int
;;
str_to_int:
	;; accumulator
	xor	rax, rax
	;; base for multiplication
	mov	rcx,  10
next:
	;; check that it is end of string
	cmp	[rsi], byte 0
	;; return int
	je	return_str
	;; mov current char to bl
	mov	bl, [rsi]
	;; get number
	sub	bl, 48
	;; rax = rax * 10
	mul	rcx
	;; ax = ax + digit
	add	rax, rbx
	;; get next number
	inc	rsi
	;; again
	jmp	next
return_str:
	ret
;;
;; Print number
;;
print:
	;;;; calculate number length
	mov	rax, 1
	mul	r12
	mov	r12, 8
	mul	r12
	mov	rdx, rax
	;;;;
	;;;; print sum
	mov	rax, SYS_WRITE
	mov	rdi, STD_IN
	mov	rsi, rsp
	;; call sys_write
	syscall
	;;
	;; newline
	jmp	printNewline
;;
;; Print number
;;
printNewline:
	mov	rax, SYS_WRITE
	mov	rdi, STD_IN
	mov	rsi, NEW_LINE
	mov	rdx, 1
	syscall
	jmp	exit
;;
;; Exit from program
;;
exit:
	;; syscall number
	mov	rax, SYS_EXIT
	;; exit code
	mov	rdi, EXIT_CODE
	;; call sys_exit
	syscall
```
## makefile
```
all:
	nasm -f elf64 -o stack.o stack.asm
	ld  -o stack stack.o
clean:
	rm stack stack.o
```
# string
## reverse.asm
```
;;
;; initialized data
;;
section .data
	SYS_WRITE	equ 1
	STD_OUT		equ 1
	SYS_EXIT	equ 60
	EXIT_CODE	equ 0
	NEW_LINE	db 0xa
	INPUT		db "Hello world!"
;;
;; non initialized data
;;
section .bss
	OUTPUT	resb 1
;;
;; code
;;
section .text
	global	_start
;;
;; main routine
;;
_start:
	;; get addres of INPUT
	mov	rsi, INPUT
	;; zeroize rcx for counter
	xor	rcx, rcx
	; df = 0 si++
	cld
	; remember place after function call
	mov	rdi, $ + 15
	;; get string lengt
	call	calculateStrLength
	;; write zeros to rax
	xor	rax, rax
	;; additional counter for reverseStr
	xor	rdi, rdi
	;; reverse string
	jmp	reverseStr
;;
;; calculate length of string
;;
calculateStrLength:
	;; check is it end of string
	cmp	byte [rsi], 0
	;; if yes exit from function
	je	exitFromRoutine
	;; load byte from rsi to al and inc rsi
	lodsb
	;; push symbol to stack
	push	rax
	;; increase counter
	inc	rcx
	;; loop again
	jmp	calculateStrLength
;;
;; back to _start
;;
exitFromRoutine:
	;; push return addres to stack again
	push	rdi
	;; return to _start
	ret
;;
;; reverse string
;;
;; 31 in stack
reverseStr:
	;; check is it end of string
	cmp	rcx, 0
	;; if yes print result string
	je	printResult
	;; get symbol from stack
	pop	rax
	;; write it to output buffer
	mov	[OUTPUT + rdi], rax
	;; decrease length counter
	dec	rcx
	;; increase additional length counter (for write syscall)
	inc	rdi
	;; loop again
	jmp	reverseStr
;;
;; Print result string
;;
printResult:
	mov	rdx, rdi
	mov	rax, 1
	mov	rdi, 1
	mov	rsi, OUTPUT
	syscall
	jmp	printNewLine
;;
;; Print new line
;;
printNewLine:
	mov	rax, SYS_WRITE
	mov	rdi, STD_OUT
	mov	rsi, NEW_LINE
	mov	rdx, 1
	syscall
	jmp	exit
;;
;; Exit from program
;;
exit:
	;; syscall number
	mov	rax, SYS_EXIT
	;; exit code
	mov	rdi, EXIT_CODE
	;; call sys_exit
	syscall
```
## makefile
```
all:
	nasm -g -f elf64 -o reverse.o reverse.asm
	ld -o reverse reverse.o
clean:
	rm reverse reverse.o
```
# sum
## sum.asm
```
;initialised data section
section .data
	; Define constants
	num1:	equ 100
	num2:	equ 50
	; http://geekswithblogs.net/MarkPearl/archive/2011/04/13/more-nasm.aspx
	msg:	db "Sum is correct", 10
;;
;; program code
;;
section .text
	global	_start
;; entry point
_start:
	; get sum of num1 and num2
	mov	rax, num1
	mov	rbx, num2
	add	rax, rbx
	; compare rax with correct sum - 150
	cmp	rax, 150
	; if rax is not 150 go to exit
	jne	.exit
	; if rax is 150 print msg
	jmp	.rightSum
; Print message that sum is correct
.rightSum:
	;; write syscall
	mov	rax, 1
	;; file descritor, standard output
	mov	rdi, 1
	;; message address
	mov	rsi, msg
	;; length of message
	mov	rdx, 15
	;; call write syscall
	syscall
	; exit from program
	jmp	.exit
; exit procedure
.exit:
	mov	rax, 60
	mov	rdi, 0
	syscall
```
## makefile
```
all:
	nasm -f elf64 -o sum.o sum.asm
	ld -o sum sum.o
clean:
	rm sum sum.o
```
# C语言与汇编之间调用
## casm1 - call C function from asm
### casm.c
```cpp
#include <stdio.h>
extern int print();
int print() {
	printf("Hello World\n");
	return 0;
}
```
### casm.asm
```
global _start
extern print
section .text
_start:
	call	print
	mov	rax, 60
	mov	rdi, 0
	syscall
```
### makefile
```
buildAsmc:
	gcc  -c casm.c -o c.o
	nasm -f elf64 casm.asm -o casm.o
	ld   -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc casm.o c.o -o casm
clean:
	rm -rf *.o
	rm -rf casm
```
## casm2 - gcc inline assembly
### casm.c
```cpp
#include <string.h>
int main() {
	char* str = "Hello World\n";
	long len = strlen(str);
	int ret = 0;
	__asm__("movq $1, %%rax \n\t"
		"movq $1, %%rdi \n\t"
		"movq %1, %%rsi \n\t"
		"movl %2, %%edx \n\t"
		"syscall"
		: "=g"(ret)
		: "g"(str), "g" (len));
	return 0;
}
```
### makefile
```
build:
	gcc casm.c -o casm
```
## casm3 - call asm function from C
### casm.c
```cpp
#include <string.h>
extern void printHelloWorld(char *str, int len);
int main() {
	char* str = "Hello World\n";
	int len = strlen(str);
	printHelloWorld(str, len);
	return 0;
}
```
### casm.asm
```
global printHelloWorld
section .text
printHelloWorld:
	;; 1 arg
	mov	r10, rdi
	;; 2 arg
	mov	r11, rsi
	;; call write syscall
	mov	rax, 1
	mov	rdi, 1
	mov	rsi, r10
	mov	rdx, r11
	syscall
	ret
```
### makefile
```
build:
	nasm -f elf64 -o casm.o casm.asm
	gcc casm.o casm.c -o casm
```
GitHub：[https://github.com/0xAX/asm](https://github.com/0xAX/asm)
