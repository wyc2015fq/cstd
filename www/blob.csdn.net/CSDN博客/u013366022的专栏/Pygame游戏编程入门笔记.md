# Pygame游戏编程入门笔记 - u013366022的专栏 - CSDN博客
2016年08月08日 16:21:51[slitaz](https://me.csdn.net/u013366022)阅读数：3345
## 目录
- [1. Python游戏编程入门](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-1)
- [2. 建立开发环境](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-2)
- [3. 简单示例](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-3)
- [4. 绘制不同形状](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-4)
- [5. 事件监听举例](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-5)
- [6. 用pygame打印文本](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-6)
- [7. 键盘事件](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-7)
- [8. 鼠标事件](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-8)
- [9. 轮询键盘](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-9)
- [10. 轮询鼠标](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-10)
- [11. 角度与弧度](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-11)
- [12. 遍历圆周](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-12)
- [13. 矢量图与位图](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-13)
- [14. 获取已有的surface](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-14)
- [15. 加载位图](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-15)
- [16. 绘制位图](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-16)
- [17. 绘制游戏背景图](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-17)
- [18. 缩放图像](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-18)
- [19. 旋转](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-19)
- [20. 用精灵实现动画](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-20)- [20.1. 加载精灵序列图](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-20-1)
- [20.2. 更改帧](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-20-2)
- [20.3. 绘制帧](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-20-3)
- [20.4. 精灵组](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-20-4)
- [20.5. 一个精灵类](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-20-5)
- [20.6. 精灵冲突](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-20-6)
- [20.7. 精灵与精灵组的矩形冲突](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-20-7)
- [20.8. 两个精灵组之间的矩形冲突](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-20-8)
- [21. 中文字体](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-21)
- [22. 全屏模式](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-22)
- [23. 裁剪屏幕](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-23)
- [24. 让pygame完全控制鼠标](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-24)
- [25. 声音](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-25)
- [26. Sound对象](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-26)
- [27. 游戏背景声音](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-27)
- [28. 打包发布到Windows平台](http://lesliezhu.github.io/public/2016/03/10/Pygame%E6%B8%B8%E6%88%8F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8%E7%AC%94%E8%AE%B0.html#sec-28)
## 1 Python游戏编程入门
买的《Python游戏编程入门》书籍到了，准备记录点学习Pygame开发2D游戏的笔记。
## 2 建立开发环境
由于书籍例子使用python3.2+pygame1.9,因此保持一致。其实，如果使用python2.7的话，应该更多软件包可以使用。
我使用的Mac，而pygame编译好的安装包只有windows版本，需要自己源码安装.
建立虚拟python环境:
$ python -m venv venv
$ source venv/bin/active
下载源码文件后，安装:
$ brew install sdl sdl_image sdl_mixer sdl_ttf portmidi
$ pip install hg+http://bitbucket.org/pygame/pygame
经过测试，发现使用python2.7完成可以运行所有的例子，因此使用python2.7，省去很多麻烦.
$ virtualenv envpygame
$ source envpygame/bin/activate
$ python --version
Python 2.7.6
 
$ pip install hg+http://bitbucket.org/pygame/pygame
  
$ pip list
 
pip (1.5.6)
pygame (1.9.2a0)
setuptools (3.6)
wsgiref (0.1.2)
## 3 简单示例
importpygamefrompygame.localsimport*importsys# 初始化Pygame资源pygame.init()screen=pygame.display.set_mode((,))pygame.display.set_caption("Hello Pygame")myfont=pygame.font.Font(None,)white=,,blue=,,textImage=myfont.render("Hello Pygame",True,white)screen.fill(blue)screen.blit(textImage,(,))# 绘制图形pygame.display.update()whileTrue:foreventinpygame.event.get():ifevent.typein(QUIT,KEYDOWN):sys.exit()screen.fill(blue)screen.blit(textImage,(,))pygame.display.update()
## 4 绘制不同形状
使用 `pygame.draw` 来绘制图形，如:
- pygame.draw.circle()
- pygame.draw.rect()
- pygame.draw.line()
- pygame.draw.arc()
画弧线举例:
start_angle=math.radians()end_angle=math.radians()pygame.draw.arc(screen,color,position,start_angle,end_angle,width)
## 5 事件监听举例
whileTrue:foreventinpygame.event.get():ifevent.type==QUIT:sys.exit()elifevent.type==KEYUP:ifevent.key==pygame.K_ESCAPE:sys.exit()elifevent.key==pygame.K_1:piece1=Trueelifevent.key==pygame.K_2:piece2=Trueelifevent.key==pygame.K_3:piece3=Trueelifevent.key==pygame.K_4:piece4=True#.....
在监听事件的时候一般只设置变量，然后再根据变量进行后续操作。
## 6 用pygame打印文本
以图片的形式绘制文本:
myfont=pygame.font.Font("Arial",)image=myfont.render(text,True,(,,))screen.blit(image,(,))
## 7 键盘事件
一般按键事件分为 `KEYDOWN`, `KEYUP` ，如果要在按键持续按住的情况下持续产生事件，需要设置:
pygame.key.set_repeat()# 表示每个10毫秒一个事件
## 8 鼠标事件
Pygame支持的鼠标事件包括: `MOUSEMOTION`, `MOUSEBUTTONUP`, `MOUSEBUTTONDOWN`.
foreventinpygame.event.get():ifevent.type==MOUSEMOTION:mous_x,mouse_y=event.posmove_x,move_y=event.relelifevent.type==MOUSEBUTTONDOWN:mouse_down=event.buttonmouse_down_x,mouse_down_y=event.poselifevent.type==MOUSEBUTTONUP:mouse_up=event.buttonmouse_up_x,mouse_up_y=event.pos
## 9 轮询键盘
通过轮询键盘接口，可以一次性获取按键列表，不需要遍历事件系统:
keys=pygame.keys.get_pressed()ifkeys[K_ESCAPE]:sys.exit()
## 10 轮询鼠标
pos_x,pos_y=pygame.mouse.get_pos()rel_x,rel_y=pygame.mouse.get_rel()button1,button2,button3=pygame.mouse.get_pressed()
## 11 角度与弧度
正弦函数、余弦函数可以以任意的半径大小来模拟一个圆。
一个完整的圆的弧度表示为 `2*PI` ，等于360°
1弧度对应的角度值:
360 / 6.28 = 57.3248
1角度对应的弧度值:
6.28 / 360 = 0.0174
使用这些数字在角度与弧度之间转换，其精度在大多数游戏里面可以接受。
在 `math` 模块封装了转换函数:
- math.degrees(): 转换为角度
- math.radians(): 转换为弧度
>>> import math
>>> math.degrees(0.5)
28.64788975654116
>>> math.radians(30)
0.5235987755982988
>>> math.radians(28.64788975654116)
0.5
## 12 遍历圆周
计算绕着一个圆的圆周的任何点的X坐标，使用余弦函数。
>>> math.cos(math.radians(90))
6.123233995736766e-17
>>> '{:.2f}'.format(math.cos(math.radians(90)))
'0.00'
>>> '{:.2f}'.format(math.cos(math.radians(45)))
'0.71'
计算圆周上任何点Y坐标，使用正弦函数.
>>> '{:.2f}'.format(math.sin(math.radians(45)))
'0.71'
>>> '{:.2f}'.format(math.sin(math.radians(90)))
'1.00'
- 
```
X
 = math.cos(math.radians(angle)) * radius
```
- 
```
Y
 = math.sin(math.radians(angle)) * radius
```
## 13 矢量图与位图
要处理基于线条绘制的矢量图形，也要处理位图图形.
在Pygame中，一个位图叫 `Surface`,比如 `pygame.display.set_mode()` 返回的就是 `Surface对象`.
## 14 获取已有的surface
screen = pygame.display.get_surface()
## 15 加载位图
通过 `pygame.image.load()` 函数加载位图文件类型:
- JPG
- PNG
- GIF
- BMP
- PCX
- TGA
- TIF
- LBM,PBM,PGM,PPM,XPM
space = pygame.image.load("space.png").convert()
space = pygame.image.load("space.png").convert_alpha()
width,height = space.get_size() 
width = space.get_width() 
height = space.get_height()
## 16 绘制位图
Surface对象函数 `blit()` 用于绘制位图:
screen.blit(space, (0,0))
## 17 绘制游戏背景图
bg = pygame.image.load("space.png").convert_alpha()
screen.blit(bg, (0,0))
## 18 缩放图像
`pygame.sprite.Sprite` 善于绘制和操作用于游戏的图像，而 `pygame.transform` 也可以缩放、翻转图像。
ship = pygame.image.load("freelance.png").convert_alpha()
width,height = ship.get_size() 
ship = pygame.transform.scale(ship, (widht//2,height//2))  # 缩放 
ship = pygame.transform.smoothscale(ship, (width//2,height//2) # 平滑缩放
## 19 旋转
提供图像与旋转角度:
scratch_ship = pygame.transform.rotate(ship, rangled)
计算旋转角度需要使用到正切函数，记住上一个位置与当前位置，计算出正切后再加180°:
delta_x = (pos.x - old_pos.x)
delta_y = (pos.y - old_pos.y) 
rangle = math.atan2(delta_y,delta_x) 
rangled = -math.degrees(rangle) % 360
## 20 用精灵实现动画
`pygame.sprite.Sprite` 并不是一个完整的解决方案，只是一个有限的类，包含一副图片(image)和一个位置(rect)属性，需要自己继续并进行扩展。
### 20.1 加载精灵序列图
加载的时候必须告诉精灵类一帧有多大，需要宽度、高度，同时还需要知道精灵序列图有多少列:
defload(self,filename,width,height,columns):self.master_image=pygame.image.load(filename).convert_alpha()self.frame_width=widthself.frame_height=heightself.rect=,,width,heightself.columns=columns#try to auto-calculate total framesrect=self.master_image.get_rect()self.last_frame=(rect.width//width)*(rect.height//height)-
对于加载的图像，有两个函数很重要:
- `image.get_rect()`:
 获取图像的矩形范围
- `image.subsurface(rect)`:
 获取图像的区域
### 20.2 更改帧
帧速率:
framerate = pygame.time.Clock()
framerate.tick(30)
运行速度:
ticks = pygame.time.get_ticks()
pygame.sprite.Sprite.update(ticks)  # 需要自己重新此函数
### 20.3 绘制帧
defupdate(self,current_time,rate=):#update animation frame numberifcurrent_time>self.last_time+rate:self.frame+=ifself.frame>self.last_frame:self.frame=self.first_frameself.last_time=current_time#build current frame only if it changedifself.frame!=self.old_frame:frame_x=(self.frame%self.columns)*self.frame_widthframe_y=(self.frame//self.columns)*self.frame_heightrect=(frame_x,frame_y,self.frame_width,self.frame_height)self.image=self.master_image.subsurface(rect)self.old_frame=self.frame
- 获取精灵序列图的大小
- 获取每一帧，即每一个小图的大小
- 根据帧数，计算出精灵序列图的子图大小
- 绘制子图作为帧图像
### 20.4 精灵组
精灵组是一个简单的实体容器，调用精灵类的update()方法，然后，绘制容器中的所有精灵。
精灵组刷新频率:
group = pygame.sprite.Group()
group.add(sprite)
framerate = pygame.time.Clock()
framerate.tick(30)
ticks = pygame.time.get_ticks()
group.update(ticks)
group.draw(screen)
### 20.5 一个精灵类
classMySprite(pygame.sprite.Sprite):def__init__(self,target):pygame.sprite.Sprite.__init__(self)#extend the base Sprite classself.master_image=Noneself.frame=self.old_frame=-self.frame_width=self.frame_height=self.first_frame=self.last_frame=self.columns=self.last_time=#X propertydef_getx(self):returnself.rect.xdef_setx(self,value):self.rect.x=valueX=property(_getx,_setx)#Y propertydef_gety(self):returnself.rect.ydef_sety(self,value):self.rect.y=valueY=property(_gety,_sety)#position propertydef_getpos(self):returnself.rect.topleftdef_setpos(self,pos):self.rect.topleft=posposition=property(_getpos,_setpos)defload(self,filename,width,height,columns):self.master_image=pygame.image.load(filename).convert_alpha()self.frame_width=widthself.frame_height=heightself.rect=Rect(,,width,height)self.columns=columns#try to auto-calculate total framesrect=self.master_image.get_rect()self.last_frame=(rect.width//width)*(rect.height//height)-defupdate(self,current_time,rate=):#update animation frame numberifcurrent_time>self.last_time+rate:self.frame+=ifself.frame>self.last_frame:self.frame=self.first_frameself.last_time=current_time#build current frame only if it changedifself.frame!=self.old_frame:frame_x=(self.frame%self.columns)*self.frame_widthframe_y=(self.frame//self.columns)*self.frame_heightrect=Rect(frame_x,frame_y,self.frame_width,self.frame_height)self.image=self.master_image.subsurface(rect)self.old_frame=self.framedef__str__(self):returnstr(self.frame)+","+str(self.first_frame)+ \
                ","+str(self.last_frame)+","+str(self.frame_width)+ \
                ","+str(self.frame_height)+","+str(self.columns)+ \
                ","+str(self.rect)
### 20.6 精灵冲突
边界矩形冲突检测是比较两个精灵的矩形看它们是否重叠:
pygame.sprite.collide_rect(sprite1,sprite2)
边界圆形冲突检测是比较两个精灵的半径:
pygame.sprite.collide_circle(first, second)
pygame.sprite.collide_circle_radio(radio)(first,second)
像素精确遮罩冲突检测:
pygame.sprite.collide_mask(first,second)
除非是一个移动缓慢的游戏并且高度精确性又很重要，否则不建议使用这个，耗资源.
### 20.7 精灵与精灵组的矩形冲突
collide_list = pygame.sprite.spritecollide(arrow,flock_of_birds,False)
- 第一个参数是单个的精灵
- 第二个参数是精灵组
- 第三个参数是布尔值，传递True将导致精灵组中的所有冲突的精灵被删除掉
- 精灵与精灵组中挨个进行冲突检测，返回一个冲突精灵的列表
pygame.sprite.spritecollideany(arrow, flock_of_birds)
重要精灵与精灵组中任何一个精灵有冲突，则返回True.
### 20.8 两个精灵组之间的矩形冲突
不要轻易使用，耗费资源:
hit_list = pygame.sprite.groupcollide(bombs, cities, True, False)
- 第一个参数是精灵组
- 第二个参数是另一个精灵组
- 第三个参数，是否删除有冲突的精灵，在第一个精灵组中
- 第四个参数，是否删除有冲突的精灵，在第二个精灵组中
## 21 中文字体
要显示中文，需要以下几点:
- 对程序编码使用 `utf-8` ，添加 
```
#
 coding: utf-8
```
- 将字体文件找到，最好把字体文件复制过来，比如Mac上使用 `fc-list` 找到字体 `/Library/Fonts/Microsoft/Kaiti.ttf` ,楷体
- 创建字体对象:
font1 = pygame.font.Font('resources/fonts/Kaiti.ttf',18)
text = font1.render(u"夏目友人帐", True, (255,0,0))  # 注意使用Unicode
## 22 全屏模式
一般设置大小是:
screen=pygame.display.set_mode((600, 400))
如果要全屏:
screen=pygame.display.set_mode((600, 400),FULLSCREEN,32)
各个标志位说明:
- FULLSCREEN: 创建一个全屏窗口
- DOUBLEBUF: 创建一个“双缓冲”窗口，建议在HWSURFACE或者OPENGL时使用
- HWSURFACE: 创建一个硬件加速的窗口，必须和FULLSCREEN同时使用
- OPENGL: 创建一个OPENGL渲染的窗口
- RESIZABLE: 创建一个可以改变大小的窗口
- NOFRAME: 创建一个没有边框的窗口
当窗口大小改变时候,可以相应修改屏幕大小:
if event.type == VIDEORESIZE:
   SCREEN_SIZE = event.size
   screen = pygame.display.set_mode(SCREEN_SIZE, RESIZABLE, 32)
如果使用 `双缓冲` ，则刷新使用 `pygame.display.flip()`
## 23 裁剪屏幕
screen.set_clip(0, 400, 200, 600)
draw_map()    #在左下角画地图 
screen.set_clip(0, 0, 800, 60) 
draw_panel()  #在上方画菜单面板
## 24 让pygame完全控制鼠标
pygame.mouse.set_visible(False)
pygame.event.set_grab(True)
## 25 声音
通过 `pygame.mixer.init` 来初始化声音.
在Pygame上只支持 `未压缩的WAV` 和 `OGG` 音频文件， `将WAV用于较短的音频文件，而OGG用于较长的音频文件。`
## 26 Sound对象
sound = pygame.mixer.Sound("1.wav")
- fadeout: 淡出声音，可接受一个数字（毫秒）作为淡出时间
- get_length: 获得声音文件长度，以秒计
- get_num_channels: 声音要播放多少次
- get_volume: 获取音量（0.0 ~ 1.0）
- play: 开始播放，返回一个Channel对象，失败则返回None
- set_volume: 设置音量
- stop: 立刻停止播放
对于要播放的Sound:
channel = pygame.mixer.find_channel(True)   # 返回可用最低优先级的频道
channel.play(sound_clip)
## 27 游戏背景声音
pygame.mixer.music.load("resources/audio/吉森信 春を知らせるもの 続 夏目友人帐のテ マ.ogg")
pygame.mixer.music.play(-1, 0.0) 
pygame.mixer.music.set_volume(0.25)
如果要切换背景音乐:
# part 1
pygame.mixer.music.load("xxx") 
pygame.mixer.music.play() 
# part 2 
pygame.mixer.music.fadeout(1000) 
pygame.mixer.music.load("xxx") 
pygame.mixer.music.play()
## 28 打包发布到Windows平台
[http://eyehere.net/2011/python-pygame-novice-professional-py2exe/](http://eyehere.net/2011/python-pygame-novice-professional-py2exe/)
