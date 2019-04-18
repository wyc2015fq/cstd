# Velocity脚本新手教程 - weixin_33985507的博客 - CSDN博客
2015年10月03日 17:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3

##      从网络下的数据汇编
## 一、Velocity简介
Velocity它是Apache该公司的开源产品，它是一套基于Java语言模板引擎，背景可以非常灵活的数据与模板文件一起反对。他直言不讳地说：，人使用模板语言引用后台java代码定义的对象Velocity应用于Web开发时，界面设计人员能够和java程序开发者同步开发一个遵循MVC架构的web网站。也就是说。页面设计人员能够仅仅关注页面的显示效果，而由Java程序开发者关注业务逻辑编码。Velocity将 java代码从web页面中分离出来。这样为web网站的长期维护提供了便利。
Velocity是一个基于java的模板引擎（templateengine），它同意不论什么人只简单的使用模板语言（template language）来引用由java代码定义的对象。
作为一个比較完好的模板引擎。Velocity的功能是比較强大的，但强大的同一时候也添加了应用复杂性。
非常多人下载了EasyJWeb的开源应用演示样例。使用EasyJWeb的目的，是在于把页面－程序完毕分开。就也就是一个项目中，程序猿使用专业Java开发工具（如Eclipse、JBuilder等)来编写、调试、測试程序，页面制作人员使用专业的网页制作工具（如Macromedia Dreamweaver）来设计制作网页。而两者之间的协调通过一个规范的接口协议来解决。须要在页面里面加如一些标签。来生成动态内容。这一工作能够交由网页制作人员来完毕。由于使用Velocity作为视图，由于他的语法、功能及用法都比較简单，因此普通情况下，一天以内就能让页面制作人员熟练掌握其用法。
## 二、Velocity基本的语法
**1、声明:#set ($var=XXX)**
　　左边能够是下面的内容
　　Variable reference 
　　String literal 
　　Property reference 
　　Method reference 
　　Number literal #set ($i=1) 
　　ArrayList #set ($arr=["yt1","t2"])
　　算术运算符
**2、凝视:**
　　单行## XXX
　　多行#* xxx
　　    XXX*#
**3变量 Variables **
　　以 "$" 开头，第一个字符必须为字母。character followed by a VTL Identifier. (a .. z or A .. Z).变量能够包括的字符有下面内容：
　　alphabetic (a .. z, A .. Z) 
　　numeric (0 .. 9) 
　　hyphen ("-") 
　　underscore ("_") 
**4、属性Properties **
　　$Identifier.Identifier
　　$user.name
　　hashtable user中的的name值.类似：user.get("name")
**5、方法Methods **
　　object user.getName() = $user.getName()
**6、正规引用格式注意事项Formal Reference Notation **
　　用{}把变量名跟字符串分开，如
　　#set ($user="csy"}
　　${user}name 
　　返回csyname
　　$username
　　$!username
**$与$!的差别**
　　当找不到username的时候，$username返回字符串"$username"，而$!username返回空字符串"" 
**7、双引號 与引號**
　　#set ($var="helo")
　　test"$var" 返回testhello
　　test'$var' 返回test'$var'
　　能够通过设置 stringliterals.interpolate=false改变默认处理方式
**8、条件语句**
　　#if( $foo ) 
　　 <strong>Velocity!</strong>
　　#end
　　#if($foo)
　　#elseif()
　　#else
　　#end
　　当$foo为null或为Boolean对象的false值运行.
**9、逻辑运算符:== && || !10、循环语句**
#foreach($varin $arrays )
 // 集合包括以下三种Vector, aHashtable or an Array
