
# How to Strike a Match - lpty的博客 - CSDN博客

2018年05月02日 15:55:08[lpty](https://me.csdn.net/sinat_33741547)阅读数：742标签：[StrikeAMatch																](https://so.csdn.net/so/search/s.do?q=StrikeAMatch&t=blog)[相似度																](https://so.csdn.net/so/search/s.do?q=相似度&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=StrikeAMatch&t=blog)个人分类：[结构算法																](https://blog.csdn.net/sinat_33741547/article/category/7513227)[论文阅读																](https://blog.csdn.net/sinat_33741547/article/category/7727913)[
							](https://blog.csdn.net/sinat_33741547/article/category/7513227)
所属专栏：[自然语言基础](https://blog.csdn.net/column/details/22512.html)



## 一、概念
来自于Simon White发表的一篇文章：[How to Strike a Match](http://www.catalysoft.com/articles/StrikeAMatch.html)
这个算法主要为了满足以下三个需求：
`（1）字符串之间只是在某几个字符处出现不同，那么相似度应该比较高。
（2）字符串的区别只是相同的词组以不同的顺序排列，那么相似度应该比较高。
（3）语言无关性，算法应该满足多种语言的的相似度计算。`论文中对比了几种相似度算法，
Soundex Algorithm会因为字符串开头的字符不同而导致字符串形成的编码不一致而导致相似度降低，如：‘FRANCE’ 和 ‘REPUBLIC OF FRANCE’；
Edit Distance则对字符串长度敏感，随字符串长度差距增大而相似度降低，如：‘FRANCE’ 和 ‘QUEBEC’ 的相似度要比 ‘FRANCE’ 和 'REPUBLIC OF FRANCE’的相似度高；
Longest Common Substring提取公共子字符串，则会造成拥有相同子字符串的字符串相似度一致，如：‘FRENCH REPUBLIC’ 与 'REPUBLIC OF FRANCE’的相似度 和’REPUBLIC OF CUBA’的相似度一致。
## 二、思想
论文里算法的思想相当简单，对于转化为大写的两个字符串，根据字符串的顺序，以2-gram的形式取出字符对，如：
`FRANCE: {FR, RA, AN, NC, CE}
FRENCH: {FR, RE, EN, NC, CH}`之后，取两个集合的交集，记录交集字符对的数量，最后再将两倍的交集字符对总数量除以两个集合字符对的总数量，如：
![这里写图片描述](https://img-blog.csdn.net/20180502154400786?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
具体到例子中的字符串，代入可得：
![这里写图片描述](https://img-blog.csdn.net/20180502154754455?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
最终得到的结果便是字符串之间的相似度。
## 三、例子
论文中还贴出了一些例子，简单摘录如下：
（1）与‘Healed’的相似度
![这里写图片描述](https://img-blog.csdn.net/20180502155045280?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
（2）图书名称相似度
![这里写图片描述](https://img-blog.csdn.net/20180502155137188?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
注：本文中图片均来自于论文截图

[
](https://img-blog.csdn.net/20180502155045280?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
