# Python中读取txt文件的三种可行办法 - 技术分享 - CSDN博客

2018年11月08日 11:51:49[丿一叶秋丶](https://me.csdn.net/zhuqiang9607)阅读数：640


DataTest.txt中的文件内容，文件最后尽量不要留空行，否则有的时候会出现error

```
1,2,3
4,5,6
7,8,9
```

## 第一种方式：使用 csv.reader()读取txt文件

```
import csv

data = []
with open('E:/DataTest.txt', 'rt') as csvfile:
    reader = csv.reader(csvfile, delimiter=',')
    for row in reader:
        data.append(row)
    #输出结果是列表
    print(data)
```

输出结果：

```
[['1', '2', '3'], ['4', '5', '6'], ['7', '8', '9']]
```

## 第二种方式：使用numpy.loadtxt()读取txt文件

```
import numpy as np  

data= np.loadtxt('E:/DataTest.txt',delimiter=',') 
#输出结果是numpy中数组格式
print(data)
```

输出结果：

```
[[1. 2. 3.]
 [4. 5. 6.]
 [7. 8. 9.]]
```

不过在后面添加如下语句都可以转换成DataFrame格式：

```
df = pd.DataFrame(data)  
df.to_csv()
print(df)
```

输出结果：

```
0    1    2
0  1.0  2.0  3.0
1  4.0  5.0  6.0
2  7.0  8.0  9.0
```

## 第三种方式：使用pandas.red_csv()读取txt文件

```
import pandas as pd  

data= pd.read_csv('E:/DataTest.txt',names=['0', '1', '2'])
#输出结果是numpy中数组格式
print(data)
```

输出结果：

```
0  1  2
0  1  2  3
1  4  5  6
2  7  8  9
```

