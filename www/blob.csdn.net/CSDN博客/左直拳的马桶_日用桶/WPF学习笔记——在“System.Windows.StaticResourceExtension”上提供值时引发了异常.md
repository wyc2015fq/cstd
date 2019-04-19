# WPF学习笔记——在“System.Windows.StaticResourceExtension”上提供值时引发了异常 - 左直拳的马桶_日用桶 - CSDN博客
2014年05月10日 13:35:24[左直拳](https://me.csdn.net/leftfist)阅读数：7921
# [在“System.Windows.StaticResourceExtension”上提供值时引发了异常](http://q.cnblogs.com/q/27557/)
因应需要，写了一个转换器，然后窗体上引用，结果就出来这个错。编译的时候没事，运行阶段就异常。
难道是转换器写错啦？断点调试，发现根本还没运行到转换器代码。仔细一看，系统报的是XAML解释的错，就纯粹是页面这里的问题。
页面代码是这样的：
```
<Window
    xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
    xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
    xmlns:d="http://schemas.microsoft.com/expression/blend/2008"
    xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006" 
    xmlns:cvt="clr-namespace:Converters"
    IsEnabled="{Binding HasShowedUser,Converter={StaticResource cvtBool},Mode=OneWay}">
    <Window.Resources>
        <cvt:GenericTypeConverter x:Key="cvtBool"/>
    </Window.Resources>
    <Grid>
        <Button x:Name="btnShowUser" Content="秀一下" Height="50" Width="100" Click="btnShow_Click"  />
    </Grid>
</Window>
```
寻寻觅觅，冷冷清清凄凄惨惨戚戚，百撕不得骑姐。
后来将 
`IsEnabled="{Binding HasShowedUser,Converter={StaticResource cvtBool},Mode=OneWay}">`这一句从<Window> 移到 <Button>里，结果一切正常。
这说明，之前的错误是因为资源声明在后，而我的调用在前，因此报错了。
怎么办？难道<Window>就不能使用了吗？
解决方法是：将资源声明放到一个独立的XAML文件里，比如起名叫 StaticResources.xaml，然后，在App.xaml里包含这个StaticResources.xaml，最后在窗体就可以正常使用这些资源了。
StaticResources.xaml
```
<ResourceDictionary 
    xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
    xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
    xmlns:cvt="clr-namespace:Converters"
    >
    <cnvtrs:GenericTypeConverter x:Key="cvtBool" />
</ResourceDictionary>
```
app.xaml
```
<Application 
    x:Class="My.App"
    xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
    xmlns:telerik="http://schemas.telerik.com/2008/xaml/presentation" 
    xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
    StartupUri="TestWindow.xaml">
    <Application.Resources>
        <ResourceDictionary>
            <ResourceDictionary.MergedDictionaries>
		<ResourceDictionary Source="Styles.xaml"/>
                <ResourceDictionary Source="StaticResources.xaml"/>
            </ResourceDictionary.MergedDictionaries>
        </ResourceDictionary>
    </Application.Resources>
</Application>
```
调用窗体
```
<Window
    xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
    xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
    xmlns:d="http://schemas.microsoft.com/expression/blend/2008"
    xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006" 
    IsEnabled="{Binding HasShowedUser,Converter={StaticResource cvtBool},Mode=OneWay}">
    <Grid>
        <Button x:Name="btnShowUser" Content="秀一下" Height="50" Width="100" Click="btnShow_Click"  />
    </Grid>
</Window>
```

