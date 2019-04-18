# Skywind Inside » Windows 10 – 1803 更新又给我出幺蛾子
## Windows 10 – 1803 更新又给我出幺蛾子
May 2nd, 2018[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
Windows 10 的 1803 版本更新后，Cygwin 的 Open SSH 服务无法始用，调查半天，Windows 10 新建了个 sshd 的服务，原来的 Cygwin Open SSH 服务被删除了，也安装不上去，解决方法是 ssh-host-config 后面加个参数 `--name sshd2` 换个新名字重新安装服务。
