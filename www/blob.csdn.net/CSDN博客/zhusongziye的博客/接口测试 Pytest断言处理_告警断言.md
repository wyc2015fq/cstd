# 接口测试 Pytest断言处理_告警断言 - zhusongziye的博客 - CSDN博客





2018年04月09日 20:12:57[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：542








pytest中对告警进行断言采用pytest.warns()方法，其断言的方法与pytest.raises()类似。pytest.warns()除了能断言告警外，还能够捕获告警信息，并对捕获的告警信息进行分类处理，可以设定出现特定告警信息时，则用例执行失败。

### Pytest的pytest.warns()用法
- 
pytest.warns()断言告警信息

- 
pytest.warns()捕获告警信息，并对告警信息进行判断

- 
pytest.warns()记录告警信息


#### 1、pytest.warns()断言告警信息

pytest.warns()断言，适用于被测方法会抛出特定告警，当出现这类告警则用例通过，否则用例失败。

###### 1.1采用pytest.warns()检查代码抛出的告警，如果没有对应的告警抛出，则用例失败.示例中设定了一个失败用例和一个成功用例：

```
# make_warnings.pyimport warnings

def fxn():
    warnings.warn("deprecated", DeprecationWarning)

def not_warn():
    pass
```

对上面的2个方法进行测试：

```
import sys
sys.path.append(".")

import pytest
import make_warnings

class TestWarns():
    def test_depre(self):
        with pytest.warns(DeprecationWarning):
            make_warnings.fxn()    
    
    def test_not_warn(self):
        with pytest.warns(DeprecationWarning):
            make_warnings.not_warn()
```

运行结果：




![](https://img-blog.csdn.net/20180409201228156?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




从运行结果中看出，第2个用例执行失败，是因为该方法不会抛出告警。从失败结果中`The list of emitted warnings is: []`看出，告警信息存储在list中。

###### 1.2 采用match正则匹配抛出的告警信息，若告警信息和告警类型不同时匹配，则用例执行失败

```
import warnings
import pytest

def warn_message():
    warnings.warn("I'm a warning message", UserWarning)

def test_warn_match():
    with pytest.warns(UserWarning, match=r'^I.*e'):
        warn_message()    

# warn_message() # 加上这句，执行用例会看到抛出的告警
```

###### 1.3 将告警信息存入一个变量中，通过读取这个变量中的信息进行断言，包括：告警的个数、告警信息参数等。

```
import warnings
import pytest

def warn_message():
    warnings.warn("user", UserWarning)
    warnings.warn("runtime", RuntimeWarning)

def test_warn_match():
    with pytest.warns(UserWarning, match=r'.*t.*') as record:
        warn_message()    
    
    assert len(record) == 2
    assert str(record[0].message) == "user"
    assert str(record[1].message) == "runtime"
```

运行结果：



![](https://img-blog.csdn.net/20180409201243647?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




从运行结果中，可以看到，返回的list中存储了2个warnings信息，即record是一个list，可以计算长度，并通过record[i].message获取告警信息。
PS：将示例中的`match=r'.*t.*'`更改为`match=r'.*u.*'`即可执行成功。

下面不是pytest.warns()的断言介绍了

### pytest捕获告警信息
- 
pytest默认捕获除DeprecationWarning和PendingDeprecationWarning外的所有告警信息,可以在pytest.ini中进行配置，使出现这两类告警时也会抛出告警信息：


```
# pytest.ini[pytest]
filterwarnings =
    once::DeprecationWarning
    once::PendingDeprecationWarning
```
- 
如果出现特定告警需要使用例执行失败，可以采用-W命令：`pytest -q test_show_warnings.py -W error::UserWarning`

- 
可以在pytest.ini中设置过滤或者执行失败,在这个过滤条件中，除UserWarning外的告警都会识别为错误进行处理。


```
# pytest.ini[pytest]
filterwarnings =
    error
    ignore::UserWarning
```
- 
pytest的标记函数处理告警信息


```
# 忽略function中的告警
@pytest.mark.filterwarnings('ignore:function')

# 将用例中所有的告警都转换为错误，将装饰器作用于测试类，则测试类中的所有用例出现告警都会失败
@pytest.mark.filterwarnings('error')
```
- 
不捕获告警信息，可以在文件中进行配置，或者在命令行传递`-p no:warnings`，那么当用例存在告警信息时，都不会在结果中输出告警信息：


```
# pytest.ini
[pytest]
addopts = -p no:warnings
```

### pytest记录告警信息
- 
可以采用recwarn fixture记录函数的全部告警信息
每个告警记录包含以下属性：

- 
message

- 
category

- 
filename

- 
lineno

- 
file

- 
line


每个告警记录具有list的属性，可调用以下方法：
- 
pop()

- 
clear()


```
import warningsdef test_hello(recwarn):
    warnings.warn("hello", UserWarning)
    assert len(recwarn) == 1
    w = recwarn.pop(UserWarning)
    assert issubclass(w.category, UserWarning)    
    assert str(w.message) == "hello"
    assert w.filename    assert w.lineno
```



