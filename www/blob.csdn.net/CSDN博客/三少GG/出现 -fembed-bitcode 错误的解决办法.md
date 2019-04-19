# 出现"-fembed-bitcode"错误的解决办法 - 三少GG - CSDN博客
2016年02月17日 22:05:17[三少GG](https://me.csdn.net/scut1135)阅读数：1429

错误提示
> 
-fembed-bitcode is not supported on versions of iOS prior to 6.0
![这里写图片描述](https://img-blog.csdn.net/20150924121113097)
这时候到build setting里面设置查找”enable_bitcode”, 并把设置改为”NO” 
![这里写图片描述](https://img-blog.csdn.net/20150924121230959)
## Impact of Xcode build options “Enable bitcode” Yes/No
来自: 
[http://stackoverflow.com/questions/31088618/impact-of-xcode-build-options-enable-bitcode-yes-no/32753202#32753202](http://stackoverflow.com/questions/31088618/impact-of-xcode-build-options-enable-bitcode-yes-no/32753202#32753202)
From the [docs](https://developer.apple.com/library/prerelease/watchos/documentation/IDEs/Conceptual/AppDistributionGuide/AppThinning/AppThinning.html#//apple_ref/doc/uid/TP40012582-CH35-SW2)
- can I use the above method without any negative impact and without compromising a future appstore submission?
> 
Bitcode will allow apple to optimise the app without you having to submit another build. But, you can only enable this feature if all frameworks and apps in the app bundle have this feature enabled. Having it helps, but not having it should not have any negative
 impact.
- What does the ENABLE_BITCODE actually do, will it be a non-optional requirement in the future?
> 
For iOS apps, bitcode is the default, but optional. If you provide bitcode, all apps and frameworks in the app bundle need to include bitcode. For watchOS apps, bitcode is required.
- Are there any performance impacts if I enable / disable it?
> 
The App Store and operating system optimize the installation of iOS and watchOS apps by tailoring app delivery to the capabilities of the user’s particular device, with minimal footprint. This optimization, called app thinning, lets you create apps that use
 the most device features, occupy minimum disk space, and accommodate future updates that can be applied by Apple. Faster downloads and more space for other apps and content provides a better user experience.
There should not be any performance impacts.
## Xcode 7中ENABLE_BITCODE错误解决
来自: 
[http://anotheren.com/48](http://anotheren.com/48)
最近编译中出现了ENABLE_BITCODE报错的问题，我使用的是Xcode7bate4(7A165t)，使用调试的iOS版本是8.4正式版。开始的时候我还以为是iOS8.4与Xcode7有兼容问题，顺便把手机升到了9.0(13A4305g)，问题依旧。
认真的Google了一下，发现ENABLE_BITCODE是Xcode的一项新特性，引用一下官方说明： 
[https://developer.apple.com/library/prerelease/watchos/documentation/IDEs/Conceptual/AppDistributionGuide/AppThinning/AppThinning.html#//apple_ref/doc/uid/TP40012582-CH35-SW2](https://developer.apple.com/library/prerelease/watchos/documentation/IDEs/Conceptual/AppDistributionGuide/AppThinning/AppThinning.html#//apple_ref/doc/uid/TP40012582-CH35-SW2)
也就是对于iOS的应用来说，Bitcode是可选的，但对于新的watchOS而言，它是必须的。很明显，是由于我使用的PLCameraStreamingKit未包含bitcode，导致编译失败。所以，我们需要找Enable Bitcode选项，将默认的Yes改为No即可临时解决这个问题。那什么时候可以彻底根治这个问题呢，只有等到你工程中所有使用的框架均支持Bitcode才行。
