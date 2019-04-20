# ubuntu常见错误--could not get lock /var/lib/dpkg/lock -open - h13 - 博客园
最近研究ubuntu，用apt-get命令安装一些软件包时，总报错：E：could not get lock /var/lib/dpkg/lock -open等
出现这个问题的原因可能是有另外一个程序正在运行，导致资源被锁不可用。而导致资源被锁的原因，可能是上次安装时没正常完成，而导致出现此状况。
解决方法：输入以下命令
sudo rm /var/cache/apt/archives/lock
sudo rm /var/lib/dpkg/lock
之后再安装想装的包，即可解决。
