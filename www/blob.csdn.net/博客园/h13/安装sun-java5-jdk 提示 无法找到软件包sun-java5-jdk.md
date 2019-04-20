# 安装sun-java5-jdk 提示"无法找到软件包sun-java5-jdk - h13 - 博客园
摘自：http://hi.baidu.com/embedexpert/blog/item/03d2ef15126b2964ca80c4dc.html   
sun-java5-jdk在ubuntu 9.10的source里已经去掉了，如果安装则提示如下：
root@mike-desktop:/home/mike/ti_android#  sudo apt-get install sun-java5-jdk
正在读取软件包列表...完成
正在分析软件包的依赖关系树      
正在读取状态信息...完成E:无法找到软件包sun-java5-jdk
[成都明诚科技](http://www.embedexpert.com/)的解决办法是：
1、sudo gedit /etc/apt/sources.list
加入如下源。
deb [http://cz.archive.Ubuntu.com/ubuntu](http://cz.archive.ubuntu.com/ubuntu) hardy-updates main multiverse
2、并执行：sudo apt-get update 更新源后再执行上述指命令既可成功下载。
3 、为了使你的系统使用Java 5，你应该做一定的选择配置
$ sudo update-alternatives --config java
4、 选择使用java-1.5.0-sun
