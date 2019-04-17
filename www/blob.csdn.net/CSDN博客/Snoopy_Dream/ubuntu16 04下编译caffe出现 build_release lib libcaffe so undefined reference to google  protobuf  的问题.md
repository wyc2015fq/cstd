# ubuntu16.04下编译caffe出现.build_release/lib/libcaffe.so: undefined reference to google ::protobuf...的问题 - Snoopy_Dream - CSDN博客





2018年03月12日 20:37:24[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：4611










----------------------------------------------------------------------------------------------

为什么会出现undefined reference to 'xxxxx'错误？

首先这是链接错误，不是编译错误，也就是说如果只有这个错误，说明你的程序源码本身没有问题，是你用编译器编译时参数用得不对，你没有指定链接程序要用到得库，比如你的程序里用到了一些数学函数，那么你就要在编译参数里指定程序要链接数学库，方法是在编译命令行里加入-l m。-l参数就是用来指定程序要链接的库，-l参数紧接着就是库名，那么库名跟真正的库文件名有什么关系呢？就拿数学库来说，他的库名是m，他的库文件名是libm.so，很容易看出，把库文件名的头lib和尾.so去掉就是库名了。

上面是后面补充的原因，但具体怎么修改so博主还不是很清楚，所以你如果不清楚的话，可以按照下面的说法来，也可以自己查一查so文件的相关资料。

**---------------------------------------------------------**以上为20180314更新**-------------**

注意：有时候旧版本的protobuffer生成的文件在新版本中使用会产生各种不易排查的错信息，所以推荐在需要运行caffe的环境中都使用同一版本的。所以出现标题问题的时候，强硬的方法就是卸载原先的旧版本，重新编译安装protoc。

sudo apt-get remove libprotobuf-dev 

sudo apt-get remove libprotobuf-compile 

#### 编译以及安装protoc
出现标题的问题，cd选择进入protoc的文件夹目录下，make clean，重新编译安装protoc。然后在caffe-master目录下，make clean，重新make all -j4。

实际上，有些教程是让人用apt安装`sudo apt-get install libprotobuf-dev`。我开始以为很方便，但实际上到最后回遇到一个问题，就是当你所有依赖库还有环境配好后，总是会遇到（我总共装过三次，前两次没有问题，第三次出现了问题，被逼的按照这里的来了，如果可以apt安装的话，尽量用那个，会简单很多）
`.build_release/lib/libcaffe.so: undefined reference to google ::protobuf...  `
这类的错误。所以，还是用源码包来编译安装才能把这个问题解决掉，因为只要在编译的时候出现这个问题，只要重新编译一次protobuf就ok了。 
在安装之前需要先把工具automake给装了：

```bash
sudo apt-get install autoconf automake libtool
```

那接下来就下载protobuf吧，在github下载，解压[https://github.com/google/protobuf](https://github.com/google/protobuf)
解压后，进入目录protobuf-master, 运行：
`./autogen.sh`
这是会提醒你的curl没有安装的错误。所以安装curl吧,然后在[https://github.com/paulsapps/gmock-1.7.0](https://github.com/paulsapps/gmock-1.7.0)下载gmock（如果你的电脑不能翻墙就从我给的链接下载）。下完后不用解压，直接把gmock-1.7.0-master.zip放到protobuf-master的目录下就ok了。。然后再次运行`./autogen.sh`。 
运行成功后，依次输入命令（还是在protobuf-master的目录下）：

```
./congigure --prefix = /home/yourname/local_install/
make
make check
```



之所以选择本地根目录下面安装，而不在默认的/usr或者/usr/local/是因为便于迁移，这样之后的make和make install产生的东西都会在根目录下自建文件夹里面，**无需重新编译和安装**。

`make`跟`make check`都会费一点时间（**如果你遇到很多次坑，每次都想重新编译protobuf的话记得make之前要make clean一下**）。如果`make check`之后出现类似： 
![这里写图片描述](https://img-blog.csdn.net/20160602175313455)
就可以开始

```bash
sudo make install
```



**安装之后还需要做如下操作**

1、创建文件 /etc/ld.so.conf.d/libprotobuf.conf 包含内容：

/home/yourname/local_install/lib   
2、sudo ldconfig 这时，再运行protoc --version 就可以正常看到版本号了







---------------------------------------------------------------------------------------------------------

由于上面设置的安装目录发生变化，所以我们要修改caffe的makefile.config文件，在include_dir后面添加 ~/local_install/include,在library_dir后面添加~local_install/bin 

2018/3/14更新







参考链接：http://blog.csdn.net/fansetcool/article/details/51569374