#end
　　#foreach( $product in $allProducts )
　　 <li>$product</li>
　　#end
　　#foreach( $key in $allProducts.keySet() )
　　 <li>Key: $key -> Value: $allProducts.get($key)</li>
　　#end
　　#foreach( $customer in $customerList )
　　<tr><td>$velocityCount</td><td>$customer.Name</td></tr>
#end
**11、velocityCount变量在配置文件里定义**
　　# Default name of the loop counter
　　# variable reference.
　　directive.foreach.counter.name = velocityCount
　　# Default starting value of the loop
　　# counter variable reference.
　　directive.foreach.counter.initial.value = 1
**12、包括文件**
　　#include( "one.gif","two.txt","three.htm")
**13、Parse导入脚本**
　　#parse("me.vm" )
**14、#stop 停止运行并返回 15、定义宏Velocimacros ,相当于函数 支持包括功能**
　　#macro( d )
　　 <tr><td></td></tr>
　　#end
　　调用 
　　#d()
**带參数的宏**
　　#macro( tablerows $color $somelist )
　　#foreach( $something in $somelist )
　　 <tr><td bgcolor=$color>$something</td></tr>
　　#end
　　#end
**16、范围操作符Range Operator **
　　#foreach( $foo in [1..5] )
**17、"#"用来标识Velocity的脚本语句。包含#set、#if 、#else、#end、#foreach、#end、#iinclude、#parse、#macro等；**
如:
#if($info.imgs)
<img src="$info.imgs" border=0>
#else
<img src="noPhoto.jpg">
#end
**18、"$"用来标识一个对象(或理解为变量)；**
如：$i、$msg、$TagUtil.options(...)等。
**19、"{}"用来明白标识Velocity变量。**
比方在页面中。页面中有一个$someonename。此时，Velocity将把someonename作为变量名，若我们程序是想在someone这个变量的后面紧接着显示name字符，则上面的标签应该改成${someone}name。
**20、"!"用来强制把不存在的变量显示为空白。**
如当页面中包括$msg。假设msg对象有值，将显示msg的值。假设不存在msg对象同，则在页面中将显示$msg字符。
这是我们不希望的，为了把不存在的变量或变量值为null的对象显示为空白，则仅仅须要在变量名前加一个“!”号就可以。
如：$!msg
## 三、EasyJWeb建议
在界面模板中使用过多过复杂的脚本表达方式，在万不得已的情况下。不要在界面模板中增加不论什么复杂的逻辑。更不要在界面模板中增加变量声明、逻辑运算符等等。
EasyJWeb提供了五条主要的模板脚本语句，基本上就能满足全部应用模板的要求。这四条模板语句非常easy，能够直接由界面设计人员来增加。
在当前非常多EasyJWeb的应用实践中，我们看到，全部界面模板中归纳起来仅仅有以下四种简单模板脚本语句就可以实现：
1、$!obj 　直接返回对象结果。
　　 如：在html标签中显示java对象msg的值。<p>$!msg</p>
　 在html标签中显示经过HtmlUtil对象处理过后的msg对象的值　　<p>$!HtmlUtil.doSomething($!msg)</p>
2、#if($!obj) #else#end 推断语句
　　 如：在EasyJWeb各种开源应用中，我们常常看到的用于弹出提示信息msg的样例。
　　 #if($msg)
　　 <script>
　　 alert('$!msg');
　　 </script>
　　 #end
  的脚本表示当对象msg对象存在时，输出<script>等后面的内容。
