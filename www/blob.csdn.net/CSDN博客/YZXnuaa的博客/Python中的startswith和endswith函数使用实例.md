# Python中的startswith和endswith函数使用实例 - YZXnuaa的博客 - CSDN博客
2018年01月31日 10:24:45[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：355
在Python中有两个函数分别是startswith()函数与endswith()函数，功能都十分相似，startswith()函数判断文本是否以某个字符开始，endswith()函数判断文本是否以某个字符结束。
**startswith()函数**
此函数判断一个文本是否以某个或几个字符开始，结果以True或者False返回。
复制代码 代码如下:
text='welcome to qttc blog'
print text.startswith('w')      # True
print text.startswith('wel')    # True
print text.startswith('c')      # False
print text.startswith('')       # True
**endswith()函数**
此函数判断一个文本是否以某个或几个字符结束，结果以True或者False返回。
复制代码 代码如下:
text='welcome to qttc blog'
print text.endswith('g')        # True
print text.endswith('go')       # False
print text.endswith('og')       # True
print text.endswith('')         # True
print text.endswith('g ')       # False
**判断文件是否为exe执行文件**
我们可以利用endswith()函数判断文件名的是不是以.exe后缀结尾判断是否为可执行文件
复制代码 代码如下:
# coding=utf8
fileName1='qttc.exe'
if(fileName1.endswith('.exe')):
    print '这是一个exe执行文件'   
else:
    print '这不是一个exe执行文件'
# 执行结果：这是一个exe执行文件
**判断文件名后缀是否为图片**
复制代码 代码如下:
# coding=utf8
fileName1='pic.jpg'
if fileName1.endswith('.gif') or fileName1.endswith('.jpg') or fileName1.endswith('.png'):
    print '这是一张图片'
else:
    print '这不是一张图片'
# 执行结果：这是一张图片
            
