# Selenium Python自动化测试环境搭建



# 一.需要使用的工具

1.浏览器（Firefox、Chrome、IE）

2.Python（Python2或Python3）

3.Selenium

4.浏览器驱动（geckodriver、chromedriver、IEDriverServer）

5.IDE（Eclipse、Pychram等）

6.数据库（MySQL、Oracle、Sql Server、DB2等均可）

7.插件（FireBug或浏览器开发模式下的对象识别工具）

8.数据库连接工具

9.Jenkins持续集成工具

 

# 二.浏览器及驱动安装

　　安装Firefox、Chrome、IE三个浏览器均可，注意想使用某个浏览器需要下载对应的浏览器驱动程序，同时驱动程序支持的版本需与浏览器版本对应；

1.Firefox：

　　下载地址：<http://ftp.mozilla.org/pub/firefox/releases/>

　　驱动地址：<https://github.com/mozilla/geckodriver/releases>

2.Chrome：

　　下载地址：网上下载

　　驱动地址：<http://chromedriver.storage.googleapis.com/index.html>

3.IE：

　　下载地址：系统自带

　　驱动地址：<http://selenium-release.storage.googleapis.com/index.html>

#  

# 三.Python安装

1.官网下载python3，并安装；

下载地址：<https://www.python.org/getit/>

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170036824-1795407585.png)

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170224111-1807882786.png)

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170232480-1986897951.png)

 

2.配置环境变量

在path中添加python的安装目录和Scripts的安装目录；

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170241470-1380111637.png)

 

3.验证安装

打开cmd命令行，输入python –version，若显示python的版本号则表示安装成功；

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170249089-935853555.png)

 

# 四.Selenium安装

Python环境搭建好了以后，安装selenium就简单了，

安装方法：

--打开cmd命令行

--输入命令 pip install selenium

--回车，等待自动安装结束

--当最后一行代码出现Successfully install selenium-XX时，表示安装成功。

#  

# 五.驱动存放

浏览器及驱动下载结束后，需要将对应的驱动拷贝至python安装目录下的Scripts目录下；

#  

# 六.Eclipse PyDev安装

本次以eclipse中安装pydev的方式为例说明：

1.安装PyDev：

打开eclipse后，在Help -- Eclipse Marketplace窗口输入pydev，直接搜索后点击install安装即可；

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170319042-1246160746.png)

 

若未搜索到或安装失败，可尝试手动安装，打开eclipse后，在Help -- Install New Software窗口输入：

Name：PyDev

Location：<http://pydev.org/updates>

注：下载Pydev自动安装,如果失败,则手动访问地址，然后关掉其他使用网络程序，重启eclipse再次安装！提示重启按照完毕！

 

2.eclipse和python关联：

打开eclipse，在 Windows -- Preferences --PyDev -- Interpreters -- Python Interpreter

点击New按钮，选择python.exe的路径，打开后显示出一个包含很多复选框的窗口，点击OK；

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170350326-1940001816.png)

 

3.测试python：

创建一个python项目，New -- Project -- PyDev -- PyDev Project，

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170404976-112856786.png)

输入项目名称，点击finish，

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170417287-1638458700.png)

 

在项目中先创建PyDev Package，再创建一个PyDev Module类，填写py文件名称；

输入代码：print(“this is my first python project!”)，鼠标右键Run As ->Python Run若出现执行结果则正常；

#  

# 七.测试打开浏览器

在IDE中输入代码：

\# -*- coding: utf-8 -*-

from selenium import webdriver

driver = webdriver.Firefox()

driver.get("https://www.baidu.com")

运行后若能在浏览器中打开百度首页则正常；

 

# 八.数据库安装

以MySql数据为例，下载安装包后安装，创建数据库及登录用户，创建表，用于存在自动化测试案例数据、期望输出和案例执行结果；

想要连接数据库，需要在cmd命令行中输入：pip install PyMySQL命令，安装pymysql工具：

在配置文件中添加数据库的基本信息：

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170505546-286037022.png)

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170514148-1256133419.png)

 

在源码中创建连接和操作数据库的工具类：

from framework.logger import Logger

import pymysql as mdb

import configparser

import os

 

logger = Logger("数据库连接").getlog()

config = configparser.ConfigParser()

dir = os.path.abspath('.').split('src')[0]

config.read( dir+"/config/config.ini") 

 

class database:

​    \# 数据库连接

​    def connectMySQL(self):

​        conn = False

​        try:

​            conn = mdb.connect(host=config.get("MySql", "dbhost"),

​                                   user=config.get("MySql", "userName"),

​                                   passwd=config.get("MySql", "passWord"),

​                                   db=config.get("MySql", "dbName") ,

​                                   port=int(config.get("MySql", "dbport")),

​                                   )

​        except Exception as data:

​            logger.error("connect database failed, %s" %data)

​            conn = False

​        return conn

 

​    \# 获取查询结果集

​    def fetch_all(self, sql):

​        self._conn = self.connectMySQL()

​        self._cursor = self._conn.cursor()

​        res = ''

​        if (self._conn):

​            try:

​                self._cursor.execute(sql)

