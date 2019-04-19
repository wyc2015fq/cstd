# 吐槽一下Activiti的用户手册和一本书 - SHINING的博客 - CSDN博客
2014年10月19日 13:27:12[snsn1984](https://me.csdn.net/snsn1984)阅读数：8830
业余没事的时候，看了点Java的资料，无意之中发现了Activiti，就打算自己跑几个例子看看到底是怎么回事。一直搞底层，也得偶尔关心下上层到底发展到什么程度了不是。
悲惨的过程就是这么开始的，首先是Activiti的用户手册，第一次见到这么混乱的用户手册，完全打破了“Hello World”的过程。一上来你还没看到Activiti到底是什么东西的时候，就各种配置各种代码往上贴，好像生怕吓不走你一样。整个用户手册没有任何条理可研，如果按顺序看，是根本看不下去的，到最后也没琢磨明白这个用户手册是按照什么思路组织的。不得不说，开源软件搞了两年多了，第一次被这样的用户手册给打的败败的，有种故作玄虚的感觉。重点不突出，文档没有层次感。彻底无语了，这也是好多人吐槽Activiti新手不好入门的一个重大原因吧。（谢绝：你行你上啊，这种幼稚思维的喷子。出门左转。）
另外想吐槽的就是一本书，当时搜了下看到书名算是一个大系列里的（具体书名就不提了，省的拉仇恨），这个系列给我印象还是很不错的，特别是对于新接触的人来说，细致入微，手把手的教。可是这本关于Activiti的书我就想说我次奥了，该细致的地方绝对顾左右而言他，该大致介绍下的介绍的很详细，大段大段的用户手册中的关于API的介绍占了书的绝大部分篇幅。另外该书内部大篇幅的代码，并且没有提供随书光盘，也没有提供代码下载地方。第一个例子就出现了：这个部分需要另外寻找。我就想说真能很顺利能找到的人，还需要看你这本书么？！虽然我也提倡大家多动手，但是一个用户手册也好，一本书也好，好歹第一个例子完整一点，让大家先有个大体的认识，然后可以介绍主要的部分，次要的部分让读者自己补充，这样才能让大家逐步深入。像这本书这么搞，骗钱的嫌疑很大。不清楚这个大系列的原作者清楚不清楚，这可是砸招牌的事情。
附一些自己的经验教训：
1. 一定要用Maven，不管用过没用过，在Activiti主要用来做依赖库管理。这块你不用Maven，很麻烦。（那本书里介绍的是，如果你不想读Activiti的源码，这里可以不用。呵呵一下）
2.买书之前一定要找电子版的好好翻翻，不管这系列的书之前多好，也不代表现在就好。
3.简单项目的pom.xml，可以跑简单的例子程序，这里主要是看一下jar包的依赖。
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.shining</groupId>
  <artifactId>testmaven</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <repositories>
  <repository>
    <id>Alfresco Maven Repository</id>
    <url>https://maven.alfresco.com/nexus/content/groups/public/</url>
  </repository>
  </repositories>
  <dependencies>
  <dependency>
  <groupId>org.activiti</groupId>
  <artifactId>activiti-engine</artifactId>
  <version>5.16</version>
  </dependency>
<dependency>
  <groupId>org.springframework</groupId>
  <artifactId>spring-context</artifactId>
  <version>4.1.1.RELEASE</version>
</dependency>
<dependency>
  <groupId>org.springframework</groupId>
  <artifactId>spring-jdbc</artifactId>
  <version>4.1.1.RELEASE</version>
</dependency>
<dependency>
  <groupId>org.springframework</groupId>
  <artifactId>spring-tx</artifactId>
  <version>4.1.1.RELEASE</version>
</dependency>
  <dependency>
    <groupId>org.apache.logging.log4j</groupId>
    <artifactId>log4j-api</artifactId>
    <version>2.0.2</version>
  </dependency>
  <dependency>
    <groupId>org.apache.logging.log4j</groupId>
    <artifactId>log4j-core</artifactId>
    <version>2.0.2</version>
  </dependency>
<dependency>
  <groupId>org.slf4j</groupId>
  <artifactId>slf4j-log4j12</artifactId>
  <version>1.7.7</version>
</dependency>
<dependency>
  <groupId>com.h2database</groupId>
  <artifactId>h2</artifactId>
  <version>1.4.181</version>
</dependency>
</dependencies>
</project>
