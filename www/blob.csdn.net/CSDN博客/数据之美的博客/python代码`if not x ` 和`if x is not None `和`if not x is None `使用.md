# python代码`if not x:` 和`if x is not None:`和`if not x is None:`使用 - 数据之美的博客 - CSDN博客
2017年03月17日 16:18:58[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：215
个人分类：[python](https://blog.csdn.net/oppo62258801/article/category/6738404)

代码中经常会有变量是否为None的判断，有三种主要的写法：
 第一种是`if x is None`；
第二种是 `if not x：`；
第三种是`if not x is None`（这句这样理解更清晰`if not (x is None)`） 。
如果你觉得这样写没啥区别，那么你可就要小心了，这里面有一个坑。先来看一下代码：
**[python]**[view
 plain](http://blog.csdn.net/sasoritattoo/article/details/12451359#)[copy](http://blog.csdn.net/sasoritattoo/article/details/12451359#)
- >>> x = 1
- >>> not x  
- False
- >>> x = [1]  
- >>> not x  
- False
- >>> x = 0
- >>> not x  
- True
- >>> x = [0]         # You don't want to fall in this one.
- >>> not x  
- False
在[Python](http://lib.csdn.net/base/python)中 None,
  False, 空字符串"", 0, 空列表[], 空字典{}, 空元组()都相当于False ，即：
**[python]**[view
 plain](http://blog.csdn.net/sasoritattoo/article/details/12451359#)[copy](http://blog.csdn.net/sasoritattoo/article/details/12451359#)
- <strong>notNone == notFalse == not'' == not0 == not [] == not {} == not ()</strong>  
因此在使用列表的时候，如果你想区分x==[]和x==None两种情况的话, 此时`if not x:`将会出现问题：
**[python]**[view
 plain](http://blog.csdn.net/sasoritattoo/article/details/12451359#)[copy](http://blog.csdn.net/sasoritattoo/article/details/12451359#)
- >>> x = []  
- >>> y = None
- >>>   
- >>> x isNone
- False
- >>> y isNone
- True
- >>>   
- >>>   
- >>> not x  
- True
- >>> not y  
- True
- >>>   
- >>>   
- >>> not x isNone
- >>> True
- >>> not y isNone
- False
- >>>   
也许你是想判断x是否为None，但是却把`x==[]`的情况也判断进来了，此种情况下将无法区分。
**对于习惯于使用if not x这种写法的pythoner，必须清楚x等于None,  False, 空字符串"", 0, 空列表[], 空字典{}, 空元组()时对你的判断没有影响才行。 **
而对于`if x is not None`和`if not x is None`写法，很明显前者更清晰，而后者有可能使读者误解为`if (not x) is None`，因此推荐前者，同时这也是谷歌推荐的风格
**结论：**
**`if x is not None`是最好的写法，清晰，不会出现错误，以后坚持使用这种写法。**
**使用if not x这种写法的前提是：必须清楚x等于None,  False, 空字符串"", 0, 空列表[], 空字典{}, 空元组()时对你的判断没有影响才行。**
================================================================
不过这并不适用于变量是函数的情况，以下转载自：[https://github.com/wklken/stackoverflow-py-top-qa/blob/master/contents/qa-control-flow.md](https://github.com/wklken/stackoverflow-py-top-qa/blob/master/contents/qa-control-flow.md)
### foo is None 和 foo == None的区别
问题 [链接](http://stackoverflow.com/questions/26595/is-there-any-difference-between-foo-is-none-and-foo-none)
```
if foo is None: pass
if foo == None: pass
```
如果比较相同的对象实例，is总是返回True 而 == 最终取决于 "**eq**()"
```
>>> class foo(object):
    def __eq__(self, other):
        return True
>>> f = foo()
>>> f == None
True
>>> f is None
False
>>> list1 = [1, 2, 3]
>>> list2 = [1, 2, 3]
>>> list1==list2
True
>>> list1 is list2
False
```
另外
`(ob1 is ob2) 等价于 (id(ob1) == id(ob2))`
################################################################################
补充，2013.10.09
转自[](http://zhidao.baidu.com/question/307786603.html)[http://zhidao.baidu.com/question/514056244.html](http://zhidao.baidu.com/question/514056244.html)
# python中的not具体表示是什么，举个例子说一下，衷心的感谢
在python中not是逻辑判断词，用于[布尔型](http://zhidao.baidu.com/search?word=%E5%B8%83%E5%B0%94%E5%9E%8B&fr=qb_search_exp&ie=utf8)True和False，not True为False，not False为True，以下是几个常用的not的用法：
(1) not与逻辑[判断句](http://zhidao.baidu.com/search?word=%E5%88%A4%E6%96%AD%E5%8F%A5&fr=qb_search_exp&ie=utf8)if连用，代表not后面的表达式为False的时候，执行[冒号](http://zhidao.baidu.com/search?word=%E5%86%92%E5%8F%B7&fr=qb_search_exp&ie=utf8)后面的语句。比如：
a = False
if not a:   (这里因为a是False，所以not a就是True)
    print "hello"
这里就能够输出结果hello
(2) 判断元素是否在列表或者字典中，if a not in b，a是元素，b是列表或字典，这句话的意思是如果a不在列表b中，那么就执行[冒号](http://zhidao.baidu.com/search?word=%E5%86%92%E5%8F%B7&fr=qb_search_exp&ie=utf8)后面的语句，比如：
a = 5
b = [1, 2, 3]
if a not in b:
    print "hello"
这里也能够输出结果hello
not x     意思相当于     if x is false, then True, else False
