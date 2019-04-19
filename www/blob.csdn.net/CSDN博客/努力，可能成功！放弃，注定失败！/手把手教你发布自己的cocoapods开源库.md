# 手把手教你发布自己的cocoapods开源库 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年08月04日 14:43:15[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：283

[原文地址:http://www.jianshu.com/p/3a365f273439](http://www.jianshu.com/p/3a365f273439)
发布自己的`cocoapods`开源库按照以下步骤，良心制作，包教包会！！
下面我会通过一个名为[IFMMenu](https://github.com/liugangios/IFMMenu)的项目来讲解一下整个过程。
### 1.写好代码，上传到github
`github`上创建项目仓库的时候记得创建`LICENSE`(许可证/授权)文件,此文件必须要有。
github上创建项目仓库
### 2.将自己的项目打成`tag`
因为`cocoapods`是依赖`tag`版本的,所以必须打`tag`,以后再次更新只需要把你的项目打一个`tag`，然后修改`.podspec`文件中的版本接着提交到`cocoapods`官方就可以了,提交命令请看下面
```
//为git打tag, 第一次需要在前面加一个v
git tag "v1.0.0"  
git push --tags
```
### 3.注册CocoaPods
`trunk`需要`CocoaPods 0.33`版本以上，用`pod --version`命令查看版本，如果版本低，需要升级。
```
pod --version
//版本低于0.33
sudo gen install cocoapods
pod setup
```
已经注册过的不需要注册,怎么看自己有没有注册
```
pod trunk me
```
pod trunk me信息
若未注册，执行以下命令，邮箱以及用户名请对号入座。用户名我使用的是`Github`上的用户名。
```
// 加上--verbose可以输出详细错误信息，方便出错时查看。
pod trunk register example@example.com 'liugangios'  --verbose
```
注册完成之后会给你的邮箱发个邮件,进入邮箱邮件里面有个链接,需要点击确认一下。
注册完成后使用`pod trunk me`检验注册是否成功。
### 4.创建.podspec
`cd`到你项目的目录,执行命令
```
pod spec create IFMMenu
```
### 5.编辑.podspec
创建好后，打开`.podspec`,删除注释, 前面有#的为注释,也可以用以下代码替换，然后编辑自己的库信息。
```
```
Pod::Spec.new do |s|
s.name        = 'IFMMenu'
s.version     = '1.0.2'
s.authors     = { 'liugangios' => 'example@example.com' }
s.homepage    = 'https://github.com/liugangios/IFMMenu'
s.summary     = 'a dropdown menu for ios like wechat homepage.'
s.source      = { :git => 'https://github.com/liugangios/IFMMenu.git',
:tag => s.version.to_s }
s.license     = { :type => "MIT", :file => "LICENSE" }
s.platform = :ios, '7.0'
s.requires_arc = true
s.source_files = 'IFMMenu'
s.public_header_files = 'IFMMenu/*.h'
s.ios.deployment_target = '7.0'
end
```
```
- `s.name：`名称，`pod search`搜索的关键词,注意这里一定要和.podspec的名称一样,否则报错
- `s.version：`版本号，to_s：返回一个字符串
- `s.author:`作者
- `s.homepage:`项目主页地址
- `s.summary:` 项目简介
- `s.source:`项目源码所在地址
- `s.license:`许可证
- `s.platform:`项目支持平台
- `s.requires_arc:` 是否支持ARC
- `s.source_files:`需要包含的源文件
- `s.public_header_files:`需要包含的头文件
- `s.ios.deployment_target:`支持的pod最低版本
其他一些非必要字段
- `s.social_media_url:`社交网址
- `s.resources:`资源文件
- `s.dependency:`依赖库，不能依赖未发布的库
> 
s.license= { :type => "MIT", :file => "LICENSE" }
这里建议这样写,如果写别的会报警告,导致后面一直提交失败。
**source_files写法及含义**
```
"IFMMenu/*
"IFMMenu/IFMMenu/*.{h,m}"
"IFMMenu/**/*.h"
```
`*`表示匹配所有文件
`*.{h,m}`表示匹配所有以`.h`和`.m`结尾的文件
`**`表示匹配所有子目录
**s.source常见写法**
```java
```java
s.source = { :git => "https://github.com/liugangios/IFMMenu.git", :commit => "68defea" }
s.source = { :git => "https://github.com/liugangios/IFMMenu.git", :tag => 1.0.0 }
s.source = { :git => "https://github.com/liugangios/IFMMenu.git", :tag => s.version }
```
```
- `commit => "68defea"` 表示将这个`Pod`版本与`Git`仓库中某个`commit`绑定
- `tag => 1.0.0`表示将这个`Pod`版本与`Git`仓库中某个版本的comit绑定
- `tag => s.version`表示将这个`Pod`版本与`Git`仓库中相同版本的`comit`绑定
### 6.验证.podspec
到此检查一下你工程中有以下文件：
- 你的项目,
- `.podspec`文件,
- `LICENSE`文件
项目内文件
使用以下命令测试本地`.podspec`文件是否存在语法错误。
```
pod spec lint IFMMenu.podspec --verbose
```
### 7.发布
```
pod trunk push IFMMenu.podspec
```
时间较长，耐性等待，大概5-10分钟，发布成功后会有以下提示
发布成功截图
### 8.测试自己的cocoapods
这个时候使用`pod search`搜索的话会提示搜索不到，可以执行以下命令更新本地`search_index.json`文件
```
rm ~/Library/Caches/CocoaPods/search_index.json
```
然后
```
pod search IFMMenu
```
该命令会重新创建`search_index.json`文件，5-10分钟，耐性等待
搜索结果
### 完整命令
```
git tag "v1.0.0"
git push --tags 
pod trunk register example@example.com 'liugangios'  --verbose
pod trunk me
pod spec create IFMMenu
//编辑 IFMMenu.podspec
pod spec lint IFMMenu.podspec
pod trunk push IFMMenu.podspec 
rm ~/Library/Caches/CocoaPods/search_index.json
pod search IFMMenu
```
### 报错处理
```
[!] Unable to accept duplicate entry for: XXXXX (1.0.0)
```
意思是：不能接受同一版本的提交
**解决方案：**
1、执行如下命令
```
git tag 1.0.1
git push --tags
```
2、修改`XXXX.podspec`文件中的内容
```java
```java
s.version      = "1.0.1"
  s.source       = { :Git => "https://github.com/xxx/xxx.git", :tag => "1.0.1" }
```
```
3、执行命令`pod spec lint`、`pod trunk push 库名.podspec`验证并提交到`CocoPods`
### 最后
[IFMMenu](https://github.com/liugangios/IFMMenu)是一个仿微信首页添加菜单的多功能、多种属性可自定义的下拉菜单，欢迎`download`下载提意见，star，感谢阅读。
IFMMenu展示图
### 下一篇预告:
cocoapods私有仓库搭建
参考文章：
[如何发布自己的开源框架到CocoaPods](http://www.cocoachina.com/ios/20160301/15459.html)
[把自己的开源库添加cocoapods](http://blog.csdn.net/lvmaker/article/details/51723589)
[3分钟让你的框架支持cocoapods,podspec文件讲解](http://www.jianshu.com/p/8a7b9232cbab)
[iOS：手把手教你发布代码到CocoaPods(Trunk方式)](http://www.cnblogs.com/wengzilin/p/4742530.html)
[iOS开发CocoPods支持报错](http://blog.csdn.net/yj_sail/article/details/52368094)
作者：檀羽冲
链接：http://www.jianshu.com/p/3a365f273439
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
            
