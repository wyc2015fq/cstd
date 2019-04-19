# WPF学习笔记——为BUTTON添加背景图片 - 左直拳的马桶_日用桶 - CSDN博客
2014年05月28日 20:51:28[左直拳](https://me.csdn.net/leftfist)阅读数：11275
首先要肯定，代码：
```
<Style x:Key="UserItemButton" TargetType="Button">
        <Setter Property="BorderBrush"  Value="Transparent"></Setter>
        <Setter Property="FocusVisualStyle" Value="{x:Null}"/>    
        <Setter Property="Background">
            <Setter.Value>
                <ImageBrush ImageSource="/wpffirst;component/Assets/Images/User/transparent.gif" Stretch="Fill"/>
            </Setter.Value>
        </Setter>
    </Style>
```
是正确的。这里对Background的设置，就是用于添加一张背景图片。
这样的代码，编译绝对没有问题。但一运行，马上报错。报的是XML解析的错误。
找来找去，问题在于图片 “transparent.gif" 上。图片是存在的，但问题在于它还没有被包含进项目中，所以运行就出错了。在VS中，将图片包含进项目，或是将路径改成绝对路径，就一切OK。
这种错误，不报找不到图片，却报XML解析错误，真让人丈二金刚，摸不着头脑。
