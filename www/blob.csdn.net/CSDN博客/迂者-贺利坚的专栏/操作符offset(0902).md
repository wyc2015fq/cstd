
# 操作符offset(0902) - 迂者-贺利坚的专栏 - CSDN博客

2017年03月31日 20:38:14[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：562标签：[汇编语言																](https://so.csdn.net/so/search/s.do?q=汇编语言&t=blog)个人分类：[汇编语言																](https://blog.csdn.net/sxhelijian/article/category/6698546)



本文为《汇编语言程序设计》0902小节例程。点击[链接…](http://blog.csdn.net/sxhelijian/article/details/56671827)进课程主页。
用操作符offset取得标号的偏移地址
```python
assume cs:codeseg
codeseg segment
start: mov ax,
```
```python
offset
```
```python
start ; 相当于 mov ax,
```
```python
0
```
```python
s: mov ax,
```
```python
offset
```
```python
s       ; 相当于mov ax,
```
```python
3
```
```python
codeseg
```
```python
ends
```
```python
end
```
```python
start
```
下面的程序，在运行中将s处的一条指令复制到s0处（有点黑客的味道）。
```python
assume cs:codesg
 codesg segment
   s:
```
```python
mov
```
```python
ax,bx
```
```python
mov
```
```python
si,offset s
```
```python
mov
```
```python
di,offset s0
```
```python
mov
```
```python
ax,cs:[si]
```
```python
mov
```
```python
cs:[di],ax
 s0:
```
```python
nop
```
```python
nop
```
```python
codesg ends
ends
```

