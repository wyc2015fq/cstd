# 图像滤镜艺术---Hudson滤镜(Instagram)

2014年11月19日 20:18:48

 

Trent1985

 

阅读数：2375

更多

所属专栏： [图像滤镜艺术](https://blog.csdn.net/column/details/zphotoimagefilter.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/41286483

​    今天给大家实现的是Instagram中的Hudson滤镜，为什么介绍Instagram滤镜，原因很简单，Instagram本身就是一个巨大的成功，迄今为止，没有哪个软件的滤镜效果可以超越它的。

​    Hudson滤镜比较简单，主要是一个模板的寻找，这个模板，我也是在网上找到的，如果单单用PS去实现这个模板效果，我觉得我们是舍易取难，我们追求的是最后的效果，而不是过程呵呵。

​    首先看下对比效果：

​    ![img](https://img-blog.csdn.net/20141119201241437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​                                    (a) Instagram  Hudson效果

​    ![img](https://img-blog.csdn.net/20141119201517000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​                                                 (b) PS效果图

​     ![img](https://img-blog.csdn.net/20141119201336365?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​                                                                    (c) C# 效果图

​    按照原则，先介绍PS实现过程：

​    1，打开原始图像以及模板图像，Instagram的目标效果图像：

![img](https://img-blog.csdn.net/20141119194318750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

其中，图层0为原始图像，图层1为模板图像(模板图像与psd文件一起，我都会放到包里供大家下载使用)，另外的Hudson即Instagram中的效果图像。

​    2，选中图层1，选择“叠加”图层混合模式，即得到效果图：

![img](https://img-blog.csdn.net/20141119194621201?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​    然后，我们介绍C#实现：

​    按照PS的过程，C#代码如下：

​    

```csharp
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
 
namespace SpecialeffectDemo
{
    unsafe class HudsonFilter
    {
        public HudsonFilter(Bitmap src,Bitmap bg_a)
        {
            this.srcBitmap = src;
            this.map = bg_a;
        }
        private Bitmap srcBitmap = null;
        private Bitmap map = null;
        public Bitmap Apply()
        {
            if (srcBitmap != null)
            {
                Bitmap src = new Bitmap(srcBitmap);
                int w = srcBitmap.Width;
                int h = srcBitmap.Height;
                BitmapData srcData = src.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
                BitmapData mapaData = map.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
                byte* p = (byte*)srcData.Scan0;
                byte* pa = (byte*)mapaData.Scan0;
                int r = 0, g = 0, b = 0;
                for (int j = 0; j < h; j++)
                {
                    for (int i = 0; i < w; i++)
                    {
                        b = p[0];
                        g = p[1];
                        r = p[2];
 
                        b = SpecialEffectClass.ModeSuperpositionEffect(b, pa[0]);
                        g = SpecialEffectClass.ModeSuperpositionEffect(g, pa[1]);
                        r = SpecialEffectClass.ModeSuperpositionEffect(r, pa[2]);
 
                        p[0] = (byte)b;
                        p[1] = (byte)g;
                        p[2] = (byte)r;
 
                        p += 4;
                        pa += 4;
                    }
                    p += srcData.Stride - w * 4;
                    pa += mapaData.Stride - w * 4;
                }
                src.UnlockBits(srcData);
                map.UnlockBits(mapaData);
                return src;
            }
            else
            {
                return null;
            }
        }
       
    }
}
```


 

界面效果：

![img](https://img-blog.csdn.net/20141119201017480?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​    最后给出代码和PS实现过程的包，免费下载地址<http://download.csdn.net/detail/trent1985/8177015>

​    以上就是Hudson这款滤镜的实现过程，希望大家喜欢，有什么问题可以给我留言或者邮件：

邮箱[dongtingyueh@163.com](mailto:dongtingyueh@163.com)  QQ:1358009172



**最后，分享一个专业的图像处理网站（微像素），里面有很多源代码下载：**

<http://www.zealpixel.com/portal.php>