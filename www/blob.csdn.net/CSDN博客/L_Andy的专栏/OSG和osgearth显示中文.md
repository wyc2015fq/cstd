# OSG和osgearth显示中文 - L_Andy的专栏 - CSDN博客

2015年03月11日 21:01:38[卡哥](https://me.csdn.net/L_Andy)阅读数：4819



做osg相关的项目有一段时间了，一直想写几篇这方面的博文，今天终于开始了。今天主要介绍一下怎么解决osg 和osgearth中显示中文的的问题，这个问题我反复遇到让我很是纠结啊。

**  一、知识储备**

要想很好的理解和解决这个问题，首先要了解什么是多字节和宽字节。说实话我之前也知道这两个字节到底有什么区别，只是简单查了一下资料。这里引用了这篇博客，我感觉博主写的很有意思，通俗易懂，在这里先谢谢这位博主的奉献。[http://blog.163.com/baijianguo00@126/blog/static/1375326052011018101334714/](http://blog.163.com/baijianguo00@126/blog/static/1375326052011018101334714/)

** 二、问题提出**

在大致了解了什么是多字节和款字节之后，我们来看看具体的问题。osg是老外开发的源码，没办法对中文支持很差，虽然这一点儿也不能影响osg带给我们的快感，我们在使用osg中肯定会或多或少要显示中文，但是你会发现你按显示英文那样就做显示的都是乱码。比如我使用LableNode 加一个标记在地球上，如下

labelGroup->addChild( new PlaceNode(mapNode, GeoPoint(geoSRS, 117.5, 39.38), "北京" , pin));

labelGroup->addChild( new PlaceNode(mapNode, GeoPoint(geoSRS, -100.10, 40.60), "U.S.A" , flag));

这时发现北京显示的乱码，而“U.S.A”显示是正确的，还有我们在加载矢量的shp数据时，如果是地名标记数据，显示在地球上也是乱码，还用LableControl显示中文时也是一样的。这里有两种成功的方法，一、改源码；二、转换字符，其实这两种方法本质都是在显示中文时转换字符。改源码一劳永逸但是难度大有风险而且还要重新编译，鉴于此我还是推荐第二种方法-------在程序实时地转换字符。

**  三、解决方法**

中文显示要用宽字节，这里提供几个转换函数。

函数一：

**void unicodeToUTF8(const wstring &src, string& result){int n = WideCharToMultiByte( CP_UTF8, 0, src.c_str(), -1, 0, 0, 0, 0 );result.resize(n);::WideCharToMultiByte( CP_UTF8, 0, src.c_str(), -1, (char*)result.c_str(), result.length(), 0, 0 );}**

**函数二：void gb2312ToUnicode(const string& src, wstring& result){int n = MultiByteToWideChar( CP_ACP, 0, src.c_str(), -1, NULL, 0 );result.resize(n);::MultiByteToWideChar( CP_ACP, 0, src.c_str(), -1, (LPWSTR)result.c_str(), result.length());}当物在osg程序中显示汉字时，就如下调用上述两个函数即可，void gb2312ToUtf8(const string& src, string& result){wstring strWideChar;gb2312ToUnicode(src, strWideChar);unicodeToUTF8(strWideChar, result);}我们拿上面显示的北京标记的做例子，**

**Style pin;**

**pin.getOrCreate<IconSymbol>()->url()->setLiteral(m_PngFilepath);//指定标注图片路径**

**pin.getOrCreate<osgEarth::Symbology::TextSymbol>()->font()=m_FontFilepath;//指定中文字体路径pin.getOrCreate<osgEarth::Symbology::TextSymbol>()->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;pin.getOrCreate<TextSymbol>()->alignment() = TextSymbol::ALIGN_CENTER_CENTER;pin.getOrCreate<TextSymbol>()->fill()->color() = Color::Red;**

std::string _strName;

_strName = "北京";

std::string _strWideName;

gb2312ToUtf8(_strName,_strWideName);//这时的_strWideName就是宽字节用来显示就正确了

labelGroup->addChild( new PlaceNode(mapNode, GeoPoint(geoSRS, 117.5, 39.38), _strWideName , pin));

显示地名标记和LableControl中的中文都是这个方法，先转换在显示。

注明：这些方法都是在看了许多例子和博客的才解决的问题，其实我没什么创新只是整合了一下供大家参考，在这里感谢那些大牛们的无私奉献。


