
# Python二分查找详解 - jiahaowanhao的博客 - CSDN博客


2018年06月22日 21:04:51[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：334


[Python二分查找详解](http://cda.pinggu.org/view/25892.html)
这篇文章主要给大家汇总介绍了Python二分查找的几种实现的方法，有需要的小伙伴可以参考下。
先来看个实例
\#!/usr/bin/env python
import sys
def search2(a,m):
low = 0
high = len(a) - 1
while(low <= high):
mid = (low + high)/2
midval = a[mid]
if midval < m:
low = mid + 1
elif midval > m:
high = mid - 1
else:
print mid
return mid
print -1
return -1
if __name__ == "__main__":
a = [int(i) for i in list(sys.argv[1])]
m = int(sys.argv[2])
search2(a,m)
运行:
administrator@ubuntu:~/Python$ python test_search2.py 123456789 4
3
注:
1.'__'：由于python的类成员都是公有、公开的被存取public，缺少像正统面向对象语言的私有private属性。
于是就用__来将就一下，模拟私有属性。这些__属性往往是内部使用，通常情况下不用改写。也不用读取。
加上2个下划线的目的，一是不和普通公有属性重名冲突，二是不让对象的使用者(非开发者)随意使用。
2.__name__ == "__main__"表示程序脚本是直接被执行的.
如果不等于表示脚本是被其他程序用import引入的.则其__name__属性被设为模块名
Python采用二分查找找出数字的下标
要考虑有重复数字的情况
class Solution(object):
def searchRange(self, nums, target):
"""
:type nums: List[int]
:type target: int
:rtype: List[int]
"""
def binary_search(start,end,value):
while end>=start:
mid = (start+end)//2
print(mid)
if nums[mid]>target:
end = mid-1
elif nums[mid]<target:
start = mid+1
else:
if value==-1:
if mid-1>=start and nums[mid+value] == target:
end = mid+value
else:
return mid
else:
if mid+1<=end and nums[mid+value] == target:
start = mid+value
else:
return mid
return -1
a=binary_search(0,len(nums)-1,-1)
b=binary_search(0,len(nums)-1,1)
return [a,b]
a = Solution()
l = [2,2]
print(a.searchRange(l,2))
二分算法的定义不在多说了，百度一下就知道（支持国产大笑）
import sys
source = [1,2,3,4,5,6,7,8,9,10] \#must be in order
des = int(sys.argv[1])
low = 0
high = len(source) - 1
targetIndex = -1
print "des=",des
while low <= high:
middle = (low + high)/2
if des == source[middle]:
targetIndex = middle
break
elif des < source[middle]:
high = middle -1
print "middle element[index=",middle,",value=",source[middle],"] is bigger than des, continue search from[",low,"to",high,"]"
else:
low = middle + 1
print "middle element[index=",middle,",value=",source[middle],"] is smaller than des, continue search from[",low,"to",high,"]"
print "search complete, target element's index in source list is ",targetIndex
最后在分享一个
'fileName--BinarySearch.py'
src = []
def BinarySearch(low, high, target, *src):
'二分查找'
while low <= high:
mid = (low + high) // 2
midVal = src[mid]
if target < midVal:
high = mid - 1
elif target > midVal:
low = mid + 1
else:
return mid
BinarySearch(low, high, target, *src)
print('Please input 10 number:')
for number in range(10):
src.append(int(input('Num %d:' % number)))
sortList = tuple(src)
key = int(input('Please input key:'))
location = BinarySearch(0, len(src) - 1, key, *sortList)
if location != None:
print('Find target at %d' % (location + 1))
else:
print('No target!')

