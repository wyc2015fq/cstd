# 用霍夫变换&SCNN码一个车道追踪器 - 知乎
# 



选自towardsdatascience，作者：Chuan-en Lin，机器之心编译，参与：李诗萌、张倩。

> 车道标志线向人类驾驶员指示了车道的位置，并作为导向参考来引导车辆的方向，但是[无人驾驶](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650756878%26idx%3D3%26sn%3D11e0d5f33aa17609dc86b1aee8acd3c8%26chksm%3D871a9370b06d1a66ba6c2dbd9ed63a2578f79c947df74f740b4e5a78a0fab9ff095f83711a18%26token%3D1881952741%26lang%3Dzh_CN)汽车是如何「认路」的呢？这要依赖于它们识别和追踪车道的能力。这项能力对于开发无人驾驶车辆的算法来说至关重要。本教程将讲解如何用[计算机视觉](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650756878%26idx%3D3%26sn%3D11e0d5f33aa17609dc86b1aee8acd3c8%26chksm%3D871a9370b06d1a66ba6c2dbd9ed63a2578f79c947df74f740b4e5a78a0fab9ff095f83711a18%26token%3D1881952741%26lang%3Dzh_CN)技术构建车道追踪器。

本教程使用霍夫变换和 SCNN 两种方法来完成这项任务。
![](https://pic1.zhimg.com/v2-983bfeb3d6b95a045094013671cf9618_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='960' height='285'></svg>)
**方法 1：霍夫变换**

大多数车道都设计得相对简单，这不仅是为了鼓励有序，还为了让人类驾驶员更容易以相同的速度驾驶车辆。因此，我们的方法可能会通过[边缘检测](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650756878%26idx%3D3%26sn%3D11e0d5f33aa17609dc86b1aee8acd3c8%26chksm%3D871a9370b06d1a66ba6c2dbd9ed63a2578f79c947df74f740b4e5a78a0fab9ff095f83711a18%26token%3D1881952741%26lang%3Dzh_CN)和[特征提取](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650756878%26idx%3D3%26sn%3D11e0d5f33aa17609dc86b1aee8acd3c8%26chksm%3D871a9370b06d1a66ba6c2dbd9ed63a2578f79c947df74f740b4e5a78a0fab9ff095f83711a18%26token%3D1881952741%26lang%3Dzh_CN)技术先检测出摄像机馈送回来的直线。我们将用 OpenCV（一个开源的用来实现计算机视觉算法的库）。下图是我们的方法流程的概述。
![](https://pic1.zhimg.com/v2-bbe10a9002578b33f2598150e5c6cebc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='960' height='92'></svg>)
在开始之前，这里有一个结果的演示视频：
[车道检测器demo_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/i13561ejy4w.html)
**1. 配置你的环境**

如果你还没有安装 OpenCV，打开终端并运行：

`pip install opencv-python`

运行下列命令行，把教程所用的库克隆下来：

`git clone https://github.com/chuanenlin/lane-detector.git`

接着用文本编辑器打开 detector.py。这个 Python 文件中有这一节所有的代码。

**2. 处理视频**

将我们的样本视频以 10 毫秒为间隔变成一组连续的帧（图像）用于车道检测。可以随时按下「q」键退出程序。


```
import cv2 as cv

# The video feed is read in as a VideoCapture object
cap = cv.VideoCapture("input.mp4")
while (cap.isOpened()):
    # ret = a boolean return value from getting the frame, frame = the current frame being projected in the video
    ret, frame = cap.read()
    # Frames are read by intervals of 10 milliseconds. The programs breaks out of the while loop when the user presses the 'q' key
    if cv.waitKey(10) & 0xFF == ord('q'):
        break

# The following frees up resources and closes all windows
cap.release()
cv.destroyAllWindows()
```


**3. 用 Canny 检测器**

Canny 检测器是一种可以快速进行实时边缘检测的多阶段优化算法。该算法的基本目标是检测亮度的急剧变化（大梯度），比如从白色到黑色的变化，在给出一组阈值的情况下将它们定义为边。Canny 算法主要有 4 个阶段：

A. 降噪

和所有边缘检测算法一样，噪声是导致错误检测的关键问题。用 5*5 的高斯滤波器卷积（平滑）图像，从而降低检测器对噪声的敏感度。这是通过在整个图像上运行数字正态分布的核（本例中是 5*5 的核）来实现的，将每个像素的值设置为相邻像素值的加权平均值。
![](https://pic1.zhimg.com/v2-1ebb8b66be426a0ea1b8167c93dbd58c_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='330' height='155'></svg>)5*5 的高斯核。星号表示卷积运算
B．强度梯度

然后在平滑的图像上用 Sobel、Roberts 或 Prewitt 核（Sobel 用在 OpenCV 中）沿 x 轴和 y 轴检测边缘是水平的、垂直的还是呈对角线的。
![](https://pic1.zhimg.com/v2-47c156fa1bc2c5c5ae50b3417ff3c7e8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='374' height='128'></svg>)用于计算水平方向和垂直方向的一阶导数的 Sobel 核
C．非极大值抑制

非极大值抑制用于「细化」和有效地锐化边缘。检查每个像素的值在先前计算的梯度方向上是否为局部最大值。
![](https://pic4.zhimg.com/v2-f3c8c108968d7469d315dc461a1f125b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='570' height='255'></svg>)三个点上的非极大值抑制
A 在垂直方向的边上。由于梯度方向垂直于边的方向，比较 A 的像素值和 B、C 的像素值来确定 A 是否是局部最大值。如果 A 是局部最大值，则在下一个点上测试非极大值抑制，否则将 A 的像素值设置为 0 并抑制 A。

D. 滞后阈值

在非极大值抑制后，确认强像素在最终的边缘映射中。但还要对弱像素进行进一步分析来确定它是边缘还是噪声。利用预先定义的 minVal 和 maxVal 阈值，我们认为强度梯度高于 maxVal 的是边缘，低于 minVal 的不是边缘并将其删除。强度梯度在 minVal 和 maxVal 之间的像素只有在和梯度高于 maxVal 的像素相连时才是边缘。
![](https://pic4.zhimg.com/v2-92b122ae92b29027f81a99aee57d982f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='562' height='329'></svg>)滞后阈值在两条线上的例子
A 边高于 maxVal，所以是边。B 边在 maxVal 和 minVal 之间但没有和任何高于 maxVal 的边相连，所以删除。C 边在 maxVal 和 minVal 之间，且与 A 边（高于 maxVal）相连，所以是边。

根据上述流程，首先要灰度化我们的帧，因为我们只需用亮度通道来检测边缘，还要用 5*5 的高斯模糊来减少噪声从而避免判断出错误的边。


```
# import cv2 as cv

def do_canny(frame):
    # Converts frame to grayscale because we only need the luminance channel for detecting edges - less computationally expensive
    gray = cv.cvtColor(frame, cv.COLOR_RGB2GRAY)
    # Applies a 5x5 gaussian blur with deviation of 0 to frame - not mandatory since Canny will do this for us
    blur = cv.GaussianBlur(gray, (5, 5), 0)
    # Applies Canny edge detector with minVal of 50 and maxVal of 150
    canny = cv.Canny(blur, 50, 150)
    return canny

# cap = cv.VideoCapture("input.mp4")
# while (cap.isOpened()):
#     ret, frame = cap.read()

    canny = do_canny(frame)

#     if cv.waitKey(10) & 0xFF == ord('q'):
#         break

# cap.release()
# cv.destroyAllWindows()
```


**4. 分割车道区域**

我们要手动制作一个三角形掩码来分割帧中的车道区域，删除帧中的不相关区域以便提高后期的效率。
![](https://pic1.zhimg.com/v2-3e64b9010dcd9ea579193d844d03581c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='914' height='270'></svg>)
三角形掩码通过三个坐标定义，用图中的绿色圆圈表示。


```
# import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt

# def do_canny(frame):
#     gray = cv.cvtColor(frame, cv.COLOR_RGB2GRAY)
#     blur = cv.GaussianBlur(gray, (5, 5), 0)
#     canny = cv.Canny(blur, 50, 150)
#     return canny

def do_segment(frame):
    # Since an image is a multi-directional array containing the relative intensities of each pixel in the image, we can use frame.shape to return a tuple: [number of rows, number of columns, number of channels] of the dimensions of the frame
    # frame.shape[0] give us the number of rows of pixels the frame has. Since height begins from 0 at the top, the y-coordinate of the bottom of the frame is its height
    height = frame.shape[0]
    # Creates a triangular polygon for the mask defined by three (x, y) coordinates
    polygons = np.array([
                            [(0, height), (800, height), (380, 290)]
                        ])
    # Creates an image filled with zero intensities with the same dimensions as the frame
    mask = np.zeros_like(frame)
    # Allows the mask to be filled with values of 1 and the other areas to be filled with values of 0
    cv.fillPoly(mask, polygons, 255)
    # A bitwise and operation between the mask and frame keeps only the triangular area of the frame
    segment = cv.bitwise_and(frame, mask)
    return segment

# cap = cv.VideoCapture("input.mp4")
# while (cap.isOpened()):
#     ret, frame = cap.read()
    # canny = do_canny(frame)

    # First, visualize the frame to figure out the three coordinates defining the triangular mask
    plt.imshow(frame)
    plt.show()
    segment = do_segment(canny)

#     if cv.waitKey(10) & 0xFF == ord('q'):
#         break

# cap.release()
# cv.destroyAllWindows()
```


**5. 霍夫变换**

在笛卡尔坐标系中，我们可以通过绘制 y 对 x 的图像来表示 y=mx+b。但在霍夫空间中，我们也可以通过绘制 b 对 m 的图像将这条线表示为一个点。例如，直线方程 y=2x+1 在霍夫空间中可能是用 (2, 1) 表示的。
![](https://pic4.zhimg.com/v2-10054f6c54990bb58ed2cd4d42b48adf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1015' height='585'></svg>)
现在，我们要在笛卡尔坐标系中绘制一个点而不是一条线。可能会有许多条线经过这个点，每条线的参数 m 和 b 的值都不同。例如，经过 (2,12) 的线可能有 y=2x+8、y=3x+6、y=4x+4、y=5x+2 以及 y=6x 等。这些线在霍夫空间中表示为 (2, 8)、(3, 6)、(4, 4)、(5, 2) 和 (6, 0)。注意，这在霍夫空间中可能会产生一条 b 对 m 的线。
![](https://pic2.zhimg.com/v2-37e0719435ade564966c978221a0edf5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1015' height='592'></svg>)
每当我们在笛卡尔坐标系中看到一系列点，并且知道这些点可以用线连接起来时，我们可以先按上述方法绘制出笛卡尔坐标系中的每一个点在霍夫空间中的线，然后在霍夫空间中找到交点，就可以找到那条线的方程。霍夫空间中的交点表示通过这一系列点的直线的 m 值和 b 值。
![](https://pic1.zhimg.com/v2-0fb9b207c1b8fd6a63d890586150dc24_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1010' height='581'></svg>)
由于我们传入 Canny 检测器的帧可能会被简单地看成表示我们图像空间中边的一系列白点，因此我们可以用相同的技术来识别哪些点应该被连成同一条线，以及这些点连接之后的线的方程是什么，以便我们可以在帧中绘制出这条线。

为了便于解释，我们用笛卡尔坐标来对应霍夫空间。但这种方法存在一个数学上的缺陷：当这条线垂直时，梯度是无穷大的，无法在霍夫空间中表示出来。为了解决这个问题，我们用极坐标代替。过程还是大致相同的，只是我们不在霍夫空间中绘制 b 对 m 的图，我们要绘制的是 r 对 θ 的图。
![](https://pic4.zhimg.com/v2-562ae0d7eaf110aeec855137f02dd8ef_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='647' height='291'></svg>)
例如，对极坐标系中的点 (8, 6)、(4, 9) 和 (12, 3)，我们在霍夫空间中绘制出的相应图像如下：
![](https://pic2.zhimg.com/v2-e3cb01212b2424ede76ad0b9c08eebbd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='651' height='316'></svg>)
我们可以看到，霍夫空间中的线相交于 θ=0.925，r=9.6 处。极坐标系中的线是由 y = xcosθ + ysinθ 定义的，因此我们可以将所有穿过这一点的一条线定义为 9.6 = xcos0.925 + ysin0.925。

一般而言，在霍夫空间中相交的曲线越多，意味着用交点表示的线对应的点越多。在实现中，我们在霍夫空间中定义了交点的最小阈值，以便检测线。因此，霍夫变换基本上跟踪了帧中的每个点的霍夫空间交点。如果交点数量超过了定义的阈值，我们就确定一条对应参数 θ 和 r 的线。

我们用霍夫变换来识别两条直线——车道的左右边界。


```
# import cv2 as cv
# import numpy as np
# # import matplotlib.pyplot as plt

# def do_canny(frame):
#     gray = cv.cvtColor(frame, cv.COLOR_RGB2GRAY)
#     blur = cv.GaussianBlur(gray, (5, 5), 0)
#     canny = cv.Canny(blur, 50, 150)
#     return canny

# def do_segment(frame):
#     height = frame.shape[0]
#     polygons = np.array([
#                             [(0, height), (800, height), (380, 290)]
#                         ])
#     mask = np.zeros_like(frame)
#     cv.fillPoly(mask, polygons, 255)
#     segment = cv.bitwise_and(frame, mask)
#     return segment

# cap = cv.VideoCapture("input.mp4")
# while (cap.isOpened()):
#     ret, frame = cap.read()
#     canny = do_canny(frame)
#     # plt.imshow(frame)
#     # plt.show()
#     segment = do_segment(canny)

    # cv.HoughLinesP(frame, distance resolution of accumulator in pixels (larger = less precision), angle resolution of accumulator in radians (larger = less precision), threshold of minimum number of intersections, empty placeholder array, minimum length of line in pixels, maximum distance in pixels between disconnected lines)
    hough = cv.HoughLinesP(segment, 2, np.pi / 180, 100, np.array([]), minLineLength = 100, maxLineGap = 50)

#     if cv.waitKey(10) & 0xFF == ord('q'):
#         break

# cap.release()
# cv.destroyAllWindows()
```


**6. 可视化**

将车道可视化为两个浅绿色的、线性拟合的多项式，将它们叠加在我们的输入帧上。


```
# import cv2 as cv
# import numpy as np
# # import matplotlib.pyplot as plt

# def do_canny(frame):
#     gray = cv.cvtColor(frame, cv.COLOR_RGB2GRAY)
#     blur = cv.GaussianBlur(gray, (5, 5), 0)
#     canny = cv.Canny(blur, 50, 150)
#     return canny

# def do_segment(frame):
#     height = frame.shape[0]
#     polygons = np.array([
#                             [(0, height), (800, height), (380, 290)]
#                         ])
#     mask = np.zeros_like(frame)
#     cv.fillPoly(mask, polygons, 255)
#     segment = cv.bitwise_and(frame, mask)
#     return segment

def calculate_lines(frame, lines):
    # Empty arrays to store the coordinates of the left and right lines
    left = []
    right = []
    # Loops through every detected line
    for line in lines:
        # Reshapes line from 2D array to 1D array
        x1, y1, x2, y2 = line.reshape(4)
        # Fits a linear polynomial to the x and y coordinates and returns a vector of coefficients which describe the slope and y-intercept
        parameters = np.polyfit((x1, x2), (y1, y2), 1)
        slope = parameters[0]
        y_intercept = parameters[1]
        # If slope is negative, the line is to the left of the lane, and otherwise, the line is to the right of the lane
        if slope < 0:
            left.append((slope, y_intercept))
        else:
            right.append((slope, y_intercept))
    # Averages out all the values for left and right into a single slope and y-intercept value for each line
    left_avg = np.average(left, axis = 0)
    right_avg = np.average(right, axis = 0)
    # Calculates the x1, y1, x2, y2 coordinates for the left and right lines
    left_line = calculate_coordinates(frame, left_avg)
    right_line = calculate_coordinates(frame, right_avg)
    return np.array([left_line, right_line])

def calculate_coordinates(frame, parameters):
    slope, intercept = parameters
    # Sets initial y-coordinate as height from top down (bottom of the frame)
    y1 = frame.shape[0]
    # Sets final y-coordinate as 150 above the bottom of the frame
    y2 = int(y1 - 150)
    # Sets initial x-coordinate as (y1 - b) / m since y1 = mx1 + b
    x1 = int((y1 - intercept) / slope)
    # Sets final x-coordinate as (y2 - b) / m since y2 = mx2 + b
    x2 = int((y2 - intercept) / slope)
    return np.array([x1, y1, x2, y2])

def visualize_lines(frame, lines):
    # Creates an image filled with zero intensities with the same dimensions as the frame
    lines_visualize = np.zeros_like(frame)
    # Checks if any lines are detected
    if lines is not None:
        for x1, y1, x2, y2 in lines:
            # Draws lines between two coordinates with green color and 5 thickness
            cv.line(lines_visualize, (x1, y1), (x2, y2), (0, 255, 0), 5)
    return lines_visualize

# cap = cv.VideoCapture("input.mp4")
# while (cap.isOpened()):
#     ret, frame = cap.read()
#     canny = do_canny(frame)
#     # plt.imshow(frame)
#     # plt.show()
#     segment = do_segment(canny)
#     hough = cv.HoughLinesP(segment, 2, np.pi / 180, 100, np.array([]), minLineLength = 100, maxLineGap = 50)

    # Averages multiple detected lines from hough into one line for left border of lane and one line for right border of lane
    lines = calculate_lines(frame, hough)
    # Visualizes the lines
    lines_visualize = visualize_lines(frame, lines)
    # Overlays lines on frame by taking their weighted sums and adding an arbitrary scalar value of 1 as the gamma argument
    output = cv.addWeighted(frame, 0.9, lines_visualize, 1, 1)
    # Opens a new window and displays the output frame
    cv.imshow("output", output)

#     if cv.waitKey(10) & 0xFF == ord('q'):
#         break

# cap.release()
# cv.destroyAllWindows()
```


现在，打开终端并运行 python detector.py 来测试你的车道检测器。以防你遗失了任何代码，下面是带有注释的完整的解决方案：


```
import cv2 as cv
import numpy as np
# import matplotlib.pyplot as plt

def do_canny(frame):
    # Converts frame to grayscale because we only need the luminance channel for detecting edges - less computationally expensive
    gray = cv.cvtColor(frame, cv.COLOR_RGB2GRAY)
    # Applies a 5x5 gaussian blur with deviation of 0 to frame - not mandatory since Canny will do this for us
    blur = cv.GaussianBlur(gray, (5, 5), 0)
    # Applies Canny edge detector with minVal of 50 and maxVal of 150
    canny = cv.Canny(blur, 50, 150)
    return canny

def do_segment(frame):
    # Since an image is a multi-directional array containing the relative intensities of each pixel in the image, we can use frame.shape to return a tuple: [number of rows, number of columns, number of channels] of the dimensions of the frame
    # frame.shape[0] give us the number of rows of pixels the frame has. Since height begins from 0 at the top, the y-coordinate of the bottom of the frame is its height
    height = frame.shape[0]
    # Creates a triangular polygon for the mask defined by three (x, y) coordinates
    polygons = np.array([
                            [(0, height), (800, height), (380, 290)]
                        ])
    # Creates an image filled with zero intensities with the same dimensions as the frame
    mask = np.zeros_like(frame)
    # Allows the mask to be filled with values of 1 and the other areas to be filled with values of 0
    cv.fillPoly(mask, polygons, 255)
    # A bitwise and operation between the mask and frame keeps only the triangular area of the frame
    segment = cv.bitwise_and(frame, mask)
    return segment

def calculate_lines(frame, lines):
    # Empty arrays to store the coordinates of the left and right lines
    left = []
    right = []
    # Loops through every detected line
    for line in lines:
        # Reshapes line from 2D array to 1D array
        x1, y1, x2, y2 = line.reshape(4)
        # Fits a linear polynomial to the x and y coordinates and returns a vector of coefficients which describe the slope and y-intercept
        parameters = np.polyfit((x1, x2), (y1, y2), 1)
        slope = parameters[0]
        y_intercept = parameters[1]
        # If slope is negative, the line is to the left of the lane, and otherwise, the line is to the right of the lane
        if slope < 0:
            left.append((slope, y_intercept))
        else:
            right.append((slope, y_intercept))
    # Averages out all the values for left and right into a single slope and y-intercept value for each line
    left_avg = np.average(left, axis = 0)
    right_avg = np.average(right, axis = 0)
    # Calculates the x1, y1, x2, y2 coordinates for the left and right lines
    left_line = calculate_coordinates(frame, left_avg)
    right_line = calculate_coordinates(frame, right_avg)
    return np.array([left_line, right_line])

def calculate_coordinates(frame, parameters):
    slope, intercept = parameters
    # Sets initial y-coordinate as height from top down (bottom of the frame)
    y1 = frame.shape[0]
    # Sets final y-coordinate as 150 above the bottom of the frame
    y2 = int(y1 - 150)
    # Sets initial x-coordinate as (y1 - b) / m since y1 = mx1 + b
    x1 = int((y1 - intercept) / slope)
    # Sets final x-coordinate as (y2 - b) / m since y2 = mx2 + b
    x2 = int((y2 - intercept) / slope)
    return np.array([x1, y1, x2, y2])

def visualize_lines(frame, lines):
    # Creates an image filled with zero intensities with the same dimensions as the frame
    lines_visualize = np.zeros_like(frame)
    # Checks if any lines are detected
    if lines is not None:
        for x1, y1, x2, y2 in lines:
            # Draws lines between two coordinates with green color and 5 thickness
            cv.line(lines_visualize, (x1, y1), (x2, y2), (0, 255, 0), 5)
    return lines_visualize

# The video feed is read in as a VideoCapture object
cap = cv.VideoCapture("input.mp4")
while (cap.isOpened()):
    # ret = a boolean return value from getting the frame, frame = the current frame being projected in the video
    ret, frame = cap.read()
    canny = do_canny(frame)
    cv.imshow("canny", canny)
    # plt.imshow(frame)
    # plt.show()
    segment = do_segment(canny)
    hough = cv.HoughLinesP(segment, 2, np.pi / 180, 100, np.array([]), minLineLength = 100, maxLineGap = 50)
    # Averages multiple detected lines from hough into one line for left border of lane and one line for right border of lane
    lines = calculate_lines(frame, hough)
    # Visualizes the lines
    lines_visualize = visualize_lines(frame, lines)
    cv.imshow("hough", lines_visualize)
    # Overlays lines on frame by taking their weighted sums and adding an arbitrary scalar value of 1 as the gamma argument
    output = cv.addWeighted(frame, 0.9, lines_visualize, 1, 1)
    # Opens a new window and displays the output frame
    cv.imshow("output", output)
    # Frames are read by intervals of 10 milliseconds. The programs breaks out of the while loop when the user presses the 'q' key
    if cv.waitKey(10) & 0xFF == ord('q'):
        break
# The following frees up resources and closes all windows
cap.release()
cv.destroyAllWindows()
```


**方法 2：空间 CNN**

方法 1 中这种手动的传统方法对于清晰且笔直的道路来说效果还不错。但很明显当遇到弯道或急转弯时这种方法会失效。此外，我们注意到车道上有由直线组成的标记，比如绘制的箭头标志，可能会不时地扰乱车道检测器，这从演示视频中可以看得出来。解决这个问题的一种方法可能是将三角掩码进一步细化为两个独立的、更精确的掩码。但这些相当随意的掩码参数还是无法适应变化多端的道路环境。另一个缺点是车道检测器可能会忽略只有点状标记或者根本没有清晰标记的车道，因为缺乏满足霍夫变换阈值的连续直线。最后，天气和照明条件也会影响线路可见度。

**1. 架构**

尽管卷积神经网络（CNN）可以有效地识别较低层次图像的简单特征（例如边和颜色梯度）以及更高等级的复杂特征和实体（例如目标识别），但它们很难表示这些特征和实体的「姿势」——也就是说，CNN 可以从原始像素中提取语义，但无法捕获图像中像素的空间关系（例如旋转关系和平移关系）。但车道检测是具有强形状先验但弱外观相干性的任务，因此这些空间关系对车道检测任务来说至关重要。

例如，只通过提取语特征义是很难判断交通标志的，因为它们缺少清晰且连贯的外观线索，而且经常被遮挡。
![](https://pic4.zhimg.com/v2-9b15bc6a2ce1b91784bbed71b859737f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='960' height='229'></svg>)


![](https://pic4.zhimg.com/v2-9b15bc6a2ce1b91784bbed71b859737f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='960' height='229'></svg>)左上角图片中右边的车和左下角图片中右边的摩托车遮挡了右边的车道标记，对 CNN 的结果产生了负面影响
由于我们知道在交通道路上一般会出现类似于物体被垂直放在道路两边这样的空间关系，因此我们了解到加强空间信息的重要性。检测车道的情况也是类似的。

为了解决这个问题，空间 CNN（SCNN）给出了一个架构，这个架构可以「将经典的深度逐层卷积在特征映射下推广到逐片（slice-by-slice）卷积」。这是什么意思？在经典的层到层的 CNN 中，每一个卷积层都从前面的一层接收输入，应用卷积和非线性激活后，将输出传递给后面的层。SCNN 将各个特征映射行和列视为「层」，进一步应用这一步骤，按顺序进行相同的过程（这里的按顺序指的是只有当这一片从前面的一片中接收到信息才会将信息传递给后面一片），这个过程允许像素信息在同一层的不同神经元之间传递，可以有效增强空间信息。
![](https://pic2.zhimg.com/v2-7d72148dd5a0a862e00b4bff70c905b1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='960' height='208'></svg>)
SCNN 是相对较新的、2018 年早些时候才发布的方法，但已经超越了像 ReNet（RNN）、MRFNet（MRF+CNN）这样更深的 ResNet 架构，以 96.53% 的准确率赢得了 TuSimple 基准车道检测挑战赛的冠军。
![](https://pic4.zhimg.com/v2-0509e910c1eb39939c64e35caf82c1ab_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='960' height='218'></svg>)
此外，除了 SCNN，作者还发布了 CULane 数据集，这是一个包括交通车道注释的大型数据集。CULane 数据集还包括许多极具挑战性的场景，包括遮挡情况和光照条件不同的情况。
![](https://pic1.zhimg.com/v2-6bcdb9c65e7c57b04c297fa45afc7700_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='880' height='228'></svg>)
**2. 模型**

车道检测需要精确的像素识别和车道曲线预测。SCNN 的作者并未直接训练车道并进行聚类，而是先将蓝色、绿色、红色和黄色的车道分别标记为四类。模型针对每一条曲线输出了概率映射（probmaps），和语义分割任务相似，然后通过小型网络传递了 probmaps，并预测最终的 cubic spines。该模型基于 DeepLab-LargeFOV 模型的变体。
![](https://pic2.zhimg.com/v2-ba1b881088e47716d1d93860025fb505_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='652' height='529'></svg>)
每条车道用超过 0.5 的存在值标记，以 20 行为间隔搜索相应的 probmap 以找到响应最高的位置。为了确定是否检测到了车道标记，计算真实数据（正确的标签）和预测值间的 IoU，将高于设定阈值的 IoU 评估为真正（TP）样本，用来计算精度和召回率。

**3. 测试和训练**

你可以按照这个库（[https://github.com/XingangPan/SCNN](https://link.zhihu.com/?target=https%3A//github.com/XingangPan/SCNN)）中的代码来重现 SCNN 论文中的结果，也可以用 CULane 数据集来测试你自己的模型。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
原文链接：[https://towardsdatascience.com/tutorial-build-a-lane-detector-679fd8953132](https://link.zhihu.com/?target=https%3A//towardsdatascience.com/tutorial-build-a-lane-detector-679fd8953132)


