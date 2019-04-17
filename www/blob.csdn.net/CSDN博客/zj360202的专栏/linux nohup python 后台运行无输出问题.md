# linux nohup python 后台运行无输出问题 - zj360202的专栏 - CSDN博客





2017年12月25日 16:55:44[zj360202](https://me.csdn.net/zj360202)阅读数：2119










```python
nohup python test.py &
nohup python test.py 2>&1 &
nohup python test.py > test.out 2>&1 &
```

上面三种方式无论在nohup.out 或test.out都看不到数据结果，这是因为python执行有缓存输出



**解决**



```python
nohup python -u test.py > test.out 2>&1 &
```




