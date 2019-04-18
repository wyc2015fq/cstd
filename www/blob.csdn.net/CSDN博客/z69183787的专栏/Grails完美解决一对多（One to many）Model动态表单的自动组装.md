# Grails完美解决一对多（One to many）Model动态表单的自动组装 - z69183787的专栏 - CSDN博客
2015年01月05日 19:05:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：755
这里的试验是在Grails下做的，但是在Spring MVC中应该也能适用，即便不行，通过简单的扩展也能达到效果。对于其他框架像Struct之类应该也都没问题（不了解，不确定）。
能实现的自动封装包含：
1.一层简单模型
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7192228#)[copy](http://blog.csdn.net/yanical/article/details/7192228#)
- class AddressCommand {  
-     String city  
-     String street  
- int hourseNumber  
- }  
2.One to one，多层模型
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7192228#)[copy](http://blog.csdn.net/yanical/article/details/7192228#)
- class PersonCommand {  
-     String name  
- int age  
-     AddressCommand address  
- }  
3.One to many, 多层模型
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7192228#)[copy](http://blog.csdn.net/yanical/article/details/7192228#)
- class ChildCommand {  
-     String name  
- int age  
- }  
- 
- class PersonCommand {  
-     String name  
- int age  
-     AddressCommand address  
-     List children  
- }  
这个模型模型就是我们最终要使用的模型。这里我们不讨论Many to One 和 Many to Many模型，不常见（我还没遇到过），而且也可以通过转换转成以上几种类型；例子中的模型只有两层，多层的依葫芦画瓢，原理不变，增加了一些复杂性。
我们先创建一个Controller，之后我们都会把数据Post到save Action，如果数据对的话，Grails就会帮助自动组装好模型：
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7192228#)[copy](http://blog.csdn.net/yanical/article/details/7192228#)
- class PersonController {      
-     def save={PersonCommand person->       
- 
-     }  
- }  
## 一.数据传输格式
数据传输格式是最重要的问题，直接关系到后续过程中前，后台的工作量。
对于一层简单模型和One to one多层模型，数据格式是很成熟的。现有的框架都处理的很好，对于上面的model，他们的数据格式如下：
|Name|Value|
|----|----|
|name|yanical|
|age|26|
|address.city|Zhuhai|
|address.street|Jida|
|address.hourseNumber|344|
其实我们要解决的主要是一对多的问题。一对多模型里面有个list（array）对象，在form里的数据是扁平的，我们要在扁平的数据里带上list顺序特性。
对于Grails，我尝试了几种数据格式，比如:
|Name|Value|
|----|----|
|children.0.name|little|
|children.0.age|2|
都没有成功，最后在网上别人建议了如下的格式：
|Name|Value|
|----|----|
|children[0].name|little|
|children[0].age|2|
|children[1].name|large|
|children[1].age|4|
这里List有两个元素。这个时候还是没有成功，但是出了如下的exception：
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7192228#)[copy](http://blog.csdn.net/yanical/article/details/7192228#)
- Stacktrace follows:  
- java.lang.IndexOutOfBoundsException: Index: 0, Size: 0
-     at java.util.ArrayList.RangeCheck(ArrayList.java:547)  
-     at java.util.ArrayList.get(ArrayList.java:322)  
-     at java.lang.Thread.run(Thread.java:619)  
这个exception说明，grails的form->model模型组装模块已经识别了上传的数据，只是因为我们的list是空的，在组装模型的时候Grails试图从List中找到对应的element来塞数据，但是没有成功。修改Person类后，模型就可以成功组装了。
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7192228#)[copy](http://blog.csdn.net/yanical/article/details/7192228#)
- class PersonCommand {  
-     String name  
- int age  
-     AddressCommand address  
-     List children = []  
- 
-     Person() {  
-         children[0] = new ChildCommand()  
-         children[1] = new ChildCommand()  
-     }  
- }  
## 二.子对象的初始化
上面我们发现对于根对象Person，和One to one的对象，Grails都会帮忙创建对象实例，但是对于One to many，Grails并没有做相关工作。我们要想个方案能够在组装模型前，能把对应的对象实例都创建好。
有两种思路：1.是在代码里就先hardcode创建好一定数量的实例（像前面做的那样），比如10个，在能确定实例数量会很小的时候，这种方案也还可行。2.是考虑可以不可以通过插入一些代码，实现在组装模型的时候动态创建实例。 
这里的实现采用第二种思路。
看回上面的Exception，这个exception是去list里取数据的时候发现数据不存在的时候抛出来的。说明数据组装模块会去List里拿实例，再把数据塞到实例里面去，我们就在数据组装模块去List拿数据的时候插入一些代码。
我们使用一个工具来达到这个目的，这个工具就是org.apache.commons.collections.list.LazyList，LazyList在我们这里的作用就是确保数据组装模块去List取数据的时候始终能够把实例取出来。在List的里面有这个对象实例的时候，就把对象实例返回，否则就创建一个新的对象实例。修改Person类如下：
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7192228#)[copy](http://blog.csdn.net/yanical/article/details/7192228#)
- class PersonCommand {  
-     String name  
- int age  
-     AddressCommand address  
-     List children = LazyList.decorate([],  
-               FactoryUtils.instantiateFactory(ChildCommand.class))  
- }  
这个时候模型就已经可以成功创建了，即便List里面会有很多的元素，也能很好的处理。
## 三.Form表单定义
接下来，前台的任务就是要组装出前面要求的数据格式，对于One to one，我们还是一笔带过，form表单如下：
**[html]**[view
 plain](http://blog.csdn.net/yanical/article/details/7192228#)[copy](http://blog.csdn.net/yanical/article/details/7192228#)
- 
**[html]**[view
 plain](http://blog.csdn.net/yanical/article/details/7192228#)[copy](http://blog.csdn.net/yanical/article/details/7192228#)
- <inputtype="text"name="name"id="name"value="">
- <inputtype="text"name="age"id="age"value="">
- <inputtype="text"name="address.city"id="age"value="">
- <inputtype="text"name="address.street"id="age"value="">
- <inputtype="text"name="address.hourseNumber"id="age"value="">
对于One to many，页面中肯定会有Add和Delete的地方来增加和删除某一个元素。由于增加或者删除元素的过程中会调整UI，所以希望能够提取出一段公用的JS来实现这个功能。代码大致如下，包含两个方法onOneToManyAddClick和onOneToManyDeleteClick ，分别用来处理Add和Delete点击事件（基于JQuery，现在不需要理会这段代码的细节）：
**[javascript]**[view
 plain](http://blog.csdn.net/yanical/article/details/7192228#)[copy](http://blog.csdn.net/yanical/article/details/7192228#)
- function onOneToManyAddClick(triggerElement, oneToManyProperty) {  
- var table = $("table[scaffoldFor='"+oneToManyProperty+"']");  
- var count = table.attr("count");  
- if(count == ""){  
-         count = 0;  
-     }  
- 
- var tr = $("tr[scaffoldFor='"+oneToManyProperty+"']").filter(function() {  
- return $(this).is(":hidden");  
-     }).clone();  
- 
-     tr.attr("index",count);  
- var inputs = $(tr).find(":input");//has include input, textarea, select and button
-     $.each(inputs, function(i, n) {  
- var input = $(n);  
- var name = input.attr("name");  
-         name = name.replace("*","["+count+"]");  
-         input.attr("name",name);  
-         input.attr("id",name);  
-     });  
- 
-     count++;  
-     table.attr("count", count);  
-     tr.appendTo(table)  
-     tr.show();  
- }  
- function onOneToManyDeleteClick(triggerElement, oneToManyProperty) {  
- var current = $(triggerElement);  
- var currentTr;  
- while(true) {  
- if(current.is("tr") && current.attr("scaffoldFor") && oneToManyProperty.indexOf(current.attr("scaffoldFor"))>=0) {  
-             currentTr = current;  
- break;  
-         } else {  
-             current = current.parent();  
-         }  
-     }  
- 
- var currentIndex = currentTr.attr("index");  
- var currentFor = currentTr.attr('scaffoldFor');  
- 
- var table = $("table[scaffoldFor='"+currentFor+"']");  
- var count = table.attr("count");  
- 
- //remove element -- start
- var nextTrs = currentTr.nextAll("tr").filter(function() {  
- return $(this).attr("scaffoldFor") == currentFor;  
-     });  
-     $.each(nextTrs, function(i, n) {  
- var tr = $(n);  
- var index = tr.attr("index");  
- var replaceFrom = currentFor+"["+index+"]";  
- var replaceTo = currentFor+"["+(index-1)+"]";  
-         tr.attr("index", (index-1));  
-         $.each($(tr).find(":input"), function(i, n) {  
- var input = $(n);  
- var name = input.attr("name");  
-             name = name.replace(replaceFrom, replaceTo);  
-             input.attr("name",name);  
-             input.attr("id",name);  
-         });  
-     });  
- 
- if(count == ""){  
-         table.attr("count", 0);  
-     }  
-     count--;  
-     table.attr("count", count);  
- 
-     currentTr.remove();  
- //remove element -- end
- }  
而，我们的One to many form表单就是这样的了：
**[html]**[view
 plain](http://blog.csdn.net/yanical/article/details/7192228#)[copy](http://blog.csdn.net/yanical/article/details/7192228#)
- <tablescaffoldfor="children"count="1">
- <tbody>
- <trscaffoldfor="children"style="display: none">
- <td>
- <inputtype="text"name="children*.name"id="children*.name"value="">
- <inputtype="text"name="children*.age"id="children*.age"value="">
- <span>
- <ahref="#"onclick="onOneToManyDeleteClick(this,'children');return false;">Add</a>
- <ahref="#"onclick="onOneToManyAddClick(this,'children');return false;">Delete</a>
- </span>
- </td>
- </tr>
- <tr>
- </tr>
- <trscaffoldfor="children"index="0">
- <td>
- <inputtype="text"name="children[0].name"id="children[0].name"value="">
- <inputtype="text"name="children[0].age"id="children[0].age"value="">
- <span>
- <ahref="#"onclick="onOneToManyDeleteClick(this,'children');return false;">Add</a>
- <ahref="#"onclick="onOneToManyAddClick(this,'children');return false;">Delete</a>
- </span>
- </td>
- </tr>
- </tbody>
- </table>
初始化的时候包含两个TR：第二个是可见的，就是用户打开页面是可以看到的输入框；第一个是隐藏的，用户点击Add的时候，我们会克隆这个TR并append到table最后显示给用户，隐藏的TR的数据不需要提交给后台，我们设置特殊的input name属性（中间包含*，如children*.name），这样后台Grails就会忽略这些input。 
用户在点击Add的时候，我们要修正克隆的input的name熟悉；用户点击Delete的时候，可能要调整现有input的name属性。这些都在JS里实现，详情请参考JS代码，有兴趣的话，现在可以去读那段代码了。
关键字: Grails, Spring MVC，Java web, 一对多, one to may， 动态表单， 自动组装
** 版权所有，转发请注明来源**
