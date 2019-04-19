# WEB开发 路径问题 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2017年11月20日 23:40:04[dutong0321](https://me.csdn.net/dutong0321)阅读数：112
今天在开发的过程中，妹子遇到了一个问题，开发Java WEB网页开发中的struts路径问题。 
情景再现一下： 
struts.xml:
```
<action name="Study" class="com.utopia.controller.StudyAction"
    method="study">
    <result name="success">study/success.jsp</result>
</action>
```
在success.jsp中要引入jquery.js文件，该文件位于UtopiaTest项目下的js文件夹中，所以，引入的路径是
```xml
<script type="text/javascript" src="../js/jquery.js"></script>
```
现在浏览器在执行[http://localhost:8080/UtopiaTest/Study](http://localhost:8080/UtopiaTest/Study)请求后，发现无法引入该js文件中。 
查看浏览器报错为：[http://localhost:8080/js/jquery.js](http://localhost:8080/js/jquery.js) 404错误，为什么这个地方解析出来的路径是这个？ 
其实如果仅仅相对于项目中的success.jsp和jquery.js的关系来讲，jquery.js是完全可以查找到的，可是在项目的实际运行中就不可以。 
首先了解下浏览器和服务器的运行，虽然浏览器在请求路径当中，一般可以看出服务器是用什么语言进行编写的，如：.asp一般是.NET语言;.php是PHP语言，但是其实对浏览器来讲，服务器你是用什么语言编写，关我P事，我只负责向你发送HTTP请求，让其年后再把接收到的文本流解析完，我就继续睡觉了。而服务器也不管浏览器的，就是接收到请求，然后响应请求，把处理好的文本流发送回浏览器。所以，可以看出：浏览器是不关心服务器的项目路径的。 
当我们发送[http://localhost:8080/UtopiaTest/Study](http://localhost:8080/UtopiaTest/Study)后，虽然我们接收到的是以study/success.jsp为模板的内容，但是对于浏览器来讲，这就是[http://localhost:8080/UtopiaTest/Study](http://localhost:8080/UtopiaTest/Study)的内容。浏览器在读取到../js/jquery.js后，也就根据路径找到了[http://localhost:8080/js/jquery.js](http://localhost:8080/js/jquery.js)文件，但事实上并没有这个文件。 
如何规避这一类问题呢？ 
1.在struts.xml里面别瞎配文件夹了，那自然而然的项目路径相当于浏览器路径了。 
2.在写路径时尽量写绝对路径，而且对于jquery这种库，可以采用CDN的方式加载，既不占用流量，而且据说速度会很快。但是我调试的时候依然喜欢采用本地库的方式加载，有时候断网了也不影响继续敲代码。 
3.其实这种是最正规的，大部分大型项目里一般都是采用的这种方式。就是采用基路径的方式，可以把基路径设置到项目下，然后在这个jsp当中写其他项目，都采用基路径的相对路径。其实，这个的原理就是写绝对路径么。
