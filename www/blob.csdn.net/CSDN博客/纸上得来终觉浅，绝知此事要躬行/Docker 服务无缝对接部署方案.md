# Docker 服务无缝对接部署方案 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月10日 15:34:07[boonya](https://me.csdn.net/boonya)阅读数：193








服务无缝对接（热部署），需要至少两台服务器，且服务更新可以自由切换主备关系，代理服务实际上是一个服务注册中心服务。下面的设计思路大致如下：
- 代码服务器仓库管理代码
- 代理服务接收修改事件拉取代码编译打包
- 服务器主从分配
- 主机服务，备机升级测试等验证
- 备机升级成功，主机开始升级
- 都升级完成流程结束



![](https://img-blog.csdnimg.cn/20181210152911360.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

Axure RP设计文件：[https://pan.baidu.com/s/1cGPZxXfHT8G03fLr0u7NLA](https://pan.baidu.com/s/1cGPZxXfHT8G03fLr0u7NLA)



