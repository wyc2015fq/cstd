# 【ED距离】使用Python求编辑距离 - CD's Coding - CSDN博客





2016年02月29日 15:11:07[糖果天王](https://me.csdn.net/okcd00)阅读数：1258








## 0x00 前言

在处理数据相关性的时候，我们时常需要用到编辑距离。 

最小编辑距离通常作为一种相似度计算函数被用于多种实际应用，尤其在对于中文自然语言处理中，关于ED距离，[百度百科](http://baike.baidu.com/link?url=Shppxrig9i1CFsZ5pX7x0MEOub5eHN23wHwikTuxid_oy_ZPbGTv3CqxWSgKnYJr6h9MzIvxJdB8ztaBAMLPhq)有云：

> 
编辑距离（Edit Distance），又称Levenshtein距离，是指两个字串之间，由一个转成另一个所需的最少编辑操作次数。许可的编辑操作包括将一个字符替换成另一个字符，插入一个字符，删除一个字符。一般来说，编辑距离越小，两个串的相似度越大。俄罗斯科学家Vladimir Levenshtein在1965年提出这个概念。


这着实是一个比较常用的参数，网上相关的说明也比较多，然而对于中文字符串，该如何便捷简单的计算编辑距离呢？

## 0x01 代码展示

好的好的我懂，先直接上代码是不是比较好？~ 

嘛，确实只要能用能算出来就行了，关于可能出现的问题呀，调试相关的东西也就闲下来的时候看看就好，那就先把Demo贴出来吧~

```python
#-*- coding: UTF-8 -*- 
from bs4 import BeautifulSoup
from multiprocessing import Pool

import os
import re
import sys
import commands
import ConfigParser

def levenshtein(first,second):   
    if len(first) > len(second):   
        first,second = second,first   
    if len(first) == 0:   
        return len(second)   
    if len(second) == 0:   
        return len(first)   
    first_length = len(first) + 1  
    second_length = len(second) + 1  
    distance_matrix = [range(second_length) for x in range(first_length)]    
    #print distance_matrix   
    for i in range(1,first_length):   
        for j in range(1,second_length):   
            deletion = distance_matrix[i-1][j] + 1  
            insertion = distance_matrix[i][j-1] + 1  
            substitution = distance_matrix[i-1][j-1]   
            if first[i-1] != second[j-1]:   
                substitution += 1  
            distance_matrix[i][j] = min(insertion,deletion,substitution)   
    #print distance_matrix   
    return distance_matrix[first_length-1][second_length-1]   

if __name__ == '__main__' :
    # lines = [line.strip().split('\t')[0] for line in file("./test_InputFile")];
    lines = [line.strip() for line in file("./test_InputFile")];
    for i in xrange(0,len(lines)):
        l.Notice("Now Calculating %s-th Word" % str(i))
        for j in xrange(i+1,len(lines)):
            str1,str2 = lines[i].decode('utf-8'),lines[j].decode('utf-8')
            dist = levenshtein(str1,str2)
            dist = (1.0*dist) / (1.0*max(len(str1),len(str2)))
            if dist < 0.5: # 可通过设定阈值，仅输出所需求的细数矩阵
                print "%s\t%s\t%s" % (lines[i],lines[j],str(dist))
```

使用方法：更改读入文件名，或者将已有文件改名为`test_InputFile`放于python文件同目录下，运行python文件重定向输出到自定义文件名即可。 

输入格式：输入文件中，每行一个样本词句，若为多列需要选取，可以更改 
`lines = [line.strip().split('\t')[0] for line in file("./test_InputFile")];`
中的`.split('\t')[0]`部分来切分分割字选取所需列。

## 0x02 疑点解惑

为什么要单独写出来呢，当然是为了让代码（zhan）简（ge）洁（zi）啦，这里写一些本来想要放在注释项给代码里奇怪的地方做个注解的东西：
- `for j in xrange(i+1,len(lines))`

因为对于编辑距离而言 dist(i,j)=dist(j,i)，所以仅输出下三角或者上三角矩阵即可
- `str1,str2 = lines[i].decode('utf-8'),lines[j].decode('utf-8')`

关于这一点，我在上一篇[《【中文编码】使用Python处理中文时的文字编码问题》](http://blog.csdn.net/okcd00/article/details/50765404)里也提到过，因为如果不使用decode暂时性的转一下编码，在传参进入函数中的时候，len(str1)和len(str2)的长度将因为编码差异，与实际中文字符个数不同，导致计算出现错误。
- `dist = (1.0*dist) / (1.0*max(len(str1),len(str2)))`

关于这里，是使用1.0去乘整型，用以达到整型向浮点数转换的作用。

## 0x03 编辑距离伪代码解释

```
/* 伪代码 */

//动态规划经常被用来作为这个问题的解决手段之一。
//整数 Levenshtein距离(字符串 str1[1..m], 字符串 str2[1..n])
//声明变量， d[i , j]用于记录str1[1...i]与str2[1..j]的Levenshtein距离

int d[0..m, 0..n]  //初始化
for i from 0 to m
d[i, 0] := i
for j from 0 to n
d[0, j] := j

//用动态规划方法计算Levenshtein距离
for i from 1 to m
for j from 1 to n
{
    //计算替换操作的代价，如果两个字符相同，则替换操作代价为0，否则为1
    if str1[i]== str2[j] then cost := 0
    else cost := 1
    //d[i,j]的Levenshtein距离，可以有
    d[i, j] := minimum
    (
        d[i-1, j] + 1, //在str1上i位置删除字符（或者在str2上j-1位置插入字符）
        d[i, j-1] + 1, //在str1上i-1位置插入字符（或者在str2上j位置删除字符）
        d[i-1, j-1] + cost // 替换操作
    )
}

return d[m, n]
```

祝愿今后也能身心健康愉快的成长~ 
![lalala](https://img-blog.csdn.net/20160229150951137)



