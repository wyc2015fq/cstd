# 微信、QQ、微博分享 - weixin_33985507的博客 - CSDN博客
2018年02月01日 09:48:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
微信分享：http://qydev.weixin.qq.com/wiki/index.php?title=%E5%BE%AE%E4%BF%A1JS-SDK%E6%8E%A5%E5%8F%A3#.E8.8E.B7.E5.8F.96.E2.80.9C.E5.88.86.E4.BA.AB.E5.88.B0.E6.9C.8B.E5.8F.8B.E5.9C.88.E2.80.9D.E6.8C.89.E9.92.AE.E7.82.B9.E5.87.BB.E7.8A.B6.E6.80.81.E5.8F.8A.E8.87.AA.E5.AE.9A.E4.B9.89.E5.88.86.E4.BA.AB.E5.86.85.E5.AE.B9.E6.8E.A5.E5.8F.A3
步骤：
**步骤一：引入JS文件：[http://res.wx.qq.com/open/js/jweixin-1.2.0.js](https://link.jianshu.com?t=http%3A%2F%2Fres.wx.qq.com%2Fopen%2Fjs%2Fjweixin-1.2.0.js)**
**步骤二：通过config接口注入权限验证配置：所有需要使用JS-SDK的页面必须先注入配置信息，否则将无法调用**
**步骤三：通过ready接口处理成功验证**
**步骤四：通过error接口处理失败验证**
**获取“分享到朋友圈”按钮点击状态及自定义分享内容接口**
**QQ分享：https://connect.qq.com/intro/sharetoqq**
**微博分享：http://open.weibo.com/sharebutton**
