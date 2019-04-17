# matplotlib4 --设置line的属性， color，linestyle， linewidth， marker等 - 墨流觞的博客 - CSDN博客





2018年11月24日 11:10:54[墨氲](https://me.csdn.net/dss_dssssd)阅读数：907
所属专栏：[matplotlib一直在入门](https://blog.csdn.net/column/details/30379.html)









### 设置线属性

##### 1. 使用关键字参数
- 
利用fmt参数更改color， marker， 和line

`fmt=''[color][marker][line]`

三个中每一个都是可选的。

color：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018112410272079.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
Markers：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181124102755382.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
linestyle：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181124102818327.png)
先创建x，y数据：

```python
x = np.linspace(-np.pi, np.pi, 128,endpoint=True)
cosx,sinx,x_3 = np.cos(x), np.sin(x), x / 3
```

```python
x2 = np.linspace(-np.pi, np.pi, 32, endpoint=True)

fig = plt.figure(1)
axes0 = plt.subplot(211)
axes0.plot(x2, np.cos(x2), 'm*', x, cosx, 'k')

axes1 = plt.subplot(212)
axes1.plot(x, sinx, 'c-.')
plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181124104111394.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
- linewidth 设置线宽

```python
fig = plt.figure(1)
axes0 = plt.subplot(211)
axes0.plot(x, cosx, 'r', linewidth=2.5)

axes1 = plt.subplot(212)
axes1.plot(x, sinx, 'r', linewidth=1)
```

由于使用关键字参数，不能对一个Axes中多条曲线同时在一个plot函数中linewith设置线宽，否则报错：**`positional argument follows keyword argument`**

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181124104910492.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

但是可以通过使用多个plot函数绘制不同的曲线，然后设置相应线的属性

```python
fig = plt.figure(1)
axes0 = plt.subplot(211)
axes0.plot(x, cosx, 'k',linewidth=1)
axes0.plot(x, sinx, 'r', linewidth=2.5)
axes1 = plt.subplot(212)
axes1.plot(x, sinx, 'r', linewidth=1)
axes1.plot(x, x_3, 'c', linewidth=2.5)


plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181124105303526.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

##### 2.  使用Line2D的set_*方法设置属性

plot函数返回Line2D对象，可以使用该对象的set_*方法设置相应的属性，**有几条线就返回几个对象的列表**

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181124105704273.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

只需要使用`set_property`将想要改变的属性加在`set_`后面即可

```python
fig = plt.figure(1)
axes0 = plt.subplot(211)
line1, line2 = axes0.plot(x, cosx, 'k',x, x_3, 'c')

line1.set_linewidth(2.5)
line2.set_linewidth(1)


plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181124105905816.png)

##### 3.setp()方法

`plt.step(Line2D, **kwargs)`

将plot返回的Line2D对象传入第一个参数，然后使用关键字参数指定曲线的属性

注意这个地方使用的`plt.setp`

```python
fig = plt.figure(1)
axes0 = plt.subplot(211)
line1, line2 = axes0.plot(x, cosx, 'k',x, x_3, 'c')

plt.setp(line1, linewidth=2.5)
plt.setp(line2, linewidth=1)


plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181124110732776.png)







