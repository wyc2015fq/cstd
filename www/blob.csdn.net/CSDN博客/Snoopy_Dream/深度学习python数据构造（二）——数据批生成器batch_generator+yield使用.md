# 深度学习python数据构造（二）——数据批生成器batch_generator+yield使用 - Snoopy_Dream - CSDN博客





2018年09月30日 09:57:53[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：946
所属专栏：[深度学习caffe常用python代码分享](https://blog.csdn.net/column/details/22599.html)[数据生成器+数据增广+常用工具](https://blog.csdn.net/column/details/29905.html)









# 当数据集的数据过大时，采用生成器generator生成数据循环调用

# 那么，如何设置一个batchsize的批生成器以供训练深度学习模型用呢？

### 背景介绍：

下面这个例子是HWDB手写体，单张图片小，分类任务，trainset有89w+，正好涉及数据集多大，需要自己生成batch_generator提高效率。

![](https://img-blog.csdn.net/20180930095255259?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180930095307335?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 初步想法：

首先第一种方法，是**构造class**，利用class中的__next__ 迭代（**while true** index+1** if **batchsize **break**），来控制batchsize。

至于为啥用构造class的方法呢？可参考[http://www.runoob.com/w3cnote/python-yield-used-analysis.html](http://www.runoob.com/w3cnote/python-yield-used-analysis.html)
- .把所有的图片名称放到一个list中，shuffle第一次打乱,「__init__」总数目count，batchsize，index控制后面的shuffle
- 重载__next__，变成batchsize的next
- 把index放入重载的__next__中的while循环中，用index%count取余来判断是否完了一次。
- return images和lables,其中数据增强的库需要nhwc**四维的np格式的image**，所以image.append()，然后**np.asarray，lable变成one-hot编码**

那么**具体如何构造跑完一次，重新shuffle呢？**把index放入while循环中，用index%count取余来判断是否完了一次。

首先：需要运算**count**在init的时候。

其次：需要构造**index**，在**初始化**必须在**init**里面[之后添加任何变量，都要在init中self.=]，因为如果在next第二个batchsize的index还是从零开始。

```php
self.index += 1            
self.index = self.index % self.count  #1%300008=1
if self.index == 0:  #跑完一个轮回，重新打乱
    np.random.shuffle(self.image_names)
```

### 具体代码：

```python
class generator():
    def __init__(self, filepath="/home/programs/DATAS/HWDB/train/", batch_size=16, data_aug=True):  #形参用到的，路径+batchsize+是否数据增强
        self.base_dir = filepath
        self.char_set = os.listdir(filepath)    # 所有类别
        self.num_classes = len(self.char_set)   # 类别数量
        self.image_names = []               # 所有图片名称

        for char_class in self.char_set:
            self.image_names += glob(os.path.join(self.base_dir, char_class, "*.png"))  #glob返回的是list，这里用+=不用append，是因为，这样会生成一个列表中包含所有子文件夹中所有文件，之后打乱shuffle就是所有文件的打乱了。

        np.random.shuffle(self.image_names)
        self.index = 0 #注意位置
        self.count = len(self.image_names)
        self.batch_size = batch_size

        if data_aug:
            self.data_aug_sequential = init_aug_params()
        else:
            self.data_aug_sequential = None

    def next(self, batch_size=16):
        images = []
        labels = []

        while True:
            self.index += 1            
            self.index = self.index % self.count  #1%300008=1
            if self.index == 0:  #跑完一个轮回，重新打乱
                np.random.shuffle(self.image_names)
            image_path = self.image_names[self.index]
            image = read_image(image_path)
            if image is None:
                print("\n[WARRING]: 读取图片 '{}' 失败".format(image_path))
                continue

            image = resize_center_image(image)
            images.append(image)#list中存放各个图片的np格式

            label = image_path.split("/")[-2]
            labels.append(int(label))

            if len(images) == batch_size:
                break  #一次batchsize完毕，推出循环

        images = np.asarray(images, dtype=np.uint8)#将list格式的images转为n h w c用于data_aug_sequential输入

        if self.data_aug_sequential:
            images = pre_process(images, self.data_aug_sequential)
        labels = KU.to_categorical(labels, num_classes=self.num_classes)#one-hot编码，更好的控制特征表示

        return images / 255, labels

    def __next__(self):
        return self.next(self.batch_size)
```

one-hot 函数

def to_categorical(y, num_classes=None):

    y = np.array(y, dtype='int32')#one_hot

    input_shape = y.shape

    if input_shape and input_shape[-1] == 1 and len(input_shape) > 1:

        input_shape = tuple(input_shape[:-1])

    y = y.ravel()

    if not num_classes:

        num_classes = np.max(y) + 1

    n = y.shape[0]

    categorical = np.zeros((n, num_classes))

    categorical[np.arange(n), y] = 1

    output_shape = input_shape + (num_classes,)

    categorical = np.reshape(categorical, output_shape)

    return categorical

## 第二种方法：yield

参考自：[http://www.runoob.com/w3cnote/python-yield-used-analysis.html](http://www.runoob.com/w3cnote/python-yield-used-analysis.html)

首先讲解下yield，以fab函数为例

```python
def fab(max): 
    n, a, b = 0, 0, 1 
    while n < max: 
        yield b      # 使用 yield
        # print b 
        a, b = b, a + b 
        n = n + 1
 
for n in fab(5): 
    print n
```

```
1 
1 
2 
3 
5
```

简单地讲，yield 的作用就是把一个函数变成一个 generator，带有 yield 的函数不再是一个普通函数，Python 解释器会将其视为一个 generator，**调用 fab(5) 不会执行 fab 函数**，而是**返回一个 iterable 对象！**在 **for 循环执行时（实则是调用了for里自带的next函数）**，每次**循环**都会执行 fab 函数内部的代码，**执行到 yield b 时**，fab 函数就**返回一个迭代值**，**下次迭代时，代码从 yield b 的下一条语句继续执行**，而函数的本地变量看起来和上次中断执行前是完全一样的，于是函数继续执行，直到再次遇到 yield。

也可以手动调用 fab(5) 的 next() 方法（因为 fab(5) 是一个 generator 对象，该对象具有 next() 方法），这样我们就可以更清楚地看到 fab 的执行流程：

```python
>>>f = fab(5) 
>>> f.next() 
1 
>>> f.next() 
1 
>>> f.next() 
2 
>>> f.next() 
3 
>>> f.next() 
5 
>>> f.next() 
Traceback (most recent call last): 
 File "<stdin>", line 1, in <module> 
StopIteration
```

当函数执行结束时，generator 自动抛出 StopIteration 异常，表示迭代完成。在 for 循环里，无需处理 StopIteration 异常，循环会正常结束。

**我们可以得出以下结论：**

一个带有 yield 的函数就是一个 generator，它和普通函数不同，生成一个** generator 看起来像函数调用**，但**不会执行任何函数代码**，**直到对其调用 next()**（在 for 循环中会自动调用 next()）才开始执行。虽然**执行流程仍按函数的流程执行，但每执行到一个 yield 语句就会中断，并返回一个迭代值**，下次执行时从 yield 的下一个语句继续执行。看起来就好像一个函数在正常执行的过程中被 yield 中断了数次，每次中断都会通过 yield 返回当前的迭代值。

yield 的**好处**是显而易见的，把一个函数改写为一个 generator 就获得了**迭代能力**，**比起用类的实例保存状态来计算下一个 next() 的值**，不仅代码简洁，而且执行流程异常清晰。



## yield**文件读取**

另一个 yield 的例子来源于**文件读取**。如果直接对文件对象调用 read() 方法，会导致不可预测的内存占用。好的方法是**利用固定长度的缓冲区来不断读取文件内容**。通过 **yield，我们不再需要编写读文件的迭代类**，就可以轻松实现文件读取：

## 实例

```python
def read_file(fpath): 
    BLOCK_SIZE = 1024 
    with open(fpath, 'rb') as f: 
        while True: 
            block = f.read(BLOCK_SIZE) 
            if block: 
                yield block 
            else: 
                return
```



下面是另外一个，main里有一个for函数里也有一个for，可能会有些晕。。

其实说白了就是遇到main里的for循环生成器，就把它看成next函数，直接进入yield_test函数，遇到yield就返回一个值，然后

接着继续运行yield后面一句。

```python
def yield_test(n):
    for i in range(n):
        yield call(i)
        print("i=",i)
    #做一些其它的事情    
    print("do something.")    
    print("end.")

def call(i):
    return i*2

#使用for循环
for i in yield_test(5):
#先直接跳进yield_test循环，遇到yield返回0*2=0，给print得到第一行0 ,
#然后继续进入yield_test循环，yield call(i)下一行，此时i=0，打印出第二行i= 0
#第三次跳进yield_test循环，遇到yield返回1*2=2,返回给print(i,",")得到 2 ,
    print(i,",")
    
运行结果：
>>> 
0 ,
i= 0
2 ,
i= 1
4 ,
i= 2
6 ,
i= 3
8 ,
i= 4
do something.
end.
>>>
```

```python
def generator(filepath="/home/lmzwhu/lmzwhu/programs/DATAS/HWDB/train", batch_size=16, data_aug=True):
    base_dir = filepath
    char_set = os.listdir(filepath) 
    num_classes = len(char_set)
    image_names = []  
    
    for char_class in char_set:
        image_names += glob(os.path.join(base_dir, char_class, "*.png"))  #glob返回的是list，这里用+=不用append，是因为，这样会生成一个列表中包含所有子文件夹中所有文件，之后打乱shuffle就是所有文件的打乱了。
    count = len(image_names)
    np.random.shuffle(image_names)
    if data_aug:
        data_aug_sequential = init_aug_params()
    else:
        data_aug_sequential = None
    
    index=0
    images = []
    labels = []
    batch_count = 0
    while True:
        index += 1            
        index = index % count  #1%300008=1
        if index == 0:  #跑完一个轮回，重新打乱
            np.random.shuffle(image_names)
        image_path = image_names[index]
        image = read_image(image_path)
        if image is None:
            print("\n[WARRING]: 读取图片 '{}' 失败".format(image_path))
            continue

        image = resize_center_image(image)
        images.append(image)#list中存放各个图片的np格式

        label = image_path.split("/")[-2]
        labels.append(int(label))
        
        start = batch_count * batch_size
        end = start + batch_size
        batch_count += 1
        if data_aug_sequential:
            images = pre_process(images, data_aug_sequential)
        labels = KU.to_categorical(labels, num_classes=num_classes)
        yield np.asarray([d[start:end] for d in images], dtype=np.uint8)/ 255, labels

#实例化批量数据生成器
batch_gen = generator()
for i in range(20):
    train_datas = next(batch_gen)
```





