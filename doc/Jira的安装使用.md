# Jira的安装使用



## 1.什么是JIRA

JIRA是目前比较流行的基于Java架构的管理系统，由于Atlassian公司对很多开源项目实行免费提供缺陷跟踪服务，因此在开源领域，其认知度比其他的产品要高得多，而且易用性也好一些。同时，开源则是其另一特色，在用户购买其软件的同时，也就将源代码也购置进来，方便做二次开发。JIRA功能全面，界面友好，安装简单，配置灵活，权限管理以及可扩展性方面都十分出色。

## 2.JIRA的主要功能

● 问题追踪和管理：用它管理项目，跟踪任务、bug、需求，通过jira的邮件通知功能进行协作通知，在实际工作中使工作效率提高很多

● 问题跟进情况的分析报告：可以随时了解问题和项目的进展情况

● 项目类别管理功能：可以将相关的项目分组管理

● 组件/模块负责人功能：可以将项目的不同组件/模块指派相应的负责人，来处理所负责的组件的Issues

● 项目email地址功能：每个项目可以有不同的email（该项目的通知邮件从该地址发出）

● 无限制的工作流：可以创建多个工作流为不同的项目使用

# 3.JIRA的安装

## 3.1.JIRA相关数据库安装（MySQL）

1、  安装.net framework

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809165409730-17813904.png)

 

2、  安装Mysql

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809165435339-2022239646.png)

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809165454620-461366773.png)

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809165506011-1837550683.png)

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809165513167-670804910.png)

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809165520542-67059131.png)

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809165530464-1461520094.png)

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809165544277-1246733850.png)

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809165552199-100777311.png)

 

3、  创建数据库jiradb，选择语言UTF-8 

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809165601339-369205579.png)

注意：不可更新

## 3.2.JIRA安装方法

1、点击Next

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809165650527-584776234.png)

2、选择端口：默认8080，本次安装8091

3、  配置数据库mysql

配置路径：C:\Program Files\Atlassian\Application Data\JIRA下的dbconfig.xml

4、运行jira web

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809165714870-824949634.png)

 

## 3.3.JIRA 破解

1、  运行注册机：keygen.bat

或者：java  –jar  jira_keygen.jar

**注意：要求****jdk****在****1.6****以上**

**![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809165850261-518536369.jpg)**

 

输入name 和server ID:

ServerID 为安装页面显示的

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809165908386-813065084.png)

2、  点击.patch

把jiar4.2.1-crack的atlassian-extras-2.2.jar导入,

3、  stop jira

4、  拷贝注册机目录的atlassian-extras-2.2.jar到jira的工程目录：D:\Program Files\Atlassian\JIRA\atlassian-jira\WEB-INF\lib 替换原 atlassian-extras-2.2.jar

5、  点击.gen获得授权码

6、  启动jira

7、  登录jira

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809165924449-1286241891.png)

 

## 3.4.JIRA中文插件安装

\1. 停止JIRA服务

\2. 下载插件文件 jira-lang-zh_CN-4.4.jar，将插件复制到JIRA安装路径下 WEB-INF/lib

i. 将文件名称修改为 jira-lang-zh_CN-JIRA版本号.jar。

ii. 如果你使用的是JIRA4.4.1, 就将文件名修改为jira-lang-zh_CN-4.4.1.jar

\3. JIRA 系统插件中文文件:

i. 下载并解压 atlassian-bundled-plugins_4.4.zip

ii.用winRAR打开 WEB-INF/classes/atlassian-bundled-plugins.zip

iii.将附件中的中文文件加入至 WEB-INF/classes/atlassian-bundled-plugins.zip 对应的.jar包中对应的文件夹中。 保存并推出winRAR

\4. 重新启动JIRA服务

## 3.5.JIRA与SVN集成

\1. 下载当前svn插件：

A.方式一：访问管理员界面：插件

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809170215433-2137180379.png)

​       搜索subversion：如下图下载

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809170227417-1778029925.png)

B. 方式二：http://repository.atlassian.com/atlassian-jira-subversion-plugin/distributions/ 选择对应版本下载

 

\2. Svn插件集成：（atlassian-jira-subversion-plugin-0.10.7-distribution）

A.将atlassian-jira-subversion-plugin-0.10.7-distribution 下的Lib下所有jar包拷贝到jira的web-inf下的lib（D:\Program Files\Atlassian\JIRA\atlassian-jira\WEB-INF\lib）

