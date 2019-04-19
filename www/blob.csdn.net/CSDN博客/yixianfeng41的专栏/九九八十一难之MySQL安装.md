# 九九八十一难之MySQL安装 - yixianfeng41的专栏 - CSDN博客
2016年09月24日 12:56:27[第2梦](https://me.csdn.net/yixianfeng41)阅读数：1486
## MySql安装（win7)
任何东西都是“会者不难，难者不会”。即便是MySQL，迈出第一步也是不容易的。以前都用Sql Server,然后每次都要经历重装一次系统方能将其卸载干净的痛苦。虽然，作为微软忠实用户，我也实在受不了这个，那就用Mysql吧。简简单单的东西，我还是折腾了一下午才装好，这里我主要分享一下我遇到的问题，如果以后还有mysql小白，或许有帮助。
### **1、下载Mysql**
现在MySql安装已经比以前简单很多了，直接下载免安装包，解压，使用即可。下载地址：[http://dev.mysql.com/downloads/](http://dev.mysql.com/downloads/)。进入后，页面如如下：
![这里写图片描述](https://img-blog.csdn.net/20160924114301600)
点击DOWNLOAD链接，进入下载页面： 
![这里写图片描述](https://img-blog.csdn.net/20160924114653430)
这里要求注册网站账号，但可以直接通过。点击下方的No thanks,just start my download,开始下载。将下载的压缩包解压到指定目录中，比如解压到E:\MySQL文件下。
### **2、添加配置**
#### 2.1、编辑my.ini文件
将mysql解压后，会发现目录中有一个my-default.ini文件，这是一个参数配置文件，初始内容如下：
![这里写图片描述](https://img-blog.csdn.net/20160924115249057)
我们将其改名为my.ini，并添加相应参数，修改后，文件 内容如下：
![这里写图片描述](https://img-blog.csdn.net/20160924115425261)
#### 2.2、添加环境变量
操作如下： 
1）右键单击我的电脑->属性->高级系统设置(高级)->环境变量， 点击系统变量下的新建按钮 
     输入变量名：MYSQL_HOME 
     输入变量值：E:\mysql\bin 
2）选择系统变量中的Path； 
      点击编辑按钮； 
      在变量值中添加变量值：;%MYSQL_HOME%\bin；
### **3、注册服务**
从控制台进入到mysql加压目录后的bin文件下，也可以之间在bin文件下按住shift，点击鼠标右键。输入服务安装命令： mysqld install mysql –defaults-file=”E:\MySQL\my.ini”,此时有可能出现如下问题。 
![这里写图片描述](https://img-blog.csdn.net/20160924120404860)
因为刚开始我启动了一个，后来将路径改了，那么现在需要删除，如下： 
![这里写图片描述](https://img-blog.csdn.net/20160924120533827)
接着再执行上述命令： 
![这里写图片描述](https://img-blog.csdn.net/20160924120628830)
### **4、启动MySQL服务**
#### 方法一：
启动服务命令为：net start mysql,但是有可能出问题，如下： 
![这里写图片描述](https://img-blog.csdn.net/20160924120813081)
原因分析： 
 1.检查my.ini配置是否出错。 
 2. 检查basedir和datadir对应的目录是否存在，如： 
 basedir = d:/mysql  
 datadir = d:/mysql/data/ 
注意无data文件，启动肯定会失败，我的就是这个问题，运行mysqld  –initialize，运行了这条命令后，mysql目录中就会出现一个data文件夹 
 3.检查Mysql目录有没有给系统的System用户文件读写权限（最好以管理员身份运行） 
 4. 检查mysql的3306端口是否被占用，如果占用，则修改my.ini配置文件中端口参数，将端口修改为其他值。 
 5. mysql的数据库文件遭到破坏
#### 方法二：
打开管理工具 服务，找到MySQL服务。 
通过右键选择启动或者直接点击左边的启动来启动服务。
### **5、登录MySQL**
服务启动后，就可以登陆了，输入mysql –u root –p (第一次没密码，直接回车过)，登录成功。如果没登录成功，如下： 
![这里写图片描述](https://img-blog.csdn.net/20160924121746359)
因在于5.7版本在安装时自动给了一个随机密码，坑爹的是在init步骤的时候不像linux系统会给出命令行提示，需要手动在mysql的data目录下搜索*.err，以文本形式打开才能看到如下内容，其中蓝色的就是密码： 
![这里写图片描述](https://img-blog.csdn.net/20160924121835361)
输入密码后，登录成功： 
![这里写图片描述](https://img-blog.csdn.net/20160924121938300)
### **6、修改root账号和密码**
连接权限数据库：mysql>use mysql 
修改密码：mysql>update user set password=password(“123456”) where user=’root’  
刷新权限：mysql>flush privileges
如果此时出现错误，无password字段，那是因为在5.7版本后，password 字段已经换成authentication_string这个字段了。那么上述命令就改成mysql>update user set authentication_string=password(“123456”) where user=’root’ 。
### **7、建立mysql快捷方式**
- 桌面右键->新建->快捷方式->对象位置输入：C:\Windows\System32\cmd.exe.快捷方式名称自己定义，确定，快捷方式建立成功。
- 右键单击刚才建立的快捷方式->属性->把目标一栏修改成MySQL启动参数:C:\Windows\System32\cmd.exe “E:\mysql\bin” /k mysql -uroot -p123456 mysql。解释:CMD路径 “MySQL路径bin目录” /k mysql -u用户名 -p密码 数据库名。（注意最后参数，数据库名可以看上面的show databases，框里面的显示结果）
- 修改完成后点击确定保存，直接双击快捷方式即可连接到MySQL数据库
### **8、安装navicat for mysql**
我们习惯了图形界面工具，navicat for mysql就是做好的操作mysql的可视化工具，下载地址:[MySQL下载](http://navicatformysql.en.softonic.com/)，下载好了就安装，安装后，打开navicat for mysql,进行连接，此时可能出现如下错误，而控制台命令行有可以登录： 
![这里写图片描述](https://img-blog.csdn.net/20160924124211814)
解决办法：修改下密码就好了。 
SET PASSWORD = PASSWORD(‘root’);
### **9、使用navicat for mysql**
连接上了，就可以建库，建表了，都是图像界面，点鼠标就好，比如我自己建了一个dancedb库和一个stuinfo表： 
![这里写图片描述](https://img-blog.csdn.net/20160924124650176)
### **10、忘记root密码**
- 关闭正在运行的MySQL。
- 打开DOS窗口，转到mysql\bin目录。 
- 输入mysqld-nt –skip-grant-tables回车。如果没有出现提示信息，那就对了。 
- 再开一个DOS窗口（因为刚才那个DOS窗口已经不能动了），转到mysql\bin目录。 
- 输入mysql回车，如果成功，将出现MySQL提示符 > 
- 连接权限数据库>use mysql; (>是本来就有的提示符,别忘了最后的分号) 
- 改密码：> update user set password=password(“123456”) where user=”root”; (别忘了最后的分号) 
- 刷新权限（必须的步骤）>flush privileges; 
- 退出 > \q 
- 注销系统，再进入，开MySQL，使用用户名root和刚才设置的新密码123456登陆。
