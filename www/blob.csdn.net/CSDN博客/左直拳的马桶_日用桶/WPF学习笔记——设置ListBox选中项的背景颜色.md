# WPF学习笔记——设置ListBox选中项的背景颜色 - 左直拳的马桶_日用桶 - CSDN博客
2014年05月29日 16:46:58[左直拳](https://me.csdn.net/leftfist)阅读数：14290
ListBox的选中项，在我这个WIN7里面，是亮蓝色，颜色是如此之浓厚，差不多遮盖了前景的字体！
太不协调了。可是怎么设置呢？设置触发器，又是IsMouseOver，又是IsFocused，在谷歌里寻寻觅觅，无限的代码，无限的垃圾，无限的不知所谓。
踏破铁鞋无觅处，偶然发现，应该这样写：
```
<Style x:Key="UserItemContainerStyle" TargetType="ListBoxItem">
        <Style.Resources>
            <!--SelectedItem with focus-->
            <SolidColorBrush x:Key="{x:Static SystemColors.HighlightBrushKey}" Color="LightBlue" Opacity=".4"/>
            <!--SelectedItem without focus-->
            <SolidColorBrush x:Key="{x:Static SystemColors.ControlBrushKey}" Color="LightBlue" Opacity=".4"/>
        </Style.Resources>
        <!-- 设置触发器 -->
        <Style.Triggers>
            <Trigger Property="IsMouseOver" Value="true">
                <Setter Property="Background" Value="#efefef"/>
                <Setter Property="Foreground" Value="Red"/>
            </Trigger>
            <Trigger Property="IsFocused" Value="true">
                <Setter Property="Background" Value="Coral"/>
                <Setter Property="Foreground" Value="Red"/>
            </Trigger>
        </Style.Triggers>
    </Style>
```
这里设置触发器其实与选中没有关系，但还是可以用于设置鼠标滑过的样式。
