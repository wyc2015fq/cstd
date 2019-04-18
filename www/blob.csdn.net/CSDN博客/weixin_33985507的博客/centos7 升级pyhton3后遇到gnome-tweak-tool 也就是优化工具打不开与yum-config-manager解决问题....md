# centos7 升级pyhton3后遇到gnome-tweak-tool 也就是优化工具打不开与yum-config-manager解决问题... - weixin_33985507的博客 - CSDN博客
2017年06月04日 00:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：18
问题1：
![6020509-7478676e8cb20a04.png](https://upload-images.jianshu.io/upload_images/6020509-7478676e8cb20a04.png)
图片.png
解决办法：find到gnome-tweak-tool 的位置vim进去路径改成/usr/bin/python2.7
![6020509-3e1a4e23d0b24b1b.png](https://upload-images.jianshu.io/upload_images/6020509-3e1a4e23d0b24b1b.png)
图片.png
问题2：
![6020509-96bd87b6fc058049.png](https://upload-images.jianshu.io/upload_images/6020509-96bd87b6fc058049.png)
图片.png
解决办法：find到yum-config-manager 的位置vim进去路径改成/usr/bin/python2.7
![6020509-00515bd2e4e1d4bd.png](https://upload-images.jianshu.io/upload_images/6020509-00515bd2e4e1d4bd.png)
图片.png
