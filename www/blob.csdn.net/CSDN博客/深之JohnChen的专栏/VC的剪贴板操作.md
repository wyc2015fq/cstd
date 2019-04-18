# VC的剪贴板操作 - 深之JohnChen的专栏 - CSDN博客

2009年02月18日 17:47:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：12554


** VC的剪贴板操作**

1、文本内容的操作
2、WMF数据的操作
3、位图的操作
4、设置使用自定义格式
5、感知剪贴板内容的改变
6、自动将数据粘贴到另一应用程序窗口

一、文本内容的操作
下面的代码示范了如何将文本内容复制到剪贴板(Unicode编码的先转化为ASCII)：

CString source;
//文本内容保存在source变量中
if( OpenClipboard() )
{
HGLOBAL clipbuffer;
char * buffer;
EmptyClipboard();
clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength()+1);
buffer = (char*)GlobalLock(clipbuffer);
strcpy(buffer, LPCSTR(source));
GlobalUnlock(clipbuffer);
SetClipboardData(CF_TEXT,clipbuffer);
CloseClipboard();
}

下面的代码显示了如何从剪贴板上获得文本内容：

char * buffer = NULL;
//打开剪贴板
CString fromClipboard;
if ( OpenClipboard() )
{
HANDLE hData = GetClipboardData(CF_TEXT);
char * buffer = (char*)GlobalLock(hData);
fromClipboard = buffer;
GlobalUnlock(hData);
CloseClipboard();
}

二、WMF数据的操作

　　在剪贴板上读写图象数据是非常有用的功能，并且实现起来也很简单。下面的代码显示了如何将扩展图元文件复制到剪贴板：

if(OpenClipboard());
{
EmptyClipboard();

//创建图元文件DC
CMetaFileDC * cDC = new CMetaFileDC();
cDC->CreateEnhanced(GetDC(),NULL,NULL,"the_name");

//调用绘图例程

//关闭CMetafileDC并获得它的句柄
HENHMETAFILE handle = cDC->CloseEnhanced();

//复制到剪贴板
SetClipBoardData(CF_ENHMETAFILE,handle);
CloseClipboard();

//删除dc
delete cDC;
}

下面的代码演示了从剪贴板获得图元文件并将其绘制到client DC上：

if(OpenClipboard())
{
//获得剪贴板数据
HENMETAFILE handle = (HENMETAFILE)GetClipboardData(CF_ENHMETAFILE);

//显示
CClientDC dc(this);
CRect client(0,0,200,200);
dc.PlayMetaFile(handle,client);

//关闭剪贴板
CloseClipboard();
}
三、位图的操作

位图的操作稍微复杂一点，下面这个例子显示了如何在剪贴板保存位图：

if(OpenClipboard())
{
EmptyClipboard();
CBitmap * junk = new CBitmap();
CClientDC cdc(this);
CDC dc;
dc.CreateCompatibleDC(&cdc);
CRect client(0,0,200,200);
junk->CreateCompatibleBitmap(&cdc,client.Width(),client.Height());
dc.SelectObject(junk);

DrawImage(&dc,CString("Bitmap"));

//复制数据到剪贴板
SetClipboardData(CF_BITMAP,junk->m_hObject);
CloseClipboard();

delete junk;
}

下面的代码显示了如何从剪贴板上获得位图数据：

if(OpenClipboard())
{
//获得剪贴板数据
HBITMAP handle = (HBITMAP)GetClipboardData(CF_BITMAP);
CBitmap * bm = CBitmap::FromHandle(handle);

CClientDC cdc(this);
CDC dc;
dc.CreateCompatibleDC(&cdc);
dc.SelectObject(bm);
cdc.BitBlt(0,0,200,200,&dc,0,0,SRCCOPY);

CloseClipboard();
}

四、设置并使用自定义格式

使用RegisterClipboardFormat()函数，可以复制和粘贴任何你需要的数据类型。比如我们有以下一个数据类型：

