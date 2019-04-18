# matplotlib 绘制动画 - happyhorizon的算法天空 - CSDN博客
2017年09月20日 18:13:58[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：1617
# matplotlib动画
载入matplotlib动画绘制工具:
```
import matplotlib.animation as animation
import matplotlib.pyplot as plt
```
matplotlib有两个动画类: 
 - FuncAnimation: 通过反复调用同一函数来制作动画 
 - ArtistAnimation: 通过调用一个固定的Artist对象来制作动画,例如给定的系列图片或者matplotlib的绘图对象.
这两个类都需要传入实例对象. 动画由时间器timer驱动, 通常是GUI的主框架(host framework)的timer. 
动画的保存: 
Animation.save() 
Animation.to_html5_video()
## FuncAnimation
下面给出FuncAnimation 的调用例子, 附带详细说明.
```python
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
fig = plt.figure() # 获取matplotlib的绘图figure对象
def f(x, y): # 函数定义
    return np.sin(x) + np.cos(y)
x = np.linspace(0, 2 * np.pi, 120) 
y = np.linspace(0, 2 * np.pi, 100).reshape(-1, 1)
im = plt.imshow(f(x, y), animated=True) # 调用imshow实现绘图.这里参数animated=True很重要
def updatefig(*args): # FuncAnimation会将updatefig中的数据传递给绘图句柄,从而更新绘图
    global x, y
    x += np.pi / 15.
    y += np.pi / 20.
    im.set_array(f(x, y))
    return im,  # 这里的,很重要.
ani = animation.FuncAnimation(fig, updatefig, interval=50, blit=True)
plt.show()
```
同样的这段代码,如果用ArtistAnimation()实现的话:
```python
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
fig = plt.figure()
def f(x, y):
    return np.sin(x) + np.cos(y)
x = np.linspace(0, 2 * np.pi, 120)
y = np.linspace(0, 2 * np.pi, 100).reshape(-1, 1)
# ims is a list of lists, each row is a list of artists to draw in the
# current frame; here we are just animating one artist, the image, in
# each frame
ims = [] # 图片序列
for i in range(60):
    x += np.pi / 15.
    y += np.pi / 20.
    im = plt.imshow(f(x, y), animated=True) 
    ims.append([im]) # 每张图片都用单独list的形式加入到图片序列中
ani = animation.ArtistAnimation(fig, ims, interval=50, blit=True,
                                repeat_delay=1000)
# ani.save('dynamic_images.mp4')
plt.show()
```
对比一下两个绘图函数,就可以发现:
```
ani = animation.FuncAnimation(fig, updatefig, interval=50, blit=True)
ani = animation.ArtistAnimation(fig, ims, interval=50, blit=True,
                                repeat_delay=1000)
```
两者的参数基本一样,都是需要传入一个fig对象,一个更新驱动(FuncAnimation是需要一个更新的函数句柄,ArtistAnimation需要的是一个载入图像的函数句柄list.), 一个时间间隔interval,还有blit设置为True.参数repeat_delay只对ArtistAnimation起作用.
下面给出一个用opencv读取已有的图片,再用matplotlib的annimation实现动画效果的代码:
```python
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import cv2  
fig = plt.figure()
ims = []
for i in range(24):
    # 用opencv读取图片
    if i < 10:
        img = cv2.imread('200.0_00'+str(i)+'.png')
    else:
        img = cv2.imread('200.0_0'+str(i)+'.png')
    (r, g, b) = cv2.split(img)  
    img = cv2.merge([b,g,r])  
    im = plt.imshow(img, animated=True)
    ims.append([im])
# 用animation中的ArtistAnimation实现动画. 每帧之间间隔500毫秒, 每隔1000毫秒重复一次,循环播放动画.
ani = animation.ArtistAnimation(fig, ims, interval=500, blit=True, repeat_delay=1000)
```
