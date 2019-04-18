# 【Python】 enumerate() - YZXnuaa的博客 - CSDN博客
2018年03月07日 16:50:25[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：54
个人分类：[Python库](https://blog.csdn.net/YZXnuaa/article/category/7389269)
## enumerate()使用
- 如果对一个列表，既要遍历索引又要遍历元素时，首先可以这样写：
```python
list1 = ["这", "是", "一个", "测试"]
for i in range (len(list1)):
    print i ,list1[i]
```
- 1
- 2
- 3
- 上述方法有些累赘，利用enumerate()会更加直接和优美：
```python
list1 = ["这", "是", "一个", "测试"]
for index, item in enumerate(list1):
    print index, item
>>>
0 这
1 是
2 一个
3 测试
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- enumerate还可以接收第二个参数，用于指定索引起始值，如：
```python
list1 = ["这", "是", "一个", "测试"]
for index, item in enumerate(list1, 1):
    print index, item
>>>
1 这
2 是
3 一个
4 测试
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
## 补充
如果要统计文件的行数，可以这样写：
```python
count = len(open(filepath, 'r').readlines())
```
- 1
这种方法简单，但是可能比较慢，当**文件比较大**时甚至不能工作。
可以利用enumerate()：
```python
count = 0
for index, line in enumerate(open(filepath,'r'))： 
    count += 1
```
