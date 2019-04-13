
# Python八大常见排序算法定义、实现及时间消耗效率分析 - jiahaowanhao的博客 - CSDN博客


2018年06月08日 21:29:48[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：49标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)


[Python八大常见排序算法定义、实现及时间消耗效率分析](http://cda.pinggu.org/view/25769.html)
本文实例讲述了Python八大常见排序算法定义、实现及时间消耗效率分析。分享给大家供大家参考，具体如下：
昨晚上开始总结了一下常见的几种排序算法，由于之前我已经写了好几篇排序的算法的相关博文了现在总结一下的话可以说是很方便的，这里的目的是为了更加完整详尽的总结一下这些排序算法，为了复习基础的东西，从冒泡排序、直接插入排序、选择排序、归并排序、希尔排序、桶排序、堆排序。快速排序入手来分析和实现，在最后也给出来了简单的时间统计，重在原理、算法基础，其他的次之，这些东西的熟练掌握不算是对之后的工作或者接下来的准备面试都是很有帮助的，算法重在理解内在含义和理论基础，在实现的时候才能避开陷阱少出错误，这不是说练习的时候有错误不好而是说，有些不该出现的错误尽量还是少出现的好，毕竟好的编程习惯是离不开严格的约束的，好了，这里就不多说了，复习一下基础知识，共同学习吧，下面是具体实现，注释应该都很详细，就不解释了：
\#!usr/bin/env python
\#encoding:utf-8
'''''
__Author__:沂水寒城
功能：八大排序算法
'''
import time
import random
time_dict={}
def time_deco(sort_func):
'''''
时间计算的装饰器函数,可用于计算函数执行时间
'''
def wrapper(num_list):
start_time=time.time()
res=sort_func(num_list)
end_time=time.time()
time_dict[str(sort_func)]=(end_time-start_time)*1000
print '耗时为：',(end_time-start_time)*1000
print '结果为：', res
return wrapper
def random_nums_generator(max_value=1000, total_nums=20):
'''''
随机数列表生成器
一些常用函数：
random随机数生成
random.random()用于生成一个0到1之间的随机数:0 <= n < 1.0；
random.uniform(a, b)，用于生成一个指定范围内的随机符点数，两个参数其中一个是上限，一个是下限。min(a,b) <= n <= max(a,b)；
randdom.randint(a, b), 用于生成一个指定范围内的整数，其中a是下限，b是上限： a<= n <= b；
random.randrange(start, stop, step), 从指定范围内，按指定基数递增的集合获取一个随机数；
random.choice(sequence), 从序列中获取一个随机元素；
random.shuffle(x), 用于将一个列表中的元素打乱；
random.sample(sequence, k), 从指定序列中随机获取指定长度的片断；
'''
num_list=[]
for i in range(total_nums):
num_list.append(random.randint(0,max_value))
return num_list
\#@time_deco
def Bubble_sort(num_list):
'''''
冒泡排序，时间复杂度O(n^2)，空间复杂度O(1)，是稳定排序
'''
for i in range(len(num_list)):
for j in range(i,len(num_list)):
if num_list[i]>num_list[j]: \#这里是升序排序
num_list[i], num_list[j]=num_list[j], num_list[i]
return num_list
\#@time_deco
def Insert_sort(num_list):
'''''
直接插入排序，时间复杂度O(n^2)，空间复杂度O(1)，是稳定排序
'''
for i in range(len(num_list)):
for j in range(0,i):
if num_list[i]<num_list[j]: \#这里是升序排序,跟冒泡排序差别在于，冒泡是向后遍历，这个是向前遍历
num_list[i], num_list[j]=num_list[j], num_list[i]
return num_list
\#@time_deco
def Select_sort(num_list):
'''''
选择排序，时间复杂度O(n^2)，空间复杂度O(1)，不是稳定排序
'''
for i in range(len(num_list)):
min_value_index=i
for j in range(i, len(num_list)):
if num_list[j]<num_list[min_value_index]:
min_value_index=j \#乍一看，感觉冒泡，选择，插入都很像，选择跟冒泡的区别在于：冒泡是发现大
\#小数目顺序不对就交换，而选择排序是一轮遍历结束后选出最小值才交换，效率更高
num_list[i], num_list[min_value_index]=num_list[min_value_index], num_list[i]
return num_list
\#@time_deco
def Merge_sort(num_list):
'''''
归并排序，时间复杂度O(nlog₂n)，空间复杂度：O(1)，是稳定排序
'''
if len(num_list)==1:
return num_list
length=len(num_list)/2
list1=num_list[:length]
list2=num_list[length:]
result_list=[]
while len(list1) and len(list2):
if list1[0]<=list2[0]:
result_list.append(list1[0])
del list1[0] \#这里需要删除列表中已经被加入到加过列表中的元素，否则最后比较完后列表
else:       \#中剩余元素无法添加
result_list.append(list2[0])
del list1[0]
if len(list1): \#遍历比较完毕后列表中剩余元素的添加
result_list+=list1
else:
result_list+=list2
return result_list
\#@time_deco
def Shell_sort(num_list):
'''''
希尔排序，时间复杂度：O(n)，空间复杂度：O(n^2),不是稳定排序算法
'''
new_list = []
for one_num in num_list:
new_list.append(one_num)
count=len(new_list)
step=count/2;
while step>0:
i=0
while i<count:
j=i+step
while j<count:
t=new_list.pop(j)
k=j-step
while k>=0:
if t>=new_list[k]:
new_list.insert(k+1, t)
break
k=k-step
if k<0:
new_list.insert(0, t)
\#print '---------本轮结果为：--------'
\#print new_list
j=j+step
\#print j
i=i+1
\#print i
step=step/2   \#希尔排序是一个更新步长的算法
return new_list
\#@time_deco
def Tong_sort(num_list):
'''''
桶排序，时间复杂度O(1)，空间复杂度与最大数字有关，可以认为是O(n)，典型的空间换时间的做法
'''
original_list = []
total_num=max(num_list) \#获取桶的个数
for i in range(total_num+1): \#要注意这里需要的数组元素个数总数比total_num数多一个因为下标从0开始
original_list.append(0)
for num in num_list:
original_list[num] += 1
result_list = []
for j in range(len(original_list)):
if original_list[j] != 0:
for h in range(0,original_list[j]):
result_list.append(j)
return result_list
def Quick_sort(num_list):
'''''
快速排序，时间复杂度：O(nlog₂n)，空间复杂度：O(nlog₂n)，不是稳定排序
'''
if len(num_list)<2:
return num_list
left_list = [] \#存放比基准结点小的元素
right_list = [] \#存放比基准元素大的元素
base_node = num_list.pop(0) \#在这里采用pop()方法的原因就是需要移除这个基准结点，并且赋值给base_node这个变量
\#在这里不能使用del()方法，因为删除之后无法再赋值给其他变量使用，导致最终数据缺失
\#快排每轮可以确定一个元素的位置，之后递归地对两边的元素进行排序
for one_num in num_list:
if one_num < base_node:
left_list.append(one_num)
else:
right_list.append(one_num)
return Quick_sort(left_list) + [base_node] + Quick_sort(right_list)
def Heap_adjust(num_list, i, size):
left_child = 2*i+1
right_child = 2*i+2
max_temp = i
\#print left_child, right_child, max_temp
if left_child<size and num_list[left_child]>num_list[max_temp]:
max_temp = left_child
if right_child<size and num_list[right_child]>num_list[max_temp]:
max_temp = right_child
if max_temp != i:
num_list[i], num_list[max_temp] = num_list[max_temp], num_list[i]
Heap_adjust(num_list, max_temp, size) \#避免调整之后以max为父节点的子树不是堆
def Create_heap(num_list, size):
a = size/2-1
for i in range(a, -1, -1):
\#print '**********', i
Heap_adjust(num_list, i, size)
\#@time_deco
def Heap_sort(num_list):
'''''
堆排序，时间复杂度：O(nlog₂n)，空间复杂度：O(1)，不是稳定排序
'''
size=len(num_list)
Create_heap(num_list, size)
i = size-1
while i > 0:
num_list[0], num_list[i] = num_list[i], num_list[0]
size -= 1
i -= 1
Heap_adjust(num_list, 0, size)
return num_list
if __name__ == '__main__':
num_list=random_nums_generator(max_value=100, total_nums=50)
print 'Bubble_sort', Bubble_sort(num_list)
print 'Insert_sort', Insert_sort(num_list)
print 'Select_sort', Select_sort(num_list)
print 'Merge_sort', Merge_sort(num_list)
print 'Shell_sort', Shell_sort(num_list)
print 'Tong_sort', Tong_sort(num_list)
print 'Heap_sort', Heap_sort(num_list)
print 'Quick_sort', Quick_sort(num_list)
\# print '-----------------------------------------------------------------------------'
\# for k,v in time_dict.items():
\#   print k, v
结果如下：
Bubble_sort [34, 49, 63, 67, 71, 72, 75, 120, 128, 181, 185, 191, 202, 217, 241, 257, 259, 260, 289, 293, 295, 304, 311, 326, 362, 396, 401, 419, 423, 456, 525, 570, 618, 651, 701, 711, 717, 718, 752, 774, 813, 816, 845, 885, 894, 900, 918, 954, 976, 998]
Insert_sort [34, 49, 63, 67, 71, 72, 75, 120, 128, 181, 185, 191, 202, 217, 241, 257, 259, 260, 289, 293, 295, 304, 311, 326, 362, 396, 401, 419, 423, 456, 525, 570, 618, 651, 701, 711, 717, 718, 752, 774, 813, 816, 845, 885, 894, 900, 918, 954, 976, 998]
Select_sort [34, 49, 63, 67, 71, 72, 75, 120, 128, 181, 185, 191, 202, 217, 241, 257, 259, 260, 289, 293, 295, 304, 311, 326, 362, 396, 401, 419, 423, 456, 525, 570, 618, 651, 701, 711, 717, 718, 752, 774, 813, 816, 845, 885, 894, 900, 918, 954, 976, 998]
Merge_sort [34, 49, 63, 67, 71, 72, 75, 120, 128, 181, 185, 191, 202, 217, 241, 257, 259, 260, 289, 293, 295, 304, 311, 326, 362, 396, 401, 419, 423, 456, 525, 570, 618, 651, 701, 711, 717, 718, 752, 774, 813, 816, 845, 885, 894, 900, 918, 954, 976, 998]
Shell_sort [34, 49, 63, 67, 71, 72, 75, 120, 128, 181, 185, 191, 202, 217, 241, 257, 259, 260, 289, 293, 295, 304, 311, 326, 362, 396, 401, 419, 423, 456, 525, 570, 618, 651, 701, 711, 717, 718, 752, 774, 813, 816, 845, 885, 894, 900, 918, 954, 976, 998]
Tong_sort [34, 49, 63, 67, 71, 72, 75, 120, 128, 181, 185, 191, 202, 217, 241, 257, 259, 260, 289, 293, 295, 304, 311, 326, 362, 396, 401, 419, 423, 456, 525, 570, 618, 651, 701, 711, 717, 718, 752, 774, 813, 816, 845, 885, 894, 900, 918, 954, 976, 998]
Heap_sort [34, 49, 63, 67, 71, 72, 75, 120, 128, 181, 185, 191, 202, 217, 241, 257, 259, 260, 289, 293, 295, 304, 311, 326, 362, 396, 401, 419, 423, 456, 525, 570, 618, 651, 701, 711, 717, 718, 752, 774, 813, 816, 845, 885, 894, 900, 918, 954, 976, 998]
Quick_sort [34, 49, 63, 67, 71, 72, 75, 120, 128, 181, 185, 191, 202, 217, 241, 257, 259, 260, 289, 293, 295, 304, 311, 326, 362, 396, 401, 419, 423, 456, 525, 570, 618, 651, 701, 711, 717, 718, 752, 774, 813, 816, 845, 885, 894, 900, 918, 954, 976, 998]
这里没有使用到装饰器，主要自己对这个装饰器不太了解，在快速排序的时候报错了，也没有去解决，这里简单贴一下一个测试样例使用装饰器的结果吧：
Bubble_sort 耗时为： 0.0290870666504
结果为： [5, 45, 46, 63, 81, 83, 89, 89, 89, 90]
None
Insert_sort 耗时为： 0.0209808349609
结果为： [5, 45, 46, 63, 81, 83, 89, 89, 89, 90]
None
Select_sort 耗时为： 0.0259876251221
结果为： [5, 45, 46, 63, 81, 83, 89, 89, 89, 90]
None
Merge_sort 耗时为： 0.0138282775879
结果为： [5, 45, 46, 63, 81, 83, 89, 89, 89, 90]
None
Shell_sort 耗时为： 0.113964080811
结果为： [5, 45, 46, 63, 81, 83, 89, 89, 89, 90]
None
Tong_sort 耗时为： 0.0460147857666
结果为： [5, 45, 46, 63, 81, 83, 89, 89, 89, 90]
None
Heap_sort 耗时为： 0.046968460083
结果为： [5, 45, 46, 63, 81, 83, 89, 89, 89, 90]
None
Quick_sort [5, 45, 46, 63, 81, 83, 89, 89, 89, 90]
-----------------------------------------------------------------------------
<function Shell_sort at 0x7f8ab9d34410> 0.113964080811
<function Select_sort at 0x7f8ab9d34230> 0.0259876251221
<function Insert_sort at 0x7f8ab9d34140> 0.0209808349609
<function Heap_sort at 0x7f8ab9d34758> 0.046968460083
<function Merge_sort at 0x7f8ab9d34320> 0.0138282775879
<function Tong_sort at 0x7f8ab9d34500> 0.0460147857666
<function Bubble_sort at 0x7f8ab9d34050> 0.0290870666504
接下来有时间的话我想学一下装饰器的东西，感觉对于模式化的东西装饰器简直就是一个神器，但是得明白会用会写才行哈！

