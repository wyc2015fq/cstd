# 安装ubuntu和win10   升级修复引导，_用户5706034127_新浪博客
|||
			个人认为最完美的安装是，先安装win10,在安装ubuntu，
这样的引导也是完美的。。
可以有时候需要重新安装或升级win，这个时候，引导被破坏，如何回复
首先win下用easybcd，添加新条目里面，，添加linux启动的条目。
一定要注意的是选择grub2,的自动选择，而不是gurb的什么鬼东西。
但这这样的启动，和原来的linux的grub是不一样的。
接着你进入linux后。使用一下步骤来回复到之前的linux的grub启动模式
命令1,重新安装grub
sudo grub-install --root-directory=/ 
/dev/sda
第二个命令是
sudo update-grub
详情参考百度文库
https://wenku.baidu.com/view/5629995b6c85ec3a87c2c56b.html
因为我们直接进入了ubuntu系统，所以第一个命令没有使用/mnt，
而是选择了我们当前系统的根目录。
这样你每次开机的引导就是原来的grub了，
最后一个步，你发现进入win，时，easybcd的引导还在。
你进入win后，删除，你最开始添加的linux引导项条目，即可。
