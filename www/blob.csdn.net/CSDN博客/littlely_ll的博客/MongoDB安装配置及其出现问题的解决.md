# MongoDB安装配置及其出现问题的解决 - littlely_ll的博客 - CSDN博客





2018年05月28日 11:31:36[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：1109








MongoDB安装还是很容易的，但是在配置的时候出现了很多问题，浪费了不少时间，所以在此记录下来出现的问题及解决办法。

## 1. MongoDB安装

去[官网](https://www.mongodb.com/download-center?jmp=nav#community)下载安装包，我使用的是windows系统的。下载后一步一步安装，当然可以自定义安装： 
![这里写图片描述](https://img-blog.csdn.net/20180528103612320?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdHRsZWx5X2xs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180528103627415?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdHRsZWx5X2xs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
安装的时候一般不会出现什么问题，余下主要在配置上。我的安装位置为`d:\program files\MongoDB`

## 2. MongoDB配置

### 2.1 创建数据库文件的存放位置

MongoDB将数据目录存储在 db 目录下。但是这个数据目录不会主动创建，我们在安装完成后需要创建它。参考了大多数资料，一般都是把db文件夹放在MongoDB文件夹下，我第一次配置是这样做的，不过在启动MongDB服务时失败，说是在C盘或D盘找不到文件夹db，所以**数据目录应该放在根目录下（(如： C:\ 或者 D:\ 等 )**，我是放在D盘根目录下，即在D盘下先建立data文件夹，在data下建立db文件夹，路径如下：`d:\data\db`。

### 2.2 运行MongoDB服务
- 打开cmd，进入D:\program files\MongoDB\bin目录
- 输入命令运行MongoDB服务：`mongod --dbpath D:\data\db`，如果执行成功会出现一系列的信息。
- 浏览器输入[http://localhost:27017](http://localhost:27017) 看是否连接成功，成功时会出现`It looks like you are trying to access MongoDB over HTTP on the native driver port.`

### 2.3 配置本地MongoDB服务
- 在data文件夹下建立log文件夹`d:\data\log`
- 在Mongodb下新建配置文件mongo.config，内容为：

dbpath=D:\data\db 

logpath=D:\data\log\mongo.log- 用管理员身份打开cmd，并转到`d:\program files\mongodb\bin`下
- 安装MongoDB服务：`mongod --config "D:\program files\Mongodb\mongo.config" --install --serviceName "MongoDB"`

## 3 启动MongoDB服务

在命令行启动MongoDB服务时，出现`MongoDB 无法启动 发生服务特定错误:100`的情况，经查找资料，可以删除`d:\data\db`目录下的`mongod.lock`和`storage.bson`两个文件，然后再`net start MongoDB`，这样启动就成功了。 
![这里写图片描述](https://img-blog.csdn.net/20180528111706380?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdHRsZWx5X2xs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 4 总结出现的问题
- 数据库文件应放在根目录下，C:\或D:\下
- 启动MongoDB服务失败需要删除db文件夹下的两个文件`mongod.lock`和`storage.bson`






