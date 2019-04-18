# iOS APP 发布 - weixin_33985507的博客 - CSDN博客
2016年12月06日 14:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
**一.需要的证书 、定位及作用**
     1、开发者证书（分为开发----***[iOS](https://link.jianshu.com?t=http://lib.csdn.net/base/ios) Development*** 和发布---- **i*OS Distribution***两种），这个是最基础的，不论是真机调试，还是上传到appstore都是需要的，是一个基证书，用来证明自己开发者身份的；
     2、***appID***,这是每一个应用的独立标识，在设置项中可以配置该应用的权限，比如是否用到了PassBook,GameCenter,以及更常见的push服务，如果选中了push服务，那么就可以创建生成下面第3条所提到的推送证书，***所以，在所有和推送相关的配置中，首先要做的就是先开通支持推送服务的appID***；
3、推送证书（分为开发和发布两种，类型分别为***APNs Development iOS***,***APNs Distribution iOS***）,该证书在appID配置中创建生成，和开发者证书一样，安装到开发电脑上；
4、***Provisioning Profiles***,这个东西是很有苹果特色的一个东西,***该文件将appID,开发者证书，硬件Device绑定到一块儿***，***在开发者中心配置好后可以添加到Xcode上，也可以直接在Xcode上连接开发者中心生成***，真机调试时需要在***Provisioning Profiles***文件中添加***真机的udid***；是真机调试和必架必备之珍品；
> 
平常我们的制作流程一般都是按以上序列进行，***先利用开发者帐号登陆开发者中心，创建开发者证书，appID,在appID中开通推送服务，在开通推送服务的选项下面创建推送证书（服务器端的推送证书见下文），之后在***Provisioning Profiles***文件中绑定所有的证书id,添加调试真机等***；