struct MyFormatData
{
long val1;
int val2;
};

我们要把它复制到剪贴板，可以使用如下的代码：

UINT format = RegisterClipBoardFormat("MY_CUSTOM_FORMAT");
if(OpenClipboard())
{
MyFormatData data;
data.val1 = 100;
data.val2 = 200;

HGLOBAL clipbuffer;
EmptyClipboard();
clipbuffer = GlobalAlloc(GMEM_DDESHARE, sizeof(MyFormatData));
MyFormatData * buffer = (MyFormatData*)GlobalLock(clipbuffer);

//保存到内存
*buffer = data;

//保存到剪贴板
GlobalUnlock(clipbuffer);
SetClipboardData(format,clipbuffer);
CloseClipboard();
}

读取数据使用以下代码：

UINT format = RegisterClipboardFormat("MY_CUSTOM_FORMAT");
MyFormatData data;
if(Openclipboard())
{
HANDLE hData =GetClipboardData(format);
MyFormatData * buffer = (MyFormatData*)GlobalLock(hData);

data = *buffer;

GlobalUnlock(hData);
CloseClipboard();
}

五、感知剪贴板内容的改变

通过Windows消息可以感知剪贴板内容是否发生改变，代码如下：

//In your initialization code call:

SetClipboardViewer(); //将我们的程序添加到剪贴板观察链

//In your message map add:
ON_MESSAGE(WM_DRAWCLIPBOARD, OnClipChange) //添加Message handle

//Which is declared as:

afx_msg void OnClipChange();

Finally implement:
void CDetectClipboardChangeDlg::OnClipChange() 
{
CTime time = CTime::GetCurrentTime();
SetDlgItemText(IDC_CHANGED_DATE,time.Format("%a, %b %d, %Y -- %H:%M:%S"));

DisplayClipboardText();
}

六、自动将数据粘贴到另一应用程序窗口

只需获得相应窗口的句柄，并发送一个消息就可以了：

SendMessage(m_hTextWnd, WM_PASTE, 0, 0); 

传统Windows剪贴板编程

将数据复制到剪贴板:
1.调用OpenClipboard()设置数据的源窗口.
2.调用EmptyClipboard()清空剪贴板中以前的数据.
3.调用SetClipboardData()将数据存放到剪贴板上.
4.调用CloseClipboard()使别的窗口能访问剪贴板.
取得剪贴板上的数据:
1.调用OpenClipboard()访问剪贴板.
2.调用GetClipboardData()取得数据.
3.调用CloseClipboard()释放剪贴板.

使用延迟供应技术时,源数据方以NULL为数据句柄调用SetClipboardData(),数据使用方GetClipboardData()时,Windows向数据产生者发送WM_RENDERFORMAT和WM_RENDERFORMATS消息,数据提供者响应消息并产生数据.

局限性:使用全局内存来传输,数据量大时系统要使用虚拟内存管理机制来管理,对交换效率有很大影响.

//------------------------------------------------------------
OLE剪贴板

介于应用程序与标准剪贴板间,从标准剪贴板扩展而来,补充了OLE的数据传输机制,对标准剪贴板向后兼容.
OLE剪贴板使用IDataObject接口进行传输,相关的API:
.OleSetClipboard():在剪贴板上放置一个IDataObject接口指针.
.OleGetClipboard():从剪贴板上取得一个IDataObject接口指针.
.OleFlushClipboard():清空OLE剪贴板,释放上面的IDataObject接口指针.
.OleIsCurrentClipboard():判断指定的对象当前是否在剪贴板上.
OLE剪贴板的工作步骤:
1.数据创造者程序将数据放到剪贴板并实现IDataObject.数据创造者用OleSetClipboard()得IDataObject的一个拷贝.并将其放到剪贴板上.
2.剪贴板上有IDataObject指针时,OLE像普通应用程序一样使用剪贴板.OLE调用OpenClipboard来声明剪贴板的拥有者,OLE剪贴板使用延迟供应模式.OLE剪贴板会创建一个隐藏窗口作为剪贴板的拥有者(OpenClipboard需要HWND参数)--在OleInitialize()中创建.
3.OLE枚举IDataObject的格式,同时对每个在全局句柄中提供数据的格式调用SetClipboard().标准剪贴板不支持文件和结构的传输入,所以只能将全局句柄放在剪贴板上.
4.数据消耗者访问剪贴板.当它不知道OLE的信息时,使用标准方式GetClipboardData()来获取数据--数据是由延迟供应方式提供的.OLE剪贴板查询IDataObject接口,然后调用接口上的GetData()方法取得数据.如数据消耗者支持OLE,它可用OleGetClipboard()取得IDataObject指针,并用GetData()取得数据.

