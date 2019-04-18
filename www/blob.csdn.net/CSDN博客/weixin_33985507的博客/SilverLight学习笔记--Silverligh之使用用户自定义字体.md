# SilverLight学习笔记--Silverligh之使用用户自定义字体 - weixin_33985507的博客 - CSDN博客
2009年08月22日 09:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
   学习一个小技巧，如何设置用户自定义的字体。
**一、准备工作：** 去网上下载一个自己喜欢的字体，这里我下载了一个名为Disko.ttf的字体库。
**二、如何操作：**
 新建一个Silverlight应用程序命名为:SLCustomerFont。
 其Page.xaml代码如下:
```
<UserControl x:Class="SLCustomerFont.Page"
    xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation" 
    xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml" 
    Width="600" Height="300">
    <Grid x:Name="LayoutRoot" Background="LightBlue">
       <TextBlock x:Name="txtBlockUserFont" 
                  Text="Show User Font" 
                  TextAlignment="Center"
                  FontSize="56" 
                  Margin="80"
                  FontFamily="Asian"
                  Foreground="Blue">           
       </TextBlock>   
    </Grid>
</UserControl>
```
   在TextBlock我们使用系统自带的字体  FontFamily="Asian"
   如何使用我们下载的Disko.ttf字体呢 
  步骤如下:
1 在Silverlight项目SLCustomerFont上单击右键，选择“添加”——“现有项”，在此将您要使用的字体文件(Disko.ttf)添加到项目中； 
字体的英文名称（可以在项目中双击引入的字体文件，可以看到这样的内容-见图）为Disko。
![](https://images.cnblogs.com/cnblogs_com/wsdj-ittech/SLUserFont/4.png)
   2 在添加好的字体文件上右键单击，选择“属性”； 
**在此，我们有两种选择方式：**
**方式一: “复制到输出目录”属性值改为“如果较新则复制”，        “生成操作”属性值改为“内容”； **
![](https://images.cnblogs.com/cnblogs_com/wsdj-ittech/SLUserFont/2.png)
引用格式:   /字体文件名#字体英文名 (注意：含反斜杠)
 本例:
```
<TextBlock x:Name="txtBlockUserFont" 
                  Text="Show User Font" 
                  TextAlignment="Center"
                  FontSize="56" 
                  Margin="80"
                  FontFamily="/Disko.ttf#Disko"
                  Foreground="Blue">           
       </TextBlock>
```
**方式二 “复制到输出目录”属性值改为“不复制”，         “生成操作”属性值改为“Resource”； **
![](https://images.cnblogs.com/cnblogs_com/wsdj-ittech/SLUserFont/3.png)
引用格式:   字体文件名#字体英文名 (注意：不含反斜杠) 
本例:
```
<TextBlock x:Name="txtBlockUserFont" 
                  Text="Show User Font" 
                  TextAlignment="Center"
                  FontSize="56" 
                  Margin="80"
                  FontFamily="Disko.ttf#Disko"
                  Foreground="Blue">           
       </TextBlock>
```
效果如下图:
![](https://images.cnblogs.com/cnblogs_com/wsdj-ittech/SLUserFont/1.png)
[**前往：Silverlight学习笔记清单**](http://www.cnblogs.com/wsdj-ITtech/archive/2009/08/31/1557103.html)
**本文程序在Silverlight2.0和VS2008环境中调试通过。本文参照了部分网络资料，希望能够抛砖引玉，大家共同学习。(转载本文请注明出处)**
