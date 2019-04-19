# jeecg-framework-3.3.2-RELEASE 最新版本发布 - xqhrs232的专栏 - CSDN博客
2018年11月23日 16:58:48[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：78
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/zhangdaiscott/article/details/10908765](https://blog.csdn.net/zhangdaiscott/article/details/10908765)
相关文章
1、[jeecg使用心得](https://www.cnblogs.com/bweird-java/p/6547939.html)----[https://www.cnblogs.com/bweird-java/p/6547939.html](https://www.cnblogs.com/bweird-java/p/6547939.html)
2、[JEECG技术总结](https://www.cnblogs.com/shuilangyizu/p/7607721.html)----[https://www.cnblogs.com/shuilangyizu/p/7607721.html](https://www.cnblogs.com/shuilangyizu/p/7607721.html)
3、jeecg框架日常开发问题解决方法----[https://blog.csdn.net/zhangdaiscott/article/details/67053393](https://blog.csdn.net/zhangdaiscott/article/details/67053393)
4、JEECG Framework 3.5.2 (快速开发平台) ACE版本发布----[https://www.aliyun.com/jiaocheng/290599.html](https://www.aliyun.com/jiaocheng/290599.html)
5、[【视频教程】JEECG 入门视频教程](http://www.jeecg.org/forum.php?mod=viewthread&tid=197)----[http://www.jeecg.org/forum.php?mod=viewthread&tid=197&extra=page%3D1](http://www.jeecg.org/forum.php?mod=viewthread&tid=197&extra=page%3D1)
6、[JEECG](https://www.cnblogs.com/shuilangyizu/p/7607721.html)官方网站----[http://www.jeecg.org/](http://www.jeecg.org/)
平台介绍
JEECG（J2EE CodeGeneration）是一款基于代码生成器的智能开发平台，引领新开发模式(OnlineCoding模式->代码生成器模式->手工MERGE智能开发)，可以帮助解决Java项目60%的重复工作，让开发更多关注业务逻辑。既能快速提高开发效率，帮助公司节省人力成本，同时又不失灵活性。
JEECG宗旨：简单功能由代OnlineCoding配置出功能;复杂功能由代码生成器生成进行手工Merge; 复杂流程业务采用表单自定义，业务流程使用工作流来实现、扩展出任务接口，供开发编写业务逻辑。实现了流程任务节点和任务接口的灵活配置，既保证了公司流程的保密行，又减少了开发人员的工作量。
平台架构
·        JEECG V3.0版本采用SpringMVC+Hibernate+UI快速开发库+Spring jdbc+freemarker+Highcharts图形报表+bootstrap+Ehcache+MiniDao的基础架构
·        采用面向声明的开发模式，基于泛型编写极少代码即可实现复杂的数据展示、数据编辑、
          表单处理等功能，再配合Online    Coding在线开发与代码生成器的使用,将J2EE的开发效率提高6倍以上，可以将代码减少80%以上。
·        设计思想: 零配置（ 约定大于配置）
平台优势
* 采用主流开源技术框架，容易上手; 代码生成器依赖性低,很方便的扩展能力，可完全实现二次开发;
* 开发效率很高,代码生成器支持多种数据模型：单表数据模型、单表自关联模型和一对多(父子表)数据模型，代码生成功能直接使用；
* 查询SQL过滤器，后台不需要写代码，页面追加查询字段，查询功能自动实现
* 页面校验自动生成(必须输入、数字校验、金额校验、时间控件等);
* 基础的用户权限：菜单，按钮权限，角色
* 常用共通封装，各种工具类(定时任务,短信接口,邮件发送,Excel导出等),基本满足80%项目需求
* 集成简易报表工具，图像报表和数据导出非常方便，可极其方便的生成pdf、excel、word等报表；
* 集成工作流引擎Activiti5，并实现了只需在页面配置流程转向，可极大的简化工作流的开发；用Activiti5的流程设计器画出流程走向，一个工作流基本就完成了，只需进行流程的配置或者写很少量的java代码
JEECG 技术点:
·        技术点一：Online Coding开发模式（通过配置实现单表模型和一对多数据模型的增删改查功能,No 代码，无需一行代码，支持用户自定义表单布局）
·        技术点二：代码生成器，支持多种数据模型,根据表生成对应的Entity,Service,Dao,Action,JSP等,增删改查功能生成直接使用
·        技术点三：UI标签库，针对WEB UI进行标准封装，页面统一采用UI标签实现功能：数据datagrid,表单校验,Popup,Tab等，实现JSP页面零JS，开发维护非常高效
·        技术点四：工作流组件，支持在线流程定义，采用开源Activiti流程引擎，实现在线画流程,自定义表单,表单挂接,业务流转，流程监控，流程跟踪，流程委托等
·        技术点五：系统日志记录 (详细记录操作日志,便于问题追踪)
·        技术点六：Web GIS支持（基础技术GIS的支持）
·        技术点七：移动平台支持，对Bootstrap(兼容Html5)进行标准封装
·        技术点八：动态报表功能（用户输入一个sql，系统自动解析生成报表）
·        技术点九：查询过滤器(页面加查询条件，后台不需要写任何逻辑判断，动态拼HQL追加查询条件)
·        技术点十：MiniDao(最佳持久层解决方案，优于Mybtais和Hibernate)
·        技术点十一：报表工具集成（Excel简易导出工具类+Highcharts图形报表）
·        技术点十二： 常用共通封装表单校验组件/数据字典/ 邮件发送/ 定时任务/短信接口/Freemarker模板工具/Jquery
·        JEECG     V3.0,经过了专业压力测试,性能测试，保证后台数据的准确性和页面访问速度
·        支持多种浏览器: IE, 火狐, Google 等
·        支持数据库:Mysql,Oracle10g,Postgre,SqlServer等
·        基础权限: 用户，角色，菜单权限，按钮权限，数据权限
·        智能报表集成: 简易的图像报表工具和Excel导入导出
·        Web容器测试通过的有Jetty和Tomcat6,Weblogic
·        即将推出功能：分布式部署，云平台，移动平台开发，规则引擎
·        要求JDK1.6+
jeecg-framework-3.3.2-RELEASE升级日志：
———————————————————————————————————————— 
version: jeecg-framework-3.3.2-RELEASE
date:     2013-09-02 
作者:     张代浩
联系邮箱: scott@jeecg.com
————————————————————————————————————————
新增功能
[OnLine Coding]-Online Coding支持自定义按钮以及按钮权限控制
[OnLine Coding]-表单上传文件控件，在列表显示
[OnLine Coding]-加入Ehcache缓存，提高表单访问效率
[OnLine Coding]-表单POPUP弹出控件
[OnLine Coding]-列表数据过滤接口
[OnLine Coding]-系统变量实现-登录人的信息，部门信息等
[OnLine Coding]-智能表单系统变量约定字段
[系统功能]  -  集成MiniDao （持久层解决方案，兼备Hibernnate实体维护和MybatisSQL分离两大优点）
功能增强
[OnLine Coding]-SQL增强和JS增强完善，与自定义按纽整合
[OnLine Coding]-字段显示开关，表单和列表配置分离
[OnLine Coding]-字典功能支持自定义字典表
[OnLine Coding]-表单维护界面，用户体验改造
[OnLine Coding]-智能表单字段tab分页,功能调整
[系统功能]- Excel导出功能增强，支持明细，图片（支持实体关联关系的，excel数据导出）
[系统功能]-tab右键加刷新选项
[ui库]-按钮权限标签，不使用datagrid标签的情况下使用
[ui库]-按钮标签，扩展字段长度，宽度，用户可以设置弹出页面大小
[OnLine Coding]-文本框控件改为独占一行
[OnLine Coding]-数据库支持扩展，目前支持mysql,sqlserver,oracle,postgres 
BUG修复
[ui库]--datagrid的分页显示页码数错误的bug
[OnLine Coding]-表单字段，double 10,2 输入10.99数据点击编辑的时候只能看到10(四舍五入了)
[OnLine Coding]-excel数据导入报错，因为ID没有值
[ui库]-datagrid列表动态生成查询字段，字段名字太长会出现的换行问题
[动态Report]-Sql读取失败问题处理
[OnLine Coding]-表单提交，时间类型的必须输入校验不提示
[系统功能]-jdbc分页支持SqlServer
[系统功能]-初始化数据，数据库兼容问题处理，目前支持mysql,sqlserver,oracle,postgres
[ui库]-datagrid列表动态生成查询字段，字段名字太长会出现的换行问题。
[OnLine Coding]-异常友好提示处理机制
技术论坛：http://www.jeecg.org
源码下载：http://code.google.com/p/jeecg/downloads/list
在线演示：http://demo.jeecg.org:8090/
视频：JEECG-OnlineCoding实战《企业招聘系统》
交流群：
   JEECG 技术群一: 106259349(已满)
   JEECG 技术群二: 106838471
   JEECG 技术群三: 289782002
   JEECG 技术群四：325978980
   JEECG 技术群五：143858350
--------------------- 
作者：JEECG开源社区 
来源：CSDN 
原文：https://blog.csdn.net/zhangdaiscott/article/details/10908765 
版权声明：本文为博主原创文章，转载请附上博文链接！
