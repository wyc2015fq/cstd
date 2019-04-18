# Numpy基础+进阶+高级 - wsp_1138886114的博客 - CSDN博客





2018年06月01日 13:59:32[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：927
所属专栏：[科学计算库与可视化](https://blog.csdn.net/column/details/23452.html)









### 一、ndarray概念

> 
##### 1.1、回顾python基本类型

```
数字类型：     整型：  布尔型bool、整型int、长整形long
                      非整型：浮点float 、复数complex

容器：         序列：字符串str 、列表list、元组tuple
              集合：可变集合set、不可变集合frozen set
              映射：字典dict

python是C语言实现的，都是C语言设计的结构体
示例：int(1)在内存：应用计数和类型
```

##### 1.2、高效的固定类型数组：ndarray

```
list:          支持存储不同类型数据
                       可以动态增加长度
                       计算性能一般
                       存储冗余多

        ndarray:       只能存储单一数据类型
                       不可以动态增加长度
                       计算性能好
                       存储冗余少
```

##### 1.3、ndarray的基本用法

```
import numpy as np                 #导入模块，并命名为np
x = np.array([[1,2,3],[4,5,6]])    #创建一个ndarray数组
x
array([[1,2,3],
        [4,5,6]])
type(x)                            #查看类型
numpy.ndarray
```

##### 1.4、ndarray的属性
|ndim:|维度数            ndim = 2|
|----|----|
|shape：|数组形状        shape = (2,3)|
|size：|数组元素总数      size = 6|
|dtype：|数组元素的数据类型   dtype = int 64|
|itemsize：|每个元素所需内存空间  itemsize = 8|
|strides：|移动到下一个元素所需偏移量（字节）|
|nbytes：|存储该数组所需内存大小 itemsize*size|
|data:|数组元素对应的内存区域|


### 二、创建 ndarray

> 
##### 2.1 基本语法

```
array(object,dtype = None,copy=True,order= 'K',subok = False,ndmin = 0)
object:    列表或任何一个_array_方法 返回一个数组的对象
dtype:     数组元素的数据类型，支持自动向更高精度转换
order：    数组元素在内存的储存顺序
           C语言风格，行优先
           Fortran风格，列优先
```

##### 2.2 从函数创建 array

定义一个函数，根据下标计算每个位置上的值

```
from function(function,shape,**kwargs)
    function:定义一个函数，接受N个参数（N维度数），返回一个数值
    shape： 要创建的数组每个维度的大小
```

##### 创建数组（一维/多维）

```
import numpy as np          # 导入模块，并命名为np
list1 = [1,2,3,4,5]  
x1 = np.array(list)         # 创建一个ndarray二维数组,里面为python容器  
x2 =np.array([np.arange(2),np.arange(2)])   # 输出[[0,1],[0,1]]  
x3 = np.array([[1,2,3],[4,5,6]])            # 创建一个ndarray二维数组 

◆创建一个[a,b]范围内取n点的等间距分布数组
x4 =np.linspace(0,10,4,endpoint= True)      #默认True 
>>>[0. 3.33333333  6.66666667  10. ]  
x5 =np.linspace(0,10,4,endpoint=False)        
>>>[0.  2.5 5.  7.5]  

◆特殊多维矩阵
b = np.ones((1, 2))     #创建一个值均为 1 的1*2维ndarray对象  
c = np.full((2, 2), 7)  #创建一个值均为 7 的2*2维ndarray对象  
d = np.eye(2)           #创建一个 2*2 维对角矩阵  
e=np.eye(3,k=2)         #第一个参数：行数=列数，即行数或列数
```

```
#第二个参数k：默认情况下输出的是对角线全“1” ，其余全“0” 的方阵,   
#如果k为正整数，则在右上方第k条对角线全“1” 其余全“0” ，k为负整数则  
#在左下方第k条对角线全“1” 其余全“0” ，详情如下图。
```

![这里写图片描述](https://img-blog.csdn.net/20180531174348117?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


### 三、numpy中的数据类型
|名称|描述|
|----|----|
|bool|用一个字节存储的布尔类型（True或False）|
|inti|由所在平台决定其大小的整数（一般为int32或int64）|
|int8|一个字节大小，-128 至 127|
|int16|整数，-32768 至 32767|
|int32|整数，-2 ** 31 至 2 ** 32 -1|
|int64|整数，-2 ** 63 至 2 ** 63 - 1|
|uint8|无符号整数，0 至 255|
|uint16|无符号整数，0 至 65535|
|uint32|无符号整数，0 至 2 ** 32 - 1|
|uint64|无符号整数，0 至 2 ** 64 - 1|
|float16|半精度浮点数：16位，正负号1位，指数5位，精度10位|
|float32|单精度浮点数：32位，正负号1位，指数8位，精度23位|
|float64或float|双精度浮点数：64位，正负号1位，指数11位，精度52位|
|complex64|复数，分别用两个32位浮点数表示实部和虚部|
|complex128或complex|复数，分别用两个64位浮点数表示实部和虚部|

###### 自定义数据类型：

> 
```php
T=np.dtype([('name', np.str_, 40), ('numitems',np.int32), ('price',np.float32)]) 

products = np.array([('DVD',42,3.14),('Butter',13,2.72)],dtype=T)   #插入数据  
print(products.dtype)                    
>>>
[('name', '<U40'), ('numitems', '<i4'), ('price', '<f4')]
```


### 四、ndarray多维数组操作

> 
##### 4.1访问数组

```
ndarray数组的索引 切片（Slicing）
访问一维数据/切片对象  slice(start,stop,step)

[:]           所有元素
[:-1]        去除最后一个元素
[1:]          索引>=1的元素
[ :3]         索引<=3的元素
[-2:]         >=倒数第2个
[1::2]        >=1开始，以2为步长取到结束
[::-1]        所有元素倒序
访问二维
[:,1]         所有行，第一列
[:,:2]        所有行，列步长2
访问三维
[2,1,2] =[2][1][2]         第三层第二行第三列
[0,:,:]                    第一层所有
[0,1,::2]                  第一层第二行列步长2
[::-1]                     所有层数倒序
```

###### 4.1.1遍历数组

```
◆遍历每一个元素
x = np.arange(12).reshape((3,4))  
for element in x.flat:        #flat 迭代器  
    print element

◆按行遍历（即沿第一个维度切片）
x = np.arange(12).reshape((3,4))  
for row in x  
    print (row)

◆沿任意维度遍历
x = np.arange(24).reshape((2,3,4))  
for i in range(x,shape[1]):       #获取指定维度大小  
    print (x[:,i,:])              #对这个维度上的每一个截面切片
```

###### 4.1.2 访问数组元素 [ 切片 ]

```
◆访问二维数组 及其元素
import numpy as np  
heros = np.array([['苏轼','陈咬金','廉颇'],  
                   ['后羿','公孙丽','狄仁杰']],dtype='U8')  
print(heros[0][2])  
print(heros[:,1])   #获取所有行 第二列  
print(heros[:,2])   #获取所有行 前二列 

print(heros[:,::2]) #获取所有行;所有列，步长为2  
>>>：  [['苏轼' '廉颇']  
       ['后羿' '狄仁杰']] 

◆访问多维数组 及其元素
arr_1 = np.array([                                    #一栋房子三层，两行，四列  
                    [  
                        ["苏烈","程咬金","廉颇","亚瑟"],  
                        ["后羿","公孙离","狄仁杰","鲁班"],  
                    ],  
                    [  
                        ["王昭君","安其拉","貂蝉","小乔"],  
                        ["孙膑","大乔","鬼谷子","蔡文姬"]  
                    ],  
                    [  
                        ["王lang","刘邦","刘备","孙悟空"],  
                        ["相遇","刘禅","周庄","东皇太一"]  
                    ]  
                ])  

print(arr_1[2][1][3])   #获取第三层第二行第四列。>>>：东皇太一  
print(arr_1[0,1,:])     #获取第一层第二行所有值。>>>：['后羿' '公孙离' '狄仁杰' '鲁班']  
print(arr_1[0,1,::2])   #获取第一层第二行步长为2。>>>：['后羿' '狄仁杰']
```

###### 4.1.3 访问数组元素 [ index ]

```
◆每个维度都可以指定一个索引数组
x = np.arange(10,19).reshape((3,3))  
idx1 = [0,1,2]  
idx2 = [2,1,0]  
print ('原数组：\n',x) 
print ('获取\n',x[idx1,idx2])

◆每个维度都可以指定一个多维索引数组
x = np.arange(10,19).reshape((3,3))  
idx1 = np.arange([[0,0],[1,1]])  
idx2 = np.arange([[2,1],[2,1]])
print ('原数组：\n',x)   
print ('获取\n',x[idx1,idx2])

◆增加维度：newaxis:将原数组作为新的更高维数组中的一个切片
x = np.arange(6).reshape((2,3))  
y = x[:,:,np.newaxis]      #将二维数组的第三维作为1，形成三维数组  
print (y)  
print (y.shape) 

◆ 网格函数ix_    
以二维网格为例，假设x是一个二维数组
x[np.ix_]
```

##### 4.2 ndarray操作数组维度

###### 4.2.1改变ndarray 形状

```
通过reshape方法改变 ndarray 形状
    numpy.reshape(arr1,newshape,order='C')
    #arr1:    需要改变形状的数组
    #newshape:新的形状tuple，其中有一个维度为-1，会根据数组总长和其它维度计算出来
    #order:   以这个顺序来读取arr1中的元素，可选值{'C','F','A'}

通过 resize 方法改变 ndarray 形状
    numpy.resize(arr1,newshape)
    arr1.resize(newshape,refcheck=True)
```

```
print('---------------------修改多维数组---------------------')  
line = np.arange(24)  
print(line)  
方法1  
result = line.reshape(2,3,4)   #操作数组的投影（视图）（先复制，再修改）  
print(result)  

方法2  
line.shape = (2,3,4)           #直接修改数组的形状  
print(line)  

方法3  
line.resize(2,3,4)             #修改数组的形状  
print(line)

print('---------------将三维数组展平成一位数组-------------')  
d3 = np.arange(24).reshape(2,3,4)  

d1 = d3.ravel()                #直接修改数组  
print(d3)  
d2 = d3.flatten()              #操作数组的投影（视图）（先复制，再修改）  
print(d2)
```

###### 4.2.2 ndarray数组合并
|方法|用途|
|----|----|
|row_stack(tup)|多个一维array当作行，合并成2维（行拼接）== vstack（效果一样）|
|column_stack(tup)|多个一维array当作列，合并成2维（列拼接）==hstack（效果一样）|
|vstack(tup)|沿第一个维度合并（行拼接）|
|hstack(tup)|沿第二个维度合并（列拼接）|
|**通用公式**|concatenate(tup,axis=0/1)    沿着指定维度合并|

###### 4.2.3 ndarray数组拆分
|方法|用途|
|----|----|
|hsplit(arr,indices)|拆分多列|
|vsplit(arr,indices)|拆成多行|
|**split(arr,indices,axis=0)****通用公式**|**沿着指定方向拆分**|

##### 4.3 bool 数组

创建布尔（bool）数组

```python
直接创建:
x = np.array([True,False,True,False])  
print (x)
>>>  
[True,False,True,False]

通过比较操作符计算得到：
x = np.arange(6).reshape((2,3))      
print (x>3)                # 运算符
>>>  
[False,False,False]  
[False,True,True] 

通过通用函数计算
x = np.arange(6).reshape((2,3))  
print np.greater(x,3)      #使用numpy函数创建
>>>  
[False,False,False]  
[False,True,True]
```


### 五、ndarray 数组运算

> 
##### 5.1 算术运算：

```
+，-，/，*，//(floor division整除),**(幂)，%(取模)
    算数运算都是针对相同位置的元素进行的。
    更新运算符：+=，-=，*=，/=，**=
```

##### 5.2 比较运算：

```
同算术运算，返回bool值
    还可以通过通用函数：算术函数 来进行计算
```

##### 5.3 判断：

```
np.all(alltrue)    判断array 是否所有元素都为True
    np.any(sometrue)   判断array 是否至少有一个True
```

##### 5.4 聚合计算：沿着一个指定维度计算汇总

```
np.average     加权平均值（arr1 , weights = arr2）
    np.mean        算术平均值（arr1）
    np.median      计算中位数
    np.sum         求和
    np.prod        求乘积（阶乘）
    np.cumprod     数组的累积乘积
    np.min         求最小
    np.max         求最大

    np.bincount    计算每个元素出现的次数
```

##### 5.5 查找和排列

```
np.argmin                沿指定维度查找最小值下标
    np.argmax                沿指定维度查找最大值下标
    np.nonzero               查找非零元素的下标
    np.where（condition）     根据条件查找或替换
    np.take(column,index)    #根据索引获取值
    np.argsort     沿着指定维度计算下标，按这个下标元素是递增的
    np.sort        沿指定维度 元素按递增顺序排序
    np.lexsort     根据多个array进行排序
```

##### 5.6 数组的修剪

```
用clip()函数计算：将所有比给定最大值num1还大的元素全部设为num1，
                而所有比给定最小值num2还小的元素全部设为给定的最小值num2
```

```
arr1. clip(num1,num2)  
arr1= [0 1 2 3 4]  
Clipped [1 1 2 2 2]
```

##### 5.7 数组的压缩

```
利用compres ()函数计算：返回一个根据给定条件筛选后的数组。
```


### 六、线性代数

> 
##### 6.1矩阵的创建 np.mat( ) / np.bmat( )

```
import numpy as np  

mat1 = np.mat('1 2 3 4;5 6 7 8;9 11 12 13')    #创建矩阵,方法1  
mat2 = np.mat(np.arange(9)).reshape(3,3)       #创建矩阵，方法2  
print(mat1)  
print(mat2)  
>>>                #输出mat1  
[[ 1  2  3  4]  
 [ 5  6  7  8]  
 [ 9 11 12 13]]          
>>>               #输出mat2  
[[0 1 2]  
 [3 4 5]  
 [6 7 8]]

----------------------------------------------------
A = np.eye(3,3)  
B = A*2  
mat3 = np.bmat("A B;B A")  
print(A,"\n")  
print(B,"\n")  
print(mat3)  
>>>                #输出 A 矩阵  
[[1. 0. 0.]  
 [0. 1. 0.]  
 [0. 0. 1.]]   
>>>                输出 B 矩阵  
[[2. 0. 0.]  
 [0. 2. 0.]  
 [0. 0. 2.]]   
>>>                输出 mat3 矩阵  
[[1. 0. 0. 2. 0. 0.]  
 [0. 1. 0. 0. 2. 0.]  
 [0. 0. 1. 0. 0. 2.]  
 [2. 0. 0. 1. 0. 0.]  
 [0. 2. 0. 0. 1. 0.]  
 [0. 0. 2. 0. 0. 1.]]
```

##### 6.2 创建随机数矩阵函数

![这里写图片描述](https://img-blog.csdn.net/20180531215004788?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 6.3 矩阵统计函数

![这里写图片描述](https://img-blog.csdn.net/20180531220241741?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
np.random.normal(loc,scale,size):      # loc:均值；scale标准差；size 数据量
    loc：float
    此概率分布的均值（对应着整个分布的中心centre）
    scale：float
    此概率分布的标准差（对应于分布的宽度，scale越大越矮胖，scale越小，越瘦高）
```

##### 6.4 一元通用函数：
|**函数**|说明|
|----|----|
|abs、fabs|计算整数、浮点数或复数的绝对值。对于非复教值．可以使用更快的fabs|
|sqrt|计算各元素的平方根。相当于   arr**0.5|
|square|计算各元素的平方。相当于   arr**2|
|exp|计算各元素的指数e­**x|
|log、log10、 log2 、log1p|分别为自然对数（底数为e）、底数为10的log、底数为2的log、log(1+x)|
|sign|计算各元素的正负号：1 (正数)、 O(零)、-1（负数）|
|ceil|计算各元素的ceiling值．即大于等于该值的最小整数|
|floor|计算各元素的floor值．即小于等于该值的最大整数|
|rint|将各元素值四含五入到．接近的整数．保留dtype|
|modf|将数组的小数和整数部分以两个独立数组的形式返回|
|isnan|返回一个表示“哪些值是NaN（这不是一个数字）”的布尔型救组|
|isfinite 、isinf|分别返回一个表示“哪些元紊是有穷的（非inf.非 NaN )”或“哪些元素是无穷的”的布尔型数组|
|cos 、cosh 、sin、sinh、tan、tanh|普通型和双曲型三角函|
|arccos、arccosh、arcsin|反三角函数|

##### 6.5 二元通用函数：
|函数|说明|
|----|----|
|add|将数组中对应的元素相加|
|subtract|从第一个数组中减去第二个数组中的元素|
|multiply|数组元索相乘|
|divide、floor_divide|除法或向下取整除法（丢弃余数 ）|
|power|对第一个救组中的元素，根据第二个数组中的相应元索B ，计算AB.|
|max、fmax|元素级的最大值计算。 fmax 将忽峪 NaN|
|min、fmix|元素级的最小值计林。 fmin 将忽略 NaN|
|mod|元素级的求模计算（除法的余数）|
|copysign|将第二个数组中的值的符号复制给第一个数组中的值|
|greater、greater_equal、less、less_equal、equal   Not_qual|执行元素级的比较运算，最终产生布尔型数组。相当于中缀运算符 >，>=，<，<=，==．!=|
|logical_add、logical_or、   Logical_xor|执行元素级的直值逻辑运算。相当于中缀运算符＆、|

##### 6.6 自定义函数 zero_like:

```python
a = np.array(np.arange(9)).reshape(3,3)  
    def ultimate_answer(a):  
        result = np.zeros_like(a)  
        result.flat = 42  
        return result  

    function_like = np.frompyfunc(ultimate_answer,1,1)  
    text=function_like(a)  
    print(text)
```

##### 6.7 四种自定义求和函数

```
a = np.arange(9)  
print("Reduce", np.add.reduce(a))  
>>>36  
print("Accumulate", np.add.accumulate(a))  
>>>[ 0  1  3  6 10 15 21 28 36]  

print("Reduceat", np.add.reduceat(a, [0, 5, 2, 7]))  
>>>[10  5 20 15]  
    #第一步用到索引值列表中的0和5，对数组中索引值在0到5之间的元素进行reduce操作 得到10  
    #第二步用到索引值5和2。由于2比5小，所以直接返回索引值为5的元素 得到5  
    #第三步用到索引值2和7。对索引值在2到7之间的数组元素进行reduce操作 得到20  
    #第四步用到索引值7。对索引值从7开始直到数组末端的元素进行reduce操作 得到15  
print("Outer", np.add.outer(np.arange(1,3), a))  
>>>[[ 1  2  3  4  5  6  7  8  9]  
>>>[ 2  3  4  5  6  7  8  9 10]]  
    #返回1+a数组的每个元素；  
    #返回2+a数组的每个元素。
```

##### 6.8 `numpy.linalg` 模块主要函数
|**函数及描述**|**演示代码**| | | | | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|dot   两个数组的点积|a = np.array([[1,2],[3,4]]), b = np.array([[11,12],[13,14]]);`np.dot(a,b)`：`[[1*11+2*13, 1*12+2*14],[3*11+4*13, 3*12+4*14]]`vdot   两个向量的点积a = np.array([[1,2],[3,4]]) ,b = np.array([[11,12],[13,14]]) ;`np.vdot(a,b)`：`1*11 + 2*12 + 3*13 + 4*14 = 130`inner   两个数组的内积`np.inner(np.array([1,2,3]),np.array([0,1,0]))`;输出：`1*0+2*1+3*0`matmul   两个数组的矩阵积矩阵乘法determinant   数组的行列式行列式的detsolve   求解线性矩阵方程inv   寻找矩阵的乘法逆矩阵```print('------------逆矩阵与单位矩阵---------------')  import numpy as np  A = np.mat("0 1 2;1 0 3;4 -3 8")  print("A=",A)  inverse = np.linalg.inv(A)  print(u"A的逆矩阵：","\n",inverse)  I = A*inverse  print('单位矩阵:I = A*inverse：',"\n",I)  >>>  A= [[ 0  1  2]      [ 1  0  3]      [ 4 -3  8]]  A的逆矩阵：      [[-4.5  7.  -1.5]      [-2.   4.  -1. ]      [ 1.5 -2.   0.5]]  单位矩阵:I = A*inverse：      [[1. 0. 0.]      [0. 1. 0.]      [0. 0. 1.]]``````使用 np.linalg.solve(A, b) 求解线性方程组```![这里写图片描述](https://img-blog.csdn.net/20180601095335284?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)```A= np.mat("1 -2 1;0 2 -8;-4 5 9")  B= np.array([0,8,-9])  C = np.linalg.solve(A,B)  print("X=",C[0],"Y=", C[1],"Z=", C[2])  >>>  X= 29.0   Y= 16.0   Z= 3.0```#####  6.9 特征值、特征向量```Arr =np.mat("3 -2;1 0")print("单独求特征值：",np.linalg.eigvals(Arr))print("特征值，特征向量：",np.linalg.eig(Arr))>>>单独求特征值：           [2. 1.]特征值，特征向量： (array([2., 1.]),     matrix([[0.89442719, 0.70710678],            [0.4472136 , 0.70710678]]))```##### 6.10奇异值分解```print('----------------SVD奇异值分解---------------')Arr2 = np.mat("4 11 14;8 7 -2")U,Sigma,V = np.linalg.svd(Arr2,full_matrices=False)print("U",U)print("Sigma",Sigma)print("V",V)>>>U [[-0.9486833  -0.31622777]  [-0.31622777  0.9486833 ]] Sigma [18.97366596  9.48683298] V [[-0.33333333 -0.66666667 -0.66666667]  [ 0.66666667  0.33333333 -0.66666667]]```##### 6.11 计算矩阵的行列式```print('----------矩阵行列式的计算-----------')Arr3 = np.mat("3 4;5 6")print("矩阵行列式的计算：",np.linalg.det(Arr3))>>>矩阵行列式的计算： -2.0000000000000013```### 七、排序函数> 排序```7.1、 ndarray类的sort方法——可对数组进行原地排序； np.sort(-arr , axis=1)降序排序7.2、 argsort函数——返回输入数组排序后的下标；    np.argsort(-arr,axis=1)降序排序7.3、 sort函数——返回排序后的数组``````print('----------------ndarray排序-----------------')list1 = [[3,1,2],[5,7,0]]array1 = np.array(list1)array1.sort(axis=1)print(array1)>>>[[1 2 3] [0 5 7]]print("------------返回排序后的索引值----------------")list1 = [[3,1,2],[5,7,0]]array1 = np.array(list1)array2 = np.argsort(array1,axis=0)  #返回列索引array3 = np.argsort(array1,axis=1)  #返回行索引print("array2:",array2)print("array3:",array3)>>>array2:[[0 0 1]        [1 1 0]] array3:[[1 2 0]        [2 0 1]]```### 八、搜索函数> 常用搜索函数```◆ argmax函数：      返回数组中最大值对应的下标◆ nanargmax函数：   与argmax提供相同的功能，但忽略NaN值◆ argmin函数：      返回数组中最小值对应的下标◆ nanargmin函数：   与argmin的功能类似，     但忽略NaN值◆ argwhere函数：    根据条件搜索非零的元素，并分组返回对应的下标◆ searchsorted函数：为指定的插入值寻找维持数组排序的索引位置。该函数使用二分``````print('----------返回最大值的索引----------')arr_a = np.array([[3,1,2],[5,7,0]])print(np.argmax(arr_a))        #先将多维数组展平，再返回最大值的索引print("------argwhere返回对应的下标-----")arr_a = np.array([[3,1,2],[5,7,0]])print(np.argwhere(arr_a>3))>>>[[1 0]         #第二行第一列 [1 1]]        #第二行第二列print('-----searchsorted()为指定的插入值寻找维持数组排序的索引位置----')arr_b = np.arange(5)arr_insert = np.searchsorted(arr_b,[-1,7])print("插入后：",np.insert(arr_b,arr_insert,[-1,7]))>>>插入后： [-1  0  1  2  3  4  7]```### 九、数组元素抽取> ```9.1、 extract函数——根据某个条件从数组中抽取元素9.2、 nonzero函数——专门用来抽取非零的数组元素``````print('-----------元素抽取-------------')arr_ele = np.arange(8)condition =(arr_ele%2) ==0print("抽取能被2整除的元素：",np.extract(condition,arr_ele))print("非零值：",np.nonzero(arr_ele))>>抽取能被2整除的元素： [0 2 4 6]非零值： (array([1, 2, 3, 4, 5, 6, 7], dtype=int64),)print('-----------元素抽取(condition)-------------')arr_ele = np.arange(8)arr_fa = arr_ele.compress(condition=(arr_ele%2) ==0)print("抽取能被2整除的元素",arr_fa)print('-----------元素抽取(where)-------------')arr_ele = np.arange(8)arr_fa = np.where((arr_ele%2) ==0)print("抽取能被2整除的元素",arr_fa)print('---------非零元素索引提取----------')arr = np.array([[0,1,3],                [0,4,7],                [7,0,9]])row,col = np.nonzero(arr)print("非零元素行索引row_index",row)print("非零元素列索引col_index",col)```### 十、ndarray的文件IO> ##### 10.1.保存为二进制文件```保存单个 array( .npy文件)    np.save(file,arr,allow_pickle=True,fix_imports=True)        allow_pickle:允许使用pickle的方式保存对象。出于安全性和兼容性考虑，会禁用pickle        安全性：加载一个pickle的数据时，会执行任何代码        兼容性：pickle对象对版本有要求，不同版本python不兼容保存多个 array(.npz文件)    np.savez(file,*args,**kwargs)        *args:以列表参数的方式指定要保存的array，无法保存array变量名，加载时通过arr_0,arr_1这种方式获取。        **kwargs:以命名参数的方式指定array，加载时可以通过名称获取array保存多个array并压缩( .npz文件)    np.savez_compressed(file,*args,**kwargs)```##### 10.2 加载二进制文件```np.load(file,mmap_mode=None,allow_pickle=True,fix_imports=True,encoding='ASCII')       mmap_mode:可选值(None,'r+','r','w+','c')。如果不是None，则会用指定的模式把内存块映射为一个file对象。            可以实现访问array中的一部分而不用把整个array导入内存       fix_imports：只在python3中读取python2    生成含pickle对象的文件时有用       encoding：   只在python3中读取python2 生成含pickle对象的文件时有用                        可选值有('latin1','ASCII',bytes)np.load 可以支持。npy和,npz文件的读取       读取.npy文件 直接返回array       读取.npz文件，返回一个NpzFile对象（用完close），可以通过.key()方法查看包含的array```##### 10.3 保存为文本文件```np.savetxt(filename,array,fmt= '%.18e',delimiter='',            newline='\n',header='',footer='',comments='#')        -filename:  文件名，若以.gz结尾则自动以gzip压缩        -fmt:       保存格式        -delimiter：分隔符        -newline：  换行符        -header：   首行输出内容        -footer：   末行输出内容        -comments:  在header和footer前插入的字符，表示注释```##### 10.4 加载文本文件```np.loadtxt(fname,dtype=<type 'float'>,comments= '#',delimiter=None,concerters=None,skiprows=0,usecols=None,skiprows=0,uppack=False,ndmin=0)        comments :    在header和footer前插入的字符，表示注释        concerters :  转换函数字典，通过下标key,为每列定义一个转换函数        skiprows :    跳过开头的若干行        usecols :     使用指定列，下标0开始        uppack :      若为 True,可以用变量捕获每一列        ndmin :       最小的维度数``````import numpy as np■ 无参数写读arr1 = np.random.normal(size=20).reshape(4,5) #创建随机数矩阵np.savetxt('arr1_file.txt',arr1)              #默认分隔符：空格arr_data1 = np.loadtxt('arr_file.txt')■ 带参数写读arr2 = np.random.normal(size=20).reshape(4,5)           # 创建随机数矩阵np.savetxt('arr1_file.txt',arr2,fmt="%d",delimiter=",") # %d: 保存为整数，                                                        # 分隔符：逗号arr_data2 = np.loadtxt('arr2_file.txt',delimiter=",")■ 读取文件：arr3_file.csv文件内容如下："姓名","年龄","年级","体重","身高""李光",30,12,100,188"司马光",42,9,120,178"董卓",56,8,155,184"项羽",45,9,142,186arr3_data = np.loadtxt('arr3_file.csv',dtype = np.str,delimiter=",")arr3_num = arr3_data[1:,1:].astype(np.float)print(arr3_num)>>>[[ 30.  12. 100. 188.] [ 42.   9. 120. 178.] [ 56.   8. 155. 184.] [ 45.   9. 142. 186.] [ 65.   6. 152. 189.]]```###### [csv加载时间数据,请看这儿☛](https://blog.csdn.net/wsp_1138886114/article/details/80487035)### 十一、金融函数> ```fv  函数——计算所谓的终值（future value），某个金融资产在未来某一时间点的价值。pv  函数——计算现值（present value），即金融资产当前的价值。npv 函数——计算净现值（net present value），即按折现率计算的净现金流之和。pmt 函数——根据本金和利率计算每期需支付的金额。(payment)nper函数——计算定期付款的期数。rate函数——计算利率（rate of interest）。1. 某用户去银行存款，假设年利率3%、每季度续存金额    -------------计算fv    10元、存5年以及存款1000，则计算5年后可领取多少金额    分析：    rate : 0.03/4    nper : 5*4    pmt  : -10    pv   :  -1000            fv   :  存款终值是本息和        贷款终值是0``````pythonimport numpy as np                  print("Future value",np.fv(0.03/4,5*4,-10,-1000))      fvals = []for i in range(1,6):    fvals.append(np.fv(.03/4,i*4,-10,-1000))    print('第{}年的本息和是{}'.format(i,fvals[i-1]))>>>Future value 1376.0963320407982第1年的本息和是1070.7914448828128第2年的本息和是1143.7306449093103第3年的本息和是1218.8827612322955第4年的本息和是1296.3149319412119第5年的本息和是1376.0963320407982``````2.某用户去银行存款，假设年利率3%、每季度续存金额10--------------计算pv元、存5年后可领1376.0963320，则计算5年前存取的本金是多少金额``````pv = np.pv(0.03/4,5*4,-10,1376.096)print("当初存钱：",np.round(pv))    --np.round()四舍五入>>>当初存钱： -1000.0``````3.投资100，支出39、 59、 55、 20。 折现率为28.1%，则净现值为多少？---计算npv``````print("npv:",np.npv(0.281,[-100,39,59,55,20]))----rate:折现率；values:现金流>>>npv: -0.00847859163845488``````4.某同学房贷20万，准备15年还清，年利率为7.5%，则每月需还贷多少金额 ----计算pmt``````print("每月需还贷金额为:",np.pmt(0.075/12,12*15,200000))>>>每月需还贷金额为: -1854.0247200054619``````5.某同学房贷20万，年利率为7.5%，每月能还贷2000，则需要还多少期?-------计算nper``````year = np.ceil(np.nper(0.075/12,-2000,200000)/12)------np.ceil：向上取整print("需要还贷{}年。".format(year))>>>需要还贷14.0年。```### 十二、广播> ```让两个不同维度的 array进行元素运算若维数较小的array增加1的维度，直到两者维数相等``````x = np.arange(6).reshape((2,3))        y = np.arange(9).reshape((3,3))        x[:,np.newaxis,:]+y        array([                [[0,2,4],                    [3,5,7],                [6,8,10]]                [[3,5,7],                    [6,8,10],                [9,11,13]]            ])```### 十三、array数组copy> 完全不拷贝               赋值时，只是引用```x = np.array([1,2,3,4])y = xy is xTrue``````浅拷贝(copy视图/投影)        视图会创建一个新 array 对象，但和原来的 array 共享同一份数据        创建视图：ndarray.view/通过切片        判断一个array是否为视图        ndarray.flags.owndata        示例：``````python#通过ndarray.view 创建x = np.array([1,2,3,4])y = x.view()print 'y.flags.owndata:',y.flags.owndataprint 'y.base is x:',y.base is xy.flags.owndata:Falsey.base is x:True``````深拷贝：完全copy。创建新拷贝--ndarray.copy()--通过索引数组和布尔数组获取新的array（又称为Fancy Indexing）```|vdot   两个向量的点积|a = np.array([[1,2],[3,4]]) ,b = np.array([[11,12],[13,14]]) ;`np.vdot(a,b)`：`1*11 + 2*12 + 3*13 + 4*14 = 130`inner   两个数组的内积`np.inner(np.array([1,2,3]),np.array([0,1,0]))`;输出：`1*0+2*1+3*0`matmul   两个数组的矩阵积矩阵乘法determinant   数组的行列式行列式的detsolve   求解线性矩阵方程inv   寻找矩阵的乘法逆矩阵```print('------------逆矩阵与单位矩阵---------------')  import numpy as np  A = np.mat("0 1 2;1 0 3;4 -3 8")  print("A=",A)  inverse = np.linalg.inv(A)  print(u"A的逆矩阵：","\n",inverse)  I = A*inverse  print('单位矩阵:I = A*inverse：',"\n",I)  >>>  A= [[ 0  1  2]      [ 1  0  3]      [ 4 -3  8]]  A的逆矩阵：      [[-4.5  7.  -1.5]      [-2.   4.  -1. ]      [ 1.5 -2.   0.5]]  单位矩阵:I = A*inverse：      [[1. 0. 0.]      [0. 1. 0.]      [0. 0. 1.]]``````使用 np.linalg.solve(A, b) 求解线性方程组```![这里写图片描述](https://img-blog.csdn.net/20180601095335284?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)```A= np.mat("1 -2 1;0 2 -8;-4 5 9")  B= np.array([0,8,-9])  C = np.linalg.solve(A,B)  print("X=",C[0],"Y=", C[1],"Z=", C[2])  >>>  X= 29.0   Y= 16.0   Z= 3.0```#####  6.9 特征值、特征向量```Arr =np.mat("3 -2;1 0")print("单独求特征值：",np.linalg.eigvals(Arr))print("特征值，特征向量：",np.linalg.eig(Arr))>>>单独求特征值：           [2. 1.]特征值，特征向量： (array([2., 1.]),     matrix([[0.89442719, 0.70710678],            [0.4472136 , 0.70710678]]))```##### 6.10奇异值分解```print('----------------SVD奇异值分解---------------')Arr2 = np.mat("4 11 14;8 7 -2")U,Sigma,V = np.linalg.svd(Arr2,full_matrices=False)print("U",U)print("Sigma",Sigma)print("V",V)>>>U [[-0.9486833  -0.31622777]  [-0.31622777  0.9486833 ]] Sigma [18.97366596  9.48683298] V [[-0.33333333 -0.66666667 -0.66666667]  [ 0.66666667  0.33333333 -0.66666667]]```##### 6.11 计算矩阵的行列式```print('----------矩阵行列式的计算-----------')Arr3 = np.mat("3 4;5 6")print("矩阵行列式的计算：",np.linalg.det(Arr3))>>>矩阵行列式的计算： -2.0000000000000013```### 七、排序函数> 排序```7.1、 ndarray类的sort方法——可对数组进行原地排序； np.sort(-arr , axis=1)降序排序7.2、 argsort函数——返回输入数组排序后的下标；    np.argsort(-arr,axis=1)降序排序7.3、 sort函数——返回排序后的数组``````print('----------------ndarray排序-----------------')list1 = [[3,1,2],[5,7,0]]array1 = np.array(list1)array1.sort(axis=1)print(array1)>>>[[1 2 3] [0 5 7]]print("------------返回排序后的索引值----------------")list1 = [[3,1,2],[5,7,0]]array1 = np.array(list1)array2 = np.argsort(array1,axis=0)  #返回列索引array3 = np.argsort(array1,axis=1)  #返回行索引print("array2:",array2)print("array3:",array3)>>>array2:[[0 0 1]        [1 1 0]] array3:[[1 2 0]        [2 0 1]]```### 八、搜索函数> 常用搜索函数```◆ argmax函数：      返回数组中最大值对应的下标◆ nanargmax函数：   与argmax提供相同的功能，但忽略NaN值◆ argmin函数：      返回数组中最小值对应的下标◆ nanargmin函数：   与argmin的功能类似，     但忽略NaN值◆ argwhere函数：    根据条件搜索非零的元素，并分组返回对应的下标◆ searchsorted函数：为指定的插入值寻找维持数组排序的索引位置。该函数使用二分``````print('----------返回最大值的索引----------')arr_a = np.array([[3,1,2],[5,7,0]])print(np.argmax(arr_a))        #先将多维数组展平，再返回最大值的索引print("------argwhere返回对应的下标-----")arr_a = np.array([[3,1,2],[5,7,0]])print(np.argwhere(arr_a>3))>>>[[1 0]         #第二行第一列 [1 1]]        #第二行第二列print('-----searchsorted()为指定的插入值寻找维持数组排序的索引位置----')arr_b = np.arange(5)arr_insert = np.searchsorted(arr_b,[-1,7])print("插入后：",np.insert(arr_b,arr_insert,[-1,7]))>>>插入后： [-1  0  1  2  3  4  7]```### 九、数组元素抽取> ```9.1、 extract函数——根据某个条件从数组中抽取元素9.2、 nonzero函数——专门用来抽取非零的数组元素``````print('-----------元素抽取-------------')arr_ele = np.arange(8)condition =(arr_ele%2) ==0print("抽取能被2整除的元素：",np.extract(condition,arr_ele))print("非零值：",np.nonzero(arr_ele))>>抽取能被2整除的元素： [0 2 4 6]非零值： (array([1, 2, 3, 4, 5, 6, 7], dtype=int64),)print('-----------元素抽取(condition)-------------')arr_ele = np.arange(8)arr_fa = arr_ele.compress(condition=(arr_ele%2) ==0)print("抽取能被2整除的元素",arr_fa)print('-----------元素抽取(where)-------------')arr_ele = np.arange(8)arr_fa = np.where((arr_ele%2) ==0)print("抽取能被2整除的元素",arr_fa)print('---------非零元素索引提取----------')arr = np.array([[0,1,3],                [0,4,7],                [7,0,9]])row,col = np.nonzero(arr)print("非零元素行索引row_index",row)print("非零元素列索引col_index",col)```### 十、ndarray的文件IO> ##### 10.1.保存为二进制文件```保存单个 array( .npy文件)    np.save(file,arr,allow_pickle=True,fix_imports=True)        allow_pickle:允许使用pickle的方式保存对象。出于安全性和兼容性考虑，会禁用pickle        安全性：加载一个pickle的数据时，会执行任何代码        兼容性：pickle对象对版本有要求，不同版本python不兼容保存多个 array(.npz文件)    np.savez(file,*args,**kwargs)        *args:以列表参数的方式指定要保存的array，无法保存array变量名，加载时通过arr_0,arr_1这种方式获取。        **kwargs:以命名参数的方式指定array，加载时可以通过名称获取array保存多个array并压缩( .npz文件)    np.savez_compressed(file,*args,**kwargs)```##### 10.2 加载二进制文件```np.load(file,mmap_mode=None,allow_pickle=True,fix_imports=True,encoding='ASCII')       mmap_mode:可选值(None,'r+','r','w+','c')。如果不是None，则会用指定的模式把内存块映射为一个file对象。            可以实现访问array中的一部分而不用把整个array导入内存       fix_imports：只在python3中读取python2    生成含pickle对象的文件时有用       encoding：   只在python3中读取python2 生成含pickle对象的文件时有用                        可选值有('latin1','ASCII',bytes)np.load 可以支持。npy和,npz文件的读取       读取.npy文件 直接返回array       读取.npz文件，返回一个NpzFile对象（用完close），可以通过.key()方法查看包含的array```##### 10.3 保存为文本文件```np.savetxt(filename,array,fmt= '%.18e',delimiter='',            newline='\n',header='',footer='',comments='#')        -filename:  文件名，若以.gz结尾则自动以gzip压缩        -fmt:       保存格式        -delimiter：分隔符        -newline：  换行符        -header：   首行输出内容        -footer：   末行输出内容        -comments:  在header和footer前插入的字符，表示注释```##### 10.4 加载文本文件```np.loadtxt(fname,dtype=<type 'float'>,comments= '#',delimiter=None,concerters=None,skiprows=0,usecols=None,skiprows=0,uppack=False,ndmin=0)        comments :    在header和footer前插入的字符，表示注释        concerters :  转换函数字典，通过下标key,为每列定义一个转换函数        skiprows :    跳过开头的若干行        usecols :     使用指定列，下标0开始        uppack :      若为 True,可以用变量捕获每一列        ndmin :       最小的维度数``````import numpy as np■ 无参数写读arr1 = np.random.normal(size=20).reshape(4,5) #创建随机数矩阵np.savetxt('arr1_file.txt',arr1)              #默认分隔符：空格arr_data1 = np.loadtxt('arr_file.txt')■ 带参数写读arr2 = np.random.normal(size=20).reshape(4,5)           # 创建随机数矩阵np.savetxt('arr1_file.txt',arr2,fmt="%d",delimiter=",") # %d: 保存为整数，                                                        # 分隔符：逗号arr_data2 = np.loadtxt('arr2_file.txt',delimiter=",")■ 读取文件：arr3_file.csv文件内容如下："姓名","年龄","年级","体重","身高""李光",30,12,100,188"司马光",42,9,120,178"董卓",56,8,155,184"项羽",45,9,142,186arr3_data = np.loadtxt('arr3_file.csv',dtype = np.str,delimiter=",")arr3_num = arr3_data[1:,1:].astype(np.float)print(arr3_num)>>>[[ 30.  12. 100. 188.] [ 42.   9. 120. 178.] [ 56.   8. 155. 184.] [ 45.   9. 142. 186.] [ 65.   6. 152. 189.]]```###### [csv加载时间数据,请看这儿☛](https://blog.csdn.net/wsp_1138886114/article/details/80487035)### 十一、金融函数> ```fv  函数——计算所谓的终值（future value），某个金融资产在未来某一时间点的价值。pv  函数——计算现值（present value），即金融资产当前的价值。npv 函数——计算净现值（net present value），即按折现率计算的净现金流之和。pmt 函数——根据本金和利率计算每期需支付的金额。(payment)nper函数——计算定期付款的期数。rate函数——计算利率（rate of interest）。1. 某用户去银行存款，假设年利率3%、每季度续存金额    -------------计算fv    10元、存5年以及存款1000，则计算5年后可领取多少金额    分析：    rate : 0.03/4    nper : 5*4    pmt  : -10    pv   :  -1000            fv   :  存款终值是本息和        贷款终值是0``````pythonimport numpy as np                  print("Future value",np.fv(0.03/4,5*4,-10,-1000))      fvals = []for i in range(1,6):    fvals.append(np.fv(.03/4,i*4,-10,-1000))    print('第{}年的本息和是{}'.format(i,fvals[i-1]))>>>Future value 1376.0963320407982第1年的本息和是1070.7914448828128第2年的本息和是1143.7306449093103第3年的本息和是1218.8827612322955第4年的本息和是1296.3149319412119第5年的本息和是1376.0963320407982``````2.某用户去银行存款，假设年利率3%、每季度续存金额10--------------计算pv元、存5年后可领1376.0963320，则计算5年前存取的本金是多少金额``````pv = np.pv(0.03/4,5*4,-10,1376.096)print("当初存钱：",np.round(pv))    --np.round()四舍五入>>>当初存钱： -1000.0``````3.投资100，支出39、 59、 55、 20。 折现率为28.1%，则净现值为多少？---计算npv``````print("npv:",np.npv(0.281,[-100,39,59,55,20]))----rate:折现率；values:现金流>>>npv: -0.00847859163845488``````4.某同学房贷20万，准备15年还清，年利率为7.5%，则每月需还贷多少金额 ----计算pmt``````print("每月需还贷金额为:",np.pmt(0.075/12,12*15,200000))>>>每月需还贷金额为: -1854.0247200054619``````5.某同学房贷20万，年利率为7.5%，每月能还贷2000，则需要还多少期?-------计算nper``````year = np.ceil(np.nper(0.075/12,-2000,200000)/12)------np.ceil：向上取整print("需要还贷{}年。".format(year))>>>需要还贷14.0年。```### 十二、广播> ```让两个不同维度的 array进行元素运算若维数较小的array增加1的维度，直到两者维数相等``````x = np.arange(6).reshape((2,3))        y = np.arange(9).reshape((3,3))        x[:,np.newaxis,:]+y        array([                [[0,2,4],                    [3,5,7],                [6,8,10]]                [[3,5,7],                    [6,8,10],                [9,11,13]]            ])```### 十三、array数组copy> 完全不拷贝               赋值时，只是引用```x = np.array([1,2,3,4])y = xy is xTrue``````浅拷贝(copy视图/投影)        视图会创建一个新 array 对象，但和原来的 array 共享同一份数据        创建视图：ndarray.view/通过切片        判断一个array是否为视图        ndarray.flags.owndata        示例：``````python#通过ndarray.view 创建x = np.array([1,2,3,4])y = x.view()print 'y.flags.owndata:',y.flags.owndataprint 'y.base is x:',y.base is xy.flags.owndata:Falsey.base is x:True``````深拷贝：完全copy。创建新拷贝--ndarray.copy()--通过索引数组和布尔数组获取新的array（又称为Fancy Indexing）```|inner   两个数组的内积|`np.inner(np.array([1,2,3]),np.array([0,1,0]))`;输出：`1*0+2*1+3*0`|matmul   两个数组的矩阵积|矩阵乘法|determinant   数组的行列式|行列式的det|solve   求解线性矩阵方程||inv   寻找矩阵的乘法逆矩阵||
|vdot   两个向量的点积|a = np.array([[1,2],[3,4]]) ,b = np.array([[11,12],[13,14]]) ;`np.vdot(a,b)`：`1*11 + 2*12 + 3*13 + 4*14 = 130`inner   两个数组的内积`np.inner(np.array([1,2,3]),np.array([0,1,0]))`;输出：`1*0+2*1+3*0`matmul   两个数组的矩阵积矩阵乘法determinant   数组的行列式行列式的detsolve   求解线性矩阵方程inv   寻找矩阵的乘法逆矩阵```print('------------逆矩阵与单位矩阵---------------')  import numpy as np  A = np.mat("0 1 2;1 0 3;4 -3 8")  print("A=",A)  inverse = np.linalg.inv(A)  print(u"A的逆矩阵：","\n",inverse)  I = A*inverse  print('单位矩阵:I = A*inverse：',"\n",I)  >>>  A= [[ 0  1  2]      [ 1  0  3]      [ 4 -3  8]]  A的逆矩阵：      [[-4.5  7.  -1.5]      [-2.   4.  -1. ]      [ 1.5 -2.   0.5]]  单位矩阵:I = A*inverse：      [[1. 0. 0.]      [0. 1. 0.]      [0. 0. 1.]]``````使用 np.linalg.solve(A, b) 求解线性方程组```![这里写图片描述](https://img-blog.csdn.net/20180601095335284?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)```A= np.mat("1 -2 1;0 2 -8;-4 5 9")  B= np.array([0,8,-9])  C = np.linalg.solve(A,B)  print("X=",C[0],"Y=", C[1],"Z=", C[2])  >>>  X= 29.0   Y= 16.0   Z= 3.0```#####  6.9 特征值、特征向量```Arr =np.mat("3 -2;1 0")print("单独求特征值：",np.linalg.eigvals(Arr))print("特征值，特征向量：",np.linalg.eig(Arr))>>>单独求特征值：           [2. 1.]特征值，特征向量： (array([2., 1.]),     matrix([[0.89442719, 0.70710678],            [0.4472136 , 0.70710678]]))```##### 6.10奇异值分解```print('----------------SVD奇异值分解---------------')Arr2 = np.mat("4 11 14;8 7 -2")U,Sigma,V = np.linalg.svd(Arr2,full_matrices=False)print("U",U)print("Sigma",Sigma)print("V",V)>>>U [[-0.9486833  -0.31622777]  [-0.31622777  0.9486833 ]] Sigma [18.97366596  9.48683298] V [[-0.33333333 -0.66666667 -0.66666667]  [ 0.66666667  0.33333333 -0.66666667]]```##### 6.11 计算矩阵的行列式```print('----------矩阵行列式的计算-----------')Arr3 = np.mat("3 4;5 6")print("矩阵行列式的计算：",np.linalg.det(Arr3))>>>矩阵行列式的计算： -2.0000000000000013```### 七、排序函数> 排序```7.1、 ndarray类的sort方法——可对数组进行原地排序； np.sort(-arr , axis=1)降序排序7.2、 argsort函数——返回输入数组排序后的下标；    np.argsort(-arr,axis=1)降序排序7.3、 sort函数——返回排序后的数组``````print('----------------ndarray排序-----------------')list1 = [[3,1,2],[5,7,0]]array1 = np.array(list1)array1.sort(axis=1)print(array1)>>>[[1 2 3] [0 5 7]]print("------------返回排序后的索引值----------------")list1 = [[3,1,2],[5,7,0]]array1 = np.array(list1)array2 = np.argsort(array1,axis=0)  #返回列索引array3 = np.argsort(array1,axis=1)  #返回行索引print("array2:",array2)print("array3:",array3)>>>array2:[[0 0 1]        [1 1 0]] array3:[[1 2 0]        [2 0 1]]```### 八、搜索函数> 常用搜索函数```◆ argmax函数：      返回数组中最大值对应的下标◆ nanargmax函数：   与argmax提供相同的功能，但忽略NaN值◆ argmin函数：      返回数组中最小值对应的下标◆ nanargmin函数：   与argmin的功能类似，     但忽略NaN值◆ argwhere函数：    根据条件搜索非零的元素，并分组返回对应的下标◆ searchsorted函数：为指定的插入值寻找维持数组排序的索引位置。该函数使用二分``````print('----------返回最大值的索引----------')arr_a = np.array([[3,1,2],[5,7,0]])print(np.argmax(arr_a))        #先将多维数组展平，再返回最大值的索引print("------argwhere返回对应的下标-----")arr_a = np.array([[3,1,2],[5,7,0]])print(np.argwhere(arr_a>3))>>>[[1 0]         #第二行第一列 [1 1]]        #第二行第二列print('-----searchsorted()为指定的插入值寻找维持数组排序的索引位置----')arr_b = np.arange(5)arr_insert = np.searchsorted(arr_b,[-1,7])print("插入后：",np.insert(arr_b,arr_insert,[-1,7]))>>>插入后： [-1  0  1  2  3  4  7]```### 九、数组元素抽取> ```9.1、 extract函数——根据某个条件从数组中抽取元素9.2、 nonzero函数——专门用来抽取非零的数组元素``````print('-----------元素抽取-------------')arr_ele = np.arange(8)condition =(arr_ele%2) ==0print("抽取能被2整除的元素：",np.extract(condition,arr_ele))print("非零值：",np.nonzero(arr_ele))>>抽取能被2整除的元素： [0 2 4 6]非零值： (array([1, 2, 3, 4, 5, 6, 7], dtype=int64),)print('-----------元素抽取(condition)-------------')arr_ele = np.arange(8)arr_fa = arr_ele.compress(condition=(arr_ele%2) ==0)print("抽取能被2整除的元素",arr_fa)print('-----------元素抽取(where)-------------')arr_ele = np.arange(8)arr_fa = np.where((arr_ele%2) ==0)print("抽取能被2整除的元素",arr_fa)print('---------非零元素索引提取----------')arr = np.array([[0,1,3],                [0,4,7],                [7,0,9]])row,col = np.nonzero(arr)print("非零元素行索引row_index",row)print("非零元素列索引col_index",col)```### 十、ndarray的文件IO> ##### 10.1.保存为二进制文件```保存单个 array( .npy文件)    np.save(file,arr,allow_pickle=True,fix_imports=True)        allow_pickle:允许使用pickle的方式保存对象。出于安全性和兼容性考虑，会禁用pickle        安全性：加载一个pickle的数据时，会执行任何代码        兼容性：pickle对象对版本有要求，不同版本python不兼容保存多个 array(.npz文件)    np.savez(file,*args,**kwargs)        *args:以列表参数的方式指定要保存的array，无法保存array变量名，加载时通过arr_0,arr_1这种方式获取。        **kwargs:以命名参数的方式指定array，加载时可以通过名称获取array保存多个array并压缩( .npz文件)    np.savez_compressed(file,*args,**kwargs)```##### 10.2 加载二进制文件```np.load(file,mmap_mode=None,allow_pickle=True,fix_imports=True,encoding='ASCII')       mmap_mode:可选值(None,'r+','r','w+','c')。如果不是None，则会用指定的模式把内存块映射为一个file对象。            可以实现访问array中的一部分而不用把整个array导入内存       fix_imports：只在python3中读取python2    生成含pickle对象的文件时有用       encoding：   只在python3中读取python2 生成含pickle对象的文件时有用                        可选值有('latin1','ASCII',bytes)np.load 可以支持。npy和,npz文件的读取       读取.npy文件 直接返回array       读取.npz文件，返回一个NpzFile对象（用完close），可以通过.key()方法查看包含的array```##### 10.3 保存为文本文件```np.savetxt(filename,array,fmt= '%.18e',delimiter='',            newline='\n',header='',footer='',comments='#')        -filename:  文件名，若以.gz结尾则自动以gzip压缩        -fmt:       保存格式        -delimiter：分隔符        -newline：  换行符        -header：   首行输出内容        -footer：   末行输出内容        -comments:  在header和footer前插入的字符，表示注释```##### 10.4 加载文本文件```np.loadtxt(fname,dtype=<type 'float'>,comments= '#',delimiter=None,concerters=None,skiprows=0,usecols=None,skiprows=0,uppack=False,ndmin=0)        comments :    在header和footer前插入的字符，表示注释        concerters :  转换函数字典，通过下标key,为每列定义一个转换函数        skiprows :    跳过开头的若干行        usecols :     使用指定列，下标0开始        uppack :      若为 True,可以用变量捕获每一列        ndmin :       最小的维度数``````import numpy as np■ 无参数写读arr1 = np.random.normal(size=20).reshape(4,5) #创建随机数矩阵np.savetxt('arr1_file.txt',arr1)              #默认分隔符：空格arr_data1 = np.loadtxt('arr_file.txt')■ 带参数写读arr2 = np.random.normal(size=20).reshape(4,5)           # 创建随机数矩阵np.savetxt('arr1_file.txt',arr2,fmt="%d",delimiter=",") # %d: 保存为整数，                                                        # 分隔符：逗号arr_data2 = np.loadtxt('arr2_file.txt',delimiter=",")■ 读取文件：arr3_file.csv文件内容如下："姓名","年龄","年级","体重","身高""李光",30,12,100,188"司马光",42,9,120,178"董卓",56,8,155,184"项羽",45,9,142,186arr3_data = np.loadtxt('arr3_file.csv',dtype = np.str,delimiter=",")arr3_num = arr3_data[1:,1:].astype(np.float)print(arr3_num)>>>[[ 30.  12. 100. 188.] [ 42.   9. 120. 178.] [ 56.   8. 155. 184.] [ 45.   9. 142. 186.] [ 65.   6. 152. 189.]]```###### [csv加载时间数据,请看这儿☛](https://blog.csdn.net/wsp_1138886114/article/details/80487035)### 十一、金融函数> ```fv  函数——计算所谓的终值（future value），某个金融资产在未来某一时间点的价值。pv  函数——计算现值（present value），即金融资产当前的价值。npv 函数——计算净现值（net present value），即按折现率计算的净现金流之和。pmt 函数——根据本金和利率计算每期需支付的金额。(payment)nper函数——计算定期付款的期数。rate函数——计算利率（rate of interest）。1. 某用户去银行存款，假设年利率3%、每季度续存金额    -------------计算fv    10元、存5年以及存款1000，则计算5年后可领取多少金额    分析：    rate : 0.03/4    nper : 5*4    pmt  : -10    pv   :  -1000            fv   :  存款终值是本息和        贷款终值是0``````pythonimport numpy as np                  print("Future value",np.fv(0.03/4,5*4,-10,-1000))      fvals = []for i in range(1,6):    fvals.append(np.fv(.03/4,i*4,-10,-1000))    print('第{}年的本息和是{}'.format(i,fvals[i-1]))>>>Future value 1376.0963320407982第1年的本息和是1070.7914448828128第2年的本息和是1143.7306449093103第3年的本息和是1218.8827612322955第4年的本息和是1296.3149319412119第5年的本息和是1376.0963320407982``````2.某用户去银行存款，假设年利率3%、每季度续存金额10--------------计算pv元、存5年后可领1376.0963320，则计算5年前存取的本金是多少金额``````pv = np.pv(0.03/4,5*4,-10,1376.096)print("当初存钱：",np.round(pv))    --np.round()四舍五入>>>当初存钱： -1000.0``````3.投资100，支出39、 59、 55、 20。 折现率为28.1%，则净现值为多少？---计算npv``````print("npv:",np.npv(0.281,[-100,39,59,55,20]))----rate:折现率；values:现金流>>>npv: -0.00847859163845488``````4.某同学房贷20万，准备15年还清，年利率为7.5%，则每月需还贷多少金额 ----计算pmt``````print("每月需还贷金额为:",np.pmt(0.075/12,12*15,200000))>>>每月需还贷金额为: -1854.0247200054619``````5.某同学房贷20万，年利率为7.5%，每月能还贷2000，则需要还多少期?-------计算nper``````year = np.ceil(np.nper(0.075/12,-2000,200000)/12)------np.ceil：向上取整print("需要还贷{}年。".format(year))>>>需要还贷14.0年。```### 十二、广播> ```让两个不同维度的 array进行元素运算若维数较小的array增加1的维度，直到两者维数相等``````x = np.arange(6).reshape((2,3))        y = np.arange(9).reshape((3,3))        x[:,np.newaxis,:]+y        array([                [[0,2,4],                    [3,5,7],                [6,8,10]]                [[3,5,7],                    [6,8,10],                [9,11,13]]            ])```### 十三、array数组copy> 完全不拷贝               赋值时，只是引用```x = np.array([1,2,3,4])y = xy is xTrue``````浅拷贝(copy视图/投影)        视图会创建一个新 array 对象，但和原来的 array 共享同一份数据        创建视图：ndarray.view/通过切片        判断一个array是否为视图        ndarray.flags.owndata        示例：``````python#通过ndarray.view 创建x = np.array([1,2,3,4])y = x.view()print 'y.flags.owndata:',y.flags.owndataprint 'y.base is x:',y.base is xy.flags.owndata:Falsey.base is x:True``````深拷贝：完全copy。创建新拷贝--ndarray.copy()--通过索引数组和布尔数组获取新的array（又称为Fancy Indexing）```|inner   两个数组的内积|`np.inner(np.array([1,2,3]),np.array([0,1,0]))`;输出：`1*0+2*1+3*0`|matmul   两个数组的矩阵积|矩阵乘法|determinant   数组的行列式|行列式的det|solve   求解线性矩阵方程||inv   寻找矩阵的乘法逆矩阵|| | |
|inner   两个数组的内积|`np.inner(np.array([1,2,3]),np.array([0,1,0]))`;输出：`1*0+2*1+3*0`| | | | | | | | | | | | |
|matmul   两个数组的矩阵积|矩阵乘法| | | | | | | | | | | | |
|determinant   数组的行列式|行列式的det| | | | | | | | | | | | |
|solve   求解线性矩阵方程|| | | | | | | | | | | | |
|inv   寻找矩阵的乘法逆矩阵|| | | | | | | | | | | | |

```
print('------------逆矩阵与单位矩阵---------------')  
import numpy as np  
A = np.mat("0 1 2;1 0 3;4 -3 8")  
print("A=",A)  
inverse = np.linalg.inv(A)  
print(u"A的逆矩阵：","\n",inverse)  

I = A*inverse  
print('单位矩阵:I = A*inverse：',"\n",I)  
>>>  
A= [[ 0  1  2]  
    [ 1  0  3]  
    [ 4 -3  8]]  
A的逆矩阵：   
   [[-4.5  7.  -1.5]  
    [-2.   4.  -1. ]  
    [ 1.5 -2.   0.5]]  
单位矩阵:I = A*inverse：   
   [[1. 0. 0.]  
    [0. 1. 0.]  
    [0. 0. 1.]]
```

```
使用 np.linalg.solve(A, b) 求解线性方程组
```

![这里写图片描述](https://img-blog.csdn.net/20180601095335284?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
A= np.mat("1 -2 1;0 2 -8;-4 5 9")  
B= np.array([0,8,-9])  

C = np.linalg.solve(A,B)  
print("X=",C[0],"Y=", C[1],"Z=", C[2])  
>>>  
X= 29.0   
Y= 16.0   
Z= 3.0
```

#####  6.9 特征值、特征向量

```
Arr =np.mat("3 -2;1 0")
print("单独求特征值：",np.linalg.eigvals(Arr))
print("特征值，特征向量：",np.linalg.eig(Arr))
>>>
单独求特征值：           [2. 1.]
特征值，特征向量： (array([2., 1.]), 
    matrix([[0.89442719, 0.70710678],
            [0.4472136 , 0.70710678]]))
```

##### 6.10奇异值分解

```
print('----------------SVD奇异值分解---------------')
Arr2 = np.mat("4 11 14;8 7 -2")
U,Sigma,V = np.linalg.svd(Arr2,full_matrices=False)
print("U",U)
print("Sigma",Sigma)
print("V",V)
>>>
U [[-0.9486833  -0.31622777]
  [-0.31622777  0.9486833 ]] 

Sigma [18.97366596  9.48683298] 

V [[-0.33333333 -0.66666667 -0.66666667]
  [ 0.66666667  0.33333333 -0.66666667]]
```

##### 6.11 计算矩阵的行列式

```
print('----------矩阵行列式的计算-----------')
Arr3 = np.mat("3 4;5 6")
print("矩阵行列式的计算：",np.linalg.det(Arr3))
>>>
矩阵行列式的计算： -2.0000000000000013
```

### 七、排序函数

> 
排序

```
7.1、 ndarray类的sort方法——可对数组进行原地排序； np.sort(-arr , axis=1)降序排序
7.2、 argsort函数——返回输入数组排序后的下标；    np.argsort(-arr,axis=1)降序排序
7.3、 sort函数——返回排序后的数组
```

```
print('----------------ndarray排序-----------------')
list1 = [[3,1,2],[5,7,0]]
array1 = np.array(list1)
array1.sort(axis=1)
print(array1)
>>>
[[1 2 3]
 [0 5 7]]

print("------------返回排序后的索引值----------------")
list1 = [[3,1,2],[5,7,0]]
array1 = np.array(list1)
array2 = np.argsort(array1,axis=0)  #返回列索引
array3 = np.argsort(array1,axis=1)  #返回行索引
print("array2:",array2)
print("array3:",array3)
>>>
array2:[[0 0 1]
        [1 1 0]] 

array3:[[1 2 0]
        [2 0 1]]
```


### 八、搜索函数

> 
常用搜索函数

```
◆ argmax函数：      返回数组中最大值对应的下标
◆ nanargmax函数：   与argmax提供相同的功能，但忽略NaN值
◆ argmin函数：      返回数组中最小值对应的下标
◆ nanargmin函数：   与argmin的功能类似，     但忽略NaN值
◆ argwhere函数：    根据条件搜索非零的元素，并分组返回对应的下标
◆ searchsorted函数：为指定的插入值寻找维持数组排序的索引位置。该函数使用二分
```

```
print('----------返回最大值的索引----------')
arr_a = np.array([[3,1,2],[5,7,0]])
print(np.argmax(arr_a))        #先将多维数组展平，再返回最大值的索引

print("------argwhere返回对应的下标-----")
arr_a = np.array([[3,1,2],[5,7,0]])
print(np.argwhere(arr_a>3))
>>>
[[1 0]         #第二行第一列
 [1 1]]        #第二行第二列

print('-----searchsorted()为指定的插入值寻找维持数组排序的索引位置----')
arr_b = np.arange(5)
arr_insert = np.searchsorted(arr_b,[-1,7])
print("插入后：",np.insert(arr_b,arr_insert,[-1,7]))
>>>
插入后： [-1  0  1  2  3  4  7]
```


### 九、数组元素抽取

> 
```
9.1、 extract函数——根据某个条件从数组中抽取元素
9.2、 nonzero函数——专门用来抽取非零的数组元素
```

```
print('-----------元素抽取-------------')
arr_ele = np.arange(8)
condition =(arr_ele%2) ==0
print("抽取能被2整除的元素：",np.extract(condition,arr_ele))
print("非零值：",np.nonzero(arr_ele))
>>
抽取能被2整除的元素： [0 2 4 6]
非零值： (array([1, 2, 3, 4, 5, 6, 7], dtype=int64),)

print('-----------元素抽取(condition)-------------')
arr_ele = np.arange(8)
arr_fa = arr_ele.compress(condition=(arr_ele%2) ==0)
print("抽取能被2整除的元素",arr_fa)

print('-----------元素抽取(where)-------------')
arr_ele = np.arange(8)
arr_fa = np.where((arr_ele%2) ==0)
print("抽取能被2整除的元素",arr_fa)

print('---------非零元素索引提取----------')
arr = np.array([[0,1,3],
                [0,4,7],
                [7,0,9]])
row,col = np.nonzero(arr)
print("非零元素行索引row_index",row)
print("非零元素列索引col_index",col)
```


### 十、ndarray的文件IO

> 
##### 10.1.保存为二进制文件

```
保存单个 array( .npy文件)
    np.save(file,arr,allow_pickle=True,fix_imports=True)
        allow_pickle:允许使用pickle的方式保存对象。出于安全性和兼容性考虑，会禁用pickle
        安全性：加载一个pickle的数据时，会执行任何代码
        兼容性：pickle对象对版本有要求，不同版本python不兼容
保存多个 array(.npz文件)
    np.savez(file,*args,**kwargs)
        *args:以列表参数的方式指定要保存的array，无法保存array变量名，加载时通过arr_0,arr_1这种方式获取。
        **kwargs:以命名参数的方式指定array，加载时可以通过名称获取array
保存多个array并压缩( .npz文件)
    np.savez_compressed(file,*args,**kwargs)
```

##### 10.2 加载二进制文件

```
np.load(file,mmap_mode=None,allow_pickle=True,fix_imports=True,encoding='ASCII')
       mmap_mode:可选值(None,'r+','r','w+','c')。如果不是None，则会用指定的模式把内存块映射为一个file对象。
            可以实现访问array中的一部分而不用把整个array导入内存
       fix_imports：只在python3中读取python2    生成含pickle对象的文件时有用
       encoding：   只在python3中读取python2 生成含pickle对象的文件时有用
                        可选值有('latin1','ASCII',bytes)
np.load 可以支持。npy和,npz文件的读取
       读取.npy文件 直接返回array
       读取.npz文件，返回一个NpzFile对象（用完close），可以通过.key()方法查看包含的array
```

##### 10.3 保存为文本文件

```
np.savetxt(filename,array,fmt= '%.18e',delimiter='',
            newline='\n',header='',footer='',comments='#')
        -filename:  文件名，若以.gz结尾则自动以gzip压缩
        -fmt:       保存格式
        -delimiter：分隔符
        -newline：  换行符
        -header：   首行输出内容
        -footer：   末行输出内容
        -comments:  在header和footer前插入的字符，表示注释
```

##### 10.4 加载文本文件

```
np.loadtxt(fname,dtype=<type 'float'>,comments= '#',delimiter=None,
concerters=None,skiprows=0,usecols=None,skiprows=0,uppack=False,ndmin=0)
        comments :    在header和footer前插入的字符，表示注释
        concerters :  转换函数字典，通过下标key,为每列定义一个转换函数
        skiprows :    跳过开头的若干行
        usecols :     使用指定列，下标0开始
        uppack :      若为 True,可以用变量捕获每一列
        ndmin :       最小的维度数
```

```
import numpy as np
■ 无参数写读
arr1 = np.random.normal(size=20).reshape(4,5) #创建随机数矩阵
np.savetxt('arr1_file.txt',arr1)              #默认分隔符：空格
arr_data1 = np.loadtxt('arr_file.txt')

■ 带参数写读
arr2 = np.random.normal(size=20).reshape(4,5)           # 创建随机数矩阵
np.savetxt('arr1_file.txt',arr2,fmt="%d",delimiter=",") # %d: 保存为整数，
                                                        # 分隔符：逗号
arr_data2 = np.loadtxt('arr2_file.txt',delimiter=",")

■ 读取文件：
arr3_file.csv文件内容如下：
"姓名","年龄","年级","体重","身高"
"李光",30,12,100,188
"司马光",42,9,120,178
"董卓",56,8,155,184
"项羽",45,9,142,186

arr3_data = np.loadtxt('arr3_file.csv',dtype = np.str,delimiter=",")
arr3_num = arr3_data[1:,1:].astype(np.float)
print(arr3_num)
>>>
[[ 30.  12. 100. 188.]
 [ 42.   9. 120. 178.]
 [ 56.   8. 155. 184.]
 [ 45.   9. 142. 186.]
 [ 65.   6. 152. 189.]]
```

###### [csv加载时间数据,请看这儿☛](https://blog.csdn.net/wsp_1138886114/article/details/80487035)


### 十一、金融函数

> 
```
fv  函数——计算所谓的终值（future value），某个金融资产在未来某一时间点的价值。
pv  函数——计算现值（present value），即金融资产当前的价值。
npv 函数——计算净现值（net present value），即按折现率计算的净现金流之和。
pmt 函数——根据本金和利率计算每期需支付的金额。(payment)
nper函数——计算定期付款的期数。
rate函数——计算利率（rate of interest）。

1. 某用户去银行存款，假设年利率3%、每季度续存金额    -------------计算fv 
   10元、存5年以及存款1000，则计算5年后可领取多少金额
    分析：
    rate : 0.03/4
    nper : 5*4
    pmt  : -10
    pv   :  -1000        
    fv   :  存款终值是本息和        贷款终值是0
```

```python
import numpy as np                  
print("Future value",np.fv(0.03/4,5*4,-10,-1000))      
fvals = []
for i in range(1,6):
    fvals.append(np.fv(.03/4,i*4,-10,-1000))
    print('第{}年的本息和是{}'.format(i,fvals[i-1]))
>>>
Future value 1376.0963320407982
第1年的本息和是1070.7914448828128
第2年的本息和是1143.7306449093103
第3年的本息和是1218.8827612322955
第4年的本息和是1296.3149319412119
第5年的本息和是1376.0963320407982
```

```
2.某用户去银行存款，假设年利率3%、每季度续存金额10--------------计算pv
元、存5年后可领1376.0963320，则计算5年前存取的本金是多少金额
```

```
pv = np.pv(0.03/4,5*4,-10,1376.096)
print("当初存钱：",np.round(pv))    --np.round()四舍五入
>>>
当初存钱： -1000.0
```

```
3.投资100，支出39、 59、 55、 20。 折现率为28.1%，则净现值为多少？---计算npv
```

```
print("npv:",np.npv(0.281,[-100,39,59,55,20]))----rate:折现率；values:现金流
>>>
npv: -0.00847859163845488
```

```
4.某同学房贷20万，准备15年还清，年利率为7.5%，则每月需还贷多少金额 ----计算pmt
```

```
print("每月需还贷金额为:",np.pmt(0.075/12,12*15,200000))
>>>
每月需还贷金额为: -1854.0247200054619
```

```
5.某同学房贷20万，年利率为7.5%，每月能还贷2000，则需要还多少期?-------计算nper
```

```
year = np.ceil(np.nper(0.075/12,-2000,200000)/12)------np.ceil：向上取整
print("需要还贷{}年。".format(year))
>>>
需要还贷14.0年。
```


### 十二、广播

> 
```
让两个不同维度的 array进行元素运算
若维数较小的array增加1的维度，直到两者维数相等
```

```
x = np.arange(6).reshape((2,3))
        y = np.arange(9).reshape((3,3))
        x[:,np.newaxis,:]+y
        array([
                [[0,2,4],    
                [3,5,7],
                [6,8,10]]

                [[3,5,7],    
                [6,8,10],
                [9,11,13]]
            ])
```


### 十三、array数组copy

> 
完全不拷贝 

              赋值时，只是引用

```
x = np.array([1,2,3,4])
y = x
y is x

True
```

```
浅拷贝(copy视图/投影)
        视图会创建一个新 array 对象，但和原来的 array 共享同一份数据
        创建视图：ndarray.view/通过切片

        判断一个array是否为视图
        ndarray.flags.owndata
        示例：
```

```python
#通过ndarray.view 创建
x = np.array([1,2,3,4])
y = x.view()
print 'y.flags.owndata:',y.flags.owndata
print 'y.base is x:',y.base is x

y.flags.owndata:False
y.base is x:True
```

```
深拷贝：完全copy。
创建新拷贝
--ndarray.copy()
--通过索引数组和布尔数组获取新的array（又称为Fancy Indexing）
```





