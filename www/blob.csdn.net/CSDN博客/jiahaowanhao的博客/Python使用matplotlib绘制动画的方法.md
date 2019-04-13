
# Python使用matplotlib绘制动画的方法 - jiahaowanhao的博客 - CSDN博客


2018年01月22日 20:38:33[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：1517


[Python使用matplotlib绘制动画的方法](http://cda.pinggu.org/view/24534.html)
本文实例讲述了Python使用matplotlib绘制动画的方法。分享给大家供大家参考。具体分析如下：
matplotlib从1.1.0版本以后就开始支持绘制动画
下面是几个的示例：
第一个例子使用generator，每隔两秒，就运行函数data_gen：
\# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
fig = plt.figure()
axes1 = fig.add_subplot(111)
line, = axes1.plot(np.random.rand(10))
\#因为update的参数是调用函数data_gen,
\#所以第一个默认参数不能是framenum
def update(data):
line.set_ydata(data)
return line,
\# 每次生成10个随机数据
def data_gen():
while True:
yield np.random.rand(10)
ani = animation.FuncAnimation(fig, update, data_gen, interval=2*1000)
plt.show()
第二个例子使用list（metric），每次从metric中取一行数据作为参数送入update中：
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
start = [1, 0.18, 0.63, 0.29, 0.03, 0.24, 0.86, 0.07, 0.58, 0]
metric =[[0.03, 0.86, 0.65, 0.34, 0.34, 0.02, 0.22, 0.74, 0.66, 0.65],
[0.43, 0.18, 0.63, 0.29, 0.03, 0.24, 0.86, 0.07, 0.58, 0.55],
[0.66, 0.75, 0.01, 0.94, 0.72, 0.77, 0.20, 0.66, 0.81, 0.52]
]
fig = plt.figure()
window = fig.add_subplot(111)
line, = window.plot(start)
\#如果是参数是list,则默认每次取list中的一个元素,
\#即metric[0],metric[1],...
def update(data):
line.set_ydata(data)
return line,
ani = animation.FuncAnimation(fig, update, metric, interval=2*1000)
plt.show()
第三个例子：
import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation
\# First set up the figure, the axis, and the plot element we want to animate
fig = plt.figure()
ax = plt.axes(xlim=(0, 2), ylim=(-2, 2))
line, = ax.plot([], [], lw=2)
\# initialization function: plot the background of each frame
def init():
line.set_data([], [])
return line,
\# animation function. This is called sequentially
\# note: i is framenumber
def animate(i):
x = np.linspace(0, 2, 1000)
y = np.sin(2 * np.pi * (x - 0.01 * i))
line.set_data(x, y)
return line,
\# call the animator. blit=True means only re-draw the parts that have changed.
anim = animation.FuncAnimation(fig, animate, init_func=init,
frames=200, interval=20, blit=True)
\#anim.save('basic_animation.mp4', fps=30, extra_args=['-vcodec', 'libx264'])
plt.show()
第四个例子:
\# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
\# 每次产生一个新的坐标点
def data_gen():
t = data_gen.t
cnt = 0
while cnt < 1000:
cnt+=1
t += 0.05
yield t, np.sin(2*np.pi*t) * np.exp(-t/10.)
data_gen.t = 0
\# 绘图
fig, ax = plt.subplots()
line, = ax.plot([], [], lw=2)
ax.set_ylim(-1.1, 1.1)
ax.set_xlim(0, 5)
ax.grid()
xdata, ydata = [], []
\# 因为run的参数是调用函数data_gen,
\# 所以第一个参数可以不是framenum:设置line的数据,返回line
def run(data):
\# update the data
t,y = data
xdata.append(t)
ydata.append(y)
xmin, xmax = ax.get_xlim()
if t >= xmax:
ax.set_xlim(xmin, 2*xmax)
ax.figure.canvas.draw()
line.set_data(xdata, ydata)
return line,
\# 每隔10秒调用函数run,run的参数为函数data_gen,
\# 表示图形只更新需要绘制的元素
ani = animation.FuncAnimation(fig, run, data_gen, blit=True, interval=10,
repeat=False)
plt.show()
再看下面的例子:
\# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
\#第一个参数必须为framenum
def update_line(num, data, line):
line.set_data(data[...,:num])
return line,
fig1 = plt.figure()
data = np.random.rand(2, 15)
l, = plt.plot([], [], 'r-')
plt.xlim(0, 1)
plt.ylim(0, 1)
plt.xlabel('x')
plt.title('test')
\#framenum从1增加大25后,返回再次从1增加到25,再返回...
line_ani = animation.FuncAnimation(fig1, update_line, 25,fargs=(data, l),interval=50, blit=True)
\#等同于
\#line_ani = animation.FuncAnimation(fig1, update_line, frames=25,fargs=(data, l),
\#  interval=50, blit=True)
\#忽略frames参数,framenum会从1一直增加下去知道无穷
\#由于frame达到25以后,数据不再改变,所以你会发现到达25以后图形不再变化了
\#line_ani = animation.FuncAnimation(fig1, update_line, fargs=(data, l),
\#  interval=50, blit=True)
plt.show()
希望本文所述对大家的python程序设计有所帮助。

