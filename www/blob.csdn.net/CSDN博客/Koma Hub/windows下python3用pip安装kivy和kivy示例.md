# windows下python3用pip安装kivy和kivy示例 - Koma Hub - CSDN博客
2018年04月03日 23:00:50[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：2129
[kivy网址](https://kivy.org/#home)：这个网址里有详细的安装过程，我只是一个搬运工。
Kivy - 用于快速开发应用程序的开源Python库，利用创新的用户界面，如多点触控应用程序。
我的python版本：
```
D:\test>python --version
Python 3.6.4
```
安装kivy步骤：
1.确保你安装了最新版本的pip和wheel
`python -m pip install --upgrade pip wheel setuptools`
2.安装依赖
```
python -m pip install docutils pygments pypiwin32 kivy.deps.sdl2 kivy.deps.glew
python -m pip install kivy.deps.gstreamer
```
如果遇到内存错误，就在后面加上`–no-cache-dir.`
如果安装的python版本大于3.5，那么可以用下面的代替glew：
`python -m pip install kivy.deps.angle`
3.安装kivy
`python -m pip install kivy`
4.安装kivy示例：
`python -m pip install kivy_examples`
我安装的示例在：
`C:\Users\Toa\AppData\Local\Programs\Python\Python36\share\kivy-examples>`
所以我运行如下main.py程序;
`python C:\Users\Toa\AppData\Local\Programs\Python\Python36\share\kivy-examples\demo\showcase\main.py`
跳出如下窗口（这是示例）：
![](https://img-blog.csdn.net/20180403225355856)
我在安装过程中，在某步可能会出错，但是我只是简单的重复又执行了一遍，就好了。
下面给出kivy自带的一个小游戏示例，以我上面的安装路径为例，改程序的源码在：
`C:\Users\Toa\AppData\Local\Programs\Python\Python36\share\kivy-examples\tutorials\pong`
分别在同文件夹下建立main.py和pong.kv，二者内容如下：
main.py
```python
from kivy.app import App
from kivy.uix.widget import Widget
from kivy.properties import NumericProperty, ReferenceListProperty,\
    ObjectProperty
from kivy.vector import Vector
from kivy.clock import Clock
class PongPaddle(Widget):
    score = NumericProperty(0)
    def bounce_ball(self, ball):
        if self.collide_widget(ball):
            vx, vy = ball.velocity
            offset = (ball.center_y - self.center_y) / (self.height / 2)
            bounced = Vector(-1 * vx, vy)
            vel = bounced * 1.1
            ball.velocity = vel.x, vel.y + offset
class PongBall(Widget):
    velocity_x = NumericProperty(0)
    velocity_y = NumericProperty(0)
    velocity = ReferenceListProperty(velocity_x, velocity_y)
    def move(self):
        self.pos = Vector(*self.velocity) + self.pos
class PongGame(Widget):
    ball = ObjectProperty(None)
    player1 = ObjectProperty(None)
    player2 = ObjectProperty(None)
    def serve_ball(self, vel=(4, 0)):
        self.ball.center = self.center
        self.ball.velocity = vel
    def update(self, dt):
        self.ball.move()
        # bounce of paddles
        self.player1.bounce_ball(self.ball)
        self.player2.bounce_ball(self.ball)
        # bounce ball off bottom or top
        if (self.ball.y < self.y) or (self.ball.top > self.top):
            self.ball.velocity_y *= -1
        # went of to a side to score point?
        if self.ball.x < self.x:
            self.player2.score += 1
            self.serve_ball(vel=(4, 0))
        if self.ball.x > self.width:
            self.player1.score += 1
            self.serve_ball(vel=(-4, 0))
    def on_touch_move(self, touch):
        if touch.x < self.width / 3:
            self.player1.center_y = touch.y
        if touch.x > self.width - self.width / 3:
            self.player2.center_y = touch.y
class PongApp(App):
    def build(self):
        game = PongGame()
        game.serve_ball()
        Clock.schedule_interval(game.update, 1.0 / 60.0)
        return game
if __name__ == '__main__':
    PongApp().run()
```
pong.kv
```python
#:kivy 1.0.9
<PongBall>:
    size: 50, 50 
    canvas:
        Ellipse:
            pos: self.pos
            size: self.size          
<PongPaddle>:
    size: 25, 200
    canvas:
        Rectangle:
            pos:self.pos
            size:self.size
<PongGame>:
    ball: pong_ball
    player1: player_left
    player2: player_right
    
    canvas:
        Rectangle:
            pos: self.center_x-5, 0
            size: 10, self.height
    
    Label:
        font_size: 70  
        center_x: root.width / 4
        top: root.top - 50
        text: str(root.player1.score)
        
    Label:
        font_size: 70  
        center_x: root.width * 3 / 4
        top: root.top - 50
        text: str(root.player2.score)
    
    PongBall:
        id: pong_ball
        center: self.parent.center
        
    PongPaddle:
        id: player_left
        x: root.x
        center_y: root.center_y
        
    PongPaddle:
        id: player_right
        x: root.width-self.width
        center_y: root.center_y
```
效果：
![](https://img-blog.csdn.net/20180403225818987)
