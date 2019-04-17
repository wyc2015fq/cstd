# 深度学习过程中  常用 python 使用总结 - Snoopy_Dream - CSDN博客





置顶2018年06月22日 15:50:34[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：217
所属专栏：[深度学习caffe常用python代码分享](https://blog.csdn.net/column/details/22599.html)









**目录**

[-8. for循环数据的时候](#-8.%20for%E5%BE%AA%E7%8E%AF%E6%95%B0%E6%8D%AE%E7%9A%84%E6%97%B6%E5%80%99)

[-7.all() 下面是ocr里利用竖直投影裁剪下一张表格，取出掉两旁的空白区域。](#-7.all()%20%E4%B8%8B%E9%9D%A2%E6%98%AFocr%E9%87%8C%E5%88%A9%E7%94%A8%E7%AB%96%E7%9B%B4%E6%8A%95%E5%BD%B1%E8%A3%81%E5%89%AA%E4%B8%8B%E4%B8%80%E5%BC%A0%E8%A1%A8%E6%A0%BC%EF%BC%8C%E5%8F%96%E5%87%BA%E6%8E%89%E4%B8%A4%E6%97%81%E7%9A%84%E7%A9%BA%E7%99%BD%E5%8C%BA%E5%9F%9F%E3%80%82)

[-6.倒序查找字典中，第一个[0]满足条件的值](#-6.%E5%80%92%E5%BA%8F%E6%9F%A5%E6%89%BE%E5%AD%97%E5%85%B8%E4%B8%AD%EF%BC%8C%E7%AC%AC%E4%B8%80%E4%B8%AA%5B0%5D%E6%BB%A1%E8%B6%B3%E6%9D%A1%E4%BB%B6%E7%9A%84%E5%80%BC)

[-5.  enumerate的使用，统计循环次序](#-5.%C2%A0%20enumerate%E7%9A%84%E4%BD%BF%E7%94%A8%EF%BC%8C%E7%BB%9F%E8%AE%A1%E5%BE%AA%E7%8E%AF%E6%AC%A1%E5%BA%8F)

[-4.    np.where的使用，以及zip的实际使用](#-4.%C2%A0%C2%A0%C2%A0%20np.where%E7%9A%84%E4%BD%BF%E7%94%A8%EF%BC%8C%E4%BB%A5%E5%8F%8Azip%E7%9A%84%E5%AE%9E%E9%99%85%E4%BD%BF%E7%94%A8)

[-3.  outline = str(imgname) + '  '  + '  '.join(map(str, bbox))，box是列表，最终形式是x1 y1 x2 y2](#-3.%C2%A0%20outline%20%3D%20str(imgname)%20%2B%20' rel=)

[-2.pass 不做任何事情，一般用做占位语句。](#-2.pass%20%E4%B8%8D%E5%81%9A%E4%BB%BB%E4%BD%95%E4%BA%8B%E6%83%85%EF%BC%8C%E4%B8%80%E8%88%AC%E7%94%A8%E5%81%9A%E5%8D%A0%E4%BD%8D%E8%AF%AD%E5%8F%A5%E3%80%82)

[-1.列表操作](#-1.%E5%88%97%E8%A1%A8%E6%93%8D%E4%BD%9C)

[0.一行写好if else](#%E4%B8%80%E8%A1%8C%E5%86%99%E5%A5%BDif%20else)



[0.列表生成器](#0.%E5%88%97%E8%A1%A8%E7%94%9F%E6%88%90%E5%99%A8)

[1.对每一行处理的时候因为\n的存在，使用[ ].strip( )](#1.%E5%AF%B9%E6%AF%8F%E4%B8%80%E8%A1%8C%E5%A4%84%E7%90%86%E7%9A%84%E6%97%B6%E5%80%99%E5%9B%A0%E4%B8%BA%5Cn%E7%9A%84%E5%AD%98%E5%9C%A8%EF%BC%8C%E4%BD%BF%E7%94%A8%5B%20%5D.strip(%20))

[2.字典的创建](#2.%E5%AD%97%E5%85%B8%E7%9A%84%E5%88%9B%E5%BB%BA)

[3.字典的遍历（将文本写入字典中进行操作）](#3.%E5%AD%97%E5%85%B8%E7%9A%84%E9%81%8D%E5%8E%86%EF%BC%88%E5%B0%86%E6%96%87%E6%9C%AC%E5%86%99%E5%85%A5%E5%AD%97%E5%85%B8%E4%B8%AD%E8%BF%9B%E8%A1%8C%E6%93%8D%E4%BD%9C%EF%BC%89)

[numpy的学习](#numpy%E7%9A%84%E5%AD%A6%E4%B9%A0)

[1.e.shape( )](#1.e.shape(%20))

[2.排序numpy.argsort，学会使用keep，字典的学习](#2.%E6%8E%92%E5%BA%8Fnumpy.argsort%EF%BC%8C%E5%AD%A6%E4%BC%9A%E4%BD%BF%E7%94%A8keep%EF%BC%8C%E5%AD%97%E5%85%B8%E7%9A%84%E5%AD%A6%E4%B9%A0)

[sorted(iterable，cmp，key，reverse）](#sorted(iterable%EF%BC%8Ccmp%EF%BC%8Ckey%EF%BC%8Creverse%EF%BC%89)

[3.where](#3.where)

[Numpy计算(重要)](#Numpy%E8%AE%A1%E7%AE%97(%E9%87%8D%E8%A6%81))

[条件运算](#%E6%9D%A1%E4%BB%B6%E8%BF%90%E7%AE%97)

[统计运算](#%E7%BB%9F%E8%AE%A1%E8%BF%90%E7%AE%97)

[数组运算](#%E6%95%B0%E7%BB%84%E8%BF%90%E7%AE%97)

[矩阵运算np.dot()(非常重要)](#%E7%9F%A9%E9%98%B5%E8%BF%90%E7%AE%97np.dot()(%E9%9D%9E%E5%B8%B8%E9%87%8D%E8%A6%81))

[矩阵拼接](#%E7%9F%A9%E9%98%B5%E6%8B%BC%E6%8E%A5)

[Numpy读取数据np.genfromtxt](#Numpy%E8%AF%BB%E5%8F%96%E6%95%B0%E6%8D%AEnp.genfromtxt)

### -8. for循环数据的时候

```python
for i in tqdm(range(int(math.ceil(count/16.0)))):
###不用管这里到底能不能整除，你进行向上取整就好
    onehot = np.zeros((16,17),dtype=int)
    images=mean_std(AddFeatures(np.concatenate((test_sen1[i*16:(i+1)*16,:,:,:],test_sen2[i*16:(i+1)*16,:,:,:]),axis=3)))#列表：可以自动识别是否到了最后
    inputs = Variable((torch.from_numpy(images)).float().permute(0, 3, 1, 2).cuda())
    print model_best
    outputs = model_best(inputs)
    scores, preds = torch.max(outputs.data, 1)#preds
    for i in range(inputs.shape[0]):#注意这里不能用batchsize，只能用列表[:]后的shape
        onehot[i,preds[i]]=1
        f.writelines(','.join(map(str,onehot[i]))+'\n')
```

### -7.**all() 下面是ocr里利用竖直投影裁剪下一张表格，取出掉两旁的空白区域。**

```python
def remove_zero_padding(image):
    h, w, c = image.shape
    index_left = 0
    index_right = w - 1
    for col in range(w):
        if (image[:, col] == 255).all():
            index_left = col + 1
        else:
            break
    
    for col in range(w):
        if (image[:, w - col - 1] == 255).all():
            index_right = w - col - 1
        else:
            break
    
    return image[:, index_left:index_right]
```



### -6.倒序查找字典中，第一个[0]满足条件的值

```python
right = sorted(range(len(temp_y2)-1,len(temp_y2)-1-11,-1),key=lambda k :temp_y2[k],reverse=True)[0]
```

```python
def hori_proj(img):
    """水平投影"""
    img = local_threshold(img)
    w,h= img.shape[1],img.shape[0]
    count_dict_hori = {x: 0 for x in range(h)}
    for i in range(h):
        for j in range(w):
            #统计hei色像素数目
            if img[i][j]==0:
                count_dict_hori[i]=count_dict_hori[i]+1
    temp_x2 = list(count_dict_hori.keys())
    temp_y2 = list(count_dict_hori.values())      
    plt.title('hori') 
    plt.plot(temp_y2,temp_x2,c='blue') 
    top_max=max(temp_y2[0:35]) 
    down_max=max(temp_y2[35:62])
    if max(temp_y2[0:35])>0.3*w:
        top = sorted(range(35),key=lambda k :temp_y2[k],reverse=True)[0]
    else:
        top = 0
    if max(temp_y2[35:62])>0.3*w:
        down = sorted(range(len(temp_y2)-1,len(temp_y2)-1-27,-1),key=lambda k :temp_y2[k],reverse=True)[0]
    else:
        down = h
    return top,down,top_max,down_max
```

### **-5.  enumerate的使用，统计循环次序**

```python
for i, name in enumerate(jpgfiles):
        if i % 1000 == 0:
            print("\nprocessing: %.5i of %s" % (i, len(jpgfiles)))
```



```python
for i, box in enumerate(out_boxes):
            f.write(str(box[0]) + "_" + str(box[1]) + "_" + str(box[2]) + "_" + str(box[3]))
            if i != out_boxes.shape[0] - 1:
                f.write(";")
        f.write("\n")
```

### **-4.    np.where的使用，以及zip的实际使用**

```python
out_boxes = boxes[np.where(scores >= 0.45)]
        out_scores = scores[np.where(scores >= 0.45)]
        out_boxes = out_boxes.astype(np.int32)
        # (x1, y1, x2, y2) -> (x1, y1, w, h)
        out_boxes[:, 2] = out_boxes[:, 2] - out_boxes[:, 0]
        out_boxes[:, 3] = out_boxes[:, 3] - out_boxes[:, 1]

        f.write(os.path.basename(name) + ",")
        # 防止检测结果为空
        if out_boxes.shape[0] == 0:
            f.write("0_0_1_1_0")
        for box, score, i in zip(out_boxes, out_scores, range(out_boxes.shape[0])):
            f.write(str(box[0]) + "_" + str(box[1]) + "_" + str(box[2]) + "_" + str(box[3]) + "_" + str(score))
            if i != out_boxes.shape[0] - 1:
                f.write(";")
        f.write("\n")
```

result =** np.where(cond,xarr,yarr)**

当**符合条件时是x，不符合是y，常用于根据一个数组产生另一个新的数组。**

栗子：假设有一个随机数生成的矩阵，希望将所有正值替换为2，负值替换为-2

arr = np.random.randn(4,4)

arr

np.where(arr>0,2,-2)

### **-3.  outline = str(imgname) + '  '  + '  '.join(map(str, bbox))，box是列表，最终形式是x1 y1 x2 y2**

```python
for i in tqdm(range(int(math.ceil(count/16.0)))):
    onehot = np.zeros((16,17),dtype=int)
    images=mean_std(np.concatenate((test_sen1[i*16:(i+1)*16,:,:,:],test_sen2[i*16:(i+1)*16,:,:,:]),axis=3))
    inputs = Variable((torch.from_numpy(images)).float().permute(0, 3, 1, 2).cuda())
    outputs = model_best(inputs)
    _, preds = torch.max(outputs.data, 1)
    for i in range(inputs.shape[0]):
        onehot[i,preds[i]]=1
        f.writelines(','.join(map(str,onehot[i]))+'\n')
```

### **-2.pass 不做任何事情，一般用做占位语句。**

### **-1.列表操作**

**del list1[2]删除某元素**


|1|[list.pop([index=-1])](http://www.runoob.com/python/att-list-pop.html)			移除列表中的一个元素（默认最后一个元素），并且返回该元素的值|
|----|----|
|2|[list.remove(obj)](http://www.runoob.com/python/att-list-remove.html)			移除列表中某个值的第一个匹配项|

### 0.一行写好if else

```python
residual=x if self.right is None else self.right(x)
```



### 0.列表生成器

for循环**后面**还可以**加上if判断**，这样我们就可以筛选出仅偶数的平方：

```python
[x * x for x in range(1, 11) if x % 2 == 0]
得到[4, 16, 36, 64, 100]
```

还可以使用**双循环**，可以生成全排列：

```python
[m + n for m in 'ABC' for n in 'XYZ']
得到['AX', 'AY', 'AZ', 'BX', 'BY', 'BZ', 'CX', 'CY', 'CZ']
```

```python
d = {'x': 'A', 'y': 'B', 'z': 'C' }
[k + '=' + v for k, v in d.iteritems()]
得到['y=B', 'x=A', 'z=C']
```

### 1.对每一行处理的时候因为\n的存在，使用**[ ].strip( )**

```python
for box in box_ori:
        box_new = box.strip() + '_0.8'=
```

### **2.字典的创建**

```python
#1.创造空字典a={}
#2.创造空value的key,a[key1]=[]
#3.增加每一个key的value，a[key1].append()
```

```python
# for x in dic x 每次返回的也都是key
#快捷创建dic ,不用建造空字典
nameboxnmsdict = {x: [] for x in nameboxdict}
#快捷创建列表操作，不同[]然后.append（）,strip的意思是去除前后的/t /n空格什么的
splitlines = [x.strip().split(' ') for x in lines]
#相当于splitlines = []
      for x in lines:
          splitlines.append(x.strip().split(' '))
```

### **3.字典的遍历（将文本写入字典中进行操作）**

默认情况下，**dict迭代的是key。**

如果要迭代value，可以用**for value in d.itervalues()，**

如果要同时迭代key和value，**可以用for k, v in d.iteritems()。**

举例：由图1变成图2，将**同一个图片中**（dic）的检测物放到一起。

**![](https://img-blog.csdn.net/20180622161913506?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**

**![](https://img-blog.csdn.net/20180622161939825?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**

```python
f1=open('/home/ygx/666/nms之后/merge_xywh.csv','r')
f2=open('/home/ygx/666/nms之后/提交格式.csv','w')
f2.writelines('name,coordiate\n')
lines = f1.readlines()
nameboxdict = {}
for line in lines:   
    oriname=line.strip().split(' ')[0]
    det = line.strip().split(' ')[1:5]#用于存放detp[x y w h]
    if (oriname not in nameboxdict):#如果字典中没有，就创造，注意是a[key]=[]
        nameboxdict[oriname] = []        
    nameboxdict[oriname].append(det)
for k,v in nameboxdict.items():
    f2.writelines(k+',')
    lenv=len(v)
    for i in range(lenv):
        f2.writelines(v[i][0]+'_'+v[i][1]+'_'+v[i][2]+'_'+v[i][3])
        if i==lenv-1:
            f2.writelines('\n')
        else:
            f2.writelines(';')
```

----------------------------------------------------------------------------------------------

## **numpy的学习**

以前总是直觉使用列表，后期要多转到numpy操作会高效很多

----------------------------------------------------------------------------------------------

### 1.e.shape( )



```python
nd.array.size
```

### 2.排序numpy.argsort，学会使用keep，字典的学习

```python
def py_cpu_nms(dets, thresh):
    """Pure Python NMS baseline."""
    #print('dets:', dets)
    x1 = dets[:, 0]
    y1 = dets[:, 1]
    x2 = dets[:, 2]
    y2 = dets[:, 3]
    scores = dets[:, 4]
    areas = (x2 - x1 + 1) * (y2 - y1 + 1)
    ## index for dets
    order = scores.argsort()[::-1]#np.argsort()[::-1]表示倒序排放，存放的是索引
    keep = []
    while order.size > 0:#注意这里对数组nd.array.size
        i = order[0]
        keep.append(i)
        xx1 = np.maximum(x1[i], x1[order[1:]])#逐位取最大值
        yy1 = np.maximum(y1[i], y1[order[1:]])
        xx2 = np.minimum(x2[i], x2[order[1:]])
        yy2 = np.minimum(y2[i], y2[order[1:]])

        w = np.maximum(0.0, xx2 - xx1 + 1)
        h = np.maximum(0.0, yy2 - yy1 + 1)
        inter = w * h
        ovr = inter / (areas[i] + areas[order[1:]] - inter)

        inds = np.where(ovr <= thresh)[0]
        #print (areas[i] + areas[order[1:]] - inter)
        order = order[inds + 1]
    return keep

def nmsbynamedict(nameboxdict, nms,thresh):
    nameboxnmsdict = {x: [] for x in nameboxdict}#nameboxnmsdict表示存放nms之后的字典，现根据名称创建空字典
    for imgname in nameboxdict:【遍历字典】
        #print('type nms:', type(nms))【type（ ）查看变量类型】
        keep = nms(np.array(nameboxdict[imgname]),thresh)#注意这里变成了将字典中的value变成了np格式，返回一张图imgname的nms之后的索引
        #print('keep:', keep)
        outdets = []#新建列表。dic的value的创建，用于存放nms之后的dets。
        #print('nameboxdict[imgname]: ', nameboxnmsdict[imgname])#打印这张图nms之后的value，此处应该是【】空
        for index in keep:#【】循环这个索引，将keep作为保留
            # print('index:', index)
            outdets.append(nameboxdict[imgname][index])#将keep循环作为nameboxdic的保留，新增列表.append
        nameboxnmsdict[imgname] = outdets#将append后的列表放入字典当中
    return nameboxnmsdict
```

...读入文本，将其转换到字典操作中


` `
```python
nameboxdict = {}
for line in lines:   
    oriname=line.strip().split(' ')[0]
    det = line.strip().split(' ')[1:5]#用于存放detp[x y w h]
    if (oriname not in nameboxdict):#如果字典中没有，就创造，注意是a[key]=[]
        nameboxdict[oriname] = []    #如果没有就新建key   
    nameboxdict[oriname].append(det)#如果有就append
```

总结：上面的例子，一张检测的图片，检测出多个目标，需要对每张图片进行nms操作，就把imgname作为dic的key，把

det_boxes以列表的形式从文本读出，作为dic的value添加到对应的key当中进行操作。



题外话：排序，只是我经常用到的。

### **sorted(iterable，cmp，key，reverse）**

用operator.itemgette**r****函数排序 只是一个操作函数**，表示对第几个进行操作

```python
>>> from operator import itemgetter

>>> sorted(list1, key=itemgetter(1))【按照第二个排序】[('sara', 80), ('david', 90), ('mary', 90), ('lily', 95)]

>>> sorted(list1, key=itemgetter(0))【按照第一个排序】[('david', 90), ('lily', 95), ('mary', 90), ('sara', 80)]
```

### **3.where**



```python
x = np.arange(16)
print(x[np.where(x>5)])
#输出：(array([ 6,  7,  8,  9, 10, 11, 12, 13, 14, 15], dtype=int64),)
x = np.arange(16).reshape(-1,4)
print(np.where(x>5))

#(array([1, 1, 2, 2, 2, 2, 3, 3, 3, 3], dtype=int64), array([2, 3, 0, 1, 2, 3, 0, 1, 2, 3], dtype=int64))
#注意这里是坐标是前面的一维的坐标，后面是二维的坐标
```





以下来自 链接：https://www.jianshu.com/p/83c8ef18a1e8



### Numpy计算(重要)

### 条件运算

> 
![](https://upload-images.jianshu.io/upload_images/3203841-96512a0877ab0be0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/558)


> 
![](https://upload-images.jianshu.io/upload_images/3203841-9e4a2042696fe40f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)


> 
![](https://upload-images.jianshu.io/upload_images/3203841-4bad94aa37f22c07.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)


## 统计运算
- 
指定轴最大值`amax`(数组，axis=0/1; 0表示列1表示行)


> 
![](https://upload-images.jianshu.io/upload_images/3203841-209d2b8d83e9291a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)



- 
指定轴最小值`amin`


> 
![](https://upload-images.jianshu.io/upload_images/3203841-0f034cd82426a780.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)

- 
指定轴平均值`mean`


> 
![](https://upload-images.jianshu.io/upload_images/3203841-02a0bff94816cdab.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)

- 
方差`std`


> 
![](https://upload-images.jianshu.io/upload_images/3203841-c985c2cde795e912.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)


## 数组运算
- 
数组与数的运算


> 
![](https://upload-images.jianshu.io/upload_images/3203841-2dbd6cdc79c07fb3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)


> 乘法

![](https://upload-images.jianshu.io/upload_images/3203841-685a27feb0c3a47c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)

- 
数组间也支持加减乘除运算,但基本用不到


> 
![](https://upload-images.jianshu.io/upload_images/3203841-c27afaef111daf23.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/679)




## 矩阵运算`np.dot()`(非常重要)

> 
![](https://upload-images.jianshu.io/upload_images/3203841-28bdb90e9f27144e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)

根据权重计算成绩

- 
计算规则


> 
(M行, N列) * (N行, Z列) = (M行, Z列)


> 
![](https://upload-images.jianshu.io/upload_images/3203841-d3ae003477155f11.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)

- 
### 矩阵拼接
- 矩阵垂直拼接


> 
![](https://upload-images.jianshu.io/upload_images/3203841-29c4f1eb9a6d3409.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)

- 矩阵水平拼接

> 
![](https://upload-images.jianshu.io/upload_images/3203841-52b67b6a480aa91c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)


### Numpy读取数据`np.genfromtxt`

> 
![](https://upload-images.jianshu.io/upload_images/3203841-b4c47c46e94f472a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/329)


> 
![](https://upload-images.jianshu.io/upload_images/3203841-686307a57dc6211b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)

读取csv格式的文件


> 
如果数值据有无法识别的值出现,会以`nan`显示,`nan`相当于`np.nan`,为float类型.




