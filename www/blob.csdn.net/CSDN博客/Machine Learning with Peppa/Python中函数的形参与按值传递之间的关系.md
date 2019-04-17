# Python中函数的形参与按值传递之间的关系 - Machine Learning with Peppa - CSDN博客





2018年01月22日 22:09:15[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：177
所属专栏：[用Python玩转数据](https://blog.csdn.net/column/details/18811.html)










最近几天在看python , 看到函数这一段的时候 , 突然发现一个问题 , 字符串在函数体中重新赋值 , 不会对函数体外 , 该字符串本身的值产生影响 . 

例如 :

```python
def changestr (str):
    str = "inside" 
    print("这是function中 , 值为:",str)
mystr = "outside"
changestr(mystr)
print("这是函数外边 , 值为:",mystr)
```
- 1
- 2
- 3
- 4
- 5
- 6


运行结果为 ：

```
这是function中 , 值为: inside
这是函数外边 , 值为: outside
```
- 1
- 2


考虑到字符串不可变的特性 (python的六种标准类型 , 其中有三种是不可变的类型 , 即 : Number ,String , Tuple) , 我分别测试了Number和Tuple , 发现同以上结果是一样的 , 因为这三种类型只能通过重新赋值来改变对象的值 , 另外三种类型(List , Set , Dictionary) 是可以改变内部的元素 , 于是又测试了这三种seq类型 , 第一次测试如下 :

```python
def change(mylist):
    mylist.append([1,2,3,4])
mylist = ["aa",21]
print(mylist)
change(mylist)
print(mylist)
```
- 1
- 2
- 3
- 4
- 5
- 6


结果如下:

```
['aa', 21]
['aa', 21, [1, 2, 3, 4]]
```
- 1
- 2


发现在函数体中的修改 , 对对象本身的值发生了改变 , 在函数之外 , 该列表的内容依然发生了改变 , 这是事先就能猜测到的结果 , 因为python中的参数 , 传入的是变量引用的副本 , 它与变量指向同一个值. 

鉴于前三种类型的参数是直接重新赋值的 , 于是继续下一步测试 :

```python
def change2(list):
    list = [1,2,3,4]
mylist = ["aa",21]
print(mylist)
change2(mylist)
print(mylist)
```
- 1
- 2
- 3
- 4
- 5
- 6


结果如下:

```
['aa', 21]
['aa', 21]
```
- 1
- 2


有些意外 , 出现了和三种不可变类型参数一样的情况 , 在函数体中的重新赋值 , 没有对外部变量的值产生影响 , 不过仔细一想 , 却又在情理之中 . 

我对python还不够熟悉 , 但是从近两天的学习中发现 , 其存储模型与java相似 , 即变量中存储的是引用 , 是指向真正内容的内存地址(当然 ,java中的八大基本数据类型 , 变量名和值都是存储在堆栈中的 ) , 对变量重新赋值 , 相当于修改了变量副本存储的内存地址 , 而这时的变量已经和函数体外的变量不是同一个了, 在函数体之外的变量 , 依旧存储的是原本的内存地址 , 其值自然没有发生改变 .

简单来说 :

- 函数体传入的参数 , 为函数体外变量引用的副本 . 

- 在函数体中改变变量指向的堆中的值 , 对函数外变量有效. 

- 在函数体中改变变量的引用 , 对函数外变量无效


大致图如下 :(红色线代表重新赋值动作) 
![这里写图片描述](https://img-blog.csdn.net/20170327161031086?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXE5MTU2OTcyMTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



