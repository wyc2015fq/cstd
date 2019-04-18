# python学习笔记(十九)  -- 单元测试和文档测试 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月26日 15:39:08[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：517
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



**Table of Contents**

[测试的使用场景](#%E6%B5%8B%E8%AF%95%E7%9A%84%E4%BD%BF%E7%94%A8%E5%9C%BA%E6%99%AF)

[单元测试](#%E5%8D%95%E5%85%83%E6%B5%8B%E8%AF%95)

[文档测试](#%E6%96%87%E6%A1%A3%E6%B5%8B%E8%AF%95)

## 测试的使用场景

我们写一套代码想要提供某些功能，写完了以后我们需要对其进行测试，我们将这些测试的代码写在一个或多个模块中，只要一执行，就能判断代码是否有问题。说白了就是 用一些代码去测试一些功能，更改代码以后这个测试代码以后还能使用，或许会需要少量的修改。

## 单元测试

先说一下单元测试涉及的几个对象。

Testcase：每一个以test 开头的方法被称为一个测试用例。

TestSuit：可以把多个测试用例放在TestSuit中，统一执行。并且可以指定执行顺序。

    TestSuit自己有两个函数叫做addTests和addTest，可以将Testcase放入TestSuit中，测试用例执行的顺序就是放入的顺序。

    其中addTests 接收的是一个列表，列表中的每一个元素都是一个测试用例对象。

    addTest接收的 是一个 测试用例对象。

TestLoader：将Testcase 放入 TestSuit 中，但是如果使用TestLoader是无法指定测试用例的执行顺序的。

TextTestRunner：用来执行TestSuit，在创建TesxtTestRunner对象时可以指定一个参数叫verbosity。

                          verbosity 有 3个值，分为为 0、1、2 ，代表输出信息的详细程度。2最详细

**下面为一个简单的单元测试**

目录结构如下

![](https://img-blog.csdn.net/20180826144248371?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

先写一个被测试的代码

mathfunc.py 

```python
# coding=utf8
def add(a, b):
    return a+b

def minus(a, b):
    return a-b

def multi(a, b):
    return a*b

def divide(a, b):
    return a//b   # 我的python版本是3.7， 两个 /  代表取整数部分，一个 / 代表取精确的值，即包含小数

if __name__ == '__main__':
    print(divide(5, 2))
```

测试代码

manthfuct.py

```python
# -*- coding: utf-8 -*-

from my_unittest.mathfunc import *   # 导入被测试模块中的函数
import unittest     # 测试代码需要使用的模块

class TestMathFunc(unittest.TestCase): # 继承unittest.TestCase

    """Test mathfuc.py"""

    def test_add(self):    # 只有以test 开头的函数，才被认为是测试用例
        """Test method add(a,b)"""
        self.assertEqual(3, add(1, 2))    # 判断两个参数是否相等
        self.assertNotEqual(3, add(2, 2))  # 判断两个参数是否不相等

    def test_minus(self):
        """Test method minus"""
        self.assertEqual(1, minus(3, 2))

    def test_multi(self):
        """Test method multi"""
        self.assertEqual(6, multi(3, 2))

    def test_divide(self):
        """Test method divide(a, b)"""
        self.assertEqual(2, divide(6, 3))
        self.assertEqual(2.5, divide(5, 2))

    def setUp(self):   # 调用每一个测试方法之前会自动调用setUp函数
        print("setUp")

    def tearDown(self):  # 调用每一个测试方法之后会自动调用tearDown函数
        print("tearDown")

if __name__ == '__main__':  
    unittest.main()   # 以这种方式来执行测试用例
```

测试结果

```
Testing started at 15:02 ...
D:\工作软件\python\pythonw.exe "D:\PyCharm 2018.2\helpers\pycharm\_jb_unittest_runner.py" --path D:/工作资料/pythonTestWorkSpace/my_unittest/test_manthfuct.py
Launching unittests with arguments python -m unittest D:/工作资料/pythonTestWorkSpace/my_unittest/test_manthfuct.py in D:\工作资料\pythonTestWorkSpace\my_unittest
setUp
tearDown
setUp
tearDown

2 != 2.5

Expected :2.5
Actual   :2
 <Click to see difference>

Traceback (most recent call last):
  File "D:\PyCharm 2018.2\helpers\pycharm\teamcity\diff_tools.py", line 32, in _patched_equals
    old(self, first, second, msg)
  File "D:\工作软件\python\lib\unittest\case.py", line 839, in assertEqual
    assertion_func(first, second, msg=msg)
  File "D:\工作软件\python\lib\unittest\case.py", line 832, in _baseAssertEqual
    raise self.failureException(msg)
AssertionError: 2.5 != 2

During handling of the above exception, another exception occurred:

Traceback (most recent call last):
  File "D:\工作软件\python\lib\unittest\case.py", line 59, in testPartExecutor
    yield
  File "D:\工作软件\python\lib\unittest\case.py", line 615, in run
    testMethod()
  File "D:\工作资料\pythonTestWorkSpace\my_unittest\test_manthfuct.py", line 28, in test_divide
    self.assertEqual(2.5, divide(5, 2))

setUp
tearDown
setUp
tearDown

Ran 4 tests in 0.004s

FAILED (failures=1)

Process finished with exit code 1
```

可以看出，上面的测试用例报错，说是在执行test_divide的时候，我们期待返回2.5，但是返回2。 2.5 不等于 2。抛出了异常。

我们修改 manthfuct.py 模块中的divide函数中的 //  ，改为 /

再测试

```
Testing started at 15:07 ...
D:\工作软件\python\pythonw.exe "D:\PyCharm 2018.2\helpers\pycharm\_jb_unittest_runner.py" --path D:/工作资料/pythonTestWorkSpace/my_unittest/test_manthfuct.py
Launching unittests with arguments python -m unittest D:/工作资料/pythonTestWorkSpace/my_unittest/test_manthfuct.py in D:\工作资料\pythonTestWorkSpace\my_unittest
setUp
tearDown
setUp
tearDown
setUp
tearDown
setUp
tearDown

Ran 4 tests in 0.006s

OK

Process finished with exit code 0
```

这次测试通过。

**如果我们想指定上面4个测试用例的执行顺序，或者说我们有更多个测试用例并且在不同的模块当中，我们则需要使用TestSuit**

如下：

我们再新建模块

test_suite.py

```python
# coding:utf-8

import unittest
from my_unittest.test_manthfuct import TestMathFunc

"""
TestSuit可以理解为TestCase的一个容器，我们可以将多个TestCase放入TestSuit中，然后一起执行
执行的顺序，与放入的顺序相同。
"""

if __name__ == '__main__':

    # 创建TstSuite对象
    suite = unittest.TestSuite()

    # 声明一个TestCase 列表，列表中的TestCase顺序，代表测试用例的执行顺序
    tests = [TestMathFunc("test_add"), TestMathFunc("test_minus"), TestMathFunc("test_divide")]

    # 将TestCase放入TsetSuit中
    suite.addTests(tests)

    # 直接用addTest方法添加单个TestCase
    suite.addTest(TestMathFunc("test_multi"))

    # 创建TextTestRunner对象(用来运行TestSuit)
    runner = unittest.TextTestRunner(verbosity=2)  # verbosity代表信息显示的详细程度，分为 0,1,2 ，0最简单、2最详细
    # 执行TestSuite
    runner.run(suite)

"""
    将测试用例放入suit中的其他方法：
    
    # 用addTests + TestLoader
    # loadTestsFromName()，传入'模块名.TestCase名'
    suite.addTests(unittest.TestLoader().loadTestsFromName('test_manthfunc.TestMathFunc'))
    
    # loadTestsFromNames()，类似，传入列表
    suite.addTests(unittest.TestLoader().loadTestsFromNames(['test_manthfunc.TestMathFunc'])) 
         
    # loadTestsFromTestCase()，传入TestCase
    suite.addTests(unittest.TestLoader().loadTestsFromTestCase(TestMathFunc))
"""
```

运行结果如下，因为我上面的verbosity = 2 ，所以可以看到执行每一个测试用例的详细信息 

```
D:\工作软件\python\pythonw.exe D:/工作资料/pythonTestWorkSpace/my_unittest/test_suite.py
setUp
test_add (my_unittest.test_manthfuct.TestMathFunc)
tearDown
Test method add(a,b) ... ok
setUp
test_minus (my_unittest.test_manthfuct.TestMathFunc)
tearDown
Test method minus ... ok
setUp
test_divide (my_unittest.test_manthfuct.TestMathFunc)
tearDown
Test method divide(a, b) ... ok
setUp
test_multi (my_unittest.test_manthfuct.TestMathFunc)
tearDown
Test method multi ... ok

----------------------------------------------------------------------
Ran 4 tests in 0.000s

OK

Process finished with exit code 0
```

并且我们可以看到，测试用例的执行顺序，是与我们放入TestSuit中的顺序是相同的。

其实单元测试还以将 测试信息放入文件中甚至HTML页面中，这里我就不说了，感兴趣的可以看[这里](https://blog.csdn.net/weixin_40247263/article/details/81838143)。里面涉及一些笔记的补充。

## 文档测试

上面的单元测试，是将测试代码写在了别的模块当中，然后运行该模块执行代码进行测试。

而文档测试，并不是通过代码进行测试，而是直接执行我们需要被测试的代码，但是执行过程是写在文档当中的，这个文档可以通过一些手段将其执行，并判断执行的结果是否与我们期待的结果相同，从而判断测试是否通过。

如下：

我们将测试的执行步骤写在 """ """ 中，使用doctest.testmod()即可执行文档注释中的代码。

当执行到 d1.x 就会判断返回值是否与我们期待的返回值 100 相同

当执行到d2['empty'] 和 d2.empty 我们期待它会抛出异常

注意：

   1      Traceback (most recent call last):  

   2         ...

   3      KeyError: 'empty'

第一行必须这样写，第三行的异常类型也必须与实际会抛出的异常类型相对应，第二行可以用 ... 进行省略，也可以在

...     这里随便写    .... 

```python
class Dict(dict):
    """

    >>> d1 = Dict()
    >>> d1['x'] = 100
    >>> d1.x
    100
    >>> d1.y = 200
    >>> d1['y']
    200
    >>> d2 = Dict(a=1, b=2, c='3')
    >>> d2.c
    '3'
    >>> d2['empty']
    Traceback (most recent call last):  
        ...
    KeyError: 'empty'
    >>> d2.empty
    Traceback (most recent call last):
        ...
    AttributeError: 'Dict' object has no attribute 'empty'
    """
    def __init__(self, **kw):
        super(Dict, self).__init__(**kw)

    def __getattr__(self, key):
        try:
            return self[key]
        except KeyError:
            raise AttributeError(r"'Dict' object has no attribute '%s'" % key)

    def __setattr__(self, key, value):
        self[key] = value

if __name__ == '__main__':
    import doctest
    doctest.testmod()
```

输出结果：  如果没有报错就说明 测试通过 

```
Testing started at 15:31 ...
D:\工作软件\python\pythonw.exe "D:\PyCharm 2018.2\helpers\pycharm\docrunner.py" D:/工作资料/pythonTestWorkSpace/doc_test/mydict2.py

D:\PyCharm 2018.2\helpers\pycharm\docrunner.py:1: DeprecationWarning: the imp module is deprecated in favour of importlib; see the module's documentation for alternative uses
  import imp

Process finished with exit code 0
```

我们删除__getattr__函数再执行

```
Testing started at 15:32 ...
D:\工作软件\python\pythonw.exe "D:\PyCharm 2018.2\helpers\pycharm\docrunner.py" D:/工作资料/pythonTestWorkSpace/doc_test/mydict2.py
D:\PyCharm 2018.2\helpers\pycharm\docrunner.py:1: DeprecationWarning: the imp module is deprecated in favour of importlib; see the module's documentation for alternative uses
  import imp

Error
**********************************************************************
File "D:/工作资料/pythonTestWorkSpace/doc_test/mydict2.py", line 7, in mydict2.Dict
Failed example:
    d1.x
Exception raised:
    Traceback (most recent call last):
      File "D:\PyCharm 2018.2\helpers\pycharm\docrunner.py", line 140, in __run
        compileflags, 1), test.globs)
      File "<doctest mydict2.Dict[2]>", line 1, in <module>
        d1.x
    AttributeError: 'Dict' object has no attribute 'x'

Error
**********************************************************************
File "D:/工作资料/pythonTestWorkSpace/doc_test/mydict2.py", line 13, in mydict2.Dict
Failed example:
    d2.c
Exception raised:
    Traceback (most recent call last):
      File "D:\PyCharm 2018.2\helpers\pycharm\docrunner.py", line 140, in __run
        compileflags, 1), test.globs)
      File "<doctest mydict2.Dict[6]>", line 1, in <module>
        d2.c
    AttributeError: 'Dict' object has no attribute 'c'

Process finished with exit code 0
```

可以看出上面有两个错误，一个是 执行d1.x 的时候，告诉我们没有 x 这个属性,另一个是执行d2.c，告诉我们没有 c 这个属性，其实确实是没有，但是我们之前不是有_getattr__函数么，这个函数的意思就是当获取不存在的属性的时候会执行这个函数，我们通过这个函数给了指定的返回值，所以之前不会报错，删除掉__getattr__以后 文档测试就没有通过。

