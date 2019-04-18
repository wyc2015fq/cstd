# 17作pygame - weixin_33985507的博客 - CSDN博客
2018年10月23日 10:42:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
```
import pygame
import random
def rand_color():
    """随机颜色"""
    return random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)
pygame.init()
screen = pygame.display.set_mode((600, 400))
screen.fill((255, 255, 255))
# 画图(pygame.draw)
"""
1.画线
def line(Surface, color, start_pos, end_pos, width=1)
Surface: 窗口, 图片, 文字对象
color：线的颜色
start_pos,end_pos: 起点和终点(坐标)
width：宽度
"""
pygame.draw.line(screen, (0, 0, 0), (50, 50), (100, 100), 5)
"""
def lines(Surface, color, closed, pointlist, width=1)
closed: 是否连接起点和终点
pointlist: 列表，列表中的元素是点对应的元祖
"""
points = [(50, 100), (200, 100), (250, 200), (120, 250), (30, 160)]
pygame.draw.lines(screen, (255, 0, 0), True, points, 6)
"""
2.画圆
def circle(Surface, color, pos, radius, width=0)
pos: 圆心位置
radius: 半径
width: 默认0(填充)
"""
pygame.draw.circle(screen, (255, 255, 0), (100, 200), 80, 0)
"""
def arc(Surface, color, Rect, start_angle, stop_angle, width=1)
Rect: (x, y, w, h)
start_angle, stop_angle: 弧度(0->0, 90->pi/2, 45 -> pi/4)
"""
from math import pi
screen.fill((255, 255, 255))  # 将之前画的全部覆盖掉
pygame.draw.arc(screen, rand_color(), (250, 150, 100, 100), pi/4, pi/4*3, 4)
pygame.draw.circle(screen, (255, 255, 0), (300, 200), 80, 0)
pygame.draw.arc(screen,(255,0,255), (250, 150, 100, 100), pi*2/4, pi/4*3, 4)
pygame.draw.arc(screen,(255,0,255), (300, 150, 100, 100), pi*1/4, pi*2/4, 4)
pygame.draw.circle(screen, (100, 100, 0), (300, 200), 20, 0)
pygame.draw.circle(screen, (100, 100, 0), (350, 200), 20, 0)
pygame.draw.circle(screen, (0, 0,0), (300, 200), 5, 0)
pygame.draw.circle(screen, (0, 0,0), (350, 200), 5, 0)
pygame.draw.arc(screen,(0,0,0), (250, 20, 100, 100), pi*5/4, pi*6/4, 4)
pygame.draw.arc(screen,(0,0,0), (300, 150, 100, 100), pi*5/4, pi*6/4, 4)
pygame.display.flip()
while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            exit()
```
