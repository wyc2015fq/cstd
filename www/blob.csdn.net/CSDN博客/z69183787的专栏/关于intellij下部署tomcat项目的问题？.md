# 关于intellij下部署tomcat项目的问题？ - z69183787的专栏 - CSDN博客
2017年09月25日 14:55:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：613
[https://www.zhihu.com/question/54757013](https://www.zhihu.com/question/54757013)
作者：木女孩
链接：https://www.zhihu.com/question/54757013/answer/140992590
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
首先说明啊，下面说的东东有一些呢有多种方法，我不是写说明书（说的好像会写一样），嗯，你懂的，不懂也算了，这个不重要。。。
我的Tomcat装在这个鸟地方的：F:\resource\apache-tomcat-8.0.37
我们先搞一个web应用吧，一个pikaqiu，里面有一个aaa.jsp，啊，就是下面这样
&amp;lt;img src="https://pic1.zhimg.com/50/v2-7ba599c3533c12e6c6ff7be1420c274c_hd.png" data-rawwidth="554" data-rawheight="310" class="origin_image zh-lightbox-thumb" width="554" data-original="https://pic1.zhimg.com/v2-7ba599c3533c12e6c6ff7be1420c274c_r.png"&amp;gt;![](https://pic1.zhimg.com/50/v2-7ba599c3533c12e6c6ff7be1420c274c_hd.png)
可以看到这个pikaqiu在桌面，也就是C:\Users\xinxi\Desktop\
- 先来说一下虚拟目录
我们怎么才能访问这个aaa.jsp呢，直接把这个pikaqiu复制到tomcat安装目录下的webapps下就行了吧
&amp;lt;img src="https://pic2.zhimg.com/50/v2-7eada7eeccbe86462d8bf9c61bb79ce1_hd.png" data-rawwidth="327" data-rawheight="260" class="content_image" width="327"&amp;gt;然后把这个tomcat跑起来，访问![](https://pic2.zhimg.com/50/v2-7eada7eeccbe86462d8bf9c61bb79ce1_hd.png)然后把这个tomcat跑起来，访问[http://localhost/pikaqiu/aaa.jsp](https://link.zhihu.com/?target=http%3A//localhost/pikaqiu/aaa.jsp)，后面我就不说了，浏览器地址栏里有
&amp;lt;img src="https://pic2.zhimg.com/50/v2-92bb21dd390568a3657a0c5531babb45_hd.png" data-rawwidth="387" data-rawheight="118" class="content_image" width="387"&amp;gt;现在，我不想把这个pikaqiu复制到webapps目录下，让它留在桌面，这要怎么搞呢？![](https://pic2.zhimg.com/50/v2-92bb21dd390568a3657a0c5531babb45_hd.png)现在，我不想把这个pikaqiu复制到webapps目录下，让它留在桌面，这要怎么搞呢？
先把webapps下的删了啊，还有tomcat安装目录下的work目录也删了。去Tomcat安装目录\conf\[enginename]\[hostname]目录（我这里enginename是Catalina，hostname是localhost）下建立一个xml文件，注意文件名为映射的虚拟路径，就是你浏览器地址栏里面要写的
&amp;lt;img src="https://pic2.zhimg.com/50/v2-9449528346337613256ed349f033bcbd_hd.png" data-rawwidth="523" data-rawheight="151" class="origin_image zh-lightbox-thumb" width="523" data-original="https://pic2.zhimg.com/v2-9449528346337613256ed349f033bcbd_r.png"&amp;gt;再把tomcat跑起来![](https://pic2.zhimg.com/50/v2-9449528346337613256ed349f033bcbd_hd.png)再把tomcat跑起来
&amp;lt;img src="https://pic2.zhimg.com/50/v2-44531fe38e8b181d881bcf1f44d00b01_hd.png" data-rawwidth="352" data-rawheight="99" class="content_image" width="352"&amp;gt;再说一遍啊这玩意儿少说也有4~5种方法，嗯，一定是这样的。![](https://pic2.zhimg.com/50/v2-44531fe38e8b181d881bcf1f44d00b01_hd.png)再说一遍啊这玩意儿少说也有4~5种方法，嗯，一定是这样的。
我突然发现好像还得再说一种方法才行，我特么都写到下面eclipse了才发现，真是哔了狗了
&amp;lt;img src="https://pic4.zhimg.com/50/c94d321582e0d36998516261aa0f5193_hd.jpg" data-rawwidth="145" data-rawheight="82" class="content_image" width="145"&amp;gt;![](https://pic4.zhimg.com/50/c94d321582e0d36998516261aa0f5193_hd.jpg)
这种方法就是修改Tomcat安装目录\conf\server.xml，在元素中增加一个子元素<Context path="/abc" docBase="C:/Users/xinxi/Desktop/pikaqiu"/>，然后再访问[http://localhost/abc/aaa.jsp](https://link.zhihu.com/?target=http%3A//localhost/abc/aaa.jsp)就行了，浏览器访问效果和上面一样。
&amp;lt;img src="https://pic1.zhimg.com/50/v2-2eae413bbe5b6deaf3493b8e1ba0a76c_hd.png" data-rawwidth="800" data-rawheight="246" class="origin_image zh-lightbox-thumb" width="800" data-original="https://pic1.zhimg.com/v2-2eae413bbe5b6deaf3493b8e1ba0a76c_r.png"&amp;gt;![](https://pic1.zhimg.com/50/v2-2eae413bbe5b6deaf3493b8e1ba0a76c_hd.png)
- 现在来搞默认的目录
上面的访问都是[http://localhost/[虚拟目录]/aaa.jsp](https://link.zhihu.com/?target=http%3A//localhost/abc/aaa.jsp)，现在要直接[http://localhost/aaa.jsp](https://link.zhihu.com/?target=http%3A//localhost/abc/aaa.jsp)
webapps下有一个ROOT文件夹，他是干嘛的？他也是一个web应用啊，我们装好tomcat之后
&amp;lt;img src="https://pic2.zhimg.com/50/v2-d0c01f9d2e8b38ccc66e98b265923e19_hd.png" data-rawwidth="526" data-rawheight="186" class="origin_image zh-lightbox-thumb" width="526" data-original="https://pic2.zhimg.com/v2-d0c01f9d2e8b38ccc66e98b265923e19_r.png"&amp;gt;这个东西就是那个ROOT了。![](https://pic2.zhimg.com/50/v2-d0c01f9d2e8b38ccc66e98b265923e19_hd.png)这个东西就是那个ROOT了。
现在把pikaqiu复制到webapps下，然后重命名为ROOT，当然你要把它原来的ROOT删了，或者改成其他名字。再把tomcat跑起来
&amp;lt;img src="https://pic4.zhimg.com/50/v2-eaf5fa9247988644692519d555b8137f_hd.png" data-rawwidth="321" data-rawheight="103" class="content_image" width="321"&amp;gt;还和上面一样，我还想把pikaqiu留在桌面怎么办呢（还是和上面一样先把webapps下的与皮卡丘有关的都删了啊，再把work也删了），上面不是建了一个abc.xml，把它重命名为ROOT.xml就ok了，把Tomcat跑起来![](https://pic4.zhimg.com/50/v2-eaf5fa9247988644692519d555b8137f_hd.png)还和上面一样，我还想把pikaqiu留在桌面怎么办呢（还是和上面一样先把webapps下的与皮卡丘有关的都删了啊，再把work也删了），上面不是建了一个abc.xml，把它重命名为ROOT.xml就ok了，把Tomcat跑起来
&amp;lt;img src="https://pic1.zhimg.com/50/v2-081395c1a334d254dc0a0e4480a23f48_hd.png" data-rawwidth="341" data-rawheight="104" class="content_image" width="341"&amp;gt;![](https://pic1.zhimg.com/50/v2-081395c1a334d254dc0a0e4480a23f48_hd.png)
再说一下另外一种方法
&amp;lt;img src="https://pic4.zhimg.com/50/c94d321582e0d36998516261aa0f5193_hd.jpg" data-rawwidth="145" data-rawheight="82" class="content_image" width="145"&amp;gt;![](https://pic4.zhimg.com/50/c94d321582e0d36998516261aa0f5193_hd.jpg)
上面配置虚拟目录时，最后说的方法是在server.xml中增加一行，现在呢你把path="/abc"改成path=""就行了。然后访问[http://localhost/aaa.jsp](https://link.zhihu.com/?target=http%3A//localhost/aaa.jsp)就可以了。
&amp;lt;img src="https://pic3.zhimg.com/50/v2-326b523f27b4254d238ec1ae1d68baca_hd.png" data-rawwidth="768" data-rawheight="218" class="origin_image zh-lightbox-thumb" width="768" data-original="https://pic3.zhimg.com/v2-326b523f27b4254d238ec1ae1d68baca_r.png"&amp;gt;![](https://pic3.zhimg.com/50/v2-326b523f27b4254d238ec1ae1d68baca_hd.png)
- Tomcat多实例
一般是不是就跑了一个tomcat啊，可以多跑几个吧，你在C盘下装了一个tomcat，在D盘下也装了一个tomcat，把这两个都跑起来，是不是就是两个tomcat了（当然端口不能冲突啊）
除了装两个tomcat，还有没有办法跑两个呢，是有的，就是多实例啦。我现在的tomcat装在F:\resource\apache-tomcat-8.0.37的，上面截图可以看到我没有指定端口，嗯，我想说的就是这个tomcat监听的端口是80。
现在我在桌面建一个文件夹，再把F:\resource\apache-tomcat-8.0.37\conf复制到这个文件夹中，然后把端口改一下（我改成了9090）
&amp;lt;img src="https://pic4.zhimg.com/50/v2-c350a7827113c6394001e41aa8014dd3_hd.png" data-rawwidth="486" data-rawheight="187" class="origin_image zh-lightbox-thumb" width="486" data-original="https://pic4.zhimg.com/v2-c350a7827113c6394001e41aa8014dd3_r.png"&amp;gt;![](https://pic4.zhimg.com/50/v2-c350a7827113c6394001e41aa8014dd3_hd.png)
再在这个文件夹中新建一个bat文件，内容大致如下：
```bash
set "CATALINA_BASE=%cd%"
set "CATALINA_HOME=F:\resource\apache-tomcat-8.0.37"
set "EXECUTABLE=%CATALINA_HOME%\bin\catalina.bat"
call "%EXECUTABLE%" start
```
嗯，就是下面这个样子了
&amp;lt;img src="https://pic2.zhimg.com/50/v2-fb5567e6fa3b940ef7fda949b236f981_hd.png" data-rawwidth="734" data-rawheight="231" class="origin_image zh-lightbox-thumb" width="734" data-original="https://pic2.zhimg.com/v2-fb5567e6fa3b940ef7fda949b236f981_r.png"&amp;gt;简单解释一下，CATALINA_HOME就是你的Tomcat安装的位置，CATALINA_BASE就是你的这个实例的位置，默认的话这两个值是一样的。%cd%是什么东东呢![](https://pic2.zhimg.com/50/v2-fb5567e6fa3b940ef7fda949b236f981_hd.png)简单解释一下，CATALINA_HOME就是你的Tomcat安装的位置，CATALINA_BASE就是你的这个实例的位置，默认的话这两个值是一样的。%cd%是什么东东呢
&amp;lt;img src="https://pic1.zhimg.com/50/v2-0b7fbabcc1a1692b8c4e6ae23a4ad678_hd.png" data-rawwidth="401" data-rawheight="167" class="content_image" width="401"&amp;gt;现在双击这个startup.bat![](https://pic1.zhimg.com/50/v2-0b7fbabcc1a1692b8c4e6ae23a4ad678_hd.png)现在双击这个startup.bat
&amp;lt;img src="https://pic4.zhimg.com/50/v2-6122c40d1ab18d760003bf1c8026ecff_hd.png" data-rawwidth="854" data-rawheight="456" class="origin_image zh-lightbox-thumb" width="854" data-original="https://pic4.zhimg.com/v2-6122c40d1ab18d760003bf1c8026ecff_r.png"&amp;gt;看到已经启动成功了，t1目录下也多了几个文件夹，现在t1就是一个tomcat实例了（现在访问![](https://pic4.zhimg.com/50/v2-6122c40d1ab18d760003bf1c8026ecff_hd.png)看到已经启动成功了，t1目录下也多了几个文件夹，现在t1就是一个tomcat实例了（现在访问[http://localhost:9090](https://link.zhihu.com/?target=http%3A//localhost%3A9090)是没有东西的，因为t1/webapps下什么都没有），你把web应用放到t1/webapps下一样的
&amp;lt;img src="https://pic2.zhimg.com/50/v2-8adc71c1be7a8b7f0868f33c294b2455_hd.png" data-rawwidth="639" data-rawheight="161" class="origin_image zh-lightbox-thumb" width="639" data-original="https://pic2.zhimg.com/v2-8adc71c1be7a8b7f0868f33c294b2455_r.png"&amp;gt;再比如最上面我们在Tomcat安装目录\conf\[enginename]\[hostname]目录下弄了一个abc.xml，现在是不是应该到t1\conf\[enginename]\[hostname]目录下弄啦，嗯，一定是这样的。![](https://pic2.zhimg.com/50/v2-8adc71c1be7a8b7f0868f33c294b2455_hd.png)再比如最上面我们在Tomcat安装目录\conf\[enginename]\[hostname]目录下弄了一个abc.xml，现在是不是应该到t1\conf\[enginename]\[hostname]目录下弄啦，嗯，一定是这样的。
&amp;lt;img src="https://pic2.zhimg.com/50/v2-f230bc7f509542e66e811e02fcfba35d_hd.png" data-rawwidth="696" data-rawheight="310" class="origin_image zh-lightbox-thumb" width="696" data-original="https://pic2.zhimg.com/v2-f230bc7f509542e66e811e02fcfba35d_r.png"&amp;gt;![](https://pic2.zhimg.com/50/v2-f230bc7f509542e66e811e02fcfba35d_hd.png)
到现在为止，老哥，稳。
- MyEclipse中tomcat
他直接把你的应用复制到Tomcat安装目录下的webapps中，然后再把tomcat跑起来，没毛病。
&amp;lt;img src="https://pic4.zhimg.com/50/v2-6978a6cdb0ff5bb5f93649ca407f6e5f_hd.png" data-rawwidth="884" data-rawheight="469" class="origin_image zh-lightbox-thumb" width="884" data-original="https://pic4.zhimg.com/v2-6978a6cdb0ff5bb5f93649ca407f6e5f_r.png"&amp;gt;默认这个Web
 Content-root和你的项目名是一样的，可以自己改，然后，唉，没有然后了，这图该有的都有了。![](https://pic4.zhimg.com/50/v2-6978a6cdb0ff5bb5f93649ca407f6e5f_hd.png)
默认这个Web Content-root和你的项目名是一样的，可以自己改，然后，唉，没有然后了，这图该有的都有了。
- eclipse中tomcat
这个和MyEclipse不一样啊，他不是复制到webapps下面的，题主说不知道它部署去了哪里，其实这个很好搞到啊。在eclipse中新建一个web应用，然后弄一个jsp文件，内容见图，跑起来就知道它部署到哪里去了
&amp;lt;img src="https://pic3.zhimg.com/50/v2-07014513a1916f16c2247c85b0b01252_hd.png" data-rawwidth="1053" data-rawheight="438" class="origin_image zh-lightbox-thumb" width="1053" data-original="https://pic3.zhimg.com/v2-07014513a1916f16c2247c85b0b01252_r.png"&amp;gt;啊，没错，它就在这个目录里面![](https://pic3.zhimg.com/50/v2-07014513a1916f16c2247c85b0b01252_hd.png)啊，没错，它就在这个目录里面
&amp;lt;img src="https://pic3.zhimg.com/50/v2-78424790bbae2177a728abe8eacbaf06_hd.png" data-rawwidth="710" data-rawheight="219" class="origin_image zh-lightbox-thumb" width="710" data-original="https://pic3.zhimg.com/v2-78424790bbae2177a728abe8eacbaf06_r.png"&amp;gt;然后再向上一点![](https://pic3.zhimg.com/50/v2-78424790bbae2177a728abe8eacbaf06_hd.png)然后再向上一点
&amp;lt;img src="https://pic1.zhimg.com/50/v2-b1d2f8117a526c1e6144a083702dfd64_hd.png" data-rawwidth="566" data-rawheight="260" class="origin_image zh-lightbox-thumb" width="566" data-original="https://pic1.zhimg.com/v2-b1d2f8117a526c1e6144a083702dfd64_r.png"&amp;gt;是不是熟悉的面孔？我们把皮卡丘复制到这个webapps下面你说能访问不，很明显能啊对不对。这就是一个tomcat的实例嘛。![](https://pic1.zhimg.com/50/v2-b1d2f8117a526c1e6144a083702dfd64_hd.png)是不是熟悉的面孔？我们把皮卡丘复制到这个webapps下面你说能访问不，很明显能啊对不对。这就是一个tomcat的实例嘛。
现在有一个问题，我们到在wtpwebapps下瞧瞧
&amp;lt;img src="https://pic1.zhimg.com/50/v2-f6a4f9b3188bd50d9ce51c0e1aa804b0_hd.png" data-rawwidth="585" data-rawheight="183" class="origin_image zh-lightbox-thumb" width="585" data-original="https://pic1.zhimg.com/v2-f6a4f9b3188bd50d9ce51c0e1aa804b0_r.png"&amp;gt;可以看到这里面有两个，一个是我们的foo，还有一个ROOT，不过这个ROOT里面基本上什么都没有。如果我们的foo在webapps下，那没什么说的对吧，问题就在于它在wtpwebapps下啊，我们是如何访问到它的呢？去看一下这个tomcat实例的server.xml就知道了![](https://pic1.zhimg.com/50/v2-f6a4f9b3188bd50d9ce51c0e1aa804b0_hd.png)可以看到这里面有两个，一个是我们的foo，还有一个ROOT，不过这个ROOT里面基本上什么都没有。如果我们的foo在webapps下，那没什么说的对吧，问题就在于它在wtpwebapps下啊，我们是如何访问到它的呢？去看一下这个tomcat实例的server.xml就知道了
&amp;lt;img src="https://pic4.zhimg.com/50/v2-e90f0060176d9b92ffd9ce1e3c0d3fcb_hd.png" data-rawwidth="1231" data-rawheight="522" class="origin_image zh-lightbox-thumb" width="1231" data-original="https://pic4.zhimg.com/v2-e90f0060176d9b92ffd9ce1e3c0d3fcb_r.png"&amp;gt;这些东西都是eclipse的插件自己给我们搞的，这个问题完啦。![](https://pic4.zhimg.com/50/v2-e90f0060176d9b92ffd9ce1e3c0d3fcb_hd.png)
这些东西都是eclipse的插件自己给我们搞的，这个问题完啦。
还有一个问题，eclipse是如何启动这个tomcat实例的，上面我们自己手动弄tomcat实例的时候弄了一个startup.bat文件，里面最主要就是配置了CATALINA_BASE对吧，那eclipse又是怎么搞的呢？这个问题先留着后面一起说。
- Intellij IDEA中的Tomcat
和上面一样的办法可以搞到它部署到哪里去了
&amp;lt;img src="https://pic2.zhimg.com/50/v2-9eef0f14feda9dfdc87da4a5e79c9ac9_hd.png" data-rawwidth="587" data-rawheight="102" class="origin_image zh-lightbox-thumb" width="587" data-original="https://pic2.zhimg.com/v2-9eef0f14feda9dfdc87da4a5e79c9ac9_r.png"&amp;gt;![](https://pic2.zhimg.com/50/v2-9eef0f14feda9dfdc87da4a5e79c9ac9_hd.png)&amp;lt;img
 src="https://pic3.zhimg.com/50/v2-038ce86a92e8354b6ea34146c929bc12_hd.png" data-rawwidth="483" data-rawheight="180" class="origin_image zh-lightbox-thumb" width="483" data-original="https://pic3.zhimg.com/v2-038ce86a92e8354b6ea34146c929bc12_r.png"&amp;gt;然后和eclipse一样，也搞了一个tomcat的实例，在下面这个地方![](https://pic3.zhimg.com/50/v2-038ce86a92e8354b6ea34146c929bc12_hd.png)然后和eclipse一样，也搞了一个tomcat的实例，在下面这个地方
&amp;lt;img src="https://pic2.zhimg.com/50/v2-f5498ca71023ad669d6d332a58a128e5_hd.png" data-rawwidth="588" data-rawheight="214" class="origin_image zh-lightbox-thumb" width="588" data-original="https://pic2.zhimg.com/v2-f5498ca71023ad669d6d332a58a128e5_r.png"&amp;gt;我这里最后一个目录叫Unnamed_JavaWeb，这个名字目测是这样的（具体我不知道），下划线后面的是项目的名字，啊，我建的这个项目就叫JavaWeb![](https://pic2.zhimg.com/50/v2-f5498ca71023ad669d6d332a58a128e5_hd.png)我这里最后一个目录叫Unnamed_JavaWeb，这个名字目测是这样的（具体我不知道），下划线后面的是项目的名字，啊，我建的这个项目就叫JavaWeb&amp;lt;img
 src="https://pic3.zhimg.com/50/v2-f66ab24b17fde0b6cd81a7956128def2_hd.png" data-rawwidth="137" data-rawheight="38" class="content_image" width="137"&amp;gt;然后下划线前面的应该是配置Tomcat时给起的名字，默认是Unnamed![](https://pic3.zhimg.com/50/v2-f66ab24b17fde0b6cd81a7956128def2_hd.png)然后下划线前面的应该是配置Tomcat时给起的名字，默认是Unnamed&amp;lt;img
 src="https://pic3.zhimg.com/50/v2-4284b167c6605da69449448785c403aa_hd.png" data-rawwidth="538" data-rawheight="171" class="origin_image zh-lightbox-thumb" width="538" data-original="https://pic3.zhimg.com/v2-4284b167c6605da69449448785c403aa_r.png"&amp;gt;然而我已经改成Tomcat8了啊，唉，不理了，反正不重要。![](https://pic3.zhimg.com/50/v2-4284b167c6605da69449448785c403aa_hd.png)然而我已经改成Tomcat8了啊，唉，不理了，反正不重要。
上面eclipse是怎么搞的，在server.xml下增加Context元素对吧，IDEA不是这样的，它是在\conf\[enginename]\[hostname]目录下增加xml文件。看上面的截图，我直接访问[http://localhost](https://link.zhihu.com/?target=http%3A//localhost)就可以了，这说明，他把我的这个名叫JavaWeb的应用弄成了默认的，按照上面说的，应该在\conf\[enginename]\[hostname]目录下增加一个ROOT.xml吧。啊，它就是这么搞的
&amp;lt;img src="https://pic1.zhimg.com/50/v2-99f5fca547f7a0a6c60ece5ec3d2aff8_hd.png" data-rawwidth="748" data-rawheight="226" class="origin_image zh-lightbox-thumb" width="748" data-original="https://pic1.zhimg.com/v2-99f5fca547f7a0a6c60ece5ec3d2aff8_r.png"&amp;gt;![](https://pic1.zhimg.com/50/v2-99f5fca547f7a0a6c60ece5ec3d2aff8_hd.png)
- IDE是如何启动tomcat实例的
在说eclipse的时候留的这个问题，Intellij IDEA也是一样的，它们是如何启动tomcat实例的。
```bash
set "CATALINA_BASE=%cd%"
set "CATALINA_HOME=F:\resource\apache-tomcat-8.0.37"
set "EXECUTABLE=%CATALINA_HOME%\bin\catalina.bat"
call "%EXECUTABLE%" start
```
这是在干嘛啊，设置了CATALINA_HOME和CATALINA_BASE，然后用catalina.bat去吧tomcat跑起来。
&amp;lt;img src="https://pic2.zhimg.com/50/v2-54de13f5a19aacc4056e2147beb299d1_hd.png" data-rawwidth="591" data-rawheight="199" class="origin_image zh-lightbox-thumb" width="591" data-original="https://pic2.zhimg.com/v2-54de13f5a19aacc4056e2147beb299d1_r.png"&amp;gt;可以看到这个CATALINA_BASE没有的话，它默认的值就是CATALINA_HOME对吧。![](https://pic2.zhimg.com/50/v2-54de13f5a19aacc4056e2147beb299d1_hd.png)可以看到这个CATALINA_BASE没有的话，它默认的值就是CATALINA_HOME对吧。
好，双击我们自己写的startup.bat把我们自己的tomcat实例跑起来，使用jps工具看一看启动参数，tomcat本身是一个Java程序，运行在Java虚拟机上的，所以你懂的。
&amp;lt;img src="https://pic2.zhimg.com/50/v2-c6477218a19b203c52aaba30a153be45_hd.png" data-rawwidth="1249" data-rawheight="510" class="origin_image zh-lightbox-thumb" width="1249" data-original="https://pic2.zhimg.com/v2-c6477218a19b203c52aaba30a153be45_r.png"&amp;gt;![](https://pic2.zhimg.com/50/v2-c6477218a19b203c52aaba30a153be45_hd.png)
在eclipse中启动tomcat
&amp;lt;img src="https://pic4.zhimg.com/50/v2-986d0648cfdea4ae620af58d5056215f_hd.png" data-rawwidth="851" data-rawheight="366" class="origin_image zh-lightbox-thumb" width="851" data-original="https://pic4.zhimg.com/v2-986d0648cfdea4ae620af58d5056215f_r.png"&amp;gt;用jps看一看![](https://pic4.zhimg.com/50/v2-986d0648cfdea4ae620af58d5056215f_hd.png)用jps看一看
&amp;lt;img src="https://pic1.zhimg.com/50/v2-e93827549ca8c942eed5612c381415c8_hd.png" data-rawwidth="1247" data-rawheight="76" class="origin_image zh-lightbox-thumb" width="1247" data-original="https://pic1.zhimg.com/v2-e93827549ca8c942eed5612c381415c8_r.png"&amp;gt;在Intellij
 IDEA中启动tomcat![](https://pic1.zhimg.com/50/v2-e93827549ca8c942eed5612c381415c8_hd.png)在Intellij IDEA中启动tomcat
&amp;lt;img src="https://pic4.zhimg.com/50/v2-bac1a5f56f895da459469636208cdc73_hd.png" data-rawwidth="1041" data-rawheight="342" class="origin_image zh-lightbox-thumb" width="1041" data-original="https://pic4.zhimg.com/v2-bac1a5f56f895da459469636208cdc73_r.png"&amp;gt;最开始也有输出啊![](https://pic4.zhimg.com/50/v2-bac1a5f56f895da459469636208cdc73_hd.png)最开始也有输出啊
&amp;lt;img src="https://pic1.zhimg.com/50/v2-4ed91afd6d239cbf9f201b1c0bf1ade4_hd.png" data-rawwidth="1007" data-rawheight="179" class="origin_image zh-lightbox-thumb" width="1007" data-original="https://pic1.zhimg.com/v2-4ed91afd6d239cbf9f201b1c0bf1ade4_r.png"&amp;gt;这个东西是catalina.bat里面输出的，后面红色的是Java日志输出的，然而eclipse那个控制台就显示了Java输出的东东。![](https://pic1.zhimg.com/50/v2-4ed91afd6d239cbf9f201b1c0bf1ade4_hd.png)这个东西是catalina.bat里面输出的，后面红色的是Java日志输出的，然而eclipse那个控制台就显示了Java输出的东东。
&amp;lt;img src="https://pic4.zhimg.com/50/v2-290da888297e00d01645694c906f3583_hd.png" data-rawwidth="651" data-rawheight="371" class="origin_image zh-lightbox-thumb" width="651" data-original="https://pic4.zhimg.com/v2-290da888297e00d01645694c906f3583_r.png"&amp;gt;再用jps看一眼吧![](https://pic4.zhimg.com/50/v2-290da888297e00d01645694c906f3583_hd.png)再用jps看一眼吧
&amp;lt;img src="https://pic1.zhimg.com/50/v2-be21f8e6cc8711313c1ac978fd2da734_hd.png" data-rawwidth="1211" data-rawheight="158" class="origin_image zh-lightbox-thumb" width="1211" data-original="https://pic1.zhimg.com/v2-be21f8e6cc8711313c1ac978fd2da734_r.png"&amp;gt;![](https://pic1.zhimg.com/50/v2-be21f8e6cc8711313c1ac978fd2da734_hd.png)
啊，写完了
&amp;lt;img src="https://pic2.zhimg.com/50/bd189d21d8e56bc35d5f8342b5673435_hd.jpg" data-rawwidth="121" data-rawheight="138" class="content_image" width="121"&amp;gt;![](https://pic2.zhimg.com/50/bd189d21d8e56bc35d5f8342b5673435_hd.jpg)
