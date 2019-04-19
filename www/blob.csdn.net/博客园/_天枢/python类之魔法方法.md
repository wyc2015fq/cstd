# python类之魔法方法 - _天枢 - 博客园
## [python类之魔法方法](https://www.cnblogs.com/yhleng/p/9809512.html)
2018-10-18 11:31 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9809512)
python类之魔法方法：
```
class A(object):
    def __init__(self,x):
        self.x = x
    def __neg__(self):
        print('-v')
    def __pos__(self):
        print('+v')
    def __invert__(self):
        print('~v')
a = A('1')
-a
+a
~a
```
输出结果：
```
D:\python36\python3.exe D:/untitled4/temp7.py
-v
+v
~v
Process finished with exit code 0
```
**结论，从上述例子中可以看出，类实例 :**
a = A('1') 类实化化，执行了__init__方法，该方法，相当于构造函数作用
 -a执行的是类的__neg__方法
+a执行是类的__pos__方法
~a执行是类的__invert__方法
