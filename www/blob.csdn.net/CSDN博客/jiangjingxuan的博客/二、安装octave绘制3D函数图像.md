# 二、安装octave绘制3D函数图像 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 09:46:15[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：2281










![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_101_default_big.png)



octave是类似于matlab的软件，matlab的缺点是贵，octave作为免费的开源项目，能够绘制三维图形，是用来做大数据统计分析的绝佳工具，也是AndrewNG在机器学习课程中强烈推荐的工具



请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 安装方式

### ***mac系统***

安装教程在：

[http://wiki.octave.org/Octave_for_MacOS_X#Binary_installer_for_OSX_10.9.1](http://wiki.octave.org/Octave_for_MacOS_X#Binary_installer_for_OSX_10.9.1)

在[https://www.macports.org/install.php](https://www.macports.org/install.php)下载对应osx版本的安装包，下载安装

port会把下载的包安装到/opt/local/var/macports/

如果没装xcode命令行工具，需要安装：
`xcode-select --install`
然后执行

```bash
sudo port install atlas +gcc5
sudo port install arpack -accelerate+atlas
sudo port install texlive-bin
sudo port install texlive-basic
sudo port install texlive-latex
sudo port install octave +qtgui+gui
```

### ***centos7系统***

首先必须保证以图形界面启动，如果没有安装图形界面则先安装：

```bash
sudo  yum groupinstall "GNOME Desktop" "Graphical Administration Tools"
sudo ln -sf /lib/systemd/system/runlevel5.target /etc/systemd/system/default.target
```

重启后就会以图形界面形式启动

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

然后安装octave

```
yum install epel-release
yum install octave
```



## 使用样例

命令行执行octave打开octave终端，输入如下语句：

```
n = 50;
x = y = linspace (-8, 8, n)';
[xx, yy] = meshgrid (x, y);
r = sqrt (xx .^ 2 + yy .^ 2) + eps;
c = 5 * sin (r) ./ r;
h= surf(xx,yy,c,c);
shading interp
```

会自动弹出图像效果如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/7b84b428f5c9f8e0bbe86481731970a071867f9a.png)

是不是很漂亮的说！

## 解释一下

```
n = 50;
x = y = linspace (-8, 8, n)';
```

这两句是说把x和y都赋值成从-8到8之间划分成50等份的一个个点
`[xx, yy] = meshgrid (x, y);`
这句是说把[xx,yy]赋值成由x和y构成的网格点

```
r = sqrt (xx .^ 2 + yy .^ 2) + eps;
c = 5 * sin (r) ./ r;
```

这两句实现了一个公式来计算c值
`h= surf(xx,yy,c,c);`
这句就是在画图像，surface表示把图像表面展示出来
`shading interp`
这句就是做了一个图像的平滑处理





