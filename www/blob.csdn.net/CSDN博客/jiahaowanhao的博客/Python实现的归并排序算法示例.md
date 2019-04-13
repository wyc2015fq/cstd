
# Python实现的归并排序算法示例 - jiahaowanhao的博客 - CSDN博客


2018年05月04日 20:39:53[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：60


[Python实现的归并排序算法示例](http://cda.pinggu.org/view/25450.html)
本文实例讲述了Python实现的归并排序算法。分享给大家供大家参考，具体如下：
归并排序是建立在归并操作上的一种有效的排序算法，该算法是采用分治法（Divide and Conquer）的一个非常典型的应用。
将已有序的子序列合并，得到完全有序的序列；即先使每个子序列有序，再使子序列段间有序。若将两个有序表合并成一个有序表，称为二路归并。
Python实现代码如下：
\#-*- coding: UTF-8 -*-
importnumpy as np
defMerge(a, f, m, l):
i=f
j=m+1
tmp=[]
whilei <=mandj <=l:
ifa[i] <=a[j]:
tmp.append(a[i])
i+=1
else:
tmp.append(a[j])
j+=1
whilei <=m:
tmp.append(a[i])
i+=1
whilej<=l:
tmp.append(a[j])
j+=1
i=f
forxinxrange(0,len(tmp)):
a[i]=tmp[x]
i+=1
defMergeSort(a, f, l):
iff< l:
m=(l+f)/2
MergeSort(a, f, m)
MergeSort(a, m+1, l)
Merge(a, f, m, l)
if__name__=='__main__':
a=np.random.randint(0,10, size=10)
print"Before sorting..."
print"---------------------------------------------------------------"
printa
print"---------------------------------------------------------------"
MergeSort(a,0, a.size-1)
print"After sorting..."
print"---------------------------------------------------------------"
printa
print"---------------------------------------------------------------"
运行结果：
![](http://files.jb51.net/file_images/article/201711/20171121151352883.jpg?2017102115148)

