# Linux下找不到so文件的解决办法 - =朝晖= - 博客园
# [Linux下找不到so文件的解决办法](https://www.cnblogs.com/dhcn/p/9879065.html)
**http://www.cnblogs.com/xudong-bupt/p/3698294.html**
**如果使用自己手动生成的动态链接库.so文件，但是这个.so文件，没有加入库文件搜索路劲中，程序运行时可能会出现找不到动态链接库的情形。**
可以通过ldd命名来查看可执行文件依赖的动态链接库，如下(其中D为可执行程序)：
![](https://images0.cnblogs.com/i/466768/201404/282356195025963.png)
 其中的libjson_linux-gcc-4.6_libmt.so cannot found。
解决这个问题：
 (1)在系统中查找这个文件(**当然要保证系统中已经有这个.so文件，只是查找路径没有设置正确而已**)：
**sudo find / -name libjson_linux-gcc-4.6_libmt.so**
结果：/home/liu/Desktop/jsoncpp-src-0.5.0/libs/linux-gcc-4.6/libjson_linux-gcc-4.6_libmt.so
(2)将.so文件路径的目录添加到`/etc/ld.so.conf`
**sudo` vim /etc/ld.so.conf`**
文件末尾新添加一行，/home/liu/Desktop/jsoncpp-src-0.5.0/libs/linux-gcc-4.6
(3)使得修改生效
**sudo /sbin/ldconfig**
这样就不会有那个找不对.so文件的错误啦。
参考：http://www.blogjava.net/zhyiwww/archive/2007/12/14/167827.html

