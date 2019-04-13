
# spring boot @Value() 不识别 - lin_tuer的博客 - CSDN博客


2017年11月07日 17:12:58[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：6335


在配置文件中配置了mongoDB的连接参数


然后在MongoDB类中希望通过@Value() 注解来获得这些参数


可是 运行项目却报错了，

可怕的空指针异常，
于是乎，我很不专业地打印了一下，发现注解的变量都是null


可怕-----------

找了许久，网上有各种方案，我尝试的额脑袋昏昏，最后终于有一个成功啦啦啦，，
首先 在这里找到了原因，https://stackoverflow.com/questions/30767248/value-is-always-null
发现是因为我的类即使添加了@Component也没有自动加载到java bean

这篇文章给出了许多种，普通类读取application.properties的方法
http://www.cnblogs.com/hafiz/p/5876243.html

然后，，我用其方法五才成功，定义了一个读取 application.properties 属性值的方法类，利用这个工具类间接读取属性~~然后，终于好啦
具体代码暂时不贴出来啦，感谢大神~

