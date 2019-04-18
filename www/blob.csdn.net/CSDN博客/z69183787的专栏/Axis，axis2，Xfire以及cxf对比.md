# Axis，axis2，Xfire以及cxf对比 - z69183787的专栏 - CSDN博客
2014年02月21日 14:11:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：837
[http://ws.apache.org/axis/](http://ws.apache.org/axis/)
[http://axis.apache.org/axis2/java/core/](http://axis.apache.org/axis2/java/core/)
[http://xfire.codehaus.org/](http://xfire.codehaus.org/)
[http://cxf.apache.org/](http://cxf.apache.org/)
axis最新的版本为April
 22, 2006
axis2最新的版本为19
 - Dec - 2010    1.5.4
xfire最新的版本为1.2.6
 - May 3, 2007         网站已经说了“XFire
 is now CXF”  --|||
cxf 最新的版本为Feb
 28, 2011 - Apache CXF 2.3.3 is released!
现在只有axis2和cxf官方有更新。
（以下来自网络资料）
XFire
 VS Axis
XFire是与Axis2 并列的新一代WebService平台。之所以并称为新一代，因为它：
1、支持一系列Web Service的新标准--JSR181、WSDL2.0 、JAXB2、WS-Security等；
2、使用Stax解释XML,性能有了质的提高。XFire采用Woodstox 作Stax实现；
3、容易上手，可以方便快速地从pojo发布服务；
4、Spring的结合；
5、灵活的Binding机制，包括默认的Aegis,xmlbeans,jaxb2,castor。
XFire与Axis1性能的比较
1、XFire比Axis1.3快2-6倍
2、XFire的响应时间是Axis1.3的1/2到1/5
XFire与Axis2的比较
虽然XFire与Axis2都是新一代的WebService平台，但是Axis2的开发者太急于推出1.0版本，所以1.0还不是一个稳定的版本，它的开发者宣称1.1版本即将推出，希望1.1版本会是个稳定的版本。在XFire捐献给apache后有人认为Axis2将会灭亡。其实在很多人眼里，Axis2并不是pojo形式，Dan
 Diephouse证明了XFire比Axis更有市场，我也发现了有很多人开始从Axis转向XFire，包括我也在说服身边的人转向利用XFire进行WebService的开发，很典型的是我可以在几分钟之内教会我的团队实用XFire来发布一个他自己的Web服务。
本人倾向于XFire确实比Axis2简单很多
AXIS VS CXF
在SOA领域，我们认为Web Service是SOA体系的构建单元（building block）。对于服务开发人员来说，AXIS和CXF一定都不会陌生。这两个产品都是Apache孵化器下面的Web Service开源开发工具。
 Axis2的最新版本是1.3.
CXF现在已经到了2.0版本。
这两个框架 都是从已有的开源项目发展起来的。Axis2是从Axis1.x系列发展而来。CXF则是XFire和Celtix项目的结合产品。Axis2是从底层全部重新实现，使用了新的扩展性更好模块架构。 CXF也重新的深化了XFire和Celtix这两个开发工具。
新产品的退出导致了几个问题。是不是现有的使用Axis 1.x，XFire和Celix的应用需要迁移的新的版本上。如果一个开发人员确定要迁移它的应用到新的框架上，那么他应该选择哪一个呢？相反的，如果一个开发者决定从头开发一个新的Web Service，他应该使用哪个呢？ 这两个框架哪一个更好一些呢？
对于系统迁移来说，也许迁移到新的框架并不难。Axis和CXF都提供了迁移的指导。能够给开发者一些迁移的技巧和经验。但是对于这样迁移，这两个开源项目都没有提供迁移的工具。对于这样的迁移工作，尽管很值得去寻找所有的可行方案。Axis2和CXF都有各自不同的WebService开发方法，每个方法都有相当数量拥护者。
通过一个比较矩阵来比较Axis2和CXF变得有现实的意义。这两个项目都开发不够成熟，但是最主要的区别在以下几个方面：
1.CXF支持 WS-Addressing，WS-Policy， WS-RM， WS-Security和WS-I Basic Profile。Axis2不支持WS-Policy，但是承诺在下面的版本支持。
2. CXF可以很好支持Spring。Axis2不能
3. AXIS2支持更广泛的数据并对，如XMLBeans，JiBX，JaxMe和JaxBRI和它自定义的数据绑定ADB。注意JaxME和JaxBRI都还是试验性的。CXF只支持JAXB和Aegis。在CXF2.1
4. Axis2支持多语言-除了Java,他还支持C/C++版本。
　　比较这两个框架的Web Service开发方法与比较它们的特性同样重要。
从开发者的角度，两个框架的特性相当的不同。 Axis2的开发方式类似一个小型的应用服务器，Axis2的开发包要以WAR的形式部署到Servlet容器中，比如Tomcat，通过这些容器可以对工作中的Web Service进行很好的监控和管理。Axis2的Web administrion模块可以让我们动态的配置Axis2.一个新的服务可以上载，激活，使之失效，修改web服务的参数。管理UI也可以管理一个或者多个处于运行状态的服务。这种界面化管理方式的一个弊端是所有在运行时修改的参数没有办法保存，因为在重启动之后，你所做的修改就会全部失效。Axis2允许自己作为独立的应用来发布Web
 Service，并提供了大量的功能和一个很好的模型，这个模型可以通过它本身的架构（modular architecture）不断添加新的功能。有些开发人员认为这种方式对于他们的需求太过于繁琐。这些开发人员会更喜欢CXF。
　　CXF更注重开发人员的工效（ergonomics）和嵌入能力（embeddability）。大多数配置都可以API来完成，替代了比较繁琐的XML配置文件， Spring的集成性经常的被提及，CXF支持Spring2.0和CXF's API和Spring的配置文件可以非常好的对应。CXF强调代码优先的设计方式（code-first design)，使用了简单的API使得从现有的应用开发服务变得方便。
不过你选择Axis2还是CXF，你都可以从开源社区得到大量的帮助。这两个框架都有商业公司提供服务，WSO2提供AXIS2的支持，Iona提供CXF的支持。这两公司都有很活跃的开发者社区。
　　Axis2出现的时间较早，CXF的追赶速度快。
　　我的建议是：如果你需要多语言的支持，你应该选择AXIS2。如果你需要把你的实现侧重JAVA并希望和Spring集成，CXF就是更好的选择，特别是把你的Web Service嵌入其他的程序中。如果你觉得这两个框架的新特性对于你并没有太大的用处，你会觉得Axis1也是不错的选择，你应该继续使用它知道你有充分的理由去更换它。
如何抉择：
1、如果应用程序需要多语言的支持，Axis2 应当是首选了；
2、如果应用程序是遵循 Spring 哲学路线的话，Apache CXF 是一种更好的选择，特别对嵌入式的 Web Services 来说；
3、如果应用程序没有新的特性需要的话，就仍是用原来项目所用的框架，比如 Axis1，XFire，Celtrix或 BEA 等等厂家自己的 Web Services 实现，就别劳民伤财了。
（资源2）
1、JWS是Java语言对WebService服务的一种实现，用来开发和发布服务。而从服务本身的角度来看JWS服务是没有语言界限的。但是Java语言为Java开发者提供便捷发布和调用WebService服务的一种途径。
2、Axis2是Apache下的一个重量级WebService框架，准确说它是一个Web Services / SOAP / WSDL 的引擎，是WebService框架的集大成者，它能不但能制作和发布WebService，而且可以生成Java和其他语言版WebService客户端和服务端代码。这是它的优势所在。但是，这也不可避免的导致了Axis2的复杂性，使用过的开发者都知道，它所依赖的包数量和大小都是很惊人的，打包部署发布都比较麻烦，不能很好的与现有应用整合为一体。但是如果你要开发Java之外别的语言客户端，Axis2提供的丰富工具将是你不二的选择。
3、XFire是一个高性能的WebService框架，在Java6之前，它的知名度甚至超过了Apache的Axis2，XFire的优点是开发方便，与现有的Web整合很好，可以融为一体，并且开发也很方便。但是对Java之外的语言，没有提供相关的代码工具。XFire后来被Apache收购了，原因是它太优秀了，收购后，随着Java6 JWS的兴起，开源的WebService引擎已经不再被看好，渐渐的都败落了。
4、CXF是Apache旗下一个重磅的SOA简易框架，它实现了ESB（企业服务总线）。CXF来自于XFire项目，经过改造后形成的，就像目前的Struts2来自WebWork一样。可以看出XFire的命运会和WebWork的命运一样，最终会淡出人们的视线。CXF不但是一个优秀的Web
 Services / SOAP / WSDL 引擎，也是一个不错的ESB总线，为SOA的实施提供了一种选择方案，当然他不是最好的，它仅仅实现了SOA架构的一部分。
