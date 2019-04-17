# Ubuntu下一些软件，环境的安装汇总（持续更新） - 长歌行 - CSDN博客





2012年02月21日 15:08:39[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1049








       最近使用linux系统了，主要目的是迫使自己了解下程序的编译链接过程（window下的ide用得太熟了，没有办法锻炼，索性脱离下这个环境）。然后觉得linux下面系统要干净很多，少了很多不知道的东东。

       由于平时工作需要，linux下面也需要安装一些环境，故汇总如下，免得重装后忘记了。







# 1.windows与ubuntu的双系统

xp和win7略有不同，网上都有比较可行的做法。我按他们说的来做都成功了。我装了两台笔记本。不过记得不要随便格式化磁盘，会导致两个系统都进不去。

Win7


http://hi.baidu.com/tewuapple/blog/item/dfcf3fae412f48d27ed92a68.html

XP      


http://wenku.baidu.com/view/b85973f8aef8941ea76e0565.html




# 2.基本



## 2.1修改用户密码





应用程序->附件->终端

或者ctrl + alt + t


benben@benben-ubuntu:~$  sudo passwd benben
[sudo] password for benben:
123456
输入新的 UNIX 密码：123
重新输入新的 UNIX 密码：123
passwd：已成功更新密码 





## 2.2安装软件方法

http://www.digglife.cn/articles/how-to-install-software-in-ubuntu.html




# 3.OpenGL

    1.安装libglut3-dev包

    apt-get install libglut3-dev

    2.编辑源文件并编译

    假设你的程序的名字叫main.c

    gcc main.c -o main -lglut

    然后再用下面的命令：

    ./main

补充一段代码



```cpp
安装成后，我们就可以开始写我们第一个Linux下的OpenGL程序了。
QUOTE:
1 /* light.c
2 此程序利用GLUT绘制一个OpenGL窗口，并显示一个加以光照的球。
3 */
4 /* 由于头文件glut.h中已经包含了头文件gl.h和glu.h，所以只需要include 此文件*/
5 # include 
6 # include 
7 
8 /* 初始化材料属性、光源属性、光照模型，打开深度缓冲区 */
9 void init ( void )
10 {
11 GLfloat mat_specular [ ] = { 1.0, 1.0, 1.0, 1.0 };
12 GLfloat mat_shininess [ ] = { 50.0 };
13 GLfloat light_position [ ] = { 1.0, 1.0, 1.0, 0.0 };
14 
15 glClearColor ( 0.0, 0.0, 0.0, 0.0 );
16 glShadeModel ( GL_SMOOTH );
17 
18 glMaterialfv ( GL_FRONT, GL_SPECULAR, mat_specular);
19 glMaterialfv ( GL_FRONT, GL_SHININESS, mat_shininess);
20 glLightfv ( GL_LIGHT0, GL_POSITION, light_position);
21 
22 glEnable (GL_LIGHTING);
23 glEnable (GL_LIGHT0);
24 glEnable (GL_DEPTH_TEST);
25 }
26 
27 /*调用GLUT函数，绘制一个球*/
28 void display ( void )
29 {
30 glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
31 glutSolidSphere (1.0, 40, 50);
32 glFlush ();
33 }
34 
35 
36 /* 定义GLUT的reshape函数，w、h分别是当前窗口的宽和高*/
37 void reshape (int w, int h)
38 {
39 glViewport (0, 0, (GLsizei) w, (GLsizei) h);
40 glMatrixMode (GL_PROJECTION);
41 glLoadIdentity ( );
42 if (w <= h)
43 glOrtho (-1.5, 1.5, -1.5 * ( GLfloat ) h / ( GLfloat ) w, 1.5 * ( GLfloat ) h / ( GLfloat ) w, -10.0, 10.0 );
44 else
45 glOrtho (-1.5 * ( GLfloat ) w / ( GLfloat ) h, 1.5 * ( GLfloat ) w / ( GLfloat ) h, -1.5, 1.5, -10.0, 10.0);
46 glMatrixMode ( GL_MODELVIEW );
47 glLoadIdentity ( ) ;
48 }
49 
50 
51 /* 定义对键盘的响应函数 */
52 void keyboard ( unsigned char key, int x, int y)
53 {
54 /*按Esc键退出*/
55 switch (key) 
56 {
57 case 27:
58 exit ( 0 );
59 break;
60 }
61 }
62 
63 
64 int main(int argc, char** argv)
65 {
66 /* GLUT环境初始化*/
67 glutInit (&argc, argv);
68 /* 显示模式初始化 */
69 glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
70 /* 定义窗口大小 */
71 glutInitWindowSize (300, 300);
72 /* 定义窗口位置 */
73 glutInitWindowPosition (100, 100);
74 /* 显示窗口，窗口标题为执行函数名 */
75 glutCreateWindow ( argv [ 0 ] );
76 /* 调用OpenGL初始化函数 */
77 init ( );
78 /* 注册OpenGL绘图函数 */
79 glutDisplayFunc ( display );
80 /* 注册窗口大小改变时的响应函数 */
81 glutReshapeFunc ( reshape );
82 /* 注册键盘响应函数 */
83 glutKeyboardFunc ( keyboard );
84 /* 进入GLUT消息循环，开始执行程序 */
85 glutMainLoop( );
86 return 0;
87 }

编译并运行：

gcc -lglut -o light light.c
./light
```



# 4.IDE

C++ Code::Block

perl   Padre

Eclipse

以上三个都是用的ubuntu软件中心安装的




# 5.影音


用新立得装的smplayer。不过wms解码器没有安装，需要加一个源来更新mplayer <http://ppa.launchpad.net/motumedia/mplayer-daily/ubuntu>




# 6.邮件收发

我使用的是Ubuntu10.04自带的volution，该软件在“应用程序” “办公” “Volution”里面，觉得还不错，不过记得要设置字符集，在首选项里设置默认字符集，邮件字符集为gbkXXX，如果单纯查看邮件，也可以在查看里面最后一个选项，设置字符集编码。




# 7.离线浏览

window下面有webzip，可以下载网站的页面，然后离线浏览。也挺好用的。（貌似有些网站是asp, 还有一些脚本语言动态生成的页面就无能为力了）

在linux下面可以使用wget，具体命令见链接

[WGET命令](http://blog.csdn.net/iuhsihsow/article/details/7291392)


比如 benben@benben-ubuntu:~/桌面/test$ sudo wget -m http://www.csdn.net  


还是能下载一部分的，呵呵





