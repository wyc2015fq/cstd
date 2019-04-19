# jacob操作office word简单教程 - 零度的博客专栏 - CSDN博客
2017年03月10日 14:37:17[零度anngle](https://me.csdn.net/zmx729618)阅读数：5251
**现在的项目中操作word文件比较多，word文件的加密解密，转换为各种格式，插入图片，添加水印、html转word等等各种东西，大家也都知道，java语言是不能直接操作word或者excel的，不像C#，可以调用VBA的类来直接操作office。**
**目前java也有一些操作office的开源框架，比如poi，这个没怎么研究过，我估摸着它是通过解析office的xml文档模型来操作office的，因为office03和office07以及以上的文档模型结构不太一样，导致了poi处理不同版本office时要用不同的类，我用的最多的是jacob，它对office03和office07的操作基本一致，高版本的还没有试过，通过最近一段时间的使用，给大家分享一下我的心得。**
简介：
jacob（java com bridge，java com桥）分为两个部分，jacob.jar,jacob.dll，使用时两个东西的版本要一致，而且还分32位和64位，它的位数和jdk的位数有关，与操作系统的位数无关。它的原理是通过java的jni功能，调用系统组件dll，通过这个com桥来操作com组件（windows的一种软件编程技术吧），最终完成对office文档的操作。
环境配置：
1、在工程中引入jar包 
2、将dll文件放在jdk的path目录下面，通过System.getProperty("java.library.path");可以看到path路径，或者直接放到jdk/bin、jre/bin、system32/system64下面都放一份，总能找得到，版本要急着对应好。
3、附件为我使用的组件，好像是1.14版本，配合32位jdk使用。
常用类以及方法
**ComThread**：com组件管理，用来初始化com线程，释放线程，所以会在操作office之前使用，操作完成再使用。
**ActiveXComponent**：创建office的一个应用，比如你操作的是word还是excel 
**Dispatch**：调度处理类，封装了一些操作来操作office，里面所有的可操作对象基本都是这种类型，所以jacob是一种链式操作模式，就像StringBuilder对象，调用append()方法之后返回的还是StringBuilder对象
**Variant**：封装参数数据类型，因为操作office是的一些方法参数，可能是字符串类型，可能是数字类型，虽然都是1，但是不能通过，可以通过Variant来进行转换通用的参数类型，new Variant(1),new Variant("1"),
**Dispatch的几种静态方法**：这些方法就是要用来操作office的。 
•call( )方法：调用COM对象的方法，返回Variant类型值。 
•invoke( )方法：和call方法作用相同，但是不返回值。 
•get( )方法：获取COM对象属性，返回variant类型值。 
•put( )方法：设置COM对象属性。 
以上方法中有的有很多重载方法，调用不同的方法时需要放置不同的参数，至于哪些参数代表什么意思，具体放什么值，就需要参考vba代码了，仅靠jacob是无法进行变成的。
**Variant对象的toDispatch()方法**：将以上方法返回的Variant类型转换为Dispatch，进行下一次链式操作。
结合vba来进行一些操作的解读（以word为例，excel基本上差不多）
jacob操作office网上也有很多教程，比如打开文档，保存，插入图片之类的，看了那些，也只能会这几种操作，jacob文档中也不会有那种操作参数是什么，现在我结合vba代码分享一下我的理解，有什么新的操作就可以自己来完成。
**1、初始化com线程**
使用jacob之前使用下面的语句来初始化com线程，大概意思是打开冰箱门，准备放大象。。。
Java代码 ![复制代码](http://men4661273.iteye.com/images/icon_copy.gif)![收藏代码](http://men4661273.iteye.com/images/icon_star.png)![](http://men4661273.iteye.com/images/spinner.gif)
- ComThread.InitSTA();  
```java
ComThread.InitSTA();
```
使用完成后使用下面的语句来关闭现场,大概意思是关上冰箱门。。。
Java代码 ![复制代码](http://men4661273.iteye.com/images/icon_copy.gif)![收藏代码](http://men4661273.iteye.com/images/icon_star.png)![](http://men4661273.iteye.com/images/spinner.gif)
- ComThread.Release();  
```java
ComThread.Release();
```
**2、创建应用程序对象，设置参数，得到文档集合**
操作一个文档之前，我们必须要创建一个应用对应，比如是word还是excel，设置一些文档应用的参数，得到文档集合对象，（大家应该知道word是Documents，excel是WorkBooks）
Java代码 ![复制代码](http://men4661273.iteye.com/images/icon_copy.gif)![收藏代码](http://men4661273.iteye.com/images/icon_star.png)![](http://men4661273.iteye.com/images/spinner.gif)
- ActiveXComponent wordApp = new ActiveXComponent("Word.Application");//word
- ActiveXComponent wordApp = new ActiveXComponent("Excel.Application");//excel
- wordApp.setProperty("Visible", new Variant(false));//设置应用操作是文档不在明面上显示，只在后台静默处理。
```java
ActiveXComponent wordApp = new ActiveXComponent("Word.Application");//word
ActiveXComponent wordApp = new ActiveXComponent("Excel.Application");//excel
wordApp.setProperty("Visible", new Variant(false));//设置应用操作是文档不在明面上显示，只在后台静默处理。
```
这个操作就是模仿vba的写法，Application.Visible = False；vba代码中的对application的设置这里都可以用
C#的写法：new Microsoft.Office.Interop.Word.Application().Visible = false; 意思差不多
获得文档集合，用来操作我们需要处理的文档，
Java代码 ![复制代码](http://men4661273.iteye.com/images/icon_copy.gif)![收藏代码](http://men4661273.iteye.com/images/icon_star.png)![](http://men4661273.iteye.com/images/spinner.gif)
- Dispatch document = wordApp.getProperty("Documents").toDispatch();  
```java
Dispatch document = wordApp.getProperty("Documents").toDispatch();
```
vba写法Application.Documents，C#的写法:new Microsoft.Office.Interop.Word.Application().Documents
可以看出来基本相似，如果对c#比较熟悉的话，可以直接从c#中翻译过来，不熟悉的话可以直接网上查vba代码，进行翻译。
**3、打开文档**
有了文档对象集合，我们就可以来操作文档了，链式操作就此开始： 
call方法，调用open方法，传递一个参数，返回一个我们的word文档对象，
Java代码 ![复制代码](http://men4661273.iteye.com/images/icon_copy.gif)![收藏代码](http://men4661273.iteye.com/images/icon_star.png)![](http://men4661273.iteye.com/images/spinner.gif)
- Dispatch doc = Dispatch.call(document, "Open",new Variant("D:\\my.doc")).toDispatch();  
```java
Dispatch doc = Dispatch.call(document, "Open",new Variant("D:\\my.doc")).toDispatch();
```
同样从vba代码翻译过来，很多vba代码我们可以在word文档中自己找到，
![](http://dl2.iteye.com/upload/attachment/0099/6383/8afc8805-dcc0-3c12-b3ba-dde2698f3ba0.jpg)
![](http://dl2.iteye.com/upload/attachment/0099/6385/0532a80e-7541-3c19-85b9-b04a84a09279.jpg)
从vba代码中可以看到documents的open方法打开了一个文档，第一个参数就是文档的路径，后面的参数大家可以自己去研究，参数多的有多参数的方法，还有数据参数的方法，都可以用来调用。
现在打开了一个文档，我们可以进行操作了。
**4、操作文档，每页插入一个图片，并调整大小和位置**
我录制一个宏插入图片，翻到下一页，再插入一个图片，查看vba代码如下：
![](http://dl2.iteye.com/upload/attachment/0099/6387/f6a7a8be-358b-3ca2-b040-277f8f7f0f2a.jpg)
其中里面的Selection代表当前光标位置或者所选范围，官方解释如下：
该对象代表窗口或窗格中的当前所选内容。所选内容代表文档中被选定（或突出显示的）的区域，若文档中没有所选内容，则代表插入点。每个文档窗格只能有一个活动的 Selection对象，并且整个应用程序中只能有一个活动的 Selection对象。
翻译为java代码为：
Java代码 ![复制代码](http://men4661273.iteye.com/images/icon_copy.gif)![收藏代码](http://men4661273.iteye.com/images/icon_star.png)![](http://men4661273.iteye.com/images/spinner.gif)
- for(int i = 0; i < 2;i++){  
-             Dispatch selection = Dispatch.get(wordApp, "Selection").toDispatch();  
-             Dispatch inLineShapes = Dispatch.get(selection, "InLineShapes").toDispatch();   
-             Dispatch picture = Dispatch.call(inLineShapes, "AddPicture", imgPath).toDispatch();   
- //选中图片
-             Dispatch.call(picture, "Select");   
- //设置宽度高度
- 
-             Dispatch.put(picture, "Width", new Variant(10));   
-             Dispatch.put(picture, "Height", new Variant(10));   
- 
- //设置图片相对左上角偏移位置
-             selection = Dispatch.get(wordApp, "Selection").toDispatch();  
-             Dispatch shapeRange = Dispatch.get(selection, "ShapeRange").toDispatch();  
-             Dispatch.put(shapeRange, "Left", new Variant(left));  
-             Dispatch.put(shapeRange, "Top", new Variant(top));  
- 
- //翻到下一页
-             Dispatch browser = Dispatch.get(wordApp, "Browser").toDispatch();  
-             Dispatch.call(browser, "Next");  
-         }  
```java
for(int i = 0; i < 2;i++){
			Dispatch selection = Dispatch.get(wordApp, "Selection").toDispatch();
			Dispatch inLineShapes = Dispatch.get(selection, "InLineShapes").toDispatch(); 
			Dispatch picture = Dispatch.call(inLineShapes, "AddPicture", imgPath).toDispatch(); 
			//选中图片
			Dispatch.call(picture, "Select"); 
			//设置宽度高度
			
			Dispatch.put(picture, "Width", new Variant(10)); 
			Dispatch.put(picture, "Height", new Variant(10)); 
			
			//设置图片相对左上角偏移位置
			selection = Dispatch.get(wordApp, "Selection").toDispatch();
			Dispatch shapeRange = Dispatch.get(selection, "ShapeRange").toDispatch();
			Dispatch.put(shapeRange, "Left", new Variant(left));
			Dispatch.put(shapeRange, "Top", new Variant(top));
			
			//翻到下一页
			Dispatch browser = Dispatch.get(wordApp, "Browser").toDispatch();
			Dispatch.call(browser, "Next");
		}
```
从上面的代码可以看出，每一行代码获取一个对象，相应于vba的用“.”点出来的一个对象，大概意思就是得到选中位置，得到图形范围，插入图片，选中图片，设置宽高，设置相对当前页的左边距，上边距，然后将Application.Browser.Next翻译为最后两行代码，翻到下一页，继续插入图片进行处理。
这里是固定的页码，如果我们不知道文档的页码呢，请看下一个案例。
**5、得到word文档的页码：**
网上找到一段获取文档页数的VBA代码Selection.Information(wdNumberOfPagesInDocument)，按照我们的理解，翻译为java代码为：
Java代码 ![复制代码](http://men4661273.iteye.com/images/icon_copy.gif)![收藏代码](http://men4661273.iteye.com/images/icon_star.png)![](http://men4661273.iteye.com/images/spinner.gif)
- Dispatch selection = Dispatch.get(wordApp, "Selection").toDispatch();  
- //如果调用方法或者属性要得到某个值的话，直接tostring或者别的就可以了
- pages = Dispatch.call(selection, "Information",new Variant(4)).toString();  
```java
Dispatch selection = Dispatch.get(wordApp, "Selection").toDispatch();
//如果调用方法或者属性要得到某个值的话，直接tostring或者别的就可以了
pages = Dispatch.call(selection, "Information",new Variant(4)).toString();
```
调用selection的Information方法，传入一个参数可以看到，参数中我写的是4，哪里来的呢？
vba中的wdNumberOfPagesInDocument，我们不知道是什么，肯定是一个常量，都说在msdn上能找到，我是找不到，我有个办法，
打开我们word中的vba编辑器，吧vba代码贴到我们录制的宏的第一行，左边添加一个端点，运行一下，
![](http://dl2.iteye.com/upload/attachment/0099/6393/a2896360-269a-32b7-99e6-f01b27318f03.jpg)
这样就可以看到常量值了，是4，写上就行可，可以得到页码的结果，另外还有一种获得页数的方法： 
ActiveDocument.BuiltInDocumentProperties(wdPropertyPages)
比葫芦画瓢，翻译为java代码为
Java代码 ![复制代码](http://men4661273.iteye.com/images/icon_copy.gif)![收藏代码](http://men4661273.iteye.com/images/icon_star.png)![](http://men4661273.iteye.com/images/spinner.gif)
- Dispatch activeDocument  = Dispatch.get(wordApp, "ActiveDocument").toDispatch();   
- pages = Dispatch.call(activeDocument, "BuiltInDocumentProperties",new Variant(14)).toString();  
```java
Dispatch activeDocument  = Dispatch.get(wordApp, "ActiveDocument").toDispatch(); 
pages = Dispatch.call(activeDocument, "BuiltInDocumentProperties",new Variant(14)).toString();
```
通过wordApp获取ActiveDocument对象，调用它的BuiltInDocumentProperties方法，传入一个参数wdPropertyPages的值是14，而上面的还有另外一种写法，官方解释说以下两行代码意思一样，所以还可以翻译为另一种写法
BuiltinDocumentProperties.Item(1)  
BuiltinDocumentProperties(1)
先获取BuiltinDocumentProperties对象，再调用Item方法
Java代码 ![复制代码](http://men4661273.iteye.com/images/icon_copy.gif)![收藏代码](http://men4661273.iteye.com/images/icon_star.png)![](http://men4661273.iteye.com/images/spinner.gif)
- Dispatch activeDocument  = Dispatch.get(wordApp, "ActiveDocument").toDispatch();   
- Dispatch builtInDocumentProperties  = Dispatch.get(activeDocument, "BuiltInDocumentProperties").toDispatch();   
- pages = Dispatch.call(builtInDocumentProperties, "Item",new Variant(14)).toString();  
```java
Dispatch activeDocument  = Dispatch.get(wordApp, "ActiveDocument").toDispatch(); 
Dispatch builtInDocumentProperties  = Dispatch.get(activeDocument, "BuiltInDocumentProperties").toDispatch(); 
pages = Dispatch.call(builtInDocumentProperties, "Item",new Variant(14)).toString();
```
**6、保存文档**
Java代码 ![复制代码](http://men4661273.iteye.com/images/icon_copy.gif)![收藏代码](http://men4661273.iteye.com/images/icon_star.png)![](http://men4661273.iteye.com/images/spinner.gif)
- Dispatch.call(doc, "Save");  
```java
Dispatch.call(doc, "Save");
```
doc是我们打开的文档的对象
**7、退出wordapplication**
参数有很多个，我们一个都不传，执行完后winword进程关闭
Java代码 ![复制代码](http://men4661273.iteye.com/images/icon_copy.gif)![收藏代码](http://men4661273.iteye.com/images/icon_star.png)![](http://men4661273.iteye.com/images/spinner.gif)
- wordApp.invoke("Quit", new Variant[] {});  
```java
wordApp.invoke("Quit", new Variant[] {});
```
**8、释放com线程**
Java代码 ![复制代码](http://men4661273.iteye.com/images/icon_copy.gif)![收藏代码](http://men4661273.iteye.com/images/icon_star.png)![](http://men4661273.iteye.com/images/spinner.gif)
- ComThread.Release();  
```java
ComThread.Release();
```
结束
    以上是两个小案例还分享一下我个人的心得，个人感觉jacob还是很好用的，只不过有时候有的对象不知道怎么获取，只能一点一点的尝试，比如selection，activeDocument，录制宏也看不到是怎么来的，最终发现都是wordapplication来的，所以宏里面开始的东西如果找不到都可以从application里面去找的。
欢迎大家共同研究，如有不正确的地方欢迎指正。
