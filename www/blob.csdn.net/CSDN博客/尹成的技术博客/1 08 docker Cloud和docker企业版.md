
# 1.08 docker Cloud和docker企业版 - 尹成的技术博客 - CSDN博客

2018年12月10日 09:35:05[尹成](https://me.csdn.net/yincheng01)阅读数：138个人分类：[GO语言](https://blog.csdn.net/yincheng01/article/category/7679307)[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)[](https://blog.csdn.net/yincheng01/article/category/7679307)



**1.docker公司业务介绍**
2017年初，docker分为社区版和企业版
docker公司还提供培训，网址：[https://training.docker.com/](https://training.docker.com/)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120515553751.png)
docker公司还提供认证，网址：[http://success.docker.com/](http://success.docker.com/)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120515554787.png)
类似于苹果的AppStore，网址：[https://store.docker.com/](https://store.docker.com/)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155552289.png)
可以通过web界面创建service，网址：[https://cloud.docker.com/，这是收费的](https://cloud.docker.com/%EF%BC%8C%E8%BF%99%E6%98%AF%E6%94%B6%E8%B4%B9%E7%9A%84)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155557340.png)
2.docker cloud自动化构建
是docker公司第一个CaaS（container-as-a-service）产品，提供容器服务的，是在PaaS（类似于阿里云）之上的，就是在云上搭建docker服务
是提供容器的管理、编排、部署的托管服务
docker公司2015年收购了tutum公司，将tutum改装成了docker cloud
docker cloud提供的主要模块如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155609727.png)
docker cloud有两种模式，standard（单点）模式和swarm（集群）模式
使用流程图如下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155614218.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
实现docker image的自动化构建
在项目目录初始化git仓库，会多一个隐藏文件夹.git
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155620197.png)
该目录下的所有文件添加到仓库
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155624597.png)
首次使用，需要本地与github建立连接，步骤如下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155629120.png)
C盘用户目录下找到.ssh文件夹，将里面的id_rsa.pub文件内容复制
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155636160.png)
进入github点击右上角的settings
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155650281.png)
选择左侧SSH and GPG keys
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155657514.png)
点击右侧的New SSH key
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155705942.png)
Title随便填，Key将刚才复制的粘贴进来
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155712931.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
建立好连接后，创建一个git仓库，用于上传代码
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120515572172.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120515572785.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155744898.png)
复制生成的地址
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155753480.png)
本地提交代码到仓库
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155757963.png)
将远程仓库关联到本地，后面加的是上面复制的地址
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155804174.png)
若已存在，可以清除之前的仓库地址
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155810997.png)
内容推送到github
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155816277.png)
网址：[https://cloud.docker.com/](https://cloud.docker.com/)
使用docker cloud时，需要与github账号关联
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155821385.png)
点击github后的插头
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155827140.png)
点击
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155831983.png)
密码验证
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155836739.png)
关联成功
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120515584287.png)
点击创建orgainization（组织）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155847406.png)
创建
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155854605.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
忽略这一步，信用卡和账户信息
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155905712.png)
创建成功
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120515591543.png)
创建自动构建的repository，这里实际是与github关联
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155922631.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
点进去重新连接
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155929432.png)
写一个名字，然后点击下面的create
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155934329.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155939906.png)
点击Builds，然后点击配置生成
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205155954496.png)
与github项目进行关联，根据里面的Dockerfile文件去构建
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160001179.png)
点击保存和构建，如果点不了，就先save，再构建
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160006672.png)
构建成功
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160011194.png)
点击如下，可以查看构建过程日志
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160018988.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160022598.png)
打开dockerHub，可以点击自己创建的新用户查看镜像
github+dockerCloud+dockerHub实现了关联
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160029797.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160039826.png)
底下的介绍信息，就是gitHub中的介绍信息
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160053433.png)
在gitHub修改介绍页
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160100514.png)
随便修改个字母，点击commit
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160106736.png)
此时可以看到docker cloud在自动构建
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160115902.png)
也可以添加版本号，先在gitHub点击如下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160123157.png)
点击创建
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160131751.png)
如下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160140832.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160148574.png)
此时多了个1.0
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160156807.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
在docker cloud的构建页面，点击右上角的设置
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160203123.png)
操作如下，先添加一个build，然后按下面设置
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160211530.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
保存并构建
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160220755.png)
构建成功
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160226950.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
3.docker企业版的在线使用
网址：[https://www.docker.com/products/docker-enterprise](https://www.docker.com/products/docker-enterprise)
使用docker公司提供的单节点的企业版试用，试用时间12小时
点击申请直接使用docker公司搭建好的环境（需要翻墙人机验证），邮箱收到个邮件，点击邮件就可以使用
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160239660.png)
4.阿里云部署容器
网址：[https://cn.aliyun.com/](https://cn.aliyun.com/)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160246272.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
往下拉，点击右侧容器服务
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160252731.png)
点击立即开通
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160257928.png)
可以直接支付宝登录
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120516030591.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
立即开通，可能需要实名认证
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160313608.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
成功开通
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160318982.png)
同意授权
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160325921.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160332751.png)
左上角，容器服务由swarm和kubernetes
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160340768.png)
可以查看如何操作
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160348132.png)
点击创建集群
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160353715.png)
余额要大于100才可以使用
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160400345.png)
可以创建应用（有标准发布和蓝绿发布，标准发布会停一会儿服务，蓝绿发布时，服务不会停止，其实是同时有两个服务）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160404920.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
查看集群状况
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120516041747.png)
有集群后，可以直接使用镜像和方案
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160426853.png)
5.亚马逊云部署容器
网址：[https://signin.aws.amazon.com](https://signin.aws.amazon.com)
点击管理控制台，跳转到登录页面
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160435690.png)
直接点启动实例，就是创建实例
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：
721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111611182187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

