# SSH - LC900730的博客 - CSDN博客
2017年08月10日 17:50:14[lc900730](https://me.csdn.net/LC900730)阅读数：110标签：[spring																[struts																[hibernate](https://so.csdn.net/so/search/s.do?q=hibernate&t=blog)](https://so.csdn.net/so/search/s.do?q=struts&t=blog)](https://so.csdn.net/so/search/s.do?q=spring&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
### 整合步骤
1） 
    引入struts jar文件 
    Struts核心jar 
    Hibernate核心jar 
    Spring：
```
Core
    Web
    Aop
    Orm
    Jdbc/tx  jdbc支持包，事务相关包
```
### 2）配置
web.xml
```
初始化struts功能，spring容器
```
Struts.xml   配置请求路径与映射action的关系 
Spring.xml   IOC容器配置
```
bean-base.xml  公用信息
    bean-service.xml
    bean-dao.xml
    bean-action.xml
```
