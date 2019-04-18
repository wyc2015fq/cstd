# Python计算圆周率的两种方法 - 心纯净，行致远 - CSDN博客





2019年02月23日 09:42:31[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：296








一种是数学公式：

![pi=\sum_{0}^{\infty }\left [ 1/16^{k}\left ( 4/(8k+1)-2/(8k+4)-1/(8k+5)-1/(8k+6) \right ) \right ]](https://private.codecogs.com/gif.latex?pi%3D%5Csum_%7B0%7D%5E%7B%5Cinfty%20%7D%5Cleft%20%5B%201/16%5E%7Bk%7D%5Cleft%20%28%204/%288k&plus;1%29-2/%288k&plus;4%29-1/%288k&plus;5%29-1/%288k&plus;6%29%20%5Cright%20%29%20%5Cright%20%5D)

code:

```python
#CalPiV1.py
pi=0
N=100
for i in range(N):
    pi+=1/pow(16,i)*(4/(8*i+1)-\
        2/(8*i+4)-1/(8*i+5)-1/(8*i+6))
print(pi)
```

第二种是使用蒙特卡罗方法：

code:

```python
#CalPiV2.py
import random
import time
N=1000*1000
k=0
start=time.perf_counter()
for i in range(N):
    x,y=random.random(),random.random()
    dist=pow(x**2+y**2,0.5)
    if dist<=1.0:
        k+=1
pi=4*(k/N)
print("pi={},run time={:.2f}s".format(pi,time.perf_counter()-start))
```





