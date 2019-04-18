# IntelliJ IDEA 13.1.3 SVN无法正常使用问题 - z69183787的专栏 - CSDN博客
2014年11月29日 12:44:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5980
最新升级IDEA12到13版本，升级后发现IDEA中SVN无法正常使用，但文件夹下能够正常使用。
并且报错：svn: E204899: Cannot run program "svn" (in directory "G:\code\langchao\.idea"): CreateProcess error=2, ϵͳÕҲ»µ½ָ¶ 。
起初怀疑是因为IDEA不支持1.8版本，于是卸载1.8换上1.7。换成1.7后还是不行。查询资料发现13版是支持SVN1.8的，查询资料后发现1.8使用了命令工具（command line client tools）默认安装SVN时是未安装此客户端的。
![](http://static.oschina.net/uploads/space/2014/0715/152703_C7zY_269725.jpg)
![](http://static.oschina.net/uploads/space/2014/0715/152703_IR7Y_269725.jpg)
安装成功后重启IDEA发现还是在报错，进入changes视图中更换SVN版本
![](http://static.oschina.net/uploads/space/2014/0715/152832_I5zq_269725.jpg)
 去掉IDEA默认设置
![](http://static.oschina.net/uploads/space/2014/0715/153557_BbpC_269725.jpg)
搞定！
