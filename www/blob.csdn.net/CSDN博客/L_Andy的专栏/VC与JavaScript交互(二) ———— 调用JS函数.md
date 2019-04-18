# VC与JavaScript交互(二) ———— 调用JS函数 - L_Andy的专栏 - CSDN博客

2014年10月11日 18:53:30[卡哥](https://me.csdn.net/L_Andy)阅读数：5335
个人分类：[VC/MFC编程](https://blog.csdn.net/L_Andy/article/category/1099539)



这一章，我们来动手实践VC调用[JS函数](http://www.so.com/s?q=JS%E5%87%BD%E6%95%B0&ie=utf-8&src=se_lighten_f)。

我们动手写一个HTML，其中包含这样一段JS代码:

**[html]**[view
 plain](http://blog.csdn.net/charlessimonyi/article/details/18450783#)[copy](http://blog.csdn.net/charlessimonyi/article/details/18450783#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/161967/fork)

- <scripttype="text/javascript">
-     function Add(value1, value2) {  
-         return value1 + value2;  
-     }  
- </script>

然后我们用WebBrowser加载这个HTML后，在VC中这样来调用这个函数名为Add的JS函数:


**[cpp]**[view
 plain](http://blog.csdn.net/charlessimonyi/article/details/18450783#)[copy](http://blog.csdn.net/charlessimonyi/article/details/18450783#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/161967/fork)

- //别忘了#include <MsHTML.h>
- //m_WebBrowser是一个WebBrowser的Activex控件对象。
- CComQIPtr<IHTMLDocument2> spDoc = m_WebBrowser.get_Document();  
- CComDispatchDriver spScript;  
- spDoc->get_Script(&spScript);  
- 
- CComVariant var1 = 10, var2 = 20, varRet;  
- spScript.Invoke2(L"Add", &var1, &var2, &varRet);  

![](https://img-blog.csdn.net/20140118145931937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2hhcmxlc1NpbW9ueWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


spScript.Invoke2的作用是调用JS函数中名为Add的函数，传入两个参数，用varRet接收返回值。

可以看到，Invoke2调用成功后，varRet得到了返回值30。

但这样的话一次只能接受一个返回值。

如果要一次接受多个返回值的话，怎么办呢？

我们可以让JS返回一个JS中的Array数组或Object对象。

当JS函数return一个Array或一个Object对象时，VC这边的varRet将接受到一个代表该对象的[IDispatch](http://www.so.com/s?q=IDispatch&ie=utf-8&src=se_lighten_f)接口。我们仍然用CComDispatchDriver来管理这个IDispatch。用上[一篇文章](http://www.so.com/s?q=%E4%B8%80%E7%AF%87%E6%96%87%E7%AB%A0&ie=utf-8&src=se_lighten_f)介绍的CComDispatchDriver的四个方法：

GetProperty

GetPropertyByName

PutProperty

PutPropertyByName

来从这个Array或Object对象中取出我们要的数据。

实践是检验真理的唯一标准，让我们再来写一个JS函数:

**[html]**[view
 plain](http://blog.csdn.net/charlessimonyi/article/details/18450783#)[copy](http://blog.csdn.net/charlessimonyi/article/details/18450783#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/161967/fork)

- <scripttype="text/javascript">
-     function Add(value1, value2) {  
-         var array = new Array();  
-         array[0] = value1;  
-         array[1] = value2;  
-         array[2] = value1 + value2;  
-         return array;  
-     }  
- </script>

然后在VC中这样写：


**[cpp]**[view
 plain](http://blog.csdn.net/charlessimonyi/article/details/18450783#)[copy](http://blog.csdn.net/charlessimonyi/article/details/18450783#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/161967/fork)

- CComQIPtr<IHTMLDocument2> spDoc = m_WebBrowser.get_Document();  
- CComDispatchDriver spScript;  
- spDoc->get_Script(&spScript);  
- 
- CComVariant var1 = 10, var2 = 20, varRet;  
- spScript.Invoke2(L"Add", &var1, &var2, &varRet);  
- 
- CComDispatchDriver spArray = varRet.pdispVal;  
- //获取数组中元素个数，这个length在JS中是Array对象的属性，相信大家很熟悉
- CComVariant varArrayLen;  
- spArray.GetPropertyByName(L"length", &varArrayLen);  
- //获取数组中第0,1,2个元素的值：
- CComVariant varValue[3];  
- spArray.GetPropertyByName(L"0", &varValue[0]);  
- spArray.GetPropertyByName(L"1", &varValue[1]);  
- spArray.GetPropertyByName(L"2", &varValue[2]);  

![](https://img-blog.csdn.net/20140118150104609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2hhcmxlc1NpbW9ueWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


可以看到，10,20,30，这三个JS函数返回的值已经躺在我们的varValue[3]里了。

当然，如果不知道JS返回的Array对象里面有几个元素，我们可以在VC这边获取它的length属性，然后在一个循环中取出数组中的每个值。

如果我们的JS函数返回一个包含有多个属性值的Object对象，VC这边该如何接收呢？

让我们再来写一个JS函数:

**[html]**[view
 plain](http://blog.csdn.net/charlessimonyi/article/details/18450783#)[copy](http://blog.csdn.net/charlessimonyi/article/details/18450783#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/161967/fork)

- <scripttype="text/javascript">
-     function Add(value1, value2) {  
-         var data = new Object();  
- data.result = value1 + value2;  
- data.str = "Hello,我是小明!";  
-         return data;  
-     }  
- </script>

然后在VC中我们这样接收：


**[cpp]**[view
 plain](http://blog.csdn.net/charlessimonyi/article/details/18450783#)[copy](http://blog.csdn.net/charlessimonyi/article/details/18450783#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/161967/fork)

- CComQIPtr<IHTMLDocument2> spDoc = m_WebBrowser.get_Document();  
-     CComDispatchDriver spScript;  
-     spDoc->get_Script(&spScript);  
- 
-     CComVariant var1 = 10, var2 = 20, varRet;  
-     spScript.Invoke2(L"Add", &var1, &var2, &varRet);  
- 
-     CComDispatchDriver spData = varRet.pdispVal;  
-     CComVariant varValue1, varValue2;  
-     spData.GetPropertyByName(L"result", &varValue1);  
-     spData.GetPropertyByName(L"str", &varValue2);  

![](https://img-blog.csdn.net/20140118150239468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2hhcmxlc1NpbW9ueWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


我们从JS返回的Object对象里取出了它的两个属性，result和str，分别是一个整形数据和一个字符串。

这里JS代码是我们自己写的，在VC这边当然事先知道这个JS函数返回的对象有result和str这两个属性。

如果JS代码不是我们写的，或者它的属性是事先不能确定的，该怎么办呢？答案是使用IDispatchEx接口来枚举这个对象的相关信息(方法名、属性名)。

这个现在暂时不讲，在后续的文章中会讲。

当然，JS不只可以返回Object对象，返回什么对象都可以，当返回一个对象而非基本数据类型（整形、浮点、字符串）时，VC这边收到的返回值是一个IDispatch，然后我们需要调用GetPropertyByName方法从这个IDispatch代表的对象中取出它的属性来。

这样一来，VC调用JS函数，传递参数给JS和JS返回返回值给VC，大致就都会了。

对于CComVariant包装的VARIANT这种智能型变量，不了解的可以到网上看下相关资料。《[深入解析ATL](http://www.so.com/s?q=%E6%B7%B1%E5%85%A5%E8%A7%A3%E6%9E%90ATL&ie=utf-8&src=se_lighten_quotes_f)》之类的书上均有介绍。

[值得注意](http://www.so.com/s?q=%E5%80%BC%E5%BE%97%E6%B3%A8%E6%84%8F&ie=utf-8&src=se_lighten_f)的是ATL提供的这些CCom开头的智能包装类，并不依赖于ATL的动态库。因为我在VC项目中并没有选择链接ATL，程序调试运行时进程加载的模块中也有没有ATL100.dll之类的模块载入。大家可以放心使用而不用担心依赖上ATL。

VC调用JS函数没问题了。那么JS函数如何调用VC呢？我们将在下一篇文章中慢慢道来。

