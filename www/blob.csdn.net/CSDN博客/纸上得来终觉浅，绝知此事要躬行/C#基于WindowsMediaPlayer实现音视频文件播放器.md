# C#基于WindowsMediaPlayer实现音视频文件播放器 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年12月15日 17:33:36[boonya](https://me.csdn.net/boonya)阅读数：5673标签：[windows																[media																[Player](https://so.csdn.net/so/search/s.do?q=Player&t=blog)
个人分类：[音频视频开发																[C#](https://blog.csdn.net/boonya/article/category/1776321)](https://blog.csdn.net/boonya/article/category/6325036)





原文地址：[http://www.c-sharpcorner.com/uploadfile/e628d9/playing-audio-and-video-files-using-C-Sharp/](http://www.c-sharpcorner.com/uploadfile/e628d9/playing-audio-and-video-files-using-C-Sharp/)

## 目的

开发一个使用c＃.net播放音频和视频文件的Windows应用程序。

## 设计

![](https://img-blog.csdn.net/20171215172539820)





使用OpenFileDialog控件，一个按钮和“Windows Media Player”控件（COM组件）设计上述窗体。


请注意，OpenFileDialog控件出现在表单下方（不在窗体上），在我们的应用程序中用于浏览音频/视频文件。


将“Windows Media Player”控件（COM组件）添加到工具箱中的步骤：


默认情况下，工具箱中不提供“Windows Media Player”控件，如果需要，我们必须将其添加到工具箱中。


为了将“Windows Media Player”控件添加到工具箱中


右键单击工具箱中的“常规”选项卡 - >选择“选择项目...” - >选择“COM组件”选项卡 - >选择“Windows Media Player” - >单击“确定”按钮。


![](https://img-blog.csdn.net/20171215172736877)


![](https://img-blog.csdn.net/20171215172745822)








“Windows Media Player”控件将出现在工具箱中。


![](https://img-blog.csdn.net/20171215172826846)





现在，将“Windows Media Player”控件拖放到窗体上，并在文本上放置一个按钮，其中文本显示为“浏览”，如设计中所示。


## 代码





using System;

using System.Windows.Forms;




namespace mymediaplayer

{

publicpartialclassForm1 : Form

{

public Form1()

{

InitializeComponent();

}

privatevoid btnBrowse_Click(object sender, EventArgs e)

{

openFileDialog1.Filter = "(mp3,wav,mp4,mov,wmv,mpg)|*.mp3;*.wav;*.mp4;*.mov;*.wmv;*.mpg|all files|*.*";

if(openFileDialog1.ShowDialog()==DialogResult.OK)

axWindowsMediaPlayer1.URL = openFileDialog1.FileName;

}

}

}



## 输出




![](https://img-blog.csdn.net/20171215172942040)





注：原网址下载的zip需要注册，比较麻烦，实现起来很简单，已在[Github](https://github.com/BoonyaCSharp-ASP/WindowsMediaPlayer)上提交该项目，支持全屏播放。](https://so.csdn.net/so/search/s.do?q=media&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)




