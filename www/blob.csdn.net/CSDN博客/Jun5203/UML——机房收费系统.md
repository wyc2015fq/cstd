# UML——机房收费系统 - Jun5203 - CSDN博客
2018年11月02日 15:37:05[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：94
### 机房收费系统的需求分析
#### 系统功能需求
（1）用户登录
  在用户进入系统前，首先要求用户进行登录，登录时要验证用户名和密码是否匹配，验证通过后允许用户进入本系统操作。
（2）权限设置
  操作员有一般用户的权利，管理员有一般用户和操作员的权利。
（3）数据录入
  用户卡号注册（基本信息）；充值
（4）数据查询
  按用户输入的卡号查询
一般用户：学生余额；上机记录；充值记录；上机状态
操作员：收取金额；金额返还信息；上机统计信息；操作员工作记录
管理员：正在值班教师
（5）数据修改
  用户登录后，修改用户登录密码；学生基本信息维护；基本数据设定。
（6）报表打印
  打印各种结账的报表
#### 用户管理模块
（1）添加和删除用户：管理员可以对管理员、操作员和一般用户进行添加和删除操作（但不可对自己进行操作）；操作员为一般用户注册卡号。
（2）用户权限：一般用户、操作员、管理员所授的权限不同。
#### 基本数据维护模块
（1）基本数据设定
  固定用户上机一小时费用；临时用户上机一小时费用；准备时间；至少上机时间。
（2）帮助
  说明本系统的负责人，谁提供的技术支持以及所属单位；关于本系统的其他操作。
#### 数据库模块
（1）用户信息管理
  用户信息包括用户基本信息；上下机信息；注册充值信息。
（2）员工信息管理
  员工信息包括操作员和管理员基本信息和操作员工作记录。
#### 信息查询模块
按用户输入的卡号查询
一般用户：学生余额；上机记录；充值记录；上机状态
操作员：收取金额；金额返还信息；上机统计信息；操作员工作记录
管理员：正在值班教师
### UML图
#### 用例图
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101110055421.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 登录人员
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101110303817.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 一般用户
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101121622427.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 操作员
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101122042234.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 管理员
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101122109844.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
#### 类图
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101160116766.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
#### 对象图
？
#### 时序图
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101182645924.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 用户登录
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101212733265.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 学生查看余额（退卡、充值）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101211448571.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 学生查看上机记录（学生充值记录查询）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102073851386.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 学生上机状态查看
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102075358670.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 修改密码（基本数据设定）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102073923537.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 查询收取金额 （查询金额返还信息、查询学生上机统计信息）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102074514803.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 学生基本信息维护
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102121739109.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 结账（日结账单、周结账单）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102122208922.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 操作员工作记录（正在值班教师）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102074003319.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 删除和添加用户（注册）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102073627315.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
#### 协作图
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102145715135.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 用户登录
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102143353121.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 学生查看余额（退卡、充值）
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018110214530157.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 学生查看上机记录（学生充值记录查询）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102145305582.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 学生上机状态查看
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102145309906.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 修改密码（基础数据设定）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102145318159.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 查询收取金额（查询金额返还信息、查询学生上机统计信息）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102145323761.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 学生基本信息维护
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102145327566.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 结账（日结账单、周结账单）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102145332598.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 操作员记录（正在值班教师）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102145337287.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 删除和添加用户（注册）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102145341486.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
#### 状态图
![在这里插入图片描述](https://img-blog.csdnimg.cn/201811021507046.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 注册（充值、退卡、删除和添加用户）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102151426580.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 学生查看余额（学生查看上机记录、学生充值记录查询、学生上机状态查看、查询收取金额、查询金额返还信息、学生基本信息维护、学生上机统计信息查询）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102151655908.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 结账（日结账单、周结账单）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102153257484.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
#### 活动图
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102152814927.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 登录
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102152415275.?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- People
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102152503874.?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
#### 组件图
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102135228713.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018110214283679.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
#### 配置图
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101125032693.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101125645429.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
