# python np.random.seed直观程序理解 - 墨流觞的博客 - CSDN博客





2018年09月21日 17:23:39[墨氲](https://me.csdn.net/dss_dssssd)阅读数：226








`permutation(len)`返回`0~len`的一个全排列

## 第一个程序：

```python
```python
import numpy as np

# np.random.seed(42)
ls = np.random.permutation(10)
# np.random.seed(42)
ls_2 = np.random.permutation(10)
# np.random.seed(42)
# ls_add = np.random.permutation(14)
# print(ls, ls_2, ls_add, sep='\n')
print(ls, ls_2, sep='\n')
```
```

运行两次程序的输出：

> 
first:

[3 7 9 5 8 6 2 0 4 1]

[3 8 0 9 6 1 2 7 5 4]

second:

[6 7 4 9 3 5 2 1 8 0]

[0 8 4 9 7 6 2 3 5 1]
**两次程序完全不同，并且每次ls和ls_2的输出也不一样**

## 第二个程序：

**ls和ls_2的输出不同，但两次运行程序输出相同**

```python
```python
import numpy as np

np.random.seed(42)
ls = np.random.permutation(10)
# np.random.seed(42)
ls_2 = np.random.permutation(10)
# np.random.seed(42)
# ls_add = np.random.permutation(14)
# print(ls, ls_2, ls_add, sep='\n')
print(ls, ls_2, sep='\n')
```
```

运行两次程序的输出：

> 
first:

[8 1 5 0 7 2 9 4 3 6]

[0 1 8 5 3 4 7 9 6 2]

second:

[8 1 5 0 7 2 9 4 3 6]

[0 1 8 5 3 4 7 9 6 2]
## 第三个程序

**ls和ls_2的相同且两次程序运行输出也相同**

```python
```python
import numpy as np

np.random.seed(42)
ls = np.random.permutation(10)
np.random.seed(42)
ls_2 = np.random.permutation(10)
# np.random.seed(42)
# ls_add = np.random.permutation(14)
# print(ls, ls_2, ls_add, sep='\n')
print(ls, ls_2, sep='\n')
```
```

运行两次程序的输出：

> 
first:

[8 1 5 0 7 2 9 4 3 6]

[8 1 5 0 7 2 9 4 3 6]

second:

[8 1 5 0 7 2 9 4 3 6]

[8 1 5 0 7 2 9 4 3 6]
## 第四个程序

**如果permutation的全排列长度改变？seed还有用吗**

```python
```python
import numpy as np

np.random.seed(42)
ls = np.random.permutation(10)
np.random.seed(42)
ls_2 = np.random.permutation(10)
np.random.seed(42)
ls_add = np.random.permutation(14)
np.random.seed(42)
ls_add_2 = np.random.permutation(14)
# print(ls, ls_2, ls_add, sep='\n')
print(ls, ls_2,ls_add, ls_add_2, sep='\n')
```
```

两次运行程序输出：

> 
first:

[8 1 5 0 7 2 9 4 3 6]

[8 1 5 0 7 2 9 4 3 6]

[ 9 11  0 12  5  8  2  1 13  4  7 10  3  6]

[ 9 11  0 12  5  8  2  1 13  4  7 10  3  6]

second:

[8 1 5 0 7 2 9 4 3 6]

[8 1 5 0 7 2 9 4 3 6]

[ 9 11  0 12  5  8  2  1 13  4  7 10  3  6]

[ 9 11  0 12  5  8  2  1 13  4  7 10  3  6]










