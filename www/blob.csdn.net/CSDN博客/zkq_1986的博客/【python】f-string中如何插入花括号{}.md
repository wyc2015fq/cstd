# 【python】f-string中如何插入花括号{} - zkq_1986的博客 - CSDN博客





2019年03月01日 09:29:02[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：99








用两个{{}}就能输出{}，f-string里不能用反斜杠

例：

```python
>>> a=1
>>> b=f'{a}{{}}'
>>> print(b)
1{}
```





