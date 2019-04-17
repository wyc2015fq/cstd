# json.load UnicodeDecodeError: 'gbk' codec can't decode byte 0xac - 博客堂 - CSDN博客





2017年09月26日 20:11:24[最小森林](https://me.csdn.net/u012052268)阅读数：2250








# 问题：

使用 json.load()函数或者fp.read() 函数或者open()函数读取中文的时候，发现报错： 

UnicodeDecodeError: ‘gbk’ codec can’t decode byte 0xac

UnicodeDecodeError: ‘gbk’ codec can’t decode byte 0x9d in position 1270: illegal multibyte sequence

这是是字符流的问题。python无法把一些字符Decode成utf8 

，

# 解决办法：

在Decode之前先encoding成UTF8，比如： 

with open(‘labels.json’, ‘r’,encoding=’UTF-8’) as f:

fp.read(‘labels.json’, ‘r’,encoding=’UTF-8’)

等等，总之就是读取的时候就手动encoding成UTF8，这样python自动Decode的时候就不会报错。



