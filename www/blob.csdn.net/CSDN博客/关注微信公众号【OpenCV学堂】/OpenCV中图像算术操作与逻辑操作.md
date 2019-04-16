# OpenCV中图像算术操作与逻辑操作 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2016年12月05日 14:48:03[gloomyfish](https://me.csdn.net/jia20003)阅读数：7094
个人分类：[图像处理](https://blog.csdn.net/jia20003/article/category/879324)









## OpenCV中图像算术操作与逻辑操作

在图像处理中有两类最重要的基础操作分别是图像点操作与块操作，简单点说图像点操作就是图像每个像素点的相关逻辑与几何运算、块操作最常见就是基于卷积算子的各种操作、实现各种不同的功能。今天小编就跟大家一起学习OpenCV中图像点操作相关的函数与应用场景。常见算术运算包括加、减、乘、除，逻辑运算包括与、或、非、异或。准备工作： 

选择两张大小一致的图像如下、加载成功以后显示如下： 
![这里写图片描述](https://img-blog.csdn.net/20161205135337641)
![这里写图片描述](https://img-blog.csdn.net/20161205135357153)
加法操作结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20161205143636365)
减法操作结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20161205143916773)
乘法操作结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20161205143940995)
除法操作结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20161205144037070)
权重加法操作结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20161205144204622)
异或与非操作结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20161205144242008)
代码如下：
```
Mat src1, src2, dst;
    src1 = imread("D:/vcprojects/images/test1.png");
    src2 = imread("D:/vcprojects/images/moon.png");

    const char* input_title1 = "input image - 1";
    const char* input_title2 = "input image - 2";

    namedWindow(input_title1, CV_WINDOW_AUTOSIZE);
    namedWindow(input_title2, CV_WINDOW_AUTOSIZE);

    imshow(input_title1, src1);
    imshow(input_title2, src2);

    // create result windows and background image
    const char* output_title = "result image";
    namedWindow(output_title, CV_WINDOW_AUTOSIZE);
    Mat bgImg = Mat(src1.size(), src1.type());
    Mat whiteImg = Mat(src1.size(), src1.type());
    whiteImg = Scalar(255, 255, 255);

    // 临时图像
    Mat skel(src1.size(), CV_8UC1, Scalar(0));
    Mat temp(src1.size(), CV_8UC1);
    Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3), Point(-1, -1));
    bool done = false;
    int index = 9, c;
    while (true) {
        switch (index) {
        case 1:
            // 加操作
            add(src1, src2, dst, Mat(), -1);
            imshow(output_title, dst);
            break;
        case 2:
            // 减操作
            subtract(src1, src2, dst, Mat(), -1);
            imshow(output_title, dst);
            break;
        case 3:
            // 乘操作
            bgImg = Scalar(2, 2, 2);
            multiply(src1, bgImg, dst, 1.0, -1);
            imshow(output_title, dst);
            break;
        case 4:
            // 除操作
            bgImg = Scalar(2, 2, 2);
            divide(src1, bgImg, dst, 1.0, -1);
            imshow(output_title, dst);
            break;
        case 5:
            // 基于权重加法 - 调节亮度
            addWeighted(src1, 1.5, src2, 0.5, 0, dst, -1);
            imshow(output_title, dst);
            break;
        case 6:
            // 逻辑非
            bitwise_not(src1, dst, Mat());
            imshow(output_title, dst);
            break;
        case 7:
            subtract(whiteImg, src1, dst, Mat(), -1);
            imshow(output_title, dst);
            break;
        case 8:
            // 逻辑异或
            bgImg = Scalar(255, 255, 255);
            bitwise_xor(src1, bgImg, dst, Mat());
            imshow(output_title, dst);
            break;
        default:
            imshow(output_title, src2);
            break;
        }

        c = waitKey(500);
        if ((char)c == 27) {
            break;
        }
        if(c > 0) {
            index = c % 9;
        }
    }
```

此外我们还可以基于逻辑操作与形态学的腐蚀操作实现二值图像的骨架提取，Demo演示结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20161205144445681)

代码实现如下：
```
// 提取骨架
    // 转灰度与二值化
    cvtColor(src1, src1, COLOR_BGR2GRAY);
    threshold(src1, dst, 127, 255, CV_THRESH_BINARY);
    //bitwise_not(src1, src1);

    do {
        // 开操作 - 确保去掉小的干扰块
        morphologyEx(src1, temp, MORPH_OPEN, element);
        // 取反操作
        bitwise_not(temp, temp);
        // 得到与源图像不同
        bitwise_and(src1, temp, temp);
        // 使用它提取骨架、得到是仅仅比源图像小一个像素
        bitwise_or(skel, temp, skel);
        // 每次循环腐蚀，通过不断腐蚀的方式得到框架
        erode(src1, src1, element);

        // 对腐蚀之后的图像寻找最大值，如果被完全腐蚀则说明
        // 只剩下背景黑色、已经得到骨架，退出循环
        double max;
        minMaxLoc(src1, 0, &max);
        done = (0 == max);

    } while (!done);

    // 显示骨架
    imshow(output_title, skel);
```

**总结：**

通过上述代码演示，可以发现简单的图像算术运算也可以发挥大作用，基于黑色背景图像与原图权重叠加可以实现图像亮度调整、基于乘法可以实现对比度调整。基于逻辑操作与腐蚀操作可以实现二值图像的骨架提取。