MFC的IDataObject支持

.COleDataSource:一个完全的COM对象,实现了IDataObject接口.常用在数据提供者一方.
.COleDataObject:封装一个IDataObject指针,为开发者提供C++接口.常用在数据消耗者一方.

A.通过剪贴板传输数据

1.将数据放置到剪贴板中.
得到数据指针并创建COleDataSource的一个实例,用这个对象来保存数据.例:
{
LPCTSTR source = GetString() ;
COleDataSource *pCods;
HGLOBAL h = GlobalAlloc(GHND|GMEM_SHARE,(_tcslen(source)+1)*sizeof(TCHAR));
_tcscpy( LPSTR(GloballLock(h)), source ) ;
GlobalUnlock(h);//使用全局内存时这句一定要有.
pCods->CacheGlobalData(CF_TEXT,h) ;
pCods->SetClipboard() ;
//没有释放全局内存?在将数据放到OLE剪贴板的情况下,OLE自己会去释放它.
//不想使用全局内存?那用COleDataSource::CacheData()放入数据源对象,但需要提供两个描绘数据的参数.
}
2.从剪贴板中粘贴数据
声明一个COleDataObject的实例,调用COleDataObject::AttachClipboard().可用ColeDataObject::IsDataAvailable()获取数据格式;但也支持COleDataObject::BeginEnumFormats()/COleDataObject::GetNextFormat()枚举所有格式.
确定格式后,用COleDataObject的以下函数可得到数据:
.COleDataObject::GetDataObject();//最通用的,处理所有格式.
.COleDataObject::GetFileData();//数据的存储介质是文件时;文件由使用者释放.
.COldDataObject::GetGlobalData();//数据在全局内存.
例:
{
COleDataObject odo ;
odo.AttachClipboard() ;
if( odo.IsDataAvailable(CT_TEXT)){
HANDLE h = odo.GetGlobalData(CF_TEXT);
//use data
GlobalUnlock(h) ;
GlobalFree(h) ;
}
odo.Release() ;
}
B.延迟供应.
由COldDataSource中的DelayRenderData(),DelayRenderFileData(),OnRenderData()实现.需要在COleDataSource派生类中实现OnRenderData().类似的函数还有OnRenderFileData,OnRenderGlobalData().

//-----------------------------------------------------------

OLE拖放

使用3个接口:IDataObject,IDropSource(COleDropSource实现),IDropTarget(COleDropTarget实现).

控制键:[没有]=移动数据;[CONTROL]=复制数据;[CONTROL-SHIFT]=建立快捷方式;[ALT]=移动数据

开起拖放:与将数据放在剪贴板相似,不过是调用COldDataSource:oDragDrop()而不是COleDataSource::SetClipboard();
结束播放:放置目标通常需要两个COleDropTarget与CView类
1.注册CView为一个COleDropTarget类.在类中声明一个COleDropTarget变量,在OnCreate()中调用COleDropTarget::Register(this);
2.重载CView类的OnDragEnter(),OnDragOver(),OnDragLeave(),OnDrop();