B.修改atlassian-jira-subversion-plugin-0.10.7-distribution下的属性文件：subversion-jira-plugin.properties：

C.将subversion-jira-plugin.properties拷贝到如下文件：D:\Program Files\Atlassian\JIRA\atlassian-jira\WEB-INF\classes

D.修改subversion-jira-plugin.properties文件：内容如下

svn.root=http://10.1.1.237:8090/svn/svndemo

svn.display.name=Sample Repository

svn.username=test

svn.password=test

\#多SVN工程目录需要配置多个目录

svn.root.1=http://10.1.1.237:8090/svn/svndemo1

svn.display.name.1=Sample Repository1

svn.username.1=test

svn.password..1=test

 **注意：存在多个svn****的project****时，需要在此配置多个svn****的project****目录，且在jira****的subversion****的配置页面，也许配置多个服务**

E.重启jira，查看如下图：系统-高级配置-服务

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809170429199-823434200.png)

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809170437152-1436910128.png)

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809170444605-929098855.png)

 

## 3.6.JIRA与SVNWebClient

\1. 下载并安装SVNWebClient：svnwebclient-3.1.0.zip

A.解压svnwebclient-3.1.0.zip，下载地址如下：http://www.polarion.com/products/svn/svn_webclient.php

B.拷贝\svnwebclient-3.1.0文件下的svnwebclient.war到JIRA的Webapp目录下D:\Program Files\Atlassian\JIRA\webapps

C.监测是否成功：浏览器输入http://localhost:8091/svnwebclient/

单svn工程目录如下：

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809170558199-1678639992.png)

 

多svn工程目录如下：

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809170608042-1050415416.png)

 

\2. SVN高亮显示插件安装：

下载安装配置EnScript，地址为：<http://gnuwin32.sourceforge.net/packages/enscript.htm>

A.解压enscript-1.6.3-9-bin.zip，将其复制到某目录D:\Program Files\Atlassian

B.配置svnwebclient的web.xml，目录地址如下：D:\Program Files\Atlassian\JIRA\webapps\svnwebclient\WEB-INF

```
添加enscript的配置
<context-param>
             <param-name>EnscriptPath</param-name>
             <param-value>
           D:\Program Files\Atlassian\enscript-1.6.3-9-bin\bin\enscript.exe
             </param-value>
             </context-param>
                       修改svn的配置
                      <!—单工程目录->
     <context-param>
       <param-name>RepositoryUrl</param-name>
       <param-value>http://localhost:8090/svn/svndemo</param-value>
     </context-param>
                      <!—多工程目录->
     <context-param>
        <param-name>ParentRepositoryDirectory</param-name>
        <param-value>http://10.1.1.237:8090/svn/</param-value>
    </context-param>
 
      修改SVN的用户名和密码
     <context-param>
       <param-name>Username</param-name>
       <param-value>test</param-value>
     </context-param>
 
<context-param>
     <param-name>Password</param-name>
     <param-value>test</param-value>
</context-param>
      修改SVNWebClient的字符集
    <context-param>
        <param-name>DefaultEncoding</param-name>
        <param-value>UTF-8</param-value>
    </context-param>
    <context-param>
        <param-name>ZipEncoding</param-name>
        <param-value>UTF-8</param-value>
    </context-param>
对于部分SVN的中文注释提交乱码的情况配置：加入GBK
<context-param>
        <param-name>CharacterEncodings</param-name>
        <param-value>
  windows-1250,windows-1251,windows-1252,windows-1253,windows-1254,
             windows-1255,windows-1256,windows-1257,windows-1258,windows-31j,
             UTF-8,UTF-16LE,UTF-16BE,UTF-16,
             ISO-8859-1,ISO-8859-13,ISO-8859-15,ISO-8859-2,ISO-8859-3,ISO-8859-4,ISO-8859-5,ISO-8859-6,
             ISO-8859-7,ISO-8859-8,ISO-8859-9,GBK,
             US-ASCII,KOI8-R
        </param-value>
    </context-param>                
```

C.配置jira自带的TOMCat，修改conf/Server.xml （目录如下）D:\Program Files\Atlassian\JIRA\conf

```
       配置如下：
```

​    <Service name="Catalina">

​        <Connector port="8091"

URIEncoding="UTF-8"

maxThreads="150"

maxHttpHeaderSize="8192"

minSpareThreads="25"

