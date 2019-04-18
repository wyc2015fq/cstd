# Win下PHP环境Eclipse PDT+XAMPP+XDebug部署 - 专注于数据挖掘算法研究和应用 - CSDN博客





2013年08月26日 16:57:22[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3745








1、XAMPP部署

1）下载XAMPP：[http://www.apachefriends.org/zh_cn/xampp-windows.html](http://www.apachefriends.org/zh_cn/xampp-windows.html)

       版本：xampp-win32-1.8.2-1-VC9.7z

2）解压后，运行“setup_xampp.bat”（目录不在C根目录下的），

3） 运行 xampp-control.exe，启动apache，如果80端口冲突，到xampp\apache\conf目录下打开httpd.conf配置listen端口，这里配置为8080；

      访问 [http://localhost:8080/xampp/phpinfo.php](http://localhost/xampp/phpinfo.php)

     可以看到apache使用的PHP配置文件为：xampp\php\php.ini

3）启用XDebug：

      打开xampp\php\php.ini，启用文件最后的XDebug项（去掉;即可），重启apache可以看到XDebug项；



2、Eclipse PDT部署：

1）安装JDK6，新增环境变量JAVA_HOME，CLASSPATH，配置环境变量PATH；

2）下载Eclipse PDT：[http://www.eclipse.org/pdt/downloads/](http://www.eclipse.org/pdt/downloads/)

       版本：zend-eclipse-php-3.2.0-Win32.zip

       解压后，运行双击zend-eclipse-php.exe运行pdt，配置workspace，默认到安装目录\zend-eclipse-php\workspace下；

3）配置PDT的PHP运行环境：

      打开window -> perference -> PHP->PHP executables

      新增一个executable：

      Name: xampp-php

      Executable path: xampp\php\php.exe

      PHP ini file: xampp\php\php.ini

      PHP Debugger: XDebug



      打开window -> perference -> PHP->Debug，设置：

      PHP Debuger: XDedug

      Server: Default

      PHP Executable:xampp-php

      uncheck "Break at first line"



3、配置调测环境：

      1）打开xampp\apache\conf目录下httpd.conf文件，增加apache虚拟目录，在文件最后增加：


Alias /wp     /zend-eclipse-php/workspace  ——为Eclipse PDT的workspace；

           Alias /wp/    /zend-eclipse-php/workspace

          <Directory "/zend-eclipse-php/workspace/">  

                      Options Indexes FollowSymLinks Includes ExecCGI  

                       AllowOverride All  

                      Order allow,deny  

                      Allow from all    

           </Directory> 

           这个虚拟目录设置遇到权限问题，参考一些文章，说要设置目录为根目录下，修改后也不行，最后直接将eclipse的workspace设置成xampp\htdocs

       2）新建php项目proj1和文件index.php，在/zend-eclipse-php/workspace目录下；

            启动apache，访问[http://localhost:8080/wp/proj1/index.php](http://localhost:8080/xampp/phpinfo.php) 即可在Eclipse PDT内debug和run工程。











