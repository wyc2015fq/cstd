# Zabbix二次开发_03api列表_中文版 - DoubleLi - 博客园






基于ZABBIX 3.0 https://www.zabbix.com/documentation/3.0/manual/api/reference

## 参考方法


本节提供了的zabbix提供的功能的概述  的API  ，并会帮助你发现周围可用的类和方法的路。


### 监控


该的zabbix  API  允许你访问历史和监控过程中收集其它数据。


#### 历史


检索进行演示或进一步加工的zabbix监控程序收集到的历史价值。

[历史API](https://www.zabbix.com/documentation/3.0/manual/api/reference/history)


#### 活动


检索触发器，网络发现和更灵活的管理情况或第三方工具集成其他的zabbix系统生成的事件。

[事件API](https://www.zabbix.com/documentation/3.0/manual/api/reference/event)


#### 服务监控


检索有关任何IT服务的详细服务层的可用性信息。

[IT服务SLA计算](https://www.zabbix.com/documentation/3.0/manual/api/reference/service/getsla)


### 组态


该的zabbix  API  允许你管理你的监控系统的配置。


#### 主机和主机组


管理主机组，主机和一切与之相关的，包括主机接口，主机宏和维护周期。

[主机API](https://www.zabbix.com/documentation/3.0/manual/api/reference/host)  | [主机组API](https://www.zabbix.com/documentation/3.0/manual/api/reference/hostgroup)  | [主机接口（API）](https://www.zabbix.com/documentation/3.0/manual/api/reference/hostinterface)  | [用户宏API](https://www.zabbix.com/documentation/3.0/manual/api/reference/usermacro)  | [维护API](https://www.zabbix.com/documentation/3.0/manual/api/reference/maintenance)


#### 项目和应用程序


定义项目进行监控。创建或删除应用程序，并指定项目给他们。

[项目API](https://www.zabbix.com/documentation/3.0/manual/api/reference/item)  | [应用API](https://www.zabbix.com/documentation/3.0/manual/api/reference/application)


#### 触发器


配置触发器来通知您在您的系统问题。管理触发器的依赖关系。

[触发API](https://www.zabbix.com/documentation/3.0/manual/api/reference/trigger)


#### 图


编辑图形或单独的图形物品收集的数据更好的表现。

[图形API](https://www.zabbix.com/documentation/3.0/manual/api/reference/graph)  | [图形API项目](https://www.zabbix.com/documentation/3.0/manual/api/reference/graphitem)


#### 模板


管理模板，并将其链接到主机或其他模板。

[模板API](https://www.zabbix.com/documentation/3.0/manual/api/reference/template)


#### 出口和进口


配置备份，迁移或大规模配置更新导出和导入的zabbix配置数据。

[配置API](https://www.zabbix.com/documentation/3.0/manual/api/reference/configuration)


#### 低层次的发现


配置低级别发现规则以及项目，触发器和图形原型监测动态实体。

[LLD规则API](https://www.zabbix.com/documentation/3.0/manual/api/reference/discoveryrule)  | [项目原型API](https://www.zabbix.com/documentation/3.0/manual/api/reference/itemprototype)  | [触发protototype API](https://www.zabbix.com/documentation/3.0/manual/api/reference/triggerprototype)  | [图原型API](https://www.zabbix.com/documentation/3.0/manual/api/reference/graphprototype)  | [主机原型API](https://www.zabbix.com/documentation/3.0/manual/api/reference/hostprototype)


#### 屏幕


编辑全局和模板级屏幕或各屏幕单独的项目。

[屏幕API](https://www.zabbix.com/documentation/3.0/manual/api/reference/screen)  | [屏幕项API](https://www.zabbix.com/documentation/3.0/manual/api/reference/screenitem)  | [模板屏幕API](https://www.zabbix.com/documentation/3.0/manual/api/reference/templatescreen)  | [模板屏幕项目API](https://www.zabbix.com/documentation/3.0/manual/api/reference/templatescreenitem)


#### 操作和提醒


定义操作和操作通知某些事件的用户或自动执行远程命令。获取有关生成的警报和接收信息。

[操作API](https://www.zabbix.com/documentation/3.0/manual/api/reference/action)  | [警报API](https://www.zabbix.com/documentation/3.0/manual/api/reference/alert)


#### IT服务


管理IT服务，服务水平监控和检索有关的任何服务的详细信息SLA。

[IT服务API](https://www.zabbix.com/documentation/3.0/manual/api/reference/service)


#### 地图


配置映射到创建IT基础架构的详细动态表示。

[地图API](https://www.zabbix.com/documentation/3.0/manual/api/reference/map)


#### 网络监控


配置Web方案来监控您的Web应用程序和服务。

[网络情景API](https://www.zabbix.com/documentation/3.0/manual/api/reference/httptest)


#### 网络发现


管理网络级发现规则来自动发现和监控新主机。获得有关发现的服务和主机充分获得信息。

[发现规则API](https://www.zabbix.com/documentation/3.0/manual/api/reference/drule)  | [发现检查API](https://www.zabbix.com/documentation/3.0/manual/api/reference/dcheck)  | [发现主机API](https://www.zabbix.com/documentation/3.0/manual/api/reference/dhost)  | [发现服务API](https://www.zabbix.com/documentation/3.0/manual/api/reference/dservice)


### 行政


随着的zabbix  API  ，你可以改变你的监控系统的管理设置。


#### 用户


添加将有机会获得的zabbix，将其分配给用户组，并授予权限的用户。配置介质类型和方式的用户会收到通知。

[用户API](https://www.zabbix.com/documentation/3.0/manual/api/reference/user)  | [用户组API](https://www.zabbix.com/documentation/3.0/manual/api/reference/usergroup)  | [介质类型API](https://www.zabbix.com/documentation/3.0/manual/api/reference/mediatype)  | [媒体API](https://www.zabbix.com/documentation/3.0/manual/api/reference/usermedia)


#### 一般


更改某些全局配置选项。

[图标地图API](https://www.zabbix.com/documentation/3.0/manual/api/reference/iconmap)  | [图像API](https://www.zabbix.com/documentation/3.0/manual/api/reference/image)  | [用户宏API](https://www.zabbix.com/documentation/3.0/manual/api/reference/usermacro)


#### 代理


管理您的分布式监控安装中使用的代理服务器。

[代理API](https://www.zabbix.com/documentation/3.0/manual/api/reference/proxy)


#### 脚本


配置和执行脚本来帮助你与你的日常工作。

[脚本API](https://www.zabbix.com/documentation/3.0/manual/api/reference/script)


### API信息


检索的zabbix版本的  API  ，使您的应用程序可以使用特定版本的功能。

[API信息API](https://www.zabbix.com/documentation/3.0/manual/api/reference/apiinfo)