maxSpareThreads="75"

useBodyEncodingForURI="false"

enableLookups="false"

protocol="HTTP/1.1"

redirectPort="8443"

acceptCount="100"

connectionTimeout="20000"

disableUploadTimeout="true" />

\3. Jira中配置SVN插件：

A. 管理员登入JIRA系统界面

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809170945745-1973755332.png)

 

B.选择Subversion Repositories,点击编辑或增加

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809170957870-2071124814.png)

 

C.修改其配置文件

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171009308-310980717.png)

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171018417-547537578.png)

 

D. 更新保存

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171040542-2076986663.png)

 

\4. 修复高亮显示的问题  ：

将安装目录下的jhighlight-1.0.jar替换D:\Programe Files\Atlassian\JIRA\webapps\svnwebclient\WEB-INF\lib下的jhighlight-1.0.jar

如下图：SVN乱码时可选择GBK

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171108699-583484386.png)

 

## 3.7.JIRA与SVN使用方法

方法一：

\1. 创建一个问题

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171137495-1719754846.png)

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171144808-1227405592.png)

 

\2. 利用SVN客户端提交代码或文档关联问题或任务（TortoiseSVN）

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171204777-300792087.png)

Message填写问题的KEY（TEST-2）

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171220261-708115949.png)

注释：如果没有实时更新，修改其更新时间

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171232886-1206536899.png)

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171242058-1694454576.png)

 

\3. SVN权限控制

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171259308-1158275071.png)

点击Default Permission Scheme

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171319089-1998407138.png)

 

方法二：在SVN端对项目添加属性文件的方式

进入本地下载的SVN库中，利用TortoiseSVN客户端中选择TortoiseSVN—>Properties，

修改属性内容：

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171331605-824982576.png)

修改之后提交内容，在Jira Issue输入框中填写所提交内容对于JIRA中的问题的编号，在Message中输入相应的备注信息，点击OK：

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171341449-1094091422.png)

然后在JIRA中即可查看到相应的版本内容：

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171352870-927731018.png)

## 3.8.JIRA与Eclipse使用方法

\1. Eclipse中JIRA插件的安装

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171417136-1459040636.png)

点击添加，输入URL：http://update.atlassian.com/atlassian-eclipse-plugin/3.0.0/e3.5

\2. Eclipse中使用JIRA插件说明：

首先添加仓库：

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171441839-1797042768.png)

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171541917-1372287372.jpg)

然后新建查询：

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171703433-626596460.jpg)

最后任务列表：

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809171720652-844492609.png)

 

## 3.9.常见问题

\1. SVN权限控制：本地访问成功，远程访问不成功

修改系统管理员界面的通用设置，及关闭相关防火墙

\2. 登录webclient报错

javax.servlet.ServletException: org.polarion.svnwebclient.SVNWebClientException: org.polarion.svnwebclient.data.DataProviderException: org.polarion.svnwebclient.data.javasvn.SVNRepositoryPool must be initialized before first usage

​       at org.apache.jasper.runtime.PageContextImpl.doHandlePageException(PageContextImpl.java:865)

​       at org.apache.jasper.runtime.PageContextImpl.handlePageException(PageContextImpl.java:794)

​       at org.apache.jsp.directoryContent_jsp._jspService(directoryContent_jsp.java:503)

​       at org.apache.jasper.runtime.HttpJspBase.service(HttpJspBase.java:70)

​       at javax.servlet.http.HttpServlet.service(HttpServlet.java:717)

​       at org.apache.jasper.servlet.JspServletWrapper.service(JspServletWrapper.java:386)

 解决方法：

Web.xml

<context-param>

​        <param-name>ParentRepositoryDirectory</param-name>

​        <!--param-value>http://svn.polarion.org/repos/community</param-value-->

​        <param-value>http://10.1.1.237:8090/svn/</param-value>

</context-param>

subversion-jira-plugin.properties

svn.root=http://10.1.1.237:8090/svn/

**注意：修改时候先****stop jira****服务，使得修改内容可以生效**

\3. SVN安装问题：

A.Could not open the requested SVN filesystem，原因及解决方法：

SVNParentPath  E:\svn_repository\project 或者

SVNParentPath  E:\svn_repository\

