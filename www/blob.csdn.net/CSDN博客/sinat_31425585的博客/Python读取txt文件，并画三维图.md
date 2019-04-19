# Python读取txt文件，并画三维图 - sinat_31425585的博客 - CSDN博客
2016年08月29日 11:10:06[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：5018

```python
<span style="font-family: Arial, Helvetica, sans-serif;"><p>记忆力差的孩子得勤做笔记！</p><p>刚接触python，最近又需要画一个三维图，然后就找了一大堆资料，看的人头昏脑胀的，今天终于解决了！好了，废话不多说，直接上代码！</p></span>
```
```python
#由三个一维坐标画三维散点
```
```python
#coding:utf-8
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.axes3d import Axes3D
x = []
y = []
z = []
f = open("data\\record.txt")
line = f.readline()
while line:
	c,d,e = line.split()
	x.append(c)
	y.append(d)
	z.append(e)
	
	
	line = f.readline()
	
f.close()
#string型转int型
x = [ int( x ) for x in x if x ]
y = [ int( y ) for y in y if y ]
z = [ int( z ) for z in z if z ]
print x
fig=plt.figure()
ax=Axes3D(fig)
ax.scatter3D(x, y, z)
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')
plt.show()
```
最关键的步骤就是那个string类型转int类型，之前缺了这一步，死活的报错，好了，终于搞定！
```python
#画三维线
```
```python
#coding:utf-8
from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt
x = []
y = []
z = []
f = open("data\\record.txt")
line = f.readline()
while line:
	c,d,e = line.split()
	x.append(c)
	y.append(d)
	z.append(e)
	
	line = f.readline()
	
f.close()
#string型转int型
x = [ int( x ) for x in x if x ]
y = [ int( y ) for y in y if y ]
z = [ int( z ) for z in z if z ]
#print x
fig=plt.figure()
ax = fig.gca(projection = '3d')
ax.plot(x,y,z)
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')
plt.show()
```
