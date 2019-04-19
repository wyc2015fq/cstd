# C语言编写汇编的编译器，用c编写一个asm的编译器 - Koma Hub - CSDN博客
2019年03月17日 21:12:14[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：25
> 
GitHub源项目名称：[asmBuilder]()
**Table of Contents**
[汇编代码](#%E6%B1%87%E7%BC%96%E4%BB%A3%E7%A0%81)
[hello.asm](#hello.asm)
[incTest.asm](#incTest.asm)
[io.inc](#io.inc)
[std.inc](#std.inc)
[编译c代码](#%E7%BC%96%E8%AF%91c%E4%BB%A3%E7%A0%81)
[compiler.c](#compiler.c)
[头文件defineList.h](#%E5%A4%B4%E6%96%87%E4%BB%B6defineList.h)
# 汇编代码
## hello.asm
```
;--------------------------------------------------------------------------- 
; Hello World program
; written by Alexandre Savelli Bencz
;--------------------------------------------------------------------------- 
;--------------------------------------------------------------------------- 
; here comes the code  
;--------------------------------------------------------------------------- 
  call @hello                 ; call hello world procedure
  mov  ax,04c00h              ; ax<-4C00 DOS Exit
  int  021h                   ; call DOS interrupt
@hello:
  push cs                     ; put cs onto the stack
  pop  ds                     ; retrieve cs into ds
  mov  dx,offset HelloWorld   ; move string offset into dx
  mov  ah,9                   ; ah<-9 DOS print string
  int  021h                   ; call DOS interrupt
  ret                         ; return back to caller
;---------------------------------------------------------------------------
;  now here's the data
;---------------------------------------------------------------------------
HelloWorld  db "Hello World!$"
```
## incTest.asm
```
;--------------------------------------------------------------------------- 
; Hello World program
; written by Alexandre Savelli Bencz
;--------------------------------------------------------------------------- 
.include "include\std.inc"
;--------------------------------------------------------------------------- 
; here comes the code  
;--------------------------------------------------------------------------- 
  call @hello                 ; call hello world procedure
  jmp @exit
@hello:
  push cs                     ; put cs onto the stack
  pop  ds                     ; retrieve cs into ds
  mov  dx,offset HelloWorld   ; move string offset into dx
  mov  ah,9                   ; ah<-9 DOS print string
  int  021h                   ; call DOS interrupt
  ret                         ; return back to caller
;---------------------------------------------------------------------------
;  now here's the data
;---------------------------------------------------------------------------
HelloWorld  db "Hello World!$"
```
## io.inc
```
;============================================================================
; io.inc
; written by Alexandre Savelli Bencz, 2013
; A few file manipulation procedures for assembly programs written for the
; General Assembler
;
;============================================================================
;============================================================================
; Code
;============================================================================
Buffersize  equ  0100h      ;Text file buffer size
;----------------------------------------------------------------------------
; @OpenFile
; params  segment:offset of ascii NULL string containing file name 4 bytes
;         read/write  00 = read, 01 = write 02 = read/write        2 bytes
; returns AX = handle
;
;   Opens a file for reading or writing, if writing to a file then the
; file will be created or if an existing file of the same name exists,
; it will be overwritten
;----------------------------------------------------------------------------
filename  equ  [bp+6]
rwflag    equ  [bp+4]
@OpenFile:
  push  bp                  ;setup stack frame
  mov   bp,sp
  push  ds                  ;save ds
  push  dx                  ;save dx
  push  bx                  ;save bx
  lds   dx,filename         ;load file name
  mov   ax,rwflag           ;ax<-read/write flag
  cmp   ax,1                ;write a file?
  jne   @@OpenFile2         ;if not jump
@@OpenFile1:
  xor   cx,cx               ;clear cx
  mov   ah,03Ch             ;ah<-03ch DOS create file
  int   21h                 ;call DOS interrupt
  jc    @@CreateFileError   ;if error occured quit
  mov   bx,ax               ;Close the opened file
  mov   ah,03eh
  int   021h
  lds   dx,filename         ;reload the file name
@@OpenFile2:
  mov   al,0C0h             ;al<-0C0h DOS read/write flag
  or    al,rwflag           ;or in flag
  mov   ah,03dh             ;ah<-03dh DOS open file
  int   021h                ;call DOS
  jc    @@OpenFileError     ;jump on error
@@OpenFile3:
  pop   bx                  ;restore bx
  pop   dx                  ;restore dx
  pop   ds                  ;restore ds
  pop   bp                  ;restore bp
  ret   6                   ;return
@@OpenFileError:
  mov   ax,rwflag           ;check flag
  cmp   ax,2
  je    @@OpenFile1         ;is r/w flag? then truncate file
  push  cs                  ;ds<-cs
  pop   ds
  mov   dx,offset OpenFileErrorMessage
@@OpenFileError1:
  mov   ah,09h              ;ah<-09h DOS print string
  int   021h                ;call DOS interrupt
  lds   dx,filename         ;get the file name
  push  ds                  ;put ds on stack
  push  dx                  ;put dx on stack
  call  @WritePchar         ;display file name
  jmp   @exit               ;quit
@@CreateFileError:
  push  cs                  ;ds<-cs
  pop   ds
  mov   dx,offset CreateFileErrorMessage
  jmp   @@OpenFileError1    ;let code above finish this
;----------------------------------------------------------------------------
; @CloseFile
; params  filehandle  2 bytes
;----------------------------------------------------------------------------
filehandle   equ [bp+4]
@CloseFile:
  push  bp                  ;setup stack frame
  mov   bp,sp
  push  bx                  ;save bx
  push  ax                  ;save ax
  mov   bx,filehandle       ;bx<-filehandle
  mov   ah,03eh             ;ax<-03eh DOS close file
  int   021h                ;call DOS interrupt
  pop   ax                  ;restore ax
  pop   bx                  ;restore bx
  pop   bp                  ;restore bp
  ret   2                   ;return
;----------------------------------------------------------------------------
; @ReadFile
; params  seg:ofs of buffer to read to    4 bytes
;         number bytes to read            2 bytes
;         filehandle                      2 bytes
; returns AX=number bytes read
;----------------------------------------------------------------------------
buffer   equ  [bp+8]
count    equ  [bp+6]
handle   equ  [bp+4]
@ReadFile:
  push  bp                  ;setup stack frame
  mov   bp,sp
  push  cx                  ;save cx
  push  dx                  ;save dx
  push  bx                  ;save bx
  push  ds                  ;save ds
  lds   dx,buffer           ;ds:dx = address at bp+8
  mov   cx,count            ;cx<-[bp+6]
  mov   bx,handle           ;bx<-[bp+4]
  mov   ah,03fh             ;ah<-03fh
  int   021h                ;call dos
  jc    @@ReadFile2         ;read error occured
@@ReadFile1:
  pop   ds                  ;restore ds
  pop   bx                  ;restore bx
  pop   dx                  ;restore dx
  pop   cx                  ;restore cx
  pop   bp                  ;restore bp
  ret   8                   ;
@@ReadFile2:                ;had a read error
  xor   ax,ax               ;null out ax
  jmp   @@ReadFile1         ;return
;----------------------------------------------------------------------------
; @WriteFile
; params  seg:ofs of buffer to write from 4 bytes
;         number bytes to write           2 bytes
;         filehandle                      2 bytes
; returns AX=Number bytes written
;----------------------------------------------------------------------------
buffer   equ  [bp+8]
count    equ  [bp+6]
handle   equ  [bp+4]
@WriteFile:
  push  bp                  ;setup stack frame
  mov   bp,sp
  push  cx                  ;save cx
  push  dx                  ;save dx
  push  bx                  ;save bx
  push  ds                  ;save ds
  lds   dx,buffer           ;load buffer
  mov   cx,count            ;cx<-number of bytes to read from buffer
  mov   bx,handle           ;bx<-file handle
  mov   ah,040h             ;ah<-40h, DOS write to file from buffer
  int   021h                ;call DOS interrupt
  jc    @@writeerror        ;jump on error
@@WriteFile1:
  pop   ds                  ;restore ds
  pop   bx                  ;restore bx
  pop   dx                  ;restore dx
  pop   cx                  ;restore cx
  pop   bp                  ;restore bp
  ret   8                   ;return to call
@@writeerror:               ;
  xor   ax,ax               ;return zero bytes written if error
  jmp   @@WriteFile1        ;return
;----------------------------------------------------------------------------
; @OpenTextFile:
; params  seg:ofs of buffer to write from 4 bytes
;         read/write 00=read,01=write     2 bytes
; returns ax:dx = filestructure
;FILERECORD STRUCT
;    FR_handle dw     ?          read/write handle
;    FR_pos    dw     ?          position in buffer
;    FR_size   dw     ?          read/write buffer size
;    FR_flag   dw     ?          read/write buffer?
;    FR_buffer dw  0100h dup(?)  read/write buffer
;FILERECORD ENDS
;----------------------------------------------------------------------------
fname     equ  [bp+6]
rwflag    equ  [bp+4]
@OpenTextFile:
  push   bp                 ;setup stack frame
  mov    bp,sp
  push   es                 ;save es
  push   di                 ;save di
  push   bx                 ;save bx
  push   cx                 ;save cx
  les   si,fname            ;load in filename pointer
  push  es                  ;put in on the back stack
  push  si                  ;
  mov   ax,rwflag           ;now get the read write flag
  and   ax,01h              ;can only be read or write
  push  ax                  ;put flag onto the stack
  call  @OpenFile           ;now open the file
  push  ax                  ;save handle
  mov   ax,BufferSize + 8   ;File Record Size + BufferSize
  call  @AllocateMem        ;Allocate mem for file record
  mov   es,ax               ;es:di points to the read/write buffer
  mov   di,dx
  pop   ax                  ;restore handle
  mov   es:[di],ax          ;(file handle)
  xor   ax,ax               ;ax=0
  mov   es:[di+2],ax        ;(pos)
  mov   es:[di+4],ax        ;(length)
  mov   ax,rwflag           ;(read/write)
  mov   es:[di+6],ax        ;(r/w flag)
  cmp   ax,0                ;reading from text?
  jne   @@OpenTextFile1     ;if not then jump
  push  es                  ;put record segment on stack
  mov   ax,di               ;put record offset on stack
  add   ax,8                ;move up to buffer
  push  ax                  ;put offfset on stack
  mov   ax,BufferSize       ;ax<-Buffersize
  push  ax                  ;put buffersize on stack
  mov   ax,es:[di]          ;(file handle)
  push  ax                  ;put file handle onto the stack
  call  @ReadFile           ;read the file
  mov   es:[di+4],ax        ;save the number of bytes read
@@OpenTextFile1:
  mov   ax,es               ;ax<-record segment
  mov   dx,di               ;dx<-record offset
  pop   cx                  ;restore cx
  pop   bx                  ;restore bx
  pop   di                  ;restore di
  pop   es                  ;restore es
  pop   bp                  ;restore bp
  ret   6
;----------------------------------------------------------------------------
; @CloseTextFile
; params  seg:ofs to file record          4 bytes
;----------------------------------------------------------------------------
file   equ  [bp+4]
@CloseTextFile:
  push  bp                  ;setup stack frame
  mov   bp,sp
  push  ds                  ;save ds
  push  si                  ;save si
  push  dx                  ;save dx
  push  ax                  ;save ax
  lds   si,file             ;load file record
  mov   ax,[si+6]           ;check r/w flag
  cmp   ax,0                ;ax=0? read
  je    @@CloseTextFile1    ;jump if nothing to write
; dispose of write buffer
@@CloseTextFile1:
  mov   ax,[si]             ;get filehandle
  push  ax                  ;put it onto the stack
  call  @CloseFile          ;close the file
  mov   ax,ds               ;put record segment into ax
  mov   dx,si               ;put record offset into dx
  call  @FreeMem            ;get that memory back
  pop   ax                  ;restore ax
  pop   dx                  ;restore dx
  pop   si                  ;restore si
  pop   ds                  ;restore ds
  pop   bp                  ;restore bp
  ret   4
;----------------------------------------------------------------------------
; @ReadTextFile
; params  seg:ofs of buffer to write to   4 bytes
;         max number bytes to read        2 bytes
;         seg:ofs to file record          4 bytes
;
; Read text file into seg:ofs as a NULL terminated string when then end of
; a line has been met or max number of bytes-1 have been read.
;----------------------------------------------------------------------------
buffer  equ  [bp+10]
count   equ  [bp+8]
file    equ  [bp+4]
@ReadTextFile:
  push  bp                  ;setup stack frame
  mov   bp,sp
  push  es                  ;save es
  push  ds                  ;save ds
  push  si                  ;save si
  push  di                  ;save di
  push  ax                  ;save ax
  push  cx                  ;save cx
  push  dx                  ;save dx
  push  bx                  ;save bx
  cld                       ;clear decrement
  les   di,buffer           ;load buffer to write to
  lds   si,file             ;load file record
  mov   cx,count            ;cx<-Max number of bytes to read
  dec   cx                  ;
  mov   bx,[si+2]           ;
@@ReadTextFile1:            ;
  mov   al,[si+bx+8]        ;
  cmp   al,0dh              ;
  je    @@ReadTextFile2     ;
  cmp   al,0ah              ;
  je    @@ReadTextFile2     ;
  stosb                     ;
  dec   cx                  ;
@@ReadTextFile2:            ;
  inc   bx                  ;
  mov   dx,[si+2]           ;(position)
  inc   dx                  ;
  mov   [si+2],dx           ;(position)
  cmp   dx,[si+4]           ;(size)
  jne   @@ReadTextFile3     ;
  cmp   dx,BufferSize       ;end of file?
  jne   @@ReadTextFile4     ;
; read more into buffer
  push  ds                  ;
  mov   ax,si               ;
  add   ax,8                ;
  push  ax                  ;
  mov   ax,BufferSize       ;
  push  ax                  ;
  mov   ax,[si]             ;(file handle)
  push  ax                  ;
  call  @ReadFile           ;
  mov   [si+4],ax           ;(size)
  xor   ax,ax               ;
  mov   [si+2],ax           ;(position)
  xor   bx,bx               ;
@@ReadTextFile3:            ;
  cmp   al,0ah              ;
  je    @@ReadTextFile4     ;
  jcxz  @@ReadTextFile4     ;
  jmp   @@ReadTextFile1     ;
@@ReadTextFile4:            ;
  xor   al,al               ;
  stosb                     ;
  pop   bx                  ;restore bx
  pop   dx                  ;restore dx
  pop   cx                  ;restore cx
  pop   ax                  ;restore ax
  pop   di                  ;restore di
  pop   si                  ;restore si
  pop   ds                  ;restore ds
  pop   es                  ;restore es
  pop   bp                  ;restore bp
  ret   10
;----------------------------------------------------------------------------
; @TextEOF
; params  seg:ofs to file record          4 bytes
; returns ax=1 if EOF
;----------------------------------------------------------------------------
file   equ  [bp+4]
@TextEOF:
  push  bp                  ;setup stack frame
  mov   bp,sp
  push  ds                  ;save ds
  push  si                  ;save di
  xor   ax,ax               ;null out ax
  lds   si,file             ;ds:si=file record
  mov   bx,[si+2]           ;bx=buffer position
  cmp   bx,[si+4]           ;bx==number bytes read?
  jne   @@TextEOF1          ;no then quit
  cmp   bx,BufferSize       ;at end of read buffer?
  je    @@TextEOF1          ;yes then quit
  mov   ax,1                ;pos==size != fullsize
@@TextEOF1:
  pop   si                  ;restore di
  pop   ds                  ;restore ds
  pop   bp                  ;restore bp
  ret   4
;----------------------------------------------------------------------------
; @WritePchar
; params  seg:ofs of string               4 bytes
;----------------------------------------------------------------------------
string  equ  [bp+4]
@WritePchar:
  push  bp                  ;setup stack frame
  mov   bp,sp
  push  ds                  ;save ds
  push  si                  ;save si
  push  ax                  ;save ax
  push  dx                  ;save dx
  lds   si,string           ;load string pointer
@@WritePchar1:
  lodsb                     ;read a byte
  cmp   al,0                ;is it null?
  je    @@WritePchar2       ;yes? then done
  mov   ah,02h              ;ah<-02h DOS print char
  mov   dl,al               ;move character to dl
  int   021h                ;call DOS
  jmp   @@WritePchar1       ;keep going
@@WritePchar2:
  pop   dx                  ;restore dx
  pop   ax                  ;restore ax
  pop   si                  ;restore si
  pop   ds                  ;restore ds
  pop   bp                  ;restore bp
  ret   4                   ;return
;============================================================================
; Data
;============================================================================
OpenFileErrorMessage   db "Unable to open file: $"
CreateFileErrorMessage db "Unable to create file: $"
```
## std.inc
```
;============================================================================
; std.inc
; written by Alexandre Savelli Bencz, 2013
; Startup code for assembly programs written for General Assembler
;  Comments may be a bit lacking, but I have other things to work on
;============================================================================
STACKSIZE  equ  4096
; This is where the program will begin  
  mov   ax,[002ch]          ;get environment segment
  mov   [@envseg],ax        ;save it
  mov   si,0080h            ;get command line index
  lodsb                     ;get byte count
  mov   cl,al               ;put byte count in cx
  xor   ch,ch               ;clear ch
  cmp   cx,080h             ;only have 128 bytes
  ja    @skipcommandline    ;to large?
  mov   [@commcount],cx     ;save the count
  push  cs                  ;es=cs
  pop   es
  mov   di,@commline        ;es:di = @commline
  rep   movsb               ;copy it over
@skipcommandline:
;----------------------------------------------------------------------------
; save interrupt vectors 0 and 6
  mov   al,0                ;al<-0 for interrupt 0
  mov   ah,035h             ;ah<-35, DOS get interrupt vector
  int   021h                ;call DOS
  mov   ds:[@@SaveInt0],bx  ;save interrupt vector offset
  mov   bx,es               ;bx<-es, interrupt vector segment
  mov   ds:[@@SaveInt0+2],bx;save interrupt vector segment
  mov   al,6                ;al<-0 for interrupt 6
  mov   ah,035h             ;ah<-35, DOS get interrupt vector
  int   021h                ;call DOS
  mov   ds:[@@SaveInt6],bx  ;save interrupt vector offset
  mov   bx,es               ;bx<-es, interrupt vector segment
  mov   ds:[@@SaveInt6+2],bx;save interrupt vector segment
;----------------------------------------------------------------------------
; reset vectors to handle errors
  push  cs                  ;push cs
  pop   ds                  ;ds<-cs
  mov   al,0                ;al<-0 for interrupt 0
  mov   dx,@@DivisionByZero ;dx<-@@DivisionByZero offset
  mov   ah,025h             ;ah<-25, DOS set interrupt vector
  int   021h                ;call DOS
  push  cs                  ;push cs
  pop   ds                  ;ds<-cs
  mov   al,6                ;al<-6 for interrupt 6
  mov   dx,@@InvalidOpCode  ;dx<-@@InvalidOpCode offset
  mov   ah,025h             ;ah<-25, DOS set interrupt vector
  int   021h                ;call DOS
;----------------------------------------------------------------------------
; Expand the program's block size to allow for dynamic allocation
; So long as there is nothing allocated in the segment directly after the
; program was loaded this should work
  push  cs                  ;push cs
  pop   es                  ;es<-cs
  mov   bx,@@FinalOffset/16+2;bx<-@@FinalOffset(size of program block)
  mov   ah,04ah             ;ah<-4a, DOS resize block
  int   021h                ;call DOS
  jnc   @@BlockSizeOk       ;did resize work?
  mov   dx,@@BlockSizeErrorMessage
  jmp   @@ErrorExit         ;had an error, so exit
@@BlockSizeOk:
;----------------------------------------------------------------------------
;create more stack space
  mov   ax,STACKSIZE        ;ax<-4K
  call  @AllocateMem        ;allocate the mem
  mov   ss,ax               ;ss<-ax, new stack segment
  mov   sp,STACKSIZE        ;sp<-4K new stack pointer
;----------------------------------------------------------------------------
; call to the main code
;----------------------------------------------------------------------------
  mov   ax,cs               ;ax<-cs
  mov   ds,ax               ;ds=cs
  mov   es,ax               ;es=cs
  call  @start              ;call main program
;----------------------------------------------------------------------------
@exit:
;----------------------------------------------------------------------------
; get interrupt vectors back
  lds   dx,[@@SaveInt0]     ;ds:dx = old int 0
  mov   al,0                ;al<-0, int 0
  mov   ah,025h             ;ah<-25, DOS set Interrupt
  int   021h                ;call DOS
  lds   dx,[@@SaveInt6]     ;ds:dx = old int 6
  mov   al,6                ;al<-6, int 6
  mov   ah,025h             ;ah<-25, DOS set interrupt
  int   021h                ;call DOS
; get stack back
  mov   ax,ss               ;ax<-ss, get stack segment
  mov   bx,0100h            ;bx<-100, new stack size
  mov   sp,bx               ;sp<-bx=100, new stack size
  mov   bx,cs               ;bx<-cs
  mov   ss,bx               ;ss<-bx=cs,get back old stack segment
  call  @FreeMem            ;free allocated stack space
; go back to DOS
  mov   ah,04ch             ;ah<-4c, DOS terminate
  int   021h                ;call DOS
@@ErrorExit:
  push  cs                  ;push cs
  pop   ds                  ;ds = cs
  mov   ah,09h              ;ah<-9, DOS print string
  int   021h                ;call DOS
  jmp   @exit               ;quit program
;============================================================================
; Procedures
;============================================================================
;----------------------------------------------------------------------------
; New Interrupt 0
;----------------------------------------------------------------------------
@@DivisionByZero:
  mov   dx,@@DivideByZeroErrorMessage
  mov   ah,09h              ;write error message
  int   021h
  mov   bp,sp               ;write segment to screen
  mov   ax,[bp+2]
  mov   dx,4
  call  @WriteHexVal
  mov   dl,':'              ;write ':' to screen
  mov   ah,2
  int   021h
  mov   ax,[bp]             ;write offset to screen
  mov   dx,4
  call  @WriteHexVal
  jmp   @exit               ;exit program
;----------------------------------------------------------------------------
; New Interrupt 6
;----------------------------------------------------------------------------
@@InvalidOpCode:
  mov   dx,@@InvalidOpCodeErrorMessage
  mov   ah,09h              ;write error message
  int   021h
  mov   bp,sp               ;write segment to screen
  mov   ax,[bp+2]
  mov   dx,4
  call  @WriteHexVal
  mov   dl,':'              ;write ':' to screen
  mov   ah,2
  int   021h
  mov   ax,[bp]             ;write offset to screen
  mov   dx,4
  call  @WriteHexVal
  jmp   @exit               ;exit program
;----------------------------------------------------------------------------
; @AllocateMem
; AX = amount to allocate
; return AX:DX = memory location
;----------------------------------------------------------------------------
@AllocateMem:
  push  cx                  ;save cx
  push  bx                  ;save bx
  mov   bx,ax               ;bx<-ax
  mov   cl,4                ;cl<-4
  shr   bx,cl               ;bx = bx / 16
  inc   bx
  mov   ah,048h             ;ah<-48, allocate mem
  int   021h                ;call DOS
  jc    @@AllocMemError     ;error?
  xor   dx,dx               ;clear dx
  pop   bx                  ;restore bx
  pop   cx                  ;restore cx
  ret                       ;return
@@AllocMemError:
  mov   dx,@@NoMemMessage   ;display error message
  jmp   @@ErrorExit         ;quit
;----------------------------------------------------------------------------
; @FreeMem
; AX:DX = memory location
;----------------------------------------------------------------------------
@FreeMem:
  push  es                  ;save es
  mov   es,ax               ;es<-ax
  mov   ah,049h             ;ah<-49, free mem
  int   021h                ;call DOS
  pop   es                  ;restore es
  jc    @@FreeMemError      ;error?
  ret                       ;return
@@FreeMemError:
  mov   dx,@@FreeMemErrorMessage
  jmp   @@ErrorExit         ;display error message and quit
;----------------------------------------------------------------------------
; @WriteHexVal
; AX = Value
; Dx = digits
;----------------------------------------------------------------------------
@WriteHexVal:
  push  si                  ;save si
  push  ds                  ;save ds
  push  bx                  ;save bx
  push  cx                  ;save cx
  push  cs                  ;ds<-cs
  pop   ds
  mov   si,@@HexValues      ;si<-@@HexValues
  xor   dh,dh               ;clear dh
  mov   cx,dx               ;cx<-dx
@@WriteHexValLoop1:
  mov   bx,ax               ;bx<-ax
  and   bx,0fh              ;bx = bx & 0f
  mov   bx,[si+bx]          ;bx<-hex value
  push  bx                  ;push bx
  shr   ax                  ;ax = ax/16
  shr   ax
  shr   ax
  shr   ax
  loop  @@WriteHexValLoop1  ;go until no more values
  mov   cx,dx               ;cx<-dx
@@WriteHexValLoop2:
  pop   dx                  ;get value off stack
  mov   ah,02h              ;print character
  int   021h
  loop  @@WriteHexValLoop2
  pop   cx                  ;restore cx
  pop   bx                  ;restore bx
  pop   ds                  ;restore ds
  pop   si                  ;restore si
  ret
;============================================================================
; Data
;============================================================================
@envvars:   dw 0             ;environment vars
@envseg:    dw 0             ;environment vars segment
@commcount: dw 0             ;number of commands
@commline:  db 128 dup (0)   ;copy of the command line
@@SaveInt0: dw 0,0           ;Area used to save interrupts  int 0
@@SaveInt6: dw 0,0           ;                              int 6
@@HexValues: db "0123456789ABCDEF"
@@DivideByZeroErrorMessage:
  db 0dh,0ah,"Division by zero at: $"
@@InvalidOpCodeErrorMessage:
  db 0dh,0ah,"Invalid Opcode encountered at: $"
@@NoMemMessage:
  db "Error: Out of memory.$"
@@FreeMemErrorMessage:
  db "Error: Unable to free memory.$"
@@BlockSizeErrorMessage:
  db "Error: Unable to adjust program's block size.$"
```
# 编译c代码
## compiler.c
```cpp
#include <stdio.h>
#include <stdlib.h>
#include "defineList.h"
#define lower(x) (((x)>0x40 && (x)<0x5b)?(x)|0x20:(x))
#define READBUFFERSIZE	512
#define WRITEBUFFERSIZE	512
#ifdef DEBUG
char readbuffer[READBUFFERSIZE];
#endif
int buffpos = 0;                 // position in write buffer         
char wbuffer[WRITEBUFFERSIZE];   // used to buffer the output        
unsigned long org = 0x100;       // com file start offset           
unsigned long offset;            // keep tabs of the offset           
unsigned long filesize;          // keep tabs of the output file size
char errorflag = 0;              // check for errors               
unsigned char memreg2val;        // second register in a memory ref   
unsigned long memimmediate;      // immediate in a mem ref          
unsigned char PointerRef;        // assemble in 32 bit pointer?    
char dirtylabels=0;              // see if labels need fixing     
// table to store labels, names and values
typedef struct sltable
{
	struct sltable *next;
	char *name;
	unsigned long value;
} tltable;
// table to store equate names and values
typedef struct setable
{
	struct setable *next;
	char *name;
	char *value;
	char flag;
} tetable;
tltable *ltable = NULL;
tetable *etable = NULL;
typedef struct
{
	char name[4];
	unsigned char value;
	unsigned long flag;
} tregisters;
#define NUMREGS 20
tregisters registers[] =
{
	"al",0x00,regal|reg8		,"ah",0x04,regah|reg8		,"ax",0x00,regax|reg16,
	"cl",0x01,regcl|reg8		,"ch",0x05,regch|reg8		,"cx",0x01,regcx|reg16,
	"dl",0x02,regdl|reg8		,"dh",0x06,regdh|reg8		,"dx",0x02,regdx|reg16,
	"bl",0x03,regbl|reg8		,"bh",0x07,regbh|reg8		,"bx",0x03,regbx|reg16,
	"sp",0x04,regsp|reg8		,"bp",0x05,reg16|regbp		,"si",0x06,reg16|regsi,
	"di",0x07,reg16|regdi		,"es",0x00,reges|seg16		,"ds",0x03,regds|seg16,
	"ss",0x02,regss|seg16		,"cs",0x01,regcs|seg16
#ifdef CODE32BIT
	,"eax",0x00,regeax|reg32	,"ecx",0x01,regecx|reg32	,"edx",0x02,regedx|reg32
	,"ebx",0x03,regebx|reg32	,"esp",0x04,regesp|reg32	,"ebp",0x05,regebp|reg32
	,"esi",0x06,regesi|reg32	,"edi",0x07,regedi|reg32
#endif
};
typedef struct
{
	char name[6];
	int value;
} tstr_opcodes;
typedef struct
{
	int ocode;
	int length;
	unsigned char code[3];
	unsigned short val1,val2,val3;
	unsigned short instruct;      // instructions for assembling code
} topcodes;
enum eopcodes { 
	opnone		,opaaa		,opaad		,opaam,
	opaas		,opadc		,opadd		,opand,
	opclc		,opcld		,opcli		,opcall,
	opcbw		,opcmc		,opcmp		,opcmpsb,
	opcmpsw		,opcwd		,opdaa		,opdas,
	opdec		,opdiv		,ophlt		,opidiv,
	opimul		,opin		,opinc		,opint,
	opiret		,opja		,opjae		,opjb,
	opjbe		,opjc		,opjcxz		,opje,
	opjg		,opjge		,opjl		,opjle,
	opjmp		,opjmpf		,opjna		,opjnae,
	opjnb		,opjnbe		,opjnc		,opjne,
	opjng		,opjnge		,opjnl		,opjnle,
	opjno		,opjnp		,opjns		,opjnz,
	opjo		,opjp		,opjpe		,opjpo,
	opjs		,opjz		,oplahf		,oplds,
	oplea		,oples		,oplodsb	,oplodsw,
	oploop		,oploope	,oploopne	,oploopnz,
	oploopz		,opmov		,opmovsb	,opmovsw,
	opmul		,opneg		,opnop		,opnot,
	opor		,opout		,oppopf		,oppushf,
	oprcl		,oprcr		,oppop		,oppush,
	oprep		,oprepe		,oprepne	,oprepnz,
	oprepz		,opret		,opretf		,oprol,
	opror		,opsahf		,opsal		,opsar,
	opsbb		,opsub		,opscasb	,opscasw,
	opshl		,opshr		,opstc		,opstd,
	opsti		,opstosb	,opstosw	,optest,
	opwait		,opxchg		,opxlat		,opxor,
	op32bit
};
#define NUMOPCODES  116
tstr_opcodes str_opcodes[NUMOPCODES] =
{
	"aaa",opaaa,"aad",opaad,"aam",opaam,"aas",opaas,
	"adc",opadc,"add",opadd,"and",opand,"call",opcall,
	"cbw",opcbw,"clc",opclc,"cld",opcld,"cli",opcli,"cmc",opcmc,
	"cmp",opcmp,"cmpsb",opcmpsb,"cmpsw",opcmpsw,"cwd",opcwd,
	"daa",opdaa,"das",opdas,"dec",opdec,"div",opdiv,"hlt",ophlt,
	"idiv",opidiv,"imul",opimul,"in",opin,"inc",opinc,"int",opint,
	"iret",opiret,"ja",opja,"jae",opjae,"jb",opjb,"jbe",opjbe,
	"jc",opjc,"jcxz",opjcxz,"je",opje,"jg",opjg,"jge",opjge,
	"jl",opjl,"jle",opjle,"jna",opjna,"jnae",opjnae,"jnb",opjnb,
	"jnbe",opjnbe,"jnc",opjnc,"jne",opjne,"jng",opjng,"jnge",opjnge,
	"jnl",opjnl,"jnle",opjnle,"jno",opjno,"jnp",opjnp,"jns",opjns,
	"jnz",opjnz,"jo",opjo,"jp",opjp,"jpe",opjpe,"jpo",opjpo,
	"js",opjs,"jz",opjz,"jmp",opjmp,"jmpf",opjmpf,"lahf",oplahf,"lds",oplds,
	"lea",oplea,"les",oples,"lodsb",oplodsb,"lodsw",oplodsw,
	"loop",oploop,"loope",oploope,"loopne",oploopne,"loopnz",oploopnz,
	"loopz",oploopz,"mov",opmov,"movsb",opmovsb,"movsw",opmovsw,
	"mul",opmul,"neg",opneg,"nop",opnop,"not",opnot,"or",opor,
	"out",opout,"pop",oppop,"push",oppush,"popf",oppopf,
	"pushf",oppushf,"rcl",oprcl,"rcr",oprcr,"rep",oprep,
	"repe",oprepe,"repne",oprepne,"repnz",oprepnz,"repz",oprepz,
	"ret",opret,"retf",opretf,"rol",oprol,"ror",opror,"sahf",opsahf,
	"sal",opsal,"sar",opsar,"sbb",opsbb,"scasb",opscasb,
	"scasw",opscasw,"shl",opshl,"shr",opshr,"stc",opstc,"std",opstd,
	"sti",opsti,"stosb",opstosb,"stosw",opstosw,"sub",opsub,
	"test",optest,"wait",opwait,"xchg",opxchg,"xlat",opxlat,
	"xor",opxor,
	"32bit",op32bit
};
#define NUMINSTRUCTIONS (262+17)
/* machine language values */
topcodes opcodes[NUMINSTRUCTIONS] =
{
	/* opcode operand1,operand2,operand3 */
	/* ascii adjusts */
	opaaa,1,0x37,0x00,0x00,none,none,none,Inothing,
	opaad,2,0xd5,0x0a,0x00,none,none,none,Inothing,
	opaam,2,0xd4,0x0a,0x00,none,none,none,Inothing,
	opaas,1,0x3f,0x00,0x00,none,none,none,Inothing,
	/* add with carry */
	opadc,1,0x14,0x00,0x00,regal,immed8,none,Iimmedsecond,
	opadc,1,0x15,0x00,0x00,regax,immed16,none,Iimmedsecond,
	opadc,1,0x80,0xd0,0x00,reg8,immed8,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	opadc,1,0x10,0x00,0x00,reg16|memref,reg8,none,Ifirstval|Isecondval|Icalreg,
	opadc,1,0x81,0x06,0x00,reg16,immed16,none,Ifirstval|Ical2|Icalreg|Iimmedsecond,
	opadc,1,0x11,0x40,0x00,reg16|memref,reg16,none,Ifirstval|Isecondval|Icalreg,
	opadc,1,0x12,0x00,0x00,reg8,reg8,none,Ifirstval|Isecondval|Icalreg,
	opadc,1,0x12,0x00,0x00,reg8,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	opadc,1,0x13,0x00,0x00,reg16,reg16,none,Ifirstval|Isecondval|Icalreg,
	opadc,1,0x13,0x00,0x00,reg16,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	opadc,1,0x81,0x10,0x00,reg16|memref,immed16,none,Ifirstval|Icalreg|Iimmedsecond,
	opadc,1,0x83,0x10,0x00,reg16|memref,immed8,none,Ifirstval|Icalreg|Iimmedsecond,
	/* add */
	opadd,1,0x04,0x00,0x00,regal,immed8,none,Iimmedsecond,
	opadd,1,0x05,0x00,0x00,regax,immed16,none,Iimmedsecond,
	opadd,1,0x80,0xc0,0x00,reg8,immed8,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	opadd,1,0x00,0x00,0x00,reg16|memref,reg8,none,Ifirstval|Isecondval|Icalreg,
	opadd,1,0x81,0xc0,0x00,reg16,immed16,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	opadd,1,0x01,0x00,0x00,reg16|memref,reg16,none,Ifirstval|Isecondval|Icalreg,
	opadd,1,0x02,0x00,0x00,reg8,reg8,none,Ifirstval|Isecondval|Icalreg,
	opadd,1,0x02,0x00,0x00,reg8,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	opadd,1,0x03,0x00,0x00,reg16,reg16,none,Ifirstval|Isecondval|Icalreg,
	opadd,1,0x03,0x00,0x00,reg16,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	opadd,1,0x81,0x00,0x00,reg16|memref,immed16,none,Ifirstval|Icalreg|Iimmedsecond,
	opadd,1,0x83,0x00,0x00,reg16|memref,immed8,none,Ifirstval|Icalreg|Iimmedsecond,
	/* and */
	opand,1,0x24,0x00,0x00,regal,immed8,none,Iimmedsecond,
	opand,1,0x25,0x00,0x00,regax,immed16,none,Iimmedsecond,
	opand,1,0x80,0xe0,0x00,reg8,immed8,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	opand,1,0x20,0x00,0x00,reg16|memref,reg8,none,Ifirstval|Isecondval|Icalreg,
	opand,1,0x81,0xe0,0x00,reg16,immed16,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	opand,1,0x21,0x00,0x00,reg16|memref,reg16,none,Ifirstval|Isecondval|Icalreg,
	opand,1,0x22,0x00,0x00,reg8,reg8,none,Ifirstval|Isecondval|Icalreg,
	opand,1,0x22,0x00,0x00,reg8,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	opand,1,0x23,0x00,0x00,reg16,reg16,none,Ifirstval|Isecondval|Icalreg,
	opand,1,0x23,0x00,0x00,reg16,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	opand,1,0x81,0x20,0x00,reg16|memref,immed16,none,Ifirstval|Icalreg|Iimmedsecond,
	opand,1,0x83,0x20,0x00,reg16|memref,immed8,none,Ifirstval|Icalreg|Iimmedsecond,
	/* call */
	opcall,1,0xe8,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump,
	opcall,1,0xff,0x10,0x00,reg16|memref,none,none,Ifirstval|Icalreg,
	opcall,1,0x9a,0x00,0x00,pointer,none,none,Iimmedfirst,
	/* clear flag bits */
	opcbw,1,0x98,0x00,0x00,none,none,none,Inothing,
	opclc,1,0xf8,0x00,0x00,none,none,none,Inothing,
	opcld,1,0xfc,0x00,0x00,none,none,none,Inothing,
	opcli,1,0xfa,0x00,0x00,none,none,none,Inothing,
	opcmc,1,0xf5,0x00,0x00,none,none,none,Inothing,
	/* compare */
	opcmp,1,0x3c,0x00,0x00,regal,immed8,none,Iimmedsecond,
	opcmp,1,0x3d,0x00,0x00,regax,immed16,none,Iimmedsecond,
	opcmp,1,0x3a,0x00,0x00,reg8,reg8,none,Ifirstval|Isecondval|Icalreg,
	opcmp,1,0x3b,0x00,0x00,reg16,reg16,none,Ifirstval|Isecondval|Icalreg,
	opcmp,1,0x3a,0x00,0x00,reg8,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	opcmp,1,0x38,0x00,0x00,reg16|memref,reg8,none,Ifirstval|Isecondval|Icalreg,
	opcmp,1,0x3b,0x00,0x00,reg16,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	opcmp,1,0x39,0x00,0x00,reg16|memref,reg16,none,Ifirstval|Isecondval|Icalreg,
	opcmp,1,0x80,0xf8,0x00,reg8,immed8,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	opcmp,1,0x81,0xf8,0x00,reg16,immed16,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	opcmp,1,0x81,0x38,0x00,reg16|memref,immed16,none,Ifirstval|Icalreg|Iimmedsecond,
	opcmp,1,0x80,0x38,0x00,reg16|memref,immed8,none,Ifirstval|Icalreg|Iimmedsecond,
	opcmpsb,1,0xa6,0x00,0x00,none,none,none,Inothing,
	opcmpsw,1,0xa7,0x00,0x00,none,none,none,Inothing,
	opcwd,1,0x99,0x00,0x00,none,none,none,Inothing,
	opdaa,1,0x27,0x00,0x00,none,none,none,Inothing,
	opdas,1,0x2f,0x00,0x00,none,none,none,Inothing,
	/* decrease */
	opdec,1,0xfe,0xc8,0x00,reg8,none,none,Ifirstval|Ical1|Icalreg,
	opdec,0,0x48,0x00,0x00,reg16,none,none,Ifirstval|Ical1|Icalreg,
	opdec,0,0x48,0x00,0x00,reg16|memref,none,none,Ifirstval|Ical1|Icalreg,
	/* divide */
	opdiv,1,0xf6,0xf0,0x00,reg8,none,none,Ifirstval|Ical1|Icalreg,
	opdiv,1,0xf7,0xf0,0x00,reg16,none,none,Ifirstval|Ical1|Icalreg,
	/* halt */
	ophlt,1,0xf4,0x00,0x00,none,none,none,Inothing,
	/* integer divide */
	opidiv,1,0xf6,0xf8,0x00,reg8,none,none,Ifirstval|Ical1|Icalreg,
	opidiv,1,0xf7,0xf8,0x00,reg16,none,none,Ifirstval|Ical1|Icalreg,
	/* integer multiply */
	opimul,1,0xf6,0xe8,0x00,reg8,none,none,Ifirstval|Ical1|Icalreg,
	opimul,1,0xf7,0xe8,0x00,reg16,none,none,Ifirstval|Ical1|Icalreg,
	/* in */
	/* this could be a slight flaw */
	opin,1,0xe4,0x00,0x00,regal,immed8,none,Iimmedsecond, 
	opin,1,0xec,0x00,0x00,regal,regdx,none,Inothing,
	opin,1,0xe5,0x00,0x00,regax,immed16,none,Iimmedsecond,
	opin,1,0xed,0x00,0x00,regax,regdx,none,Inothing,
	/* inc */
	opinc,1,0xfe,0xc0,0x00,reg8,none,none,Ifirstval|Ical1|Icalreg,
	opinc,0,0x40,0x00,0x00,reg16,none,none,Ifirstval|Ical1|Icalreg,
	opinc,0,0x40,0x00,0x00,reg16|memref,none,none,Ifirstval|Icalreg,
	/* issue interrupt */
	opint,1,0xcd,0x00,0x00,immed16,none,none,Iimmedfirst|Iwrite8,
	/* interrupt return */
	opiret,1,0xcf,0x00,0x00,none,none,none,Inothing,
	/* basic jump */
	opjmp,1,0xe9,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump,
	opjmp,1,0xea,0x00,0x00,pointer,none,none,Iimmedfirst,
	opjmpf,1,0xff,0x2e,0x00,reg16|memref,none,none,Ifirstval|Icalreg,
	/* comparitive jumps */
	opja, 1,0x77,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjae,1,0x73,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjb, 1,0x72,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjbe,1,0x76,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjc, 1,0x72,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjcxz,1,0xe3,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opje, 1,0x74,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjg, 1,0x7f,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjge,1,0x7d,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjl, 1,0x7c,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjle,1,0x7e,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjna,1,0x76,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjnae,1,0x72,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjnb,1,0x73,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjnbe,1,0x77,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjnc,1,0x73,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjne,1,0x75,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjng,1,0x7e,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjnge,1,0x7c,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjnl,1,0x7d,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjnle,1,0x7f,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjno,1,0x71,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjnp,1,0x7b,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjns,1,0x79,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjnz,1,0x75,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjo, 1,0x70,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjp, 1,0x7a,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjpe,1,0x7a,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjpo,1,0x7b,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjs, 1,0x78,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	opjz, 1,0x74,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	/* lahf */
	oplahf,1,0x9f,0x00,0x00,none,none,none,Inothing,
	/* lds */
	oplds,1,0xc5,0x00,0x00,reg16,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	/*les*/
	oplds,1,0x8d,0x00,0x00,reg16,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	/* les */
	oples,1,0xc4,0x00,0x00,reg16,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	/* loads */
	oplodsb,1,0xac,0x00,0x00,none,none,none,Inothing,
	oplodsw,1,0xad,0x00,0x00,none,none,none,Inothing,
	/* loops */
	oploop,1,0xe2,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	oploope,1,0xe1,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	oploopne,1,0xe0,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	oploopnz,1,0xe0,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	oploopz,1,0xe1,0x00,0x00,immed16,none,none,Iimmedfirst|Ijump|Iwrite8,
	/* from reg to mem */
	/* 8 bit two byte operands should be rewritten */
	opmov,1,0x88,0x00,0x00,reg16|memref,reg8,none,Ifirstval|Isecondval|Icalreg,
	/* 16 bit */
	opmov,1,0x89,0x00,0x00,reg16|memref,reg16,none,Ifirstval|Isecondval|Icalreg,
	/* 8 and 16 bit register moves */
	opmov,1,0x8a,0x00,0x00,reg8 ,reg8,none,Ifirstval|Isecondval|Icalreg,
	opmov,1,0x8b,0x00,0x00,reg16,reg16,none,Ifirstval|Isecondval|Icalreg,
	opmov,1,0x8e,0x00,0x00,seg16,reg16,none,Ifirstval|Isecondval|Icalreg,
	opmov,1,0x8c,0x00,0x00,reg16,seg16,none,Ifirstval|Isecondval|Icalreg|Iswapval,
	/* from mem to reg */
	/* 8bit */
	opmov,1,0x8a,0x00,0x00,reg8,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	/* 16 bit */
	opmov,1,0x8b,0x00,0x00,reg16,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	/* 8 bit immediate moves */
	opmov,0,0xb0,0x00,0x00,reg8,immed8,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	/* 16 bit immediate moves */
	opmov,0,0xb8,0x00,0x00,reg16,immed16,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	opmov,1,0xc7,0x00,0x00,reg16|memref,immed16,none,Ifirstval|Icalreg|Iimmedsecond,
	opmov,1,0xc6,0x00,0x00,reg16|memref,immed8,none,Ifirstval|Icalreg|Iimmedsecond,
	/* string moves */
	opmovsb,1,0xa4,0x00,0x00,none,none,none,Inothing,
	opmovsw,1,0xa5,0x00,0x00,none,none,none,Inothing,
	/* multiply */
	opmul,1,0xf6,0xe0,0x00,reg8,none,none,Ifirstval|Ical1|Icalreg,
	opmul,1,0xf7,0xe0,0x00,reg16,none,none,Ifirstval|Ical1|Icalreg,
	/* negative */
	opneg,1,0xf6,0xd8,0x00,reg8,none,none,Ifirstval|Ical1|Icalreg,
	opneg,1,0xf7,0xd8,0x00,reg16,none,none,Ifirstval|Ical1|Icalreg,
	/* no operation */
	opnop,1,0x90,0x00,0x00,none,none,none,Inothing,
	/* not */
	opnot,1,0xf6,0xd0,0x00,reg8,none,none,Ifirstval|Ical1|Icalreg,
	opnot,1,0xf7,0xd0,0x00,reg16,none,none,Ifirstval|Ical1|Icalreg,
	/* or */
	opor,1,0x0c,0x00,0x00,regal,immed8,none,Iimmedsecond,
	opor,1,0x0d,0x00,0x00,regax,immed16,none,Iimmedsecond,
	opor,1,0x80,0xc8,0x00,reg8,immed8,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	opor,1,0x08,0x00,0x00,reg16|memref,reg8,none,Ifirstval|Isecondval|Icalreg,
	opor,1,0x81,0xc8,0x00,reg16,immed16,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	opor,1,0x09,0x00,0x00,reg16|memref,reg16,none,Ifirstval|Isecondval|Icalreg,
	opor,1,0x0a,0x00,0x00,reg8,reg8,none,Ifirstval|Isecondval|Icalreg,
	opor,1,0x0a,0x00,0x00,reg8,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	opor,1,0x0b,0x00,0x00,reg16,reg16,none,Ifirstval|Isecondval|Icalreg,
	opor,1,0x0b,0x00,0x00,reg16,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	opor,1,0x81,0x08,0x00,reg16|memref,immed16,none,Ifirstval|Icalreg|Iimmedsecond,
	opor,1,0x80,0x08,0x00,reg16|memref,immed8,none,Ifirstval|Icalreg|Iimmedsecond,
	/* out */
	opout,1,0xe6,0x00,0x00,immed16,regal,none,Iimmedsecond|Iwrite8,
	opout,1,0xee,0x00,0x00,regdx,regal,none,Inothing,
	opout,1,0xe7,0x00,0x00,immed16,regax,none,Iimmedsecond,
	opout,1,0xef,0x00,0x00,regdx,regax,none,Inothing,
	/* pop */
	oppop,0,0x07,0x00,0x00,seg16,none,none,Ifirstval|Ical2|Icalreg,
	oppop,0,0x58,0x00,0x00,reg16,none,none,Ifirstval|Ical1|Icalreg,
	oppop,1,0x8f,0x00,0x00,reg16|memref,none,none,Ifirstval|Icalreg,
	oppopf,1,0x9d,0x00,0x00,none,none,none,Inothing,
	/* push */
	oppush,0,0x06,0x00,0x00,seg16,none,none,Ifirstval|Ical2|Icalreg,
	oppush,0,0x50,0x00,0x00,reg16,none,none,Ifirstval|Ical1|Icalreg,
	oppush,1,0xff,0x30,0x00,reg16|memref,none,none,Ifirstval|Icalreg,
	oppushf,1,0x9c,0x00,0x00,none,none,none,Inothing,
	/* rotate left through carry */
	oprcl,1,0xd0,0xd0,0x00,reg8,none,none,Ifirstval|Ical1|Icalreg,
	oprcl,1,0xd2,0xd0,0x00,reg8,regcl,none,Ifirstval|Ical1|Icalreg,
	oprcl,1,0xd1,0xd0,0x00,reg16,none,none,Ifirstval|Ical1|Icalreg,
	oprcl,1,0xd3,0xd0,0x00,reg16,regcl,none,Ifirstval|Ical1|Icalreg,
	/* rotate right through carry */
	oprcr,1,0xd0,0xd8,0x00,reg8,none,none,Ifirstval|Ical1|Icalreg,
	oprcr,1,0xd2,0xd8,0x00,reg8,regcl,none,Ifirstval|Ical1|Icalreg,
	oprcr,1,0xd1,0xd8,0x00,reg16,none,none,Ifirstval|Ical1|Icalreg,
	oprcr,1,0xd3,0xd8,0x00,reg16,regcl,none,Ifirstval|Ical1|Icalreg,
	/* repeat */
	oprep,1,0xf3,0x00,0x00,none,none,none,Inothing,
	oprepe,1,0xf3,0x00,0x00,none,none,none,Inothing,
	oprepne,1,0xf2,0x00,0x00,none,none,none,Inothing,
	oprepnz,1,0xf2,0x00,0x00,none,none,none,Inothing,
	oprepz,1,0xf3,0x00,0x00,none,none,none,Inothing,
	/* return */
	opretf,1,0xcb,0x00,0x00,none,none,none,Inothing,
	opret,1,0xc2,0x00,0x00,immed16,none,none,Iimmedfirst,
	opret,1,0xc3,0x00,0x00,none,none,none,Inothing,
	/* rotate left */
	oprol,1,0xd0,0xc0,0x00,reg8,none,none,Ifirstval|Ical1|Icalreg,
	oprol,1,0xd2,0xc0,0x00,reg8,regcl,none,Ifirstval|Ical1|Icalreg,
	oprol,1,0xd1,0xc0,0x00,reg16,none,none,Ifirstval|Ical1|Icalreg,
	oprol,1,0xd3,0xc0,0x00,reg16,regcl,none,Ifirstval|Ical1|Icalreg,
	/* rotate right */
	opror,1,0xd0,0xc8,0x00,reg8,none,none,Ifirstval|Ical1|Icalreg,
	opror,1,0xd2,0xc8,0x00,reg8,regcl,none,Ifirstval|Ical1|Icalreg,
	opror,1,0xd1,0xc8,0x00,reg16,none,none,Ifirstval|Ical1|Icalreg,
	opror,1,0xd3,0xc8,0x00,reg16,regcl,none,Ifirstval|Ical1|Icalreg,
	/* store flags in ah */
	opsahf,1,0x9e,0x00,0x00,none,none,none,Inothing,
	/* arithmetic shift left */
	opsal,1,0xd0,0xe0,0x00,reg8,none,none,Ifirstval|Ical1|Icalreg,
	opsal,1,0xd2,0xe0,0x00,reg8,regcl,none,Ifirstval|Ical1|Icalreg,
	opsal,1,0xd1,0xe0,0x00,reg16,none,none,Ifirstval|Ical1|Icalreg,
	opsal,1,0xd3,0xe0,0x00,reg16,regcl,none,Ifirstval|Ical1|Icalreg,
	/* arithmetic shift right */
	opsar,1,0xd0,0xf8,0x00,reg8,none,none,Ifirstval|Ical1|Icalreg,
	opsar,1,0xd2,0xf8,0x00,reg8,regcl,none,Ifirstval|Ical1|Icalreg,
	opsar,1,0xd1,0xf8,0x00,reg16,none,none,Ifirstval|Ical1|Icalreg,
	opsar,1,0xd3,0xf8,0x00,reg16,regcl,none,Ifirstval|Ical1|Icalreg,
	/* sbb */
	opsbb,1,0x1c,0x00,0x00,regal,immed8,none,Iimmedsecond,
	opsbb,1,0x1d,0x00,0x00,regax,immed16,none,Iimmedsecond,
	opsbb,1,0x80,0xd8,0x00,reg8,immed8,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	opsbb,1,0x18,0x00,0x00,reg16|memref,reg8,none,Ifirstval|Isecondval|Icalreg,
	opsbb,1,0x81,0xd8,0x00,reg16,immed16,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	opsbb,1,0x19,0x00,0x00,reg16|memref,reg16,none,Ifirstval|Isecondval|Icalreg,
	opsbb,1,0x1a,0x00,0x00,reg8,reg8,none,Ifirstval|Isecondval|Icalreg,
	opsbb,1,0x1a,0x00,0x00,reg8,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	opsbb,1,0x1b,0x00,0x00,reg16,reg16,none,Ifirstval|Isecondval|Icalreg,
	opsbb,1,0x1b,0x00,0x00,reg16,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	opsbb,1,0x81,0x18,0x00,reg16|memref,immed16,none,Ifirstval|Icalreg|Iimmedsecond,
	opsbb,1,0x80,0x18,0x00,reg16|memref,immed8,none,Ifirstval|Icalreg|Iimmedsecond,
	/* compares */
	opscasb,1,0xae,0x00,0x00,none,none,none,Inothing,
	opscasw,1,0xaf,0x00,0x00,none,none,none,Inothing,
	/* shift left */
	opshl,1,0xc0,0xe0,0x00,reg8,immed8,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	opshl,1,0xc1,0xe0,0x00,reg16,immed16,none,Ifirstval|Ical1|Icalreg|Iimmedsecond|Iwrite8,
	opshl,1,0xd2,0xe0,0x00,reg8,regcl,none,Ifirstval|Ical1|Icalreg,
	opshl,1,0xd0,0xe0,0x00,reg8,none,none,Ifirstval|Ical1|Icalreg,
	opshl,1,0xd3,0xe0,0x00,reg16,regcl,none,Ifirstval|Ical1|Icalreg,
	opshl,1,0xd1,0xe0,0x00,reg16,none,none,Ifirstval|Ical1|Icalreg,
	/* shift right */
	/* I know that these are 286 instructions, but I had to have them */
	opshr,1,0xc0,0xe8,0x00,reg8,immed8,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	opshr,1,0xc1,0xe8,0x00,reg16,immed16,none,Ifirstval|Ical1|Icalreg|Iimmedsecond|Iwrite8,
	opshr,1,0xd2,0xe8,0x00,reg8,regcl,none,Ifirstval|Ical1|Icalreg,
	opshr,1,0xd0,0xe8,0x00,reg8,none,none,Ifirstval|Ical1|Icalreg,
	opshr,1,0xd3,0xe8,0x00,reg16,regcl,none,Ifirstval|Ical1|Icalreg,
	opshr,1,0xd1,0xe8,0x00,reg16,none,none,Ifirstval|Ical1|Icalreg,
	/* set flags */
	opstc,1,0xf9,0x00,0x00,none,none,none,Inothing,
	opstd,1,0xfd,0x00,0x00,none,none,none,Inothing,
	opsti,1,0xfb,0x00,0x00,none,none,none,Inothing,
	/* store */
	opstosb,1,0xaa,0x00,0x00,none,none,none,Inothing,
	opstosw,1,0xab,0x00,0x00,none,none,none,Inothing,
	/* sub */
	opsub,1,0x2c,0x00,0x00,regal,immed8,none,Iimmedsecond,
	opsub,1,0x2d,0x00,0x00,regax,immed16,none,Iimmedsecond,
	opsub,1,0x80,0xe8,0x00,reg8,immed8,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	opsub,1,0x28,0x00,0x00,reg16|memref,reg8,none,Ifirstval|Isecondval|Icalreg,
	opsub,1,0x81,0xe8,0x00,reg16,immed16,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	opsub,1,0x29,0x00,0x00,reg16|memref,reg16,none,Ifirstval|Isecondval|Icalreg,
	opsub,1,0x2a,0x00,0x00,reg8,reg8,none,Ifirstval|Isecondval|Icalreg,
	opsub,1,0x2a,0x00,0x00,reg8,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	opsub,1,0x2b,0x00,0x00,reg16,reg16,none,Ifirstval|Isecondval|Icalreg,
	opsub,1,0x2b,0x00,0x00,reg16,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	opsub,1,0x81,0x28,0x00,reg16|memref,immed16,none,Ifirstval|Icalreg|Iimmedsecond,
	opsub,1,0x80,0x28,0x00,reg16|memref,immed8,none,Ifirstval|Icalreg|Iimmedsecond,
	/* test */
	optest,1,0xa8,0x00,0x00,regal,immed8,none,Iimmedsecond,
	optest,1,0xa9,0x00,0x00,regax,immed16,none,Iimmedsecond,
	optest,1,0xf6,0xc0,0x00,reg8,immed8,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	optest,1,0x84,0x00,0x00,reg16|memref,reg8,none,Ifirstval|Isecondval|Icalreg,
	optest,1,0xf7,0xc0,0x00,reg16,immed16,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	optest,1,0x85,0x00,0x00,reg16|memref,reg16,none,Ifirstval|Isecondval|Icalreg|Iimmedfirst,
	optest,1,0x84,0x00,0x00,reg8,reg8,none,Ifirstval|Isecondval|Icalreg,
	optest,1,0x84,0x00,0x00,reg8,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	optest,1,0x85,0x00,0x00,reg16,reg16,none,Ifirstval|Isecondval|Icalreg,
	optest,1,0x85,0x00,0x00,reg16,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	optest,1,0xf7,0x00,0x00,reg16|memref,immed16,none,Ifirstval|Icalreg|Iimmedsecond,
	/* wait */
	opwait,1,0x9b,0x00,0x00,none,none,none,Inothing,
	/* exchange */
	opxchg,0,0x90,0x00,0x00,regax,reg16,none,Ifirstval|Ical1|Icalreg,
	opxchg,1,0x86,0x00,0x00,reg8,reg8,none,Ifirstval|Isecondval|Icalreg,
	opxchg,1,0x87,0x00,0x00,reg16,reg16,none,Ifirstval|Isecondval|Icalreg,
	/* translate */
	opxlat,1,0xd7,0x00,0x00,none,none,none,Inothing,
	/* xor */
	opxor,1,0x34,0x00,0x00,regal,immed8,none,Iimmedsecond,
	opxor,1,0x35,0x00,0x00,regax,immed16,none,Iimmedsecond,
	opxor,1,0x80,0xe8,0x00,reg8,immed8,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	opxor,1,0x30,0x00,0x00,reg16|memref,reg8,none,Ifirstval|Isecondval|Icalreg,
	opxor,1,0x81,0xe8,0x00,reg16,immed16,none,Ifirstval|Ical1|Icalreg|Iimmedsecond,
	opxor,1,0x31,0x00,0x00,reg16|memref,reg16,none,Ifirstval|Isecondval|Icalreg,
	opxor,1,0x32,0x00,0x00,reg8,reg8,none,Ifirstval|Isecondval|Icalreg,
	opxor,1,0x32,0x00,0x00,reg8,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	opxor,1,0x33,0x00,0x00,reg16,reg16,none,Ifirstval|Isecondval|Icalreg,
	opxor,1,0x33,0x00,0x00,reg16,reg16|memref,none,Ifirstval|Isecondval|Icalreg,
	opxor,1,0x81,0x30,0x00,reg16|memref,immed16,none,Ifirstval|Icalreg|Iimmedsecond,
	opxor,1,0x80,0x30,0x00,reg16|memref,immed8,none,Ifirstval|Icalreg|Iimmedsecond,
	op32bit,1,0x66,0x00,0x00,none,none,none,Inothing
};
unsigned char cal1reg(unsigned long val1)
{
	unsigned char firstregvalue=0;
	int j;
	for (j=0;j<NUMREGS;j++)
		if ((val1 & 0x0000fff0) == (registers[j].flag & 0x0000fff0))
			firstregvalue = registers[j].value;
	return (firstregvalue);
}
unsigned char cal2reg(unsigned long val1)
{
	unsigned char firstregvalue=0;
	int j;
	for (j=0;j<NUMREGS;j++)
		if ((val1 & 0x0000fff0) == (registers[j].flag & 0x0000fff0))
			firstregvalue = registers[j].value;
	return (firstregvalue << 3);
}
/* determine if register can be mem referenced */
unsigned char ismemreg(unsigned char reg)
{
	if (reg == 3)  /* bx */
		return 0x07;
	else
		if (reg == 5)  /* bp */
			return 0x06;
		else
			if (reg == 7)  /* di */
				return 0x05;
			else
				if (reg == 6)  /* si */
					return 0x04;
				else
					if (reg != 0)
					{
						printf("** invalid memory register ** ");
						errorflag = 1;
						return 0;
					}
					return 0x06;
}
/* can have more than one register in mem ref */
unsigned char addsecondval(unsigned char val1,unsigned char val2)
{
	unsigned char tmp;
	if (val1 < val2)
	{
		tmp = val1;
		val1 = val2;
		val2 = tmp;
	}
	if (val1 == 0x07 && val2 == 0x04) /* bx+si */
		return 0x00;
	else
		if (val1 == 0x07 && val2 == 0x05) /* bx+di */
			return 0x01;
		else
			if (val1 == 0x06 && val2 == 0x04) /* bp+si */
				return 0x02;
			else
				if (val1 == 0x06 && val2 == 0x05) /* bp+di */
					return 0x03;
				else
				{
					printf("** invalid memory register combination ** ");
					errorflag = 1;
					return 0;
				}
}
/* This may be a bit sloppy, it's possibly slow and I could replace
it with a table, but for now it works */
unsigned char calculateregvalue(unsigned long val1,unsigned long val2,
								unsigned short * instruct)
{
	unsigned char firstregvalue=0,secondregvalue=0;
	unsigned char ret;
	int j;
	/* get the first operand */
	for (j=0;j<NUMREGS;j++)
		if ((val1 & 0x0000fff0) == (registers[j].flag & 0x0000fff0))
			firstregvalue = registers[j].value;
	for (j=0;j<NUMREGS;j++)
		if ((val2 & 0x0000fff0) == (registers[j].flag & 0x0000fff0))
			secondregvalue = registers[j].value;
	/* first register is a memory reference, so calculate it's value */
	if (val1 & memref)
	{
		ret = ismemreg(firstregvalue);
		if (memreg2val)
			ret = addsecondval(ret,ismemreg(memreg2val));
		ret = secondregvalue << 3 | ret;
		if (firstregvalue == 0)
			*instruct |= Iimmedfirst;
		else
			if (memimmediate > 0xff)
			{
				*instruct |= Iimmedfirst;
				ret |= 0x80;
			}
			else
				if (memimmediate != 0 || firstregvalue == 0x05)
				{
					ret |= 0x40;
					*instruct |= Iimmedfirst;
					*instruct |= Iwrite8;
				}
				return ret;
	}
	else
		/* second register is a memory reference, so calculate it's value */
		if (val2 & memref)
		{
			ret = ismemreg(secondregvalue);
			if (memreg2val)
				ret = addsecondval(ret,ismemreg(memreg2val));
			ret = firstregvalue << 3 | ret;
			/* mem immediate */
			if (secondregvalue == 0)
				*instruct |= Iimmedsecond;
			else
				if (memimmediate > 0xff)
				{
					ret |= 0x80;
					*instruct |= Iimmedsecond;
				}
				else
					if (memimmediate != 0 || secondregvalue == 0x05)
					{
						ret |= 0x40;
						*instruct |= Iimmedsecond;
						*instruct |= Iwrite8;
					}
					return ret;
		}
		/* it's just a register combination */
		else
			return (((firstregvalue << 3 | secondregvalue)& 0x3f) | 0xc0);
}
/* get the length of a string */
char stringlen(char * source)
{
	int i = 0;
	while (source[i]!=0) i++;
	return i;
}
/* check to see if sub-string is in the string at the current dest */
char matches(char *source,char *dest)
{
	int i,j=0;
	if (source == NULL)
		printf("1\n");
	for (i=0;source[i] != 32 && source[i] != 9 && source[i] != ']'&&
		source[i] != '+' && source[i] != '-' && source[i] != 0x0d
		&& source[i] != 0x0a && source[i] != 0 && source[i] != ','
		&& source[i] != ':' && source[i] != '('&& source[i] != '['&&
		source[i] != '*' && source[i] != '/' && source[i] != ';'
		;i++)
		if (lower(source[i]) != lower(dest[i]))
			return 0;
	while (dest[j]!=0 && dest[j]!=']') j++;
	if (i<j)
		return 0;
	else
		return 1;
}
/* forward reference */
unsigned long getlabel(char **tmp);
/* parse out a value from a string */
unsigned long getval(char **input,int pass)
{
	unsigned long ret = 0;
	char neg = 0,hex = 0;
	if (**input == '+')(*input)++;
	else
		if (**input == '-'){(*input)++;neg=1;}
		if (**input == '?'){(*input)++;return 0;}
		if (**input == '$'){(*input)++;return offset;}
		if (**input == '@' && pass == 2) return getlabel(input);
		if (**input == '0'){(*input)++;hex=1;}
		if (**input == '@' && pass == 1)
		{
			while (**input != 0 && **input != 0x0a && **input != ']'
				&& **input !=';' && **input != 32 && **input != 9
				&& **input != '+' && **input != '-' && **input != '*' &&
				**input != '/' && **input != ';')
				(*input)++;
			return 0xffff;
		}
		/* ran across a character quote */
		if (**input == 39)
		{
			(*input)++;
			ret = (unsigned long)(**input);
			(*input)++;
			if (**input != 39)
			{
				ret = ((unsigned long)(**input) << 8) + ret;
				(*input)++;
			}
			if (**input != 39)
			{
				printf("** no closing quote on character ** ");
				errorflag = 1;
			}
			(*input)++;
		}
		else
			while (**input != ']' && **input != ',' && **input != 32 && **input != 0x0d
				&& **input != 0x0a && **input != 0 && **input != 9
				&& **input != ')' && **input != '+' && **input != '-' &&
				**input != '/'  && **input != '*' && **input != ';')
			{
				if ((**input|0x20)!= 'h')
				{
					/* convert a dec value to a hex value */
					if (((**input | 0x20) >= ('a')) && ((**input | 0x20) <= ('f'))
						&& !hex)
					{
						unsigned short oldret;
						int i;
						oldret = ret;
						ret = (short)0;
						i = 0;
						while (oldret)
						{
							ret = (short)(ret + ((oldret % 10) << (i*4)));
							oldret /= 10;
							i++;
						}
						hex = 1;
					}
					/* pointer reference */
					if (**input == ':')
					{
						PointerRef = 1;
						ret <<= 16;
						(*input)++;
					}
					else
						/* calculate the value */
						if (hex)
							ret = (int)(ret << 4 | ((**input<'A')?((**input-'0')%10) : (((**input | 0x20)-('A'-10))%16)));
						else
							if (**input >= '0' && **input <= '9')
								ret = (int)(ret * 10 + ((**input - '0')%10));
							else
							{
								printf("** invalid immediate ** ");
								errorflag = 1;
								return 0;
							}
				}
				else
					if (!hex) /* if calculated in B10, but in B16 */
					{
						unsigned short oldret;
						int i;
						oldret = ret;
						ret = (short)0;
						i = 0;
						while (oldret)
						{
							ret = (short)(ret + ((oldret % 10) << (i*4)));
							oldret /= 10;
							i++;
						}
					}
					(*input)++;
			}
			if (neg)
				ret = (~ret)+1;
			return ret;
}
void writebuffer(const void *ptr,int size,int count,FILE *fp)
{
	int i;
	for (i = 0;i < (size*count);i++)
	{
		wbuffer[buffpos++] = ((char *)ptr)[i];
		if (buffpos == WRITEBUFFERSIZE)
		{
			fwrite(wbuffer,WRITEBUFFERSIZE,1,fp) ;
			buffpos = 0;
		}
	}
}
/* insert equate value into string */
char resolveequate(char *tmp,int pass)
{
	tetable * next = etable;
	int len,i;
	char *lastpos,offsetflag = 0;
	char filler[]="[0ffff]";
	if (matches(tmp,"dup"))
		return 2;
	/* looking for an offset ? */
	if (matches(tmp,"offset"))
	{
		lastpos = tmp;
		len = 6;
		while(*(tmp+len))
		{
			*tmp = *(tmp+len);
			tmp++;
		}
		*tmp = 0;
		tmp = lastpos;
		while ((*tmp) == 32 || (*tmp) == 9 || (*tmp) == 0x0d)
			tmp++;
		offsetflag = 1;
	}
	for (i=0;i<NUMOPCODES;i++)
		if (matches(tmp,str_opcodes[i].name))
			return 2;
	while (next != NULL && !matches(tmp,next->name))
		next = next->next;
	if (next)
	{
		/* get rid of old data */
		lastpos = tmp;
		len = stringlen(next->name);
		while(*(tmp+len))
		{
			*tmp = *(tmp+len);
			tmp++;
		}
		*tmp = 0;
		/* insert new data */
		len = stringlen(next->value);
		/* are we looking for an offset? */
		if (offsetflag)
		{
			len -= 2;
			if (!next->flag)
				return 0;
			next->value++;
		}
		while( (tmp) != lastpos )
		{
			*(tmp+len) = *tmp;
			tmp --;
		}
		*(tmp+len) = *tmp;
		for (i=0;i<len;i++)
			tmp[i] = next->value[i];
		if (offsetflag)              /* correct the offset */
			next->value--;
	}
	else
		if (pass == 1)
		{
			lastpos = tmp;
			for (len=0 ;(tmp[len] != 32 && tmp[len] != 9 && tmp[len] != 0x0d
				&& tmp[len] != 0 && tmp[len] != ',' && tmp[len] != ';'
				&& tmp[len] != ']');len++);
			while(*(tmp+len))
			{
				*tmp = *(tmp+len);
				tmp++;
			}
			*tmp = 0;
			/* insert new data */
			len = stringlen(filler);
			/* are we looking for an offset? */
			while( (tmp) != lastpos )
			{
				*(tmp+len) = *tmp;
				tmp --;
			}
			lastpos=filler;
			if (offsetflag)
			{
				len -= 2;
				lastpos++;
			}
			*(tmp+len) = *tmp;
			for (i=0;i<len;i++)
				tmp[i] = lastpos[i];
		}
		else
			return 0;
	return 1;
}
char addequate(char ** label,int pass)
{
	static char buffer[256],buffer2[256];
	int i,j,datalabel;
	i = 0;
	j = 0;
	datalabel = 0;
	while (**label != 32 && **label != 9 && **label != '=')
	{
		buffer[i] = **label;
		i++;
		(*label)++;
	}
	buffer[i] = 0;
	while (**label == 32 || **label == 9) (*label)++;
	if (matches(*label,"equ"))
		(*label) +=3;
	else
		if ((**label) == '=')
			(*label)++;
		else
			/* see if it is a label */
			if (matches(*label,"db") || matches(*label,"dw"))
				datalabel = 1;
			else
				return 0;
	/* it's a label, so fill it whith an offset */
	while (**label == 32 || **label == 9) (*label)++;
	if (datalabel)
	{
		buffer2[0] = '[';
		itoa(offset,&(buffer2[1]),10);
		j = stringlen(buffer2);
		buffer2[j] = ']';
		buffer2[j+1] = 0;
	}
	else
		/* else it's an equate, so fill it with it's value */
	{
		while (**label != ';' && **label != 0x0d
			&& **label != 0x0a && **label != 0)
		{
			if (**label == '$')
			{
				itoa(offset,&(buffer2[j]),10);
				j = stringlen(buffer2);
			}
			else
			{
				buffer2[j] = **label;
				j++;
			}
			(*label)++;
		}
		buffer2[j] = 32;
		while (buffer2[j] == 32 || buffer2[j] == 9)
			buffer2[j--] = 0;
	}
	/* no table, make new one */
	if (etable == NULL)
	{
		/* allocate mem for equate link */
		if (!(etable = (tetable *)malloc(sizeof(tetable))))
		{
			printf("!! OUCH, I'm out of memory !! ");
			errorflag = 1;
			return 0;
		}
		etable->next = NULL;
		/* allocate mem for equate name */
		j=stringlen(buffer)+1;
		if(!(etable->name = (char *)malloc(j)))
		{
			printf("!! OUCH, I'm out of memory !! ");
			errorflag = 1;
			return 0;
		}
		for (i=0;i<j;i++)
			etable->name[i] = buffer[i];
		/* allocate mem for replacement value */
		j=stringlen(buffer2)+1;
		if (!(etable->value = (char *)malloc(j)))
		{
			printf("!! OUCH, I'm out of memory !! ");
			errorflag = 1;
			return 0;
		}
		for (i=0;i<j;i++)
			etable->value[i] = buffer2[i];
	}
	else
	{
		tetable * next = etable;
		/* run through the list */
		while (next->next != NULL)
		{
			if (matches(next->name,buffer))
			{
				/* it's a label, can't reuse it */
				if (datalabel)
				{
					if(pass == 1)
					{
						printf("** name is currently being used ** ");
						errorflag = 1;
						return 0;
					}
					if (matches(next->value+1,buffer2+1)==0)
						dirtylabels |= 2;
				}
				/* already in list, just replace value */
				free(next->value);
				j = stringlen(buffer2)+1;
				if (!(next->value = (char *)malloc(j)))
				{
					printf("!! OUCH, I'm out of memory !! ");
					errorflag = 1;
					return 0;
				}
				for (i=0;i<j;i++)
					next->value[i] = buffer2[i];
				return 1;
			}
			next = next->next;
		}
		/* already in list, just replace value */
		if (matches(next->name,buffer))
		{
			/* it's a label, can't reuse it */
			if (datalabel)
			{
				if (pass == 1)
				{
					printf("** name is currently being used ** ");
					errorflag = 1;
					return 0;
				}
				if (!matches(next->value+1,buffer2+1))
					dirtylabels |= 2;
			}
			free(next->value);
			j = stringlen(buffer2)+1;
			if(!(next->value = (char *)malloc(j)))
			{
				printf("!! OUCH, I'm out of memory !! ");
				errorflag = 1;
				return 0;
			}
			for (i=0;i<j;i++)
				next->value[i] = buffer2[i];
			return 1;
		}
		/* now add to the list */
		if (!(next->next = (tetable *)malloc(sizeof(tetable))))
		{
			printf("!! OUCH, I'm out of memory !! ");
			errorflag = 1;
			return 0;
		}
		next = next->next;
		next->next = NULL;
		if (datalabel)            /* set the data flag */
			next->flag = 1;
		j=stringlen(buffer)+1;
		if(!(next->name = (char *)malloc(j)))
		{
			printf("!! OUCH, I'm out of memory !! ");
			errorflag = 1;
			return 0;
		}
		for (i=0;i<j;i++)
			next->name[i] = buffer[i];
		j=stringlen(buffer2)+1;
		if(!(next->value = (char *)malloc(j)))
		{
			printf("!! OUCH, I'm out of memory !! ");
			errorflag = 1;
			return 0;
		}
		for (i=0;i<j;i++)
			next->value[i] = buffer2[i];
	}
	return 1;
}
/* get immediate from label table */
unsigned long getlabel(char ** tmp)
{
	unsigned long ret = 0;
	tltable * next = ltable;
	while (next != NULL && !matches(*tmp,next->name))
		next = next->next;
	if (next)
	{
		ret = next->value;
		(*tmp)+=stringlen(next->name);
	}
	else
	{
		printf("** label not defined ** ");
		errorflag = 1;
	}
	return ret;
}
char checklabel(char ** tmp)
{
	tltable * next = ltable;
	while (next != NULL && !matches(*tmp,next->name))
		next = next->next;
	if (next)
	{
		(*tmp)+=stringlen(next->name);
		if (next->value!=offset)
		{
			next->value=offset;
			dirtylabels |= 1;
			return 1;
		}
	}
	return 0;
}
/* put a value in the label table */
void putlabel(char *label)
{
	static char buffer[256];
	int i;
	i = 0;
	while (*label != ':' && *label != 0)
	{ buffer[i]=*label;label++;i++;}
	buffer[i] = 0;
	if (*label != ':') return;
	/* no table, make new one */
	if (ltable == NULL)
	{
		if(!(ltable = (tltable *)malloc(sizeof(tltable))))
		{
			printf("!! OUCH, I'm out of memory !! ");
			errorflag = 1;
			return;
		}
		ltable->next = NULL;
		if(!(ltable->name = (char *)malloc(stringlen(buffer)+1)))
		{
			printf("!! OUCH, I'm out of memory !! ");
			errorflag = 1;
			return;
		}
		for (i=0;i<=stringlen(buffer);i++)
			ltable->name[i] = buffer[i];
		ltable->value =offset;
	}
	/* table exists, just add to it */
	else
	{
		tltable * next = ltable;
		/* run through the list */
		while (next->next != NULL)
		{
			if (matches(next->name,buffer) || matches(next->next->name,buffer))
			{
				printf("** duplicate label ** ");
				errorflag=1;
				return;
			}
			next = next->next;
		}
		/* now add to the list */
		if(!(next->next = (tltable *)malloc(sizeof(tltable))))
		{
			printf("!! OUCH, I'm out of memory !! ");
			errorflag = 1;
			return;
		}
		next = next->next;
		next->next = NULL;
		if(!(next->name = (char *)malloc(stringlen(buffer)+1)))
		{
			printf("!! OUCH, I'm out of memory !! ");
			errorflag = 1;
			return;
		}
		for (i=0;i<=stringlen(buffer);i++)
			next->name[i] = buffer[i];
		next->value =offset;
	}
}
/* get the opcode number, the registers, immediates, and mem references */
void getcodes(char ** instruct,int * opcode,unsigned long operands[3],unsigned long
			  immediates[3],char pass,FILE * fpout)
{
	int i,j,mem,regsize;
	char done;
	*opcode = opnone;
	/* run through list to see if opcode is in it */
	for (i=0;i<NUMOPCODES && *opcode==opnone;i++)
		if (matches(*instruct,str_opcodes[i].name))
		{
			(*opcode) = str_opcodes[i].value;
			(*instruct) += stringlen(str_opcodes[i].name);
		}
		if (*opcode != opnone) /* code was in the list */
		{
			/* get rid of white space */
			while(**instruct == 32 || **instruct == 9)
				(*instruct)++;
			/* clear these out */
			for (j=0;j<3;j++)
			{
				operands[j]=0;
				immediates[j]=0;
			}
			memimmediate = 0; /* set this zero too */
			PointerRef = 0;
			if (**instruct == 0 || **instruct == ';' || **instruct== 0x0d ||
				**instruct== 0x0a)
				done = 1;
			else
				done = 0;
			mem = 0;
			regsize = 0;
			for (j=0;j<3 && !done;j++)
			{
				while(**instruct == 32 || **instruct == 9)
					(*instruct)++;
				/* is it an override? */
				if (matches(*instruct,"byte") || matches(*instruct,"word") )
				{
					if (matches(*instruct,"byte"))
						regsize = 1;
					else
						regsize = 2;
					(*instruct)+=4;
					while (**instruct == 32 || **instruct == 9) (*instruct)++;
					if (matches(*instruct,"ptr"))
					{
						(*instruct)+=3;
						while (**instruct == 32 || **instruct == 9) (*instruct)++;
					}
				}
				/* is it memory? */
				if (**instruct == '[')
				{
					memreg2val = 0;
					mem = 1;
					(*instruct)++;
				}
				/* is it register? */
				for(i=0;i<NUMREGS && !operands[j];i++)
					if (matches(*instruct,registers[i].name))
					{
						operands[j] = registers[i].flag;
						(*instruct) += stringlen(registers[i].name);
					}
					/* check equates */
					if ((**instruct >= 65 && **instruct <= 90)  ||
						(**instruct >= 97 && **instruct <= 122) &&
						operands[j]==0)
					{
						if (resolveequate(*instruct,pass) == 1)
						{
							j--;
							continue;
						}
						else
						{
							return;
						}
					}
					if (**instruct == ':')
					{
						if ((operands[j] & 0x00000007) != seg16)
						{
							printf("** invalide segment reference ** ");
							errorflag=1;
							return;
						}
						if ((operands[j] & 0x0000fff0) != regds)
						{
							offset++;
							if (pass==2)
							{
								char out = 0x26 | (registers[i-1].value << 3);
								writebuffer(&out,1,1,fpout);
							}
						}
						(*instruct)++;
						operands[j] = 0;
						j--;
						continue;
					}
					/* adjust for [reg] reference */
					if (mem && ((operands[j] & 0x00000007) == reg16))
					{
						/* is it register? */
						(*instruct) ++;
						for(i=0;i<NUMREGS;i++)
							if (matches(*instruct,registers[i].name))
							{
								memreg2val = registers[i].value;
								(*instruct) += (stringlen(registers[i].name)+1);
							}
							(*instruct) --;
							if ((**instruct >= 65 && **instruct <= 90)  ||
								(**instruct >= 97 && **instruct <= 122) &&
								memreg2val==0)
							{
								if (resolveequate(*instruct,pass)==1)
								{
									for(i=0;i<NUMREGS;i++)
										if (matches(*instruct,registers[i].name))
										{
											memreg2val = registers[i].value;
											(*instruct) += (stringlen(registers[i].name)+1);
										}
								}
							}
							while ( (**instruct) == 32 || (**instruct) == 9)
								(*instruct)++;
							/* allow for a little arithmetic */
							while ( (**instruct) == '+' || (**instruct) == '-' ||
								(**instruct) == '*' || (**instruct) == '/')
							{
								char operation;
								operation = **instruct;
								(*instruct)++;
								while ( (**instruct) == 32 || (**instruct) == 9)
									(*instruct)++;
								/* check on equate */
								if ((**instruct >= 65 && **instruct <= 90)  ||
									(**instruct >= 97 && **instruct <= 122))
									if (!resolveequate(*instruct,pass))
									{
										printf("** Unresolved Equate ** ");
										errorflag = 1;
										return;
									}
									while ( (**instruct) == 32 || (**instruct) == 9)
										(*instruct)++;
									/* now determine which instruction to use */
									switch (operation)
									{
									case '+' : immediates[j] += getval(instruct,pass);
										break;
									case '-' : immediates[j] -= getval(instruct,pass);
										break;
									case '*' : immediates[j] *= getval(instruct,pass);
										break;
									case '/' : immediates[j] /= getval(instruct,pass);
										break;
									}
									if (errorflag ) return;
									while ( (**instruct) == 32 || (**instruct) == 9)
										(*instruct)++;
							}
					}
					else
						if (operands[j] == opnone)
						{
							/* is it instruction? */
							if ((**instruct >= 65 && **instruct <= 90) ||
								(**instruct >= 97 && **instruct <= 122))
								done = 1;
							/* is it immediate? */
							else
							{
								immediates[j] = getval(instruct,pass);
								if (errorflag ) return;
								while ( (**instruct) == 32 || (**instruct) == 9)
									(*instruct)++;
								/* allow for a little arithmetic */
								while ( (**instruct) == '+' || (**instruct) == '-' ||
									(**instruct) == '*' || (**instruct) == '/')
								{
									char operation;
									operation = **instruct;
									(*instruct)++;
									while ( (**instruct) == 32 || (**instruct) == 9)
										(*instruct)++;
									/* resolve equate */
									while ((**instruct >= 65 && **instruct <= 90)  ||
										(**instruct >= 97 && **instruct <= 122))
										if(!resolveequate(*instruct,pass))
										{
											printf("** Unresolved Equate ** ");
											errorflag = 1;
											return;
										}
										while ( (**instruct) == 32 || (**instruct) == 9)
											(*instruct)++;
										/* now determine which instruction to use */
										switch (operation)
										{
										case '+' : immediates[j] += getval(instruct,pass);
											break;
										case '-' : immediates[j] -= getval(instruct,pass);
											break;
										case '*' : immediates[j] *= getval(instruct,pass);
											break;
										case '/' : immediates[j] /= getval(instruct,pass);
											break;
										}
										if (errorflag ) return;
										if (errorflag ) return;
										while ( (**instruct) == 32 || (**instruct) == 9)
											(*instruct)++;
								}
								if (PointerRef)
									operands[j] = pointer;
								else
									if (((j!=0) && ((operands[0] & 0x0000000f) == reg8) && (!mem))
										|| ((regsize == 1)&& (j != 0)))
										operands[j] = immed8;
									else
										if (mem)
											operands[j] = reg16;
										else
											operands[j] = immed16;
								if ((j!=0) && (operands[0] & memref) && (!regsize))
								{
									printf("** Memory Reference needs override ** ");
									errorflag = 1;
									return;
								}
							}
						}
						/* finish off the memory reference */
						if (mem)
						{
							if (**instruct != ']')
							{
								fprintf(stderr,"** No closing bracket in memory reference ** ");
								errorflag = 1;
								return;
							}
							else
							{
								memimmediate = immediates[j];
								operands[j] |= memref;
								(*instruct)++;
							}
						}
						/* get the next param */
						while (**instruct != ',' && **instruct != ';' && **instruct != 0
							&& !done)
							(*instruct)++;
						if (**instruct == ';' || **instruct == 0 || done)
							done = 1;
						else
						{
							/* get rid of white space */
							(*instruct)++;
							while(**instruct == 32 || **instruct == 9)
								(*instruct)++;
						}
						mem = 0;
			}
		}
		else
			/* ran across an invalid name, see if equate */
		{
			if (!addequate(instruct,pass))
			{
#ifdef DEBUG
				printf ("%s ** invalid instruction ** ",*instruct);
#else
				printf ("** invalid instruction ** ");
#endif
				errorflag = 1;
				return;
			}
		}
		if ( ( ((*opcode) == opshl) || ((*opcode) == opshr)
			|| ((*opcode) == oprol) || ((*opcode) == opror)
			|| ((*opcode) == opsal) || ((*opcode) == opsar)  )
			&& immediates[1] ==  1 )
		{
			operands[1] = opnone;
			immediates[1] = 0;
		}
}
/* take the parsed values and write them to the file */
void assemble(int opcode,unsigned long operands[3],unsigned long immediates[3],
			  int pass,FILE * fp)
{
	int i;
	char done = 0;
	unsigned short instruct;
	for (i = 0;i<NUMINSTRUCTIONS && !done;i++)
		if (opcode == opcodes[i].ocode)
			if ((((operands[0] & 0x0000000f) == (opcodes[i].val1))||
				((operands[0] & 0x0000fff8) == (opcodes[i].val1)))&&
				(((operands[1] & 0x0000000f) == (opcodes[i].val2))||
				((operands[1] & 0x0000fff8) == (opcodes[i].val2)))
#ifdef CODE32BIT
				||(((operands[2] & 0x0000000f) == (opcodes[i].val3))||
				((operands[2] & 0x0000fff8) == (opcodes[i].val3)))
#endif
				)
			{
				unsigned char opcode1regval,opcode2regval,opcode3regval;
				done = 1;
				/* == write the opcodes == */
				instruct = opcodes[i].instruct;
				offset += opcodes[i].length;
				if (pass == 2)
					writebuffer(opcodes[i].code,1,opcodes[i].length,fp);
				/* == write the operands == */
				/* find the registers' values and calculate code values */
				if (instruct & Icalreg)
				{
					unsigned char regvalue;
					if (instruct & Ifirstval &&
						instruct & Isecondval)
					{
						if (instruct & Iswapval)
							regvalue = calculateregvalue(operands[1],operands[0],
							&instruct);
						else
							regvalue = calculateregvalue(operands[0],operands[1],
							&instruct);
						offset += 1;
						if (pass == 2)
							writebuffer((char *)®value,1,1,fp) ;
					}
					else
						if (instruct & Ifirstval)
						{
							if (instruct & Ical1)
								regvalue = cal1reg(operands[0]) |
								opcodes[i].code[opcodes[i].length];
							else
								if (instruct & Ical2)
									regvalue = cal2reg(operands[0]) |
									opcodes[i].code[opcodes[i].length];
								else
									regvalue = calculateregvalue(operands[0],0,&instruct)|
									opcodes[i].code[opcodes[i].length];
							offset += 1;
							if (pass == 2)
								writebuffer((char *)®value,1,1,fp) ;
						}
						else
							if (instruct & Isecondval)
							{
								regvalue = calculateregvalue(0,operands[1],&instruct);
								offset += 1;
								if (pass == 2)
									writebuffer((char *)®value,1,1,fp) ;
							}
				}
				/* assemble in the immediates */
				if (instruct & Iimmedfirst)
				{
					if ((opcodes[i].val1 & 0x00000007) == immed8 )
					{
						offset += 1;
						if (pass == 2)
							writebuffer((char *)&immediates[0],1,1,fp) ;
					}
					else
						if ((opcodes[i].val1 & 0x00000007) == pointer )
						{
							offset += 4;
							if (pass == 2)
								writebuffer((char *)&immediates[0],4,1,fp) ;
						}
						else
						{
							offset += 2;
							if (instruct & Iwrite8)
								offset--;
							if (instruct & Iwrite8 && pass == 2 && instruct & Ijump)
								offset = offset;
							if (instruct & Ijump)
								immediates[0] -= offset;
							if (pass == 2)
							{
								if (instruct & Iwrite8)
								{
									if(instruct & Ijump &&
										(immediates[0] & 0xffffff80))
										if((~immediates[0]) & 0xffffff80)
										{
											if ((long int)immediates[0] < -128)
												immediates[0] = -1-immediates[0];
											printf("** relative jump off by %d bytes ** ",
												immediates[0] - 127);
											errorflag = 1;
										}
										writebuffer(&immediates[0],1,1,fp) ;
								}
								else
									writebuffer(&immediates[0],2,1,fp) ;
							}
						}
				}
				if (instruct & Iimmedsecond)
				{
					if ((opcodes[i].val2 & 0x00000007) == immed8 )
					{
						offset += 1;
						if (pass == 2)
							writebuffer(&immediates[1],1,1,fp) ;
					}
					else
						if ((opcodes[i].val2 & 0x00000007) == pointer )
						{
							offset += 4;
							if (pass == 2)
								writebuffer((char *)&immediates[1],4,1,fp) ;
						}
						else
						{
							offset += 2;
							if (instruct & Iwrite8)
								offset--;
							/* now write the data to file */
							if (pass == 2)
							{
								if (instruct & Iwrite8)
									writebuffer(&immediates[1],1,1,fp) ;
								else
									writebuffer(&immediates[1],2,1,fp) ;
							}
						}
				}
			}
			if (!done)
			{
				printf("** invalid opcode/operand combination ** ");
				errorflag = 1;
			}
}
/* TEST */
char * strcat(char *dest, const char *src)
{
    size_t i,j;
    for (i = 0; dest[i] != '\0'; i++)
        ;
    for (j = 0; src[j] != '\0'; j++)
        dest[i+j] = src[j];
    dest[i+j] = '\0';
    return dest;
}
/* parse out the file one line at a time */
void parse(char * infile,FILE * fpout,int pass)
{
#ifndef DEBUG
	static char readbuffer[READBUFFERSIZE];
#endif
	FILE * fpin;
	char * parser;
	int opcode;
	unsigned long operands[3];
	unsigned long immediates[3];
	int linenum = 0;
	fpin=fopen(infile,"rt");
	if (fpin == NULL)
	{
		printf("Unable to open file %s.\n",infile);
		exit(1);
	}
	while (!feof(fpin))
	{
		fgets(readbuffer,256,fpin);
		if (feof(fpin) && *(readbuffer+stringlen(readbuffer)-1) == 10)
			continue;
		linenum ++;
		parser = readbuffer;
		/* keep going if not at the end of the line or at a comment */
		while (*parser != 0 && *parser != ';' && *parser != 0x0d
			&& *parser != 0x0a)
		{
			opcode = opnone;
			/* skip past the white space */
			while (*parser == 32 || *parser == 9) parser++;
			/* check to see if at the end of the line or at a comment */
			if (*parser != 0 && *parser != ';' && *parser != 0x0d
				&& *parser != 0x0a)
			{
				/* now check to see if it's an opcode, a label or data */
				/* check to see if preproc */
				if (*parser == '.')
				{
					char fname[80];
					int i;
					parser++;
					if (matches(parser,"include"))
					{ /* have included another file */
						while (*parser != 32 && *parser != 9) parser++;
						while (*parser == 32 || *parser == 9) parser++;
						if (*parser != '"')
						{
							printf("** include definition with no open quotes ** ");
							printf("line %d, file %s\n",linenum,infile);
							printf(readbuffer);
							exit(1);
						}
						parser++; /* get rid of `"' */
						i = 0;
						while (*parser != '"')
						{
							if (*parser == 32 || *parser == 0 || *parser == 0x0a)
							{
								printf("** include definition with no closing quotes ** ");
								printf("line %d, file %s\n",linenum,infile);
								printf(readbuffer);
								exit(1);
							}
							if (i>=79)
							{
								printf("** filename to long ** ");
								printf("line %d, file %s\n",linenum,infile);
								printf(readbuffer);
								exit(1);
							}
							fname[i]=*parser;
							parser++;
							i++;
						}
						fname[i]=0;
						parse(fname,fpout,pass);
						/* force to continue to next line */
						opcode = 0;
						*parser=0;
						continue;
					}
					else
						/* origin has changed */
						if (matches(parser,"org"))
						{
							while (*parser != 32 && *parser != 9) parser++;
							while (*parser == 32 || *parser == 9) parser++;
							filesize = (offset-org)+filesize;
							offset = getval(&parser,2);
							org = offset;
						}
						if (matches(parser,"echo"))
						{
							while (*parser != 32 && *parser != 9) parser++;
							parser++;
							if (pass==2)
								printf("%s",parser);
							*parser=0;
							continue;
						}
				}
				else
					/* now continue on with the rest of the program */
					if (*parser == '@') /* it's a label */
					{
						if (pass == 1)
							putlabel(parser);
						else
							if (checklabel(&parser))
								printf("** label redundency check failed ** line %d, file %s\n",
								linenum,infile);
						if (errorflag)
						{ /* an error occured */
							printf("line %d, file %s\n",linenum,infile);
							printf(readbuffer);
							exit(1);
						}
						/* now skip past the label */
						while(*parser != ':' && *parser != ';' && *parser !=0)
							parser++;
						if (*parser != ':')
						{
							printf("** Incomplete label ** line %d, file %s\n",
								linenum,infile);
							printf(readbuffer);
							exit(1);
						}
						parser++;
					}
					else
						/* check to see if data */
						if (lower(*parser) == 'd' && (lower(*(parser+1)) == 'w'
							|| lower(*(parser+1)) == 'b') )
						{
							parser++;
							if (*parser == 'w') /* data word */
							{
								unsigned int retval;
								parser++;
								while(*parser==32 || *parser==9) parser++;
								offset += 2;
								/* check to see if an equate */
								while ((*parser >= 65 && *parser <= 90)  ||
									(*parser >= 97 && *parser <= 122))
									if (!resolveequate(parser,pass))
									{
										printf("** Unresolved Equate ** ");
										printf("line %d, file %s\n",linenum,infile);
										printf(readbuffer);
										exit(1);
									}
									if (pass==2)
									{
										retval = getval(&parser,pass);
										writebuffer(&retval,2,1,fpout);
									}
									else
										while (*parser != 0 && *parser != 0x0a
											&& *parser != 9 && *parser !=';'
											&& *parser != 32 && *parser != ',')
											(parser)++;
									while(*parser==',')
									{
										parser++;
										while(*parser==32 || *parser==9) parser++;
										offset += 2;
										while ((*parser >= 65 && *parser <= 90)  ||
											(*parser >= 97 && *parser <= 122))
											if(!resolveequate(parser,pass))
											{
												printf("** Unresolved Equate ** ");
												printf("line %d, file %s\n",linenum,infile);
												printf(readbuffer);
												exit(1);
											}
											if (pass==2)
											{
												retval = getval(&parser,pass);
												writebuffer(&retval,2,1,fpout);
											}
											else
												while (*parser != 0 && *parser != 0x0a
													&& *parser != 9 && *parser !=';'
													&& *parser != 32 && *parser != ',')
													(parser)++;
									}
							}
							else /* else data byte */
							{
								unsigned long retval;
								parser++;
								while(*parser==32 || *parser==9) parser++;
								/* write a string to the file */
								if (*parser == '"')
								{
									parser++;
									while (*parser != '"' && *parser != 0)
									{
										offset++;
										if (pass==2)
											writebuffer(parser,1,1,fpout);
										parser++;
									}
									if (*parser == '"') parser++;
								}
								/* else just write out a single byte */
								else
								{
									while (((*parser >= 65 && *parser <= 90)  ||
										(*parser >= 97 && *parser <= 122))
										&& !matches(parser,"dup"))
										if (!resolveequate(parser,pass))
										{
											printf("** Unresolved Equate ** ");
											printf("line %d, file %s\n",linenum,infile);
											printf(readbuffer);
											exit(1);
										}
										retval = getval(&parser,pass);
										/* get ready for dup command */
										while (*parser == 32 || *parser == 9)
											parser++;
										if (matches(parser,"dup"))
										{
											int i,count;
											count = retval;
											while (*parser != 32 && *parser != 9
												&& *parser != '(')
												parser++;
											while (*parser == 32 || *parser == 9)
												parser++;
											if (*parser=='(')
												parser++;
											while ((*parser >= 65 && *parser <= 90)  ||
												(*parser >= 97 && *parser <= 122))
												if (!resolveequate(parser,pass))
												{
													printf("** Unresolved Equate ** ");
													printf("line %d, file %s\n",linenum,infile);
													printf(readbuffer);
													exit(1);
												}
												retval = getval(&parser,pass);
												for (i=0;i<count;i++)
												{
													offset++;
													if (pass==2)
														writebuffer(&retval,1,1,fpout);
												}
												while (*parser != ')' && *parser != ';'
													&& *parser != 0x00)
													parser++;
												if (*parser == ')') parser++;
										}
										else
										{
											offset += 1;
											if (pass==2)
												writebuffer(&retval,1,1,fpout);
										}
								}
								/* now continue as long as there are ,'s */
								while(*parser==',')
								{
									parser++;
									while(*parser==32 || *parser==9) parser++;
									if (*parser == '"')
									{
										parser++;
										while (*parser != '"' && *parser != 0)
										{
											offset++;
											if (pass==2)
												writebuffer(parser,1,1,fpout);
											parser++;
										}
										if (*parser == '"') parser++;
									}
									else
									{
										while((*parser >= 65 && *parser <= 90)  ||
											(*parser >= 97 && *parser <= 122))
											if (!resolveequate(parser,pass))
											{
												printf("** Unresolved Equate ** ");
												printf("line %d, file %s\n",linenum,infile);
												printf(readbuffer);
												exit(1);
											}
											retval = getval(&parser,pass);
											offset += 1;
											if (pass==2)
												writebuffer(&retval,1,1,fpout);
									}
								}
							}
						}
						else
							getcodes(&parser,&opcode,operands,immediates,pass,fpout);
				if (errorflag)
				{
					printf("line %d, file %s\n",linenum,infile);
					printf(readbuffer);
					exit(1);
				}
				if (opcode)
					assemble(opcode,operands,immediates,pass,fpout);
				else
					if(*parser >= 'A' && *parser <= 'Z' &&
						*parser >= 'a' && *parser <= 'z')
						addequate(&parser,pass);
				if (errorflag)
				{
					printf("line %d, file %s\n",linenum,infile);
					printf(readbuffer);
					exit(1);
				}
			}
		}
	}
	fclose(fpin);
}
char outfile[13],ext[]=".com";
int main(int argc,char * argv[])
{
	int i;
	FILE * fpout;
	char * infile ,* outfilestart;
	if (argc<=1)
	{
		printf("Usage mode: asmBuilder <fName>\n");
		return -1;
	}
	infile = argv[1];
	outfilestart = infile;
	while (*infile)
	{
		if (*infile == '\\') outfilestart = infile+1;
		infile++;
	}
	infile = outfile;
	while (*outfilestart != '.' && *outfilestart)
	{
		*infile = *outfilestart;
		infile++;
		outfilestart++;
	}
	outfilestart = ext;
	while (*outfilestart)
	{
		*infile = *outfilestart;
		infile++;
		outfilestart++;
	}
	*infile = 0;
	// star all the process of compilation
	fpout=fopen(outfile,"wb");
	for (i=1;i<3;i++)
	{
		offset = org;
		filesize = 0;
		parse(argv[1],fpout,i);
		fwrite(wbuffer,buffpos,1,fpout) ;
		buffpos = 0;
		if (i==1) putlabel("@@FinalOffset:");
	}
	fclose(fpout);
	if (dirtylabels)
	{
		if (dirtylabels & 1)
			printf("\n Rescanning due to dirty labels!\n");
		dirtylabels = 0;
		fpout=fopen(outfile,"wb");
		offset = org;
		filesize = 0;
		parse(argv[1],fpout,2);
		fwrite(wbuffer,buffpos,1,fpout) ;
		if (dirtylabels & 1)
			printf(" Rescan Failed!\n");
		else
			printf(" Rescan Successful!\n");
	}
	printf("\n  Done. %lu bytes written to %s.\n",(offset-org)+filesize,outfile);
	return 0;
}
```
## 头文件defineList.h
```cpp
#define none	0x00000000
#define reg8	0x00000001
#define reg16	0x00000002
#define seg16	0x00000004
#define reg32	0x00000005
#define immed8	0x00000006
#define immed16	0x00000007
#define pointer 0x00000005 // maybe 0x00000008
#define memref	0x00000008
#define regal	0x00000010
#define regcl	0x00000020
#define regdl	0x00000030
#define regbl	0x00000040
#define regah	0x00000050
#define regch	0x00000060
#define regdh	0x00000070
#define regbh	0x00000080
#define regax	0x00000090
#define regcx	0x000000a0
#define regdx	0x000000b0
#define regbx	0x000000c0
#define regsp	0x000000d0
#define regbp	0x000000e0
#define regsi	0x000000f0
#define regdi	0x00000100
#define reges	0x00000110
#define regds	0x00000120
#define regss	0x00000130
#define regcs	0x00000140
#define regfs	0x00000150
#define reggs	0x00000160
#define regeax	0x00000170
#define regecx	0x00000180
#define regedx	0x00000190
#define regebx	0x00000200
#define regesp	0x00000210
#define regebp	0x00000220
#define regesi	0x00000230
#define regedi	0x00000240
// instructions
#define Inothing		0x0000
#define Ifirstval		0x0002
#define Isecondval		0x0004
#define Iswapval		0x0008
#define Iimmedfirst		0x0010
#define Iimmedsecond	0x0020
#define Icalreg			0x0040
#define Ijump			0x0080
#define Iwrite8			0x0100
#define Ical1			0x0200
#define Ical2			0x0400
```
