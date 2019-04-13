
# call 和 ret 的配合使用(1002) - 迂者-贺利坚的专栏 - CSDN博客

2017年03月31日 20:54:05[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：729


本文为《汇编语言程序设计》1002小节例程。点击[链接…](http://blog.csdn.net/sxhelijian/article/details/56671827)进课程主页。
功能：计算2的N次方，计算前，N的值由CX提供。
```python
;(下面的程序逻辑上正确，但存在严重错误！)
```
```python
assume cs:code
code segment
```
```python
start:
```
```python
mov
```
```python
ax,
```
```python
1
```
```python
mov
```
```python
cx,
```
```python
3
```
```python
call
```
```python
s
```
```python
mov
```
```python
bx,ax
```
```python
mov
```
```python
ax,
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
    s:
```
```python
add
```
```python
ax,ax
       loop s
```
```python
ret
```
```python
code ends
end start
```
下面是正确的程序
```python
; 要设置栈段，以便于call和ret指令使用
```
```python
assume cs:code, ss:stack
stack segment
       db
```
```python
8
```
```python
dup (
```
```python
0
```
```python
)
       db
```
```python
8
```
```python
dup (
```
```python
0
```
```python
)
stack ends
code segment
```
```python
start:
```
```python
mov
```
```python
ax,stack
```
```python
mov
```
```python
ss,ax
```
```python
mov
```
```python
sp,
```
```python
16
```
```python
mov
```
```python
ax,
```
```python
1000
```
```python
call
```
```python
s
```
```python
mov
```
```python
ax,
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
    s:
```
```python
add
```
```python
ax,ax
```
```python
ret
```
```python
code ends
end start
```

