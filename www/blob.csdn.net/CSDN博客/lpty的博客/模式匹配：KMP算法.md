
# 模式匹配：KMP算法 - lpty的博客 - CSDN博客

2018年03月20日 15:27:17[lpty](https://me.csdn.net/sinat_33741547)阅读数：687所属专栏：[自然语言基础](https://blog.csdn.net/column/details/22512.html)



## 一、暴力匹配算法
有一个文本串S和一个模式串P，现在需要查找模式串P在文本串S中的位置，简单的考虑，可以用以下方法：
1、将模式串的第一个字符与文本串的第一个字符进行比较，如果不相同则模式串整体后移一位，继续1步骤；如果相同则跳转步骤2
2、模式串的第一个字符与文本串的第一个字符相同，则继续比较第二个字符，如果不相同则模式串整体后移，跳转到步骤1；如果相同则继续下一个字符的比较
3、上述步骤重复，直到匹配成功，或者已经匹配到文本串的最后一个字符，匹配失败。
## 二、KMP算法
### 1、概论
从暴力匹配算法可以发现，这种方法的时间复杂度是非常高的，为O(n*m)，其中n为字符串长度，m为模式串长度。
而KMP算法则可以实现时间复杂度为O(n+m)，效率更加的快。简单的来说，KMP算法相比暴力匹配算法，在进行匹配的时候，不再是每次一个字符的移动遍历，而是先计算出一张失配表，按照失配值进行移动。
即，KMP算法=暴力匹配算法+失配表
### 2、实现
假设有文本串“BBC ABCDAB ABCDABCDABDE”和模式串“ABCDABD”，现在进行匹配
（1）首先，按照暴力匹配的思路，先对文本串和模式串的第一个字符进行比较，不相同则向后移动一位
![这里写图片描述](//img-blog.csdn.net/20180320143047323?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](//img-blog.csdn.net/20180320143047323?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
（2）继续匹配，直到文本串与模式串的第一个字符相同
![这里写图片描述](//img-blog.csdn.net/20180320143613996?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](//img-blog.csdn.net/20180320143613996?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
（3）再次对文本串与字符串的第二个字符进行对比
![这里写图片描述](//img-blog.csdn.net/20180320143732981?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](//img-blog.csdn.net/20180320143732981?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
（4）重复执行，到这里文本串和模式串出现了不同的字符
![这里写图片描述](//img-blog.csdn.net/20180320143901935?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](//img-blog.csdn.net/20180320143901935?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这个地方就是暴力匹配法和KMP算法不同的分割点，按照暴力匹配的思路，出现不同则模式串整体后移一位，如下：
![这里写图片描述](//img-blog.csdn.net/20180320144144337?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](//img-blog.csdn.net/20180320144144337?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
但是我们可以直观的理解到，接下来的这一串文本我们在之前已经匹配过了，这样的操作相当于我们再一次重复了上一轮的操作，效率非常的低。
而KMP算法与暴力匹配算法的不同在于，失配表。
（5）如下图存在失配表，顾名思义就是在匹配失败的时候进行查询的表，具体计算方式后面再详解
![这里写图片描述](//img-blog.csdn.net/20180320144626750?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](//img-blog.csdn.net/20180320144626750?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在空格与D不匹配的时候，前面已匹配成功字符数为6，查表最后一个匹配字符B的失配值为2，可按下方公式计算：
[
](//img-blog.csdn.net/20180320144626750?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`移动位数 = 已匹配的字符数 - 对应的失配值`[
](//img-blog.csdn.net/20180320144626750?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)在这里计算出来的移动位数为4
（6）移动4位之后，继续进行匹配
![这里写图片描述](//img-blog.csdn.net/20180320150835779?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](//img-blog.csdn.net/20180320150835779?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
可以看到，这里模式串前面的两个字符AB和字符串是相同的，之后继续上述步骤，已匹配字符2，最后一个字符B的失配值为0，移动位数为2
（7）重复上述过程，直到匹配成功
![这里写图片描述](//img-blog.csdn.net/20180320151115991?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 3、失配表
#### （1）概念
前缀，指除了最后一个字符以外，一个字符串的全部头部组合
后缀，指除了第一个字符以外，一个字符串的全部尾部组合
失配值，指前缀和后缀的最长的共有元素的长度
#### （2）实现
如有模式串“ABCDABD”，失配表计算方式如下：
`“A”的前缀和后缀都为空，共有元素长度为0，即表中“A”对应位置失配值为0；
“AB”的前缀为[A]，后缀为[B]，共有元素长度为0，即表中“B”对应位置失配值为0；
"ABC"的前缀为[A, AB]，后缀为[BC, C]，共有元素的长度0；
"ABCD"的前缀为[A, AB, ABC]，后缀为[BCD, CD, D]，共有元素的长度为0；
"ABCDA"的前缀为[A, AB, ABC, ABCD]，后缀为[BCDA, CDA, DA, A]，共有元素为"A"，长度为1；
"ABCDAB"的前缀为[A, AB, ABC, ABCD, ABCDA]，后缀为[BCDAB, CDAB, DAB, AB, B]，共有元素为"AB"，长度为2；
"ABCDABD"的前缀为[A, AB, ABC, ABCD, ABCDA, ABCDAB]，后缀为[BCDABD, CDABD, DABD, ABD, BD, D]，共有元素的长度为0。`即，最后计算出来的对应模式串的失配值为，0000120
## 三、参考
1、[http://www.ruanyifeng.com/blog/2013/05/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm.html](http://www.ruanyifeng.com/blog/2013/05/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm.html)

