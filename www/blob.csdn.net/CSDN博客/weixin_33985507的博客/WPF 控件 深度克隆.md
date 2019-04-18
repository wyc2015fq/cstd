# WPF 控件 深度克隆 - weixin_33985507的博客 - CSDN博客
2018年05月03日 15:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
原文:[WPF 控件 深度克隆](http://www.cnblogs.com/lanymy/archive/2012/11/12/2766029.html)
[http://social.msdn.microsoft.com/Forums/zh-SG/wpfzhchs/thread/e5c87129-966a-4d51-a934-ccb8fc7620ec](http://social.msdn.microsoft.com/Forums/zh-SG/wpfzhchs/thread/e5c87129-966a-4d51-a934-ccb8fc7620ec)
比如 canvas里面有一个RichTextBox1 ，我现在要New一个新RichTextBox2 ，将RichTextBox1 的内容事件复制到RichTextBox2，俩个可以共存在canvas;有没有简单点类似Winform的克隆呢！
string xaml =System.Windows.Markup.XamlWriter.Save(rtb1);
RichTextBox rtb2 =System.Windows.Markup.XamlReader.Parse(xaml)asRichTextBox;
