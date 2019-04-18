# 企业帐号进行IPA的打包的详细流程(部署到蒲公英上面的,注册上传应用即可。蒲公英不限制证书，可以用个人（公司）开发者或企业开发者账号发布)... - weixin_33985507的博客 - CSDN博客
2018年04月19日 15:24:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：49
参考：[https://blog.csdn.net/iOS_CocoaMrLiuhe/article/details/53301427](https://link.jianshu.com?t=https%3A%2F%2Fblog.csdn.net%2FiOS_CocoaMrLiuhe%2Farticle%2Fdetails%2F53301427)
提示：蒲公英的收费标准。
蒲公英里面的企业签名服务： 付费企业签名不用上架到App Store免添加UDID。（没有试用版和免费版）
下载流程：上传ad hoc的ipa包—选择付费—我方签名—邮件/短信通知—获取签名包请点击：[https://zc.pgyer.com/signature?sa=a2e182a0e719cecff33e119d34eee623](https://link.jianshu.com?t=https%3A%2F%2Fzc.pgyer.com%2Fsignature%3Fsa%3Da2e182a0e719cecff33e119d34eee623)
费用：
内测版 600元/月（分发量较小 安装量上限：200）
公测版 1000元/月（分发量中等 安装量上限：2000）
分发版 2000元/月（分发量较大 无限制安装）
更新 100元/次 （不可以改变APP名称以及bundleid，仅供已购客户更新版本使用）
企业签名应用不支持推送且需要信任证书才可以正常使用。
（内测、公测、分发版三者的区别就是安装数量和收费标准。）
1，企业帐号介绍
（1）使用企业开发帐号，我们可以发布一个 ipa 放到网上，所有人（包括越狱及非越狱设备）都可以直接通过链接下载安装，而不需要通过 AppStore 下载，也不需要安装任何证书。
（2）当然，使用企业帐号发布的 iOS 应用是不能提交到 AppStore 上的。而且企业级开发账号也比个人帐号更贵些（299刀/年）。
（3）既然叫企业帐号，就说明是用来开发企业自己的内部应用，给自己的员工使用的。所以不要用企业号做大规模应用分发的一个渠道，否则有可能会被苹果封账号。
2，IPA打包
（1）首先要上苹果开发者中心，生成发布证书和相关配置文件。然后下载到本地安装下，这个我就不具体说明了。
（2）打开项目，在“General”->“Team”中选择团队名称。
![2127575-719f3c71537d43e7.png](https://upload-images.jianshu.io/upload_images/2127575-719f3c71537d43e7.png)
屏幕快照 2018-04-19 下午3.19.16.png
（3）在“Build Settings” -> “ Code Signing ”区域中选择发布证书。
![2127575-f1c5e6d6ee741db4.png](https://upload-images.jianshu.io/upload_images/2127575-f1c5e6d6ee741db4.png)
屏幕快照 2018-04-19 下午3.19.37.png
（4）发布编译目标选择“Generic iOS Device”
![2127575-611e61ebb3d29620.png](https://upload-images.jianshu.io/upload_images/2127575-611e61ebb3d29620.png)
屏幕快照 2018-04-19 下午3.19.47.png
5）顶部菜单选择“Product”->“Archive”
![2127575-43704441a5e528fe.png](https://upload-images.jianshu.io/upload_images/2127575-43704441a5e528fe.png)
屏幕快照 2018-04-19 下午3.19.59.png
（6）在弹出的界面中点击“Export ...” 进入打包方式选择界面。
![2127575-991df31c28454266.png](https://upload-images.jianshu.io/upload_images/2127575-991df31c28454266.png)
屏幕快照 2018-04-19 下午3.20.06.png
（7）选择“Sava for Enterprise Deployment” 注意第一个是打包发布到APPStore； 第二个是内测的只有绑定设备才可以安装（需要绑定UDID） ；第三个是发布版本所有人可以安装下载!
坑:一次选择了第二个,导致只有我可以安装。。。。
![2127575-1f7404043bb05881.png](https://upload-images.jianshu.io/upload_images/2127575-1f7404043bb05881.png)
屏幕快照 2018-04-19 下午3.34.19.png
（8）选择对应的企业帐号，然后继续即可。(以下就是一直继续就行了)
（9）接下来就是安装设备的要求选择。我们选择第一项（默认项），让所有设备都可以安装。
（10）接下来是确认页面，我们可以核对下各个配置是否正确。点击继续即可!
（11）接下来配置 .plist 文件的相关信息：应用名、发布地址、图标地址、大图地址。
（12）然后选择点击“Export”就可以导出.ipa安装包。
