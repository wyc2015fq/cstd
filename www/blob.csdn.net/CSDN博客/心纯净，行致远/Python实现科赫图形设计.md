# Python实现科赫图形设计 - 心纯净，行致远 - CSDN博客





2019年02月24日 21:52:37[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：74








```python
#KochDrawV1.py
import turtle
def koch(size,n):
    if n==0:
        turtle.fd(size)
    else:
        for i in [0,90,-90,-90,90]:
            turtle.left(i)
            koch(size/3,n-1)
def main():
    turtle.setup(800,600)
    turtle.pu()
    turtle.goto(-200,100)
    turtle.pensize(3)
    turtle.pd()
    level=1
    bianshu=3
    koch(400,level)
    turtle.right(360/bianshu)
    koch(400,level)
    turtle.right(360/bianshu)
    koch(400,level)
    turtle.hideturtle
main()
```

![](https://img-blog.csdnimg.cn/20190224215207705.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)





