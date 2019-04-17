# nginx开发（一） 源码－编译 - DoubleLi - 博客园






1:获取源码

[http://nginx.org/download/nginx-1.8.0.tar.gz](http://nginx.org/download/nginx-1.8.0.tar.gz)



2:编译

解压之后，进入根目录，执行

./configuer.sh

make

make install



3:nginx执行

编译完之后，可以进入 /usr/local/nginx，进入sbin目录。



执行 ./nginx -v，就会看到   nginx version: nginx/1.8.0  。



补充：如果提示需要安装pcre，可参考如下方法：

从官网下载链接：http://www.pcre.org/

./configure --prefix=/usr/local --enable-utf8 
make 
sudo make install 



提示缺少open ssl：

http://www.openssl.org/source/

./configure ----prefix=/usr/local

make

sudo make install



MAC编译补充：




今天在mac os 上编译安装Nginx时候，报错：ld: symbol(s) not found for architecture x86_64， 经过一番折腾之后发现，由于Nginx依赖openssl库，查看openssl的./config 文件发现，这个问题应该是 openssl/config脚本猜对你的系统是64位，但是 会根据$KERNEL_BITS来判断是否开启x86_64编译，默认不开启，他会给你5秒时间确认是否停止编译，手动设置x86_64编译，所以默认你生成的openssl库文件是32位的，最后静态链接到nginx会出错。目前看来没有很好的方法把x86_64的参数传到openssl配置文件中 (openssl/config 猜测os[架构](http://lib.csdn.net/base/architecture)，设置编译的参数是32位还是64位，默认是32位，然后调用openssl/Configure生成Makefile)，

解决办法就是：

先运行nginx源码目录下运行
$ ./configure

然后在objs里，打开Makefile，

找到： ./config --prefix=xxx.openssl no-shared        (注释：XXX是已存在的openssl源码路径)

把该段的 ./config 改成 ./Configure darwin64-x86_64-cc 其他后面参数不变，保存

然后再make就编译通过了










