# 使用python获取电脑的磁盘信息 - 小灰笔记 - CSDN博客





2017年04月13日 00:05:35[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：6157








使用Python获取电脑的磁盘信息需要借助于第三方的模块psutil，这个模块需要自己安装，纯粹的CPython下面不具备这个功能。

在iPython交互界面中进行如下演示：

查看电脑的磁盘分区：

In [**1**]:
**import****psutil**




In [**2**]:
psutil.disk_partitions()

Out[**2**]:
[sdiskpart(device='/dev/disk2', mountpoint='/', fstype='hfs', opts='rw,local,rootfs,dovolfs,journaled,multilabel')]




In [**3**]: len(psutil.disk_partitions())

Out[**3**]:
1

从上面的结果看出，电脑只有一个分区。针对最后的结果专门通过判断字典的元素个数来确认了一下。

查看电脑的磁盘使用百分比：




In [**4**]:
psutil.disk_usage('/')

Out[**4**]:
sdiskusage(total=1114478608384, used=305497878528, free=808718585856, percent=27.4)




相对来说还好，新电脑使用时间还不是很长，硬盘暂时还没有塞满。总共用了大约27.4%。




查看电脑磁盘的IO计数：




In [**5**]:
psutil.disk_io_counters()

Out[**5**]:
sdiskio(read_count=112237L, write_count=99750L, read_bytes=5243863040L, write_bytes=7047483392L, read_time=80568L, write_time=138699L)







In [**7**]:
psutil.disk_io_counters(perdisk=True)

Out[**7**]: 

{'disk0': sdiskio(read_count=103533L, write_count=86260L, read_bytes=5120090624L, write_bytes=4813373440L, read_time=29774L, write_time=27654L),

 'disk1': sdiskio(read_count=8740L, write_count=13723L, read_bytes=124141056L, write_bytes=2237206528L, read_time=50840L, write_time=111871L)}




上面分别使用了两种方式，第一种方式是获取磁盘总的IO信息，第二种是查看电脑分区的磁盘信息。从结果上来看，信息的统计不是按照逻辑分区来统计的，而是按照物理磁盘来统计的。我的电脑上正好是有两块磁盘，一块机械硬盘还有一块固态硬盘，其实这个按照物理磁盘进行统计的说法是根据这个推断出来的。



