# Django error信息邮件通知功能配置部署 - =朝晖= - 博客园
# [Django error信息邮件通知功能配置部署](https://www.cnblogs.com/dhcn/p/7124663.html)
1、 最近QA成为项目的重点，除了突破依赖外部表的阻力开始启用单元测试，还有一点就是对在线的生产服务的错误信息启用这个邮件通知功能，下面简单说一下启用方法：
1、发送邮件账号配置配置：
- EMAIL_HOST = 'smtp.smthserver.com'  
- EMAIL_PORT = '25'  
- EMAIL_HOST_USER = 'no-reply@mydomain.com'  
- EMAIL_HOST_PASSWORD = 'mailpassword'  
2、ADMINS配置：
- ADMINS = [('John', 'john@example.com'), ('Mary', 'mary@example.com')]  
3、SERVER_EMAIL配置
- SERVER_EMAIL = 'no-reply@mydomain.com'  
4、如果你做了自定义的logging配置，这个时候需要在logging中添加mail hander，并把它加到error信息的handlers列表中。
5、敏感变量信息屏蔽，对于view级的敏感信息屏蔽用装饰器@sensitive_variables，对于全局需要自定义ExceptionReporterFilter,看了一下系统默认的SafeExceptionReporterFilter的实现，最简单的自定义实现方法是改写那个reporter模板。
