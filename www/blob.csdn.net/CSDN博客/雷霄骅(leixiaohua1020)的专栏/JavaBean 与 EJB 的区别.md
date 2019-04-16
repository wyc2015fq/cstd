# JavaBean 与 EJB 的区别 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月16日 23:13:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：27









JavaBean在一般情况下指的是实体类，在大部分情况下和POJO是同义词，基本构成就是一些字段和与之对应的
setter、getter方法，如果一个JavaBean需要在不同的JVM的进程中进行传递，还需要实现Serializable接口；
EJB = Enterprise Java Bean，它和JavaBean有本质的区别，最好不要将他们混淆起来，就像不要将Java和
Javascript混淆起来一样。EJB有3中类型：Session, Entity和Message-driven。EJB2.x使用起来很复杂,
这些缺点在EJB3.0已经不存在了。[http://blog.csdn.net/pathuang68/archive/2009/04/19/4091645.aspx](http://blog.csdn.net/pathuang68/archive/2009/04/19/4091645.aspx)这里有怎样开发EJB的详细教程，说到JavaBean和EJB的区别，我们可以这么说，他们几乎没有什么是相同的，如果非要说有什么区别的话，那就是：
1. JavaBean的使用可以不需要容器，EJB的运行一般需要EJB容器(即应用服务器，如JBoss/Weblogic/Websphere...等等)
2. EJB可以使用JavaBean，尤其是Entity EJB的时候，但几乎没有看到JavaBean可以使用EJB的。

说说 WebService在 开发中的运用。
1. WebService由于采用http协议，而且使用和web服务相同的端口(如80)，因此它可以不受防火墙的限制
2. WebService由于采用了XML做传输载体，因此它对所有的编程语言来说都是中性的，也就是说，不同的编程语言可以通过WebService进行通讯
3. 也正因为WebService采用XML做传输载体，由于XML中存在很多标记(就像HTML中的 <html>之类的东西)，因此通信效率相对比较低。
4. 以前Webservice的通信，在网络上传输的时候不是很安全，现在这些都已经解决，如MS的WSE，当然也可以自己写代码来保证安全。
5. Webservice出现之初，由于采用XML进行传输，因此传输二进制文件如图片就存在问题，解决办法是首先将图片文件进行诸如Base64之类的编码，传输到接收端后，再有接收端进行反编码，从而得到二进制文件。

DCOM 与COM的区别
1. COM不支持分布式通讯，而DCOM(Distributed COM)支持
2. COM的运行不需要容器，而DCOM需要，如MTS
3. COM可以通过工具转换成DCOM
4. COM和DCOM有点过时，但目前仍有很多应用在使用他们

JavaEE方面我都会做开发。但不太理解如何利用。
JavaBean（SSH） VS EJB
这个问题我觉得改成SSH vs. EJB可能更合适一点。SSH = Spring + Struts + Hibernate，他们组合起来可以实现和EJB类似的功能。但一般情况下SSH应用与小型项目，EJB通常用于较正式的、大型的项目。比如想象中国移动这样的公司可能会用Weblogic或者Webshpere，即使用EJB，而不会采用SSH，其中一个很重要的原因是SSH都是开源框架，没有专门的技术服务支持，当然还有一些其他原因。
EJB Vs WebService。
1. 他们的通信方式不同。EJB采用的是IIOP的机制，Webservice用的就是http
2. EJB仅限于Java应用之间的通信，Webservice的通信可以跨语言
3. EJB通信的效率要比Webservice要高
4. EJB也可以部署成Webservice

JavaBean Vs COM
两者之间没有太大的可比性。COM的原理是非常复杂的(如果感兴趣，可以去研究一下MSDN相关技术文档)，JavaBean如前面所说是非常简单的。

EJB Vs DCOM
这两个东西的确比较类似，它们运行都需要容器EJB需要诸如Weblogic，Webshpere以及JBoss这样的EJB容器，DCOM的容器则是MTS，他们都可以进行分布式计算。不过DCOM目前已经逐渐被COM+代替，不过而开发和部署人员来说DCOM和COM+的过渡是相当平滑的。



DCOM/COM+是Microsoft的技术，EJB是SUN的技术，EJB被支持的基础更广泛一些，著名的厂家如IBM，Oracle等等如支持EJB，这主要是因为EJB得益于Java是开源的缘故。



原文地址：[http://aguang520.iteye.com/blog/904161](http://aguang520.iteye.com/blog/904161)




