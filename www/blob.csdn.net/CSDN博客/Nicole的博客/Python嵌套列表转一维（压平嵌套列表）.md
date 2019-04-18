# Python嵌套列表转一维（压平嵌套列表） - Nicole的博客 - CSDN博客
2018年04月24日 09:55:34[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：351

  前一段去云英面试，技术官很”不厚道“了问了一个非常简单的问题：如何将多维列表转化了一维的？当时虽然想到了使用迭代或者列表生成式可以做到，但是可以没能可行的代码，回来后一顿后悔。
     对于规范的且嵌套维度较低的多维列表，python中有很多方法可以实现：
```python
a=[[1,2],[3,4],[5,6]]  
    print [j for i in li for j in i]  
    #or  
    from itertools import chain  
    print list(chain(*a))  
    #or  
    import itertools  
    a = [[1,2,3],[4,5,6], [7], [8,9]]  
    out = list(itertools.chain.from_iterable(a))  
    #or  
    a=[[1,2],[3,4],[5,6]]  
    t=[]  
    [t.extend(i) for i in a]  
    print t  
    #or  
    a=[[1,2],[3,4],[5,6]]  
    print sum(a,[])  
    #or  
    reduce(lambda x, y: x+ y, a)
```
   对于复杂的多维嵌套列表就需要使用一些复杂的方法：
```python
#递归的方法比较容易理解  
    def expand_list(nested_list):  
        for item in nested_list:  
            if isinstance(item, (list, tuple)):  
                for sub_item in expand_list(item):  
                    yield sub_item  
            else:  
                yield item  
                  
    #在stackoverflow看到大牛的列表生成式版本  
    func = lambda x: [y for l in x for y in func(l)] if type(x) is list else [x]  
    #生成式的方法还有很多，可以自行摸索
```
    对于使用python2.X的童鞋，还可以使用flatten函数来做：
```python
>>> d=[1,2,3,[4,5],[6,7,[8,9]]]  
    >>>from compiler.ast import flatten  
    >>>flatten(a)  
    [1, 2, 3, 4, 5, 6,7,8,9]
```
   其实上边列举的方法仅仅是冰山一角，大海下的冰山就等这童鞋们自行去寻找挖掘了。
上文为转载文章，原文出处：https://blog.csdn.net/acmer_ding/article/details/73195260