将其改为SVNParentPath  E:\svn_repository

 B.在project目录下建立new的文件夹：svn import -m "new import" E:\new [http://localhost：8090/svn/project](http://localhost:8090/svn/project)

\4. 集成后，定位SVN失败，location失败

修改jira 配置页面的配置文件，注意修改SVNwebClient的指向文件：下文红色注释的地方：

http://10.1.1.237:8091/svnwebclient/changedResource.jsp?location**=svndemo1**/&url=${path}&rev=${rev}&action=add

# 4.JIRA的相关配置

## 4.1.项目创建

1、项目创建

利用管理员权限登陆JIRA，进入管理员页面，项目-添加-添加一个新的项目：

包括项目名称及键值，及相应的项目负责人

其中项目键值为英文描述，项目创建人为默认的创建人(一般为项目经理)

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809172424558-1985363192.jpg)

2、项目类别划分

 ![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809172438089-525813657.png)

点击类别：选择项目所属类别，根据创建项目的类别进行选择。

目前公司的变更管理服务器JIRA中的项目类别包含：None、测试、研发、维护四种。

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809172124542-1474954339.png)

 

## 4.2. 用户角色的配置

### 4.2.1新增角色

利用管理员权限登陆JIRA，进入管理员页面，选择用户—>角色，点击进入页面

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809172209261-1856949928.png)

 

在“添加项目角色”模块填写名称及描述，点击“添加项目角色”即可。

目前公司的变更管理服务器JIRA中的项目角色包含：Administrator、Developers、Testers、Users四种角色。

### 4.2.2.新增用户组

利用管理员权限登陆JIRA，进入管理员页面，选择用户—>用户组，点击进入页面：

在下部的“添加用户组”模块填写用户组名称，点击添加。

目前公司的变更管理服务器JIRA中的用户组包含：jira-administrators、jira-developers、jira-project managers、jira-testers、jira-users五种。

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809173027136-1977230194.png)

 

### 4.2.3.新增用户

利用管理员权限登陆JIRA，进入管理员页面，选择用户—>用户，点击进入页面

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809172641261-1523369592.png)

点击右侧的“添加用户”，输入用户名、密码、确认密码、全名、邮箱等信息，点击创建。

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809172831120-2013940317.png)

 

## 4.3.全局权限的配置

利用管理员权限登陆JIRA，进入管理员界面，选择用户—全局权限，进入界面：

查看到JIRA的权限包含：JIRA系统管理员、JIRA管理员、JIRA用户、浏览用户、创建共享的对象、管理群组的过滤器订阅、批量更改共七种权限。从这其中权限后面的“查看用户”链接可以查看每个权限所对应的用户。

若想添加新的权限，则在底部选择相应的权限和用户组，点击添加即可。若想删除某个权限，则点击删除即可直接删除。

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809172936245-91460898.png)

 

当想要对某个用户设置权限时，具有两种方法：

方法一：可以通过在用户—>用户组界面中，将此用户加入此权限的用户组中。

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809173055777-265996670.png)

 

方法二：通过在用户—>用户界面中，找到此用户，点击后面的“编辑角色”，然后编辑项目角色，在相对应的项目下勾选想要赋予的权限，然后点击保存。

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809173111199-1441120386.png)

 

## 4.4.权限方案的配置

权限方案是允许管理员先建立一系列权限控制，并将权限方案应用到任何项目中。

利用管理员权限登陆JIRA，进入管理员页面，选择问题—>权限方案，进入页面：

通过点击底端的“添加权限方案”按钮填写所添加权限的名称及描述即可填写权限方案。再对项目设置权限方案时，点击创建的项目名称进入页面，在权限—>方案处即可修改。

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809173237699-740207645.png)

## 4.5.问题类型方案的配置

利用管理员权限登陆JIRA，进入管理员页面，选择问题—>问题类型，进入页面：

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809173259667-581123587.png)

若想添加新的问题类型，则在底部的“添加新的问题类型”模块填写名称、描述、类型、图标URL等信息，点击增加即可。

目前公司的变更管理服务器JIRA中的问题类型包含：Bug、Improvement、New Feature、Task、TestCase、Sub-task、Sub-testcase七种，中文名称分别为：缺陷、改进、新功能、任务、测试用例、子任务、子测试用例，其中前五种都是标准问题类型，后两种是子任务问题类型。

## 3.6.        问题属性的配置

\1. 问题状态

利用管理员权限登陆JIRA，进入管理员页面，选择问题—问题属性—状态，进入页面：

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809173419199-1299842276.png)

