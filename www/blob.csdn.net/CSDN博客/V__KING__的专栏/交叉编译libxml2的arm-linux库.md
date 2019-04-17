# 交叉编译libxml2的arm-linux库 - V__KING__的专栏 - CSDN博客





2017年07月06日 13:35:55[v__king__](https://me.csdn.net/V__KING__)阅读数：1354








# 编译libxml2

一般情况下：只需要coufigure –host和–prefix就可以了

configure的交叉编译参考：[http://blog.csdn.net/v__king__/article/details/59113188](http://blog.csdn.net/v__king__/article/details/59113188)

libxml2交叉编译参考： 
[http://blog.csdn.net/q1302182594/article/details/44975527](http://blog.csdn.net/q1302182594/article/details/44975527)
[http://www.th7.cn/Program/Python/201410/294146.shtml](http://www.th7.cn/Program/Python/201410/294146.shtml)
这里给出我对arm-linux-gcc交叉编译工具的配置项：
`./configure --prefix=/home/fangy/xml2_arm --host=arm-linux CC=/home/vk/arm-linux-gcc-3.4.1/usr/local/arm/3.4.1/bin/arm-linux-gcc --without-zlib --without-python`
上述配置中，–without-python –without-zlib，是用来掉和zlib和python的绑定的，因为在我得arm板子上的项目，不需要用到Python和zlib。

运行configure 命令的时候没有出现错误。 

运行make： 没有出现错误。 

运行make install： 出现了错误：
```
make  install-recursive
make[1]: Entering directory `/home/vk/Downloads/libxml2-2.9.2'
Making install in include
make[2]: Entering directory `/home/vk/Downloads/libxml2-2.9.2/include'
Making install in libxml
make[3]: Entering directory `/home/vk/Downloads/libxml2-2.9.2/include/libxml'
make[4]: Entering directory `/home/vk/Downloads/libxml2-2.9.2/include/libxml'
make[4]: Nothing to be done for `install-exec-am'.
 /bin/mkdir -p '/home/fangy/xml2_arm/include/libxml2/libxml'
 /usr/bin/install -c -m 644 SAX.h entities.h encoding.h parser.h parserInternals.h xmlerror.h HTMLparser.h HTMLtree.h debugXML.h tree.h list.h hash.h xpath.h xpathInternals.h xpointer.h xinclude.h xmlIO.h xmlmemory.h nanohttp.h nanoftp.h uri.h valid.h xlink.h xmlversion.h DOCBparser.h catalog.h threads.h globals.h c14n.h xmlautomata.h xmlregexp.h xmlmodule.h xmlschemas.h schemasInternals.h xmlschemastypes.h xmlstring.h xmlunicode.h xmlreader.h relaxng.h dict.h '/home/fangy/xml2_arm/include/libxml2/libxml'
 /usr/bin/install -c -m 644 SAX2.h xmlexports.h xmlwriter.h chvalid.h pattern.h xmlsave.h schematron.h '/home/fangy/xml2_arm/include/libxml2/libxml'
make[4]: Leaving directory `/home/vk/Downloads/libxml2-2.9.2/include/libxml'
make[3]: Leaving directory `/home/vk/Downloads/libxml2-2.9.2/include/libxml'
make[3]: Entering directory `/home/vk/Downloads/libxml2-2.9.2/include'
make[4]: Entering directory `/home/vk/Downloads/libxml2-2.9.2/include'
make[4]: Nothing to be done for `install-exec-am'.
make[4]: Nothing to be done for `install-data-am'.
make[4]: Leaving directory `/home/vk/Downloads/libxml2-2.9.2/include'
make[3]: Leaving directory `/home/vk/Downloads/libxml2-2.9.2/include'
make[2]: Leaving directory `/home/vk/Downloads/libxml2-2.9.2/include'
Making install in .
make[2]: Entering directory `/home/vk/Downloads/libxml2-2.9.2'
make[3]: Entering directory `/home/vk/Downloads/libxml2-2.9.2'
 /bin/mkdir -p '/home/fangy/xml2_arm/lib'
 /bin/bash ./libtool   --mode=install /usr/bin/install -c   libxml2.la '/home/fangy/xml2_arm/lib'
libtool: install: /usr/bin/install -c .libs/libxml2.so.2.9.2 /home/fangy/xml2_arm/lib/libxml2.so.2.9.2
libtool: install: (cd /home/fangy/xml2_arm/lib && { ln -s -f libxml2.so.2.9.2 libxml2.so.2 || { rm -f libxml2.so.2 && ln -s libxml2.so.2.9.2 libxml2.so.2; }; })
libtool: install: (cd /home/fangy/xml2_arm/lib && { ln -s -f libxml2.so.2.9.2 libxml2.so || { rm -f libxml2.so && ln -s libxml2.so.2.9.2 libxml2.so; }; })
libtool: install: /usr/bin/install -c .libs/libxml2.lai /home/fangy/xml2_arm/lib/libxml2.la
libtool: install: /usr/bin/install -c .libs/libxml2.a /home/fangy/xml2_arm/lib/libxml2.a
libtool: install: chmod 644 /home/fangy/xml2_arm/lib/libxml2.a
libtool: install: arm-linux-ranlib /home/fangy/xml2_arm/lib/libxml2.a
./libtool: line 1085: arm-linux-ranlib: command not found
make[3]: *** [install-libLTLIBRARIES] Error 127
make[3]: Leaving directory `/home/vk/Downloads/libxml2-2.9.2'
make[2]: *** [install-am] Error 2
make[2]: Leaving directory `/home/vk/Downloads/libxml2-2.9.2'
make[1]: *** [install-recursive] Error 1
make[1]: Leaving directory `/home/vk/Downloads/libxml2-2.9.2'
make: *** [install] Error 2
```

仔细的查看出错信息，虽然make install时，有错误信息，但是那些错对与在arm板子上使用libxml2库不影响。我们需要的是：交叉编译后生成的的include和lib就可以了，而不需要bin目录或其他。 

所以交叉编译libxml2之后，虽有错，但无妨，只要arm平台的库生成了，而库需要的就是头文件和.so或.a文件。

接下来，可以将libxml2.so或者加入到Makefile中，就不会出现链接错误了。





