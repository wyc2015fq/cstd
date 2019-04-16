# 基于Swing实现的截屏工具 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年06月12日 17:25:17[gloomyfish](https://me.csdn.net/jia20003)阅读数：4036








****

**基于Swing实现的截屏工具**





**主要思路：**


首先绘制截取到的屏幕，然后绘制mask层，根据用户鼠标选择，将选择区域的mask层像素设置


为完全透明，即不透明度为0，当不透明度为255时表示完全不透明。关于如何操作


BufferedImage像素参见这里：[http://blog.csdn.net/jia20003/article/details/7279667](http://blog.csdn.net/jia20003/article/details/7279667)


图示如下：

![](https://img-my.csdn.net/uploads/201206/12/1339492064_4250.png)




**技术点详解：**


1.       使用Javarobot类实现对屏幕的截取


2.       使用MouseListener与MouseMotionListener实现对用户鼠标操作的位置跟踪


3.       使用UI与控制分离的原则完成编码


4.       使用Java2D功能完成对选择区域的标识与像素提取


5.       使用以前的FullScreen功能实现全屏功能-[http://blog.csdn.net/jia20003/article/details/6876995](http://blog.csdn.net/jia20003/article/details/6876995)


6.       使用Swing的ToolKit类实现剪切板操作


7.       使用字体文件完成特殊字体显示功能


8.       通过对像素数据的透明度设置实现mask功能



**关键代码解释：**



**实现Robot截屏的代码如下：**

// Make a rectangle accordingto the size of the screen

Rectangle rectangle = **new** Rectangle(Toolkit.*getDefaultToolkit*().getScreenSize() );

//Take the screen snapshot

Robotrobot = **new** Robot();

**return** robot.createScreenCapture(rectangle);




**实现Swing剪切板功能的代码如下：**

CustomTransferabletransferable = **new** CustomTransferable(bi,1.0f);

Toolkit.*getDefaultToolkit*().getSystemClipboard().setContents(transferable,**null**);



**实现图像保存的代码如下：**

File outputfile =fileChooser.getSelectedFile();

System.*out*.println(outputfile.getAbsolutePath());

ImageIO.*write*(bi, "png", outputfile);



**实现选择区域矩形获取与更新UI的代码如下：**

endX = (**int**)event.getPoint().getX();

endY = (**int**)event.getPoint().getY();

**if**(startX > 0&& startY > 0
 && (endX > startX) && (endY > startY))
 {

**int** width = endX - startX;

**int** height = endY - startY;

mainView.updateView(**new** Rectangle(startX, startY,
 width,height));

}



**创建Mask的代码如下：**

Dimension screenSize = Toolkit.*getDefaultToolkit*().getScreenSize();

**int** height = (**int**)screenSize.getHeight();

**int** width = (**int**)screenSize.getWidth();

maskImage = **new**BufferedImage(width, height, BufferedImage.*TYPE_4BYTE_ABGR*);

**int**[] pixels = **newint**[height *width];

**int** index = 0;

**int** ta = 0, tr = 0,tg = 0, tb = 0;

**for**(**int** row = 0; row< height; row++) {

**for**(**int** col = 0; col< width; col++) {

index = row * width + col;

ta = 255/2;

tr = Color.*GRAY*.getRed();

tg = Color.*GRAY*.getGreen();

tb = Color.*GRAY*.getBlue();

pixels[index] = (ta << 24) | (tr<< 16) | (tg << 8) | tb;

}

}

setRGB(maskImage, 0, 0, width,height, pixels);

**有如下功能：**

**-         支持mask功能**

**-         直接截屏选取，类似QQ截屏选取功能**

**-         支持copy到系统剪切板**

**-         支持保存截屏为png或者jpg格式文件**

**-          支持鼠标文字添加-类似Windows Paint添加文字方式（新功能2012-06-19）**

****
****
使用说明：


-         直接双击jar文件可以直接运行


-         如果你的winrar或者其它压缩工具关联了.jar文件，请右键选择Java图标运行


-         当点[Capture Screen]之后，变成全屏模式，只有按<Esc>键才会退出


-         当按<Esc>键退出时，鼠标选择区域的图像将会自动被显示在窗口中，如图所示：




程序启动界面：
![](https://img-my.csdn.net/uploads/201206/12/1339492326_7693.png)






完成截屏按<Esc>键返回窗口模式界面：
![](https://img-my.csdn.net/uploads/201206/12/1339492356_5647.png)






点击[SaveImage]按钮时候效果：
![](https://img-my.csdn.net/uploads/201206/12/1339492391_5128.png)





**特别提示：运行该工具需要预先安装JRE6或JRE6以上版本运行环境**

**下载：可执行jar文件可以在这里下载使用（已经更新，速度更快）：**

[](http://download.csdn.net/detail/jia20003/4368866)[http://download.csdn.net/detail/jia20003/4370539](http://download.csdn.net/detail/jia20003/4370539)














