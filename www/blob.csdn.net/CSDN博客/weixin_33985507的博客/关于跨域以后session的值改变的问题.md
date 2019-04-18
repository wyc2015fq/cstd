# 关于跨域以后session的值改变的问题 - weixin_33985507的博客 - CSDN博客
2017年12月11日 18:50:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：22
这个问题真是困扰了我们好几天呀
我们的项目登录以后
![5417710-55036dadac81a462.png](https://upload-images.jianshu.io/upload_images/5417710-55036dadac81a462.png)
在cookie下面会生成sessionId，这其实也没有什么，可是我们这个项目要iframe嵌套其他服务器上的项目，要从本页面传数据过去，就得使用跨域，跨域后竟然把本项目的sessionId就给覆盖了，最后解决办法是，把本项目的sessionId换一个名字就不会被覆盖了
