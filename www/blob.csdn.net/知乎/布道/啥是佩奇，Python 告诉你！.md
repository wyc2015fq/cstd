# 啥是佩奇，Python 告诉你！ - 知乎
# 



今天，

被《啥是佩奇》这支广告片刷屏了。

佩奇明明是个喜剧角色，

却看哭了所有人。

**《啥是佩奇》？？？**
[今天听到的最暖的一句话：啥是佩奇  农村老爷爷这么努力_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/m0828x153iv.html)
> 快过年了，
在农村爷爷给城里的小孙子打电话，
小孙子说想要“佩奇”，
为了满足小孙子的愿望，
爷爷开始满村子找佩奇。
开片爷爷在电话这头的一段话就很抓人心，
全世界的爷爷都是这样疼孙儿的，
一下子就让观众有共情的心理。
一句“什么是佩奇？”
铺垫了爷爷接下来的一段寻找佩奇之路，
最后爷爷找到的佩奇骨络清奇，
却是圈主见过的全世界最可爱的小猪佩奇！
![](https://pic1.zhimg.com/v2-709eaafbd72a6e003f43f83e363dfba0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='595'></svg>)
不知道大家看完什么感觉，反正我看完是哭了。我看网友们也纷纷留言，说自己哭笑两重奏......

看着爷爷满村子找佩奇，我有点心疼了。为此我想用纯粹的Python来告诉爷爷，啥是佩奇？

**这就是佩奇！**

先看看效果视频：
[小猪佩奇社会人_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/c1341vh69yx.html)
基本思路：选好画板大小，设置好画笔颜色，粗细，定位好位置，依次画鼻子，头、耳朵，眼睛，腮，嘴，身体，手脚，尾巴，完事。

都知道，turtle 是 Python 内置的一个比较有趣味的模块，俗称海龟绘图，它是基于 tkinter 模块打造，提供一些简单的绘图工具。

在海龟作图中，我们可以编写指令让一个虚拟的（想象中的）海龟在屏幕上来回移动。这个海龟带着一只钢笔，我们可以让海龟无论移动到哪都使用这只钢笔来绘制线条。通过编写代码，以各种很酷的模式移动海龟，我们可以绘制出令人惊奇的图片。使用海龟作图，我们不仅能够只用几行代码就创建出令人印象深刻的视觉效果，而且还可以跟随海龟看看每行代码如何影响到它的移动。这能够帮助我们理解代码的逻辑。所以海龟作图也常被用作新手学习 Python 的一种方式。更丰富详细的功能及知识可以参考官方文档：[https://docs.python.org/3/library/turtle.html](https://link.zhihu.com/?target=https%3A//docs.python.org/3/library/turtle.html)。

了解了tuttle的用法之后就可以开始实战了。

代码示例：


```
from turtle import*

def nose(x,y):#鼻子
    penup()#提起笔
    goto(x,y)#定位
    pendown()#落笔，开始画
    setheading(-30)#将乌龟的方向设置为to_angle/为数字（0-东、90-北、180-西、270-南）
    begin_fill()#准备开始填充图形
    a=0.4
    for i in range(120):
        if 0<=i<30 or 60<=i<90:
            a=a+0.08
            left(3) #向左转3度
            forward(a) #向前走a的步长
        else:
            a=a-0.08
            left(3)
            forward(a)
    end_fill()#填充完成

    penup()
    setheading(90)
    forward(25)
    setheading(0)
    forward(10)
    pendown()
    pencolor(255,155,192)#画笔颜色
    setheading(10)
    begin_fill()
    circle(5)
    color(160,82,45)#返回或设置pencolor和fillcolor
    end_fill()

    penup()
    setheading(0)
    forward(20)
    pendown()
    pencolor(255,155,192)
    setheading(10)
    begin_fill()
    circle(5)
    color(160,82,45)
    end_fill()

def head(x,y):#头
    color((255,155,192),"pink")
    penup()
    goto(x,y)
    setheading(0)
    pendown()
    begin_fill()
    setheading(180)
    circle(300,-30)
    circle(100,-60)
    circle(80,-100)
    circle(150,-20)
    circle(60,-95)
    setheading(161)
    circle(-300,15)
    penup()
    goto(-100,100)
    pendown()
    setheading(-30)
    a=0.4
    for i in range(60):
        if 0<=i<30 or 60<=i<90:
            a=a+0.08
            lt(3) #向左转3度
            fd(a) #向前走a的步长
        else:
            a=a-0.08
            lt(3)
            fd(a)
    end_fill()

def cheek(x,y):#腮
    color((255,155,192))
    penup()
    goto(x,y)
    pendown()
    setheading(0)
    begin_fill()
    circle(30)
    end_fill()

def mouth(x,y): #嘴
    color(239,69,19)
    penup()
    goto(x,y)
    pendown()
    setheading(-80)
    circle(30,40)
    circle(40,80)

def setting():          #参数设置
    pensize(4)
    hideturtle()        #使乌龟无形（隐藏）
    colormode(255)      #将其设置为1.0或255.随后 颜色三元组的r，g，b值必须在0 .. cmode范围内
    color((255,155,192),"pink")
    setup(840,500)
    speed(10)

def main():
    setting()           #画布、画笔设置
    nose(-100,100)      #鼻子
    head(-69,167)       #头
    ears(0,160)         #耳朵
    eyes(0,140)         #眼睛
    cheek(80,10)        #腮
    mouth(-20,30)       #嘴
    done()

if __name__ == '__main__':
    main()
```


思路其实很简单，就是通过trutle模块实现基本的圆，椭圆，曲线等，难点在于，如何定位每个部位的位置（建议先草图画画）。
![](https://pic1.zhimg.com/v2-3f5abf484f65476e0624b0a2378d3dbc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1440'></svg>)
> 作者：丁彦军，一名痴恋于Python的码农，公共帐号：恋习Python，在这里我们一起用Python做些有意义的事。
声明：本文为作者投稿，版权归其个人所有。


