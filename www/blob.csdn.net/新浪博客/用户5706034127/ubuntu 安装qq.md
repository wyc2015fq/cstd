# ubuntu 安装qq_用户5706034127_新浪博客
|||
ubantu 安装qq，
qq下载地址：
http://www.ubuntukylin.com/application/show.php?lang=cn&id=279
百度经验：
http://jingyan.baidu.com/article/e6c8503c6f5fa5e54f1a18de.html
另外需要关闭设备锁，因为国际版本太低了
有时候不能输入中文，参考下面两个：
wineqq的运行脚本是/usr/share/deepinwine/qqintl/wine-qqintl
编辑此脚本，在最开始加入：
export XMODIFIERS="@im=fcitx"
export GTK_IM_MODULE="fcitx"
export QT_IM_MODULE="fcitx"
保存，重新启动QQ，即可以输入中文了。
最近使用链接[Ubuntu14.04安装wineqq国际版](http://blog.csdn.net/beyond_ray/article/details/38966251)成功安装qq，但发现无法输入中文，英文正常。最后各种google+百度找到问题：我的ubuntu14.04的语言环境是English，而wine-qqintl是默认中文环境，so我们只需要该一行代码就ok：
- 执行命令：sudo vim
/usr/bin/wine-qqintl
- 将”export
LANG=zh_CN.utf8”改为”export LANG=en_US.utf8”即可。
/usr/bin下面的只是一个链接，连接到/usr/share/deepinwine/qqintl/wine-qqintl