​                res = self._cursor.fetchall()

​                \#print(res)

​            except Exception as data:

​                res = False

​                logger.error("query database exception, %s" %data)

​            finally:

​                self.close()

​        return res

 

​    \#更新数据库中表的某几列：update login set result = %s where id = %s

​    def updateByParams(self, sql, params):

​        self._conn = self.connectMySQL()

​        self._cursor = self._conn.cursor()

​        flag = False

​        if (self._conn):

​            try:

​                self._cursor.execute(sql,params)

​                self._conn.commit()

​                flag = True

​            except mdb.Error as data:

​                flag = False

​                logger.warn("update database by params exception, %s" % data)

​            finally:

​                self.close()

​        return flag

​    \# 关闭数据库连接

​    def close(self):

​            try:

​                if (type(self._cursor) == 'object'):

​                    self._cursor.close()

​                if (type(self._conn) == 'object'):

​                    self._conn.close()

​            except Exception as data:

​                logger.warn("close database exception, %s,%s,%s" % (data, type(self._cursor), type(self._conn)))

 

if __name__ == '__main__':

​    db = database()

​    sql = "select username,userpwd,expect from login"

​    db.fetch_all(sql)

​    update_sql = "update login set result =%s"

​    db.updateByParams(update_sql, '测试')

#  

# 九.Jenkins工具安装

1. 安装jdk，并配置环境变量，不做详细说明；
2. 下载maven，解压后配置环境变量，不做详细说明；
3. 下载Jenkins，下载地址：<https://jenkins.io/download/>，选择war包下载；
4. 下载tomcat，并解压；
5. Jenkins第一种启动方法：切换到jenkins.war存放的目录，输入如下命令：**java -jar jenkin.war**，然后在浏览器中输入http://localhost:8080，localhost可以是本机的ip，也可以是计算机名，就可以打开jenkins了；
6. Jenkins第二种启动方法：用tomcat打开，将jenkins.war文件放入tomcat下的webapps目录下，启动jenkins时会自动在webapps目录下建立jenkins目录，所以在地址栏上需要输入的地址于上一种方法有点不一样，输入：<http://localhost:8080/jenkins>，打开的jenkins页面与第一种方法相同；

#  

# 十.Jenkins工具使用

对使用tomcat的方法进行详细说明：

1.启动tomcat，浏览输入<http://localhost:8080/jenkins>

2.初始的管理员密码，jenkins是自动生成的，根据图中红色的路径找到密码，拷到下面的输入框中；

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170607955-1048017320.png)

 

3.安装插件，可以默认的安装，也可以自己选择，（jenkins配置好后也可以安装插件），这里我们选择默认的；

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170617094-1219076073.png)

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170627429-2004730793.png)

 

4.完成以后到创建自己的用户；

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170638892-2101222778.png)

5.创建以后就可以用新账户登录，进入jenkins的主页；

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170654893-2103037797.png)

 

6.选择“系统管理”--“插件管理”，选择所需的相关插件进行安装，若安装失败可能是Jenkins版本与插件版本不兼容，可以采取升级Jenkins的方式解决；

推荐安装的插件包括：Git、subversion、Deploy to container Plugin、Maven Integration Plugin、Email Extension Plugin等；

7.选择“系统管理”--“Global Tool Configuration" 配置 jdk、maven等信息；

点击“JDK安装”，配置jdk信息：

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170716118-1919915330.png)

 

点击“Maven安装“，配置Maven配置：

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170725389-826736602.png)

 

8.接下来我们“构建一个maven项目”：

点击“新建”--构建一个maven项目，

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170736302-1776274366.png)

 

填写项目名称等基本信息：

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170806564-1994551780.png)

 

若从svn或git获取项目文件，则添加svn信息或git信息，

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170824947-1159483886.png)

 

点击 “Add” 填写svn的账号和密码， 

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170832265-1030607043.png)

 

添加完成后如下：

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170840036-1141788855.png)

 

配置构建触发器，如下图配置为每天晚上9：30开始构建（Cron表达式）：

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170847099-1889538192.png)

 

添加构建后操作

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170852736-1466782878.png)

 

直接将构建好的项目打包部署到tomcat中，

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170858321-1484819420.png)

 

 添加tomcat容器，

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170906721-329904626.png)

 

 tomcat中要配置用户名和密码，且该用户还要有足够的权限，在tomcat-user.xml中配置的

```
<role rolename="admin" />
<role rolename="admin-gui" />
<role rolename="manager" />
<role rolename="manager-script" />
<role rolename="manager-gui" />
<user username="admin" password="123456" roles="admin,manager,manager-script,manager-gui,admin-gui"/>
用户名为admin 密码为 123456
```

在 My Views 能看到自己建的项目：

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170934854-852828733.png)

进入项目后，点击“立即构建”就会执行构建任务，jenkins就会从svn上将代码拉下来，打好包后然后部署到tomcat上。

![img](https://img2018.cnblogs.com/blog/1205757/201811/1205757-20181106170943648-1668659685.png)

 



分类: [自动化测试](https://www.cnblogs.com/bumengru/category/1047154.html)