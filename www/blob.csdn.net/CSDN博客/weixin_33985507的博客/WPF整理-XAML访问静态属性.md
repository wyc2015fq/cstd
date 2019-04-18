# WPF整理-XAML访问静态属性 - weixin_33985507的博客 - CSDN博客
2018年05月29日 00:46:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：15
原文:[WPF整理-XAML访问静态属性](https://www.cnblogs.com/DebugLZQ/p/3153104.html)
*"XAML provides an easy way to set values of properties—**type converters** and the **extended property syntax** allow for flexible setting of values. However, some things cannot be expressed as a simple value, such as setting a property to the value of some static property."*
这个相对比较简单，知道就行，Code Snip如下：
```
<StackPanel>
        <Ellipse Stroke="Black" Height="50" Fill="{x:Static SystemColors.DesktopBrush}"/>
        <Rectangle Stroke="Black" Height="50" Fill="{x:Static SystemColors.ActiveCaptionBrush}"/>
    </StackPanel>
```
请对比：[WPF整理-使用用户选择主题的颜色和字体](http://www.cnblogs.com/DebugLZQ/archive/2013/06/27/3158540.html) ，区别两者的不同点。
