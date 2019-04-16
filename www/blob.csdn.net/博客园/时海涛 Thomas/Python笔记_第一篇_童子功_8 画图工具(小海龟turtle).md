# Python笔记_第一篇_童子功_8.画图工具(小海龟turtle) - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Python笔记_第一篇_童子功_8.画图工具(小海龟turtle)](https://www.cnblogs.com/noah0532/p/8440357.html)





　　turtle 是一个简单的绘图工具。　

　　提供一个小海龟，可以把它理解为一个机器人，只能听懂有限的命令，且绘图窗口的原点(0,0)在中间，默认海龟的方向是右侧
海龟的命令包括三类：**运动命令、笔画控制命令、其他命令**。

**1.   运动命令**
    forward(d)：向前移动d长度（右侧开始）
    backward(d): 向后移动d长度
    right(d): 向右旋转多少度
    left(d): 向左旋转多少度
    goto(x,y)：移动到指定的(x,y)坐标轴的位置
    turtle.clear() # 情况窗口，不会重置turtle
    turtle.reset() # 回复所有设置，清空窗口，重置turtle状态
    speed(): 小海龟移动的速度

**2.   笔画控制命令**
    circle(r,e) 绘制一个圆形，r为半径，e为度，满圆是365度,还可以steps= 4，用步数来表示。对应的这三个值分别为：radius(半径);extent(弧度) (optional);steps (optional) (做半径为radius的圆的内切正多边形,多边形边数为steps)
    up()  笔画抬起，在移动的是后不再画图
    down() 笔画方向
    setheading(d) 重置小海龟的方向
    pensize(width) 画笔的宽度
    pencolor(colorstr) 画笔的颜色

    begin_fill()  # 填充命令开始
    fillcolor(colorstr) # 填充颜色
    end_fill() # 结束填充

**3.   其他命令**
    done(): 程序继续执行
    undo(): 撤销上一次动作
    hideturtle(): 隐藏海龟
    showturtle(): 显示海龟
    screensize(x,y): 屏幕大小



**举例1：**

```
1 turtle.screensize(1000, 100) # 设置屏幕大小
 2 turtle.forward(10) # 向前移动（右）10步
 3 turtle.undo() # 撤销上一步
 4 turtle.forward(100) # 向前移动（右）100步
 5 turtle.undo() # 撤销上一步
 6 turtle.hideturtle() # 隐藏小海龟（光标）
 7 turtle.showturtle() # 展现小海龟（光标）
 8 turtle.clear() # 情况窗口，不会重置turtle
 9 turtle.reset() # 回复所有设置，清空窗口，重置turtle状态
10 turtle.done() # 继续执行——这个一般加载在小海龟的末尾，保存图形保存。
```



**举例2：相关的运行指令，用forward 和 left命令画一个矩形。**

```
1 # 举例2：
 2 # 相关的运动命令 用forward 和 left命令画一个矩形
 3 turtle.forward(100)
 4 turtle.left(90)
 5 turtle.forward(100)
 6 turtle.left(90)
 7 turtle.forward(100)
 8 turtle.left(90)
 9 turtle.forward(100)
10 turtle.hideturtle()
11 turtle.done()
```

　　显示图形：

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180211024706295-1561126390.png)



**举例3：**

```
1 turtle.backward(50) # 向后移动
2 turtle.right(50) # 向右旋转多少度
3 turtle.goto(50, 100) # 移动到指定的(x, y)
4 turtle.speed(50) # 小海龟移动的速度
5 turtle.done()
```

　　显示图形：

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180211024803795-2058356303.png)



**　　举例4：**

```
1 turtle.speed(1.5)
 2 turtle.pensize(10)
 3 turtle.pencolor("black")
 4 turtle.begin_fill()  # 开始填充
 5 turtle.circle(200, steps= 10)
 6 turtle.fillcolor("blue")  # 填充颜色
 7 turtle.end_fill()  # 结束填充
 8 turtle.up()
 9 turtle.down()
10 turtle.setheading(30)
11 turtle.done()
```

　　显示图形：

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180211024849966-1610026556.png)














