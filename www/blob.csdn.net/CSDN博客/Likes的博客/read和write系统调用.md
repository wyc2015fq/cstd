# read和write系统调用 - Likes的博客 - CSDN博客
2019年03月18日 11:38:50[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：21标签：[read																[write																[内部原理](https://so.csdn.net/so/search/s.do?q=内部原理&t=blog)](https://so.csdn.net/so/search/s.do?q=write&t=blog)](https://so.csdn.net/so/search/s.do?q=read&t=blog)
个人分类：[操作系统](https://blog.csdn.net/songchuwang1868/article/category/8768253)
read和write系统调用的使用是简单的，但实际上其封装了及其复杂的操作。
因为Linux视一切的设备为文件，读写设备也被抽象成了读写文件。所以read和write系统调用下封装了复杂的对各种设备的读写操作。以read为例大体原理如下：
read系统调用中，会根据用户传入的fd进行查表，发现相应的fd对应的到底是什么设备，再进入相应设备专有的read逻辑。比如fd是open file返回的磁盘文件的fd，read系统调用在查表会选用相应的读磁盘的read进行操作。
