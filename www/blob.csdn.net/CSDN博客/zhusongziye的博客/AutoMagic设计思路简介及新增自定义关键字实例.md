# AutoMagic设计思路简介及新增自定义关键字实例 - zhusongziye的博客 - CSDN博客





2018年04月04日 21:06:40[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：345










目录

    简介

    AutoMagic介绍

    SeleniumKey介绍

    自定义关键字




# 简介

AutoMagic 是一个基于WebUI的自动化管理平台。为什么叫AutoMagic呢？因为自动化在执行起来的时候是一个很神奇的事情，它可以无人值守的模拟人的操作，就像魔术（Magic）一样。 所以我给她取名叫AutoMagic。

### AutoMagic从其工作分工应该分两部分：
- 
Web管理平台，它是基于Python Django框架+Mysql开发，主要用来管理自动化测试用例和设计自动化测试场景、存放界面元素标识，通过界面录入存放在数据库中进行管理，它是一个纯粹基于Selenium关键字自动化原件管理平台。

- 
客户端执行脚本（seleniumkeyword ），AutoMagic的执行效果展示主要靠它来体现，它的执行依附于web平台存储的用例和场景数据，seleniumkeyword可以不需要部署在服务器端，只要它能够连接到服务器端的数据库，部署在任何平台都可以执行。（这样也方便我们做分布式执行测试用例）





# AutoMigc简介

自动化测试管理平台，可以进行用户、产品、项目、模块、测试用例、测试元素、测试关键字、测试报告的管理等。

## 平台结构设计

![](https://img-blog.csdn.net/20180404210454407?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


## 用例关系

![](https://img-blog.csdn.net/20180404210508755?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




## 平台页面展示

![](https://img-blog.csdn.net/20180404210527392?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 用例执行log展示

![](https://img-blog.csdn.net/20180404210541208?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




# Seleniumkeyword介绍

seleniumkword是客户端执行脚本，AutoMagic的执行效果展示主要靠它来体现，它的执行依附于web平台存储的用例和场景数据，seleniumkeyword可以不需要部署在服务器端，只要它能够连接到服务器端的数据库，在本地部署即可执行。（这样也方便我们做分布式执行测试用例）

## 安装运行环境

在安装python、selenium、 mysqldb 的环境下运行

## 目录脚本说明

TestSuite.py 测试用例执行脚本引擎是 ，它可以通过参数实现不同的执行方式

> 
-t [taskid] 指定执行的AutoMagic任务


> 
-u [userid] 指定执行脚本的用户


> 
-r [runid] 执行测试用例脚本要同步的TestRailRunid，它只有在执行 -t 参数时才会生效。


> 
-c [caseid] 指定要执行的单个用例的编号


> 
-p [projectid] 指定要执行的项目id，会依次执行项目中所有模块和用例状态是启用的所有用例


> 
-b [browser] 指定执行用例过程所使用浏览器，默认使用chrome浏览器,也可在执行case过程中通过openbrowser关键字重置


> 
-v [1] 指定是否启用录屏功能（仅支持Linux系统，需要安装recordmydesktop录屏软件）


> 
-d [1] 钉钉消息接口，启用后可以将测试结果发送的钉钉群中


Exp:

`python TestSuite.py -t 1 -u tsbc -r 1433 -b chrome -v 1`

CustomKeyword.py 是我们自定义关键字维护脚本。

> 
自定义关键字示例：


![](https://img-blog.csdn.net/20180404210602767?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

AddCase.py 同步用例到TestRail到脚本

> 
-t [taskid] 指定执行同步的AutoMagic任务


> 
-u [userid] 指定执行脚本的用户


> 
-s [sctionid] 执行同步case到TestRail的那个用例集（section_id代表所在用例集）


HTMLTestRunner.py 生成报告的脚本文件

RestApiUtil.py、RestApiUtil.py 是发包MW模拟安全事件和发送pcap发包接口

testrail.py TestRail的接口API

result目录： 为报告接口目录

> 
在生成的每日报告目录中会生成用例执行过程日志文件


data目录： 执行上传附件关键字，附件存放目录

sendlog目录： 通过tcp、udp 514 发送Syslog 的应用




## 关键字创建分为两部分：
- 
automagic平台关键字

- 
seleniumkeyword中的关键字 两者是一一对应的，界面添加关键字名称和描述，后台写关键字实现方法，缺一不可。


平台已经内置的一些关键字写在 Base.py中。

前台可以使用该sql文件进行导入 autoplat_keyword.sql

自定义的关键字 CustomKeyword.py 文件中，可以仿照以下方式进行自定义：
@Action.add_action('InputText')defaction_InputText(action_object, step_desc, value, loc):	"""	文本框输入内容	:param action_object:	:param step_desc:	:param value: text	:param loc:	:return:"""print loc, value
	action_object.send_keys(loc, value)



github地址： https://github.com/radiateboy/automagic




> 
作者： Ray


博客地址：http://www.cnblogs.com/tsbc/

作者Email: tsbc@vip.qq.com

声明：本文转载，著作权归作者所有。








