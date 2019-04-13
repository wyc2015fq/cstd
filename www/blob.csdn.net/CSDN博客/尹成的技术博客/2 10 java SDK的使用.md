
# 2.10 java SDK的使用 - 尹成的技术博客 - CSDN博客

2018年12月10日 09:45:00[尹成](https://me.csdn.net/yincheng01)阅读数：350


**1.centOS7安装node.js**
杀掉活跃的容器
官网https://nodejs.org/en/下载tar.xz包，然后解压
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202623714.png)
设置node为全局变量
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202626132.png)
设置npm为全局变量
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202628487.png)
查看版本
2.搭建网络
赋予执行权限
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202632307.png)
启动网络
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202637539.png)
进入容器
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202644105.png)
查看通道，已经存在
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202646197.png)
3.链码编写和测试
编写nodeExample.go
安装链码
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202650400.png)
实例化链码
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202653753.png)
初始化车辆信息
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202657381.png)
查询车辆信息
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202659839.png)
查询所有车辆信息
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202705377.png)
添加车辆信息，再次查询全部，已经有这条信息了
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120520271055.png)
改变车辆拥有者，再次查询全部，已经改变了
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202712547.png)
4.node外部服务调用
安装python2.7
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202719994.png)
配置环境变量
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202722475.png)
创建并初始化
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202726573.png)
安装依赖包
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202728788.png)
安装fabric-client
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202730815.png)
安装fabric-ca-client
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202735339.png)
运行enrollAdmin.js
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202738883.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120520274124.png)
运行registerUser.js
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120520274583.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202747575.png)
运行query.js
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202749560.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202751356.png)
运行invoke.js
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202755438.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120520275746.png)
[](https://img-blog.csdnimg.cn/20181205202755438.png)学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：
721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111611182187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

