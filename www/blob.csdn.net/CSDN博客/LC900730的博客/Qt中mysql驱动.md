# Qt中mysql驱动 - LC900730的博客 - CSDN博客
2017年07月10日 23:28:27[lc900730](https://me.csdn.net/LC900730)阅读数：178
MAC下，在Qt中，使用mysql导入头文件会，连接还会报错，原因是缺少一个驱动。 
打开/Users/edz/Qt/5.6/clang_64/plugins/sqldrivers， 
otool打开： 
![这里写图片描述](https://img-blog.csdn.net/20170710232545029?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
发现驱动指向了其他的目录。 
网上各种说编译安装的，这里有一个简单方法。 
那就是复制一份驱动到其指向的目录下。
找到mysql的安装目录，如果是brew安装的，那么应该是/usr/local/mysql，那么再进入到lib目录下，有一个libmysqlclient.20.dylib文件，可以拷贝到qt中mysql驱动指向的目录，即： 
cp /usr/local/mysql/lib/libmysqlclient.20.dylib /opt/local/lib/mysql55/mysql/libmysqlclient.18.dylib。 
即可在Qt下正常连接mysql。
