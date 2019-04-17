# Ubuntu Octave命令窗口乱码解决方案 Ubuntu安装最新版Octave - DumpDoctorWang的博客 - CSDN博客





2018年07月21日 14:36:32[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：421








# 一、安装Octave 4.2.2

Ubuntu16.04

```bash
sudo apt-add-repository ppa:octave/stable
sudo apt-get update
sudo apt-get install octave
```

参考链接：[Ubuntu 16.04 安装 带图形界面(GUI) Octave 4.2.1](https://blog.csdn.net/mdzzname/article/details/79582677)

Ubuntu 18.04

```bash
sudo apt-get install octave
```

# 二、Octave设置中文

在菜单栏依次点击”Edit“-->"Preferences..."，然后进入如下界面

![](https://img-blog.csdn.net/20180721140843585?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 在”Language(require restart)“后面那个下拉选项里面选择”zh_CN“，重启软件之后就是中文了。

# 三、Octave命令窗口乱码

在菜单栏点击”Edit(编辑)“-->"Preferences...(首选项...)"，进入上图界面，选择”Terminal(终端)“，进入如下界面

![](https://img-blog.csdn.net/20180721143318943?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在”Font(字体)“ 后面的下拉菜单里选择"DejaVu Sans Mono"，点击”Ok“，命令窗口乱码问题即可解决。

# 四、Octave编辑器乱码

在菜单栏点击”Edit(编辑)“-->"Preferences...(首选项...)"，进入上图界面，选择”Editor Style(编辑器风格)“，进入如下界面

![](https://img-blog.csdn.net/20180722171249842?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在切换面板中选择“Octave”，然后在“Default”（第一行）后面的下拉菜单里选择“DejaVu Sans Mono“，后面的12是字体大小。然后把”C++“、”Perl“、“Batch”、“Diff“、”Bash“、”Text“全设置一遍。这样，乱码问题基本解决。



