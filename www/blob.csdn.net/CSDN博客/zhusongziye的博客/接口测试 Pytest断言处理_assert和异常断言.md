# 接口测试 Pytest断言处理_assert和异常断言 - zhusongziye的博客 - CSDN博客





2018年04月08日 20:46:36[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：734








### Pytest的断言方式及应用场景
- 
使用assert语句

- 
断言预期的异常

- 
断言预期的告警

- 
利用上下文信息进行断言

- 
自定义断言方式


### 使用assert语句进行断言
- 
pytest允许使用python的标准assert语句进行断言处理

- 
采用assert断言时，可添加备注信息，当断言失败时，备注信息会以assertionerror抛出，并在控制台输出





```
import requests

class TestAssert():

    def test_assert(self):
        r = requests.get('http://www.baidu.com')
        assert r.status_code == 100, "返回200说明访问成功"
```
- 
输出信息





![](https://img-blog.csdn.net/20180408204457921?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




### 断言预期的异常
- 
在测试过程中，对某些方法进行测试时，预测输入某些特定数据，会抛出特定异常，若出现特定异常，则用例执行通过。

- 
对这类特定异常的断言，可以采用pytest中的pytest.raises()进行处理。
以下示例对一个判断是否为闰年的方法进行测试：


```
# is_leap_year.py

def is_leap_year(year):
    # 先判断year是不是整型
    if isinstance(year, int) is not True:        
        raise TypeError("传入的参数不是整数")    
    elif year == 0:        
        raise ValueError("公元元年是从公元一年开始！！")    
    elif abs(year) != year:        
        raise ValueError("传入的参数不是正整数")    
    elif (year % 4 ==0 and year % 100 != 0) or year % 400 == 0:
        print("%d年是闰年" % year)        
        return True
    else:
        print("%d年不是闰年" % year)        
        return False
```

##### 1、直接用pytest.raises()处理异常

```
import sys
sys.path.append(".")

import requests
import pytest
import is_leap_year

class TestAssert():
    # 对一个判断是否是闰年的方法进行测试
    def test_exception_typeerror(self):
        with pytest.raises(TypeError):
            is_leap_year.is_leap_year('ss')    
    
    def test_true(self):
        assert is_leap_year.is_leap_year(400) == True
```

运行结果：



`PS E:\python_interface_test\requests_practice> pytest -q .\test_assert.py..                                                                       [100%]2 passed in 0.31 seconds`
##### 2、将异常信息存储到一个变量中，变量的类型则为异常类，包含异常的type、value和traceback等信息

```
import sys
sys.path.append(".")

import requests
import pytest
import is_leap_year

class TestAssert():
    def test_exception_value(self):
        with pytest.raises(ValueError) as excinfo:
            is_leap_year.is_leap_year(0)        
        
        assert "从公元一年开始" in str(excinfo.value)        
        assert excinfo.type == ValueError
```

##### 3、可以在用例中定义抛出的异常信息是否与预期的异常信息匹配，若不匹配则用例执行失败

```
import sys
sys.path.append(".")

import requests
import pytest
import is_leap_year

class TestAssert():
    def test_exception_match(self):
        with pytest.raises(ValueError, match=r'公元33元年是从公元一年开始') as excinfo:
            is_leap_year.is_leap_year(0)        

        assert excinfo.type == ValueError
```

运行结果：




![](https://img-blog.csdn.net/20180408204523129?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




将match中的Pattern该为能够匹配的信息，则该用例能够执行成功。

##### 4、使用标记函数检查异常

> 
pytest.mark.xfail(raises=xx)


```
import sys
sys.path.append(".")

import requests
import pytest
import is_leap_year

class TestAssert():    
    @pytest.mark.xfail(raises=ValueError)
    def test_a(self):
        is_leap_year.is_leap_year(-100)
```

输出结果：




![](https://img-blog.csdn.net/20180408204544560?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




### 总结

pytest的断言方式非常简洁明确。本节主要介绍了对异常信息的断言，包括4种情况：
- 
直接断言，不添加assert语句

- 
将异常信息存储在变量中，再读取异常信息进行断言判断

- 
对异常的输出信息进行断言，异常类型、异常输出信息同时匹配成功，用例才能执行成功

- 
采用标记函数进行异常断言





> 
作者： 乐大爷


博客：https://www.jianshu.com/u/39cef8a56bf9

声明：本文转载，著作权归作者所有。




