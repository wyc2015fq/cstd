# AutoLine开源平台常见问题解答 - zhusongziye的博客 - CSDN博客





2018年05月20日 16:39:46[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：162








# AutoLine开源平台常见问题解答




## 为什么别人部署安装AutoLine开源平台没碰到各种问题，我一安装部署使用就各种问题呢？

答：
- 
请及时访问https://github.com/small99/AutoLine, 更新最新的代码

- 
请严格遵循安装部署教程进行，不要遗漏任何命令

- 
有问题，请说明Python版本、OS版本以及截图和日志提交到github上





## AutoLine开源平台最低Python版本要求是多少？

答：AutoLine基于Python3开发，不兼容Python2，最低要求Python3.4版本




## AutoLine开源平台支持哪些类型的自动化测试？

答：支持Web UI、App UI（android & IOS）以及HTTP接口测试




## 怎么安装AutoLine开源平台的依赖包？

答： 在AutoLine根目录下运行 pip install  -r requirements.txt即可




## 运行python manage.py db init命令提示如下信息时怎么处理？

![](https://img-blog.csdn.net/20180520163902414?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




答：不需要你编辑该文件，请忽略该提示即可。




## 运行python manage.py db init命令提示：migrations目录已存在时，怎么处理？

答：请删除migrations目录，再执行python manage.py db init




## 在创建测试步骤时，无法加载关键字列表怎么办？

答：你的代码是旧的，请更新最新的代码




## 我想支持不同系统，不同浏览器怎么办？

答：下载对应系统的对应浏览器驱动，放在bin目录下即可




## 运行AutoLine开源平台提示数据库相关的错误时怎么办？

答：
- 
请确保MySQL服务已经启动

- 
请确保你的MySQL账号和密码具备远程访问权限

- 
请确保你创建的MySQL数据库使用了UTF- 8编码

- 
请确保.env里关于数据库的连接字符串是正确的





## AutoLine开源平台是否支持分布式测试执行？

答：支持，需要配合selenium-grid使用，同时需要在写测试步骤时，使用对应的关键字配合




## AutoLine开源平台如何支持自动调度的？

答：AutoLine通过APScheduler模块，支持cron表达式方式进行自动调度，具体cron表达式使用请参见公众号里的简明教程




## AutoLine开源平台支持哪些关键字？

答：支持RobotFramework所有标准的关键字，同时支持SeleniumLibrary、Requests、Appium这三个扩展库的所有关键字。




## AutoLine开源平台的任务一直显示运行中，无法结束怎么办？

答：这个可能是调度过程出了异常无法正常生成测试报告，请忽略即可，后续会进一步进行优化以确保状态正常




## 我可以修改AutoLine开源平台构进行二次发布吗？

答：请尽量在保留许可声明的情况下进行二次发布，如果不想保留，那请随意了

[AutoLine开源平台发布](http://mp.weixin.qq.com/s?__biz=MzI0NDQ5NzYxNg==&mid=2247484940&idx=1&sn=b1c4c7976f6d99b1b66894b5e47d5f60&chksm=e95dabb3de2a22a5c1bcf01c49ad112215f26ae9ec89e816cbcbc26fa63fbc0550e007b90d0b&scene=21#wechat_redirect)


[AutoLine开源平台安装部署教程](http://mp.weixin.qq.com/s?__biz=MzI0NDQ5NzYxNg==&mid=2247484944&idx=1&sn=1be06a61bb63ea486b05280a55b9aa53&chksm=e95dabafde2a22b95c83f1dcaa620d8ac30ad1bf4d90f16b767f9466e36da3f0d88d038c7901&scene=21#wechat_redirect)


[AutoLine开源平台简明教程](http://mp.weixin.qq.com/s?__biz=MzI0NDQ5NzYxNg==&mid=2247484999&idx=1&sn=50d8f28150487643b4d96b8f93ce2358&chksm=e95dabf8de2a22eec82580650bb5eaab0588409906187ec512a4c447f4fdc4ea7f0bb4f80c04&scene=21#wechat_redirect)



