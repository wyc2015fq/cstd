# Python入门--15--文件读取、保存 - 默槑 - 博客园







# [Python入门--15--文件读取、保存](https://www.cnblogs.com/modaidai/p/7290340.html)





先看文件读取，open

1、文件打开模式：

打开模式　　执行操作

　'r' 　　　　以只读方式打开文件(默认)

   'w'　　　    以写入的方式打开文件,会覆盖已存在的文件

　'x' 　　　　如果文件已经存在,使用此模式打开将引发异常

　'b'　　　　 以二进制模式打开文件

　't'　　　　  以文本模式打开(默认) '+' 可读写模式(可添加到其他模式中使用)

   'U'　　　    通用换行符支持



2、文件对象方法：

文件对象方法　　　 　　　　 执行操作

f.close()　　　　　　　　　　关闭文件

f.read([size=-1]) 　　　　　　从文件读取size个字符,当未给定size或给定负值的时候,读取剩余的所有字符,然后作为字符串返回

f.readline([size=-1]) 　　 　　从文件中读取并返回一行((包括行结束符),如果有size有定义则返回size个字符

f.write(str) 　　　　　　  　　将字符串str写入

f.writelines(seq)　　　　　　 向文件写入字符串序列seq,seq应该是一个返回字符串的可迭代对象

f.seek(offset, from) 　　  　　在文件中移动文件指针,从from(0代表文件起始位置,1代表当前位置,2代表文件末尾)偏移offset个字节

f.tell() 　　　　　　　　 　　 返回当前在文件中的位置

f.truncate([size=file.tell()]) 　　截取文件到size个字节,默认是截取到文件指针当前



实例：

f=open(r'D:\all_acc1.txt')

f.read(5)    #只读取前五个字符

print(f.tell())         #输出文件现在读取到的位置

f.seek(40,0)         #从零开始，指针向后移动40个字符，然后输出这时指针对应的位置

f.readline()　　    #从刚才的40的位置，打印当前这一行

list(f)   　　　　　#可以变为列表，一行是一个元素

for i in f:

　　print(i)　　　　#这样可以直接把每一行的数据输出到屏幕



这里插一句代码：以后拆分可以这样写,：

if each_line[:6] != '======':　　　　　　　　　　#如果each_line的前六个字符是'======'

　　(role,line_spiken) = each_line.split(':',1)          #那就以冒号为分割，拆分为两段，[0]赋值为role，[1]赋值为line_spliken



文件保存：

可以使用f=open('xx.txt','a')来打开文件，一会准备追加进一些数据

f.write('我爱你妹')　　　　　　#这就追加进去了












