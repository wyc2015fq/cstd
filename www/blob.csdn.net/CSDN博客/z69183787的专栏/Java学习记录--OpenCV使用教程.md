# Java学习记录--OpenCV使用教程 - z69183787的专栏 - CSDN博客
2017年08月17日 15:35:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：737
[http://blog.csdn.net/u012706811/article/details/52779271](http://blog.csdn.net/u012706811/article/details/52779271)
环境搭建:
下载openCV后,把build文件夹里面的[Java](http://lib.csdn.net/base/java)拷贝到项目目录
主程序里面启动前加载该库
```
static {
        System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
        //注意程序运行的时候需要在VM option添加该行 指明opencv的dll文件所在路径
        //-Djava.library.path=$PROJECT_DIR$\opencv\x64
    }
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
IDEA内这样配置
![这里写图片描述](https://img-blog.csdn.net/20161010165043825)
例图: 
![这里写图片描述](https://img-blog.csdn.net/20161010162841031)
## 1.Mat与BufferedImage的转换
Mat类是OpenCV最基本的一个数据类型，它可以表示一个多维的多通道的数组。Mat常用来存储图像，包括单通道二维数组——灰度图，多通道二维数组——彩色图。
### 1.1 mat转BufferedImage
```
Mat grayMat = Highgui.imread("E:/temp/3.jpg");
        byte[] data1 = new byte[grayMat.rows() * grayMat.cols() * (int)(grayMat.elemSize())];
        grayMat.get(0, 0, data1);
        BufferedImage image1 = new BufferedImage(grayMat.cols(),
                                grayMat.rows(),BufferedImage.TYPE_BYTE_GRAY);
        image1.getRaster().setDataElements(0, 0, grayMat.cols(), grayMat.rows(), data1);
```
- 1
- 2
- 3
- 4
- 5
- 6
- 1
- 2
- 3
- 4
- 5
- 6
### 1.2 BufferedImage转mat
```
BufferedImage src = ImageIO.read(input);
    Mat srcMat = new Mat(src.getHeight(), src.getWidth(), CvType.CV_8UC3);
```
- 1
- 2
- 1
- 2
## 2.图片灰度化
就是把图片转换为黑白照
`        Imgproc.cvtColor(srcMat, destMat, Imgproc.COLOR_RGB2GRAY);`- 1
- 1
该函数把原srcMat转换为灰度图像放入grayMat中,自己再转换为BufferedImage显示即可.
![这里写图片描述](https://img-blog.csdn.net/20161010162928531)
## 3.二值化处理
也就是只留两个值,黑白
```
Mat binaryMat = new Mat(grayMat.height(),grayMat.width(),CvType.CV_8UC1);
        Imgproc.threshold(grayMat, binaryMat, max1, max2, Imgproc.THRESH_BINARY);
```
- 1
- 2
- 1
- 2
![这里写图片描述](https://img-blog.csdn.net/20161010163033001)
## 4.图像腐蚀
这里使用3*3的图片去腐蚀
```
Mat element = Imgproc.getStructuringElement(Imgproc.MORPH_RECT, new Size(3, 3));
        Imgproc.erode(srcMat,destMat,element);
```
- 1
- 2
- 1
- 2
腐蚀后变得更加宽,粗.便于识别 
![这里写图片描述](https://img-blog.csdn.net/20161010163233145)
## 5.遍历,获取和修改像素值
二值图片,当成一个二维矩阵就可以了,双重循环,使用get方法获取像素点,使用put方法修改像素点
```
for (int y = 0; y < imgSrc.height(); y++)
        {
            for (int x = 0; x < imgSrc.width(); x++)
            {
                //得到该行像素点的值
                    double[] data = imgSrc.get(y,x);
                    for (int i1 = 0; i1 < data.length; i1++) {
                        data[i1] = 255;//像素点都改为白色
                    }
                    imgSrc.put(i,j,data);
            }
        }
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
## 6.过滤和切割
过滤和切割要找到一个开始行或列,结束行或列,这就有一个简单的[算法](http://lib.csdn.net/base/datastructure).该算法从第一行开始遍历,统计每一行的像素点值符合阈值的个数,再根据个数判断该点是否为边界.找到后过滤和切割自然就很简单了,直接创建个该范围的新的Mat对象.
```
for (int y = 0; y < imgSrc.height(); y++)
        {
            int count = 0;
            for (int x = 0; x < imgSrc.width(); x++)
            {
                //得到该行像素点的值
                byte[] data = new byte[1];
                imgSrc.get(y, x, data);
                if (data[0] == 0)
                    count = count + 1;
            }
            if (state == 0)//还未到有效行
            {
                if (count >= 150)//找到了有效行
                {//有效行允许十个像素点的噪声
                    a = y;
                    state = 1;
                }
            }
            else if (state == 1)
            {
                if (count <= 150)//找到了有效行
                {//有效行允许十个像素点的噪声
                    b = y;
                    state = 2;
                }
            }
        }
        System.out.println("过滤下界"+Integer.toString(a));
        System.out.println("过滤上界"+Integer.toString(b));
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
截取
```
//参数,坐标X,坐标Y,截图宽度,截图长度
        Rect roi = new Rect(0, 0, imgSrc.width(), b - a);
        Mat res = new Mat(new Size(roi.width, roi.height),CvType.CV_8UC1);
```
- 1
- 2
- 3
- 1
- 2
- 3
过滤是从上到下,判断该行的黑色像素点是否大于一定值,大于则留下,找到上边界,下边界后立即停止. 
![这里写图片描述](https://img-blog.csdn.net/20161010163320037)
## 7.识别
使用tesseract-ocr进行识别,效果还是很不错的.[github地址](https://github.com/tesseract-ocr/tesseract)
![这里写图片描述](https://img-blog.csdn.net/20161010163542446)
## 8.资料
后来发现一个很不错的网站,分享一下,里面有一些使用案例
`http://www.w3ii.com/java_dip/default.html`
