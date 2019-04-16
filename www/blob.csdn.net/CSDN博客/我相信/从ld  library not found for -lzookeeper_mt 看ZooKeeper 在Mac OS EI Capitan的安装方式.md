# 从ld: library not found for -lzookeeper_mt 看ZooKeeper 在Mac OS EI Capitan的安装方式 - 我相信...... - CSDN博客





2016年08月26日 11:50:56[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：2266标签：[python																[library																[MacOS-EI																[setup-py机理																[zookeeper](https://so.csdn.net/so/search/s.do?q=zookeeper&t=blog)
个人分类：[python](https://blog.csdn.net/wireless_com/article/category/5586333)





## 起因

同事的Mac 升级到EI  Capitan后，zookeeper 的python client 用不了了。zookeeper 的python client 一般使用的有两种：zkpython和kazoo。 kazoo是纯python的实现，zkpython是zookeeper c client 的python binding。 这里使用的是zkpython。

## 现象

重新编译安装，报错如下：

```
sudo python setup.py build
Password:
running build
running build_ext
building 'zookeeper' extension
cc -fno-strict-aliasing -fno-common -dynamic -arch i386 -arch x86_64 -g -Os -pipe -fno-common -fno-strict-aliasing -fwrapv -DENABLE_DTRACE -DMACOSX -DNDEBUG -Wall -Wstrict-prototypes -Wshorten-64-to-32 -DNDEBUG -g -fwrapv -Os -Wall -Wstrict-prototypes -DENABLE_DTRACE -arch i386 -arch x86_64 -pipe -I/usr/include/c-client-src -I/usr/local/include/c-client-src -I/usr/include/zookeeper -I/usr/local/include/zookeeper -I/System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7 -c zookeeper.c -o build/temp.macosx-10.11-intel-2.7/zookeeper.o
zookeeper.c:362:17: warning: implicit conversion loses integer precision:
      'Py_ssize_t' (aka 'long') to 'int32_t' (aka 'int') [-Wshorten-64-to-32]
  acls->count = PyList_Size( pyacls );
              ~ ^~~~~~~~~~~~~~~~~~~~~
1 warning generated.
cc -bundle -undefined dynamic_lookup -arch i386 -arch x86_64 -Wl,-F. build/temp.macosx-10.11-intel-2.7/zookeeper.o -lzookeeper_mt -o build/lib.macosx-10.11-intel-2.7/zookeeper.so
ld: library not found for -lzookeeper_mt
clang: error: linker command failed with exit code 1 (use -v to see invocation)
error: command 'cc' failed with exit status 1
```

## 原因分析

错误的核心是 ld: library not found for -lzookeeper_mt，zookeeper的多线程c语言库没有找到。

检查zookeeper 3.4.7 的安装结果，发现/usr/lib 中并没有生成最新的libzkmt 相关库。

原因何在呢？ 

老规矩，检查环境，感觉是EI Capitan 的权限问题。经历了XCode编译器代码被注入的事件后， Mac OS X El Capitan系统的升级，启用了更高的安全性保护机制：系统完整性保护System Integrity Protection (SIP)。简单来讲就是更加强制性的保护系统相关的文件夹。开发者不能直接操作相关的文件内容。

苹果官方给出的解释：

```
System Integrity Protection is a security technology in OS X El Capitan that’s designed to help prevent potentially malicious software from modifying protected files and folders on your Mac. 
In OS X, the “root” user account previously had no permission restrictions and could access any system folder or application on your Mac. Software gained root-level access when you entered your administrator name and password to install it and could then modify or overwrite any system file or application. 
System Integrity Protection restricts the root account and limits the actions that the root user can perform on protected parts of OS X. 
Paths and applications protected by System Integrity Protection include: 
/System 
/usr 
/bin 
/sbin 
Apps that are pre-installed with OS X 
Paths and applications that third-party apps and installers can write to include: 
/Applications 
/Library 
/usr/local 
System Integrity Protection is designed to allow modifications of these protected parts only by processes that are signed by Apple and have special entitlements to write to system files, like Apple software updates and Apple installers. 
Apps that you download from the Mac App Store already work with System Integrity Protection. Other third-party software that conflicts with System Integrity Protection might be set aside when you upgrade to OS X El Capitan. 
System Integrity Protection also helps prevent software from changing your startup volume. To boot your Mac from a different volume, you can use the Startup Disk pane in System Preferences or you can hold down the Option key while you reboot, and select a volume from the list. 
Information about products not manufactured by Apple, or independent websites not controlled or tested by Apple, is provided without recommendation or endorsement. Apple assumes no responsibility with regard to the selection, performance, or use of third-party websites or products. Apple makes no representations regarding third-party website accuracy or reliability. Risks are inherent in the use of the Internet. Contact the vendor for additional information. Other company and product names may be trademarks of their respective owners. 
https://support.apple.com/en-us/HT204899
```

## 初步尝试

原因基本找到，重新编译安装zookeeper 3.4.7，指定安装路径为/usr/local

```
$sudo ./configure --prefix=/usr/local
$sudo make
$sudo make install
```

编译安装成功，在/usr/local/lib 中已经生成了zookeeper 的相关库。

现在，重新编译zkpython 0.4.2, 发现问题依旧！

## 解决

在编译链接的时候如何指定自定义的库路径呢？研读setup.py 的相关文档，就很快得到答案了。

修改zkpython 的setup.py 文件，增加zookeeper的库路径：

```bash
zookeepermodule = Extension("zookeeper",
                            sources=["zookeeper.c"],
                            include_dirs=[ "/usr/local/include/c-client-src",
                                  "/usr/local/include/zookeeper"],
                            library_dirs=["/usr/local/lib"],
                            libraries=["zookeeper_mt"],
                            )
```

重新安装zkpython，sudo python setup.py install, 安装成功。

## 检验确认

验证一下，

```
$ python
Python 2.7.11 |Anaconda 2.5.0 (x86_64)| (default, Dec  6 2015, 18:57:58) 
[GCC 4.2.1 (Apple Inc. build 5577)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
Anaconda is brought to you by Continuum Analytics.
Please check out: http://continuum.io/thanks and https://anaconda.org
>>> import zookeeper
>>>
```

一起正常了，可以自由使用zookeeper 强大的配置管理能力了。](https://so.csdn.net/so/search/s.do?q=setup-py机理&t=blog)](https://so.csdn.net/so/search/s.do?q=MacOS-EI&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)




