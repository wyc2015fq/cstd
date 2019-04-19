# 数据库导入Excel数据的简易方法 - =朝晖= - 博客园
# [数据库导入Excel数据的简易方法](https://www.cnblogs.com/dhcn/p/7105498.html)
        当然，最糙猛的方式就是自己写程序读取Excel程序然后插进[数据库](http://lib.csdn.net/base/mysql)，但那种方式要求太高。说个简单方法，主流数据库的管理工具支持CSV文件格式数据向表导入，而Excel可以另存外CSV文件，这种导入就手工操作就Ok了，其中注意Excel另存以后的CSV文件编码如果和数据库表的默认字符编码不一样，可以用NotePad++之类的工具转换一下。
        关于大多数数据库管理工具的[测试](http://lib.csdn.net/base/softwaretest)已确认名单：[MySQL](http://lib.csdn.net/base/mysql)的PHPMyAdmin、SQLServer企业管理器、[Oracle](http://lib.csdn.net/base/oracle)的Oracel SQL Developer、PostgreSQL的pgAdminIII.

