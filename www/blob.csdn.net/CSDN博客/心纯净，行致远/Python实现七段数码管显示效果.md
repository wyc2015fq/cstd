# Python实现七段数码管显示效果 - 心纯净，行致远 - CSDN博客





2019年02月24日 08:27:41[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：74标签：[Python																[turtle](https://so.csdn.net/so/search/s.do?q=turtle&t=blog)](https://so.csdn.net/so/search/s.do?q=Python&t=blog)
个人分类：[【Python】](https://blog.csdn.net/zhanshen112/article/category/7930576)








Python实现七段数码管显示效果，可以实现显示年月日，但是不能动态地显示秒数的变化。因为海归作图的速度太慢，无法满足秒数的刷新要求

```python
#8DrawShuMaGuanV1.py
import turtle,time      #引入海龟作图体系和时间库
def DrawGap():  #绘制数码管之间的gap
    turtle.pu()
    turtle.fd(5)
def DrawLine(draw): #绘制单段数码管
    DrawGap()
    turtle.pendown() if draw else turtle.penup()
    turtle.fd(40)
    DrawGap()
    turtle.right(90)
def DrawDigit(digit):   #利用单段数码管遍历整个数码管，实现数字
    DrawLine(True) if digit in [2,3,4,5,6,8,9] else DrawLine(False)
    DrawLine(True) if digit in [0,1,3,4,5,6,7,8,9] else DrawLine(False)
    DrawLine(True) if digit in [0,2,3,5,6,8,9] else DrawLine(False)
    DrawLine(True) if digit in [0,2,6,8] else DrawLine(False)
    turtle.left(90)
    DrawLine(True) if digit in [0,4,5,6,8,9] else DrawLine(False)
    DrawLine(True) if digit in [0,2,3,5,6,7,8,9] else DrawLine(False)
    DrawLine(True) if digit in [0,1,2,3,4,7,8,9] else DrawLine(False)
    turtle.left(180)
    turtle.pu()
    turtle.fd(20)
def DrawDate(date): #data为日期，格式：'%Y-%m=%d+'
    turtle.pencolor('red')
    for i in date:
        if i =='-':
            turtle.write('年',font=('Arial',18,'normal'))
            turtle.pencolor('green')
            turtle.pu()
            turtle.fd(40)
        elif i =='=':
            turtle.write('月',font=('Arial',18,'normal'))
            turtle.pencolor('blue')
            turtle.pu()
            turtle.fd(40)
        elif i =='+':
            turtle.write('日',font=('Arial',18,'normal'))
        else:
            DrawDigit(eval(i))
def main(): #调用
    turtle.setup(800,350,200,200)
    turtle.penup()
    turtle.fd(-350)
    turtle.pensize(5)
    turtle.pencolor('green')
    DrawDate(time.strftime('%Y-%m=%d+',time.gmtime()))
    turtle.hideturtle()
    turtle.done()
main()
```

显示效果为：

![](https://img-blog.csdnimg.cn/20190224082554566.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)






