# Apache启动失败，请检查相关配置。√MySQL5.1已启动。 - Jun5203 - CSDN博客
2018年10月25日 16:30:15[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：290
所属专栏：[Linux](https://blog.csdn.net/column/details/25476.html)
最近无聊在乱捣鼓的时候遇到了这样一个问题，见下图：
![在这里插入图片描述](https://img-blog.csdn.net/201810251334320?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
毫无疑问，肯定出问题了呗~不将就是发现的原动力，所以这么小小的困难自然也不能放过它。说干就干，在站在巨人的肩膀上，我找到了以下解决方法，虽然其中的有些方案并没有解决我的问题，但是总结出来一并分享给大家！
- 方案一
打开“APMServ5.2.6文件夹\Zend\eAccelerator”，查看在eAccelerator里是否有eAccelerator.dll；
- 若有
（1）在APMServ5.2.6文件夹中找到php.ini文件，把
[eaccelerator]
zend_extension_ts=“C:/Users/TEST/Desktop/APMServ5.2.6/zend/eAccelerator/eaccelerator.dll”
改为
[eaccelerator]
zend_extension_ts=“C:/Users/TEST/Desktop/APMServ5.2.6/zend/eAccelerator/eAccelerator.dll”
（2）在APMServ5.2.6文件夹中找到php.ini文件，把
[eaccelerator]
zend_extension_ts=“C:/Users/TEST/Desktop/APMServ5.2.6/zend/eAccelerator/eAccelerator0953_5.2.5.dll”
改为：
[eaccelerator]
zend_extension_ts=“C:/Users/TEST/Desktop/APMServ5.2.6/zend/eAccelerator/eAccelerator.dll”
- 若没有
[下载地址](http://www.sitebuddy.com/PHP/Accelerators/eAccelerator_windows_binaries_builds)
注意下载eAccelerator 0953 for PHP 5.2.6，下载后直接放到APMServ5.2.6\Zend\eAccelerator文件夹中，再打开php.ini文件，把
[eaccelerator]
zend_extension_ts=“C:/Users/TEST/Desktop/APMServ5.2.6/zend/eAccelerator/eAccelerator0953_5.2.5.dll”
改为：
[eaccelerator]
zend_extension_ts=“C:/Users/TEST/Desktop/APMServ5.2.6/zend/eAccelerator/eAccelerator0953_5.2.6.dll”
结果：未解决
- 方案二
1、打开“APMServ5.2.6文件夹\Apache\conf\extra\httpd-ssl.conf”；
2、用记事本打开“httpd-ssl.conf”；
3、查找“/cgi-bin”；
4、修改路径为软件所在位置
注意：此种问题一般只有在盘数较多的情况下出现。
结果：未解决
- 方案三
把“APMServ5.2.6”界面左下角的“SSL”前的对勾去掉（如果没有证书，去掉对勾后基本上就没问题，后期装上证书也可以打上对勾）
![在这里插入图片描述](https://img-blog.csdn.net/20181025133943907?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
重新运行后正常
![在这里插入图片描述](https://img-blog.csdn.net/20181025133948648?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
结果：解决
- 方案四
检查操作系统“设置 - 管理工具 - 服务” 里面的选项，Telnet选项被禁止，把它设置成手动开启。开启Telnet后，再启动APMServ5.2.6，也可能就正常。
- 方案五
将PHP的安装目录设置为系统的环境变量：右键点击“我的电脑”-“属性”-“高级”-“环境变量”-“系统变量”，修改Path，加上“C:/Users/TEST/Desktop/APMServ5.2.6/PHP;C:/Users/TEST/Desktop/APMServ5.2.6/PHP/ext”路径。
注意用英文分号;进行分隔，同时上面的“D:\APMServ5.2.6\PHP;D:\APMServ5.2.6\PHP\ext”
或
把需要用到的dll复制到操作系统的System32文件夹中。即把PHP和PHP/ext下面的.dll文件拷到C:\WINDOWS\system32文件夹中。
- 方案六
php.ini里的extension_dir配置是正确的，可能是以前在本机安装过PHP，有一些残留文件，windows PATH变量或拷贝到C:\WINDOWS\system32下的dll文件被Apache优先加载了,而没有加载应该加载的APACHE目录中的dll,，注意:windows加载dll文件的顺序是按系统环境变量PATH里指定的目录的先后来的，一般是%SystemRoot%\system32;%SystemRoot%，在安装Apache+PHP的时候如果出现“Unable to load dynamic library”错误，但配置文件又是正确的情况下，一定要注意这些目录下有没有当前PHP版本需要使用的dll不同版本的同名文件。
- 方案七
点击"Apache设置(A)-查看运行日志(E)",发现下面的文字: PHP Warning: [eAccelerator] This build of “eAccelerator” was compiled for PHP version 5.2.6. Rebuild it for your PHP version (5.2.9) or download precompiled binaries.\n in Unknown on line 0 PHP Fatal error: Unable to start eAccelerator module in Unknown on line 0
由上面的日志可以清楚的判断到是php版本的原因。由于PHP version (5.2.6) 不会改动,所以问题就是eAccelerator编译时的版本要用5.2.9。
