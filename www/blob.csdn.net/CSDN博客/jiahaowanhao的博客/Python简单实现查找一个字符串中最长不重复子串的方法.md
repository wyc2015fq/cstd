
# Python简单实现查找一个字符串中最长不重复子串的方法 - jiahaowanhao的博客 - CSDN博客


2018年07月02日 21:34:25[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：224


[Python简单实现查找一个字符串中最长不重复子串的方法](http://cda.pinggu.org/view/25982.html)
这篇文章主要介绍了Python简单实现查找一个字符串中最长不重复子串的方法,涉及Python针对字符串的简单遍历、运算等相关操作技巧,需要的朋友可以参考下
刚结束的一个笔试题，很简单，不多说简单贴一下具体的实现：
\#!usr/bin/env python
\#encoding:utf-8
'''''
__Author__:沂水寒城
功能：找出来一个字符串中最长不重复子串
'''
def find_longest_no_repeat_substr(one_str):
'''''
找出来一个字符串中最长不重复子串
'''
res_list=[]
length=len(one_str)
for i in range(length):
tmp=one_str[i]
for j in range(i+1, length):
if one_str[j] not in tmp:
tmp+=one_str[j]
else:
break
res_list.append(tmp)
res_list.sort(lambda x,y:cmp(len(x),len(y)))
return res_list[-1]
if __name__ == '__main__':
one_str_list=['120135435','abdfkjkgdok','123456780423349']
for one_str in one_str_list:
res=find_longest_no_repeat_substr(one_str)
print '{0}最长非重复子串为：{1}'.format(one_str, res)
结果如下：
120135435最长非重复子串为：201354
abdfkjkgdok最长非重复子串为：abdfkj
123456780423349最长非重复子串为：123456780

