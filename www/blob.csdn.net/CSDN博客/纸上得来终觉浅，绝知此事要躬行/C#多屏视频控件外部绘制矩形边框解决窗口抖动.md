# C#多屏视频控件外部绘制矩形边框解决窗口抖动 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年09月07日 12:09:07[boonya](https://me.csdn.net/boonya)阅读数：148








视频在播放过程中如果是绘制矩形边框到视频界面那么视频播放的时候会冲掉边框，所以需要每播放一次画面更新一下边框，这样就增加了额外性能开销，所以此方法不推荐使用（图形本身比较流畅，但因为加了边框之后看起来是在抖动的）。于是后来想着通过线条绘制窗口来实现边框，但是比较复杂，每个图形的边界和位置都需要计算，而且实现过程中相同的算法出现了线条模糊的情况。最终，选择调整内部视频的大小来允许绘制一个边框，这种方式只需要对多个视频进行重新布局就可以了。



**目录**

[需求](#%E9%9C%80%E6%B1%82)

[设计边框](#%E8%AE%BE%E8%AE%A1%E8%BE%B9%E6%A1%86)

[线条绘制模型](#%E7%BA%BF%E6%9D%A1%E7%BB%98%E5%88%B6%E6%A8%A1%E5%9E%8B)

[内嵌布局模型](#%E5%86%85%E5%B5%8C%E5%B8%83%E5%B1%80%E6%A8%A1%E5%9E%8B)

[成品验收](#%E6%88%90%E5%93%81%E9%AA%8C%E6%94%B6)

[绘图代码](#%E7%BB%98%E5%9B%BE%E4%BB%A3%E7%A0%81)



### 需求

选中视频边框标识显示：添加边框效果。

### 设计边框

![](https://img-blog.csdn.net/20180907120213948?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 线条绘制模型

![](https://img-blog.csdn.net/20180907120309959?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 内嵌布局模型

![](https://img-blog.csdn.net/2018090712035862?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 成品验收

![](https://img-blog.csdn.net/20180907120513670?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180907120542249?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 可以看到视频播放边框也不会抖动，因为边框做到了与播放器无关。

### 绘图代码

这个比较简单

```
#region 绘制视频选中区域边框    
        /// <summary>
        /// 矩形对象
        /// </summary>
        Rectangle rect = new Rectangle();
        /// <summary>
        /// 画图颜色
        /// </summary>
        static Color GreenColor = Color.FromArgb(60, 255, 60);
        static Color RedColor = Color.FromArgb(255, 60, 60);
        static Color BlackColor = Color.FromArgb(105, 105, 105);
        static Color WhiteColor = Color.FromArgb(255, 255, 255);
        /// <summary>
        /// 画图画笔
        /// </summary>
        Pen GreenPen = new Pen(GreenColor);
        Pen RedPen = new Pen(RedColor);
        Pen BlackPen = new Pen(BlackColor);
        Pen WhitePen = new Pen(WhiteColor);
        /// <summary>
        /// 绘制播放视频控件边框(1:绿色,2:红色,3:黑色,4:白色)
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <param name="color">1:绿色,2:红色,3:黑色,4:白色</param>
        public void DrawVideoBorder(int x,int y,int width,int height,int color)
        {
            // 画图对象需要每次都创建，类似初始化一个不同的画布,否则出现矩形不完整情况
            Graphics graphics = this.CreateGraphics();

            lock (graphics)
            {
                rect.X = x;
                rect.Y = y;
                rect.Width = width;
                rect.Height = height;
                rect.Location = new Point(rect.X, rect.Y);

                Pen pen = null;// 当前需要的画笔颜色
                switch (color)
                {
                    case 1:
                        pen = GreenPen;
                        break;
                    case 2:
                        pen = RedPen;
                        break;
                    case 3:
                        pen = BlackPen;
                        break;
                    case 4:
                        pen = WhitePen;
                        break;
                }
                graphics.DrawRectangle(pen, rect);
            }
            
        }
```

只需要布局重绘一次内部视频控件就OK。代码逻辑如下：

```
/// <summary>
        /// 布局视频:支持分屏数量(1、4、6、9、10、16)
        /// </summary>
        /// <returns></returns>
        public void LayoutUCVideosForBorder(int num)
        {
            var selectedIndex = this.SelectedIndex;
            // 重新布局视频
            LayoutUCVideos(screenNumber);
            // 重新设置选中的窗口ID
            this.SelectedIndex = selectedIndex;
            // 画矩形边框
            for (var i = 0; i < this.Controls.Count; i++)
            {
                UCVideo video = this.Controls[i] as UCVideo;
                if (this.SelectedIndex == video.Id)
                {
                    // 允许播放SDL背景框
                    video.DrawSDLBorder(true);
                    // 被选中窗口播放SDL音频
                    video.OpenSDLAudio(SDL_AUDIO_OBJECT);
                    // 选中状态标识
                    video.IsSelected = true;

                    // 缩小视频窗口并画边框
                    video.Width = video.Width - 2;
                    video.Height = video.Height - 2;
                    video.Location = new Point(video.X + 1, video.Y + 1);
                    DrawVideoBorder(video.X, video.Y, video.Width + 1, video.Height + 1, 1);
                    break;
                }
            }
        }
```

上面方法是放在控件单击事件处理的。



