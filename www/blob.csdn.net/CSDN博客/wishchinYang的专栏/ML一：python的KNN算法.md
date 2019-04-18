# ML一：python的KNN算法 - wishchinYang的专栏 - CSDN博客
2014年03月05日 22:01:56[wishchin](https://me.csdn.net/wishchin)阅读数：850
 （1）：list的排序算法：
参考链接：[http://blog.csdn.net/horin153/article/details/7076321](http://blog.csdn.net/horin153/article/details/7076321)
示例： DisListSorted = sorted(DisList, key = lambda DisList: DisList[1], reverse =False) 使用lambda函数，利用元素索引作为key；
在 Python 中, 当需要对一个 list 排序时, 一般可以用 list.sort() 或者 sorted(iterable[, cmp[, key[, reverse]]]).
其中:
cmp(e1, e2) 是带两个参数的比较函数, 返回值: 负数: e1 < e2, 0: e1 == e2, 正数: e1 > e2. 默认为 None, 即用内建的比较函数；
key 是带一个参数的函数, 用来为每个元素提取比较值. 默认为 None, 即直接比较每个元素；
reverse 是一个布尔值, 表示是否反转比较结果.。reverse =False 代表从小到大...
（2）：几种排序算法：
```python
1, 通常, key 和 reverse 比 cmp 快很多, 因为对每个元素它们只处理一次; 而 cmp 会处理多次.
也就是说, 同等情况下, 写 key 函数比写 cmp 函数要高效很多.
2, 对一些貌似比较复杂的排序, 也是不需要写 cmp 函数的, 举例如下:
>>> students = [('john', 'A', 15), ('jane', 'B', 12), ('dave', 'B', 10),]
用元素索引做 key:
>>> sorted(students, key=lambda student: student[2])   # sort by age
[('dave', 'B', 10), ('jane', 'B', 12), ('john', 'A', 15)]
用元素已经命名的属性做 key:
>>> sorted(students, key=lambda student: student.age)   # sort by age
[('dave', 'B', 10), ('jane', 'B', 12), ('john', 'A', 15)]
用 operator 函数来加快速度, 上面两个排序等价于:
>>> from operator import itemgetter, attrgetter
>>> sorted(students, key=itemgetter(2))
>>> sorted(students, key=attrgetter('age'))
用 operator 函数进行多级排序, 这个就是比较复杂的应用. 按我以前的理解, 是一定要写个定制的 cmp 函数的. 现在看来, 以前真的够笨.
# sort by grade then by age
>>> sorted(students, key=itemgetter(1,2))
[('john', 'A', 15), ('dave', 'B', 10), ('jane', 'B', 12)]
>>> sorted(students, key=attrgetter('grade', 'age'))
[('john', 'A', 15), ('dave', 'B', 10), ('jane', 'B', 12)]
3, 根据字典值排序
>>> d = {'a':2, 'b':23, 'c':5, 'd':17, 'e':1}
#1, 返回 pair 对:
from operator import itemgetter
>>> sorted(d.iteritems(), key=itemgetter(1), reverse=True)
[('b', 23), ('d', 17), ('c', 5), ('a', 2), ('e', 1)]
#2, 仅返回 keys:
>>> sorted(d, key=d.__getitem__, reverse=True)
['b', 'd', 'c', 'a', 'e']
 4, sorted() 会返回一个新的已经排好序的 list.
list.sort() 是就地排序, 以节约空间, 当然就不会返回一个排好序的新的 list 了. 这对大的 list 排序是有空间优势的.
```
（3）：KNN算法实例：
```python
<p>#  DataMatrix 为向量数组 列表
</p><p> def creatDataSetLabel(DataMatrix, LabelVec):
    
    Classifier ={}
    if len (LabelVec) !=len (DataMatrix):
        return Classifier
    
    for idx in range(len (LabelVec)):
        Classifier[LabelVec[idx] ]  = DataMatrix[idx]        
    
    return Classifier</p>
<p>#filename 为文件夹
</p><p>def   trainClassifier(filename):
    DataMatrix= creatDataSet(filename)
    LabelNum  = len(DataMatrix )
    LabelVec  = creatLabel(LabelNum)
    Classifier= creatDataSetLabel(DataMatrix,LabelVec)
 
    return Classifier
</p><p>
</p><p>#输入向量，返回k近邻！
</p><p>def classify(Classifier, Feature ,K):
    
    if len(Feature) != len(Classifier[0] ):
        return 0
    
    kNear = []
    Distance ={}# similarity
    
    for i in range(len(Classifier) ):
        DisVec = Classifier[i] - Feature
        DisMat = DisVec **2
        Distance[i] = DisMat.sum()
    
    DisList =[]
    for key in Distance:
        DisList.append([key, Distance[key] ])</p><p>
    DisListSorted = sorted(DisList, key = lambda DisList: DisList[1], reverse =False) #排序 距离
    
    for i in range(K):
        kNear.append(DisListSorted[i] ) 
    
    return kNear
</p>
```
