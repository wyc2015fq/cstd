# weblogic9/10/11g 应用服务用户名密码自动认证 - z69183787的专栏 - CSDN博客
2014年09月11日 17:01:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1164
weblogic下发布应用程序时，选择生产模式，要输入用户名和密码，应用程序启动的时候，必须如入才能正常启动服务。这给自动化运维带来一些困难。
下面介绍用户名密码自动认证
weblogic9/10版本:
在domains下面的应用域下建立一个boot.properties文件，内容如下：
username=weblogic
password=weblogic
保存即可，启动服务后，weblogic会自动加密，然后去调用里面的用户名密码去认证，正常启动服务。
weblogic 11g:
boot.properties文件的存放路径有了改变，具体可以按照如下方法实现用户名密码自动认证：
在目录：domains\你的域名称\servers\你的服务名称\
如：domains\test_domain\servers\TestServer\security\boot.properties
在这个目录下面建立一个名为security的目录，然后将boot.properties文件建立好之后，放到这个目录即可。启动服务的时候会自动调用这个目录下面的文件去认证用户名密码。
