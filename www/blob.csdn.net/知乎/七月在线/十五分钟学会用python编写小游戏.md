# 十五分钟学会用python编写小游戏 - 知乎
# 



**一、Python及Pygame简介**

作为一款开源的程序设计语言，Python有着众多优势。凭借着越来越丰富、功能越来越强大的扩展库，Python在各个领域越来越突出：数据分析（R语言）、矩阵运算（Matlab）、深度学习……Python的强大正在于它有着如此多的外部扩展库，你可以只关注你感兴趣的功能，而不必在意它的底层是如何实现的。 目前流行的版本是2.7与3.5，后者是作为新版本有某些改变，但Python2用户仍居多。不过值得注意的是Python3对于Unicode的处理更加方便，也因此逐渐被普及。本文的所使用Python版本为3.5。

Pygame是Python的一个package，是一个很经典的游戏制作包，可以完成几乎所有的2D游戏制作。Pygame需要通过pip安装（具体可百度），其中各模块的方法和属性信息可在Pygame官网的帮助手册查询。本文使用的Pygame版本匹配Python3.5-win64。




**二、初次实现**

现在我们尝试着用Pygame来制作一个微信打飞机小游戏。首先我们要做的是制作一个简单的游戏模型，而不是在一开始就让程序过于复杂。因此，我们从最简单的开始，先通过以下几步来实现一只飞机下降的动画。

（1）初始化Pygame主框架

（2）初始化plane类

（3）对按键进行检查并响应

（4）更新画面

（5）重复（3）（4）

首先，导入相关库。


```
import sys,pygame
from pygame.locals import*
from random import randrange
```


然后初始化pygame主框架。我们将会使用到display模块下这些函数：
- set_mode：设定显示的类型和尺寸。默认为窗口，有可选参数FULLSCREEN（全屏）。
- get_surface：返回一个可画图的Surface对象，相当于一帧。一个Surface对象到另一个Surface对象的变化即产生动画的变化。其blit()方法可更新Surface对象。
- flip：更新显示。
- update：更新屏幕一部分，可以提高游戏性能。可以配合后文的RenderUpdates类draw方法中返回的矩形列表使用。
- set_caption：设定Pygame程序标题，窗口化时会用做窗口标题。

Pygame中两个最重要的类是Sprite类和Group类（Sprite类将在后文飞机初始化部分介绍）。Group类（和它的子类）是Sprite类的容器，RenderUpdates类是Group类的子类。创建了一个RenderUpdates类对象之后，可以使用add()方法向其中添加Sprite类对象，也就产生了一只飞机。用该容器存储Sprite对象的好处是容易管理，并且可以只对需要进行更新的显示部分进行更新，提高游戏性能。


```
#初始化
pygame.init()
screen_size=400,600   #screen屏幕大小
pygame.display.set_mode(screen_size)   #窗口
#全屏为.set_mode(screen_size,FULLSCREEN)
pygame.mouse.set_visible(0)   #隐藏鼠标

plane_image=pygame.image.load('plane2.png')   
#导入飞机图片(图片大小应做调整)

sprites=pygame.sprite.RenderUpdates()  
#创建Sprite的容器
sprites.add(plane())   
#向容器中添加一个plane对象

screen=pygame.display.get_surface()   
#可用于画图的Surface对象
bg=(255,255,255)   #背景颜色RGB格式red-blue-green
screen.fill(bg)   #屏幕填充背景颜色
pygame.display.flip()   #显示更新后的屏幕
```


