# UTF-8、en_US.UTF-8和zh_CN.UTF-8的区别 - 数据之美的博客 - CSDN博客
2017年06月30日 11:41:39[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：4791
个人分类：[linux(ubuntu)](https://blog.csdn.net/oppo62258801/article/category/6738406)
en_US.UTF-8、zh_CN.UTF-8叫做字符集，就是说‘A’、‘B’、‘中’、‘国’等对应的整数值， 
en_US.UTF-8只包含了ASCII码，zh_CN.UTF-8包含了6000多个汉字？ 
如果是这样的话，那所谓UTF-8，就是把这些整数，编写成一串字节的方法？
UTF-8 是编码方式，
en_US.UTF-8 和 zh_CN.UTF-8 是语言环境，也就是字符集
en_US.UTF-8 和 zh_CN.UTF-8 包含的字符数量是基本上一样的，大概是七万个汉字,编码都是 UTF-8 编码，字符集是 Unicode，版本是 4.1 吧？zh_CN.GB18030 和 zh_CN.GBK 的字符集都与 Unicode 3 是等价的，记得应该是这样 
语言环境的差别就是另一回事了，中国人和外国人表示时间啦，数字啦，习惯都不一样，所以必须区分开。大部分程序根据语言环境变量来选择界面的语言是中文还是英文。
**所以一般[Linux](http://lib.csdn.net/base/linux)系统的语言环境设置成：export LANG=zh_CN.UTF-8，代表中国人使用的unicode字符集**
en_US.UTF-8：你说英语，你在美国，字符集是utf-8
zh_CN.UTF-8：你说中文，你在中国，字符集是utf-8
如果你的LANG环境变量是en_US.UTF-8，那么系统的菜单、程序的工具栏语言、输入法默认语言就都是英文的。
如果你的LANG环境变量是zh_CN.UTF-8，那么系统的菜单、程序的工具栏语言、输入法默认语言就都是中文的。
http://825635381.iteye.com/blog/2087814
http://www.iteye.com/problems/90396
[](http://blog.csdn.net/huoyunshen88/article/details/41113633#)[](http://blog.csdn.net/huoyunshen88/article/details/41113633#)[](http://blog.csdn.net/huoyunshen88/article/details/41113633#)[](http://blog.csdn.net/huoyunshen88/article/details/41113633#)[](http://blog.csdn.net/huoyunshen88/article/details/41113633#)[](http://blog.csdn.net/huoyunshen88/article/details/41113633#)
