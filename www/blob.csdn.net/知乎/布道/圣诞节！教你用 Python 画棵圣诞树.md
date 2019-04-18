# 圣诞节！教你用 Python 画棵圣诞树 - 知乎
# 



如何用Python画一个圣诞树呢？

最简单：


```
height = 5

stars = 1
for i in range(height):
    print((' ' * (height - i)) + ('*' * stars))
    stars += 2
print((' ' * height) + '|')
```


效果：



![](https://pic4.zhimg.com/v2-8913a6cccf4d598bcbfb22b77a1a12a7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='110' height='147'></svg>)
哈哈哈哈，总有一种骗了大家的感觉。

其实本文是想介绍Turtle库来画圣诞树。

方法一：


```
import turtle
 screen = turtle.Screen()
 screen.setup(800,600)
 circle = turtle.Turtle()
 circle.shape('circle')
 circle.color('red')
 circle.speed('fastest')
 circle.up()
 square = turtle.Turtle()
square.shape('square')
square.color('green')
square.speed('fastest')
square.up()
circle.goto(0,280)
circle.stamp()
k = 0
for i in range(1, 17):
    y = 30*i
    for j in range(i-k):
        x = 30*j
        square.goto(x,-y+280)
        square.stamp()
        square.goto(-x,-y+280)
        square.stamp()
    if i % 4 == 0:
        x = 30*(j+1)
        circle.color('red')
        circle.goto(-x,-y+280)
        circle.stamp()
        circle.goto(x,-y+280)
        circle.stamp()
        k += 2
    if i % 4 == 3:
        x = 30*(j+1)
        circle.color('yellow')
        circle.goto(-x,-y+280)
        circle.stamp()
        circle.goto(x,-y+280)
        circle.stamp()
square.color('brown')
for i in range(17,20):
    y = 30*i
    for j in range(3):
        x = 30*j
        square.goto(x,-y+280)
        square.stamp()
        square.goto(-x,-y+280)
        square.stamp()
turtle.exitonclick()
```


效果：
![](https://pic2.zhimg.com/v2-25a94240b2e5da0f37622f6232d79611_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='963' height='922'></svg>)



方法二：


```
from turtle import *
 import random
 import time

 n = 80.0
 speed("fastest")
 screensize(bg='seashell')
 left(90)
forward(3*n)
color("orange", "yellow")
begin_fill()
left(126)
for i in range(5):
    forward(n/5)
    right(144)
    forward(n/5)
    left(72)
end_fill()
right(126)

color("dark green")
backward(n*4.8)
def tree(d, s):
    if d <= 0: return
    forward(s)
    tree(d-1, s*.8)
    right(120)
    tree(d-3, s*.5)
    right(120)
    tree(d-3, s*.5)
    right(120)
    backward(s)
tree(15, n)
backward(n/2)

for i in range(200):
    a = 200 - 400 * random.random()
    b = 10 - 20 * random.random()
    up()
    forward(b)
    left(90)
    forward(a)
    down()
    if random.randint(0, 1) == 0:
            color('tomato')
    else:
        color('wheat')
    circle(2)
    up()
    backward(a)
    right(90)
    backward(b)
time.sleep(60)
```


效果：
![](https://pic3.zhimg.com/v2-927ed02a7ba740f6710ad172a6e1b226_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='932' height='775'></svg>)
感兴趣的话就赶紧尝试一下吧！

祝大家平安夜快乐！！！圣诞节快乐！！！
![](https://pic1.zhimg.com/v2-5e2678cf4bc8ad081088380bd05d3f30_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='64' height='64'></svg>)![](https://pic1.zhimg.com/v2-5e2678cf4bc8ad081088380bd05d3f30_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='64' height='64'></svg>)![](https://pic1.zhimg.com/v2-5e2678cf4bc8ad081088380bd05d3f30_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='64' height='64'></svg>)



> 作者：糖甜甜甜，985高校经管研二，擅长用Python、R、tableau等工具结合统计学和机器学习模型做数据分析。个人公众号：经管人学数据分析（ID:DAT-2017）

声明：本文为作者投稿，版权归对方所有。


