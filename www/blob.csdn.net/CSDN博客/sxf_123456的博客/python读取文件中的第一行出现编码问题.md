# python读取文件中的第一行出现编码问题 - sxf_123456的博客 - CSDN博客
2018年01月25日 10:14:32[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：396
                
在windows上使用open打开utf-8编码的txt文件时开头会有一个多余的字符
它叫BOM,是用来声明编码等信息的,但python会把它当作文本解析
解决办法:open的encoding参数
for line in open('1.txt', encoding='utf_8_sig' ): 
            
