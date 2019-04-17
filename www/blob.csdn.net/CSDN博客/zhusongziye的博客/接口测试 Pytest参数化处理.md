# 接口测试 Pytest参数化处理 - zhusongziye的博客 - CSDN博客





2018年04月11日 20:50:07[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：3342








### pytest的参数化方式
- 
pytest.fixture()方式进行参数化，fixture装饰的函数可以作为参数传入其他函数

- 
conftest.py 文件中存放参数化函数，可作用于模块内的所有测试用例

- 
pytest.mark.parametrize()方式进行参数化





本节测试依然以is_leap_year.py方法作为测试目标：




```
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

### pytest.fixture()




fixture是pytest的闪光点，在pytest中fixture的功能很多，本节主要介绍用fixture的参数化功能。
- 
pytest.fixture()中传入的参数为list，用例执行时，遍历list中的值，每传入一次值，则相当于执行一次用例。




- 
ps：@pytest.fixture()装饰的函数中，传入了一个参数为request，试试改成其他的会出现什么情况。




- 
这里的测试数据是直接存在list中的，能否存入json文件或者xml文件再进行读取转换为list呢?




![](https://img-blog.csdn.net/20180411204843174?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

fixture_param.png





#### 测试数据和用例分离



- 
参数化数据和用例怎么进行分离呢？可以采用conftest.py文件存储参数化数据和函数，模块下的用例执行时，会自动读取conftest.py文件中的数据





```
# conftest.py 记住 他叫conftest.py

import pytest

# 准备测试数据
is_leap = [4, 40, 400, 800, 1996, 2996]
is_not_leap = [1, 100, 500, 1000, 1999, 3000]
is_valueerror = [0, -4, -100, -400, -1996, -2000]
is_typeerror = ['-4', '4', '100', 'ins', '**', '中文']

# params中需要传入list
@pytest.fixture(params=is_leap)

def is_leap_y(request):
    return request.param@pytest.fixture(params=is_typeerror)

def is_type_error(request):
    return request.param
```
- 
测试用例文件：


```
# test_para.py

import sys
sys.path.append('.')
import is_leap_year
import pytest
class TestPara():
    def test_is_leap(self, is_leap_y):
        assert is_leap_year.is_leap_year(is_leap_y) == True

    def test_is_typeerror(self, is_type_error):
        with pytest.raises(TypeError):
            is_leap_year.is_leap_year(is_type_error)
```
- 
测试结果：

`PS E:\python_interface_test\requests_practice> pytest -q .\test_para.py............                                                             [100%]12 passed in 0.03 seconds`
### pytest.mark.parametrize()方式进行参数化
- 
采用标记函数参数化，传入单个参数，pytest.mark.parametrize("参数名"，lists）




![](https://img-blog.csdn.net/20180411204900318?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

mark.png

- 
采用标记函数传入多个参数，如pytest.mark.parametrize("para1, para2", [(p1_data_0, p2_data_0), (p1_data_1, p2_data_1),...]

- 
测试用例中传入2个参数，year和期望结果，使输入数据与预期结果对应，构造了2组会失败的数据，在执行结果中，可以看到失败原因：


![](https://img-blog.csdn.net/20180411204925225?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

image.png

```
import sys
sys.path.append('.')
import is_leap_year
import pytest

class TestPara():

    # 参数传入year中    
    @pytest.mark.parametrize('year, expected', 
    [(1, False), 
    (4, True), 
    (100, False), 
    (400, True), 
    (500, True)])
    def test_is_leap(self, year, expected):
        assert is_leap_year.is_leap_year(year) == expected    
    
    @pytest.mark.parametrize('year, expected', 
    [(0, ValueError), 
    ('-4', TypeError), 
    (-4, ValueError), 
    ('ss', TypeError), 
    (100, ValueError)])
    def test_is_typeerror(self, year,expected):
        if expected == ValueError:
            with pytest.raises(ValueError) as excinfo:
                is_leap_year.is_leap_year(year)        
                
            assert excinfo.type == expected        
        else:
            with pytest.raises(TypeError) as excinfo:
                is_leap_year.is_leap_year(year)
            
            assert excinfo.type == expected
```





