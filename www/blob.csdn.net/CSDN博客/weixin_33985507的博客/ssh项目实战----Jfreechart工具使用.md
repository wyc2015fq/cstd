# ssh项目实战----Jfreechart工具使用 - weixin_33985507的博客 - CSDN博客
2018年01月29日 20:51:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：16
## 一、图形报表jfreechart
- 
将解压的资源包拷贝出来，放置到无中文的目录中，使用下列指令运行
java -jar jfreechart-1.0.13-demo.jar
注意：不能只拷贝该文件，必须将相关的资源全部拷贝出去
![5824016-0fc8ce3b6dd6d16a](https://upload-images.jianshu.io/upload_images/5824016-0fc8ce3b6dd6d16a)
好好学java
- 
打开演示程序，查看内容
![5824016-4cc4d02525666ba8](https://upload-images.jianshu.io/upload_images/5824016-4cc4d02525666ba8)
好好学java
- 
制作参考饼状图
![5824016-752c740731b61c2a](https://upload-images.jianshu.io/upload_images/5824016-752c740731b61c2a)
好好学java
- 
使用资源/jfreechart/jd-gui.exe工具，对原始jar文件进行反编译，查看源码
![5824016-e940a0c45f9bfdd5](https://upload-images.jianshu.io/upload_images/5824016-e940a0c45f9bfdd5)
好好学java
- 
根据目录结构，查找对应的饼图制作案例Dome源码程序
![5824016-3784a4ba34af8073](https://upload-images.jianshu.io/upload_images/5824016-3784a4ba34af8073)
好好学java
- 
将代码复制到项目中，进行测试。所需jar包均已全部导入
![5824016-6ac3bf851f938955](https://upload-images.jianshu.io/upload_images/5824016-6ac3bf851f938955)
好好学java
- 
根据运行结果，修改个别参数，理解该工具的使用方式
createDataset方法用于控制饼图显示数据。使用时，将第一个数据替换为产品名称，第二个数据替换成数量即为要展示的饼图数据
![5824016-525c00f0a97ef473](https://upload-images.jianshu.io/upload_images/5824016-525c00f0a97ef473)
这里写图片描述
- 
createChart方法用于创建图形，需要依赖于数据才可以创建。将其中没有用的设置剔除掉。
![5824016-246cecd9aad10d78](https://upload-images.jianshu.io/upload_images/5824016-246cecd9aad10d78)
这里写图片描述
根据上述业务方法功能，进行改造，不能在页面中使用AWT的显示格式，需要将显示的内容转化为图片，页面加载图片完成显示。删除其他所有方法设置与继承关系。
- 
创建测试main方法
![5824016-01f56dca900959a2](https://upload-images.jianshu.io/upload_images/5824016-01f56dca900959a2)
好好学java
查看工程目录下生成的图片，选用png格式。实际工程中不可能先生成图片再将图片传送到页面，因此，需要使用IO流的形式完成数据传递。
## 二、页面加入jfreechart饼图
**1.将jfreechart改造成工具类**
![5824016-bed5ed7929398d94](https://upload-images.jianshu.io/upload_images/5824016-bed5ed7929398d94)
这里写图片描述
**2.提供调用方法**
该方法提供一组数据，可以将数据组织成jfreechart饼状图对象，然后出入到传递的输出流对象中
![5824016-57b2ff19e68b206c](https://upload-images.jianshu.io/upload_images/5824016-57b2ff19e68b206c)
这里写图片描述
**3.Action中提供方法，用于获取转化完毕的jfreechart输出流数据**
获得相应输出流，将该流传递给获取jfreechart饼图方法，将饼图数据加入到流中，最终刷新流，将输出传输到页面
![5824016-4c5ba0fabaa6ade6](https://upload-images.jianshu.io/upload_images/5824016-4c5ba0fabaa6ade6)
这里写图片描述
**4.页面通过请求格式，对后台再次发起条用，参数从请求参数中获取**
![5824016-9894a9cc4bb57456](https://upload-images.jianshu.io/upload_images/5824016-9894a9cc4bb57456)
这里写图片描述
**5.中文显示处理**
将其中的内容添加到工具类中，静态设置jfreechart的字符中文过滤
## 三、知识总结
1.Jfreechart工具使用
2.页面图片通过请求方式获取数据
> 
如果想获取更多源码或者视频教程，欢迎关注我的微信公众号 `好好学java`，在公众号里，回复：`java基础、html5、javaEE基础、struts2、spring、redis、luncene、oracle`等，将可获得以上的优质视频教程及源码。
![5824016-90cc91579981967b](https://upload-images.jianshu.io/upload_images/5824016-90cc91579981967b)
这里写图片描述
