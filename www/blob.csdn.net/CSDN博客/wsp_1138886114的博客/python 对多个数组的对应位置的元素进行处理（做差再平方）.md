# python 对多个数组的对应位置的元素进行处理（做差再平方） - wsp_1138886114的博客 - CSDN博客





2018年12月04日 17:26:05[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：604
所属专栏：[python基础及相关](https://blog.csdn.net/column/details/23328.html)









实例：对多个数组的对应位置的元素进行处理

对图像的相似度进行处理，用到了这一步，所以写了这个

如需要查看 [**图像相似度请点击**]()
```python
diff_list_all = [[1,2,3],
                 [4,5,6],
                 [10,10,10]]

sum_all = []
for i in range(len(diff_list_all)):
    for j in range(len(diff_list_all[i])):
        for k in range(i+1,(len(diff_list_all))):
            diffe_squra = (diff_list_all[i][j] - diff_list_all[k][j]) ** 2
            sum_all.append(diffe_squra)
print("sum_all：", sum_all)

sum_All = []
for i in range(len(diff_list_all)):
    sum_All.append(sum_all[(len(diff_list_all)-1)*i])
print("sum_All_1:",sum_All)

# for i in range(len(diff_list_all)):
#     sum_All.append(sum_all[(len(diff_list_all)-1)*i+1])
# print("sum_All_2:",sum_All)
"""
依次计算 (1-4)**2; (1-10)**2  
        (2-5)**2; (2-10)**2
        (3-6)**2; (3-10)**2
        (4-10)**2; (5-10)**2;(6-10)**2
"""
输出：
sum_all： [9, 81, 9, 64, 9, 49, 36, 25, 16]
sum_All_1: [9, 9, 9]

注释部分输出
sum_all： [9, 81, 9, 64, 9, 49, 36, 25, 16]
sum_All_2: [81, 64, 49]
```

封装函数

```python
diff_list_all = [[1,2,3],
                 [4,5,6],
                 [10,10,10]]

def get_sum_all(diff_list_all):
    sum_all = []
    for i in range(len(diff_list_all)):
        for j in range(len(diff_list_all[i])):
            for k in range(i + 1, (len(diff_list_all))):
                diffe_squra = (diff_list_all[i][j] - diff_list_all[k][j]) ** 2
                sum_all.append(diffe_squra)
    return sum_all

def double_img_all(n,sum_all,diff_list_all):
    sum_All = []
    if n>=0 and n<len(diff_list_all):
        for i in range(len(diff_list_all)):
            sum_All.append(sum_all[(len(diff_list_all)-1)*i+n])
        print("sum_All:", sum_All)
    return sum_All

sum_all = get_sum_all(diff_list_all)
for n in range(len(diff_list_all)-1):      #遍历输出： 数组1与数组2的计算值;数组1与数组3的计算值
    double_img_all(n,sum_all,diff_list_all)

输出：
sum_All: [9, 9, 9]
sum_All: [81, 64, 49]
```

封装成一个函数

```python
diff_list_all = [[1,2,3],
                 [4,5,6],
                 [10,10,10]]

def double_img_all(n,diff_list_all):
    sum_All = []
    sum_all = []
    for i in range(len(diff_list_all)):
        for j in range(len(diff_list_all[i])):
            for k in range(i + 1, (len(diff_list_all))):
                diffe_squra = (diff_list_all[i][j] - diff_list_all[k][j]) ** 2
                sum_all.append(diffe_squra)
    if n>=0 and n<len(diff_list_all):
        for i in range(len(diff_list_all)):
            sum_All.append(sum_all[(len(diff_list_all)-1)*i+n])
    else:
        print("error number")
    return sum_all,sum_All

for n in range(len(diff_list_all)-1):      #遍历输出： 数组1与数组2的计算值;数组1与数组3的计算值
    sum_all, sum_All = double_img_all(n,diff_list_all)
    print("sum_All:", sum_All)
    print("sum_all:", sum_all)

输出：
sum_All: [9, 9, 9]
sum_all: [9, 81, 9, 64, 9, 49, 36, 25, 16]
sum_All: [81, 64, 49]
sum_all: [9, 81, 9, 64, 9, 49, 36, 25, 16]
```




