# OTA方式来分发iOS的应用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年09月07日 09:34:12[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：166
**OTA分发的实现方式**
针对iOS应用分发，需要在服务器里上包括三个文件，来实现OTA方式的分发
1.ipa文件，也就是应用安装包，可以是开发者证书签名的，也可以是企业证书。
2.plist文件，主要用来说明iOS应用的安装信息，主要的安装信息包括ipa文件的名称和存放路径。可以通过Xcode来创建plist文件，具体创建过程和plist文件这里就不赘述了。
3、一个提供itms-services协议链接的页面，同时该页面是通过HTTPS的方式访问。itms-services协议链接指向plist文件的路径，例如：
> 
<a href="itms-services://?action=download-manifest&url=https://gitee.com/DevYoung/MXFootBall/raw/master/manifest.plist">下载安装</a>
有了上面三个文件，apple设备就可以通过提供HTTPS服务器，读取plist文件的iOS安装的ipa文件的路径，从而实现下载和安装。
iOS 自定义服务器下载安装ipa流程
一、在码云[https://gitee.com](https://gitee.com/)上新建一个项目,并命名MXFootBall,需要注意的是一定要是public模式,得到工程仓库:[https://gitee.com/DevYoung/MXFootBall.git](https://gitee.com/DevYoung/MXFootBall.git)
二、新建iOS测试工程项目,并命名MXFootBall
三、将iOS项目打包,注意:这里为了测试方便,一定要采用Apple官方的打包模式.在进行到导出ipa的时候,进行到下一步时:
![](https://upload-images.jianshu.io/upload_images/4120050-87eb6845e93af97a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
需要将图示部分选中,为了导出manifest.plist文件
四、点击Next,接下来出现如下界面
![](https://upload-images.jianshu.io/upload_images/4120050-bf34986b03d35fbb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
plist的链接要求一定是https的，而且必须是公网ssl.所以暂时填写之前仓库的地址
五、最后到处ipa,得到如下内容:
![](https://upload-images.jianshu.io/upload_images/4120050-70ec1862dcc73fff.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
manifest.plist内容如下
![](https://upload-images.jianshu.io/upload_images/4120050-06a436fb569bb1b8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
1
六、接下来,将得到的manifest.plist上传至仓库中
![](https://upload-images.jianshu.io/upload_images/4120050-8e83e2b5c1bcf3ce.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
选择上传文件
![](https://upload-images.jianshu.io/upload_images/4120050-e93c023627aeb45c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
拖入manifest.plist然后提交
七、获取manifest.plist下载链接:
![](https://upload-images.jianshu.io/upload_images/4120050-bb477811322a4e37.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
https://gitee.com/DevYoung/MXFootBall/raw/master/manifest.plist
八、在自己Mac电脑上搭建Apache服务器可参考我的[另一篇文章](https://www.jianshu.com/p/9bd12b55e3b1)
在目录下/Library/WebServer/Documents新建文件夹MXFootBall
1、将iOS工程导出的ipa放入 文件夹MXFootBall中
2、在文件夹MXFootBall中新建index.html
内容如下：
![](https://upload-images.jianshu.io/upload_images/4120050-96e1c42ca996d5b1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
九、获取MXFootBall.ipa下载链接
> 
http://192.168.3.83/MXFootBall/index.html
十、更新仓库中manifest.plist的内容
![](https://upload-images.jianshu.io/upload_images/4120050-4bcb884f141b9c9d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
十一、可以进行App下载了.在手机浏览器上访问
http://192.168.3.83/MXFootBall/index.html
![](https://upload-images.jianshu.io/upload_images/4120050-631647a634cae6cf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/639/format/webp)
作者：没技术的BUG开发攻城狮
链接：https://www.jianshu.com/p/d47ca5c15dd9
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
