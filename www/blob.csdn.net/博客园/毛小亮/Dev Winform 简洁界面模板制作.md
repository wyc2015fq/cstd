# Dev Winform 简洁界面模板制作 - 毛小亮 - 博客园
# [Dev Winform 简洁界面模板制作](https://www.cnblogs.com/xianerwonder/p/4447986.html)
今天看到Dev的安装程序，发现界面很漂亮如下图：
![](https://images0.cnblogs.com/blog2015/688944/201504/221627332031985.png)
于是想到做个类似的简洁明了的界面出来，平常开发小程序什么的都方便很多。
1、首先是自己添加了一个XtraForm，我们发现它有点丑（我为了性能，习惯把系统的特效都关了，所以这样更丑了。。）：
![](https://images0.cnblogs.com/blog2015/688944/201504/221633364065957.png)
2、这个窗体有个FormBorderStyle属性，调成None，然后随便加上几个控件（依然是丑，但是清新好多了）：
![](https://images0.cnblogs.com/blog2015/688944/201504/221639164374175.png)
3、最后调整一下控件的各种属性，包括Button的size和背景颜色，checkedit和label的大小，结果如下：
![](https://images0.cnblogs.com/blog2015/688944/201504/221645510624982.png)
4、已经是很简洁明快了呢~然后有个重要的问题，就是我们的窗口取消了标题栏，所以需要一个关闭和最小化按钮，窗体拖动的功能也没有，我在网上找了一下还是好实现的，用到了windows自己的api：
```csharp;gutter
//契约
        [DllImport("user32")]
        private static extern bool ReleaseCapture();
        [DllImport("user32")]
        private static extern bool SendMessage(IntPtr hwnd,int wMsg ,int wParam,int IParam);
//最终的mouseDown事件
        private void XtraForm1_MouseDown(object sender, MouseEventArgs e)
        {
            ReleaseCapture();
            SendMessage(this.Handle,0x0112,0xF012,0);
        }
```
5、最后结果:
![](https://images0.cnblogs.com/blog2015/688944/201504/221655225788814.png)
很简单的就完成了，也不复杂。而且本身dev自身带了很多模板，像是经典的ribbonForm。

