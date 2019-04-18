# rmmod出现的can't unload 'xxx': Device or resource busy错误 - TaigaComplex求职中 - 博客园







# [rmmod出现的can't unload 'xxx': Device or resource busy错误](https://www.cnblogs.com/TaigaCon/archive/2012/11/06/2757578.html)





今天发现了个问题

写驱动的时候写错了个驱动出口（module_exit给拼错了），结果导致加载驱动成功，确无法卸载:

　　rmmod的时候显示can't unload 'xxx': Device or resource busy

然后用lsmod查看了下，发现驱动多了个属性：

　　xxx 1816 0 [permanent], Live 0xbf00c000



 另外贴一下关于Linux的bus_device_driver（总线设备驱动）机制的相关文章：



[http://blog.chinaunix.net/uid-25014876-id-109733.html](http://blog.chinaunix.net/uid-25014876-id-109733.html)

[http://blog.chinaunix.net/uid-25014876-id-110295.html](http://blog.chinaunix.net/uid-25014876-id-110295.html)

[http://blog.chinaunix.net/uid-25014876-id-110738.html](http://blog.chinaunix.net/uid-25014876-id-110738.html)
















