# 如何在Linux系统上安装字体 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年08月09日 10:30:28[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：158
libreoffice添加字体
TrueType字体文件的扩展名是.ttf，ttf就是TrueType Font的首字母缩写
一般在 /usr/share/fonts/truetype/ 目录下，这个是所有用户都能用
输入下面的命令刷新系统字体信息文件。
fc-cache -f -v
如果你只想提供给当前登录的用户使用，那么我们只要把字体文件复制到~/.fonts目录下。如果没有~/.fonts这个目录，那么我们先创建这个目录
同样也需要
fc-cache -f -v
Windows系统的字体安装目录是C:\Windows\Fonts，这个目录下的字体文件分成3个扩展名，一个是.fon扩展名，是DOS系统的字体；一个是.ttf扩展名，这个是西文字体，还有一个是.TTF扩展名，是中文字体。DOS系统的字体我们用不上。
mkdir /usr/share/fonts/truetype/WindowsFonts
cp -r /mnt/Windows/Fonts/*.ttf /usr/share/fonts/truetype/WindowsFonts/
cp -r /mnt/Windows/Fonts/*.TTF /usr/share/fonts/truetype/WindowsFonts/
 TTC类型字体一般是几个TTF合成的字库，安装后字体列表中会看到两个以上的字体。
查看字体
fc-list :lang=zh
以上命令是查看中文字体的命令
