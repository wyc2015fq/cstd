# 在Windows下用Virtualbox虚拟linux时共享文件夹设置的方法 - weixin_33985507的博客 - CSDN博客
2013年03月16日 14:17:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
说明一下我的环境：
[主机操作系统](http://wenwen.soso.com/z/Search.e?sp=S%E4%B8%BB%E6%9C%BA%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F&ch=w.search.yjjlink&cid=w.search.yjjlink)是Windows7，[虚拟机](http://wenwen.soso.com/z/Search.e?sp=S%E8%99%9A%E6%8B%9F%E6%9C%BA&ch=w.search.yjjlink&cid=w.search.yjjlink)是Ubuntu 9.04。安装好Ubuntu 9.04后，点击设备菜单，如图所示。先点击安装增强功能包。
然后在命令行里运行：
cd /cdromsudo ./VboxLinuxAdditions.run
安装完毕后重启。
重启完成后依然点击设备菜单，选择共享文件夹。然后添加共享文件夹。注意注意，下面便是要注意的问题所在了。
一定要自己手动填写一个共享文件夹的名称，千万不要用默认的名称！比如，你选择的文件夹名字是sharedfolder，其默认的共享文件夹名称也是这个，这时候一定要改掉成别的名字，否则在Ubuntu里不能使用。
接下来在Ubuntu系统里运行类似的命令，替换成你的共享文件夹名称和[挂载点](http://wenwen.soso.com/z/Search.e?sp=S%E6%8C%82%E8%BD%BD%E7%82%B9&ch=w.search.yjjlink&cid=w.search.yjjlink)即可。
sudo mount -t vboxsf Your_folder_name Your_mount_point
OK，现在ubuntu和主机可以互传文件了。
