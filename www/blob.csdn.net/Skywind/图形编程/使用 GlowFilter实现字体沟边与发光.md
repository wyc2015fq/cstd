# Skywind Inside » 使用 GlowFilter实现字体沟边与发光
## 使用 GlowFilter实现字体沟边与发光
April 19th, 2013[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
如果你正在使用 Flash，那么实现下面一个字体效果是一件十分简单的事情：
![image](http://www.skywind.me/blog/wp-content/uploads/2013/04/image_thumb.png)
textfield.filters = [ new GlowFilter(0, 1， 2， 2， 10) ];
这样就可以了，接着把字体设置成宋体12号，颜色是0xffff99，就成了。
![image](http://www.skywind.me/blog/wp-content/uploads/2013/04/image_thumb1.png)
如果要实现上面类似QQ面板的发光效果，也只需要一行：
textfield.filters = [ new GlowFilter(0xffffffff, 1, 6, 6, 0.9) ];
看起来这个 GlowFilter是无所不能呀，那么如果你在使用C++的话，如何用C++来实现一个Glow效果呢？
而且如果你正在使用3D引擎的话，如何用GPU来实现上面的效果呢？详细见下文：
TO BE CONTINUE….
