# iOS-本地私有库和远程私有库.其实并不难 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年03月23日 08:25:31[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2063
### 本文来自简书，原文地址:[http://www.jianshu.com/p/0b6299f80bbd](http://www.jianshu.com/p/0b6299f80bbd)
### 私有库的介绍
##### 为什么要学习私有库?
- 因为随着项目代码的体积不断增加,为了更好的管理项目，我们需要借助CocoaPods版本管理工具对原有项目进行拆分,然后创建对应的私有库进行保存
##### 什么是本地私有库?什么是远程私有库呢?
- 举个例子: 本地私有库就是你将钱(代码),放在身上(放在当前电脑中某个文件夹), 而远程私有库呢,就是你将钱放在第三方平台(支付宝,银行)之类的
##### 为什么是私有库?
- 私有的意思是只有相对应权限的人才能访问,因为项目是公司的根本,正常人都不会选择开源公司的项目,只会开源对应的框架
##### 本地私有库和远程私有库的区别是什么?
- 
本地私有库:
1,需要主动指明路径,如果调整路径,需要修改相关的配置
2,版本升级不好维护
3,不能团队共享
- 
远程私有库可以将你的代码传到第三方托管平台进行公司内部开发人员共享,从而实现组件化开发模式
##### 私有库应该放在什么地方呢?
- 本地私有库放在本地(电脑上的某个文件夹),而远程私有库需要放在第三方托管平台(如:github,码云,码市)
### 本地私有库
创建一个仓库(文件夹), 存储在本地, 在本地的其他工程中直接使用
> 
流程:
创建一个文件夹,把你需要拆分的代码放到文件夹中(例如:基类,分类,工具类等)
然后通过终端把你的这个文件夹进行git进行管理
`git init  /   git add. / git commit -m 'xx'/`
之后创建个spec文件进行框架的描述
`pod spec create xxx(你创建的文件夹的名字)`
用Xcode打开spec文件进行编辑框架的描述
```
s.name = "框架的名字"
s.version = "版本号"
s.summary = "简单描述" 
s.description "详细猫叔"  #要比简单描述长度大
s.homepage = "主页"
s.license = "MIT" # 开放协议根据自己的需要填写
s.autor = "作者"
s.platform = "支持平台和版本"
s.soure = "托管git代码仓库地址"
s.souce_files = "soure中关键代码的路径"
s.requires_are = true # 支持ARC
```
```
进行完上面的步骤,你简单的本地私有库就已经搭建好了
#简单点   搭建仓库的方式简单点...
```
你现在可以进行主工程项目进行cocoapods管理
```bash
cd 进入你的主工程
pod init 初始化仓库
然后对你的Podfile文件进行编写
```
![](http://upload-images.jianshu.io/upload_images/1846074-9c16f09836415ad3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
编写podfile文件
> 
```java
```java
:path => 后面跟的是你文件的本地路径 ../ 是跳到上一级目录
好,配置完成之后 直接pod install 安装你本地的私有化库吧
运行安装完美
```
```
#### 以上的方案成功之后 下面讲解本地私有库的优化
- 
###### 因为之前的本地库(你拆分的分类或者基类文件) 是用来导入到主工程的,所以在导入主工程之前我们需要先测试,保证当前这个库的稳定性
> 
创建一个LibTest文件夹 ,将你的本地库拖进去,之后在LibTest文件中创建一个Example工程进行测试
测试很简单 上面已经提过, 将pod init 初始化 然后在podfile文件中配置本地库 然后pod install 导入就OK了
### 本地私有库的优化
```
进入LibTest文件夹里面
创建一个pods库的模板库
pod lib create xxx(你库的名字例如:LionBase)
```
之后会提示你输入对应的信息
![](http://upload-images.jianshu.io/upload_images/1846074-d936a206c70f4a2a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
配置信息
![](http://upload-images.jianshu.io/upload_images/1846074-82b818961b5f810a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
需要替换的文件
之后直接 pod install 就OK啦
![](http://upload-images.jianshu.io/upload_images/1846074-fc8ba81424c9afa8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
文件夹目录
### 远程私有库
> 
##### 现在这个是重点了 因为在开发中我们都是使用远程私有库
如果有不理解或者疑问,可以简信私聊我
##### one 远程私有库的创建
首先如果需要在第三方的托管平台创建一个私有的库(可以在免费的 码云或者码市 )
- 创建私有索引库
![](http://upload-images.jianshu.io/upload_images/1846074-185aa158dd880a25.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
索引库
- 
添加repo索引库
`pod repo add LionFMSpecs xxxx(你刚创建的仓库地址 )`
![](http://upload-images.jianshu.io/upload_images/1846074-0ddc3ae787db6d3d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
- 
如果提示下面的信息 说明你电脑没有配置公钥和私钥
![](http://upload-images.jianshu.io/upload_images/1846074-bee31a3d5c9eb2f3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
错误信息
- 
添加公钥和私钥
`终端输入  ssh-keygen`
![](http://upload-images.jianshu.io/upload_images/1846074-2f02eeb9566a6e56.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
公钥和私钥
- OK 完成上面的步骤之后
在码市上创建一个基础库 (存放分类 工具类 这些)
![](http://upload-images.jianshu.io/upload_images/1846074-64c05542fbc30953.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
基础库
- 将模板库clone下来
![](http://upload-images.jianshu.io/upload_images/1846074-bb3d1e0fd94ff8aa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
模板库
之后进行配置 将你的库拖进对应的文件夹中(LionFMBase)
再进入Example(测试工程) 进行导入基础类 pod install
ok 看下图
![](http://upload-images.jianshu.io/upload_images/1846074-ef3108b99a0ed093.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
配置信息
```
s.name = "框架的名字"
s.version = "版本号"
s.summary = "简单描述" 
s.description "详细猫叔"  #要比简单描述长度大
s.homepage = "主页"
s.license = "MIT" # 开放协议根据自己的需要填写
s.autor = "作者"
s.platform = "支持平台和版本"
s.soure = "托管git代码仓库地址"
s.souce_files = "soure中关键代码的路径"
s.requires_are = true # 支持ARC
```
配置完之后就可以提交到你远程的私有库了 第一次要进行关联!!
```
pod stauts
pod add . 
pod commit -m 'xx'
//添加关联
git remote add origin 你那个Base库的地址
//提交
git push origin master  
如果第一次提交还会要求你验证账号和密码
```
```
现在就开始绑定tag了  
可以通过git tag 查看tag
git tag '0.1.0'
git psh --tags
提交完之后  可以用下面的命令行验证
pod spec lint 验证远程
pod lib lint 验证本地
```
```
开始提交
pod repo 查看索引库
pod repo push LionFMSpecs LionFMBase.podspec
```
如果第一次配置 会比较麻烦 好了 只剩最后一步了
> 
打开你的主工程 然后来到podfile文件进行配置
![](http://upload-images.jianshu.io/upload_images/1846074-1673f36a66feb633.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
配置
如果你不配置系统的索引库 会找不到其他的库 只能找到你自己创建的仓库
最近一个命令 pod install
Nice 完成了
- 
### 本地私有库的升级维护
> 
当拆分的库需要进行修改或者添加新的内容
修改或者添加完之后 在测试工程进行pod install 进行更新导入
之后进入你拆分的库的podspec 将版本更改
```
pod status
pod add .
pod commit -m 'xxx'
pod push origin master
```
将代码传到远程的私有库 完成之后 更新tag
```
git tag '0.2.0'
git push --tags
```
pod spec lint 验证
更新自己的库
`pod repo push LionFMSpecs LionFMBase.podspec`
可以进入主工程了 pod update --no-repo-update进行更新
如果你的私有库有引用外界的框架 只需要在 podspec 添加 s.dependency '你引用的库'就OK了 详情见下图
![](http://upload-images.jianshu.io/upload_images/1846074-4010d9879f1ae74d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
![](http://upload-images.jianshu.io/upload_images/1846074-d92de1b117809624.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
`
#### 下面是流程图 方便各位理解
![](http://upload-images.jianshu.io/upload_images/1846074-326facf3abf95435.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
流程图
