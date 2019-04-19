# ubuntu 更新安装源  sudo apt-get update失败_用户5706034127_新浪博客
|||
W: Failed to fetch
http://us.archive.ubuntu.com/ubuntu/dists/trusty-updates/main/binary-amd64/Packages
 Hash Sum mismatch
E: Some index files failed to download. They have been
ignored, or old ones used instead.
sudo apt-get update失败
很可能是安装源不好，
两种方法，一种是：
可以将目录下/var/lib/apt/lists/partial/所有的文件清掉，
或者/var/lib/apt/lists/下所有文件都清除
再次运行apt-get
update即可！自带源在大陆不好。
如果还是不行, 那就建议更换镜像源了,
更换完源之后，最好先清理一下上面路径的内容.
出现原因可能有以下几种情况:
1. 该源此次同步不完整(only this
time),国内镜像源基本上每天也要定时从国外主站源同步一次内容,可能因为国际链路问题.
2. 你更换过镜像源，偶尔也会导致这种情况出现.

更新安装源的方法：
http://jingyan.baidu.com/article/7f41ecec1b7a2e593d095ce6.html
