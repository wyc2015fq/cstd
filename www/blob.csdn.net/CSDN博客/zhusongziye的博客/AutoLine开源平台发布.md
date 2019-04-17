# AutoLine开源平台发布 - zhusongziye的博客 - CSDN博客





2018年05月20日 16:35:58[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：282








# AutoLine开源平台
- 
AutoLine开源平台
- 
什么是AutoLine开源平台

- 
为什么要开发AutoLine

- 
采用了哪些开源技术/框架

- 
安装

- 
配置

- 
如何运行

- 
一些截图

- 
支持与联系

- 
许可

- 
公众号



## 什么是AutoLine开源平台

AutoLine开源平台是一个开源自动化测试解决方案，基于RobotFramework进行二次开发，支持RobotFramework几乎所有的库。

## 为什么要开发AutoLine
- 
为中小企业提供一个好用的自动化测试解决方案

- 
为广大软件测试从业人员提供一个基于开源框架的自动化测试平台开发示例

- 
业余写写，练习下敲代码的速度


## 采用了哪些开源技术/框架
- 
Python3

- 
Flask

- 
EasyUI

- 
Chartjs

- 
Robotframework

- 
Requests

- 
SQLAlchemy

- 
Appium-Python-Client

- 
Selenium3


等等其他及上述技术相关的插件或技术包

## 安装

下载AutoLine工程, 在AutoLine根目录下有requirements.txt文件，该文件包含了所有依赖的包，安装好Python3并确保Python3的pip可用，使用以下命令安装AutoLinie依赖库

> 
pip install -r requirements.txt


等待安装完成

## 配置

在AutoLine目录下的.env为所有关键的配置项

## 如何运行

在配置好上述配置后如何运行AutoLine？

一. 初始化数据库

> 
python manage.py db init


> 
python manage.py db migrate


> 
python manage.py db upgrade


二. 初始化数据

> 
python manage.py deploy


三. 运行

> 
python manage.py runserver


![](https://img-blog.csdn.net/20180520163213231?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

使用默认账号： autoline@126.com/123456 登录如图：

![](https://img-blog.csdn.net/20180520163249729?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 一些截图

产品管理 

![](https://img-blog.csdn.net/20180520163307699?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

项目管理 

![](https://img-blog.csdn.net/20180520163320639?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

调度管理 

![](https://img-blog.csdn.net/20180520163334219?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

查看任务 

![](https://img-blog.csdn.net/20180520163431220?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

测试报告 

![](https://img-blog.csdn.net/20180520163447549?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 支持与联系

公众号：开源优测

email：lymking#foxmail.com

知识星球：开源优测

关于开源测试技术相关文章首发公众号。

## 许可

AutoLine基于Apache License 2.0协议发布




github地址：点击阅读原文进入github地址

或是直接访问  https://github.com/small99/AutoLine



