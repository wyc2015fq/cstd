
# 汇编程序：查表法找ASCII - 迂者-贺利坚的专栏 - CSDN博客

2017年05月13日 09:14:42[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：1830


【任务】用查表法输出16进制数的各个符号
（本任务旨在演示lodsb、stosb串传送指令，以及xlat查表指令）
【参考解答】
```python
assume cs:coseg,ds:
```
```python
table
```
```python
,ss:stack,es:
```
```python
table
```
```python
table
```
```python
segment
tab_da    db
```
```python
30
```
```python
h,
```
```python
31
```
```python
h,
```
```python
32
```
```python
h,
```
```python
33
```
```python
h,
```
```python
34
```
```python
h,
```
```python
35
```
```python
h,
```
```python
36
```
```python
h,
```
```python
37
```
```python
h,
```
```python
38
```
```python
h,
```
```python
39
```
```python
h
          db
```
```python
41
```
```python
h,
```
```python
42
```
```python
h,
```
```python
43
```
```python
h,
```
```python
44
```
```python
h,
```
```python
45
```
```python
h,
```
```python
46
```
```python
h
hextab    db
```
```python
0
```
```python
,
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
,
```
```python
6
```
```python
,
```
```python
7
```
```python
,
```
```python
8
```
```python
,
```
```python
9
```
```python
,
```
```python
0
```
```python
ah,
```
```python
0
```
```python
bh,
```
```python
0
```
```python
ch,
```
```python
0
```
```python
dh,
```
```python
0
```
```python
eh,
```
```python
0
```
```python
fh
asibuf    db
```
```python
16
```
```python
dup(?)
eol       db
```
```python
'$'
```
```python
table
```
```python
ends
stack segment para stack
```
```python
'stack'
```
```python
stapn db
```
```python
100
```
```python
dup(?)
stack ends
coseg segment
start:
        mov ax,
```
```python
table
```
```python
mov ds,ax
        mov es,ax
        mov si,offset hextab ;根据si
        mov bx,offset tab_da ;表的基址必须放在bx中（必须的）
        mov di,offset asibuf ;结果放进di
        mov cx,
```
```python
16
```
```python
;循环计数器
```
```python
next
```
```python
:
        lodsb       ;从[si] 装入一个字节
```
```python
--> al
```
```python
xlat tab_da ;xlat以al为索引，查tab_da表，结果还是放在al中
        stosb       ;再将al存入[di]
        loop
```
```python
next
```
```python
;cx
```
```python
--,if cx<> 进入下一循环
```
```python
mov cx,
```
```python
16
```
```python
mov ah,
```
```python
2
```
```python
;显示字符的功能号
        xor si,si
again:
        mov dl,asibuf[si]
        int
```
```python
21
```
```python
h
        inc si
        loop again
    mov ax,
```
```python
4
```
```python
c00h
    int
```
```python
21
```
```python
h
coseg ends
```
```python
end
```
```python
start
```

