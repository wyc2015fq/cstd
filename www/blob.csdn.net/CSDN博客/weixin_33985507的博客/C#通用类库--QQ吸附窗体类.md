# C#通用类库--QQ吸附窗体类 - weixin_33985507的博客 - CSDN博客
2010年12月04日 16:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
不用多说，贴出代码就能看懂！
```
1 //类名:EcanQQ
 2 //作用:QQ吸附窗体
 3 //作者：刘典武
 4 //时间：2010-12-01
 5 //用法：添加timer控件，enable设置为true,实例化类EcanQQ qqfrm = new EcanQQ();timer1_Tick时间调用qqfrm.hide_show(this, ref height, timer1);     
 6 
 7 using System;
 8 using System.Collections.Generic;
 9 using System.Text;
10 using System.Windows.Forms;
11 
12 namespace Ecan
13 {
14     public class EcanQQ
15     {
16         /// <summary>
17         /// QQ吸附窗体
18         /// </summary>
19         /// <param name="frm">要吸附边缘的窗体</param>
20         /// <param name="frmHeight">窗体的高度</param>
21         /// <param name="timer">定时器控件</param>
22         //用法：在对应窗体timer控件的Tick事件中写代码 int height = this.Height; EcanQQ.hide_show(this, ref height, timer1);
23         
24         public void hide_show(Form frm, ref int frmHeight, Timer timer)
25         {
26             if (frm.WindowState != FormWindowState.Minimized)
27             {
28                 timer.Interval = 100;                
29                 if (Cursor.Position.X > frm.Left - 1 && Cursor.Position.X < frm.Right && Cursor.Position.Y > frm.Top - 1 && Cursor.Position.Y < frm.Bottom)
30                 {
31                     if (frm.Top <= 0 && frm.Left > 5 && frm.Left < Screen.PrimaryScreen.WorkingArea.Width - frm.Width)
32                     {
33                         frm.Top = 0;
34                     }
35                     else if (frm.Left <= 0)
36                     {
37                         frm.Left = 0;
38                     }
39                     else if (frm.Left + frm.Width > Screen.PrimaryScreen.WorkingArea.Width)
40                     {
41                         frm.Left = Screen.PrimaryScreen.WorkingArea.Width - frm.Width;
42                     }
43                     else
44                     {
45                         if (frmHeight > 0)
46                         {
47                             frm.Height = frmHeight;
48                             frmHeight = 0;
49                         }
50                     }
51                 }
52                 else
53                 {
54                     if (frmHeight < 1)
55                     {
56                         frmHeight = frm.Height;
57                     }
58                     if (frm.Top <= 4 && frm.Left > 5 && frm.Left < Screen.PrimaryScreen.WorkingArea.Width - frm.Width)
59                     {
60                         frm.Top = 3 - frm.Height;
61                         if (frm.Left <= 4)
62                         {
63                             frm.Left = -5;
64                         }
65                         else if (frm.Left + frm.Width >= Screen.PrimaryScreen.WorkingArea.Width - 4)
66                         {
67                             frm.Left = Screen.PrimaryScreen.WorkingArea.Width - frm.Width + 5;
68                         }
69                     }
70                     else if (frm.Left <= 4)
71                     {
72                         frm.Left = 3 - frm.Width;
73                     }
74                     else if (frm.Left + frm.Width >= Screen.PrimaryScreen.WorkingArea.Width - 4)
75                     {
76                         frm.Left = Screen.PrimaryScreen.WorkingArea.Width - 3;
77                     }
78                 }
79             }
80         }
81     }
82 }
83
```
效果：窗体移动到屏幕边缘会自动伸缩，鼠标移上去的话就显示出来！
一个C#资源分享平台，专业分享学习高质量代码，每周期布置学习任务，激发学习C#兴趣！(QQ群：128874886)
