# Python中DataFrame按照行遍历 - Machine Learning with Peppa - CSDN博客





2018年07月15日 13:29:15[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：542








在做分类模型时候，需要在DataFrame中按照行获取数据以便于进行训练和测试。

```
import pandas as pd

dict=[[1,2,3,4,5,6],[2,3,4,5,6,7],[3,4,5,6,7,8],[4,5,6,7,8,9],[5,6,7,8,9,10]]
data=pd.DataFrame(dict)
print(data)
for indexs in data.index:
    print(data.loc[indexs].values[0:-1])
```
- 


实验结果：

```
/usr/bin/python3.4 /home/ubuntu/PycharmProjects/pythonproject/findgaoxueya/test.py
   0  1  2  3  4   5
0  1  2  3  4  5   6
1  2  3  4  5  6   7
2  3  4  5  6  7   8
3  4  5  6  7  8   9
4  5  6  7  8  9  10
[1 2 3 4 5]
[2 3 4 5 6]
[3 4 5 6 7]
[4 5 6 7 8]
[5 6 7 8 9]

Process finished with exit code 0
```



