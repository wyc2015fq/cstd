
# no stack segment警告为何还在？ - 迂者-贺利坚的专栏 - CSDN博客

2017年04月05日 21:02:43[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：7718个人分类：[汇编语言																](https://blog.csdn.net/sxhelijian/article/category/6698546)



在8086汇编中，在masm5.0下link的时候出现warning L4021: no stack segment是习以为常的现象。
如，针对下面的程序：
```python
assume cs:codesg,ds:datasg
datasg segment
    dw
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
datasg ends
codesg segment
```
```python
start:
```
```python
mov
```
```python
ax, datasg
```
```python
mov
```
```python
ds, ax
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
codesg ends
end start
```
编译和连接的结果是：
![这里写图片描述](https://img-blog.csdn.net/20170405200145103?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170405200145103?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
对此的解释是：程序中没有安排堆栈段。这个警告可以忽略，程序能够运行，并且得到正确的结果。
好吧！那在在程序中加上栈段，警告就该没有了吧？
写下面的程序：
```python
assume cs:codesg,ds:datasg, ss:stacksg
datasg segment
    dw
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
datasg ends
stacksg segment
      dw
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
stacksg ends
codesg segment
```
```python
start:
```
```python
mov
```
```python
ax, stacksg
```
```python
mov
```
```python
ss, ax
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
ax, datasg
```
```python
mov
```
```python
ds, ax
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
codesg ends
end start
```
程序中，定义了“`stacksg segment`”，也`assume`了“`ss:stacksg`”。然而，编译并连接的结果：
![这里写图片描述](https://img-blog.csdn.net/20170405201052895?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170405201052895?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
学生问我这个事情时，我意识到自己好像也观察到了，却没有顾上理会。
我将程序用debug装入内存，发现SS的值不对啊！
![这里写图片描述](https://img-blog.csdn.net/20170405201708966?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170405201708966?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从图中可以看出，装载进内存的程序起始物理地址在075A0H（DS=075AH），跨过100H的程序段前缀PSP，datasg段该开始于076A0。这一段尽管只有10个字节，但由于下面的stacksg的段起始地址应该是16（10H）的倍数，stacksg段的起始地址该是076B0H才对。然则，SS的值是0769H纯粹没有道理。
只能说明一个事实，人家没有把村长当干部！在连接过程中，并未因为有“`stacksg segment`”，和`assume`了“`ss:stacksg`”就认为设置了堆栈段。
怎样做才能被承认呢？
找度娘，说是将段定义写成下面的形式：
```python
stacksg segment stack
      ……
stacksg
```
```python
ends
```
按指点修改程序，写为：
```python
assume cs:codesg,ds:datasg, ss:stacksg
datasg segment
    dw
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
datasg ends
stacksg segment stack
      dw
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
stacksg ends
codesg segment
```
```python
start:
```
```python
mov
```
```python
ax, stacksg
```
```python
mov
```
```python
ss, ax
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
ax, datasg
```
```python
mov
```
```python
ds, ax
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
codesg ends
end start
```
编译和连接结果：
![这里写图片描述](https://img-blog.csdn.net/20170405202002613?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170405202002613?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
完美消除warning！
再debug连接好的可执行文件：
![这里写图片描述](https://img-blog.csdn.net/20170405202218270?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170405202218270?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
SS=076BH是对的！居然还有SP=0010H！这是在还没有执行程序中任何指令的情况下发生的事情！是连接程序自动地发现定义的堆栈段大小就是10H！
那，这是为什么呢？
查阅汇编程序中段的定义语法，完整的是：
```python
segname SEGMENT [
```
```python
align_type
```
```python
][
```
```python
combine_type
```
```python
][
```
```python
user_type
```
```python
][
```
```python
'class'
```
```python
]
...
segname ENDS
```
其中的组合类型(combine_type)可以是：
PUBLIC：该段连接时将与有相同名字的其他分段连接在一起，其连接次序由连接命令指定。
COMMON：该段在连接时与其他同名分段有相同的起始地址，所以会产生覆盖。
AT expression： 使段的起始地址是表达式所计算出来的16位段地址，但它不能用来指定代码段。
STACK：指定该段在运行时为堆栈段的一部分。
这些选项的含义不解释了。要解释通可能还需要再补充不少其他知识。我们先明白，stacksg segment stack中最后的stack让连接程序将定义的堆栈段当堆栈段用了。
最后多说一句是，要真正说清楚这件事，得谈多个.obj是如何link为一个.exe的。眼下先学现在要紧的。


[
  ](https://img-blog.csdn.net/20170405202218270?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)