
# [bx+idata]方式寻址(0702) - 迂者-贺利坚的专栏 - CSDN博客

2017年03月28日 04:44:55[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：618标签：[汇编语言																](https://so.csdn.net/so/search/s.do?q=汇编语言&t=blog)个人分类：[汇编语言																](https://blog.csdn.net/sxhelijian/article/category/6698546)



本文为《汇编语言程序设计》0702小节例程。点击[链接…](http://blog.csdn.net/sxhelijian/article/details/56671827)进课程主页。
例：用[bx+idata]的方式进行数组的处理
问题：在codesg中填写代码，将datasg中定义的
- 第一个字符串，转化为大写
- 第二个字符串转化为小写。
```python
assume cs:codesg,ds:datasg
datasg segment
      db
```
```python
'BaSiC'
```
```python
db
```
```python
'MinIX'
```
```python
datasg
```
```python
ends
```
```python
codesg segment
start: mov ax,datasg
      mov ds,ax
      mov bx,
```
```python
0
```
```python
mov cx,
```
```python
5
```
```python
s:    mov al,[bx]
```
```python
and
```
```python
al,
```
```python
11011111
```
```python
b
      mov [bx],al
      mov al,[
```
```python
5
```
```python
+bx]
```
```python
or
```
```python
al,
```
```python
00100000
```
```python
b
      mov [
```
```python
5
```
```python
+bx],al
      inc bx
      loop s
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
codesg
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

