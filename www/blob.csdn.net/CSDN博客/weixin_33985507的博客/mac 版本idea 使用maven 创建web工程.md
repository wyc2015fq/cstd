# mac 版本idea 使用maven 创建web工程 - weixin_33985507的博客 - CSDN博客
2019年01月24日 11:14:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
1:mac电脑
2:idea
3:maven  
1:新建工程，选择maven，在选择create from archetype
![7285342-66f3aadf8209f952.png](https://upload-images.jianshu.io/upload_images/7285342-66f3aadf8209f952.png)
2:
![7285342-0323b2a788f7904b.png](https://upload-images.jianshu.io/upload_images/7285342-0323b2a788f7904b.png)
3:注意maven 路径
![7285342-8c0da13d453695a5.png](https://upload-images.jianshu.io/upload_images/7285342-8c0da13d453695a5.png)
4:这一步就不用更改什么了，直接next就好了
![7285342-0caba27533595913.png](https://upload-images.jianshu.io/upload_images/7285342-0caba27533595913.png)
5:创建成功，工程目录显示
![7285342-0e2c581872c5f9ed.png](https://upload-images.jianshu.io/upload_images/7285342-0e2c581872c5f9ed.png)
6: 先创建一个包， testPackage，颜色是黑色的，
![7285342-c35a295fb081cce3.png](https://upload-images.jianshu.io/upload_images/7285342-c35a295fb081cce3.png)
7:选中testpackage包，右键----》》》》选择完毕会发现包颜色发生改变了，
![7285342-21085bc2e237907f.png](https://upload-images.jianshu.io/upload_images/7285342-21085bc2e237907f.png)
8:
（1）在testpackage 包下创建myServlet类 
（2）此时会报错，找不到httpservlelt 类，
（3）选择alt + enter 键，选择最后一个，添加依赖，程序会自动在pom.xml添加httpservlet的依赖，（如果添加之后还是报错，在最右边选择maven，刷新一下就好了）
![7285342-e09873da2c943b41.png](https://upload-images.jianshu.io/upload_images/7285342-e09873da2c943b41.png)
（4）并实现HttpServlet 接口doget方法，在没收到请求时，跳转到hello.jsp页面
req.getRequestDispatcher("hello.jsp").forward(req,resp);
![7285342-3a62837739c3195c.png](https://upload-images.jianshu.io/upload_images/7285342-3a62837739c3195c.png)
9:设置映射
![7285342-718d69797f1acb20.png](https://upload-images.jianshu.io/upload_images/7285342-718d69797f1acb20.png)
10:创建hello.jsp页面
![7285342-bf99616fef3de900.png](https://upload-images.jianshu.io/upload_images/7285342-bf99616fef3de900.png)
11:配置tomcat，
![7285342-9efd5f3d9e427c88.png](https://upload-images.jianshu.io/upload_images/7285342-9efd5f3d9e427c88.png)

![7285342-4a30f76b95357169.png](https://upload-images.jianshu.io/upload_images/7285342-4a30f76b95357169.png)
12:运行tomcat
![7285342-310e1d501212dc29.png](https://upload-images.jianshu.io/upload_images/7285342-310e1d501212dc29.png)
