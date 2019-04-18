# 网络请求之mac 安装libcurl我也不知道在干嘛 - weixin_33985507的博客 - CSDN博客
2018年04月27日 14:26:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
总的来说，我就是想在C++代码里加一个网络请求，也是我不懂C++的原因吧，找了半天也没有找到一句代码就解决的方法，然后说这个libcurl可是进行网络请求，所以我就试一下，那么第一步就是安装。。。
来吧
首先，需要安装autoconf和automake工具
1）安装m4
curl http://ftp.gnu.org/gnu/m4/
tar -xzvf m4-1.4.17.tar.gz
cd m4-1.4.17
./configure --prefix=/usr/local
make
sudo make install
2）安装autoconf
curl http://ftp.gnu.org/gnu/autoconf/
tar -xzvf autoconf-2.69.tar.gz
cd autoconf-2.69
./configure --prefix=/usr/local
make
sudo make install
3）安装automake
curl http://ftp.gnu.org/gnu/automake/
tar xzvf automake-1.15.tar.gz
cd automake-1.15
./configure --prefix=/usr/local
make
sudo make install
4）安装libtool
curl http://mirror.hust.edu.cn/gnu/libtool/
tar xzvf libtool-2.4.6.tar.gz
cd libtool-2.4.6
./configure --prefix=/usr/local
make
sudo make install
5）下载libcurl源码，运行libcurl初始化脚本
网上给的是错的，反正我按那个操作是错的，我的方法就是去官网上下好包自己解压，再运行脚本。。
下载地址：[curl - Download](https://link.jianshu.com?t=https%3A%2F%2Fcurl.haxx.se%2Fdownload.html)
cd curl
./buildconf
6)编译libcurl
最后一步也是跟着操作是报错我是看的[这个文章](https://link.jianshu.com?t=http%3A%2F%2Fblog.51cto.com%2Farmyshen%2F1401279)操作的
当当~最后就得到文中的东东啦，可是接下来我要怎么做我还不造~~~哎
也希望看到这篇文章，然后又很懂的人指点一下
最后还是我的信念，帮助他人就是帮助自己，希望我可以解决你现在的困惑，然后天降大神来帮助我~~~
世界和平，耶！
------------------------------这是一条清纯而不做作的分隔线--------------------
修正以上错误：（画重点）
我按照以上的操作在XOCDE编译时报了下图错误 
![4155676-ba6a5e85ffea7f5f.png](https://upload-images.jianshu.io/upload_images/4155676-ba6a5e85ffea7f5f.png)
这是为什么呢，本来就不懂，只能各种查~~
于是我找到[这篇文章](https://link.jianshu.com?t=https%3A%2F%2Fblog.csdn.net%2Fq96456%2Farticle%2Fdetails%2F44493821)
又于是我重新做了最后一步重点是（./configure --prefix=./libcurl --without-ssl  --disable-ldap --disable-ldaps --without-zlib CC=clang）
之后重新加新的文件就可以了~~~
我再贴上我加的库
![4155676-00e4c31cc1dd7997.png](https://upload-images.jianshu.io/upload_images/4155676-00e4c31cc1dd7997.png)
友情链接：
[libcurl在mac上编译 - CSDN博客](https://link.jianshu.com?t=https%3A%2F%2Fblog.csdn.net%2Fyuanya%2Farticle%2Fdetails%2F58671758)
[编译问题：configure: error: invalid variable name: `–prefix' - CSDN博客](https://link.jianshu.com?t=https%3A%2F%2Fblog.csdn.net%2Fmyweishanli%2Farticle%2Fdetails%2F23944729)
[转 mac下autoconf automake安装_hhool_新浪博客](https://link.jianshu.com?t=http%3A%2F%2Fblog.sina.com.cn%2Fs%2Fblog_586c261a0101jil1.html)
[mac上编译安装libcurl-纯洁码农-51CTO博客](https://link.jianshu.com?t=http%3A%2F%2Fblog.51cto.com%2Farmyshen%2F1401279)
