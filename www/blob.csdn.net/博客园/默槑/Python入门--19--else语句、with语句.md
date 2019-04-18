# Python入门--19--else语句、with语句 - 默槑 - 博客园







# [Python入门--19--else语句、with语句](https://www.cnblogs.com/modaidai/p/7290843.html)





1、else与while连用：

x=input('请输出一个整数：')

while x>0

　　x=x-2

　　print(x)

else:

　　print('x已经小于等于零了！')



2、else与try连用：



x=input('请随便输入：')

try：

　　print(int(x))

except ValueError as reason:

　　print('出错了'+str(reason))

else:　　　　　　　　　　　　　　　　#如果程序报错，输出的是字母之类的东西，那输出的只有报错信息，else部分不会输出

　　print('没有任何异常')　　　　　　　#如果没有报错，输出的是int(x)和‘没有任何异常’，也就是else也输出





下面说with

with语句主要用来和open一起用：

with open('xx.txt') as f:

　　print(f.read())

但当你不再需要这个文件时，或已经读取完毕，已经可以关闭的时候，它会帮你自动关闭，不需要你手动close












