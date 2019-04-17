# iOS初学之填坑总结 - DEVELOPER - CSDN博客





2019年04月01日 17:47:58[学术袁](https://me.csdn.net/u012827205)阅读数：23








## Bug(2019-4-1)

![Linker command failed with exit code 1(use-v to see invocation)](https://img-blog.csdnimg.cn/20190401172701565.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=,size_16,color_FFFFFF,t_70)

Linker command failed with exit code 1(use-v to see invocation)

为什么会出现这样的问题呢？
![Build Phases](https://img-blog.csdnimg.cn/20190401174239240.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=,size_16,color_FFFFFF,t_70)

是因为在 `Build Phases` 下的 `Compile Sources` 中没有把你项目中的.swift 文件添加进入，怎么会找的到（Link），怎么能够进行编译呢？！所以，修改这个bug，只需要把你的项目中文件添加进入就ok。
## Bug(2019-4-2)

在使用xcode9.0集成HandyJSON的时候，安装成功，编译却一直不会通过。使尽各种方法，最终还是给我尝试到了。这里分享一下我的经验：

在我配置了cocoaPods之后，在Podfile文件中做如下配置：

```
# Uncomment the next line to define a global platform for your project
platform :ios, '11.0'

target 'Demo' do
  # Comment the next line if you're not using Swift and don't want to use dynamic frameworks
  use_frameworks!

  # Pods for Demo
  pod 'Alamofire'
  pod 'SwiftyJSON'
  pod 'HandyJSON','4.1'
  #pod 'HandyJSON', git: 'https://github.com/alibaba/HandyJSON.git' , branch: 'dev_for_swift5.0'

end
```

可以看到我在其中一共集成了三个函数库：`Alamofire`   ,   `SwiftyJSON`,  `HandyJSON`.

第一次使用的HandyJSON的版本是 `'HandyJSON', '4.2.0'` 然后是 `'HandyJSON', '4.2.0-beta1'` 安装之后的编译结果都会报错。

错误内容是：`Undefined symbols "_swift_getFieldAt"`

这个在github上找到的原因是，函数库自身在适配swift时的bug。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190402173832910.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=,size_16,color_FFFFFF,t_70)
遇到问题了，当然要想法设法的去解决。然后进行百度搜索，尝试很多博客中指出的方法，没有成功。这时候一个上午已经过去了，下午也到15点了。没办法进github翻翻 issue 吧。也找了好多同样问题的帖子。
[解决方法一](https://github.com/alibaba/HandyJSON/issues/307)
[解决方法二](https://github.com/alibaba/HandyJSON/issues/312)

都不管用，尝试了高版本的HandyJSON几个。由原来的旧问题，变成了新的问题。如下图展示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190402173854650.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=,size_16,color_FFFFFF,t_70)
错误内容是：`"_swift_getTypeByMangledNameInContext", referenced from:`

有说使用高版本xcode进行适配。但是我的电脑又不支持太高的xcode版本。而且我突然想到这个HandyJSON之前的旧版本也一直在使用啊，而且在issue上提出了好多相关的问题。由此，我想到了试一试低版本的HandyJSON。然后就用了上面的`pod 'HandyJSON','4.1'`
**果然就成功了。** 所以，当有朋友遇到这种编译问题（只针对初学者）可以尝试一下各种版本进行试用！
## Bug(2019-4-9)

![Terminating app due to uncaught exception NSUnKnownKeyException](https://img-blog.csdnimg.cn/20190409122906993.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=,size_16,color_FFFFFF,t_70)

一个XIB小问题让我处理了2天，哈哈！问题就在于这里：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409123431434.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=,size_16,color_FFFFFF,t_70)
第一次使用XIB画图的时候是ok的，只是后来修改了一下“大V的变量名”然后在对应的XIB中没能同步修改，所以就报上面的错误。当然，从这个过程中学到的则是，如果去看打印的错误日志。因为上面标注的很清楚：`Terminating app due to uncaught exception NSUnKnownKeyException: this class is not key value coding-compliant for the key vipImageView .`

## Bug(2019-4-10)

![Value of type [UIView] has no member compactMap](https://img-blog.csdnimg.cn/20190410105745177.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=,size_16,color_FFFFFF,t_70)

引入第三方函数库报错，`Value of type [UIView] has no member compactMap`

是因为我直接引入了 `pod 'IBAnimatable', '5.1.0'`
[根据官方，作出正确的解决方法](https://github.com/IBAnimatable/IBAnimatable/issues/599)
```
# Uncomment the next line to define a global platform for your project
platform :ios, '11.0'

target 'Demo' do
  # Comment the next line if you're not using Swift and don't want to use dynamic frameworks
  use_frameworks!

  # Pods for Demo
  pod 'Alamofire'
  pod 'SwiftyJSON'
  pod 'HandyJSON','4.1'
  #pod 'HandyJSON', git: 'https://github.com/alibaba/HandyJSON.git' , branch: 'dev_for_swift5.0'

  pod 'Kingfisher', '4.1.1'
  # Pods for JNUHonorSchoolStudentPlatform
  pod 'IBAnimatable', '5.0.0'
end

post_install do |installer|
  installer.pods_project.targets.each do |target|
    if target.name == 'IBAnimatable'
      target.build_configurations.each do |config|
        config.build_settings['SWIFT_VERSION'] = '4.0'
      end
    end
  end
end
```









