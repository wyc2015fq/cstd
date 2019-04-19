# MognoDB3.4.2用户访问配置管理 - =朝晖= - 博客园
# [MognoDB3.4.2用户访问配置管理](https://www.cnblogs.com/dhcn/p/7130748.html)
       说在前面，官方文档似乎略有瑕疵。
       说一下大规则：把绑定IP换成127.0.0.1IP之后，把security的authorization关闭掉做用户添加操作。
       添加用户的方法必须是createUser了，还必须加role项目。
       db和用户权限数据改完，再改配置文件，启用 authorization ，重新启动mongod服务

