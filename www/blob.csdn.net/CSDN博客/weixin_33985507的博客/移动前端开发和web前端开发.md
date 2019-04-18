# 移动前端开发和web前端开发 - weixin_33985507的博客 - CSDN博客
2017年03月27日 23:43:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：44
## 区别
直接上一张图
![1784460-5d5102b6f48d0df3.png](https://upload-images.jianshu.io/upload_images/1784460-5d5102b6f48d0df3.png)
两个的关系和区别
## 前端
前端是个很大的概念，我的理解是用户能够看到，直接接触到的层面都算是前端，比如IOS客户端界面，安卓客户端界面，网页界面，甚至PC/MAC 桌面端软件界面；现在最常见的说法一般是指Web前端，也就是针对于网页端开发的工作。
- 
**Web App：**指的是【[Web application](https://link.jianshu.com?t=http%3A%2F%2Fen.wikipedia.org%2Fwiki%2FWeb_application)】，也就是以浏览器作为客户端的软件。
- 
**Mobile Web App ：**当然就是指在手机端打开的Web App啦。
## 移动客户端的开发类型
- 
**Native App（原生APP）：**也就是完全使用移动设备系统语言写的客户端，iPhone iPad就是纯Object-C，安卓就是纯JAVA, 就是用户看到的界面啦体验到的交互啦都是原生的。这是性能最棒的开发方式，但灵活性就没下面的好。
- 
**Web App：**这个就是在移动浏览器里打开的，纯HTML+CSS+JS，说白了就是个网页，只不过非常的富应用，比如手机浏览器访问的GMAIL啥啥的。但说白了就是在浏览器里打开的页面。。IOS支持可以在桌面创建访问的快捷方式，但是说到底还是打开Safari跑。。而且对设备硬件的接口什么的挺薄弱。
- 
**Hybrid App：**[[HTML5 in mobile devices](https://link.jianshu.com?t=http%3A%2F%2Fen.wikipedia.org%2Fwiki%2FHTML5_in_mobile_devices%23Hybrid_Mobile_Apps)] 我觉得这个更为合适一些。实际上是使用原生写了一个容器，然后使用HTML+CSS+JS来实现用户界面和交互。Web App的短处便可以克服（因为自己写的容器可以辅助暴露偏底层的接口，比如本地存储或者麦克风控制之类），同时比起纯原生的java或者object-c开发灵活性要高（更新可以更快更迅速，也不依赖于市场，因为说白了，就是自己下载更新网页资源。。）实际上这种方式已经不限于移动端。。豌豆荚其实是个pc端的hybrid app 哇~~~ 而且说实在的，桌面开发的性能就现在来说要比移动好很多。
