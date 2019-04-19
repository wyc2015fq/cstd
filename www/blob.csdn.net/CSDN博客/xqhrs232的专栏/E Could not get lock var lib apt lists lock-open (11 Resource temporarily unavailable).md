# E:Could not get lock /var/lib/apt/lists/lock - open (11: Resource temporarily unavailable) - xqhrs232的专栏 - CSDN博客
2012年04月26日 14:36:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：615标签：[ubuntu																[kill																[list																[终端																[工作](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=kill&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/zyxlinux888/article/details/6358615](http://blog.csdn.net/zyxlinux888/article/details/6358615)
出现这个问题的原因可能是有另外一个程序正在运行，导致资源被锁不可用。而导致资源被锁的原因，可能是上次安装时没正常完成，而导致出现此状况。
解决方法：输入以下命令
sudo rm /var/cache/apt/archives/lock
sudo rm /var/lib/dpkg/lock
之后再安装想装的包，即可解决

今天玩ubuntu的时候，在弄更新源的时候，突然出现以下错误：
[1]+ Stopped                 sudo apt-get update
haiquan@haiquan-desktop:~$ sudo apt-get update
E: Could not get lock /var/lib/apt/lists/lock - open (11: Resource temporarily unavailable)
E: Unable to lock the list directory
开始以为是权限不够，就是用 sudo apt-get update,发现还是报错，问题没有解决。于是上网搜索了一下，答案如下：
问题应该是之前那个更新被强制取消的问题，进程仍然还在。用这个命令查看一下：
ps -e | grep apt
显示结果如下：
6362 ? 00:00:00 apt
6934 ? 00:00:00 apt-get
7368 ? 00:00:00 synaptic
然后就执行
sudo killall apt
sudo killall apt-get
sudo killall synaptic
再次在终端里查看ps -e | grep apt 没有任何结果了
继续执行sudo apt-get update
OK！

原因：刚装好的Ubantu系统，内部缺少很多软件源，这时，系统会自动启动软件源更新进程“apt-get”，并且它会一直存活。由于它在运行时，会占用软件源更新时的系统锁（以下称“系统更新锁”，此锁文件在“/var/lib/apt/lists/”目录下），而当有新的apt-get进程生成时，就会因为得不到系统更新锁而出现"E: 无法获得锁 /var/lib/apt/lists/lock - open (11: Resource temporarily unavailable)"错误提示！因此，我们只要将原先的apt-get进程杀死，从新激活新的apt-get进程，就可以让新立德软件管理器正常工作了！
这时，你可以按一下步骤来解决：
1、输入以下命令：“ps -aux  > temp.txt”.敲回车确定。这时会有“warning”出现，但不予理会。
2、输入以下命令：“grep -n apt-get temp.txt”键入回车确定。而后从命令输出中找到 apt-get 进程的PID。
3、输入以下命令：“sudo kill <PID>”。
4、完成。
or
如果提示错误:E: Could not get lock /var/lib/dpkg/lock - open (11 Resource temporarily unavailable) 
输入
sudo rm /var/lib/apt/lists/lock即可
祝好运！
