# MyEclipse10搭建Strust2开发环境 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [MyEclipse10搭建Strust2开发环境](https://www.cnblogs.com/xdp-gacl/p/3496242.html)



## 一、创建一个JavaWeb项目

启动MyEclipse10  ![](https://images0.cnblogs.com/blog/289233/201312/29110415-769ad3f11c5047978ff4da6e3dfc23d6.png)  ，然后在MyEclipse中创建一个JavaWeb项目，点击【File】---->【New】---->【WebProject】，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29110459-f5713a595ebe4d3dbffe725465b23777.png)

此时将会弹出一个创建项目的对话框，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29110639-6708efb0f37e4d1488bee82ef2eb352c.png)

点击【Finish】按钮后，如果工作空间当前的编译环境和项目需要的编译环境不一致，此时会弹出一个警告对话框，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29110750-1681ae8d6788431789dccdc0710256c8.png)

这个警告对话框说的是“Basic_Struts2”项目的编译环境必须是6.0的，对应的JDK就是JDK1.6，而我们当前的工作空间默认使用的是JDK1.4的编译环境，因此我们需要手动把编译环境设置成JDK1.6

点击![](https://images0.cnblogs.com/blog/289233/201312/29110950-76d740ebe20442fb980bf4ef1e7b9ded.png)这个超链接，将弹出如下图所示的对话框

**![](https://images0.cnblogs.com/blog/289233/201312/29111027-a63cf9f0cdee4247aab761c5f2c8828c.png)**

点击【Apply】或者【OK】按钮后，将弹出如下的对话框

![](https://images0.cnblogs.com/blog/289233/201312/29111159-0595d6977ff14bbca62e8d74cb25517c.png)

直接点击【Yes】即可，此时又返回到了创建项目的那个对话框，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29111304-d15e94497b8f456a8c62c2cd361940b0.png)

点击【Finish】，此时就可以看到创建的项目了，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29111430-c191fc9955eb4c00aab8825d0ca6b6b0.png)

展开项目文件夹，可以看到项目中的文件结构

![](https://images0.cnblogs.com/blog/289233/201312/29113032-5a3e9ba37e66432cabe612317e3ce643.png)

## 二、添加Struts2 Jar包到项目中

我们从[**http://struts.apache.org/download.cgi#struts2312**](http://struts.apache.org/download.cgi#struts2312)下载Strust2的Jar包，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29113236-7feffbe5383543d1a44ecae0f2a162f3.png)

下载完成之后，得到的是一个压缩包![](https://images0.cnblogs.com/blog/289233/201312/29113400-10640094e0d347cd8cda82740f17fc0d.png)，解压到当前目录之后，得到一个![](https://images0.cnblogs.com/blog/289233/201312/29113558-9538fbbc402e42c196dd12e7567006d0.png)文件夹，打开文件夹，可以看到里面有如下的内容

![](https://images0.cnblogs.com/blog/289233/201312/29113656-cdf386d4ec8b4fde84d2763ca1570312.png)

打开【lib】文件夹，可以看到里面有很多的Jar文件(Jar包)，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29113802-f6fffe98e4c14d1cad5739b59afdf690.png)![](https://images0.cnblogs.com/blog/289233/201312/29113847-a25e5557528d4bd59953e8b195f4e881.png)

![](https://images0.cnblogs.com/blog/289233/201312/29113924-c06764743f3d45978cf2c6687d39f149.png)![](https://images0.cnblogs.com/blog/289233/201312/29114011-c0af80e646d94f33a0ccb8a870fe37bc.png)

这里罗列出来的就是Strust2框架的所有Jar包，但是这些Jar包在搭建Struts2开发环境时不是每一个都要用到的，通过参考Strust2的参考文档可以知道，Struts 2 framework开发环境至少需要以下的Jar包：
- **asm-x.x.jar**
- **asm-commons-x.x.jar**
- **asm-tree-x.x.jar**
- **commons-io-X.X.X.jar**
- **commons-lang3-X.X.X.jar**
- **commons-fileupload-X.X.X.jar**
- **freemarker-X.X.X.jar**
- **javassist-X.X.X.jar**
- **ognl-X.X.X.jar**
- **struts2-core-X.X.X.X.jar**
- **xwork-core-X.X.X.jar**

找到Struts 2 framework开发环境必须用到的11个Jar包，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29114317-91e7156d67f64776a8318d65de187452.png)

然后拷贝到项目的lib文件夹中

![](https://images0.cnblogs.com/blog/289233/201312/29114416-19868f61144242039479503d7099671f.png)

## 三、添加Struts2 Filter

找到项目的“web.xml”文件，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29114621-6898c49c9ef54ff192c7e99e7e498a55.png)

然后在web.xml文件中加入Struts2 Filter的配置信息：

```
1 <filter>
2   <filter-name>struts2</filter-name>
3   <filter-class>org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter</filter-class>
4 </filter>
5 
6 <filter-mapping>
7   <filter-name>struts2</filter-name>
8    <url-pattern>/*</url-pattern>
9 </filter-mapping>
```

如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29114932-ba4a8af3ac7a47bfad06cdf1474d01a6.png)

## 四、添加struts.xml文件

在项目的src文件夹下添加一个“struts.xml”文件，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29115123-df144e059f004ae98eb917e856dc3574.png)

![](https://images0.cnblogs.com/blog/289233/201312/29115211-114d0be8f504497985e02ed4676a1ed7.png)

添加好之后，在“struts.xml”文件里面添加如下的内容：

```
1 <!DOCTYPE struts PUBLIC
 2     "-//Apache Software Foundation//DTD Struts Configuration 2.0//EN"
 3     "http://struts.apache.org/dtds/struts-2.0.dtd">
 4 <struts>
 5     <constant name="struts.devMode" value="true" />
 6     <package name="basicstruts2" extends="struts-default">
 7         <action name="index">
 8             <result>/index.jsp</result>
 9         </action>
10     </package>
11 </struts>
```

最终Strust.xml文件中的内容如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29115359-aa4d5dc5032d4748b48967798c5cd10b.png)

到此，Strust2的开发环境就全部搭建完成了。

## 五、测试Strust2的开发环境是否搭建成功

在index.jsp中加入一句话

![](https://images0.cnblogs.com/blog/289233/201312/29115535-d92ddb30398f451bb529f7ffa8abd098.png)

然后部署项目，如果项目能够正常运行起来，那么在浏览器中就可以看到index.jsp文件中的内容

在MyEclipse中部署项目：

选中项目，然后右键---->【Run As】---->【MyEclipse Server Application】，此时就可以将项目发布到MyEclipse自带的服务器中，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29115636-835c3fb66c874101a9370038fc806f48.png)

项目部署成功之后，MyElcipse自带的Tomcat服务器就会自动启动，在Tomcat服务器中可以看到我们部署成功的项目：

![](https://images0.cnblogs.com/blog/289233/201312/29115735-80f37b62edda495f823dd5650ef52bea.png)

打开浏览器，在浏览器中输入“**http://localhost:8080/Basic_Struts2/index.action**”，此时看到的效果如下图所示：![](https://images0.cnblogs.com/blog/289233/201312/29115831-ef6b2bdb2bef443b88794e3fc02c8533.png)

Index.jsp中的内容已经可以看到了，这证明strust2的开发环境已经搭建成功！















