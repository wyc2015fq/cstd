
# 2.07 hyperledger fabric链码案例 - 尹成的技术博客 - CSDN博客

2018年12月10日 09:43:27[尹成](https://me.csdn.net/yincheng01)阅读数：299


**1.链码入门**
hello.go
安装链码
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171324150.png)
实例化链码
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171326464.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171328457.png)
调用链码
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171332258.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171334221.png)
[](https://img-blog.csdnimg.cn/20181205171332258.png)2.账户相关链码
[](https://img-blog.csdnimg.cn/20181205171332258.png)payment.go
安装链码
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171338362.png)
实例化链码
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171342787.png)
查询账户
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171347585.png)
转账
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171350895.png)
查询账户
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171352838.png)
存款
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171359285.png)
查询账户
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120517140483.png)
取钱
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171406504.png)
查询账户
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171409176.png)
3.贷款还款相关链码
Trace/*
安装链码
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171413526.png)
实例化链码
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171415748.png)
运行测试
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171417435.png)
贷款
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171420828.png)
还款
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171423207.png)
4.公民身份信息相关链码
citizens.go
安装链码
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171426257.png)
实例化链码
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171428705.png)
身份录入
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171432684.png)
身份查询
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171438145.png)
5.合约相关链码
contract.go
安装链码
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171442919.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171445263.png)
发布合约
![在这里插入图片描述](https://img-blog.csdnimg.cn/201812051714486.png)
响应合约
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171452601.png)
合约成交
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171456524.png)
关闭合约
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171458521.png)
查询最新的合约
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171502421.png)
查询所有的合约
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171510356.png)
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：
721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111611182187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

