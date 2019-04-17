# 使用kivy和pygame显示从摄像头拍的照片 - 墨流觞的博客 - CSDN博客





2018年09月26日 09:21:20[墨氲](https://me.csdn.net/dss_dssssd)阅读数：569








#### 使用kivy和pygame显示从摄像头拍的照片

###### 1.pygame的安装
- windows 下推荐从[https://www.lfd.uci.edu/~gohlke/pythonlib](https://www.lfd.uci.edu/~gohlke/pythonlibs)网站下载pygame和Videocapture
- linux 下安装pygame，不需要手动安装Videocapture
[https://blog.csdn.net/qq_31383385/article/details/79983019](https://blog.csdn.net/qq_31383385/article/details/79983019)

##### 2.测试代码

```python
```python
import pygame.camera
import pygame.image
import sys

pygame.camera.init()

cameras = pygame.camera.list_cameras()

# print("Using camera %s ..." % cameras[0])
SIZE = (320, 240)
webcam = pygame.camera.Camera(cameras[0], SIZE)

webcam.start()

# grab first frame
img = webcam.get_image()

WIDTH = img.get_width()
HEIGHT = img.get_height()

screen = pygame.display.set_mode( ( WIDTH, HEIGHT ) )
pygame.display.set_caption("pyGame Camera View")

while True :
   for e in pygame.event.get() :
       if e.type == pygame.QUIT :
           sys.exit()

   # draw frame
   screen.blit(img, (0,0))
   pygame.display.flip()
   # grab next frame    
   img = webcam.get_image()
```
```

##### 3.kivy和pygame结合

```
# .kv 文件

QrtestHome:
<QrtestHome>:
  qrcam: qrcam
  recog_name: recog_name
  BoxLayout:

      orientation: "vertical"
      canvas.before:
          Color:
              rgba: 0.584, 0.604, 0.608, 1
          Rectangle:
              size: self.size
              pos: self.pos
      Label:
          height: 20
          size_hint_y: None
          text: 'Video player'
      
      Label:
          canvas.before:
              Color:
                  rgba:  [47 / 255., 167 / 255., 212 / 255., 1.]
              Rectangle:
                  size: (self.width, self.height)
                  pos: (self.x, self.y)
          size_hint_y: None
          height: 1
      BoxLayout:
          orientation:"horizontal"

          KivyCamera:
              id: qrcam
          Label:
              size_hint_x: None
              width: dp(2)
              canvas.before:
                  Color:
                      rgba: [47 / 255., 167 / 255., 212 / 255., 1.]
                  Rectangle:
                      size: (self.width, self.height)
                      pos: (self.x, self.y)
          Label:
              canvas.before:
                  Color:
                      rgba: 0.208, 0.196, 0.196, 1
                  Rectangle:
                      size: (self.width, self.height)
                      pos: (self.x, self.y)
              size_hint_x: 0.25
              
              id: recog_name
              text: 'Recognizing'
```
- 在QrtestHome的dostart函数中初始化并启动camera
- 在KivyCamera的start函数中接受启动的camera对象capture，并启动schedule函数定时调用update函数
- 在KivyCamera的update函数中获得frame图像并显示
**注意：** KivyCamera，  update函数中 `texture.blit_buffer(frame)`接受的是byte-like,所以要进行转换，具体的网站讨论[https://stackoverflow.com/questions/19193187/how-to-save-pygame-surface-as-an-image-to-memory-and-not-to-disk](https://stackoverflow.com/questions/19193187/how-to-save-pygame-surface-as-an-image-to-memory-and-not-to-disk)

```python
```python
# Import 'kivy.core.text' must be called in entry point script
# before import of cv2 to initialize Kivy's text provider.
# This fixes crash on app exit.
from kivy.properties import ObjectProperty
import kivy.core.text
import cv2
from kivy.app import App
from kivy.base import EventLoop
from kivy.uix.image import Image
from kivy.clock import Clock
from kivy.graphics.texture import Texture
from kivy.uix.boxlayout import BoxLayout
from kivy.core.window import Window
import os
from kivy.uix.popup import Popup
from kivy.uix.label import Label

# change code
import pygame.camera
import pygame.image

class KivyCamera(Image):

    def __init__(self, **kwargs):
        super(KivyCamera, self).__init__(**kwargs)
        self.capture = None
        self.clock_event = None
        # 在KivyCamera的start函数中接受启动的camera对象capture，并启动schedule函数定时调用update函数
    def start(self, capture, fps=30):
        self.capture = capture
        self.clock_event = Clock.schedule_interval(self.update, 1.0 / fps)

    def stop(self):
        Clock.unschedule(self.clock_event)
        if self.capture != None:
            self.capture.release()
    # 在KivyCamera的update函数中获得frame图像并显示
    def update(self, dt):
        return_value = True 
        frame = self.capture.get_image()
        frame = pygame.image.tostring(frame, 'RGB')
        if return_value:
            texture = self.texture
            # w, h = frame.shape[1], frame.shape[0]
            # if not texture or texture.width != w or texture.height != h:
            self.texture = texture = Texture.create(size=(320, 240))
            texture.flip_vertical()
            # texture.blit_buffer(frame.tobytes(), colorfmt='bgr')
            texture.blit_buffer(frame)
            self.canvas.ask_update()




class QrtestHome(BoxLayout):
    capture = None
    recog_name = ObjectProperty()
    # def change_racob_name(self, name_):
    #     self.recog_name

    def dostart(self, *largs):
        # self.capture = cv2.VideoCapture(0)
        # self.capture.set(3, 320)
        # self.capture.set(4, 240)
        
        # 在QrtestHome的dostart函数中初始化并启动camera
        pygame.camera.init()

        cameras = pygame.camera.list_cameras()

        # print("Using camera %s ..." % cameras[0])
        SIZE = (320, 240)
        webcam = pygame.camera.Camera(cameras[0], SIZE)

        webcam.start()
        self.capture = webcam
        self.ids.qrcam.start(self.capture)

    def doexit(self):
        # global capture
        self.ids.qrcam.stop()
        if self.capture != None:
            # print(self.capture)
            self.capture.release()
            # print(self.capture)
            self.capture = None
            cv2.destroyAllWindows()
            # print(self.capture)
        #EventLoop.close()




class FaceApp(App):
    homeWin = None
    def build(self):
        #Window.clearcolor = (.4,.4,.4,1)
        Window.size = (800, 480)
        self.homeWin = QrtestHome()
        # homeWin.init_()
        return self.homeWin
    
    def on_start(self):
        self.homeWin.dostart()
    # def on_stop(self):
    #     global capture
    #     if capture:
    #         capture.release()
    #         capture = None
if __name__ == '__main__':
    FaceApp().run()
```
```



