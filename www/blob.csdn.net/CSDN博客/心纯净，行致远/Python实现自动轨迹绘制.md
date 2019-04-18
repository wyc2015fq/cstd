# Python实现自动轨迹绘制 - 心纯净，行致远 - CSDN博客





2019年02月25日 10:56:27[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：324








### code:

```python
#AutoTraceDraw.py
import turtle as t
t.title('自动轨迹绘制')
t.setup(800,600,0,0)
t.pencolor("red")
t.pensize(5)
#数据读取
datals=[]   #定义列表
f=open("data.txt")
for line in f:
    line=line.replace("\n","")
    datals.append(list(map(eval,line.split("，"))))
f.close()
#自动绘制
for i in range(len(datals)):
    t.pencolor(datals[i][3],datals[i][4],datals[i][5])
    t.fd(datals[i][0])
    if datals[i][1]:
        t.right(datals[i][2])
    else:
        t.left(datals[i][2])
```

### 代码运行需要data.txt，文件内容如下：

300，0，144，1，0，0

300，0，144，0，1，0

300，0，144，0，0，1

300，0，144，1，1，0

300，0，144，0，1，1

184，0，72，1，0，1

184，0，72，0，0，0

184，0，72，0，0，0

184，0，72，0，0，0

184，1，72，1，0，1

184，1，72，0，0，0

184，1，72，0，0，0

184，1，72，0，0，0

184，1，72，0，0，0

184，1，720，0，0，0

### 运行结果：

![](https://img-blog.csdnimg.cn/20190225105535143.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)





