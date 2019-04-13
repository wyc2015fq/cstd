
# call指令和ret指令(1001) - 迂者-贺利坚的专栏 - CSDN博客

2017年03月31日 20:51:04[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：935


本文为《汇编语言程序设计》1001小节例程。点击[链接…](http://blog.csdn.net/sxhelijian/article/details/56671827)进课程主页。
用ret返回
```python
assume cs:codesg, ss:stack
stack segment
       db
```
```python
16
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
codesg segment
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
0
```
```python
push
```
```python
ax
```
```python
mov
```
```python
bx,
```
```python
0
```
```python
ret
```
```python
codesg ends
end start
```
用retf返回
```python
assume cs:codesg, ss:stack
stack segment
       db
```
```python
16
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
codesg segment
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
0
```
```python
push
```
```python
cs
```
```python
push
```
```python
ax
```
```python
mov
```
```python
bx,
```
```python
0
```
```python
retf
codesg ends
end start
```

