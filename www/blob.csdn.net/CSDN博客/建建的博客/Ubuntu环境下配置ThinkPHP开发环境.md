# Ubuntu环境下配置ThinkPHP开发环境 - 建建的博客 - CSDN博客
2018年08月23日 09:10:26[纪建](https://me.csdn.net/u013898698)阅读数：87
个人分类：[java学习路线](https://blog.csdn.net/u013898698/article/category/7152763)
## 1.首先还是安装php
这里不多说，具体参考[这篇文章](http://blog.csdn.net/abby___/article/details/78632860)
## 2.安装composer
```
cd 你要下载的目录
curl -sS https://getcomposer.org/installer | php
```
- 1
- 2
这里如果之前php配置的时候没有开启openssl拓展的话就会报错，我们可以尝试开启openssl拓展，当然也可以重新编译php 
![这里写图片描述](https://img-blog.csdn.net/20171125223356853?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQWJieV9fXw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
***我们首先得保证openssl已经正确地安装***
```
wget https://www.openssl.org/source/openssl-1.1.0g.tar.gz
tar -zxvf openssl-1.1.0g.tar.gz
./config --prefix=/usr/local/openssl
make 
make install
```
- 1
- 2
- 3
- 4
- 5
切换到php安装目录，编译安装openssl拓展
```
cd /ext/openssl
phpize 
./configuree --with-openssl=/usr/local/openssl
make  
make install
```
- 1
- 2
- 3
- 4
- 5
phpize这一步可能会出现autoconf未配置的问题，依次安装m4和autoconf就可以了
```
wget http://ftp.gnu.org/gnu/m4/m4-1.4.18.tar.gz
wget http://ftp.gnu.org/gnu/autoconf/autoconf-2.69.tar.gz
//然后解压编译安装
```
- 1
- 2
- 3
然后在php.ini中添加
```
extension_dir=你的拓展位置
extension=openssl.so
```
- 1
- 2
用phpinfo()查看配置信息，看到下图时说明openssl拓展已经成功配置了 
![这里写图片描述](https://img-blog.csdn.net/20171126102211484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQWJieV9fXw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
再次下载composer，如果php没有开启zlib拓展的话会得到这样的信息，大致就是没有zlib拓展的话composer就会不稳定，而且速度会大幅下降，那我们可以开启zlib拓展，方法和上面的一致，这里就不多说了 
![这里写图片描述](https://img-blog.csdn.net/20171126102546578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQWJieV9fXw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
接下来将composer添加到/usr/local/bin
`mv composer.phar /usr/local/bin/composer`- 1
## 3.安装TP5
切换到web根目录下面并执行下面的命令
```
composer create-project topthink/think tp5  --prefer-dist
```
- 1
- 2
如果之前没有安装过git的话会报错，因为要用到git来clone 
![这里写图片描述](https://img-blog.csdn.net/20171126105923020?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQWJieV9fXw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
`apt-get install git就好了`- 1
浏览器访问/localhost/tp5/public，看到下图说明tp5已经成功配置 
![这里写图片描述](https://img-blog.csdn.net/20171126110215155?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQWJieV9fXw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4.配置Nginx支持Pathinfo 
Nginx默认是不支持Pathinfo的，可以在nginx.conf文件中修改
```
fastcgi_split_path_info ^(.+\.php)(.*)$;       #增加这一句
     fastcgi_param PATH_INFO $fastcgi_path_info;    #增加这一句
```
- 1
- 2
重启Nginx就可以了
***到这里所有的配置都完成了，希望这篇文章能够帮到有需要的人***
