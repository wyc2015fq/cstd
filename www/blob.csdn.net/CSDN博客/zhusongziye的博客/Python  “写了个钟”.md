# Python | “写了个钟” - zhusongziye的博客 - CSDN博客





2018年08月12日 20:31:41[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：178








概述：
- 
显示时间的钟表

- 
使用pygame模块

- 
使用time、sys、math模块




一、展示

![](https://img-blog.csdn.net/2018081220312522?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



二、代码：

```
import sys, math, pygame
from pygame.locals import *
from datetime import datetime, date, time

def print_text(font, x, y, text, color = (255, 255, 255)):
   imgText = font.render(text, True, color)
   screen.blit(imgText, (x, y))

def wrap_angle(angle):
   return abs(angle % 360)

# main program begins
pygame.init()
screen = pygame.display.set_mode((600, 500))
pygame.display.set_caption("Analog Clock Demo")
font = pygame.font.Font(None, 24)
orange = 200, 180, 0
white = 255, 255, 255
yellow = 255, 255, 0
pink = 255, 100, 100

pos_x = 300
pos_y = 250
radius = 250
angle = 360

# repeating loop
while True :
   for event in pygame.event.get():
       if event.type == QUIT:
           sys.exit()
   keys = pygame.key.get_pressed()
   if keys[K_ESCAPE]:
       screen.fill((0, 0, 100))
       sys.exit()

   # draw the clock numbers 1-12
   for n in range(1, 13):
       angle = math.radians(n * (360 / 12) - 90)
       x = math.cos(angle) * (radius - 20) - 10
       y = math.sin(angle) * (radius - 20) - 10
       print_text(font, pos_x + x, pos_y + y, str(n))

   # get the time of day
   today = datetime.today()
   hours = today.hour
   minutes = today.minute
   seconds = today.second

   # draw the hours hand
   hour_angle = wrap_angle(hours * (360 / 12) - 90)
   hour_angle = math.radians(hour_angle)
   hour_x = math.cos(hour_angle) * (radius - 80)
   hour_y = math.sin(hour_angle) * (radius - 80)
   target = (pos_x + hour_x, pos_y + hour_y)
   pygame.draw.line(screen, pink, (pos_x, pos_y), target, 25)

   # draw the minute hand
   min_angle = wrap_angle(minutes * (360 / 60) - 90)
   min_angle = math.radians(min_angle)
   min_x = math.cos(min_angle) * (radius - 60)
   min_y = math.sin(min_angle) * (radius - 60)
   target = (pos_x + min_x, pos_y + min_y)
   pygame.draw.line(screen, orange, (pos_x, pos_y), target, 12)
   
   # draw the second hand
   sec_angle = wrap_angle(seconds * (360 / 60) - 90)
   sec_angle = math.radians(sec_angle)
   sec_x = math.cos(sec_angle) * (radius - 40)
   sec_y = math.sin(sec_angle) * (radius - 40)
   target = (pos_x + sec_x, pos_y + sec_y)
   pygame.draw.line(screen, yellow, (pos_x, pos_y), target, 6)

   # cover the center
   pygame.draw.circle(screen, white, (pos_x, pos_y), radius, 6)
   pygame.draw.circle(screen, white, (pos_x, pos_y), 20)
   print_text(font, 0, 0, str(hours) + " : " + str(minutes) + " : " + str(seconds))
   
   pygame.display.update()
   screen.fill((0,0,0))
```



三、代码讲解



**关于颜色：可在此处修改颜色信息**

```
orange = 200, 180, 0
white = 255, 255, 255
yellow = 255, 255, 0
pink = 255, 100, 100
```



**获取系统时间**

```
# get the time of day
   today = datetime.today()
   hours = today.hour
   minutes = today.minute
   seconds = today.second
```



**定位指针**

```
# draw the hours hand
  hour_angle = wrap_angle(hours * (360 / 12) - 90)
  hour_angle = math.radians(hour_angle)
  hour_x = math.cos(hour_angle) * (radius - 80)
  hour_y = math.sin(hour_angle) * (radius - 80)
  target = (pos_x + hour_x, pos_y + hour_y)
  pygame.draw.line(screen, pink, (pos_x, pos_y), target, 25)

  # draw the minute hand
  min_angle = wrap_angle(minutes * (360 / 60) - 90)
  min_angle = math.radians(min_angle)
  min_x = math.cos(min_angle) * (radius - 60)
  min_y = math.sin(min_angle) * (radius - 60)
  target = (pos_x + min_x, pos_y + min_y)
  pygame.draw.line(screen, orange, (pos_x, pos_y), target, 12)
  
  # draw the second hand
  sec_angle = wrap_angle(seconds * (360 / 60) - 90)
  sec_angle = math.radians(sec_angle)
  sec_x = math.cos(sec_angle) * (radius - 40)
  sec_y = math.sin(sec_angle) * (radius - 40)
  target = (pos_x + sec_x, pos_y + sec_y)
  pygame.draw.line(screen, yellow, (pos_x, pos_y), target, 6)
```



四、笔者自述



这是一个模拟时钟软件。实现原理是利用time模块获取系统时间，然后利用pygame模块强大的图形处理能力，将时间变成钟表样式。这中间需要用到数学模块。



Pygame是一个为2D小游戏开发的模块，使用这个模块可以帮助开发者，快速，独立地开发具有可玩性的游戏。但是一个其用途并不局限于游戏开发，其优秀的图形图像，音频处理能力，让它可以适用于更多场合。



整体思路，建立一个画布，在上面画好一个时钟的轮轮廓。外边的大圆和圆心。之后画好时钟的指针（时针、分针、秒针），再利用math模块提供的数学函数，计算出指针旋转的角度。一个时钟就这样完成了。



