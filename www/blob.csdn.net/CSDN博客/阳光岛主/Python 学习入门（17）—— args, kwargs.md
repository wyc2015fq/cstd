
# Python 学习入门（17）—— args, kwargs - 阳光岛主 - CSDN博客

2013年12月05日 23:32:34[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：5218所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



The special syntax,`*args`and`**kwargs`in function definitions is used to pass a variable number of arguments to a function. The single asterisk form (`*args`) is used to pass anon-keyworded, variable-length argument list, and the double asterisk form is used to pass akeyworded, variable-length argument list. Here is an example of how to use the non-keyworded form. This example passes one formal (positional) argument, and two more variable length arguments.
deftest_var_args(farg,*args):print"formal arg:",fargforarginargs:print"another arg:",argtest_var_args(1,"two",3)
Results:
formal arg: 1
another arg: two
another arg: 3
Here is an example of how to use the keyworded form. Again, one formal argument and two keyworded variable arguments are passed.
deftest_var_kwargs(farg,**kwargs):print"formal arg:",fargforkeyinkwargs:print"another keyword arg:%s:%s"%(key,kwargs[key])test_var_kwargs(farg=1,myarg2="two",myarg3=3)
Results:
formal arg: 1
another keyword arg: myarg2: two
another keyword arg: myarg3: 3
#### Using
#### *args
#### and
#### **kwargs
#### when
#### calling
#### a function
This special syntax can be used, not only in function definitions, but also whencallinga function.
deftest_var_args_call(arg1,arg2,arg3):print"arg1:",arg1print"arg2:",arg2print"arg3:",arg3args=("two",3)test_var_args_call(1,*args)
Results:
arg1: 1
arg2: two
arg3: 3
Here is an example using the keyworded form when calling a function:
deftest_var_args_call(arg1,arg2,arg3):print"arg1:",arg1print"arg2:",arg2print"arg3:",arg3kwargs={"arg3":3,"arg2":"two"}test_var_args_call(1,**kwargs)
Results:
arg1: 1
arg2: two
arg3: 3
**代码示例**

```python
#!/usr/bin/python
# -*- coding:utf-8 -*-
#
# http://blog.ithomer.net

# *args
def test1(farg, *args):
    print "formal arg", farg
    for arg in args:
        print "another arg:", arg
# **kwargs
def test2(farg, **kwargs):
    print "formal arg", farg
    for key in kwargs:
        print "another keyword arg: %s, %s" % (key, kwargs[key])
# *args
def test3(arg1, arg2, arg3):
    print "arg1:", arg1
    print "arg2:", arg2
    print "arg3:", arg3
# **kwargs
def test4(arg1, arg2, arg3):
    print "arg1:", arg1
    print "arg2:", arg2
    print "arg3:", arg3

# *args
def test5(*args):
    for count, thing in enumerate(args):
        print "%d -> %s" % (count, thing)
# **kwargs
def test6(**kwargs):
    for name, value in kwargs.items():
        print name, "=", value
def test7(a, b, c):
    print "a=",a, "& b=", b, "& c=", c
if __name__ == "__main__":
    test1(1, "two", 3)
    test2(farg=1, myarg2='two', myargs=3)
    args = ("two", 3)
    test3(1, *args)
    kwargs = {"arg3":3, "arg2":"two"}
    #test4(1, **kwargs)
    test4(arg1=1, **kwargs)

    test5("apple", "banana", "cabbage")
    test6(apple="fruit", cabbage="vagetable")
    mylist = ['aa', 'bb', 'cc']
    test7(*mylist)
```
运行结果：
formal arg 1
another arg: two
another arg: 3
formal arg 1
another keyword arg: myarg2, two
another keyword arg: myargs, 3
arg1: 1
arg2: two
arg3: 3
arg1: 1
arg2: two
arg3: 3
0 -> apple
1 -> banana
2 -> cabbage
cabbage = vagetable
apple = fruit
a= aa & b= bb & c= cc


**参考推荐：**
[How to use *args and **kwargs in Python](http://www.saltycrane.com/blog/2008/01/how-to-use-args-and-kwargs-in-python/)
[python *args and * kwargs](http://www.cnblogs.com/xiami303/archive/2012/05/31/2528186.html)



