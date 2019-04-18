# COM 组件设计与应用（一） - ljx0305的专栏 - CSDN博客
2009年02月05日 17:20:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：896标签：[存储																[磁盘																[excel																[微软																[microsoft																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=微软&t=blog)](https://so.csdn.net/so/search/s.do?q=excel&t=blog)](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)
个人分类：[COM组件](https://blog.csdn.net/ljx0305/article/category/511030)
**COM 组件设计与应用（一）起源及复合文件**
作者：[杨老师](mailto:good_yf@sina.com)
**一、前言**
　　公元一九九五年某个夜黑风高的晚上，我的一位老师跟我说：“小杨呀，以后写程序就和搭积木一样啦。你赶快学习一些OLE的技术吧......”，当时我心里就寻思 ：“开什么玩笑？搭积木方式写程序？再过100年吧......”，但作为一名听话的好学生，我开始在书店里“踅摸”（注1）有关OLE的书籍（注2）。功夫不负有心人，终于买到了我的第一本COM书《OLE2 高级编程技术》，这本800多页的大布头花费了我1/5的月工资呀......于是开始日夜耕读.....
功夫不负有心人，我坚持读完了全部著作，感想是：这本书，在说什么呐？
功夫不负有心人，我又读完了一遍大布头，感想是：咳~~~，没懂！
功夫不负有心人，我再，我再,我再读 ... 感想是：哦~~~，读懂了一点点啦，哈哈哈。
...... ......
功夫不负有心人，我终于，我终于懂了。
800页的书对现在的我来说，其实也就10几页有用。到这时候才体会出什么叫“书越读越薄”的道理了。到后来，能买到的书也多了，上网也更方便更便宜了......
　　为了让VCKBASE上的朋友，不再经历我曾经的痛苦、不再重蹈我“无头苍蝇”般探索的艰辛、为了VCKBASE的蓬勃发展、为了中国软件事业的腾飞（糟糕，吹的太也高了）......我打算节约一些在 BBS 上赚分的时间，写个系列论文，就叫“COM组件设计与应用”吧。今天是第一部分——起源。
**二、文件的存储**
　　传说350年前，牛顿被苹果砸到了头，于是发现了万有引力。但到了二十一世纪的现在，任何一个技术的发明和发展，已经不再依靠圣人灵光的一闪。技术的进步转而是被社会的需求、商业的利益、竞争的压力、行业的渗透等推动的。微软在Windows平台上的组件技术也不例外，它的发明，有其必然因素。什么是这个因素那？答案是——文件的存储。
　　打开记事本程序，输入了一篇文章后，保存。——这样的文件叫“非结构化文件”；
　　打开电子表格程序，输入一个班的学生姓名和考试成绩，保存。——这样的文件叫“标准结构化文件”；
　　在我们写的程序中，需要把特定的数据按照一定的结构和顺序写到文件中保存。——这样的文件叫“自定义结构化文件”；（比如 *.bmp 文件）
　　以上三种类型的文件，大家都见的多了。那么文件存储就依靠上述的方式能满足所有的应用需求吗？恩~~~，至少从计算机发明后的50多年来，一直是够用的了。嘿嘿，下面看看商业利益的推动作用，对文件 的存储形式产生了什么变化吧。30岁以上的朋友，我估计以前都使用过以下几个著名的软件：WordStar（独霸DOS下的英文编辑软件），WPS（裘伯君写的中文编辑软件，据说当年的市场占有率高达90%，各种计算机培训班的必修课程），LOTUS-123（莲花公司出品的电子表格软件）......
微软在成功地推出 Windows 3.1 后，开始垂涎桌面办公自动化软件领域。微软的 OFFICE 开发部门，各小组分别独立地开发了 WORD 和 EXCEL 等软件，并采用“自定义结构”方式，对文件进行存储。在激烈的市场竞争下，为了打败竞争对手，微软自然地产生了一个念头------如果我能在 WORD 程序中嵌入 EXCEL，那么用户在购买了我 WORD 软件的情况下，不就没有必要再买 LOTUS-123 了吗？！“恶毒”（中国微软的同志们看到了这个词，不要激动，我是加了引号的呀）的计划产生后，他们开始了实施工作，这就是 COM 的前身 OLE 的起源（注3）。但立刻就遇到了一个严重的技术问题：需要把 WORD 产生的 DOC 文件和 EXCEL 产生的 XLS 文件保存在一起。
|方案|优点|缺点|
|----|----|----|
|建立一个子目录，把 DOC、XLS 存储在这同一个子目录中。|数据隔离性好，WORD 不用了解 EXCEL 的存储结构；容易扩展。|结构太松散，容易造成数据的损坏或丢失。不易携带。|
|修改文件存储结构，在DOC结构基础上扩展出包容 XLS 的结构。|结构紧密，容易携带和统一管理。|WORD 的开发人员需要通晓 EXCEL 的存储格式；缺少扩展性，总不能新加一个类型就扩展一下结构吧？！|
以上两个方案，都有严重的缺陷，怎么解决那？如果能有一个新方案，能够合并前两个方案的优点，消灭缺点，该多好呀......微软是作磁盘操作系统起家的，于是很自然地他们提出了一个非常完美的设计方案，那就是把磁盘文件的管理方式移植到文件中了------复合文件，俗称“文件中的文件系统”。连微软当年都没有想到，就这么一个简单的想法，居然最后就演变出了 COM 组件程序设计的方法。可以说，复合文件是 COM 的基石。下图是磁盘文件组织方式与复合文件组织方式的类比图：
![](http://writeblog.csdn.net/document/journal/vckbase43/images/stmtutpic1.jpg)
图一、左侧表示一个磁盘下的文件组织方式，右侧表示一个复合文件内部的数据组织方式。
**三、复合文件的特点**
- 复合文件的内部是使用指针构造的一棵树进行管理的。编写程序的时候要注意，由于使用的是单向指针，因此当做定位操作的时候，向后定位比向前定位要快； 
- 复合文件中的“流对象”，是真正保存数据的空间。它的存储单位为512字节。也就是说，即使你在流中只保存了一个字节的数据，它也要占据512字节的文件空间。啊~~~，这也太浪费了呀？不浪费！因为文件保存在磁盘上，即使一个字节也还要占用一个“簇”的空间那； 
- 不同的进程，或同一个进程的不同线程可以同时访问一个复合文件的不同部分而互不干扰； 
- 大家都有这样的体会，当需要往一个文件中插入一个字节的话，需要对整个文件进行操作，非常烦琐并且效率低下。而复合文件则提供了非常方便的“增量访问”能力； 
- 当频繁地删除文件，复制文件后，磁盘空间会变的很零碎，需要使用磁盘整理工具进行重新整合。和磁盘管理非常相似，复合文件也会产生这个问题，在适当的时候也需要整理，但比较简单，只要调用一个函数就可以完成了。 
**四、浏览复合文件**
　　VC6.0 附带了一个工具软件“复合文件浏览器”，文件名是“vc目录/Common/Tools/DFView.exe”。为了方便使用该程序，可以把它加到工具(tools)菜单中。方法是：Tools/Customize.../Tools卡片中增加新的项目。运行 DFView.exe，就可以打开一个复合文件进行观察了（注4）。但奇怪的是，在 Microsoft Visual Studio .NET 2003 中，我反而找不到这个工具程序了,汗！不过这恰好提供给大家一个练习的机会，在你阅读完本篇文章并掌握了编程方法后，自己写一个“复合文件浏览编辑器”程序，又练手了，还有实用的价值。
**五****、复合文件函数**
　　复合文件的函数和磁盘目录文件的操作非常类似。所有这些函数，被分为3种类型：WIN API 全局函数，存储 IStorage 接口函数，流 IStream 接口函数。什么是接口？什么是接口函数？以后的文章中再陆续介绍，这里大家只要把“接口”看成是完成一组相关操作功能的函数集合就可以了。
|**WIN API 函数**|**功能说明**|
|----|----|
|StgCreateDocfile()|建立一个复合文件，得到根存储对象|
|StgOpenStorage()|打开一个复合文件，得到根存储对象|
|StgIsStorageFile()|判断一个文件是否是复合文件|
|| |
|**IStorage 函数**|**功能说明**|
|CreateStorage()|在当前存储中建立新存储，得到子存储对象|
|CreateStream()|在当前存储中建立新流，得到流对象|
|OpenStorage()|打开子存储，得到子存储对象|
|OpenStream()|打开流，得到流对象|
|CopyTo()|复制存储下的所有对象到目标存储中，该函数可以实现“整理文件，释放碎片空间”的功能|
|MoveElementTo()|移动对象到目标存储中|
|DestoryElement()|删除对象|
|RenameElement()|重命名对象|
|EnumElements()|枚举当前存储中所有的对象|
|SetElementTimes()|修改对象的时间|
|SetClass()|在当前存储中建立一个特殊的流对象，用来保存CLSID（注5）|
|Stat()|取得当前存储中的系统信息|
|Release()|关闭存储对象|
|| |
|**IStream 函数**|**功能说明**|
|Read()|从流中读取数据|
|Write()|向流中写入数据|
|Seek()|定位读写位置|
|SetSize()|设置流尺寸。如果预先知道大小，那么先调用这个函数，可以提高性能|
|CopyTo()|复制流数据到另一个流对象中|
|Stat()|取得当前流中的系统信息|
|Clone()|克隆一个流对象，方便程序中的不同模块操作同一个流对象|
|Release()|关闭流对象|
|| |
|**WIN API 补充函数**|**功能说明**|
|WriteClassStg()|写CLSID到存储中，同IStorage::SetClass()|
|ReadClassStg()|读出WriteClassStg()写入的CLSID，相当于简化调用IStorage::Stat()|
|WriteClassStm()|写CLSID到流的开始位置|
|ReadClassStm()|读出WriteClassStm()写入的CLSID|
|WriteFmtUserTypeStg()|写入用户指定的剪贴板格式和名称到存储中|
|ReadFmtUserTypeStg()|读出WriteFmtUserTypeStg()写入的信息。方便应用程序快速判断是否是它需要的格式数据。|
|CreateStreamOnHGlobal()|内存句柄 HGLOBAL 转换为流对象|
|GetHGlobalFromStream()|取得CreateStreamOnHGlobal()调用中使用的内存句柄|
为了让大家快速地浏览和掌握基本方法，上面所列表的函数并不是全部，我省略了“事务”函数和未实现函数部分。更全面的介绍，请阅读 MSDN。
下面程序片段，演示了一些基本函数功能和调用方法。 
示例一：建立一个复合文件，并在其下建立一个子存储，在该子存储中再建立一个流，写入数据。
void SampleCreateDoc()
{
	::CoInitialize(NULL);	// COM 初始化
				// 如果是MFC程序，可以使用AfxOleInit()替代
	HRESULT hr;		// 函数执行返回值
	IStorage *pStg = NULL;	// 根存储接口指针
	IStorage *pSub = NULL;	// 子存储接口指针
	IStream *pStm = NULL;	// 流接口指针
	hr = ::StgCreateDocfile(	// 建立复合文件
		L"c://a.stg",	// 文件名称
		STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE,	// 打开方式
		0,		// 保留参数
		&pStg);		// 取得根存储接口指针
	ASSERT( SUCCEEDED(hr) );	// 为了突出重点，简化程序结构，所以使用了断言。
				// 在实际的程序中则要使用条件判断和异常处理
	hr = pStg->CreateStorage(	// 建立子存储
		L"SubStg",	// 子存储名称
		STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE,
		0,0,
		&pSub);		// 取得子存储接口指针
	ASSERT( SUCCEEDED(hr) );
	hr = pSub->CreateStream(	// 建立流
		L"Stm",		// 流名称
		STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE,
		0,0,
		&pStm);		// 取得流接口指针
	ASSERT( SUCCEEDED(hr) );
	hr = pStm->Write(		// 向流中写入数据
		"Hello",		// 数据地址
		5,		// 字节长度(注意，没有写入字符串结尾的/0)
		NULL);		// 不需要得到实际写入的字节长度
	ASSERT( SUCCEEDED(hr) );
	if( pStm )	pStm->Release();// 释放流指针
	if( pSub )	pSub->Release();// 释放子存储指针
	if( pStg )	pStg->Release();// 释放根存储指针
	::CoUninitialize()		// COM 释放
				// 如果使用 AfxOleInit(),则不调用该函数
}
![](http://writeblog.csdn.net/document/journal/vckbase43/images/stmtutpic2.jpg)
图二、运行示例程序一后，使用 DFView.exe 打开观察复合文件的效果图
示例二：打开一个复合文件，枚举其根存储下的所有对象。
#include <atlconv.h>	// ANSI、MBCS、UNICODE 转换
void SampleEnum() 
{	// 假设你已经做过 COM 初始化了
	LPCTSTR lpFileName = _T( "c://a.stg" );
	HRESULT hr;
	IStorage *pStg = NULL;
	
	USES_CONVERSION;				// （注6）
	LPCOLESTR lpwFileName = T2COLE( lpFileName );	// 转换T类型为宽字符
	hr = ::StgIsStorageFile( lpwFileName );	// 是复合文件吗？
	if( FAILED(hr) )	return;
	hr = ::StgOpenStorage(			// 打开复合文件
		lpwFileName,			// 文件名称
		NULL,
		STGM_READ | STGM_SHARE_DENY_WRITE,
		0,
		0,
		&pStg);				// 得到根存储接口指针
	IEnumSTATSTG *pEnum=NULL;	// 枚举器
	hr = pStg->EnumElements( 0, NULL, 0, &pEnum );
	ASSERT( SUCCEEDED(hr) );
	STATSTG statstg;
	while( NOERROR == pEnum->Next( 1, &statstg, NULL) )
	{
		// statstg.type 保存着对象类型 STGTY_STREAM 或 STGTY_STORAGE
		// statstg.pwcsName 保存着对象名称
		// ...... 还有时间，长度等很多信息。请查看 MSDN
		::CoTaskMemFree( statstg.pwcsName );	// 释放名称所使用的内存（注6）
	}
	
	if( pEnum )	pEnum->Release();
	if( pStg )	pStg->Release();
}
**六、小结**
　　复合文件，结构化存储，是微软组件思想的起源，在此基础上继续发展出了持续性、命名、ActiveX、对象嵌入、现场激活......一系列的新技术、新概念。因此理解和掌握 复合文件是非常重要的，即使在你的程序中并没有全面使用组件技术，复合文件技术也是可以单独被应用的。祝大家学习快乐，为社会主义软件事业而奋斗:-)
留作业啦......
作业1：写个小应用程序，从 MSWORD 的 doc 文件中，提取出附加信息（作者、公司......）。
作业2：写个全功能的“复合文件浏览编辑器”。
注1：踅摸(xuemo)，动词，北方方言，寻找搜索的意思。
注2：问：为什么不上网查资料学习？
答：开什么国际玩笑！在那遥远的1995年代，我的500块工资，不吃不喝正好够上100小时的Internet网。
注3：OLE，对象的连接与嵌入。
注4：可以用 DFView.exe 打开 MSWORD 的 DOC 文件进行复合文件的浏览。但是该程序并没有实现国际化，不能打开中文文件名的复合文件，因此需要改名后才能浏览。
注5：CLSID，在后续的文章中介绍。
注6：关于 COM 中内存使用的问题，在后续的文章中介绍。
第一个例子调试成功:)
需要建立一个win32 控制台应用程序并且包含头文件
#define _WIN32_WINNT 0x0500
#include <ole2.h>
#include <assert.h>
在第二个实例中需要加头文件
#include <atlbase.h>  
#include <atlconv.h>
引用于:http://www.vckbase.com/document/viewdoc/?id=1483
