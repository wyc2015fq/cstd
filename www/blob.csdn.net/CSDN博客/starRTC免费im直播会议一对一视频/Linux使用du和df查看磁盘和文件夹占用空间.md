# Linux使用du和df查看磁盘和文件夹占用空间 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年11月17日 16:30:06[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：278
## df
df可以查看一级文件夹大小、使用比例、档案系统及其挂入点，但对文件却无能为力。
df -lh
参数 -h 表示使用「Human-readable」输出，也就是使用 GB、MB 等易读的格式。
$ **df**-hFilesystem            Size  Used Avail Use**%** Mounted on**/**dev**/**sda1             3.9G  300M  3.4G   8**%****/****/**dev**/**sda7             100G  188M   95G   1**%****/**data0**/**dev**/**sdb1             133G   80G   47G  64**%****/**data1**/**dev**/**sda6             7.8G  218M  7.2G   3**%****/**var**/**dev**/**sda5             7.8G  166M  7.2G   3**%****/**tmp**/**dev**/**sda3             9.7G  2.5G  6.8G  27**%****/**usr
tmpfs                 2.0G     0  2.0G   0**%****/**dev**/**shm
可以看到 /dev/sda1 这个分割区被挂在根目录下。
## [[编辑](http://192.168.1.100/wiki_elesos_com/index.php?title=Linux%E4%BD%BF%E7%94%A8du%E5%92%8Cdf%E6%9F%A5%E7%9C%8B%E7%A3%81%E7%9B%98%E5%92%8C%E6%96%87%E4%BB%B6%E5%A4%B9%E5%8D%A0%E7%94%A8%E7%A9%BA%E9%97%B4&action=edit&section=2)]du
du可以查看文件或文件夹的大小。
查看指定目录的大小
# du -sh <目录名>             //s表示计算总量
如果当前目录下文件和文件夹很多，使用不带参数du的命令，可以循环列出所有文件和文件夹所使用的空间。但这对查看哪个目录过大是不利的，所以得指定深入目录的层数，使用参数：--max-depth=，这是个极为有用的参数！
使用“*”，还可以得到文件的空间使用大小.
如果要排序再在后面加上 | sort -n
du -h --max-depth=1du -h --max-depth=1 work/testingdu -h --max-depth=1 work/testing/*
## [[编辑](http://192.168.1.100/wiki_elesos_com/index.php?title=Linux%E4%BD%BF%E7%94%A8du%E5%92%8Cdf%E6%9F%A5%E7%9C%8B%E7%A3%81%E7%9B%98%E5%92%8C%E6%96%87%E4%BB%B6%E5%A4%B9%E5%8D%A0%E7%94%A8%E7%A9%BA%E9%97%B4&action=edit&section=3)]艺搜参考
[http://www.yayu.org/look.php?id=162](http://www.yayu.org/look.php?id=162)
[http://baike.baidu.com/view/43913.htm](http://baike.baidu.com/view/43913.htm)
[http://dadoneo.iteye.com/blog/984963](http://dadoneo.iteye.com/blog/984963)
[http://blog.csdn.net/iamlaosong/article/details/7085178](http://blog.csdn.net/iamlaosong/article/details/7085178)
[http://yjw1983.blog.51cto.com/265702/410453](http://yjw1983.blog.51cto.com/265702/410453)
