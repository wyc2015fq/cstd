# iOS 极光推送接收不到推送消息 - weixin_33985507的博客 - CSDN博客
2017年10月23日 17:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：17
[两种鉴权方式的配置](https://link.jianshu.com?t=https://docs.jiguang.cn/jpush/client/iOS/ios_cer_guide/)
公司需要配置极光，以前弄过百度的，以为差不多，结果遇坑了，在此记录下
备注：使用的是3.0.7版本的、手机iOS 10 +
1、一开始按照官方介绍 .p12 方式鉴权方式，按照提示 配置了
![7407369-bc4a1633afb1c391.png](https://upload-images.jianshu.io/upload_images/7407369-bc4a1633afb1c391.png)
第二种，第一种并没有配置，结果全部配置好之后，消息提示发送成功。但是推送记录中
![7407369-3228f3902428188b.png](https://upload-images.jianshu.io/upload_images/7407369-3228f3902428188b.png)
一直提示目标和成功都为 0。然后按照第二种鉴权试了  也是不行，最后试了下 开发阶段使用的推送配置，然后在两种都配置(极光后台配置并验证成功)的情况下，推送成功。记录一下这个坑
2、容易忽略的点  capabilities 开关配置
![7407369-90a410f786cb40f5.png](https://upload-images.jianshu.io/upload_images/7407369-90a410f786cb40f5.png)
只打开 push Notification 并确认对号已开启 还不行，还要设置 background Modes -> 开启 -> remote notifications 打开；< 这个官方文档没看到，一个小坑 >
3、我是pod导入的JPush，使用
![7407369-b6d66b37f07f77eb.png](https://upload-images.jianshu.io/upload_images/7407369-b6d66b37f07f77eb.png)
这个初始化时，忘记导入 AdSupport.framework (获取IDFA需要；如果不使用IDFA，请不要添加)，重新看了文档才找到。。。
4、获取registrationID的接口，可以直接对某个机子测试：
//2.1.9版本新增获取registration id block接口。
[JPUSHService registrationIDCompletionHandler:^(int resCode, NSString *registrationID) {
if(resCode == 0){
NSLog(@"registrationID获取成功：%@",registrationID);
}
else{
NSLog(@"registrationID获取失败，code：%d",resCode);
}
}];
5、另外说下 .p12 文件导出时，.p12文件格式一直不能选择，最后看到一小哥的建议，直接拖到登录里面 [iOS开发无法导出p12证书的问题解决办法](https://www.jianshu.com/p/bfec1ade1420)
