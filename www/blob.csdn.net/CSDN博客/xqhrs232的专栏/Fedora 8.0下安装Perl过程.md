# Fedora 8.0下安装Perl过程 - xqhrs232的专栏 - CSDN博客
2016年04月15日 10:47:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：388
原文地址::[http://www.linuxidc.com/Linux/2008-04/12583.htm](http://www.linuxidc.com/Linux/2008-04/12583.htm)
相关文章
1、fedora 安装perl----[http://blog.sina.com.cn/s/blog_6dbecc200100p5gb.html](http://blog.sina.com.cn/s/blog_6dbecc200100p5gb.html)
想在[Fedora](http://www.linuxidc.com/topicnews.aspx?tid=5) 8.0下学学用Perl来对文本和Excel数据进行分析，于是在chinaunix网站下载[perl-5.8.8.tar.bz2](http://down1.chinaunix.net/distfiles/perl-5.8.8.tar.bz2)，下载到本地后，
tar jxvf perl-5.8.8.tar.bz2
cd perl-5.8.8
./Configure -de
make
make test
make install
几点说明：
1 -de为配置参数， 意思是缺省配置；
2 这里有一步make test需要进行；
3 安装完成后 perl 所在目录为 /usr/local/lib/perl5, perl 执行文件在 /usr/local/bin里；
4 如果第一次没有安装成功，再次安装的时候要记得删除config.sh， Policy.sh两个文件。
5 安装完成后，由于已经注册环境变量，perl已经可以在shell里被识别，即可以tab自动补全。
6 在vim里编辑一个简单的测试脚本test是：
＃！ /usr/local/bin/perl
print "Hello, world!\n";
退出，perl test
如果出现：Hello,world!
恭喜你，成功了。
