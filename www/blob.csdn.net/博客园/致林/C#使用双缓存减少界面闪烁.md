
# C\#使用双缓存减少界面闪烁 - 致林 - 博客园






# [C\#使用双缓存减少界面闪烁](https://www.cnblogs.com/bincoding/p/7772547.html)
场景：一个panel中动态加载多个自定义控件item，类似QQ聊天窗口
问题：加载panel时界面会卡顿，先显示阴影再显示界面；移动滚动条时item会闪烁
解决方法：
panel 添加方法，减少界面闪烁：
protectedoverrideCreateParams CreateParams
{get{
         CreateParams cp=base.CreateParams;
         cp.ExStyle|=0x02000000;returncp;
    }  
}
item构造函数中添加代码，减少移动滚动条时闪烁：
//构造函数publicitem()
{
    SetStyle(
          ControlStyles.AllPaintingInWmPaint|//不闪烁ControlStyles.OptimizedDoubleBuffer//支持双缓存,true);
    InitializeComponent();
}
网络限制不能复制，代码手打的，可能拼写有错





