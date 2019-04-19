# 安装xgboost中的 坑！！！Command "python setup.py egg_info" failed with error code 1 in /private/var/folders - 数据之美的博客 - CSDN博客
2018年04月28日 18:00:54[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：551
我最开始学习xgboost是直接使用机器学习平台跑的模型，当然前提是在了解了xgboost的原理之后。
之后自己想安装xgboost的时候，发现了一个问题，困扰了很长一段时间。
我最开始使用的是python3，然后安装xgboost时会报错，网上找也找不到原因。
然后就直接放弃python3，改装上python2，再去安装xgboost，但是也碰到了跟之前同样的问题，后来安装成功了，但是之前错误的log，没有截图了。
但是错误还记得：
Command "python setup.py egg_info" failed with error code 1 in /private/var/folders
对，就是这个原因。后来去网上找遇到同样问题的人，找答案，没有一个解决办法能解决这个问题，后来才发现
是自己按照教程操作时，误解了教程的意思
教程中说
在执行完
1.
/usr/bin/ruby -e "$(curl -fsSL [https://raw.githubusercontent.com/Homebrew/install/master/install](https://raw.githubusercontent.com/Homebrew/install/master/install))"
2.
brew install gcc --without-multilib
之后，再安装xgboost
```
cd <directory>
git clone --recursive https://github.com/dmlc/xgboost
```
对，没错。
然后的步骤是：
```
cd <directory>/xgboost
cp make/config.mk .
```
意思就是将 make/config.mk 做个备份，备份到当前目录下即可，即./config.mk，然后就可以修改 config.mk 中的内容了，官方的意思是很好的，意思就是修改一下文件make/config.mk中的相应内就行，修改方法：
export CC = gcc
export CXX = g++
将上面两行取消注释，并修改为下面的样子（当然了，如果你的gcc版本是5,就将7改为5即可）：
export CC = gcc-7
export CXX = g++-7
但是，初次安装xgboost的我，却想当然的认为，只需要修改文件 ./config.mk 中的相应内容即可，而不用修改文件make/config.mk中的相应内容，天真的我就一直跑啊跑，怎么都不能安装成功，直到我看见了这篇教程https://www.ibm.com/developerworks/community/blogs/jfp/entry/Installing_XGBoost_on_Mac_OSX?lang=zh ，我就知道我错在哪里了。
教程中的安装顺序跟之前看的其他教程是不太一样的，这个教程里面是要先修改config.mk中的相应内容（而此时由于还没有做备份，所以只能修改文件make/config.mk中的内容），然后才做备份操作 cp make/config.mk .当然了，就事论事，他这个操作也是有问题的（因为对重要的文件做备份的原则是对源文件做备份，而这里是直接对修改后的文件做备份，已经失去了备份的意义），但是不妨碍帮我解决问题啊，哈哈哈哈。
到此为止，大家明白了吧！
为了安装xgboost更加顺利，再次友情推荐安装xgboost的教程
https://www.ibm.com/developerworks/community/blogs/jfp/entry/Installing_XGBoost_on_Mac_OSX?lang=zh
严格按照操作中的顺序来进行操作！！！切记切记