3、#foreach( $info in$list) $info.someList #end　　循环读取集合list中的对象，并作对应的处理。
　　 如：EasyJF开源论坛系统中论(0.3)坛首页显示热门主题的html界面模板脚本：
　　#foreach( $info in $hotList1) 
<a href="/bbsdoc.ejf?easyJWebCommand=show&&cid=$!info.cid"target="_blank">$!info.title</a><br>
　　　 #end 
　　 上面的脚本表示循环遍历hotList1集合中的对象，并输出对象的相关内容。
4、#macro(macroName)#end 脚本函数(宏)调用，不推荐在界面模板中大量使用。
　　 如：在使用EasyJWebTools高速生成的添删改查演示样例中。能够点击列表的标题栏进行升降排序显示，这是我们在EasyJWeb应用中常常看到的一个排序状态显示的模板内容。
　　 函数(宏)定义，一般放在最前面。案例：
#macro(orderPic $type)
　　 #if ($orderField.equals($type)) 
　　 <imgsrc="http://images.cnblogs.com/ico/${orderType}.gif"> 
　　 #end
　　 #end
详细的调用如：<font color="#FFFFFF">头衔#orderPic("title")</font>
5、包括文件#inclue("模板文件名称")或#parse("模板文件名称")
　　主要用于处理具有同样内容的页面。比方每一个站点的顶部或尾部内容。如：#parse("/blog/top.html")或#include("/blog/top.html")
　　parse与include的差别在于，若包括的文件里有Velocity脚本标签，将会进一步解析。而include将原样显示。
## 四、入门样例
Velocity应用于javaproject时，创建完一个普通的javaproject后，须要引入对应的jar包。才干让project支持Velocity模板引擎(velocity-1.7.jar/commons-*.jar)
**1.样例一：**
VelJavaproject的后台java代码`VelJava.java`
//构造函数  
public VelJava() throws IOException{  
    //初始化Velocity模板  
    Velocity.init();  
    //创建一个VeloctiyContext对象  
    VelocityContext context=new VelocityContext();  
    //向VelocityContext对象中放入一个键值对  
    context.put("list", getNames());  
    Template template=null;  
    //通过静态方法获取一个模板  
    template=Velocity.getTemplate("test.vm");  
    //创建一个输出流  
    BufferedWriter writer=new BufferedWriter(new OutputStreamWriter(System.out));  
    //将模板与context中的对象结合，然后输出  
    if(template!=null)  
        template.merge(context, writer);  
    //刷新缓存  
    writer.flush();  
    //关闭writer  
    writer.close();  
}  
//VelocityContext键值对中的值  
public ArrayList<String> getNames(){  
    ArrayList<String> list=new ArrayList<String>();  
    list.add("element 1");  
    list.add("element 2");  
    list.add("element 3");  
    list.add("element 4");  
    return list;  
}
备注
在这个样例中。以健值对的形式将getNames()函数的返回值存放在Velocity容器中，并给其一个键值为list
模板文件的代码例如以下test.vm：
##声明了一个变量  
#set( $this = "ppTest")  
##将变量打印出来  
$this is great!  
##对后台的list进行扫描。将集合中的元素一个一个的打印出来  
#foreach( $name in $list )  
$name is showed!  
#end  
##设置一个推断条件，将将推断条件赋值为真  
#set( $condition = true)  
##推断条件为真。运行  
#if ($condition)  
The condition is true!  
##推断条件为假时，运行  
#else  
The condition is false!  
#end
备注
当中的##表示的是凝视
#set类表示的是预处理指令
$变量名表示声明的是一个变量
上面的代码凝视的已经非常清楚了。假设仅仅是应用，不须要了解很多其它细节的话，上面的代码已经够了，从上面的代码中能够看出Velocity的一个优点就是模板文件与后台文件能够同步开发，仅仅要约定好一些共同拥有的变量定义就可以。在模板输出时。由模板引擎进行变量的替换,替换之后再进行对应的输出
**样例二：Velocity动态创建模板并渲染(Javaproject)**
演示了在Javaproject将模板渲染后以指定的编码方式GBK输出。同一时候也演示了动态创建模板。然后进行渲染
public static void main(String[] args) {  
    //初始化Velocity引擎  
    Velocity.init();  
    //获取一个VelocityContext对象  
    VelocityContext context=new VelocityContext();  
    //向此对象容器中增加对应的键值对  
    context.put("name", "Velocity");  
    context.put("project", "Jakarta");  
    //StringWriter底层事实上就是一个StringBuffer  
    StringWriter w=new StringWriter();  
    //将test2.vm与context进行合并，生成的终于代码写入StringWriter的buf中  
    Velocity.mergeTemplate("test2.vm", "GBK",context,w);  
    //将其在控制台上打印出来  
    System.out.println("模板:"+w);  
    //动态创建模板  
    String s="正在使用 $project $name 渲染模板";  
    w=new StringWriter();  
    /* 
     *context：对输入的字符串进行渲染 
     *w:渲染后的结果输出的地方 
     *mystring:错误发生时，被用来作为错误文件的名字 
     *s:包含VTL语言的输入字符串 
    */   
    Velocity.evaluate(context, w, "mystring", s);  
    //将结果进行输出来  
    System.out.println(w);  
}
样例二的模板文件test2.vm例如以下：
Hello 来自于 $name 在 $project 工程里.
样例二与样例一极为相似，样例二的模板仅仅只是是在代码中创建的，样例一中的模板是直接引用外部的test.vm文件，从上面的代码中能够揣測一下Velocity是怎样工作的，大概是这种
1、将外部的vm文件读入内存
2、Velocity模板引擎对vm文件进行解析
3、解析之后对模板文件里的变量进行替换。并运行vm中的对应推断逻辑
4、运行完后。整个页面的渲染结果就知道了，再直接输出就可以
从这里能够看出仅仅要前后台代码的开发者能够约定同样的变量。则前后台的开发全然能够并行运行
推荐文章：http://www.cnblogs.com/yasin/archive/2010/04/02/1703188.html
                 http://www.yanyulin.info/pages/2014/03/velocity_disabuse_1.html
版权声明：本文博主原创文章，博客，未经同意不得转载。
