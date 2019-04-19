# ubuntu下的eclipse支持中文---GBK编码【本人验证】 - 三少GG - CSDN博客
2011年11月07日 13:50:24[三少GG](https://me.csdn.net/scut1135)阅读数：1148标签：[eclipse																[ubuntu																[windows																[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
原创作品，允许转载，转载时请务必以超链接形式标明文章 [原始出处](http://leaze.blog.51cto.com/83088/195584) 、作者信息和本声明。否则将追究法律责任。[http://leaze.blog.51cto.com/83088/195584](http://leaze.blog.51cto.com/83088/195584)
今天，把windows下的工程导入到了Linux下eclipse中，由于以前的工程代码，都是GBK编码的，而Ubuntu默认是不支持GBK编码的。所以，首先我们要先让Ubuntu支持GBK，方法如下：
- 
**修改/var/lib/locales/supported.d/local文件**,在文件中添加
zh_CN.GBK GBK
zh_CN.GB2312 GB2312
- 
sudo dpkg-reconfigure --force locales
然后在输出的结果中会出现
zh_CN.GB2312 done
zh_CN.GBK done 
  这样， Ubuntu就支持GBK编码了， 下面设置eclipse。
 首先Windows->Preferences, 然后选择General下面的Workspace. Text fileencoding选择Other GBK，
**  如果没有GBK的选项，没关系，直接输入GBK三个字母， Apply， GBK编码的中文，已经不是乱码了。**
