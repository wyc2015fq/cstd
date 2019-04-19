# ImageMagick 使用经验 - =朝晖= - 博客园
# [ImageMagick 使用经验](https://www.cnblogs.com/dhcn/p/7105523.html)
from:[http://community.itbbs.cn/thread/20402/](http://community.itbbs.cn/thread/20402/)
1.如何用ImageMagic水平或垂直拼接图片 因为是分片下载的，现在只能用montage拼接图片列阵，但如何水平拼接一组等高的图片 或如何垂直拼接一组等宽的图片呢?
拼图
montage *.jpg -tile 22x2 -geometry 64x256+0+0 10-.jpg
将目录里的jpg文件按顺序拼成x轴22块，y轴2块的图 ，每个图块的大小为64X512像素，输出文件为10-.jpg
你可以把22*2改成你想要得值，例如你形成的图x轴有50个快，y轴有30个块，你可以写成50x30
把你所有要合成的文件拷在一个目录里，用一个命令就可以合成
2合并图片
用法：
convert +append 1.jpg 2.jpg 3.jpg .... 0.jpg
把 1.jpg、2.jpg、3.jpg等多张图片沿“水平方向”（ +append）拼成 0.jpg（最后一个文件名是拼出的成品）
convert -append 1.jpg 2.jpg 3.jpg .... 0.jpg
把 1.jpg、2.jpg、3.jpg等多张图片沿“垂直方向”（ -append）拼成 0.jpg（最后一个文件名是拼出的成品）
可以分两步，第一步把小图拼成多个水平方向的长条，第二步把长条按垂直方向合并成一个大图。
3.切割图片
切割图片
用法：
convert -crop 128x128 1.jpg 0.jpg
把大图1.jpg按128x128分割成了多张小图0-1.jpg 0-2.jpg ...
convert -crop 128x64+6+7 1.jpg 0.jpg
从1.jpg 以座标 6,7 为起点切一片 128x64 的块生成 0.jpg
3.锐化
ImageMagick 方法：
convert -sharpen 5 0.jpg 1.jpg
将0.jpg 锐化后生成1.jpg，锐化指数5，此数值越大，锐化度越高，图像细节损失越大
4. 如何让animate在显示图片的过程保持窗口的标题不变？
animate -title "My Image Sequence" images.
5. import抓得的图片与X服务器上显示的不一致怎么办？
import -descend image.miff
6. 如何动画显示YUV 图片序列?
animate -geometry 352x240 -scene 0-71 yuv3:frame%d
7. 如果只看一个RGB图片的红色通道(red channel)?
display -gamma 1.0,0.0,0.0 image.miff
8. 如何改变PostScript默认纸张大小?
要改变默认的612x792大小，修改magick/image.h中的PSPageGeometry的值并重编译.
10. 如何自左至右无边无框无文字半三幅画连接在一起？
convert +append image1.ppm image2.ppm image3.ppm side_by_side.miff
或
convert -size 350x500 xc:black composite.miff
composite -geometry +0+0 composite.miff image1.gif composite.miff
composite -geometry +100+0 composite.miff image2.gif composite.miff
composite -geometry +0+300 composite.miff image3.gif composite.miff
composite -geometry +0+375 composite.miff image4.gif composite.miff
11. 如何生成gif动画?
convert -delay 20 frame*.gif animation.gif
convert -delay 20 frame1.gif -delay 10 frame2.gif -delay 5 frame3.gif animation.gif
convert frame1.gif -page +50+100 frame2.gif -page +0+100 frame3.gif animation.gif
convert -loop 50 frame*.gif animation.gif
convert +adjoin images.* frames%d.gif
12. 如果完整的显示PostScript图片？
display -page letter image.ps
13. 什么是图片目录？如何使用它们?
图片目录（visual image directory ，VID）的创建：
montage *.jpg directory.vid
convert 'vid:*.jpg' directory.vid
显示图片目录
display directory.vid
display vid:movie.mpg
15. 如果带框架保存一个窗口?
xwininfo -frame
import -frame -window ID window.miff
16. 图片以单一颜色（a solid color）显示，是怎么回事？
图片被破坏，或含有值为0的matte层，试着:
display +matte image.miff
17. 出现 "???? library is not available ..."是怎么回事?
没安装相关的函数库。参见安装说明。
18. 如何查看图片不光滑层(matte channel)的值？
convert image.tiff image.matte
display -size 640x480 gray:image.matte
19. 运行"Convert in.gif out.png" 时显示 "Invalid Parameter - out.png"
你运行的convert是windows的分区类型转换程序。
20. 如何在图片增加斜角?
对图片边缘的处理共分四类。
1）增加有色边
convert -bordercolor red -border 25x25 image.jpg image.gif
2）加亮或变暗图片边缘，以增强3D效果
convert -raise 25 image.jpg image.gif
3）在图片周围增加装饰性框架。
convert -mattecolor gray -frame 25x25 image.jpg image.gif
4）在图片边缘增加升、降斜角
convert -mattecolor gray -frame 25x25+0+25 image.jpg image.gif
convert -mattecolor gray -frame 25x25+25+0 image.jpg image.gif
21. 用窗口管理器运行display失败了，怎么办?
用下列命令之一来确认display运行在一个交互环境中：
display logo:Untitled
display < /dev/console
22. 将PostScript文件转换为其它格式时，如何改善文字的外观？
convert -density 288 -geometry 25% image.ps image.gif
23. 如何用2－3英寸高的文字注释一个图片?
先检查有无可缩放字体：
xlsfonts -fn '*-0-0-0-0-*'
有则：
convert -font '-*-helvetica-*-*-*--300-300-*-*-*-*-iso8859-1' -fill green -draw 'text 50,300 Magick' image.gif annotated.gif
If you have the FreeType support built into ImageMagick, just increase your pointsize and/or density:
convert -font Helvetica -pointsize 100 -density 300 ...
24. 如何将gif文件分析为一系列的单个图片？
convert animation.gif frame%02d.gif
25. 用NetScape显示的图片，如何去掉周围的背景色？
对convert用+page参数：
convert +page alpha.gif beta.gif
26. 如何用web安全的颜色（Web safe colors）创建GIF或PNG图片？
对convert用 -map 参数：
convert -map netscape: alpha.gif beta.gif
27. 为何Adobe Acrobat 2.1不能读ImageMagick's制作的PDF?
用 Acrobat 3.0或以上版本，来阅读这种经zip压缩的pdf，或制作PDF时不进行压缩。
convert +compress images.tiff image.pdf
28. 如何在图片上增加一个不光滑层(matte channel)?
convert image.gif -matte temp.miff
composite -compose CopyOpacity mask.xbm temp.miff transparent.gif
29. 在NT环境下如何用convert在图片上写字？
convert -font Arial -fill blue -draw "text 10,10 'your text here'" d:test.tif png:d:test.png
convert.exe -pointsize 18 -draw "text 0,0 "This is my text!"" C:blank.gif c:text.gif
30. 为何GIF文件比期望的大?
需要在编译源码时加 --enable-lzw 参数，来打开LZW压缩功能。
31. 为何JPEG文件比期望的大?
可能嵌入Exif或IPTC等"profiles", 或者含有没压缩的thumbnails. 可以用命令行参数 +profile "*"或相关的API来将它们去掉。
32. 如何从图片文件中抽取第一张图片？
convert "Image.gif[0]" first.gif
33. 如何创建一张空白图片？
convert -size 800x600 xc:"#ddddff" ltblue.ppm
convert -size 800x600 null:white white.ppm
convert in.png -threshold 100% black.ppm <--与in.png同大小< p="">
34 在一副图片中裁切其中一部分
convert -crop 20x20+10+10 1.jpg 2.jpg
宽x(英文字母小写x)高+x值+y值
切出图片大小范围 起始xy坐标
35 把图片缩小到固定大小
convert -resize 20x20 1.jpg 2.jpg
宽x(英文字母小写x)高
变化出的图片大小范围 
36 图片增加光亮度,饱和度,色彩
convert -modulate 150,150,150 1.jpg 2.jpg
brightness,saturation,hue
37 增加饱和度
变深
convert -contrast 1.jpg 2.jpg 加深1倍
convert -contrast -contrast 1.jpg 2.jpg 加深2倍
变浅
convert +contrast 1.jpg 2.jpg 变浅1倍
convert +contrast +contrast 1.jpg 2.jpg 变浅2倍
38 单纯旋转角度
正值是顺时针 负值是逆时针
convert -rotate 30 origin.jpg dog.jpg
convert -rotate -30 origin.jpg dog.jpg
39 模糊
高斯模糊:
convert -blur 80 foo.jpg foo.png-blur
参数还可以这样-blur 80×5。后面的那个5表示的是Sigma的值，这个是图像术语，我也不太清楚，总之，它的值对模糊的效果起关键的作用。
40 翻转
上下翻转：
convert -flip foo.png bar.png
左右翻转：
convert -flop foo.png bar.png
41 反色
形成底片的样子：
convert -negate foo.png bar.png
42 单色
把图片变为黑白颜色：
convert -monochrome foo.png bar.png
43 加噪声
convert -noise 3 foo.png bar.png
44 油画效果
我们可用这个功能，把一张普通的图片，变成一张油画，效果非常的逼真
convert -paint 4 foo.png bar.png
45 旋转
把一张图片，旋转一定的角度：
convert -rotate 30 foo.png bar.png
上面的30，表示向右旋转30度，如果要向左旋转，度数就是负数。
46 炭笔效果
convert -charcoal 2 foo.png bar.png
形成炭笔或者说是铅笔画的效果。
47 散射
毛玻璃效果：
convert -spread 30 foo.png bar.png
48 漩涡
以图片的中心作为参照，把图片扭转，形成漩涡的效果：
convert -swirl 67 foo.png bar.png
49 凸起效果
用-raise来创建凸边：
convert -raise 5×5 foo.png bar.png
执行后，你会看到，照片的四周会一个5×5的边，如果你要一个凹下去的边，把-raise改为+raise就可以了。其实凸边和凹边看起来区别并不是很大。
其他
其他功能都是不太常用的，如果你感兴趣的话，可以看它的联机文档
50. 图片翻转后添加背景色
convert -rotate 45 -background #FFB6C1 origin.jpg or.jpg
import
import是一个用于屏幕截图的组件，下面列出的是我们常用的功能，其他的功能，你参考它的man好了。
截取屏幕的任一矩形区域
import foo.png在输入上述的命令后，你的鼠标会变成一个十字，这个时候，你只要在想要截取的地方划一个矩形就可以了
截取程序的窗口
import -pause 3 -frame foo.png
回 车后，用鼠标在你想截的窗口上点一下即可。参数-frame的作用是告诉import，截图的时候把目标窗口的外框架带上，参数-pause的作用很重 要，你可以试着把它去掉，对比一下，你会发现，目标窗口的标题栏是灰色的，pause就是让import稍微延迟一下，等你的目标窗口获得焦点了，才开始 截图，这样的图才比较自然。
截取一个倾斜的窗口
如果想让你的截图比较cool，你可以把截取一个倾斜的窗口，方法如下：
import -rotate 30 -pause 3 -frame foo.png
截取整个屏幕
import -pause 3 -window root screen.png
注意，暂停了3秒钟，你需要在3秒钟内切换到需要截取的画面噢。
display
display应该是我们使用的最为频繁的图像处理软件了，毕竟，还是看的多
显示图片
display foo.png
如果你要显示多个文件，你可以使用通配符
display *.png
幻灯片
display -delay 5 *
每隔5个百分之秒显示一张图片
一些快捷键
space(空格): 显示下一张图片
backspace(回删键):显示上一张图片
h: 水平翻转
v: 垂直翻转
/:顺时针旋转90度
:逆时针旋转90度
>: 放大
<: 缩小
F7:模糊图片
Alt+s:把图片中间的像素旋转
Ctrl+s:图象另存
Ctrl+d:删除图片
q: 退出
/*************************************************************************************************
/*************************************************************************************************
/*************************************************************************************************
/*************************************************************************************************
/*************************************************************************************************
一个例子
下面还是让我们来把几个 EmacsColorTheme 的抓图制作成几张叠加的照片的
效果。首先分别设定几个 color-theme ，然后分别建立抓图：
代码:import -frame 1.png
import -frame 2.png
import -frame 3.png
import -frame 4.png
抓的图片太大了，让我们先把他们缩小一点吧：
代码:for file in *.png; do
convert $file -resize 400x $file
done
好了，现在让我们来制作相片的边框，其实就是先加一个白色的边框，再加一个
灰色的边框，先用产生一个小一点的图来试验一下吧：
代码:convert 1.png -resize 100x100 -bordercolor white -border 6
-bordercolor grey60 -border 1 1-border.png
OK！这样就是边框了，如图所示：
这样还不够 cool ，让我们来产生一点阴影：
代码:convert 1-border.png -shadow 60x4+4+4 1-shadow.png
这样就产生了阴影：
接下来要做的是把阴影和原来的图形融合到一起：
代码:convert 1-shadow.png 1-border.png -background none
-mosaic 1-border-shadow.png
把上面的步骤连在一起做就是：
代码:convert 1.png -resize 100x100 
-bordercolor white -border 6 
-bordercolor grey60 -border 1 
( +clone -shadow 60x4+4+4 ) 
+swap -background none -mosaic
1-border-shadow.png
好了，知道如何制作边框了，现在要把几张“照片”组合在一起，让我们再用同样
的办法产生一个 2-border-shadow.png ，然后把一张旋转一下，移动一点，然
后把他们组合在一起：
代码:convert 1-border-shadow.png 
( -page 20x30 2-border-shadow.png 
-background none -rotate 30 ) 
-background none -mosaic mosaic.png
好了！这就是基本流程，用类似的办法，我们可以把四张“照片”组合在一起了，
而且我决定最后才加上阴影，而不是每一张都加阴影：
代码:
convert ( -page +0+40 1.png 
-bordercolor white -border 6 
-bordercolor grey60 -border 1 
-background none -rotate -5 ) 
( -page +30+0 2.png 
-bordercolor white -border 6 
-bordercolor grey60 -border 1 
-background none -rotate 20 ) 
( -page +10+40 3.png 
-bordercolor white -border 6 
-bordercolor grey60 -border 1 
-background none -rotate 40 ) 

