# 【FastJSON】解决FastJson中“$ref 循环引用”的问题 - z69183787的专栏 - CSDN博客
2019年01月04日 16:00:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：85
[https://blog.csdn.net/qq_35873847/article/details/78850528](https://blog.csdn.net/qq_35873847/article/details/78850528)
【需求与环境描述】
0、开发环境
　　SSH，EasyUI，MySQL 
1、需求要求：
　　(1)首先获取所有的贷款订单数据，即List <LoanOrder>。
　　(2)然后从单个贷款订单实体LoanOrder去访问贷款人实体Loaner的信息。 
2、实体之间的关系描述
　　(1)LoanOrder实体与Loaner实体是双向的多对一和一对多关系。
　　(2)LoanOrder是“多方”，其中的关系属性为“private Loaner loaner”。
　　(3)Loaner是“一方”，其中的关系属性为“Set<LoanOrder> orders”。 
3、代码示例
　　(1) 贷款订单LoanOrder代码
|12345678910111213|`@Entity``@Table``(name = ``"t_bp_loan_order"``)``public``class``LoanOrder ``implements``java.io.Serializable {``    ``    ``/*省略其他次要属性*/``    ``private``Loaner loaner;``    ``@ManyToOne``(fetch = FetchType.LAZY)``    ``@JoinColumn``(name = ``"LOANER_ID"``)``    ``public``Loaner getLoaner() {``        ``return``this``.loaner;``    ``}``}`|
　　(2) Loaner方代码
|12345678910111213|`@Entity``@Table``(name = ``"t_bp_loaner"``)``public``class``Loaner ``implements``java.io.Serializable {``    ``/* 其他普通属性略去 */``    ``private``Set<LoanOrder> loanOrders = ``new``HashSet<LoanOrder>(``0``);``    ``@OneToMany``(fetch = FetchType.LAZY, mappedBy = ``"loaner"``)``    ``@JSONField``(serialize = ``true``)``    ``public``Set<LoanOrder> getLoanOrders() {``        ``return``this``.loanOrders;``    ``}``}`|
【障碍再现】
1、需求01：
首先获取所有的贷款订单数据，即List <LoanOrder>，发现贷款人“张三”有两个订单。
![](http://images0.cnblogs.com/blog2015/668603/201507/081451585333169.png)
2、需求02：
　　然后，依次在第一个和第二个贷款订单中点击“张三”，从而去访问“名字叫做‘张三’”贷款人实体Loaner的信息。
结果，第一个订单可以显示贷款人的数据，但是在第二个订单数据中，不能获取到“loaner（贷款人）”的数据，并且loaner中提示“$ref”。
经过两次点击后，服务器后台传送到前台页面上的JSON数据如下所示：
![](http://images0.cnblogs.com/blog2015/668603/201507/081452267526048.png)
【解决方案】
第一步：禁用FastJson的“循环引用检测”特性。
1、核心代码
![](http://images0.cnblogs.com/blog2015/668603/201507/081453098147860.png)
2、作用
决定了生成的“多个”JSON对象中，是否加载被引用的同一个对象的数据。
在此，决定了生成的“多个”贷款订单JSON对象中，是否加载被引用的同一个贷款人JSON对象的数据。
3、开启和关闭FastJson的“循环引用检测”特性的对比
![](http://images0.cnblogs.com/blog2015/668603/201507/081453336743770.png)
当从服务器端传来的多个LoanOrder对象通过FASTJSON被序列化到“前端”后，会被浏览器解析成“DOM”对象。 
(1) 当开启FastJson的“循环引用检测”特性时：
1）对于第一个LoanOrder 01，fastjson会完全解析并加载它的所有数据，包括它所关联的Loaner贷款人信息，如下图所示。
![](http://images0.cnblogs.com/blog2015/668603/201507/081453593306865.png)
2） 对于第二个LoanOrder 02，fastjson仅仅解析并加载其贷款订单部分的数据，对于“$ref”所指向的 Loaner贷款人的数据，fastjson会因为“开启了fastJson的‘循环引用检测’机制”而不去加载该贷款人数据。
当加载第二个贷款订单数据时，fastjson检测到已经在第一个订单LoanOrder 01中加载了“贷款人Loaner”的数据，fastjson会因为“开启了‘循环引用检测’机制”而不去再次加载该贷款人数据，而仅仅将一个指向第一个贷款订单LoanOrder01中“贷款人”的引用赋值给第二个贷款订单中的贷款人的位置。
因此，在生成的第二个贷款订单的JSON串中，对于贷款人信息，仅仅只有一个“$ref”。
而jQuery这个前端技术又无法解析该引用，因此，就无法读取贷款人的数据，如下图所示。
![](http://images0.cnblogs.com/blog2015/668603/201507/081455150804323.png)
第二步：禁止Loaner对象获取Set<LoanOrder>的数据。
方法一：将原来的“双向关系”修改为“单向关系”
1、原来：LoanOrder与Loaner之间是双向关系。
2、修改后：只能从LoanOrder访问Loaner，从Loaner无法访问到LoanOrder。
3、具体方法01
       重要前提：不删除Loaner中的“Set<LoanOrder> orders”属性。
![](http://images0.cnblogs.com/blog2015/668603/201507/081457022839371.png)
　　注意，若在采用注解映射实体类的方式中，没有使用“@Transient”注解，则数据库会报错。
4、具体方法02
       直接删除“Set<LoanOrder> orders”属性极其相关的setter()和getter()方法。
方法二：不修改关系的前提下，禁止序列化
在不修改LoanOrder和Loaner双向关系的情况下，Loaner对象中的Set<LoanOrder>集合完成数据的加载，当其向前端Browser传递JSON数据时，禁止序列化Set<LoanOrder>集合。
具体方法：
设置注解“@JSONField(serialize = false)”。
说明：
A.“@JSONField”是fastjson提供的注解标签，其作用为控制其所标注的属性“能否被序列化”。
B.在此其作用为：禁止"loanOrders"这个Set集合被序列化。
具体如下图所示。
![](http://images0.cnblogs.com/blog2015/668603/201507/081458007838299.png)
【解决后的效果】　
![](http://images0.cnblogs.com/blog2015/668603/201507/081458298307292.png)
读者如要转载，请标明出处和作者名，谢谢。
地址01：[http://space.itpub.net/25851087](http://space.itpub.net/25851087)
地址02：http://www.cnblogs.com/zjrodger
作者名：zjrodger  
### 问题描述：
今天在做后台传数据到前端解析的时候遇到了这个问题。背景介绍下：后台传过去json数据是用阿里的fastjson转换的，调用的是这个方法**[java]**[view plain](http://blog.csdn.net/u013185616/article/details/52799166#)[copy](http://blog.csdn.net/u013185616/article/details/52799166#)
- String s = JSON.toJSONStringWithDateFormat(o,dateformat,SerializerFeature.WriteDateUseDateFormat);  
- ServletActionContext.getResponse().getWriter().write(s);  
其中dataformat是格式化时间数据的。传过去的是一个PageBean对象，该对象继承了Page对象，Page里面包含list，pageNo,pageSize。前台在通过data.list准备取出list循环做点事情的时候，发现全是undefined，通过console.log(data.list)，发现这个数据竟然是这个东西$ref: "$.list[0]"。百度搜了下，这里就是循环引用造成的。
### 问题分析：
循环引用：当一个对象包含另一个对象时，fastjson就会把该对象解析成引用。引用是通过$ref标示的，下面介绍一些引用的描述
- "$ref":".." 上一级
- "$ref":"@" 当前对象，也就是自引用
- "$ref":"$" 根对象
- "$ref":"$.children.0" 基于路径的引用，相当于 root.getChildren().get(0)
### 解决方案：
fastjson提供了多种json转换方案，有兴趣的同学可以自己看看源码，这里我们可以采用禁止循环引用的方案：
**[java]**[view plain](http://blog.csdn.net/u013185616/article/details/52799166#)[copy](http://blog.csdn.net/u013185616/article/details/52799166#)
- String s = JSON.toJSONStringWithDateFormat(0,"yyyy-MM-dd HH:mm:ss",SerializerFeature.DisableCircularReferenceDetect);  
其中：**SerializerFeature.DisableCircularReferenceDetect**就是禁止循环引用的方案，我们可以通过枚举类SerializerFeature来查看到底有多少种方式：
**[java]**[view plain](http://blog.csdn.net/u013185616/article/details/52799166#)[copy](http://blog.csdn.net/u013185616/article/details/52799166#)
- public enum SerializerFeature {  
-     QuoteFieldNames,  
-     UseSingleQuotes,  
-     WriteMapNullValue,  
-     WriteEnumUsingToString,  
-     UseISO8601DateFormat,  
-     /** 
-      * @since 1.1 
-      */  
-     WriteNullListAsEmpty,  
-     /** 
-      * @since 1.1 
-      */  
-     WriteNullStringAsEmpty,  
-     /** 
-      * @since 1.1 
-      */  
-     WriteNullNumberAsZero,  
-     /** 
-      * @since 1.1 
-      */  
-     WriteNullBooleanAsFalse,  
-     /** 
-      * @since 1.1 
-      */  
-     SkipTransientField,  
-     /** 
-      * @since 1.1 
-      */  
-     SortField,  
-     /** 
-      * @since 1.1.1 
-      */  
-     @Deprecated  
-     WriteTabAsSpecial,  
-     /** 
-      * @since 1.1.2 
-      */  
-     PrettyFormat,  
-     /** 
-      * @since 1.1.2 
-      */  
-     WriteClassName,  
- 
-     /** 
-      * @since 1.1.6 
-      */  
-     DisableCircularReferenceDetect,  
- 
-     /** 
-      * @since 1.1.9 
-      */  
-     WriteSlashAsSpecial,  
- 
-     /** 
-      * @since 1.1.10 
-      */  
-     BrowserCompatible,  
- 
-     /** 
-      * @since 1.1.14 
-      */  
-     WriteDateUseDateFormat,  
- 
-     /** 
-      * @since 1.1.15 
-      */  
-     NotWriteRootClassName,  
- 
-     /** 
-      * @since 1.1.19 
-      */  
-     DisableCheckSpecialChar,  
- 
-     /** 
-      * @since 1.1.35 
-      */  
-     BeanToArray  
-     ;  
- 
-     private SerializerFeature(){  
-         mask = (1 << ordinal());  
-     }  
- 
-     private final int mask;  
- 
-     public final int getMask() {  
-         return mask;  
-     }  
- 
-     public static boolean isEnabled(int features, SerializerFeature feature) {  
-         return (features & feature.getMask()) != 0;  
-     }  
- 
-     public static int config(int features, SerializerFeature feature, boolean state) {  
-         if (state) {  
-             features |= feature.getMask();  
-         } else {  
-             features &= ~feature.getMask();  
-         }  
- 
-         return features;  
-     }  
- }  
