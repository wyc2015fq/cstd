# ubuntu安装rpm包，deb包等各种包 - xcw0754 - 博客园
# [ubuntu安装rpm包，deb包等各种包](https://www.cnblogs.com/xcw0754/p/5245253.html)
### ubuntu下的各种包的安装方法
Ubuntu麒麟自带的包管理工具有apt-get，但是里面的软件包一般年代比较久远，源更新很慢，支持也很少。如果想安装一些比较新的包，可以尝试到PPA上去找找看，找到之后只需要使用`add-apt-repository`命令将找到的源添加到本机，然后能使用apt-get安装了。
不过本文不关心如何使用ppa添加源，而是**下载完软件包后如何安装**。首先，我们需要一个工具叫`alien`，先用`apt-get` install它吧。然后按照下面择所需。
- 
deb包的安装（二选一）
`alien -i filename.deb`
`dpkg -i filename.deb`
- rpm包的安装：
`alien -i -c filename.rpm `- rpm转换成deb
`alien -d -c filename.rpm #会生成一个filename.deb包`- tar.gz包
`alien -d -c filename.tar.gz  #生成一个filename.deb `- tar.bz2包
```
#将软件包转换为tar.gz,再转成deb，命令熟练后可以一步到位，下面是分解操作 
    tar -jxvf   filename.tar.bz2     #解压 
    tar -zcvf   filename    # 生成filename.tar.gz包
    alien -d -c filename.tar.gz  #转换为deb包
```

