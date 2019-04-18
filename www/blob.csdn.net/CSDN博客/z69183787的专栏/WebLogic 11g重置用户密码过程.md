# WebLogic 11g重置用户密码过程 - z69183787的专栏 - CSDN博客
2014年04月12日 09:18:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1700
weblogic安装后，很久不用，忘记访问控制台的用户名或者密码，可通过以下步骤来重置用户名密码。
版本：WebLogic [**Server**](http://blog.csdn.net/yang_95/article/details/8141412)**[1](http://blog.csdn.net/yang_95/article/details/8141412)2c**
os:   windows 7 
说明：%DOMAIN_HOME%：指WebLogic Server 域(Domain）目录
例如我的做测试的域的根目录 DOMAIN_HOME=\bea/weblogic\user_projects\domains\mydomain
1.为了保证操作安全，备份%DOMAIN_HOME%\security\DefaultAuthenticatorInit.ldift
2. 进入%DOMAIN_HOME%/security目录，执行下列命令：
 java -cp D:\Oracle\Middleware\wlserver_12.1\server\lib\weblogic.jar  weblogic.security.utils.AdminAccount username password .
特点注意最后有个“ .”，一个空格和一个点。
此命令将生成新的DefaultAuthenticatorInit.ldift文件覆盖原来的。
3. 进入管理服务器的AdminServer目录，如：%DOMAIN_HOME%\servers\AdminServer。将其中的[**data**](http://blog.csdn.net/yang_95/article/details/8141412)目录重命名，如：data_old。或者[**备份**](http://blog.csdn.net/yang_95/article/details/8141412)到别的地方。
4. 修改管理服务器的boot.properties文件，如：%DOMAIN_HOME%\servers\AdminServer\security\boot.properties，修改其中的用户名与密码（用明文，第一次启动服务器时明文将被加密），要和上面命令
中的用户名密码一致。
例：修改后：
username=weblogic
password=weblogic
5. 重新启动服务器后，就可以使用用户weblogic登录管理控制台了。
