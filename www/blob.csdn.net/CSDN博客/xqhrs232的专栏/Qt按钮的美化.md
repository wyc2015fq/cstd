# Qt按钮的美化 - xqhrs232的专栏 - CSDN博客
2017年08月07日 17:45:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：403
原文地址::[http://blog.sina.com.cn/s/blog_618a9b97010112rh.html](http://blog.sina.com.cn/s/blog_618a9b97010112rh.html)
相关文章
1、[Qt之统一的UI界面格式基调，漂亮的UI界面](http://www.cnblogs.com/appsucc/archive/2012/11/05/2754767.html)----[http://www.cnblogs.com/appsucc/archive/2012/11/05/2754767.html](http://www.cnblogs.com/appsucc/archive/2012/11/05/2754767.html)
2、Qt自定义/自绘按钮类----[http://download.csdn.net/detail/lh806732/3889019](http://download.csdn.net/detail/lh806732/3889019)
3、QT自绘开关按钮----[http://download.csdn.net/detail/nightrainljh/6911723](http://download.csdn.net/detail/nightrainljh/6911723)
第一篇
[http://blog.csdn.net/yiyaaixuexi/article/details/6706729](http://blog.csdn.net/yiyaaixuexi/article/details/6706729)
第二篇
[http://blog.csdn.net/yiyaaixuexi/article/details/6343337](http://blog.csdn.net/yiyaaixuexi/article/details/6343337)
有很多朋友都抱怨，为什么自己使Qt做的小项目，UI那么土那么俗，方方框框，基本控件很丑，要不是Qt的跨平台，才不去学习它。呵呵，其实我想说，嵌入式系统中的图形界面，通通交给QT，绝对没问题！
    简单说说自定义Button，QPushButton是常用组件之一，先看下效果。
当单击按钮后，效果如图：
![2](http://hi.csdn.net/attachment/201104/23/6157840_13035243754W0p.jpg)
![点击按钮效果](http://hi.csdn.net/attachment/201104/23/6157840_13035243757e2j.jpg)
      实现代码：
[view plain](http://blog.csdn.net/yiyaaixuexi/article/details/6343337#)
- Button::Button(QWidget *parent) : QPushButton(parent)  
- {  
- //保存图片成员初始化
-     buttonPicture = **new** QPixmap();  
-     pressPicture = **new** QPixmap();  
-     releasePicture = **new** QPixmap();  
- 
- //关闭按钮的默认显示
- **this** -> setFlat(**true**);  
- }  
- 
- **void** Button::setButtonPicture(QPixmap pic)  
- {  
-     *buttonPicture = pic;  
- 
- **this** -> setIcon(QIcon(*buttonPicture));  
- }  
- 
- **void** Button::setPressPicture(QPixmap pic)  
- {  
-     *pressPicture = pic;  
- }  
- 
- **void** Button::setReleasePicture(QPixmap pic)  
- {  
-     *releasePicture = pic;  
- }  
- 
- **void** Button::set_X_Y_width_height(**int** x, **int** y, **int** width, **int** height)  
- {  
- **this** -> setIconSize(QSize(width, height));  
- **this** -> setGeometry(x, y, width, height);  
- }  
- 
- **void** Button::mouseDoubleClickEvent(QMouseEvent *event)  
- {  
- //null
- }  
- 
- **void** Button::mousePressEvent (QMouseEvent *event)  
- {  
- **this** -> setIcon (QIcon(*pressPicture));  
- }  
- 
- **void** Button::mouseMoveEvent(QMouseEvent *event)  
- {  
- //null
- }  
- 
- 
- **void** Button::mouseReleaseEvent (QMouseEvent *event)  
- {  
- **this** -> setIcon(QIcon(*releasePicture));  
-     emit clicked();  
- }  
 这下明白喽？我们只是需要在mousePressEvent和mouseReleaseEvent中，添加setIcon(QIcon(*buttonPicture))的处理，就让Button动起来了o(∩_∩)o ...
  顺便说下icon的资源处理，你不要告诉我你不会![PS](http://hi.csdn.net/attachment/201104/23/6157840_1303525809xkz2.jpg)哦，即使不会，咱可是可以写PS的无所不能的程序员，学一下怎么使用应该不难吧？
  将图片抠出所选区域，设置为透明png格式，这样不会覆盖背景。建议大家把文字也做进图片里，而不是在QT里文本，因为不同分辨率不同大小的嵌入式设备屏幕，显示效果不好控制。制作静态（同释放后）与点击状态两种按钮图标。
![好](http://hi.csdn.net/attachment/201104/23/6157840_13035267008fZ6.png)![人](http://hi.csdn.net/attachment/201104/23/6157840_1303526705zGt1.png)
  就是这样！简单吧？
  这里预留了mouseDoubleClickEvent(QMouseEvent *event)，mouseMoveEvent(QMouseEvent
 *event)的响应，热爱Qt的朋友不如自己动动手，美化自己的button~