查看到问题状态包含：Open、In Progress、Reopened、Resolved、Closed五种，中文名称分别为：打开、正在处理、重新打开、已解决、关闭。

若想添加新的问题状态，则在底部填写新状态的名称及描述，点击增加即可。

 

\2. 问题解决结果

利用管理员权限登陆JIRA，进入管理员页面，选择问题—问题属性—解决结果，进入页面：

 ![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809173521558-1968537614.png)

查看到问题的解决结果包含：Fixed、Won’t Fix、Duplicate、Incomplete、Cannot Reproduce五种，中文名称分别为：已解决、无需解决、问题重复、不完整、无法重现。

若想添加新的问题解决结果，只需填写名称及描述，点击增加即可。

 

\3. 问题优先级

利用管理员权限登陆JIRA，进入管理员页面，选择问题—问题属性—优先级，进入页面：

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809173654136-232021990.png)

​         查看到问题的属性包含：Blocker、Critical、Major、Minor、Trivial五种，这五种属性的中文名分别为：紧急、严重、一般、次要、无关紧要，优先级由高到低，优先级越高问题越严重越应最早解决。

​         若想添加新的属性，只需填写名称、描述、图标URL、优先级颜色，点击增加即可。

## 4.7.工作流的配置

利用管理员权限登陆JIRA，进入管理员页面，选择问题—工作流，进入页面：

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809173745402-1902396552.png)

查看到工作流的状态包含：已生效、草稿、未生效三种。每一种工作流都至少具有这三种状态中的一个。

若想添加新的工作流，则在底部填写新工作流的名称、描述，点击增加即可，或者直接从XML文件直接导入工作流。

若想删除一个工作流，则必须先从工作方案中将此工作流删除，然后才能在删除。

## 4.8.界面的配置

利用管理员权限登陆JIRA，进入管理员页面，选择问题—界面，进入界面：

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809173929777-406682114.png)

界面是对字段的排列布局，是创建问题、编辑问题或执行工作流过程时显示的页面。公司变更管理服务器JIRA中的界面主要包含：Default Screen、Resolved Issue Screen、Workflow Screen三种界面。

若添加新的界面则只需填写名称及描述，点击增加即可。

## 4.9.字段的配置

利用管理员权限登陆JIRA，进入管理员页面，选择问题—字段，进入页面：

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809173936089-1917189768.png)

公司的变更管理服务器JIRA中的自定义字段包含：实际结果、期望结果、测试结果、输入条件、预期结果五种。

若想添加新的字段，则点击“添加自定义字段”链接，在跳转后的页面中先选择字段类型，然后填写新增字段的项目明细信息，点击完成即可。

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809173948620-1571910903.png)

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809174015074-245427653.png)

 

# 5.JIRA的备份与恢复

## 5.1.JIRA的备份数据

JIRA中自带数据备份，利用管理员权限登陆JIRA，进入管理员界面，在管理员界面中包含一个“备份数据”链接，进入之后输入相应的文件名，要求备份成XML文件，可以指定文件的具体存放位置，如果不指定则默认存放在C:\Windows\system32下，最后点击“备份”按钮即可完成。

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809174040824-1844370390.png)

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809174102652-912230160.png)

 

## 5.2.JIRA的恢复数据

JIRA中自带对备份数据的恢复，利用管理员权限登陆JIRA，进入管理员页面，在管理员页面中包含一个“恢复数据”的链接，进入之后找到之前备份好的XML文件，点击恢复即可恢复JIRA的备份数据。

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809174218511-1232141149.png)

##  

## 5.3.JIRA的项目导入

若想要将JIRA的备份数据导入后成为一个新的项目，则可以利用管理员权限登陆JIRA，进入管理员页面，在此页面中包含“项目导入”链接，进入后直接找到备份文件的路径导入即可，但要求备份文件和附件路径必须在同一台JIRA实例服务器中 。

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170809174309964-1371550635.png)



分类: [配置管理](https://www.cnblogs.com/bumengru/category/1047152.html)

标签: [配置管理](https://www.cnblogs.com/bumengru/tag/%E9%85%8D%E7%BD%AE%E7%AE%A1%E7%90%86/), [缺陷管理](https://www.cnblogs.com/bumengru/tag/%E7%BC%BA%E9%99%B7%E7%AE%A1%E7%90%86/), [jira](https://www.cnblogs.com/bumengru/tag/jira/)

