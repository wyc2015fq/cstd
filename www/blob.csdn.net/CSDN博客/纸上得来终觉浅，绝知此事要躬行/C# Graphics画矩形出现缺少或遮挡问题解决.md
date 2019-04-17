# C# Graphics画矩形出现缺少或遮挡问题解决 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年08月31日 14:03:59[boonya](https://me.csdn.net/boonya)阅读数：589








下面我们以多屏画视频播放的边框为例。

错误效果:

![](https://img-blog.csdn.net/20180831140150313?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

期望效果：

![](https://img-blog.csdn.net/20180831140324367?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
/// <summary>
        /// 绘制播放视频控件边框(0:黑色,1:绿色)
        /// </summary>
        /// <param name="color">0:黑色,1:绿色</param>
        public void DrawVideoBorder(int color)
        {
            // 画图对象需要每次都创建，类似初始化一个不同的画布,否则出现矩形不完整情况
            graphics = this.CreateGraphics();
            
            //if (graphics == null)
            //{
            //    graphics = this.CreateGraphics();
            //}
            lock (graphics)
            {
                if (color == 1)
                {
                    graphicsColor = Color.FromArgb(60, 255, 60);
                    // 绿色
                    pen = new Pen(graphicsColor);
                } else if (color == 2)
                {
                    graphicsColor = Color.FromArgb(255, 60, 60);
                    // 红色
                    pen = new Pen(graphicsColor);
                }
                else
                {
                    graphicsColor = Color.FromArgb(105, 105, 105);
                    // 黑色
                    pen = new Pen(graphicsColor);
                }
                Rectangle rect = new Rectangle();
                rect.X = 0;
                rect.Y = 0;
                rect.Width = this.Width - 3;
                rect.Height = this.Height - 3;
                rect.Location = new Point(0,0);
                graphics.DrawRectangle(pen, rect);
            }
        }
```

请注意看注释行处的说明。



