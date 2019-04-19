# uuid的使用 - xqhrs232的专栏 - CSDN博客
2015年04月23日 10:22:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：566
原文地址::[http://blog.csdn.net/superch0054/article/details/9843093](http://blog.csdn.net/superch0054/article/details/9843093)
相关文章
1、UUID百度百科----[http://baike.baidu.com/link?url=buCZWYd2WODNW-67kfsouFOr7UQifPRGROOv9HvftNsDRVoOrSTHiB22zolLy0P-KhkWmS3PTPvdE1cRXoiQoK](http://baike.baidu.com/link?url=buCZWYd2WODNW-67kfsouFOr7UQifPRGROOv9HvftNsDRVoOrSTHiB22zolLy0P-KhkWmS3PTPvdE1cRXoiQoK)
UUID含义是通用唯一识别码 (Universally Unique Identifier)，这是一个[软件](http://baike.baidu.com/view/37.htm)建构的标准.
       UUID 的目的，是让[分布式系统](http://baike.baidu.com/view/991489.htm)中的所有元素，都能有唯一的辨识资讯，而不需要透过中央控制端来做辨识资讯的指定。如此一来，每个人都可以建立不与其它人冲突的 UUID。在这样的情况下，就不需考虑数据库建立时的名称重复问题。目前最广泛应用的 UUID，即是[微软](http://baike.baidu.com/view/2353.htm)的 Microsoft's
 Globally Unique Identifiers (GUIDs)，而其他重要的应用，则有 Linux ext2/ext3 档案系统、GNOME、KDE、Mac
 OS X 等等。
UUID是指在一台机器上生成的数字，它保证对在同一时空中的所有机器都是唯一的。通常平台会提供生成的API。按照[开放软件基金会](http://baike.baidu.com/view/770031.htm)(OSF)制定的标准计算，用到了以太网卡地址、纳秒级时间、芯片ID码和许多可能的数字
UUID由以下几部分的组合：
（1）当前日期和时间，UUID的第一个部分与时间有关，如果你在生成一个UUID之后，过几秒又生成一个UUID，则第一个部分不同，其余相同。
（2）时钟序列。
（3）全局唯一的IEEE机器识别号，如果有网卡，从网卡MAC地址获得，没有网卡以其他方式获得。
UUID的唯一缺陷在于生成的结果串会比较长。关于UUID这个标准使用最普遍的是[微软](http://baike.baidu.com/view/2353.htm)的GUID(Globals
 Unique Identifiers)。使用UUID的好处在分布式的[软件系统](http://baike.baidu.com/view/8343.htm)中，能保证每个节点所生成的标识都不会重复，并且随着WEB服务等整合技术的发展，UUID的优势将更加明显。
GUID是一个128位长的数字，一般用16进制表示。算法的核心思想是结合机器的网卡、当地时间、一个随即数来生成GUID。从理论上讲，如果一台机器每秒产生10000000个GUID，则可以保证（概率意义上）3240年不重复。
UUID是jdk1.5中新增的一个类，在java.util下，用它可以产生一个号称全球唯一的ID. UUID是由一个十六位的数字组成,表现出来的形式例如
550E8400-E29B-11D4-A716-446655440000  
下面是java代码生成uuid的例子（这个就比时间戳或者是随机数更加专业和靠谱了）
import java.util.UUID;
  public static String getUUID() {   
        UUID uuid =UUID.randomUUID();   
        String str = uuid.toString();  
        // 去掉"-"符号
        String temp = str.substring(0, 8) +str.substring(9, 13) + str.substring(14, 18) + str.substring(19, 23) +str.substring(24);   
        returnstr+","+temp;   
    }   
