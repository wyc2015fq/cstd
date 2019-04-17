# 菜鸟学习python编程--杂记 (不定期更新) - Snoopy_Dream - CSDN博客





置顶2018年10月02日 10:29:43[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：192
所属专栏：[数据生成器+数据增广+常用工具](https://blog.csdn.net/column/details/29905.html)











## **目录**

[1.bool类型的形参+return 的合理使用](#1.bool%E7%B1%BB%E5%9E%8B%E7%9A%84%E5%BD%A2%E5%8F%82%2Breturn%20%E7%9A%84%E5%90%88%E7%90%86%E4%BD%BF%E7%94%A8)

[其实当i有明确含义时，如下for col in xrange(w)](#%C2%A0%20%C2%A0%E5%85%B6%E5%AE%9E%E5%BD%93i%E6%9C%89%E6%98%8E%E7%A1%AE%E5%90%AB%E4%B9%89%E6%97%B6%EF%BC%8C%E5%A6%82%E4%B8%8Bfor%20col%20in%20xrange(w))

[3.for与np的联合使用，](#3.for%E4%B8%8Enp%E7%9A%84%E8%81%94%E5%90%88%E4%BD%BF%E7%94%A8%EF%BC%8C)

[4. import copy](#4.%20import%20copy)

[5. np.clip(data,min,max)，](#5.%20np.clip(data%2Cmin%2Cmax)%EF%BC%8C)

[6. np.where 两种用法](#6.%20np.where)

[7. np.oneslike(x)](#7.%20np.oneslike(x))

[8. s_list = np.random.choice(self.schools_majors, batch_size)](#8.%20s_list%20%3D%20np.random.choice(self.schools_majors%2C%20batch_size))

[9. np.expand_dims(im, axis=0) n h w c](#9.%20np.expand_dims(im%2C%20axis%3D0)%20n%20h%20w%20c)

[10. .format](#10.%C2%A0.format)

[11. z.reshape(-1) 变成一行   z.reshape(-1，1)变成一列](#11.%20z.reshape(-1)%20%E5%8F%98%E6%88%90%E4%B8%80%E8%A1%8C%C2%A0%C2%A0%20z.reshape(-1%EF%BC%8C1)%E5%8F%98%E6%88%90%E4%B8%80%E5%88%97)

[12. continue遇到一种情形先跳过，这次循环](#12.%20continue%E9%81%87%E5%88%B0%E4%B8%80%E7%A7%8D%E6%83%85%E5%BD%A2%E5%85%88%E8%B7%B3%E8%BF%87%EF%BC%8C%E8%BF%99%E6%AC%A1%E5%BE%AA%E7%8E%AF)

[13.map 映射 map(表达式, 列表等) 不用for循环](#13.map%20%E6%98%A0%E5%B0%84%20map(%E8%A1%A8%E8%BE%BE%E5%BC%8F%2C%20%E5%88%97%E8%A1%A8%E7%AD%89)%20%E4%B8%8D%E7%94%A8for%E5%BE%AA%E7%8E%AF)

### 1.**bool**类型的**形参+return **的合理使用

  bool型的形参，**代表的是一种情况**，搭配return使用，一旦执行，return后面的语句不再执行。

  比如下面代表的是只去除左边的空白。    

2.总是习惯于for** i **in range这种，

###    其实当i有明确含义时，如下for **col**in xrange(w)

### 3.**for与np的联合使用**，

**   这样就不用两个for嵌套使用**

   例子：  ![](https://img-blog.csdn.net/20181002102910716?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

   如下：for col in xrange(w):

                   if(im[:,col,:]>240)**.all()**:如果全这一列全部都大于240，则左边索引+1

   备注：这里的all是np当中的，(im[:,col,:]>240)返回一个bool类型的ndarray，也可以**all(im[:,col,:]>240)**

**    当你脑海中想到每一个，的时候，可以考虑any all。如：一列当中每一行的元素都要小于240，表明这一列是空白的。**

```python
#############多的地方代表要学习的地方
def remove_zero_padding(image, only_left=False):.
'''OCR分割出性别后去除两边空白的区域'''
    h, w = image.shape[:2]
    index_left = 0
    index_right = w - 1
    for col in range(w):
        if (image[:, col] >= 240).all():
            index_left = col + 1
        else:
            break  
    if only_left:#############
        return image[:, index_left:]#############
    
    for col in range(w):##########
        if (image[:, w - col - 1] >= 240).all():#############
            index_right = w - col - 1
        else:
            break
    
    return image[:, index_left:index_right]
```

### 4. import copy

    out = copy.deepcopy(im) 重新重建一个np，和原先的没关系

    out = im.copy() 也可以，操作的时候如下

    e.g. im = remove_zero_padding(**im**.copy()[:, 5:-5])

### 5. **np.clip**(data,min,max)**，**

**小于min的都为min**，**大于max都变为max**

```python
x=np.array([1,2,3,5,6,7,8,9])
np.clip(x,3,8)
array([3, 3, 3, 5, 6, 7, 8, 8])
```

### 6. np.**where 两种用法**
- np.where(condition, x, y)

    满足条件(condition)，输出x，不满足输出y。image = np.where(image >= 240, 255,image)
- 
np.where(condition)


    a = np.array([2,4,6,8,10])

    a[np.where(a > 5)] # 等价于 a[a>5] array([ 6, 8, 10])

### 7. np.**oneslike**(x)

   返回一个用1填充的跟x 形状和类型 一致的数组。

### 8. s_list = np.random.choice(self.schools_majors, batch_size)

    #随机选取多少个学校或专业

### 9. np.expand_dims(im, axis=0) n h w c

### 10. .format

** 索引：**![](https://img-blog.csdn.net/20181002114354734?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) python3中  print("**{0}/{1}**".format(**i**,**100000**),end=**"\r"**)，{0}表示第0个，{1}表第一个。

```python
for i in range(100000):
    print("{0}/{1}".format(i,100000),end="\r")
```
`**精度：**print('**{:.3f}**'.format(123.45678)) 四舍五入`
```python
#类似字典
line = "{pic_id},{sex},{nation},{weight} kg,{blood_type},{native_place}".format(pic_id=pic_id,
                                                                                        sex=sex,
                                                                                        nation=nation,
                                                                                        weight=weight,
                                                                                        blood_type=blood_type,
                                                                                        native_place=native_place) + \
               "," + ",".join([school['gaozhong'], major['gaozhong'], xuewei['gaozhong'], time['gaozhong'], graduate['gaozhong']]) + \
               "," + ",".join([school['dazhuan'], major['dazhuan'], xuewei['dazhuan'], time['dazhuan'], graduate['dazhuan']]) + \
               "," + ",".join([school['benke'], major['benke'], xuewei['benke'], time['benke'], graduate['benke']]) + \
               "," + ",".join([school['yanjiusheng'], major['yanjiusheng'], xuewei['yanjiusheng'], time['yanjiusheng'], graduate['yanjiusheng']]) + \
               ",无" * 5        # 高中、专科、本科、研究生、其它（全是无）
```

11. z.reshape(-1) 变成一行   z.reshape(-1，1)变成一列

源码：x_image = tf.reshape(x, [-1, 1,28, 28])

这里是将一组图像矩阵x重建为新的矩阵，该新矩阵的维数为（a，1，28，28），其中-1表示a由实际情况来定。例如，x是一组图像的矩阵（假设是50张，大小为56×56），则执行

x_image = tf.reshape(x, [-1, 1,28, 28])  nchw

可以计算a=50×56×56/28/28/1=200。即x_image的维数为（200，1，28，28）。

### 12. continue遇到一种情形先跳过，这次循环

    跳过太白的图片

```python
if origin_im.mean() > 250:
    continue
```

### 13.map 映射 map(表达式, 列表等) 不用for循环

```python
>>> map(lambda x: x ** 2, [1, 2, 3, 4, 5])  # 使用 lambda 匿名函数
[1, 4, 9, 16, 25]

>>> map(lambda x, y: x + y, [1, 3, 5, 7, 9], [2, 4, 6, 8, 10])
[3, 7, 11, 15, 19]
```





