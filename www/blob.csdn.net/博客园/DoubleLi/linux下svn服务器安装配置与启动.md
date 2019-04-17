# linux下svn服务器安装配置与启动 - DoubleLi - 博客园






1. 采用源文件编译安装。源文件共两个，为：


     subversion-1.6.1.tar.gz  （subversion 源文件） 

     subversion-deps-1.6.1.tar.gz  （subversion依赖文件） 

     注意文件版本必须一致,否则很容易产生各种奇怪的问题.

2. 上传以上两个文件到服务器上，解压。解压命令为： 

      tar xfvz subversion-1.6.1.tar.gz 

      tar xfvz subversion-deps-1.6.1.tar.gz 

3. 解压后生成 subversion-1.6.1 子目录，两个压缩包解压后都会自动放到此目录下，不用手动更改。


4. 进入解压子目录：  cd subversion-1.6.1

5. 执行 ./configure --prefix=/usr/local/svn

      ./configure时会出现以下错误：configure: error: We require OpenSSL; try –with-openssl configure               failed for serf。



      这是因为没有安装openssl相关包

      openssl官方网址：http://www.openssl.org/

      linux下openssl的安装：

      wget [http://www.openssl.org/source/openssl-1.0.0g.tar.gz](http://www.openssl.org/source/openssl-1.0.0g.tar.gz)


       tar xvzf openssl-1.0.0g.tar.gz
       cd openssl-1.0.0g

      然后：
./config --prefix=/usr/local/ssl shared zlib-dynamic enable-camellia

      然后执行：
make  depend

Make

Make  test

make install

         编辑/etc/ld.so.conf, add to paths /usr/local/ssl/lib（可以直接在/etc/ld.so.conf.d文件

     夹下再创建一个.conf文件，把/usr/local/ssl/lib拷贝到里面，另外也可以在现有的.conf文件中

     添加/usr/local/ssl/lib这个路径，反正ld.so.conf中包含了这个文件夹下的所有.conf文件）

            安装之后会在/usr/local下生成一个ssl目录，设置环境变量，在/etc/profile的PATH中增加如下内



        容：PATH=/usr/local/ssl/bin:/sbin/:$PATH:/usr/sbin


          export PATH



 6.  再进到subversion-1.6.1 再执行 ./configure --prefix=/opt/subversion 进行配置设定并指明安装



      目录. 注意prefix前是两个减号. 默认不带BDB包, 所以默认使用的是FSFS模式. 若想使用BDB模式需另外下



      载BDB包.建议使用FSFS模式.



        configure完成可能会出现:



     You don’t seem to have Berkeley DB version 4.0.14 or newer  installed and linked to APR-UTIL. 



 We have created Makefiles which  will build without the Berkeley DB back-end; your repositories 

 will useFSFS as the default back-end.  You can find the latest version of Berkeley DB here:



[http://www.oracle.com/technology/software/products/berkeley-db/index.html](http://www.oracle.com/technology/software/products/berkeley-db/index.html)



        你似乎没有Berkeley DB版本4.0.14或更新版本安装并链接至四月- util的。我们已创建Makefile文件的将建立一个



         没有了Berkeley DB后端，你的版本库作为默认后端使用FSFS格式。你可以找到最新版本Berkeley DB的位置。



          这个提示可以跳过,因为不使用BDB存储。



7. 执行 make 编译. 


8. 执行 make install 安装.





9. 添加环境变量: 


    vi /etc/profile 

      在文件最后加入: 

   SVNPATH=$PATH:/usr/local/svn/bin 

   export SVNPATH 

10. 测试SVN是否安装成功,执行: 

                    svnserve --version 

        如果显示版本信息, 则安装成功. （如果没有安装svnserve，请先安装apt-get install subversion）





以上是在linux上安装svn服务器的步骤，下面将介绍svn服务器配置：





1. 建立版本库目录，建立好的版本库目录与安装目录不在同级目录下(可建立多个，新建库后以下各项都需



    重新配置。注意区别安装目录与版本库目录,以下讲的都是版本库目录) 


     mkdir –p /opt/svndata/repos 



2. 建立svn版本库(与上面目录对应) 


      svnadmin create /opt/svndata/repos 

      执行此命令后svn自动在repos目录下添加必须的配置文件. 



3. 修改版本库配置文件


      vi /opt/svndata/repos/conf/svnserve.conf



  各参数功能在配置文件的注释中有说明, 此处配置如下: 


[general] 

anon-access = none # 使非授权用户无法访问 

auth-access = write # 使授权用户有写权限 

password-db = /opt/svndata/repos/conf/passwd  # 指明密码文件路径 

authz-db = /opt/svndata/repos/conf/authz    # 访问控制文件

realm = /opt/svndata/repos   # 认证命名空间，subversion会在认证提示里显示，并且



作为凭证缓存的关键字。 

其它采用默认配置. 各语句都必须顶格写, 左侧不能留空格, 否则会出错. 

        指令简介：这是svn配置文件的格式，请按照以上格式书写。



4.配置用户   ：passwd文件


输入以下内容： 
[users] 
<用户1> = <密码1> admin = admin
<用户2> = <密码2> username2 = password2

可以添加多个，此为用户名密码对。

5. 配置权限 ：authz文件

    指令简介：此配置文件采用“基于路径的授权”策略，中括号里指定路径，以下列出对各用户的授权。包括只读r，读写rw。没有列出的用户，则不允许访问。还可以对用户分组，具体请参考svn手册，以下是简单例子：


<!--[if !supportLineBreakNewLine]-->       用户组格式：

    [groups]

    <用户组名> = <用户1>,<用户2>

    其中，1个用户组可以包含1个或多个用户，用户间以逗号分隔。



    版本库目录格式：
    [<版本库>:/项目/目录]
    @<用户组名> = <权限>
    <用户名> = <权限>
    其中，方框号内部分可以有多种写法:
     /,表示根目录及以下。根目录是svnserve启动时指定的，我们指定/opt/svndata。这样，/就是表示对全部版本库设置权限。
     repos1:/,表示对版本库1设置权限
     repos2:/abc, ,表示对版本库2中的abc项目设置权限
     repos2:/abc/aaa, ,表示对版本库2中的abc项目的aaa目录设置权限
     权限主体可以是用户组、用户或*，用户组在前面加@，*表示全部用户。权限可以是w、r、wr和空，空表示没有任何权限。




  注意：配置auth 、passwd和svnserve.conf一定要心细，否则客户端访问时会认证失败，如不能访问，百度和谷歌上有很多资料可供参考。




  配置完成，接下来该启动svn喽：svn默认端口是3690



1.启动命令： svnserve -d -r 版本库路径 --listen-host 192.168.1.178  



            如：svnserve -d -r /opt/svndata --listen-host 192.168.1.178



     启动时遇到的问题: 

 启动svnserve时出现cannot bind server socket:address already in use 


 解决方法: 
 方法一:找出svnserve id 杀死掉进程 

       netstat -ntlp 

       kill -9 id 


 方法二:重新指定一个端口号(默认端口3690) 

        svnserve -d --listen-port 3691 -r /opt/svndata/repos





2.检查是否启动成功:（查看是否存在svnserve 进程）



ps -ef|grep svnserve







3.最后，安装客户端，下载客户端[TortoiseSVN-1.6.15.21042-win32-svn-1.6.16.msi](http://sourceforge.net/projects/tortoisesvn/files/1.6.15/Application/TortoiseSVN-1.6.15.21042-win32-svn-1.6.16.msi)，并下载对应版



  本的语言包[LanguagePack_1.6.15.21042-win32-zh_CN.msi](http://sourceforge.net/projects/tortoisesvn/files/1.6.15/Language%20Packs/LanguagePack_1.6.15.21042-win32-zh_CN.msi)解压安装即可。





  安装好后，这样访问服务器时就可以直接用svn:// 服务器ip 来访问了。









