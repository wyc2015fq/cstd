
# Ubuntu12.04创建 eclipse launcher - 阳光岛主 - CSDN博客

2013年09月08日 23:07:20[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：5595个人分类：[Java/JSP																](https://blog.csdn.net/sunboy_2050/article/category/730435)



Ubuntu 12.04 默认无法launcher Eclipse快捷图标到左侧Dash，需要手工配置，步骤如下：
1） 首先，创建并打开 ~/.local/share/applications/opt_eclipse.desktop（如果没有applications文件夹或opt_eclipse.desktop文件，请自行创建）
`gedit  ~/.local/share/applications/opt_eclipse.desktop`
2）接着，复制下面内容到 opt_eclipse.desktop ， 并根据自己eclipse路径，修改**.....**中间的配置路径
`[Desktop Entry]
Type=Application
Name=Eclipse
Comment=Eclipse Integrated Development Environment
Icon=``** something like /opt/eclipse/icon.xpm **``Exec=``** something like /opt/eclipse/eclipse **``Terminal=false
Categories=Development;IDE;Java;
StartupWMClass=Eclipse`如下我的配置图：
![](https://img-blog.csdn.net/20130819181937171)

3） 保存后，修改opt_eclipse.desktop文件访问权限，有以下两种方法：
方法1：  chmod +x ~/.local/share/applications/opt_eclipse.desktop
方法2： 进入~/.local/share/applications/目录，右键 opt_eclipse.desktop 文件，依次选择Properties -> Permissions -> Allow executing file as program（勾选上）
![](https://img-blog.csdn.net/20130820101016671)

4） 勾选上后，opt_eclipse.desktop 立即变为eclipse的图标，拖至左侧dash launcher即可，结果如下：
![](https://img-blog.csdn.net/20130820101531421)



**参考推荐：**
[How to pin Eclipse to the Unity launcher?](http://askubuntu.com/questions/80013/how-to-pin-eclipse-to-the-unity-launcher)



