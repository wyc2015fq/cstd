# 图像滤镜艺术---保留细节的磨皮之C#程序实现

2015年12月14日 10:31:36

 

Trent1985

 

阅读数：8940

更多

所属专栏： [图像滤镜艺术](https://blog.csdn.net/column/details/zphotoimagefilter.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/50291097

上一篇博文“[保留细节的磨皮滤镜之PS实现](http://blog.csdn.net/trent1985/article/details/50260881)”一文中，我简单介绍了本人自己总结的一种非常简单的磨皮滤镜，这个滤镜在磨光皮肤的同时，会保留很不错的细节，今天，我将介绍使用C#程序实现这个磨皮的过程。

这里，我们同样是使用ZPhotoEngine库来实现，毕竟这个库中实现的效果跟PS是几乎一模一样的，关于下载地址，文章最后会给出，废话不多说了，磨皮步骤如下：



一，对原图的副本a执行表面模糊，半径15；

二，对原图执行高反差保留，半径1.0；

三，对高反差结果与原图做线性光图层处理，50%透明度即可；

根据以上三步，我的磨皮类主要代码如下：



```csharp
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
 
namespace TestDemo
{
    unsafe class ImageFilter
    {
        ZPhotoEngineDll zp = new ZPhotoEngineDll();
        public Bitmap SoftSkinFilter(Bitmap src, int blurRadius)
        {
            //表面模糊图层
            Bitmap a = zp.SurfaceBlur(src, 28, blurRadius);
            //高反差图层
            Bitmap highPass = zp.HighPassProcess(src, 1.0f);
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData dstData = highPass.LockBits(new Rectangle(0, 0, highPass.Width, highPass.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            byte* p = (byte*)srcData.Scan0;
            byte* dstP = (byte*)dstData.Scan0;
            int offset = srcData.Stride - a.Width * 4;
            int temp = 0;
            for (int j = 0; j < a.Height; j++)
            {
                for (int i = 0; i < a.Width; i++)
                {
                    ////////////////Process image...
                    //线性光图层混合
                    temp = zp.ModeLinearLight(p[0], dstP[0]);
                    //透明度50%
                    dstP[0] = (byte)((p[0] + temp) >> 1);
                    temp = zp.ModeLinearLight(p[1], dstP[1]);
                    dstP[1] = (byte)((p[1] + temp) >> 1);
                    temp = zp.ModeLinearLight(p[2], dstP[2]);
                    dstP[2] = (byte)((p[2] + temp) >> 1);
                    dstP += 4;
                    p += 4;
                }
                dstP += offset;
                p += offset;
            }
            a.UnlockBits(srcData);
            highPass.UnlockBits(dstData);
            return highPass;
        }
        
    }
}
```

界面部分主要代码如下：



```csharp
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;
 
namespace TestDemo
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
 
        #region  变量声明
        //图像路径
        private String curFileName = null;
        //当前图像变量
        private Bitmap curBitmap = null;
        //原始图像变量
        private Bitmap srcBitmap = null;
        //
        ImageFilter imfilter = new ImageFilter();
        #endregion
 
        #region  图像打开保存模块
        //打开图像函数
        public void OpenFile()
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "所有图像文件 | *.bmp; *.pcx; *.png; *.jpg; *.gif;" +
                   "*.tif; *.ico; *.dxf; *.cgm; *.cdr; *.wmf; *.eps; *.emf|" +
                   "位图( *.bmp; *.jpg; *.png;...) | *.bmp; *.pcx; *.png; *.jpg; *.gif; *.tif; *.ico|" +
                   "矢量图( *.wmf; *.eps; *.emf;...) | *.dxf; *.cgm; *.cdr; *.wmf; *.eps; *.emf";
            ofd.ShowHelp = true;
            ofd.Title = "打开图像文件";
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                curFileName = ofd.FileName;
                try
                {
                    curBitmap = (Bitmap)System.Drawing.Image.FromFile(curFileName);
                    srcBitmap = new Bitmap(curBitmap);
                }
                catch (Exception exp)
                { MessageBox.Show(exp.Message); }
            }
        }
        //保存图像函数
        public void SaveFile()
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "PNG文件(*.png)|*.png";
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                pictureBox1.Image.Save(sfd.FileName, ImageFormat.Png);
            }
 
        }
        //打开图像
        private void openBtn_Click(object sender, EventArgs e)
        {
            OpenFile();
            if (curBitmap != null)
            {
                pictureBox1.Image = (Image)curBitmap;
            }
        }
        //保存图像
        private void saveBtn_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
                SaveFile();
        }
        #endregion
 
        //确定
        private void okBtn_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                int radius =  Convert.ToInt32(textBox1.Text.ToString());
                if (radius >= 0 && radius <= 20)
                {
                    pictureBox1.Image = (Image)imfilter.SoftSkinFilter(curBitmap, radius);
                }
            }
        }
 
    }
}
```

程序界面如下：

![img](https://img-blog.csdn.net/20151214102253312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
最后，放上效果图：

![img](https://img-blog.csdn.net/20151214102323274?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![img](https://img-blog.csdn.net/20151214102347406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

原图                                                                                  C#程序效果图



![img](https://img-blog.csdn.net/20151214102400339?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

PS效果图

大家可以对比一下，PS效果跟本文实现效果是一模一样的，差别几乎是肉眼看不到的呵呵。

最后，放上一些下载连接：

1，ZPhotoEngine库下载连接：[点击打开链接](http://www.zealpixel.com/portal.php?mod=view&aid=45)

2，磨皮代码DEMO免费下载连接：[点击打开链接](http://download.csdn.net/detail/trent1985/9353265)