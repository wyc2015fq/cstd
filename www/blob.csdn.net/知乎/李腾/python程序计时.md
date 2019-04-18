# python程序计时 - 知乎
# 

为了优化程序运行速度，很多时候需要对程序某些片段的运行进行计时。只要在目标程序片段的起始和结束处插入以下几行代码即可实现，简便灵活。

## 例程

```python
# 程序片段前插入以下两行
import time
time_start = time.time()

# 程序片段：此处为样例，可整体替换为你自己拟计时的程序片段
time.sleep(0.5)

# 程序片段后插入以下两行
time_end = time.time()
print('Time cost = %fs' % (time_end - time_start))
```

## 运行结果
![](https://pic4.zhimg.com/v2-16d7604670542106964e6c75b7671103_b.jpg)
## 参考

[python 计时程序运行时间 - Anlovescode - 博客园](https://link.zhihu.com/?target=https%3A//www.cnblogs.com/anloveslife/p/7729312.html)。代码基本照搬，加了一些注释