构造函数__init__（注意是两个下划线_+_）中image属性可为其赋值.png/.jpg图片以改变其外观，在这里我们先用plane_image代替，在前面的主函数编写中已为其指定具体的图片（本文图片均为自制，随代码给出。大家也可以直接在网上找图片素材或者PS自制，甚至可以直接用Python图像处理处理）。rect属性记录位置信息，rect.top/bottom/left/right分别记录坐标信息，rect.height/width分别记录长宽。Pygame中的Sprite类是所有可视游戏对象的基类，我们需要从它继承创建子类plane，用来表示下降的飞机，并覆盖它的构造函数和image，rect属性（相当于其数据成员）。 
![](https://pic3.zhimg.com/v2-1bb8894b423c3a9e1c2379e1809a0b96_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='471' height='250'></svg>)- reset()函数随机确定飞机产生的横坐标，并固定纵坐标在画面最上方。
- update()函数改变飞机的位置。当飞机抵达画面下方时，调用reset()函数随机重置其于顶端。


```
class plane(pygame.sprite.Sprite):
   def __init__(self):
       pygame.sprite.Sprite.__init__(self)       
       self.image=plane_image
       self.rect=self.image.get_rect()     
       self.rect.top=-self.rect.height
       self.rect.centerx=
randrange(screen_size[0]-self.rect.width)
+self.rect.width/2 
#控制飞机在窗口内

   def update(self):        
       self.rect.top+=1
       if self.rect.top>screen_size[1]:
           self.kill()   
#飞机飞出屏幕则将其移除
```


      接下来，只要在主程序中添加响应部分、更新部分和循环部分即可。      


      其中pygame.event.get()获取用户操作事件列表，比如键盘输入、鼠标点击等。可以通过判断按键来做出响应。现在我们只是简单地响应“退出”事件，在之后程序完善时完全可以使其响应“暂停”、“重新开始”等。


```
def clear_callback(surf,rect):   
#清除旧Sprite图形
   surf.fill(bg,rect)

while True:
   for eventinpygame.event.get():
       if event.type==QUIT:   #关闭窗口
           sys.exit()
       if event.type==KEYDOWNand
event.key==K_ESCAPE:
#Esc退出
           sys.exit()
   sprites.clear(screen,clear_callback)
   sprites.update()   def clear_callback(surf,rect):   
#清除旧Sprite图形
   surf.fill(bg,rect)

while True:
   for eventinpygame.event.get():
       if event.type==QUIT:   #关闭窗口
           sys.exit()
       if event.type==KEYDOWNand
event.key==K_ESCAPE:
#Esc退出
           sys.exit()
   sprites.clear(screen,clear_callback)
   sprites.update()   
#调用update()方法更新Sprites对象
   updates=sprites.draw(screen)   
#调用父类RenderUpdates的draw方法
#返回需要更新的部分
   pygame.display.update(updates)   
#更新显示
```


至此，我们将上述代码保存在一个.py文件中便可以运行了。结果如下： 
![](https://pic3.zhimg.com/v2-5209ff56d37942aa160bc0d549b8262e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='1001'></svg>)



但是，实现后会发现飞机的速度实在是太快了，我们可以考虑在其构造时为其加上帧率的限制。

      首先，在第一步初始化主框架的时候定义pygame中的Clock对象。


```
#定义一个时间对象
clock=pygame.time.Clock()
#最大帧率
speed=100
```


      然后在最后主程序循环中，更新之前加入语句：

`clock.tick(speed)`

**三、进一步完善**

现在我们已经有了一个基本的框架，但是这仅仅是一只慢慢飞过的飞机。不过不要灰心，我们只要再完成3小步，就可以制作出基本的打飞机小游戏啦。 

（1）初始化我方战机

（2）实现敌方战机的产生

（3）实现敌我战机的相互作用

首先，仿造之前敌方战机的构造方式，编写如下类：


```
class myplane(pygame.sprite.Sprite):
   def __init__(self):
       pygame.sprite.Sprite.__init__(self)      
       self.image=myplane_image
       self.rect=self.image.get_rect()
       self.rect.bottom=screen_size[1]
       self.rect.centerx=screen_size[0]/2

   def update(self,pressed_keys):
       if pressed_keys[K_UP]:
           self.rect.move_ip(0,-3)
       if pressed_keys[K_DOWN]:
           self.rect.move_ip(0,3)
       if pressed_keys[K_LEFT]:
          self.rect.move_ip(-3,0)
       if pressed_keys[K_RIGHT]:
          self.rect.move_ip(3,0)

       #限定player在屏幕中       
       if self.rect.bottom>screen_size[1]:
           self.rect.bottom=screen_size[1]
       elif self.rect.top<=0:
           self.rect.top=0
       if self.rect.left<=0:
           self.rect.left=0  
       elif self.rect.right>screen_size[0]:
           self.rect.right=screen_size[0]
```


      其中与plane类不同的地方有两处，一是其更新方式不同，它需要传入一个pressed_keys参数，代表键盘的按键，分别对上下左右四个键做出响应。二是要限制myplane对象运动不超出屏幕。


      接着，我们不妨把子弹bullets类也编写了再把他们一起实例化。因为子弹的位置需要由myplane确定，因此我们可以在myplane类中定义特定的fire()函数，实现子弹的生成。


```
def fire(self):
       axis=[self.rect.centerx,self.rect.top]
       new_bullets=bullets(axis)
       mybullets.add(new_bullets)       
       all_sprites.add(new_bullets)
```


以下是bullets类，其构造函数需坐标参数axis。


```
class bullets(pygame.sprite.Sprite): 
   def __init__(self,axis):
       pygame.sprite.Sprite.__init__(self)      
       self.image=bullets_image
       self.rect=self.image.get_rect()
       self.rect.centerx=axis[0]
       self.rect.centery=axis[1]

   def update(self):       
       self.rect.top-=3
       if self.rect.bottom<=0:
           self.kill()
```


      现在这三种类的实例化应改为：


```
sprites=pygame.sprite.RenderUpdates()
sprites.add(plane())
mysprites=pygame.sprite.RenderUpdates()
myplane=myplane()
mysprites.add(myplane)
mybullets=pygame.sprite.RenderUpdates()
all_sprites=pygame.sprite.RenderUpdates()
all_sprites.add(mysprites)
all_sprites.add(sprites)
all_sprites.add(mybullets)
```


all_sprites表示所有的类实例（或对象），这是为了更新的简便起见：

注意：其中plane()为plane类的对象，但我们无法直接对其进行操作；而myplane=myplane()这里简便起见对象与类取相同的名字，之后才能在主程序中直接调用其方法（函数）。


```
while True:
while True:
   for eventinpygame.event.get():
       if event.type==QUIT:
           sys.exit()
       elif event.type==KEYDOWNandevent.key==K_ESCAPE:
           sys.exit()
   myplane.fire()
   pressed_keys=pygame.key.get_pressed()
   clock.tick(speed) 
   all_sprites.clear(screen,clear_callback)
   sprites.update()
   mybullets.update()
   mysprites.update(pressed_keys)
   updates=all_sprites.draw(screen)
   pygame.display.update(updates)
```


      现在，我们已经有了可以运动的我方战机，虽然子弹速度太快以致于我们的子弹像是激光抢，我们接下来会解决这个问题，并且尝试着产生更多的敌方战机。 

      这两个问题都涉及到对象产生的速率，我们不妨先看看怎么产生更多敌方战机。首先应该定义产生敌机事件：


```
ADDENEMY=pygame.USEREVENT+1   #为添加敌人创建自定义事件
pygame.time.set_timer(ADDENEMY,1200)   #产生事件的时间(毫秒)
```


      通过这种方式，我们使产生敌机的速率降低，同样的我们也可以这样来限制子弹产生的速度（具体见源代码）。

      然后再在循环过程中判断事件event是否为ADDENEMY来决定是否产生敌机。

现在我们的游戏变成了这样：
![](https://pic1.zhimg.com/v2-a65df9dd934409455bdd0c037c0d8800_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='1008'></svg>)



      终于到了最后一步！我们只要为各种类之间加上相应的关系就完成啦。

      对于敌机，只要触碰到子弹便被kill，对于我机，只要触碰到敌机便kill而且gameover了。在主循环中可加入如下判断语句：


```
for j in mybullets.sprites():
           for i in antisprites.sprites():
               if j.rect.colliderect(i.rect):
                    i.kill()
                    j.kill()
                    break
```


      接下来判断我机与敌机的位置：

      其中mybullets.sprites(),antisprites.sprites()分别返回现有的子弹，敌机对象的列表。它们是按产生时间先后顺序排列的。rect的colliderect()方法可以判断两个矩形是否相交并返回布尔值。通过遍历子弹和敌机对象，我们可以判断子弹和敌机的位置关系，但是考虑到一颗子弹应该只能消灭一只敌机，因此每当一颗子弹消灭一只敌机时，我们应跳出内层循环，继续遍历剩下的子弹对象。


```
if pygame.sprite.spritecollideany(myplane,antisprites):
       myplane.kill()

        #游戏结束提示
       screen.fill((255,255,255))   #填充颜色
       font=pygame.font.Font(None,48)   #设置字体
       text="YOU  LOST!"
       height=font.get_linesize()  #获取文字高度

       #设置字体显示参数
       antialias=1#光滑字体
       black=0,0,0#字体颜色
       text2=font.render(text,antialias,black)
       r=text2.get_rect()  #获取文字图形的矩形
       center,top=screen.get_rect().center   #设置文字位置
       r.midtop=center,top
       screen.blit(text2,r)    #更新Surfacee对象
       pygame.display.flip()   #更新显示
       break
```


其中pygame.sprite.spritecollideany()方法可以用来判断两个Group里sprites对象间的交叉部分。    




**四、最后说几句**

至此，我们已经制作出了简单的打飞机小游戏。其实还有很多可以丰富改善的地方，比如：        
- 最后判断不同对象是否接触的时候仅仅用对象的矩形判断可能不够准确，为此我们可以考虑将对象的真实部分切分为几个小矩形再进行判断；
- 我们的程序仅仅包含在一个.py文件中，我们可以考虑对其进行封装，把游戏对象、游戏信息（得分、等级）、游戏状态（暂停、结束）等分别抽象成一个或几个类，使程序更容易拓展；
- 我们可以使用py2exe包将.py文件制作为.exe可执行文件。

反观全文，我们的整体思路应该是：

类的功能——类的实现——类间的关系。

我们从需要实现的功能出发，定义相应的类及其方法、属性；再到复杂的情形需要不同类的相互联系作用，因此再回头丰富某些类的方法（或者派生继承），使它们能产生联系；完成上述两步之后，又重新回到第一步，考虑类仍可完善的功能，循环反复……


