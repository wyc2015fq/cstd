# MPEG-7实例入门 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月14日 23:54:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：49
个人分类：[MPEG7/图像检索](https://blog.csdn.net/leixiaohua1020/article/category/1362935)










**摘要：**看完本文，你将(1)了解什么是MPEG-7(2)理解MPEG-7中的一些基本概念（什么是D,DS,DDL）。



**一. MPEG-7简介**



**(1)什么是MPEG-7**

MPEG-7的正式名称是“多媒体内容描述接口”(Multimedia Content Description Interface)，是由运动图像专家组(MPEG,Moving Picture Experts Group)提出的一个用于描述多媒体内容的ISO/IEC标准。

简单而言，MPEG-7其实就是一个规定如何来描述多媒体内容的特征的标准。

明确一点：MPEG-7跟MPEG-2、MPEG-4等除了名字有点像以外没有更多的相同点。MPEG-2、MPEG-4关注的是多媒体本身的编码压缩，而MPEG-7关注的是多媒体内容特征的描述。



**(2) MPEG-7标准的范围**

MPEG-7标准仅仅规定了如何描述，组织特征。特征的提取、特征的使用不属于MPEG-7标准的范围。

以图片内容搜索引擎为例，图片特征的提取算法不属于MPEG-7标准范围（提取算法也没有标准化的必要），同样搜索引擎如何使用图片特征也不在MPEG-7范围，只有特征的描述（下文将介绍如何描述特征）是MPEG-7标准范围内的。



![](http://pic002.cnblogs.com/images/2011/63234/2011011817170890.gif)


图1. MPEG-7标准的范围



**(3) MPEG-7应用领域**

MPEG-7比不针对特定的应用领域。就目前来看，适合应用MPEG-7的领域包括：

<1>基于内容的多媒体搜索（包括图像搜索、哼唱搜索、语音搜索等）

<2>图像理解

<3>其他需要使用大量多媒体特征的应用



**二. MPEG-7主要概念**

**(1)特征( Feature )**

用于描述数据的有区分性的特性。比如声音的频率，图形的灰度直方图等就是一些简单的特征。



**(2)描述符( Ds, Descriptors )**

也称描述子。描述符通常缩写成D或Ds。D通常是一些基本特征的语法和语义。D是最基本的MPEG-7元素，也就是说一个描述符的定义只能使用基本的数据类型，而不能包含别的D。



**(3)描述方案(DSs, Description Schemes)**

也称描述模式。描述方案通常所写成DS或DSs。一个DS的定义除了可以使用基本的数据类型外，还可以包含其他的DS或D。



**(4)描述值( Descriptor Value )**
 是描述符的一个实例。



**(5)描述( Description )**

由一个描述方案(结构)和一组描述值组成。



**(6)描述定义语言( DDL, Description Definition Language)**

用于定义新的D和DS。MPEG-7 DDL是在XML Schema基础上扩展而成，主要所作的扩展有两点：支持数组与矩阵类型，增加了其他的派生数据类型。



![](http://pic002.cnblogs.com/images/2011/63234/2011011817171969.gif)

图2. D，DS，DDL，Description的关系



图2说明了D，DS，DDL之间的关系。这三者的关系可以用下面几句话简要概括：

<1> D与DS规定了描述特征的语法。DS和D实例化后就是对一个多媒体特征的描述。

<2> DS可以由D和其他DS构成。

<3> DDL定义D和DS。

这三者的关系可以用C++与类的关系作类比，C++定义类，类可以用基本的数据类型来定义，也可以通过组合、继承其他类来定义，类是对象的结构定义，对象是类的实例化。

类似的，DDL定义了D和DS，D只能用基本的数据类型来定义，DS的定义则可以使用其他的DS与D，描述值是D的实例化。



什么？你还不懂？上实例！

**三. MPEG-7实例**

下面我将通过实际代码来说明描述符，描述值，描述方案，描述的概念。保证你看得懂，前提是你已经能看懂XML Schema。（如果您还不会XML Schema请问谷哥度娘。）



**(1)描述符**

下面是一个名为“ScalableColorType”的描述符的定义：



```
<complexType name="ScalableColorType" final="#all">
  <complexContent>
    <extension base="mpeg7:VisualDType">
      <sequence>
        <element name="Coeff" type="mpeg7:integerVector"/>
      </sequence>
      <attribute name="numOfCoeff" use="required">
        <simpleType>
          <restriction base="integer">
            <enumeration value="16"/>
            <enumeration value="32"/>
            <enumeration value="64"/>
            <enumeration value="128"/>
            <enumeration value="256"/>
          </restriction>
        </simpleType>
      </attribute>
      <attribute name="numOfBitplanesDiscarded" use="required">
        <simpleType>
         <restriction base="integer">
            <enumeration value="0"/>
            <enumeration value="1"/>
            <enumeration value="2"/>
            <enumeration value="3"/>
            <enumeration value="4"/>
            <enumeration value="6"/>
            <enumeration value="8"/>
          </restriction>
        </simpleType>
      </attribute>
    </extension>
  </complexContent>
</complexType>
```





可以看出这个“ScalableColorType”描述符是由“mpeg7:VisualDType”类型扩展而来，包含有一个名为“Coeff”的“mpeg7:integerVector”类型的元素，以及名为“numOfCoeff”和名为“numOfBitplanesDiscarded”的枚举类型属性各一个。

由于“ScalableColorType”的定义仅仅使用了基本的数据类型，而没有用到其他的D或DS，因此我们将其称为一个描述符(D)。



**(2)描述值**



下面的代码是“ScalableColorType”描述符的一个实例，也就是一个描述值。　　



```
<VisualDescriptor type="ScalableColorType“ numOfBitplanesDiscarded="0" numOfCoeff="64">
    <Coeff>
        -121 8 -3 87 12 14 22 37 31 13 11 3 50 14 19 21 -3 1 0 11
        -8 5 0 17 -8 2 2 4 -15 5 1 -1 1 0 0 1 0 0 1 1 6 1 1 3 1 2 4 12
        -1 0 2 2 2 3 3 -4 15 0 0 -2 1 0 -3 6
    </Coeff>
</VisualDescriptor>
```







将这个描述值跟上面给出的“ScalableColorType”描述符对比着看，可以发现描述值的结构是由描述符规定的，描述值是描述符的一个实例。



**(3)描述方案**



下面是一个名为“GoFGoPColorType”的描述方案的定义。








```
<complexType name="GoFGoPColorType" final="#all">
  <complexContent>
    <extension base="mpeg7:VisualDType">
      <sequence>
        <element name="ScalableColor" type="mpeg7:ScalableColorType"/>
      </sequence>
      <attribute name="aggregation" use="required">
        <simpleType>
          <restriction base="string">
            <enumeration value="Average"/>
            <enumeration value="Median"/>
            <enumeration value="Intersection"/>
          </restriction>
        </simpleType>
      </attribute>
    </extension>
  </complexContent>
</complexType>
```




可以看出“GoFGoPColorType”描述方案是由“mpeg7:VisualDType”类型扩展而来，包含了一个“ScalableColorType”类型的元素，以及一个名为“aggregation”的枚举类型的属性。

由于“GoFGoPColorType”的定义中包含了一个“ScalableColorType”描述符，因此我们将其称为一个描述方案(DS)。



**(4)描述**

下面的代码是对一个图片文件生成的一个描述。其中包含了一个“ScalableColorType”类型的描述值。





```
<?xml version="1.0" encoding="UTF-8"?>
<image>
    <title>Twilight</title>
    <comments>nice picture</comments>
    <tags>sunset sky</tags>
    <Mpeg7>
        <Description type="ContentEntityType">
            <MultimediaContent type="ImageType">
                <Image>
                    <VisualDescriptor type="ScalableColorType“ numOfBitplanesDiscarded="0" numOfCoeff="64">

         <Coeff>
        -121 8 -3 87 12 14 22 37 31 13 11 3 50 14 19 21 -3 1 0 11
        -8 5 0 17 -8 2 2 4 -15 5 1 -1 1 0 0 1 0 0 1 1 6 1 1 3 1 2 4 12
        -1 0 2 2 2 3 3 -4 15 0 0 -2 1 0 -3 6
          </Coeff>
                        </VisualDescriptor>

                </Image>
            </MultimediaContent>
        </Description>
    </Mpeg7>
</image>
```







**四. MPEG-7标准组成**



(1) MPEG-7视频

(2) MPEG-7音频

(3) MPEG-7多媒体

(4)描述定义语言DDL

(5) MPEG-7参考软件



其中(1)(2)(3)主要是对视频，音频，多媒体成百上千个的描述符和描述方案，而且至今仍在不断增加。而(5)则是根据MPEG-7标准开发的软件实验平台。

如果你想深入了解MPEG-7 DDL可以阅读参考文献2。关于MPEG-7视频、音频、多媒体以后我有时间还会写些文章深入探讨。





**参考资料：**

[1]*MPEG-7 Overview*

[http://mpeg.chiariglione.org/standards/mpeg-7/mpeg-7.htm](http://mpeg.chiariglione.org/standards/mpeg-7/mpeg-7.htm)



[2]Jane Hunter.*An Overview of the MPEG-7 Description Definition Language (DDL)*




