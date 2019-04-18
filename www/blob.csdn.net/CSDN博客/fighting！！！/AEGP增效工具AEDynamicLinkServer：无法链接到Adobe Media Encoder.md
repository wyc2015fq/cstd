# AEGP增效工具AEDynamicLinkServer：无法链接到Adobe Media Encoder - fighting！！！ - CSDN博客
2017年12月16日 20:51:41[dujiahei](https://me.csdn.net/dujiahei)阅读数：21536
原地址：AEGP增效工具AEDynamicLinkServer：无法链接到Adobe Media Encoder_影视后期特效培训-薛大龙_新浪博客 http://blog.sina.com.cn/s/blog_9948ff720102wzif.html
AEGP增效工具AEDynamicLinkServer：无法链接到Adobe Media Encoder
![f7e143c63751d4a5e81f978edf6a3e99002.png](http://i.xsteach.cn/uploaded/f7/e1/43/f7e143c63751d4a5e81f978edf6a3e99002.png@850w_650h_1l_0e_0r_0sh.png)
解决这个问题首先要求ae和Adobe Media Encoder同版本
然后打开cmd
![d2d345836bd84422817f0125c2d7ac64002.png](http://i.xsteach.cn/uploaded/d2/d3/45/d2d345836bd84422817f0125c2d7ac64002.png@850w_650h_1l_0e_0r_0sh.png)
鼠标右键
![9537262433a349c20cac7a84f8c6a91c002.png](http://i.xsteach.cn/uploaded/95/37/26/9537262433a349c20cac7a84f8c6a91c002.png@850w_650h_1l_0e_0r_0sh.png)
输入
mklink /J "C:\Program Files\Adobe\Adobe Media Encoder CC 2015" "这里改成你安装Media Encoder的目录"
![ff789ef00c9042aeadbf50688b84d008002.png](http://i.xsteach.cn/uploaded/ff/78/9e/ff789ef00c9042aeadbf50688b84d008002.png@850w_650h_1l_0e_0r_0sh.png)
提示创建的联接就是成功，这个时候重启ae执行添加到Adobe Media Encoder队列就可以了
