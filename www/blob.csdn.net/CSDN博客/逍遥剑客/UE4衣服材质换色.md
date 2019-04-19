# UE4衣服材质换色 - 逍遥剑客 - CSDN博客
2017年10月23日 09:57:29[xoyojank](https://me.csdn.net/xoyojank)阅读数：5780
游戏中衣服换色是个很普遍的需求, 除直接替换贴图外, 还有使用程序参数来控制颜色的做法. 
原理就是使用一张灰白Mask图, 乘一个颜色, 替换原Diffuse贴图上的颜色 
![这里写图片描述](https://img-blog.csdn.net/20171023093653621?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171023093534865?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
效果如下: 
![这里写图片描述](https://img-blog.csdn.net/20171023093716973?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171023093825419?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171023093743549?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171023093757511?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这种制作方式有两个缺点: 一是换色区域只能换一个固定颜色, 只有明亮不一样; 二是如果让玩家来自定义颜色, 总会出现一些亮瞎眼的配色, 影响整个游戏的画面美感. 
那有没有一种方法可以同时更换多种颜色, 并且保证颜色搭配不要反差太大呢? 之前在天涯明月刀中看到过类似的实现, 后来一想就明白了, 把颜色转换到HSV空间就可以: 
![这里写图片描述](https://img-blog.csdn.net/20171023094122471?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
看上面这个色环, 每个颜色的色调其实可以定义成一个角度, 如果把想换色调的话, 其实只需要旋转这个色环, 相当于我们对整张图定义了一个调节板(或者叫颜色查找表), 每个颜色加个角度偏移就实现色调的调整, 同时又能保证颜色的亮度不会发生变化, 与之搭配的颜色也能同步调节到一个合适的色调上.  
参考[RGB to HSV to RGB – for Shaders](https://www.laurivan.com/rgb-to-hsv-to-rgb-for-shaders/), 可以实现RGB与HSV分量的互换转换, 那思路就是在转换到HSV空间后, 针对H加一个偏移值, 然后再转换回RGB空间, 在UE4中可以使用Custom Shader Node来实现: 
![这里写图片描述](https://img-blog.csdn.net/20171023095108044?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
定义成一个Material Function后就可以在每个材质中进行复用了: 
![这里写图片描述](https://img-blog.csdn.net/20171023095120859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
最终效果如下: 
![这里写图片描述](https://img-blog.csdn.net/20171023095210068?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171023095242086?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171023095259731?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171023095323199?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
实际使用时仍然可以搭配mask贴图来控制换色的区域, 但是免去了灰度图的制作和调色板的控制, 开放给玩家随意调节也没有大的风险, 颜色的搭配总是能控制在一个比较和谐的范围内. 
