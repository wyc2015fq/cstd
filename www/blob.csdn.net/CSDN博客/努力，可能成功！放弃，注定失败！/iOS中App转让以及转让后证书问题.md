# iOS中App转让以及转让后证书问题 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年01月17日 11:39:19[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：81
由于公司之前做的产品是帮分公司做的，但是分公司的营业执照当时没有下来，所以一直以来的开发者账号是以主公司名义申请的。这不，app开发完成也上线好几版了，现在公司要以分公司的名义创建开发者证书(不会创建的童鞋看一下我这篇文章[开发者账号申请2016-12](https://www.jianshu.com/p/c4d5dd3ccf9d)),然后把当前app转让到此账号下。 说实话之前也考虑过重新开发个工程，但是考虑到此项目中用到的一些和别的第三方公司有合作的sdk，并提供过正式的相关key，所以还是转让保险点吧。那就干吧~
在此，将原帐号称为 A帐号，新的帐号 称为 B帐号。现在需要将A帐号中的App 转让到 B帐号中。
# 1.登录B账号，首先找到Team ID(之后会用到)
![](https://upload-images.jianshu.io/upload_images/1903176-01e88a071d191988.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Paste_Image.png
# 2.注销当前账号，登录A账号,找到对应app，在app信息中点击转让
![](https://upload-images.jianshu.io/upload_images/1903176-424dbe75a67b3812.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Paste_Image.png
# 3.点击继续
![](https://upload-images.jianshu.io/upload_images/1903176-c11ee0bbbdc33e05.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Paste_Image.png
# 4.输入接收者的apple id和team id
![](https://upload-images.jianshu.io/upload_images/1903176-9de3454108425417.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Paste_Image.png
# 5.点击转让
![](https://upload-images.jianshu.io/upload_images/1903176-4f28ed31c8515a6b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Paste_Image.png
# 6.登录B账号，然后到iTunes Connect内
![](https://upload-images.jianshu.io/upload_images/1903176-5cbe1f0bbc43c57b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Paste_Image.png
# 7.点击
![](https://upload-images.jianshu.io/upload_images/1903176-2c2b1b7badf61c71.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Paste_Image.png
# 8.点击review
![](https://upload-images.jianshu.io/upload_images/1903176-4082896d621a24d4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Paste_Image.png
# 9.信息填写，韩国的什么就不用填了
![](https://upload-images.jianshu.io/upload_images/1903176-d5c78724d9b53e39.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Paste_Image.png
# 10.完成后在第八步那个界面点击done按钮(当时是review)
好了，转让app便完成了。。。。。。大概5-10分钟，当收到了苹果发来已经转让成功的信息后，在appstore里查看，发现名字已经改了~
# 转移后证书的问题，我在这里提一下
转移之后，到开发者中心查看证书，此时只有APP IDs是一起带过来的，然而其他证书都没了，如下图
![](https://upload-images.jianshu.io/upload_images/1903176-6b88e3207ea9fb87.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Paste_Image.png
点击图中的标记，依次创建开发者证书，描述文件(创建描述文件之前记得将iPhone设备添加，需要测试的。。)
其实到这里的步骤就跟我们之前创建证书都是一样的了，只不过app ids指定的唯一标识buddle id已经由app带过来给我们了。
作者：哈哈大p孩
链接：https://www.jianshu.com/p/5632ac9a363c
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
