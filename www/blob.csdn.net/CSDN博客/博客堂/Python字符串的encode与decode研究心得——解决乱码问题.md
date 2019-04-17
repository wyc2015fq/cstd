# Python字符串的encode与decode研究心得——解决乱码问题 - 博客堂 - CSDN博客





2017年05月02日 20:00:03[最小森林](https://me.csdn.net/u012052268)阅读数：3075








为什么Python使用过程中会出现各式各样的乱码问题，明明是中文字符却显示成“/xe4/xb8/xad/xe6/x96/x87”的形式？

为什么会报错“UnicodeEncodeError: ‘ascii’ codec can’t encode characters in position 0-1: ordinal not in range(128)”？

为啥’gb2312’ codec can’t decode byte 0x95 

本文就来研究一下这个问题。


- [基本知识decode与encode区别](#基本知识decode与encode区别)
- [判断是否为unicode](#判断是否为unicode)
- [实例1](#实例1)
- [实例2](#实例2)



# 基本知识：decode与encode区别

字符串在Python内部的表示是unicode编码，因此，在做编码转换时，通常需要以unicode作为中间编码，即：先将其他编码的字符串解码（decode）成unicode，再从unicode编码（encode）成另一种编码。 
- 
**decode的作用**是将其他编码的字符串转换成unicode编码。 

如str1.decode(‘gb2312’)，表示将gb2312编码的字符串str1转换成unicode编码。 

- 
**encode的作用**是将unicode编码转换成其他编码的字符串。 

如str2.encode(‘gb2312’)，表示将unicode编码的字符串str2转换成gb2312编码。 


因此，转码的时候一定要先搞明白，字符串str是什么编码，然后decode成unicode，然后再encode成其他编码

# 判断是否为unicode

如果一个字符串已经是unicode了，再进行解码则将出错，因此通常要对其编码方式是否为unicode进行判断：
`isinstance(s, unicode)  #用来判断是否为unicode `
用非unicode编码形式的str来encode会报错 

# 实例1

原理说了半天，最后来个包治百病的吧：）

```
#!/usr/bin/env python  
#coding=utf-8  
s="中文"  

if isinstance(s, unicode):  
#s=u"中文"  
    print s.encode('gb2312')  
else:  
#s="中文"  
    print s.decode('utf-8').encode('gb2312')
```

# 实例2

有的时候，会提示错误：

‘gb2312’ codec can’t decode byte 0x95 in position 252873: illegal multibyte sequence

或者：

‘gb2312’ codec can’t decode byte 0x95 in position 252873: illegal multibyte sequence

这是因为：对string进行解码的时候，有个别字符解不出来，比如0x01就是空格。可以直接忽略，加入errors=’ignore’。具体解法如下：

```bash
html = buff.decode("gb2312",errors='ignore')
```



