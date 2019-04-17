# Python实现inpolygon函数 - DumpDoctorWang的博客 - CSDN博客





2019年02月27日 18:42:58[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：90








使用Python实现MATLAB中inpolygon函数。

## 1、参考链接

[http://blog.sina.com.cn/s/blog_70012f010102xnel.html](http://blog.sina.com.cn/s/blog_70012f010102xnel.html)

## 2、代码实现

```python
# coding=utf8
import numpy as np
from matplotlib.path import Path

def inpolygon(xq, yq, xv, yv):
    """
    reimplement inpolygon in matlab
    :type xq: np.ndarray
    :type yq: np.ndarray
    :type xv: np.ndarray
    :type yv: np.ndarray
    """
    # 合并xv和yv为顶点数组
    vertices = np.vstack((xv, yv)).T
    # 定义Path对象
    path = Path(vertices)
    # 把xq和yq合并为test_points
    test_points = np.hstack([xq.reshape(xq.size, -1), yq.reshape(yq.size, -1)])
    # 得到一个test_points是否严格在path内的mask，是bool值数组
    _in = path.contains_points(test_points)
    # 得到一个test_points是否在path内部或者在路径上的mask
    _in_on = path.contains_points(test_points, radius=-1e-10)
    # 得到一个test_points是否在path路径上的mask
    _on = _in ^ _in_on
    
    return _in_on, _on
```

## 3、测试代码

```python
if __name__ == '__main__':
    xv = np.array([-4, 0, 4, 0])
    yv = np.array([0, 4, 0, -4])
    X = np.array([0, 1, 3.5, 4, 5])
    Y = np.array([0, 1, 0,   0, 0])

    _in, _on = inpolygon(X, Y, xv, yv)
    print _in, _on
```

X和Y中的值一一对应组成测试点，xv和yv中的值一一对应组成闭合路径。



