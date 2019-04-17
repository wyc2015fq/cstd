# 如何用Python开发拼图游戏 - zhusongziye的博客 - CSDN博客





2018年11月25日 10:12:22[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：113








# **一、效果的演示**



![](https://img-blog.csdnimg.cn/20181125101059962.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



![](https://img-blog.csdnimg.cn/20181125101121884.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

# **小编用了91步才完成这拼图...你会用多少步呢？**

# **二、 游戏的玩法**

思路：先设置一个棋盘，棋盘里面有我们的图像，在图像里面我们有一个个的小方块，通过这些错乱的小方块拼接图板。拼接的过程就是鼠标点击事件的一个过程

# **三、具体的实现步骤**

1.设置图像

2.定义一个图像块的类

3.定义一个方法开始拼接图板

4.重置游戏

5.绘制游戏界面各元素

6.定义鼠标的点击事件

7.创建框架

8.注册鼠标事件

9.初始化游戏

10.启动框架





**环境：Python 3.6 + Windows**

**IDE： sublime txt3**

**使用到的模块：Simpleguitk**

**安装模块：pip install simpleguitk**



**Python代码**



```
#!/usr/bin/env python
# -*- coding: utf-8 -*-

import simpleguitk as simplegui
import random

byamax = simplegui.load*_image('https://timgsa.baidu.com/timg?image&quality=80&size=b9999_*10000&sec=1523374883465&di=d0545c2c8adb05310f4f56a35e2c6976&imgtype=0&src=http%3A%2F%2Fimg01.taopic.com%2F160625%2F235106-1606250Q05845.jpg')



WIDTH = 600
HEIGHT = WIDTH+100

IMAGE_SIZE = WIDTH/3

all*_coordinates = [[IMAGE_*SIZE**0.5, IMAGE_SIZE**0.5], [IMAGE*_SIZE*1.5, IMAGE_*SIZE*0.5],
                        [IMAGE*_SIZE*2.5, IMAGE_*SIZE**0.5], [IMAGE_SIZE**0.5, IMAGE_SIZE*1.5],
                        [IMAGE*_SIZE*1.5, IMAGE_*SIZE**1.5], [IMAGE_SIZE**2.5, IMAGE_SIZE*1.5],
                        [IMAGE*_SIZE*0.5, IMAGE_*SIZE**2.5], [IMAGE_SIZE**1.5, IMAGE_SIZE*2.5], None]


ROWS = 3
COLS = 3

steps = 0

board = [[None,None,None],[None,None,None],[None,None,None]]

class Square:

    def **__init__**(self,coordinage):
        self.center = coordinage

    def draw(self,canvas,board_pos):

        canvas.draw*_image(byamax,self.center,[IMAGE_*SIZE,IMAGE_SIZE],
                          [(board*_pos[1]+0.5)*IMAGE_*SIZE,(board*_pos[0]+0.5)*IMAGE_*SIZE],[IMAGE*_SIZE,IMAGE_*SIZE])



def init_board():

    random.shuffle(all_coordinates)

    for i in range(ROWS):
        for j in range(COLS):

            idx = i * ROWS + j
            square*_center = all_*coordinates[idx]

            if square_center is None:
                board[i][j] = None
            else:
                board[i][j] = Square(square_center)


def play_game():

    global steps
    steps = 0
    init_board()

def draw(canvas):

    canvas.draw_image(byamax,[WIDTH/2,WIDTH/2],[WIDTH,WIDTH],[50,WIDTH+50],[98,98])

    canvas.draw_text('步数：'+str(steps),[400,680],22,'White')

    for i in range(ROWS):
        for j in range(COLS):
            if board[i][j] is not None:
                board[i][j].draw(canvas,[i,j])



def mouseclick(pos):

    global steps

    r = int(pos[1]//IMAGE_SIZE)
    c = int(pos[0]//IMAGE_SIZE)

    if r<3 and c<3:
        if board[r][c] is None:
            return
        else:
            current_square = board[r][c]
            if r - 1 >= 0 and board[r - 1][c] is None:  # 判断上面
                board[r][c] = None
                board[r - 1][c] = current_square
                steps += 1
            elif c + 1 <= 2 and board[r][c + 1] is None:  # 判断右面
                board[r][c] = None
                board[r][c + 1] = current_square
                steps += 1
            elif r + 1 <= 2 and board[r + 1][c] is None:  # 判断下面
                board[r][c] = None
                board[r + 1][c] = current_square
                steps += 1
            elif c - 1 >= 0 and board[r][c - 1] is None:  # 判断左面
                board[r][c] = None
                board[r][c - 1] = current_square
                steps += 1



frame = simplegui.create_frame('拼图',WIDTH,HEIGHT)
frame.set*_canvas_*background('Black')
frame.set*_draw_*handler(draw)
frame.add*_button('重新开始',play_*game,60)

frame.set*_mouseclick_*handler(mouseclick)

play_game()

frame.start()
```









