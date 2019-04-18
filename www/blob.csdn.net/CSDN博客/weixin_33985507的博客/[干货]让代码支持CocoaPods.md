# [干货]让代码支持CocoaPods - weixin_33985507的博客 - CSDN博客
2015年12月08日 17:41:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
#### 1. 注册trunk账号
```
pod trunk register useremail 'username' --verbose
```
第一次尝试的时候报错如下：
`[!] There was an error registering with trunk: getaddrinfo: nodename nor servname provided, or not known`
我这边的情况是开启了ShadowSocks，把ShadowSocks关闭再重试就好了，根本原因是DNS不对，附上解决方案链接：
[https://github.com/CocoaPods/CocoaPods/issues/2174](https://link.jianshu.com?t=https://github.com/CocoaPods/CocoaPods/issues/2174)
注册成功之后会有如下提示：
`[!] Please verify the session by clicking the link in the verification email that has been sent to 670306170@qq.com`
同时你的邮箱会受到一封激活邮件，点击里面的链接激活就可以了
可以使用以下命令查看自己的信息
```
pod trunk me
```
#### 2. 添加podspec描述文件
描述文件的意义是让CocoaPods搜索引擎知道代码作者、名称、版本号、源代码地址、依赖库等等信息。
参考文献：
[http://guides.cocoapods.org/making/specs-and-specs-repo.html](https://link.jianshu.com?t=http://guides.cocoapods.org/making/specs-and-specs-repo.html)
[https://guides.cocoapods.org/syntax/podspec.html#group_root_specification](https://link.jianshu.com?t=https://guides.cocoapods.org/syntax/podspec.html#group_root_specification)
编辑完podspec之后需要用以下命令校验以下文件格式是否正确，校验通过才可以上传。
参考文献里提到的校验命令为：
```
pod spec lint
```
但是我建议用以下命令：
```
pod spec lint NAME.podspec
```
亲测用第一条命令校验通过，但是推送又不成功，之后用第二条命令校验有提示错误。只有完全不报错才可以推送成功，所以推荐用后者校验。
`这一步骤我遇到的几个错误：`
`- WARN | [iOS] license: Unable to find a license file 原因：没有找到License文件，根据规则也可以直接把文本卸载podspec文件里`
`解决方案：文件里面加一句即可：`
`s.license = "Copyright (c) 2015年 Lisa. All rights reserved."`
`- WARN | source: The version should be included in the Git tag.`
`原因：podspec里的version要跟git上的tag相匹配`
`解决方案：`
`git tag '1.0.0'`
`git push --tags`
#### 3. 用trunk推送podspec文件
确保podspec文件已校验通过之后，用以下命令将该podspec文件上传至trunk
```
pod trunk push NAME.podspec
```
成功之后会看到如图：
![569159-aea04c36fdc01c3e.png](https://upload-images.jianshu.io/upload_images/569159-aea04c36fdc01c3e.png)
D5837875-4773-4F7A-8F94-DB4F61761913.png
据说上传之后会有一个审核的过程，用以下命令在cocoapods上搜索看是否能找到证明是否通过审核：
```
pod search LJPagingCove
```
如下图证明已经上传成功:
![569159-a3ae62a7e899ad19.png](https://upload-images.jianshu.io/upload_images/569159-a3ae62a7e899ad19.png)
A61B57AA-9E4B-467D-AD03-287769C09E89.png
据说这个审核过程是要两三天的，但是我这边上传成功之后马上就可以搜到了。如果你那里搜不到的话，执行`pod setup`命令进行本地依赖库更新，然后再搜索。
