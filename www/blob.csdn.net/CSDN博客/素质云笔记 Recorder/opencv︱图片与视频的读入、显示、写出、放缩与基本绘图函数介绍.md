# opencv︱图片与视频的读入、显示、写出、放缩与基本绘图函数介绍 - 素质云笔记/Recorder... - CSDN博客





2017年04月09日 15:34:15[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：4454标签：[opencv																[图片																[视频																[显示](https://so.csdn.net/so/search/s.do?q=显示&t=blog)
个人分类：[图像opencv及图像基础技术](https://blog.csdn.net/sinat_26917383/article/category/6847304)





> 
本文来自于段力辉 译《OpenCV-Python 中文教程》





### 文章目录
- [一、图片 + 读入、显示、写出](#___10)
- [1、cv2.imread()](#1cv2imread_18)
- [2、cv2.imshow()](#2cv2imshow_52)
- [3、cv2.imwrite()](#3cv2imwrite_68)
- [4、图片放缩](#4_83)
- [案例一：opencv加载一个灰度图](#opencv_109)
- [案例二：opencv+matplotib绘图](#opencvmatplotib_132)
- [二、视频的读取、显示、保存](#_154)
- [1、摄像头捕获视频](#1_157)
- [2、从文件中播放视频](#2_198)
- [3、保存视频](#3_237)
- [三、opencv中的绘图函数](#opencv_282)
- [1、画线：cv2.line](#1cv2line_292)
- [.
2、画矩形](#
2_307)
- [.
3、画圆](#
3_316)
- [.
4、画椭圆](#
4_325)
- [.
5、画多边形](#
5_335)
- [6、在图片上添加文字](#6_352)
- [延伸一： 获取图片属性](#__376)
- [.
延伸二：图像求差值报错：](#
_391)
- [延伸三：视频保存](#_414)
- [延伸四：图像画框+写中文+python3读写中文](#python3_460)
- [延伸六: 最简单检验矩形框,cv2代码](#_cv2_544)
- [延伸五：更精致的画框（带中文）](#_563)




# 一、图片 + 读入、显示、写出

opencv中读入、显示、写出图片：cv2.imread()， cv2.imshow()， cv2.imwrite()

## 1、cv2.imread()

```
import numpy as np
import cv2

 # Load an color image in grayscale
img = cv2.imread('messi5.jpg',0)
```

第二个参数是要告诉函数应该如何读取这幅图片。

• cv2.IMREAD_COLOR：读入一副彩色图像。图像的透明度会被忽略，

这是默认参数。

• cv2.IMREAD_GRAYSCALE：以灰度模式读入图像

• cv2.IMREAD_UNCHANGED：读入一幅图像，并且包括图像的 alpha 通道
**注意：**

就算图像的路径是错的， OpenCV 也不会提醒你的，但是当你使用命令print img时得到的结果是None。

其中：

在Py3中若路径中出现中文则需要以下的形式读入：

```
cv2.imdecode(np.fromfile(img_name_1, dtype=np.uint8), -1)
```

其中，0-灰度，1-彩色

**注意二：**

有时候，`cv2.imread('messi5.jpg')`读入之后，在ipython中会发生形变，感觉就跟通道换了一样。这种只是形式上的变化，只要用cv2的输出，还是一个色系，譬如下图，左边就是感觉颜色不对，但是通过`cv2.imwrite`导出还是正常的。
![这里写图片描述](https://img-blog.csdn.net/20171117120328483?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
.

## 2、cv2.imshow()

使用函数 cv2.imshow() 显示图像。窗口会自动调整为图像大小。第一个参数是窗口的名字，其次才是我们的图像。你可以创建多个窗口，只要你喜欢，但是必须给他们不同的名字

```
cv2.imshow('image',img)
cv2.waitKey(0)
cv2.destroyAllWindows()
```

其中cv2.waitKey() 是一个键盘绑定函数。

cv2.destroyAllWindows() 可以轻易删除任何我们建立的窗口。如果你想删除特定的窗口可以使用 cv2.destroyWindow()，在括号内输入你想删除的窗口名。

.

## 3、cv2.imwrite()

使用函数 cv2.imwrite() 来保存一个图像。首先需要一个文件名，之后才是你要保存的图像

```
cv2.imwrite('messigray.png',img)
```

如果路径带中文：

```
cv2.imencode('.jpg', edges)[1].tofile('/../../1.jpg')
```

其中之前的’.jpg’是代表一种格式，如果是图片一定得是.png或者.jpg

.

## 4、图片放缩

目标：你可以对图像进行倍数的放大和缩小  也可以直接的输入尺寸大小

变换的方法：
- CV_INTER_NN - 最近邻插值,
- CV_INTER_LINEAR - 双线性插值 (缺省使用)
- CV_INTER_AREA - 使用象素关系重采样。当图像缩小时候，该方法可以避免波纹出现。当图像放大时，类似于 CV_INTER_NN

方法…
- CV_INTER_CUBIC - 立方插值.

如 我要将一个图片变为32*32大小的

```
image=cv2.imread('test.jpg')
  res=cv2.resize(image,(32,32),interpolation=cv2.INTER_CUBIC)
  cv2.imshow('iker',res)
  cv2.imshow('image',image)
  cv2.waitKey(0)
  cv2.destoryAllWindows()
```

.

## 案例一：opencv加载一个灰度图

按下’s’键保存后退出，或者按下 ESC 键退出不保存。

```
import numpy as np
 import cv2

img = cv2.imread('messi5.jpg',0)
cv2.imshow('image',img)
1k = cv2.waitKey(0)

if k == 27: 
	# wait for ESC key to exit
	cv2.destroyAllWindows()
elif k == ord('s'): 
	# wait for 's' key to save and exit
	 cv2.imwrite('messigray.png',img)
	 cv2.destroyAllWindows()
```

.

## 案例二：opencv+matplotib绘图

```
import numpy as np
import cv2
from matplotlib import pyplot as plt

img = cv2.imread('messi5.jpg',0)
plt.imshow(img, cmap = 'gray', interpolation = 'bicubic')
plt.xticks([]), plt.yticks([]) 
	 # to hide tick values on X and Y axis
plt.show()
```

注意：彩色图像使用 OpenCV 加载时是 BGR 模式。但是 Matplotib 是 RGB模式。所以彩色图像如果已经被 OpenCV 读取，那它将不会被 Matplotib 正确显示。

.

# 二、视频的读取、显示、保存

## 1、摄像头捕获视频

为了获取视频，你应该创建一个 VideoCapture 对象。他的参数可以是设备的索引号，或者是一个视频文件。设备索引号就是在指定要使用的摄像头。

一般的笔记本电脑都有内置摄像头。所以参数就是 0。你可以通过设置成 1 或者其他的来选择别的摄像头。之后，你就可以一帧一帧的捕获视频了。但是最后，别忘了停止捕获视频。

```
import numpy as np
import cv2

cap = cv2.VideoCapture(0)

 while(True):
 # Capture frame-by-frame
	 ret, frame = cap.read()

 # Our operations on the frame come here
	 gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

 # Display the resulting frame
	 cv2.imshow('frame',gray)
	 if cv2.waitKey(1) & 0xFF == ord('q'):
 break

 # When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
```

cap.read() 返回一个布尔值（True/False）。如果帧读取的是正确的，就是 True。所以最后你可以通过检查他的返回值来查看视频文件是否已经到

了结尾。

有时 cap 可能不能成功的初始化摄像头设备。这种情况下上面的代码会报错。

你可以使用 cap.isOpened()，来检查是否成功初始化了。如果返回值是True，那就没有问题。否则就要使用函数 cap.open()。

你可以使用函数 cap.get(propId) 来获得视频的一些参数信息。这里propId 可以是 0 到 18 之间的任何整数。每一个数代表视频的一个属性，见

下表其中的一些值可以使用 ap.set(propId,value) 来修改， value 就是你想要设置成的新值。

例如，我可以使用 cap.get(3) 和 cap.get(4) 来查看每一帧的宽和高。

默认情况下得到的值是 640X480。但是我可以使用 ret=cap.set(3,320)和 ret=cap.set(4,240) 来把宽和高改成 320X240。
.

## 2、从文件中播放视频

与从摄像头中捕获一样，你只需要把设备索引号改成视频文件的名字。

在播放每一帧时，使用 cv2.waiKey() 设置适当的持续时间。如果设置的太低视频就会播放的非常快，如果设置的太高就会播放的很慢（你可以使用这种方法控制视频的播放速度）。通常情况下 25 毫秒就可以了。

```
import numpy as np
import cv2

cap = cv2.VideoCapture(0)

# Define the codec and create VideoWriter object
fourcc = cv2.VideoWriter_fourcc(*'XVID')
out = cv2.VideoWriter('output.avi',fourcc, 20.0, (640,480))

while(cap.isOpened()):
	ret, frame = cap.read()
	if ret==True:
		frame = cv2.flip(frame,0)
# write the flipped frame
		out.write(frame)

		cv2.imshow('frame',frame)
			if cv2.waitKey(1) & 0xFF == ord('q'):
				break
		else:
			break

# Release everything if job is finished
cap.release()
out.release()
cv2.destroyAllWindows()
```

注意： 你应该确保你已经装了合适版本的 ffmpeg 或者 gstreamer。 如果你装错了那就比较头疼了。

.

## 3、保存视频

在我们捕获视频，并对每一帧都进行加工之后我们想要保存这个视频。对于图片来时很简单只需要使用 cv2.imwrite()。但对于视频来说就要多做点工

作。

这次我们要创建一个 VideoWriter 的对象。我们应该确定一个输出文件的名字。接下来指定 FourCC 编码（下面会介绍）。播放频率和帧的大小也都需要确定。最后一个是 isColor 标签。如果是 True，每一帧就是彩色图，否则就是灰度图。

FourCC 就是一个 4 字节码，用来确定视频的编码格式。可用的编码列表可以从fourcc.org查到。这是平台依赖的。下面这些编码器对我来说是有用个。

下面的代码是从摄像头中捕获视频，沿水平方向旋转每一帧并保存它。
```
import numpy as np
import cv2

cap = cv2.VideoCapture(0)

# Define the codec and create VideoWriter object
fourcc = cv2.cv.FOURCC(*'XVID')
out = cv2.VideoWriter('output.avi',fourcc, 20.0, (640,480))

while(cap.isOpened()):
	ret, frame = cap.read()
	if ret==True:
		frame = cv2.flip(frame,0)

		# write the flipped frame
		out.write(frame)

		cv2.imshow('frame',frame)
		if cv2.waitKey(1) & 0xFF == ord('q'):
			break
	else:
		break

# Release everything if job is finished
cap.release()
out.release()
cv2.destroyAllWindows()
```

.

# 三、opencv中的绘图函数

你将会学习到这些函数：cv2.line()， v2.circle()， cv2.rectangle()，cv2.ellipse()， cv2.putText()

上面所有的这些绘图函数需要设置下面这些参数：

• img：你想要绘制图形的那幅图像。

• color：形状的颜色。以 RGB 为例，需要传入一个元组，例如： （255,0,0）代表蓝色。对于灰度图只需要传入灰度值。

• thickness：线条的粗细。如果给一个闭合图形设置为 -1，那么这个图形就会被填充。默认值是 1.

• linetype：线条的类型， 8 连接，抗锯齿等。默认情况是 8 连接。 cv2.LINE_AA为抗锯齿，这样看起来会非常平滑。
## 1、画线：cv2.line

要画一条线，你只需要告诉函数这条线的起点和终点。我们下面会画一条从左上方到右下角的蓝色线段。

```
import numpy as np
import cv2

# Create a black image
img=np.zeros((512,512,3), np.uint8)

# Draw a diagonal blue line with thickness of 5 px
cv2.line(img,(0,0),(511,511),(255,0,0),5)
```

## .2、画矩形

要画一个矩形，你需要告诉函数的左上角顶点和右下角顶点的坐标。这次我们会在图像的右上角话一个绿色的矩形。

```
cv2.rectangle(img,(384,0),(510,128),(0,255,0),3)
```

## .3、画圆

要画圆的话，只需要指定圆形的中心点坐标和半径大小。我们在上面的矩形中画一个圆

```
cv2.circle(img,(447,63), 63, (0,0,255), -1)
```

## .4、画椭圆

画椭圆比较复杂，我们要多输入几个参数。一个参数是中心点的位置坐标。

下一个参数是长轴和短轴的长度。椭圆沿逆时针方向旋转的角度。椭圆弧演顺时针方向起始的角度和结束角度，如果是 0 很 360，就是整个椭圆。查看cv2.ellipse() 可以得到更多信息。下面的例子是在图片的中心绘制半个椭圆。

```
cv2.ellipse(img,(256,256),(100,50),0,0,180,255,-1)
```

## .5、画多边形

画多边形，需要指点每个顶点的坐标。用这些点的坐标构建一个大小等于行数 X1X2 的数组，行数就是点的数目。这个数组的数据类型必须为 int32。

这里画一个黄色的具有四个顶点的多边形。

```
pts=np.array([[10,5],[20,30],[70,20],[50,10]], np.int32)
pts=pts.reshape((-1,1,2))
# 这里 reshape 的第一个参数为-1, 表明这一维的长度是根据后面的维度的计算出来的。
```

如果第三个参数是 False，我们得到的多边形是不闭合的（首尾不相连）。

cv2.polylines() 可以被用来画很多条线。 只需要把想要画的线放在一个列表中，将这个列表传给函数就可以了。每条线都会被独立绘制。这会比用cv2.line() 一条一条的绘制要快一些。

.

## 6、在图片上添加文字

要在图片上绘制文字，你需要设置下列参数：

• 你要绘制的文字

• 你要绘制的位置

• 字体类型（通过查看 cv2.putText() 的文档找到支持的字体）

• 字体的大小

• 文字的一般属性如颜色，粗细，线条的类型等。为了更好看一点推荐使用linetype=cv2.LINE_AA。

在图像上绘制白色的 OpenCV。
```
font=cv2.FONT_HERSHEY_SIMPLEX
cv2.putText(img,'OpenCV',(10,500), font, 4,(255,255,255),2)
```

所 有 的 绘 图 函 数 的 返 回 值 都 是 None， 所 以 不 能 使 用 img =cv2.line(img,(0,0),(511,511),(255,0,0),5)。

.

## 延伸一： 获取图片属性

参考：[Python-OpenCV 处理图像（一）：基本操作](https://segmentfault.com/a/1190000003742422)

```
import cv2
img = cv2.imread('img/image.png')
print img.shape
# (640, 640, 3)
print img.size
# 1228800
print img.dtype
# uint8
# 在debug的时候，dtype很重要
```

## .延伸二：图像求差值报错：

来源：[Python问题记录](http://www.cnblogs.com/s5plus1/p/5569107.html)

```
__main__:2: RuntimeWarning: overflow encountered in ubyte_scalars
```

问题分析：ubyte_scalars出现溢出，定位到具体的代码：

```
val = img[y1_, x1_] - img[y2_, x2_]
```

这段代码的目的是为了计算两个像素的差值，img是numpy的ndarray类型，二维数组中的每个数值类型是uint8，因此两个uint8类型的数相减，得到的仍是一个uint8，如果被减数小于减数，val的结果也并不会得到负数，而是1，同时输出这样一个warning。

**解决方法：解决的方法是将uint8转为int，再相减。**

```
val = int(img[y1_, x1_]) - int(img[y2_, x2_])
```

.

# 延伸三：视频保存

需要一个存放每帧图片的文件夹output_dir，需要知道视频保存在哪output_video_file，

list_files，先遍历文件夹中各个图片

```
# 参数输入
output_dir='/home/ubuntu/keras/matt/frcnn/output'
output_path= os.path.join(output_dir, '')                       # 输入图片存放位置
output_video_file = '/home/ubuntu/keras/matt/frcnn/output6.avi' # 输入视频保存位置以及视频名称
save_to_video(output_video_file,25)                                   

# 函数生成器
def get_file_names(search_path):
	for (dirpath, _, filenames) in os.walk(search_path):
		for filename in filenames:
			yield filename#os.path.join(dirpath, filename)
# yield 把值迭代进去，存进入，然后可以for 迭代出来
# [/home/ubuntu/keras/matt/frcnn/output/]

# 保存函数
def save_to_video(output_path,output_video_file,frame_rate):
    list_files = sorted(get_file_names(output_path), key=lambda var:[int(x) if x.isdigit() else x for x in re.findall(r'[^0-9]|[0-9]+', var)])
    # 拿一张图片确认宽高
    img0 = cv2.imread(os.path.join(output_path,'0.jpg'))
	height , width , layers =  img0.shape
	# 视频保存初始化 VideoWriter
    fourcc = cv2.cv.CV_FOURCC(*'mp4v')
    videowriter = cv2.VideoWriter(output_video_file,fourcc, frame_rate, (width,height))
    # 核心，保存的东西
    for f in list_files:
        print("saving..." + f)
        img = cv2.imread(os.path.join(output_path, f))
        videowriter.write(img)
    videowriter.release()
    cv2.destroyAllWindows()
```

其中：fourcc有以下几种情况：

```
fourcc = cv2.cv.CV_FOURCC(*'mp4v')
# fourcc = cv2.VideoWriter_fourcc(*'mp4v')
#fourcc = cv2.cv.CV_FOURCC(*'XVID')
```

## 延伸四：图像画框+写中文+python3读写中文

1.python3，中文路径，读写

python3中，路径中若有中文，比较麻烦，跟一般的读写方式不一样（[参考](http://blog.csdn.net/u010627377/article/details/72388973)）。

```
import cv2
from matplotlib import pyplot as plt

# 读入
img = cv2.imdecode(np.fromfile('..\\1.jpg',dtype = np.uint8),1)

# 写出
cv2.imwrite("..\\1.jpg",img, [int(cv2.IMWRITE_JPEG_QUALITY), 100] )
cv2.imencode('.jpg', edges)[1].tofile('/../../1.jpg')
```

其中之前的’.jpg’是代表一种格式，如果是图片一定得是.png或者.jpg

2.写文字 + 画框

(187,176),(241,205)代表左上角，右下角，或者是（x,y）（x+width,y+height）或者另外一种写法：(left,top)(left+width,top+height),当然还有一些写法：(x,y,x+w,y+h)
![这里写图片描述](https://img-blog.csdn.net/20171103102803727?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里得看清楚，x,y的起始位置。(x/left代表横轴-左起，y/top代表竖轴-上起)
```
# 画框
plt.imshow(cv2.rectangle(img,(187,176),(241,205),(0,255,0),3), cmap = 'gray', interpolation = 'bicubic')

# 写文字
plt.text(187,176, 'words')   # plt的方式
cv2.putText(img,'words',(187,176) ,0, 1,(255,255,255),2)  #cv2的方式
        # 添加文字，1.2表示字体大小，（0,40）是初始的位置，(255,255,255)表示颜色，2表示粗细
```

以上只能写英文，中文的话，会出现？？？？

187,176代表左上角的点

3.写中文

opencv写中文字，可以参考：[python+freetype+opencv 图片中文（汉字）显示 详细图文教程和项目完整源代码](http://m.blog.csdn.net/wyx100/article/details/75579581)

当然，我在自己尝试的时候失败了，因为没找到ft2这个packages，难道py3没有这个吗？
```
# 
import cv2
import freetype as ft2

import freetype
face = freetype.Face("Vera.ttf")


img = cv2.imread('pic/lena.jpg')
line = '你好,我是 lena'

color = (0, 255, 0)  # Green
pos = (3, 3)
text_size = 24

# ft = put_chinese_text('wqy-zenhei.ttc')
ft = ft2.put_chinese_text('msyh.ttf')
image = ft.draw_text(img, pos, line, text_size, color)
```

之后用ImageDraw可以实现用中文，但是需要加载中文的字体结构。.ttc文件（[参考](Python%203%E4%B8%8BMatplotlib%E7%94%BB%E5%9B%BE%E4%B8%AD%E6%96%87%E6%98%BE%E7%A4%BA%E4%B9%B1%E7%A0%81%E7%9A%84%E8%A7%A3%E5%86%B3%E6%96%B9%E6%B3%95)）。

msyh.ttc是中文简体字

```
from matplotlib import pyplot as plt
import matplotlib
from PIL import Image, ImageDraw, ImageFont
# 显示中文 plt
fnt = "...\\msyh.ttc"
fnt = ImageFont.truetype(fnt, 20)

img = Image.open('..\\1.jpg')
draw = ImageDraw.Draw(img)
draw.rectangle((187,176,241,205), outline=(255, 255, 255, 30))
draw.text((187,176),'你好',(0,0,0), font=fnt)
img.save("..\\t1.jpg")
```

![这里写图片描述](https://img-blog.csdn.net/20171103103907434?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.

## 延伸六: 最简单检验矩形框,cv2代码

```
from matplotlib import pyplot as plt
import cv2
img = cv2.imread('images/images_all/005A5pE3gy1flekxbm2bpj30zk0qoqg2.jpg')
img = cv2.rectangle(img,(x_min,y_min),(x_max,y_max),(0,255,255),3)
plt.imshow(img)
# save
from scipy import misc
# numpy直接保存出来
misc.imsave('img.jpg', r_image)
```

## 延伸五：更精致的画框（带中文）

code起初来源于项目：[allanzelener/YAD2K](https://github.com/allanzelener/YAD2K/blob/1ebda3e58503d29d6c5232c934e6b60be4b423b7/train_overfit.py)

主要有两个函数:`get_colors_for_classes(num_classes)`
`draw_boxes(image, boxes, box_classes, class_names, scores=None,fnt ="msyh.ttc" )`。
其中：`get_colors_for_classes(num_classes)`，输入个数，就会返回相应的颜色，RGB值：

```
get_colors_for_classes(2)
>>> [(0, 255, 255), (255, 0, 0)]
```

那么`draw_boxes`函数各个参数的意思为：
- **image**:矩阵(width, height, 3)
- **boxes**:np.array，`(y_min, x_min, y_max, x_max)`，比如：`array([[120, 516, 221, 714], [306, 753, 363, 847], [148, 14, 222, 78]])`
- **box_classes** = [1,2,6]，这里是下面的class_names索引，代表本次标注的内容；
- **class_names**：[‘a’,‘b’,‘c’,‘d’,‘e’,‘f’]，这里的class_names指的是全部的标签名称，不代表本次，代表全部；
- **scores**=None：或者为`np.array(['0.1','0.2','0.3'])`
- **fnt** =“msyh.ttc” ,这个为字体，一般要中文输入的话，需要指定中文的字体。

```
import colorsys
import random

import numpy as np
from PIL import Image, ImageDraw, ImageFont


def get_colors_for_classes(num_classes):
    """Return list of random colors for number of classes given."""
    # Use previously generated colors if num_classes is the same.
    if (hasattr(get_colors_for_classes, "colors") and
            len(get_colors_for_classes.colors) == num_classes):
        return get_colors_for_classes.colors

    hsv_tuples = [(x / num_classes, 1., 1.) for x in range(num_classes)]
    colors = list(map(lambda x: colorsys.hsv_to_rgb(*x), hsv_tuples))
    colors = list(
        map(lambda x: (int(x[0] * 255), int(x[1] * 255), int(x[2] * 255)),
            colors))
    random.seed(10101)  # Fixed seed for consistent colors across runs.
    random.shuffle(colors)  # Shuffle colors to decorrelate adjacent classes.
    random.seed(None)  # Reset seed to default.
    get_colors_for_classes.colors = colors  # Save colors for future calls.
    return colors


def draw_boxes(image, boxes, box_classes, class_names, scores=None,fnt ="msyh.ttc" ):
    """Draw bounding boxes on image.
    Draw bounding boxes with class name and optional box score on image.
    Args:
        image: An `array` of shape (width, height, 3) with values in [0, 1].
        boxes: An `array` of shape (num_boxes, 4) containing box corners as
            (y_min, x_min, y_max, x_max).
        box_classes: A `list` of indicies into `class_names`.
        class_names: A `list` of `string` class names.
        `scores`: A `list` of scores for each box.
    Returns:
        A copy of `image` modified with given bounding boxes.
    """
    #image = Image.fromarray(np.floor(image * 255 + 0.5).astype('uint8'))
	image = Image.fromarray(image)
	
    font = ImageFont.truetype(
        font=fnt,
        size=np.floor(3e-2 * image.size[1] + 0.5).astype('int32'))
    thickness = (image.size[0] + image.size[1]) // 300

    colors = get_colors_for_classes(len(class_names))

    for i, c in list(enumerate(box_classes)):
        box_class = class_names[c]
        box = boxes[i]
        if isinstance(scores, np.ndarray):
            score = scores[i]
            label = '{} {:.2f}'.format(box_class, score)
        else:
            label = '{}'.format(box_class)

        draw = ImageDraw.Draw(image)
        label_size = draw.textsize(label, font)

        top, left, bottom, right = box
        top = max(0, np.floor(top + 0.5).astype('int32'))
        left = max(0, np.floor(left + 0.5).astype('int32'))
        bottom = min(image.size[1], np.floor(bottom + 0.5).astype('int32'))
        right = min(image.size[0], np.floor(right + 0.5).astype('int32'))
        print(label, (left, top), (right, bottom))

        if top - label_size[1] >= 0:
            text_origin = np.array([left, top - label_size[1]])
        else:
            text_origin = np.array([left, top + 1])

        # My kingdom for a good redistributable image drawing library.
        for i in range(thickness):
            draw.rectangle(
                [left + i, top + i, right - i, bottom - i], outline=colors[c])
        draw.rectangle(
            [tuple(text_origin), tuple(text_origin + label_size)],
            fill=colors[c])
        draw.text(text_origin, label, fill=(0, 0, 0), font=font)
        del draw

    return np.array(image)
```](https://so.csdn.net/so/search/s.do?q=视频&t=blog)](https://so.csdn.net/so/search/s.do?q=图片&t=blog)](https://so.csdn.net/so/search/s.do?q=opencv&t=blog)




