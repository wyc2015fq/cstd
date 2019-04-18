# Struts2中json的使用总结 - z69183787的专栏 - CSDN博客
2015年11月16日 16:25:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2970
首先看下面的配置文件中的两种写法：当然Action所在的包必须继承json-default:
<package name="default" extends ="json-default" >
第一种：
**[html]**[view
 plain](http://blog.csdn.net/sunshine_bean/article/details/8287812#)[copy](http://blog.csdn.net/sunshine_bean/article/details/8287812#)
- <actionname="upload_*"class="uploadAction"method="{1}">
- <resulttype="json">
- <paramname="root">fileName</param>
- </result>
- </action>
第二种：
**[html]**[view
 plain](http://blog.csdn.net/sunshine_bean/article/details/8287812#)[copy](http://blog.csdn.net/sunshine_bean/article/details/8287812#)
- <actionname="uploadJson"class="uploadJsonAction"method="upload">
- <resulttype="json"name="success">
- </result>
- </action>
当然，两种方式都能达到相应的效果，但是开发中我们建议使用第一种；
- 第一种指定了struts2需要帮你转换成的json的Action中bean的属性：filename；也就是struts2只会调用getFileName()这一个get方法，忽略其他bean属性的的get*方法，自然节省了不少资源。
- 第二种没有指定需要转换成json的属性，只要Action中有get*方法的，strut2都会帮你转换，你节省的代码换来了程序不必要的开支。
但是，strut2的开发者显然做的比较得当，下面的注解方式可以帮你改进不少你的代码，当然个人觉得还是加上root方便、简单：
- @JSON(serialize=false) ：如果你不想让某个属性被json序列化，只要在其get*方法上面加上以下注解即可；
- @JSON(name="newName")：返回的json数据中名称是指定的新名称
- @JSON(format ="yyyy-MM-dd'T'HH:mm:ss")设置用于格式化输出、解析日期表单域的格式。
- @JSON(deserialize=true)设置是否反序列化该属性
下面看看其他几种配置的写法：
第三种：excludeNullProperties 参数：表示是否去掉空值， 默认值是false，如果设置为true会自动将为空的值过滤，只输出不为空的值。
**[html]**[view
 plain](http://blog.csdn.net/sunshine_bean/article/details/8287812#)[copy](http://blog.csdn.net/sunshine_bean/article/details/8287812#)
- <resulttype="json">
- <paramname="excludeNullProperties">true</param>
- </result>
第四种：ignoreHierarchy 参数：表示是否忽略等级，也就是继承关系，比如：TestAction继承于BaseAction，那么TestAction中返回的json字符串默认是不会包含父类BaseAction的
        属性值，ignoreHierarchy值默认为true，设置为false后会将父类和子类的属性一起返回。
**[html]**[view
 plain](http://blog.csdn.net/sunshine_bean/article/details/8287812#)[copy](http://blog.csdn.net/sunshine_bean/article/details/8287812#)
- <resulttype="json">
- <paramname="ignoreHierarchy">false</param>
- </result>
第五种： includeProperties 参数：输出结果中需要包含的属性值(如果属性是对象的引用，若要json化引用的所有属性，则使用person.*)，这里正则表达式和属性名匹配，可以用“,”分割填充多个正则表达式。
**[html]**[view
 plain](http://blog.csdn.net/sunshine_bean/article/details/8287812#)[copy](http://blog.csdn.net/sunshine_bean/article/details/8287812#)
- <resulttype="json">
- <paramname="includeProperties">person.*, person\.name</param>
- </result>
第六种：excludeProperties 参数：输出结果需要剔除的属性值，也支持正则表达式匹配属性名，可以用“,”分割填充多个正则表达式，
**[html]**[view
 plain](http://blog.csdn.net/sunshine_bean/article/details/8287812#)[copy](http://blog.csdn.net/sunshine_bean/article/details/8287812#)
- <resulttype="json">
- <paramname="excludeProperties ">person.*, person\.name</param>
- </result>
下面讲讲几种特殊的情况：
action 代码：
**[html]**[view
 plain](http://blog.csdn.net/sunshine_bean/article/details/8287812#)[copy](http://blog.csdn.net/sunshine_bean/article/details/8287812#)
- public class TestingAction extends ActionSupport {  
- 
-     private static final long serialVersionUID = 1L;  
-     private static Log log = LogFactory.getLog(TestingAction.class);  
- 
-     private String resultStr = "success";//判断自测是否成功  
- 
-     private Integer testHeadId;// 试卷id  
-     private String testLineId;// 试题id  
-     private String userId;// 登录用户的id  
-     private String testResult;// 测试结果 google gson拼凑的结果集  
-     private TestLine testLine;//该道题目的相关信息  
-     public List<ITestLine> testLines;//android端返回的json  
Bean：
**[html]**[view
 plain](http://blog.csdn.net/sunshine_bean/article/details/8287812#)[copy](http://blog.csdn.net/sunshine_bean/article/details/8287812#)
- public class TestLine  implements  
-         java.io.Serializable {  
- 
-     // Fields  
- 
-     private String testLineId;  
-     private TestHead testHead;  
-     private Integer type;  
-     private Integer sortIndex;  
-     private Integer score;  
-     private String tigan;  
-     private String tizhi;  
-     private String jiexi;  
-     private String daan;  
-     private Timestamp lastUpdateTime;  
-     private String lastUpdateBy;  
上面大家看到，action中有一个对象的引用testLine；而testLine实体中又包含另一个对象TestHead的引用；其他的我就不说了，如果你也碰到这种情况，实体嵌套，那么使用struts2的json序列化时，默认会帮你序列化这个bean（testLine）中所有的属性，属性中如果涉及其他实体引用时，会继续帮你序列化该引用（TestHead）的属性值，就这样，你有多少层的嵌套，struts2就会帮你序列化多少层，这是你想要的结果吗？
处理这种问题的方法是：我们只需要告诉struts2我需要序列化那些属性:我们拿到经过Hibernate处理的之后的结果oldTestLine之后，需要重新创建我们需要序列化的对象：
TestLine testLine = new TestLine（）;
然后将需要的字段set值即可；
testLine .set(oldTestLine.get*);
处理需要序列化的集合（list、 map）问题（List<ITestLine> testLines）也是同解；
至于比较不错的json架包，个人推荐google gson。
推荐篇使用struts2返回各种类型的文章：[http://www.open-open.com/lib/view/open1325518231062.html](http://www.open-open.com/lib/view/open1325518231062.html)
