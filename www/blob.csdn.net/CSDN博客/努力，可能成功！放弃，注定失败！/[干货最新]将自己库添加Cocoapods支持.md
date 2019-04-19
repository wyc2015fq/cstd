# [干货最新]将自己库添加Cocoapods支持 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年09月22日 19:05:34[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：290标签：[将自己库添加Cocoapods支持																[iOS学习资料																[iOS 资源大全](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)](https://so.csdn.net/so/search/s.do?q=将自己库添加Cocoapods支持&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
给库添加Cocoapods支持, 使这个工具使用起来更加方便, 更好的使用Cocoapods, 助力iOS程序开发, 下面进入正题, 想要实现这个过程, 绝对不虚此读.
- 
首先写好一个要添加Cocoapods支持的库 这是我第一个添加Cocoapods支持的工具[iOS7兼容自定义带动画的AlertViewController](https://github.com/summerxx27/XTAlertViewController) 你可以看看
- 
注册trunk
```
// 升级至最新版本
$sudo gem install cocoa pods
// 完成注册 邮箱和GitHub用户名字
pod trunk register 1005430006@qq.com 'summerxx27' --verbose // 之后点击邮箱链接进行验证
```
3 查看注册信息如图
`pod trunk me`
![](http://upload-images.jianshu.io/upload_images/1506501-fbaf40ab00274732.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
注册信息
4 创建 .spec文件
```
// XTAlertViewController : 库名字
pod spec create XTAlertViewController
```
5 编辑文件 vim
``vim XTAlertViewController.podspec``
6 填写信息: 看名字就知道意思了. 具体还可以看官方文档 我的XTAlertViewController.podspec这个文件是这样写的, 或者找一个优秀的第三方, 模仿着写也是可以的
```
```
Pod::Spec.new do |s|
s.name = 'XTAlertViewController'
s.version = '1.0.1'
s.license = 'MIT'
s.summary = 'A view like UIAlertViewController on iOS.'
s.homepage = 'https://github.com/summerxx27/XTAlertViewController'
s.authors = { 'summerxx27' => '1005430006@qq.com' }
s.source = { :git => 'https://github.com/summerxx27/XTAlertViewController.git', :tag => s.version.to_s }
s.requires_arc = true
s.ios.deployment_target = '7.0'
s.source_files = 'XTAlertViewController/*.{h,m}'
end
```
```
7 验证一下本地文件编写是否正确
``pod lib lint XTAlertViewController.podspec``
![](http://upload-images.jianshu.io/upload_images/1506501-d6f76376126de37e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
成功
8 把编辑好的文件推送到GitHub (库文件 和 .spec文件)(使用GitHub Desktop就可以) 并且打上标记
```
git tag '1.0.1'
git push --tags
```
9 把编写的文件告诉Cocoapods
`pod trunk push XTAlertViewController.podspec`
10 之后使用查找命令就可以看到了
![](http://upload-images.jianshu.io/upload_images/1506501-98b92f5fc08ad1ec.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
查找
注意事项
> 
库文件应该在仓库的第一层目录(具体如不清楚 你可以下载我文章开头提到的XTAlertViewController 看一下文件层级)
总结 : 大致的过程就是这个样子, 博主在尝试的时候 走了不少弯路, 昨天一直到凌晨也没有成功, 好在最后成功了, 按照这个步骤, 基本没有错误, .podspec这个文件如果填写正确的话.
[我的第一个支持Cocoapods的工具 就制作完成了, 你可以点击这里看见这个工具的介绍](http://www.jianshu.com/p/80be099e9154)
参考 : [整个步骤是学习的 -](http://www.cnblogs.com/dsxniubility/p/4813864.html)[董铂然的博客](http://www.cnblogs.com/dsxniubility/)
感谢您的阅读, 同时祝您过程顺利 ! 
走心文章, 值得点赞 ---文/夏天然后
[微博-点我@夏天是个大人了](http://weibo.com/345100543) || QQ群: 498143780
End 文/ 夏天然后
文／夏天然后（简书作者）
原文链接：http://www.jianshu.com/p/489520b69d8b
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
