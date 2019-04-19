# 让 CAS 5.1.8 支持http，解决未认证授权服务错误提示问题 - 左直拳的马桶_日用桶 - CSDN博客
2018年03月02日 19:41:08[左直拳](https://me.csdn.net/leftfist)阅读数：3903标签：[tomcat																[casia-3d](https://so.csdn.net/so/search/s.do?q=casia-3d&t=blog)](https://so.csdn.net/so/search/s.do?q=tomcat&t=blog)
个人分类：[Java](https://blog.csdn.net/leftfist/article/category/2412493)
好不容易将CAS 5.1.8编译完毕，生成war包，并修改配置，支持数据库（oracle）方式认证，部署到tomcat，准备用了，孰料一打开客户端应用程序，跳到CAS，没有账号密码输入框，只有一句刺目的错误提示： 
![这里写图片描述](https://img-blog.csdn.net/20180302191225254?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
方便各位后来者在搜索引擎上查找答案，附录相关文字：
```
未认证授权的服务
CAS的服务记录是空的，没有定义服务。 希望通过CAS进行认证的应用程序必须在服务记录中明确定义。
Application Not Authorized to Use CAS
You are not authorized to access the application as your account \
is missing privileges required by the CAS server to authenticate into this service. Please notify your support desk.
The application you attempted to authenticate to is not authorized to use CAS. Contact your CAS administrator to learn how you might register and integrate your application with CAS.
The services registry of CAS is empty and has no service definitions. \
Applications that wish to authenticate with CAS must explicitly be defined in the services registry.
```
我刚开始看到这个提示，以为要将应用程序的地址在CAS上注册，这样CAS才会给你认证、提供服务。后来搜索了好久，没看到有人提起。基本都是说其实涉及到https/http认证的问题。因为据说从cas 4.*开始，CAS 就默认只支持https的方式。现在我们这个应用程序都是http的，所以CAS不认。
搜遍了古今中外，最后的解决方案是：
1、修改 \WEB-INF\classes\services\HTTPSandIMAPS-10000001.json
```php
"serviceId" : "^(https|imaps)://.*" 
改为==>
 "serviceId" : "^(https|http|imaps)://.*",
```
2、修改项目配置文件\WEB-INF\classes\application.properties
```
cas.tgc.secure=false
cas.serviceRegistry.initFromJson=true #这一点真是太重要了！！！！！！！！
```
基本上，都没有人会提到最后一条：`cas.serviceRegistry.initFromJson=true`，所以无论我怎么改HTTPSandIMAPS-10000001.json，都不起作用，永远都是出现红色的错误提示。后来在一个老外的回答里看到有这么一句，我心里知道，应该就是这个问题了。为什么这些答案会隐藏得这么深！
另外，我看到有人提到要修改 `c:\tomcat\etc\cas\config\cas.properties` ，这是什么鬼？其实，就是有些人将上面第二个步骤的配置写在了这里而已。将配置统一写在application.properties不好吗，要分开多处。况且这个etc目录，是CAS运行的时候自动生成的，里面放了日志。它没有位于tomcat里，而是在tomcat所在的盘的根目录下。估计路径是可以改的，但统一写在application.properties比较好吧，CAS能读到的，一点问题没有。
至于这个提示 
![这里写图片描述](https://img-blog.csdn.net/20180302194952646?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
是去不掉的，除非支持https，或者干脆手动改这个页面。
