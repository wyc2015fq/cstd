# Cocoapods制作自己的共有库、 - weixin_33985507的博客 - CSDN博客
2018年10月25日 16:04:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
![6545546-81e64232c6f9dbb5.png](https://upload-images.jianshu.io/upload_images/6545546-81e64232c6f9dbb5.png)
# 【一】前言：
> 
每个语言发展到一个阶段，就会出现相应的依赖管理工具，或者说是中央代码仓库。
比如Java的Maven，Python的Pip。而CocoaPods就是一个iOS项目中的第三方开源代码管理工具。
#### 以前的问题：
> 
1、使用一个第三方SDK，需要在工程里面添加一堆配置，少添加一步配置就会报一大堆错误。这个时候为了排查少了哪一个步骤，只能从头开始重做一遍，太浪费时间和精力。有些配置还不好配置，就更使人烦躁郁闷了。
2、如果使用的第三方SDK有比较重要的更新，想使用最新的SDK只能到官方下载最新的SDK，重新导入到自己的项目工程中。
3、如果公司有多个项目，每一个工程都这样配置一遍，这是多么痛苦的领悟呀。
#### 它的优势：
> 
1、集成第三方库，不用我们手动配置。
2、更新方便，一条命令搞定。
3、统一管理，简单高效，开发者集中精力于功能实现上，不用关注 太多其他的东西。
4、方便组件化（建立在私有库基础上）
#### 预备知识：
> 
Cocoapods安装和使用 和 GitHub的使用 这里无需多言。基础知识，如果有疑问的可以自己百度处理。
# 【二】制作目录：
先把大致流程要点总结一下：
> 
1、GitHub创建共有库
2、创建podspec文件
2.1、配置podspec文件
3、本地库验证
4、推送打标签
5、验证podspec文件
6、注册，推送podspec到cocoapods，搜索验证
7、搜索验证
# 【三】具体步骤：
**1、GitHub创建共有库**
打开GitHub，开始Create a new repository。具体如图：
![6545546-2b4115f56c7442a3.png](https://upload-images.jianshu.io/upload_images/6545546-2b4115f56c7442a3.png)
创建完成之后，克隆到本地，我这边全程使用终端命令进行操作。打开终端，具体操作如下： 
![6545546-6f0bf0a293a0d67c.png](https://upload-images.jianshu.io/upload_images/6545546-6f0bf0a293a0d67c.png)
> 
** cd Desktop/**
> 
**git clone git@github.com:wnkpzzz/CZTestLibDemo.git**
**2、创建podspec文件**
![6545546-2899419382bd7aea.png](https://upload-images.jianshu.io/upload_images/6545546-2899419382bd7aea.png)
经过上一个步骤，我们已经把CZTestLibDemo克隆到桌面了。进入该文件夹，创建podspec文件：
> 
**pod spec create CZTestLibDemo**
![6545546-cd4e19b3b0d49702.png](https://upload-images.jianshu.io/upload_images/6545546-cd4e19b3b0d49702.png)
**2.1配置podspec文件**
 找到CZTestLibDemo.podspec文件，选择 文本编辑 进行打开。我们看到全是英文，不要慌张，且听我一步一步解析。
![6545546-6ab667a96a2e674e.png](https://upload-images.jianshu.io/upload_images/6545546-6ab667a96a2e674e.png)
它的意思其实就是：
> 
1.你要使用哪个平台？iOS
2.你要使用哪种语言？
3.库中是否包含一个实例程序？（一般选择示例程序）
4.你要使用哪个测试框架？（没有就写None）
5.是否要UI测试？（我一直都是NO，没有测试过）
6.类名前缀是什么？(这个我创建Swift库的时候没有这一项)
#开头的都是注释，我们先把它删掉，下面是我整理出来的直接填写好的，完整版。**（文章最后有源代码，可复制）**
![6545546-f044d4db301e3f07.png](https://upload-images.jianshu.io/upload_images/6545546-f044d4db301e3f07.png)
> 
特别说明：在source_files这一点 s.source_files  = 'SourceFiles'
是因为有的同学可能会遇到下面的错误，怎么都无法解决： ERROR | [iOS] file patterns: The `source_files` pattern did not match any file.  
s.source_files  是存放库资源文件的。这里指向SourceFiles文件夹，里面随便放入一个OC类即可。
![6545546-683b3d2dec46d343.png](https://upload-images.jianshu.io/upload_images/6545546-683b3d2dec46d343.png)
> 
**特别提醒: 还需创建一个LICENSE文件，做版权声明的。    （文章最后有源代码，可复制）这样完整的结构如下图：**
![6545546-b3cbfc07c5001d2c.png](https://upload-images.jianshu.io/upload_images/6545546-b3cbfc07c5001d2c.png)
**3、本地库验证**
**通过终端，在CZTestLibDemo文件夹里面执行下面命令：**
> 
**pod lib lint CZTestLibDemo.podspec**
如果有警告（WARN），需要根据提示内容解决警告，忽略警告 
> 
**pod lib lint CZTestLibDemo.podspec --allow-warnings**
执行成功会提示： **CZTestLibDemo passed validation **
![6545546-a839c87f1a2a8e20.png](https://upload-images.jianshu.io/upload_images/6545546-a839c87f1a2a8e20.png)
**4、推送打标签**
**通过终端，在CZTestLibDemo文件夹里面执行下面命令：**
> 
**git add .**
**git commit -m "1.0"**
**git push**
> 
**git tag -m  'release version 1.0.0'  1.0.0**
**git push origin 1.0.0 **
**5、验证podspec文件**
**通过终端，在CZTestLibDemo文件夹里面执行下面命令：**
> 
**pod spec lint CZTestLibDemo.podspec**
![6545546-cc8ea5b75d74831f.png](https://upload-images.jianshu.io/upload_images/6545546-cc8ea5b75d74831f.png)
**6、注册，推送podspec到cocoapods，验证**
通过终端，在CZTestLibDemo文件夹里面执行下面命令：
> 
**pod trunk register [email] ‘用户名’ --description='描述，可不填'**
**example**
**pod trunk register 123@qq.com 'wnkpzzz' --description='MacBook Pro'**
之后会给你发送一条邮箱，进行确认，这里的-**-description='MacBook Pro'**可以省略，复制邮箱的链接，在浏览器打开如下图：
![6545546-a04d5b2dd8986816.png](https://upload-images.jianshu.io/upload_images/6545546-a04d5b2dd8986816.png)
查看个人信息。通过终端，执行下面命令：
> 
**pod trunk me**
![6545546-d891a23cde0aa269.png](https://upload-images.jianshu.io/upload_images/6545546-d891a23cde0aa269.png)
**推送podspec到Cocoapods  **
通过终端，在CZTestLibDemo文件夹里面执行下面命令：
> 
**pod trunk push  CZTestLibDemo.podspec**
**推送成功之后，终端输出如下**
![6545546-18fea5cfee289935.png](https://upload-images.jianshu.io/upload_images/6545546-18fea5cfee289935.png)
**7、搜索验证**
> 
**pod search CZTestLibDemo**
如果发现搜索不到出现如下提示：
> 
[!] Unable to find a pod with name, author, summary, or description matching `CZTestLibDemo`
解决方法：
> 
//删除本地索引
**rm ~/Library/Caches/CocoaPods/search_index.json**
  //搜索
** pod search [库名]**
  //更新索引
**  pod repo update **
![6545546-5b4c7b25b5d11e50.png](https://upload-images.jianshu.io/upload_images/6545546-5b4c7b25b5d11e50.png)
GitHub源码：[点击跳转下载](https://github.com/wnkpzzz/CZTestLibDemo)
