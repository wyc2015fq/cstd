# 图像滤镜艺术--Toaster滤镜

2014年11月07日 15:46:18

 

Trent1985

 

阅读数：1809

更多

所属专栏： [图像滤镜艺术](https://blog.csdn.net/column/details/zphotoimagefilter.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/40893175

​    根据Instagram CEO的说法，Toaster滤镜是Instagram所有滤镜中最复杂的滤镜，这个滤镜给人一种新奇的红色烘烤感，很能让人联想起这个Toaster单词的本意呵呵，今天我将用C#实现这个滤镜，当然我也只是探索，如何逼近它的效果，差异的存在在所难免，大家勿怪哦。

​    按照之前的说法，还是先使用Photoshop进行模拟，然后根据模拟步骤，进行C#代码编写。

首先，看下效果图：

 

![img](https://img-blog.csdn.net/20141107151146453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​                                                               (a)PS效果图

![img](https://img-blog.csdn.net/20141107151210593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​                                                             (b)Instagram效果图

![img](https://img-blog.csdn.net/20141107151540203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​                                                         (c)C#代码效果图

​    效果怎么样我说了不算呵呵，下面介绍PS实现步骤：

​    1，打开测试图像，命名为图层Source

​    2，创建三个模板，这里三个模板我已经给大家提供，在下载包里，当然这三个模板是我根据Instagram变换出来的，大家只需要拿来使用就行了，将三个模板分别命名为a,b,c，如下图所示：

![img](https://img-blog.csdn.net/20141107152419875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​    3，对图层a执行"混合图层"---"正片叠底"

![img](https://img-blog.csdn.net/20141107152543890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​    4，对图层b执行"混合图层"---"滤色"：

![img](https://img-blog.csdn.net/20141107152613023?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​    5，对图层c执行"混合图层"---"柔光"：

![img](https://img-blog.csdn.net/20141107152805468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​    这样效果图就出来了，看起来是不是相当简单呢？

​    下面我们介绍C#代码实现：

​    这里实现的关键也就是讲三个图层分别按照相应的图层混合模式混合起来就可以了，我这里新建了一个ToasterFilter的类，代码如下：

```csharp
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
 
namespace SpecialeffectDemo
{
    unsafe class ToasterFilter
    {
        public ToasterFilter(Bitmap src,Bitmap bg_a,Bitmap bg_b,Bitmap bg_c)
        {
            this.srcBitmap = src;
            this.mapa = bg_a;
            this.mapb = bg_b;
            this.mapc = bg_c;
        }
        private Bitmap srcBitmap = null;
        private Bitmap mapa = null;
        private Bitmap mapb = null;
        private Bitmap mapc = null;
        public Bitmap Apply()
        {
            if (srcBitmap != null)
            {
                Bitmap src = new Bitmap(srcBitmap);
                int w = srcBitmap.Width;
                int h = srcBitmap.Height;
                BitmapData srcData = src.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
                BitmapData mapaData = mapa.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
                BitmapData mapbData = mapb.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
                BitmapData mapcData = mapc.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
                byte* p = (byte*)srcData.Scan0;
                byte* pa = (byte*)mapaData.Scan0;
                byte* pb = (byte*)mapbData.Scan0;
                byte* pc = (byte*)mapcData.Scan0;
                int r = 0, g = 0, b = 0, a = 0, t = 0;
                for (int j = 0; j < h; j++)
                {
                    for (int i = 0; i < w; i++)
                    {
                        b = p[0];
                        g = p[1];
                        r = p[2];
                        
                        b = SpecialEffectClass.ModeMultiplyEffect(b, pa[0]);
                        g = SpecialEffectClass.ModeMultiplyEffect(g, pa[1]);
                        r = SpecialEffectClass.ModeMultiplyEffect(r, pa[2]);
 
                        b = SpecialEffectClass.ModeFilterColorEffect(b, pb[0]);
                        g = SpecialEffectClass.ModeFilterColorEffect(g, pb[1]);
                        r = SpecialEffectClass.ModeFilterColorEffect(r, pb[2]);
 
                        a = pc[3];
                        if (a == 255)//由于模板图像中存在透明度变化，因此选用32位bgra格式
                        {
                            b = SpecialEffectClass.ModeSmoothLightEffect(b, pc[0]);
                            g = SpecialEffectClass.ModeSmoothLightEffect(g, pc[1]);
                            r = SpecialEffectClass.ModeSmoothLightEffect(r, pc[2]);
                        }
                        else
                        {
                            t = SpecialEffectClass.ModeSmoothLightEffect(b, pc[0]);
                            b = (b * (255 - a) + t * a)/255;
                            t = SpecialEffectClass.ModeSmoothLightEffect(g, pc[1]);
                            g = (g * (255 - a) + t * a)/255;
                            t = SpecialEffectClass.ModeSmoothLightEffect(r, pc[2]);
                            r = (r * (255 - a) + t * a)/255;
                        }
                        p[0] = (byte)b;
                        p[1] = (byte)g;
                        p[2] = (byte)r;
                        p += 4;
                        pa += 4;
                        pb += 4;
                        pc += 4;
                    }
                    p += srcData.Stride - w * 4;
                    pa += mapaData.Stride - w * 4;
                    pb += mapbData.Stride - w * 4;
                    pc += mapcData.Stride - w * 4;
                }
                src.UnlockBits(srcData);
                mapa.UnlockBits(mapaData);
                mapb.UnlockBits(mapbData);
                mapc.UnlockBits(mapcData);
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

注意：由于图层C是具有不同的透明度的，因此，我们在程序中要使用PixelFormat.Format32bppArgb，而不再是24位；

程序效果图稍微偏暗，大家可以调整一下亮度即可，最后，放上Demo的下载链接，当然，里面有两个包，一个是Photoshop文件，供大家进行PS实践检验，一个是C#的Demo：<http://download.csdn.net/detail/trent1985/8132433>

如有问题，联系我。邮箱：[dongtingyueh@163.com,QQ:13634132112](mailto:dongtingyueh@163.com,QQ:13634132112)

 

**最后，分享一个专业的图像处理网站（微像素），里面有很多源代码下载：**

<http://www.zealpixel.com/portal.php>