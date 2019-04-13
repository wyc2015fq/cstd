
# 2.02  hyperledger fabric入门 - 尹成的技术博客 - CSDN博客

2018年12月10日 09:37:08[尹成](https://me.csdn.net/yincheng01)阅读数：210个人分类：[GO语言](https://blog.csdn.net/yincheng01/article/category/7679307)[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)[](https://blog.csdn.net/yincheng01/article/category/7679307)



**1.需要的环境**
docker
docker-compose
Go
git
npm和nodejs
**2.下载fabric组件的Docker镜像**
hyperledger的docker商店地址：[https://store.docker.com/profiles/hyperledger](https://store.docker.com/profiles/hyperledger)
下载peer、orderer、tools、baseos和ccenv五个组件
**3.下载fabric源码库**
创建目录
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162726963.png)
进入目录
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162733124.png)
下载fabric
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162737457.png)
进入下载后的目录
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120516274193.png)
切换版本
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162745520.png)
进入目录
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162750911.png)
安装
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162755750.png)
退到之前目录，然后进入目录
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162802958.png)
安装
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162808654.png)
4.下载fabric-samples
进入目录
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162813420.png)
下载Samples
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120516281769.png)
使用官方提供的例子建立一个fabirc网络
5.部署第一个网络（hello-world）
进入fabric-samples的目录
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162823327.png)
切换分支
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162834298.png)
进入
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162841359.png)
生成配置
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162844443.png)
查看生成的配置文件
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162850133.png)
查看ca和msp
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162854688.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162859523.png)
查看Org1MSPanchors.tx和Org2MSPanchors.tx
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162905528.png)
启动网络
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162911987.png)
执行成功
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162916257.png)
6.日志和示例代码解析
fabric网络启动完毕
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162920696.png)
创建通道，安装链码等
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162927318.png)
安装chaincode
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162933906.png)
查询结果为100
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162939459.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162944715.png)
查看docker
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162949833.png)
查看生成的三个链码
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162955611.png)
查看链码位置
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205163001803.png)
查看chaincode
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120516300864.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205163012511.png)
查看链码
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205163017568.png)
[查看script.sh](http://xn--script-2i5my41h.sh)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205163023345.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120516303021.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
[](https://img-blog.csdnimg.cn/20181205163023345.png)7.关闭网络
[](https://img-blog.csdnimg.cn/20181205163023345.png)关闭
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205163034100.png)
查看容器，已经没有正在运行的了
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205163038937.png)
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：
721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111611182187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

