# CMarkup  概述 - 深之JohnChen的专栏 - CSDN博客

2007年08月17日 09:57:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2467



**CMarkup7.2 概述**

[Markup72.zip](http://www.firstobject.com/Markup72.zip) 422k

原文出自[http://www.firstobject.com/dn_markup.htm](http://www.firstobject.com/dn_markup.htm)

创建一个新的XML文档，分析修改现存的XML文档，所有的这些都中可以从一个利用MFC或STL字符串所实现的一个简单类的方法中得到。关于CMarkup，你可以运行Zip文件右边的例子，如果你愿意，你可以通过看源码，几分钟把它加入到你自己的应用中。

**特征**

Cmarkup是依据EDOM的，这是这个简单XML分析的关键。EDOM是操作XML元素、属性和数据的简明方法集。当你不需要复杂的XML特性时，它避免你读大量的DOM和SAX文档。

如果你希望使用微软的MSXML，CMarkup工程中CmarkupMSXML类全面的演示了通过EDOM来使用MSXML，并且它是以C++ COM语法开始。

下载包（看上面zip文件的链接）中包含了测试对话框工程的源码、Cmarkup类的所有测试和示例、以及编译选项。关于用在商业用途中的详细说明请看文章底部的在Licensing中的注意事项。       

下面列出了CMarkup的特征：

独立：不需要任何外部的XML组件。

小：编译到你的工程只是很小的一个类，并且对于文档只维持了一个字符串，以及一个总计小于字符串内存大小的索引数组。

快：分析器在相当快的建立索引数组。

简单：EDOM方法使创建、导航和修改ＸＭＬ无比的简单。

MSXML：CMarkupMSXML类用EDOM方法包装了微软的XML服务。MSXML编译选项用来演示这个类。

UNICODE（统一字符编码标准）：对于WindowsNT和WindowsCE平台能够被编译成UNICODE，在WindowsNT UNICODE下，XML文档是流到UTF-8文件中，但是内部用宽字符。

UTF-8：对于UTF-8文件，这接受和返回UTF-8字符串（确保_MBCS没有被定义）

MBCS：对于双字节数据（定义_MBCS），它不与UTF-8兼容.

STL：CMarkupSTL类完全没有使用MFC，它的演示要使用STL编译选项。

**每天数据的XML**

 我们经常在一个文件里需要保存或传递信息，或从计算机Ａ 发送一个信息块到计算机Ｂ。总是有这样一个问题出现：*我要怎样安排这些数据的格式？*出现XML之前，你可能要考虑“env”类型，例如PATH=C:/WIN95；　“ini”类型（一些部分的组合），逗号限制或其它一些限制、或者是固定了字符串的长度。现在XML就是被确定的这些问题的答案，但是某些时候，程序员很气馁，他们需要一些便于帮助分析和格式尖括号所有这些的解决XML的复杂性。对于最低限度读XML标签匹配规则，我建议阅读在CcodeProject站点的“[Beginning XML - Chapter 2: Well-Formed XML](http://211.94.207.99:1111/*401http://www.codeproject.com/books/wrox_beginningxml2.asp)”

XML更好是因为它的灵活和自然的等级，加上它广泛的应用。虽然XML比限定格式用了更多的字符，如果需要的话它可以良好的向下压缩。当你需要扩展你文档信息类型时，XML的灵活性得到了体现，不需要要求信息的每个消费者重写逻辑过程。你能够保持旧的信息标识，并且制定用相同的方式增加新属性和元素。

**使用CMarkup**

Cmarkup是基于“压缩”文档对象模型(EDOM ： "Encapsulated" Document Object Model,)，这是这个简单XML处理器的关键。它的一套XML处理方法和产生的效果与DOM(Document Object Model)是一样的。但是整个DOM有无数的对象类型，而EDOM只定义了一个对象：XML文档，EDOM回到了XML原始的吸引力—简单。关于方法的完整文档，请看[**EDOM Specification**](http://211.94.207.99:1111/*401http://www.firstobject.com/dn_edom.htm).

CmarkUp封装了XML文档文本、结构和当前位置。它提供了增加元素、导航和得到元素属性和数据的所有方法以。在文档中操作被执行的地方取决于当前位置和当前孩子位置。当前位置允许你增加一个元素到文档中的这个位置，而不需要明确指出增加对象到文档的这点上，在CMarkup的有效生命周期里，对象一直保持着一个用来描述文档的字符串，当你调用`GetDoc`.可以得到它。 

查看[free firstobject XML editor](http://www.firstobject.com/dn_editor.htm)的源码，这是用CMarkup创建、导航你自己的XML文档。

**创建一个XML****文档**

对于创建一个XML文档，需要实例化一个CMarkup对象，并调用`AddElem`创建根元素。.在这个位置，如果你调用 `AddElem("ORDER")` ，你的文档会简单的装一个空ORDER元素`<``ORDER``/>`. 然后调用`AddChildElem` 在根元素的下面创建元素 (例如：“进入”根元素内部，层次表示).下面的示例代码创建一个XML文档并返回它（的内容）到一个字符串中。
`CMarkup xml;``xml.AddElem( "ORDER" );``xml.AddChildElem( "ITEM" );``xml.IntoElem();``xml.AddChildElem( "SN", "132487A-J" );``xml.AddChildElem( "NAME", "crank casing" );``xml.AddChildElem( "QTY", "1" );``CString csXML = xml.GetDoc();`
这些代码产生了下面的XML，这个根结点是ORDER元素；注意它的开始标签`<``ORDER``>` 在开头，结束标签`</``ORDER``>`在结尾。当一个元素是在一个父下面（深入或被包含），这个父元素的开始标签要在它之前，结束标签要在它之后。ORDER元素包含一个ITEM元素，而ITEM元素包含了三个字子元素：SN、NAME和QTY；
`<ORDER>``<ITEM>``<SN>132487A-J</SN>``<NAME>crank casing</NAME>``<QTY>1</QTY>``</ITEM>``</ORDER>`
如例子中所显示的，你也能够在一个子元素下创建新元素，这需要调用`IntoElem` 移动你的当前主位置到当前子元素位置，然后你就可以在这下面增加一个子元素了。CMarkup在索引中保持了一个当前位置指针，以保证你的源码更加短和更简单，当导航文件时，相同的逻辑位置也会被使用。

**导航****XML****文档**

上面的例子所创建的XML字符串，用SetDoc方法加入到CMarkup对象中能够被解析，你也可以引导它正确的进入被创建的同一个CMarkup对象中，如果你要设置当前位置到文档的开始时，需要调用ResetPos.

在下面的例子中，从csXML字符串生成CMarkup对象后，我们循环ORDER元素下的所有ITEM元素，并得到每个项目的序号和数量。
`CMarkup xml;``xml.SetDoc( csXML );``while ( xml.FindChildElem("ITEM") )``{``    xml.IntoElem();``    xml.FindChildElem( "SN" );``    CString csSN = xml.GetChildData();``    xml.FindChildElem( "QTY" );``    int nQty = atoi( xml.GetChildData() );``    xml.OutOfElem();``}`
对于我们发现的每个元素，在查询它了子元素之前要调用`IntoElem`，查询完之后再调用`OutOfElem` ，当你习惯于这种导航类型时，你将知道，检查你的循环时，要确定每个`IntoElem` 调用都有一个与之对应的`OutOfElem` 调用 。

**增加元素和属性**

上面创建文档的例子中仅创建了一个ITEM元素，现在这个例子是创建多个项目，从前一个内容加裁后，再增加数据源，加上SHIPMENT信息元素中有一个属性，这段代码也演示了你能调用调用`IntoElem`和`AddElem`来代替`AddChildElem`,函数调用。虽然这意味着更多的调用，但许多人认为这样更直观。
`CMarkup xml;``xml.AddElem( "ORDER" );``xml.IntoElem(); // inside ORDER``for ( int nItem=0; nItem<aItems.GetSize(); ++nItem )``{``    xml.AddElem( "ITEM" );``    xml.IntoElem(); // inside ITEM``    xml.AddElem( "SN", aItems[nItem].csSN );``    xml.AddElem( "NAME", aItems[nItem].csName );``    xml.AddElem( "QTY", aItems[nItem].nQty );``    xml.OutOfElem(); // back out to ITEM level``}``xml.AddElem( "SHIPMENT" );``xml.IntoElem(); // inside SHIPMENT``xml.AddElem( "POC" );``xml.SetAttrib( "type", csPOCType );``xml.IntoElem(); // inside POC``xml.AddElem( "NAME", csPOCName );``xml.AddElem( "TEL", csPOCTel );`
这段代码产生了下面的XML，根元素ORDER包含两个ITEM元素和一个SHIPMENT元素，ITEM元素全都包含SN、NAME、和QTY元素，SHIPMENT元素包含一个带有属性类型的POC元素，和NAME及TEL子元素。

<ORDER>

<ITEM>

<SN>**132487A-J**</SN>

<NAME>**crank casing**</NAME>

<QTY>**1**</QTY>

</ITEM>

<ITEM>

<SN>**4238764-A**</SN>

<NAME>**bearing**</NAME>

<QTY>**15**</QTY>

</ITEM>

<SHIPMENT>

<POC type="**non-emergency**">

<NAME>**John Smith**</NAME>

<TEL>**555-1234**</TEL>

</POC>

</SHIPMENT>

</ORDER>

**查找元素**

`FindElem` 和 `FindChildElem`方法用于到下一个兄弟元素。如果可选的标签名被指定，那么它们将到下一个与标签名相匹配的元素，被发现的元素是当前元素，并且下次调用Find将会到当前位置后的下一个兄弟或下一个匹配兄弟。

当你无法判断元素的索引时，在调用两个Find方法之间，一定要复位当前位置。看上面的例子中ITEM元素，如果是别的人创建的XML文件，你不能确定SN元素在QTY元素之前，那么在查找QTY元素之前就要调用ResetChildPos();

对于用一个特定的序号去查找元素，你需要完全循环ITEM元素，并比较SN元素的数据和你正在搜索的序号。这个例子不同于先前导航的例子，它调用`IntoElem` 进入到ORDER元素，并且用`FindElem("ITEM")`替换`FindChildElem("ITEM")`;其实两种方式都挺好。需要注意的是，在Find方法中指定ITEM元素的标签名，我们会忽略所有其它的兄弟元素，例如SHIPMENT元素。
`CMarkup xml;``xml.SetDoc( csXML );``xml.FindElem(); // ORDER element is root``xml.IntoElem(); // inside ORDER``while ( xml.FindElem("ITEM") )``{``    xml.FindChildElem( "SN" );``    if ( xml.GetChildData() == csFindSN )``        break; // found``}`
**编码**

ASCII编码引用了我们所依靠的字符码128以下的字符，如用英语编程。如果你只使用ASCII码，很方便，UTF-8编程与你拉公共ASCII集相同。

如果你所使用的字符集不在Unicode编码集(UTF-8，UTF-16，UCS-2)中，那么出于交互性以及在IE中很好的显示，你真的需要在XML声明中进行描述。像ISO-8859-1(西欧)字符集指定字符值在一个比特且在128到255之间。以便每个字符仍然使用一个比特。Windows双字节字符集像GB2312，Shift_JIS和EUC-KR，每个字符都是用一个或两个字节，对于这些Windows字符集，在你的预处理中需要定义_MBCS ，并要确定用户的操作系统设置到合适的编码页。

关于用一个XML描述的XML文档前缀，像`<?xml version="1.0" encoding="ISO-8859-1"?>``，`需要通过用SetDoc或Cmarkup的构造函数来传递。在结尾要包括回车符，这样根结点会显示在下一行。
`xml.SetDoc( "<?xml version=/"1.0/" encoding=/"ISO-8859-1/"?>/r/n" );``xml.AddElem( "island", "Curaçao" );`
**测试对框**

Markup.exe是一Cmarkup的测试程序，这是一个VC6下的MFC工程。当它开始时，工程在OnTest函数里运行诊断程序，在选择了特定的编译选项下测试Cmarkup.,你可以一步一步的调试OnTest函数，会看到怎样用Cmarkup的一些例子，用Open和Parse函数测试一个文件。

在下面的插图里，显示的编译版本是“CMarkup 7.0 Unicode”，这意味着Cmarkup类是用在_UNICODE定义下编译的。成功的运行了RunTest，分析错误在Charsets_err.xml文件中被遇到，文件被加载，文件原是1500比特转换到1033Unicode宽字符(例如2066字节)，但是它遇到了一个结束标签与起始标签不对应的错误 。

![](http://blog.csdn.net/images/blog_csdn_net/aqua_aqua/131802/o_runtest.gif)

这个测试对话框保存着最后一个被分析文件和对话框的屏幕位置信息，这些都被注册表中的HKEY_CURRENT_USER/ Software/ First Objective Software/ Markup/ Settings.键下。

