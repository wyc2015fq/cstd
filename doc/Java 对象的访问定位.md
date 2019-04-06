# Java 对象的访问定位

 

建立对象是为了使用对象，Java程序通过栈上的reference数据来操作堆上的具体对象。

目前主流的访问方式有使用句柄和直接指针两种。

# 使用句柄访问

如果使用句柄访问方式，Java堆中将会划分出一块内存来作为句柄池，reference中存储的就是对象的句柄地址，而句柄中包含了对象实例数据和类型数据各自的具体地址信息如下图所示：

![这里写图片描述](https://img-blog.csdn.net/20180213093052762?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzU5NTQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 使用直接指针访问

如果使用直接指针访问方式，java堆对象的布局中就必须考虑如何放置访问类型数据的相关信息，reference中直接存储的就是对象地址。如下图所示：

![这里写图片描述](https://img-blog.csdn.net/20180213093108611?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzU5NTQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 后记

这两种对象的访问方式各有优势。

使用句柄访问方式的最大好处就是reference中存储的是稳定的句柄地址，在对象被移动时只会改变句柄中的实例数据指针，而reference本身不需要被修改。

使用直接指针访问方式的最大好处就是速度更快，它节省了一次指针定位的的时间开销。