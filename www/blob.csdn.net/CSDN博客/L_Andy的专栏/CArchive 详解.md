# CArchive 详解 - L_Andy的专栏 - CSDN博客

2014年06月11日 18:24:36[卡哥](https://me.csdn.net/L_Andy)阅读数：955



CArchive没有基类。 

CArchive允许以一个永久二进制（通常为磁盘存储）的形式保存一个对象的复杂网络，它可以在对象被删除时，还能永久保存。可以从永久存储中装载对象，在内存中重新构造它们。使得数据永久保留的过程就叫作“串行化”。 

可以把一个归档对象看作一种二进制流。象输入/输出流一样，归档与文件有关并允许写缓冲区以及从硬盘读出或读入数据。输入/输出流处理一系列ASCII字符，但是归档文件以一种有效率、精练的格式处理二进制对象。 

必须在创建一个CArchive对象之前，创建一个CFile对象。另外，必须确信归档文件的装入/存储与文件的打开模式是兼容的。每一个文件只限于一个活动归档文件。 

当构造一个CArchive对象时，要把它附加给表示一个打开文件的类CFile（或派生类）的对象上。还要指定归档文件将用于装载还是存储。CArchive对象不仅可以处理主要类型，而且还能处理为串行化而设计的CObject_派生类的对象。一个串行化类通常有一个Serialize成员函数并且使用DECLARE_SERIAL和IMPLEMENT_SERIAL宏。这些在CObject类中有所描述。 

重载提取（>>）和插入（<<）是方便的归档编程接口。它支持主要类型和CObject派生类。 

CArchive还支持使用MFC Windows套接字类CSocket和CSocketFile编程。IsBufferEmpty成员函数也支持这种使用。如果要了解有关CArchive的更多信息，请参阅联机文档“Visual C++ 程序员指南”中的“串行化（永久对象）” 和“Windows套接字:在归档文件中使用套接字” 

include<afx.h> 

请参阅   CFile,CObject,CSocket,CSocketFile 

CArchive类的成员 

数据成员 

m_pDocument 指向被串行化的CDocument对象 

构造函数 

Carchive 创建一个Carhcive对象 

Abort在不异常的情况下，关闭归档文件 

Close冲掉未写入数据并且释放与CFile的连接 

基础输入/输出 

Flush从归档文件缓冲区中冲掉未写入数据 

运算符>>装载对象和归档文件的主要类型 

运算符<<存储对象和归档文件的主要类型 

Read读入原始类型 

Write写入原始类型 

WriteString写一行文本 

ReadString读一行文本 

状态 

GetFile获得此归档文件的 

CFile对象指针 

GetObjectSchema由Serialize函数调用来确定被非串行化的对象的版本SetObjectSchema在归档文件中存储对象概要 

IsLoading确定归档文件是否被装载 

IsStoring确定归档文件是否被存储 

IsBufferEmpty确定在一个Windows Socket接收过程中缓冲区是否被清空 

对象输入/输出 

ReadObject调用一个用于装载的Serialize函数 

WriteObect调用一个用于装载的Serialize函数 

MapObject在没有对文件串行化的映射中放置对象，但是此映射对参考的子对象有效 

SetStoreParams设置哈希表的大小和映射的块的大小，在串行化的过程中识别唯一的对象 

LoadParams设置装载数组扩展的大小。必须在被装载对象之前或调用MapObject或ReadObject之前 

ReadClass读入一个原先存储在WriteClass中的类的参考 

WriteClass把对CRuntime的参考写入Carchive 

SerializeClass根据CArchive方向，读入或写入对CArchive对象的类的参考 

成员函数 

CArchive::Abort 

void Abort( ); 

说明 

调用此函数在不异常的情况下关闭归档文件。CArchive析构程序将调用Close，它将冲掉任何没有被存储在相关CFile对象中的数据。这会引起异常。 

当获取这些异常时，有一个好的方法就是使用Abort，这样析构CArchive 

对象就不会再引起异常。当处理异常时，在失败时CArchive::Abort将不会异常，因为与CArchive::Clsoe不同，Abort忽略失败。 

如果使用new在堆上分配CArchive对象，则在关闭文件之后，必须删除它。 

请参阅 

CArchive::Close, 

Cfile::Close 

CArchive::CArchive 

CArchive(CFile*pFile,UINTnMode,int nBufSize=4096,void*lpBuf=NULL ); 

throw(CmemoryException,CArchiveException,CFileException); 

参数 

pFileCFile 

对象的指针。CFile对象是永久数据的最终的源或目标。 

nMode标识。它指定了对象是否从归档文件中装载或存储到文件中去。 

nMode参数必须有下列值之一：l CArchive::load   从归档文件装载数据。CFile只读。l CArchive::store   把数据保存到归档文件中。允许CFile写操作。l CArchive::bNoFlushOnDelete　当归档文件析构程序被调用时，防止归档文件自动调用Flush。如果设定了此标识，则在析构程序被调用之前必须负责调用Close。如果不这样做，数据就会崩溃。 

nBufSize 

指定内部文件缓冲区大小的整数，以字节计算。注意缺省的缓冲区 

大小为4096字节。如果例程归档大的对象，使用大一些的缓冲区，即多个文件缓冲区，那么将会提高例程的执行效率。 

lpBuf 

指向nBufSize大小的提供缓冲区的指针。如果不指定这个参数，归档文 件从本地堆为归档文件分配一个缓冲区并且当对象被毁弃时，释放缓冲 区。归档文件不能释放一个提供的缓冲区。 

说明 

构造CArchive对象并且指定它将用于装载或存储对象。在创建归档文件之后，不能改变这个指定内容。不能使用CFile操作来改变文件的状态直到已经关闭归档文件时。任何这样的操作都将会毁弃归档文件的完整性。通过由GetFile成员函数获得归档文件的文件对象使得可在串行化过程中的任何时候访问文件指针的位置。然后使用Cfile::GetPosition函数。应该在获得文件指针位置之前，调用CArchive::Flush。 

示例 

extern char* pFileName 

CFile f; 

char buf[512]; 

if(!f.Open( pFileName,Cfile::modeCreate| Cfile::modeWrite)){   

#ifdef_DEBUG       

afxDump<< “unable to open file”<<”\n”; 

     exit(1);   

#endif 

} 

CArchive ar( &f, Cachive::strore,512,buf); 

请参阅   CArchive::Close, 

CArchive::Flush, 

Cfile::Close 

CArchive::Close 

void Close( ) 

throw( CArchiveException,CFileException); 

说明 

冲掉保存在缓冲区中的任何数据，关闭归档文件并且释放归档文件与文件的链接。对于归档文件没有允许的其它操作。在关闭一个归档文件之后，可以为一个同样文件创建另一个归档文件或者关闭文件。成员函数Close保证所有数据从归档文件传输到文件并且使归档文件无效。为了完成从文件到存储介质的传输，必须首先使用Cfile::Close并且再毁弃CFile对象。 

请参阅   CArchive::Flush, 

CArchive::Abort 

CArchive::Flush 

void Flush( );   

throw( CFileException); 

说明 

迫使保留在归档文件中的数据写入文件。成员函数Flush保证所有的数据从归档文件传输到文件。必须调用Cfile::Close来完成从文件到存储介质的传输。 

请参阅   CArchive::Close, 

Cfile::Flush,Cfile::Close 

CArchive::GetFile 

CFile*GetFile( ) const 

返回值 

指向正在使用的CFile对象的指针。 

说明 

为此归档文件获得CFile对象指针。必须在使用GetFile之前冲掉归档文件。 

示例 

extern CArchive ar; 

const CFile*fp=ar.GetFile( ); 

请参阅   

CArchive::Flush 

CArchive::GetObjectSchemaUINT 

GetObjectSchema( ) 

返回值 

返回在非串行化过程中，被读入对象的版本。 

说明 

由Serialize函数调用此函数，确定当前被非串行化的对象的版本。当CArchive对象正在被装载时，调用此函数才是有效的（CArchive::IsLoading则返回非零值）。它必须是Serialize函数的第一次调用而且只能调用一次。返回值-1（UINT）说明版本数未知。CObject派生类可以使用与大纲版本自己（在IMPLEMENT_SERIALH宏里）结合起来使用的VERSIONABLE_SEHEMA，创建一个“版本化对象”，即一个对象它的成员函数可以读入多个版本。当版本不匹配时，缺省的框架功能（不带有VERSIONABLE_SHEMA）将异常信号。 

示例 

IMPLEMENT_SERIAL（CMyObject,CObject,VERSIOANABLE_SCHEMA|1） 

void CMyObject::Serialize(CArchive& ar) 

{   

if (ar.Isloading( ))   

{ 

int nVersion=ar.GetObjectSchma( );     

switch(nVersion)     

{     

case 0: 

        //read in previous version of       / 

/this object       

break;   

case 1:       

//read in current version of       

//this object       

break;   

default:     

//report unknown version of     

//this object       

break;     

}   

}   

else   

{     

//Normal soring code goes here   

} 

} 

请参阅   CObject::Serialize, 

CObject::IsSerializable, 

IMPLEMNET_SERIAL,DECLARE_SERIAL, 

CArchive::IsLoading 

CArchive::IsBufferEmpty 

BOOL IsBufferEmpty( ) const 

返回值 

如果归档文件的缓冲为空，则返回非零值，否则为0。 

说明 

调用此函数，确定归档文件的缓冲区是否为空。此函数支持使用Windows套接字类CSocketFile编程。对与CFile对象有关的归档文件则不需要使用它。 

与CSocketFile对象相关的归档文件使用IsBufferEmpty的原因是归档文件的缓冲区可能包含多个消息或记录。在接收一个消息时，应该使用IsBufferEmpty控制这样一个循环，它不断地接收数据，直到缓冲区为空。如果要了解更多的内容，请参阅类CAsyncSocket的Rceive成员函数以及MFC高级概念样例CHATSRVR，它将告诉你如何使用IsBufferEmpty。 

如果要了解更多的内容，请参阅联机文档“Visual C++程序员指南”中的“Windows套接字：在归档文件中使用套接字”。 

请参阅   CSocketFile, 

CAsyncSocket::Receive 

CArchive::IsLoading 

BOOL IsLoading( ) const 

返回值 

如果正在装载归档文件，则返回非零值，否则为0。 

说明 

确定归档文件是否正在装载数据。此成员函数由归档文件类的Serialize函数调用。 

示例 

int i; 

extern CArchive ar; 

if (ar.Isloading( ))   

ar>>i;else   

ar << i; 

请参阅   CArchive::IsStoring 

CArchive::IsStoring 

BOOL IsStorng( ) const; 

返回值 

如果正在存储归档文件，则返回非零值，否则为0。 

说明 

确定是否正在存储归档文件。这个函数由归档文件类的Serialize函数调用。如果归档文件的IsStoring状态为非零，则它的IsLoading状态为0，反 

之亦然。 

示例 

int i; 

extern CArchive ar; 

if( ar.IsStoring( ))   

ar<<i;else   ar>>i; 

请参阅   CArchive::IsLoading 

CArchive::MapObject 

void MapObject(const CObject*pOb); 

参数 

pOb指向正在存储的对象的指针。 

说明 

调用此成员函数在映射中放置对象。此映射没有真的对文件串行化，但是对参考的子对象有效。举例来说，可能不想串行化一个文档，但是想对作为文档一部分的项串行化。通过调用MapObject，可允许那些项或子对象参考文档。而且，串行化子项还可以串行化它们的m_pDocument向后的指针。 

当要把数据存储到CArchive对象和从它装载数据时，可以调用MapObject。在串行化和非串行化的过程中，MapObject向由CArchive保持的内部数据结构添加特定的对象，但是它不象ReadObject和WriteObject那样，它不会对对象串行化。 

示例 

//MyDoc.h 

//Document should have DECLEAR_SERIAL and IMPLEMENT_SERIAL 

class CMyDocument: public CDocument 

{   

CObList m_listOfSubItems;   

…… 

DECLARE_SERIAL(CMyDocument) 

}; 

//MyDoc.cpp 

……. 

MPLEMENT_SERIAL(CMyDocument.CObject,1) 

……. 

void CMyDocument::Serialize(CArchive& ar) 

{   

if (ar.IsStoring( ))   

{     

//TODO:add storing code here   

}   

else 

{       

//TODO:add loading code here   

}   

ar.MapObject(this);   

//serialize the subitems in the documents;   

//they will be able to serialize their m_pDoc   

//back 

pointer   m_listOfSubItems.Serialize(ar); 

} 

//SubItem.hclass 

CSubItem:public CObject 

{ 

public   CSubItem(CMYDocument * Pdoc)       

{m_pDoc=pDoc}   

//back pointer to owning document   

CMYDocument* m_pDoc   

WORD m_I; 

//other item data 

   virtual void Serialize(CArchive& ar); 

}; 

//SubItem.cpp 

void CSubItem:Serialize(CArchive& ar) 

{   

if (ar.IsStoring( )) 

{     //will serializing a reference     

//to the “mapped”document pointer     

ar <<m_pDoc;     

ar<<m_i;   

}   

else   

{     

//will load a reference to     

//the the “mapped”document pointer 

ar >>m_pDoc;     

ar >>m_i;   

} 

} 

请参阅   CArchive::ReadObject,CArchive::WriteObject 

CArchive::Read 

UINT Read(void*lpBuf,UINT nMax);   

throw(CFileException); 

返回值 

包含实际读入字节数的无符号整数。如果返回值小于所需要的数值，则说明已达到了文件的末尾。达到文件末尾时，没有异常。 

参数 

lpBuf指向提供的缓冲区的指针。这个提供的缓冲区接收从归档文件读取的数据。 

nMax标明从归档文件读取的字节数的无符号指针。 

说明 

从归档文件读取指定的字节数。归档文件不解释字节数。可以在Serialize函数里使用Read成员函数，读取包含在对象中的普通结构。 

示例 

extern CArchive ar; 

char pb[100]; 

UINT nr=ar.Read(pb.100); 

CArchive::ReadClass 

CRuntimeClass*ReadClass(const 

RuntimeClass*pClassRefRequested=NULL, 

UINT* pSchema=NULL,DWORD* obTag=NULL); 

throw CArchiveException; 

throw CNotSupportedException; 

返回值 

指向CRuntimeClass结构的指针。 

参数 

pClassRefRequested 

指向CRuntimeClass结构的指针，此结构对应于所需要的类的参考。可以为NULL。 

pSchema 

指向原先存储的运行时类的大纲的指针。 

obTag 

代表对象的唯一标识的数值。通过ReadObject的实现，在内部使用。只用于高级编程。obTag通常应为NULL。 

说明 

调用此成员函数来读取一个原先存储在WriteClass中的类的参考。 

如果pClassRefReguested不为NULL，ReadClass证明了归档文件类信息与例程类是兼容的。如果不兼容，ReadClass将产生一个CArchiveException。例程必须使用DECLARE_SERIAL和IMPLEMENT_SERIAL，否则，ReadClass将产生一个CNotSupportedException。如果pSchema为NULL，则存储类的大纲可通过调用CArchive::GetObjectSchema来恢复，否则，*pSchema将会包含原先存储的运行时类的大纲。可以使用SerializeClass来代替ReadClass，它可以处理类参考的读和写。 

请参阅   CArchive::WriteClass, 

CArchive::GetObjectSchema, 

CArchive::SetObjectSchema, 

CArchiveExcepti_on, 

CNotSupportedException, 

CArchive::SerializeClass 

CArchive::ReadObject 

CObject*ReadObject(const CRuntimeClass *pClass);   throw(CFileException,CArchiveException,CMemoryException); 

返回值 

CObject指针。它必须通过使用CObject::IsKindOf安全地指向当前的派生类。 

参数 

pClass 

指向CRumtimeClass结构的常量指针。此结构对应于期待读入的对象。 

说明 

从归档文件中读入对象并构造一个合适类型的对象。此函数通常由CArchive提取（>>）运算符重载一个CObject指针调用它。ReadObject，反过来，调用归档文件类的Serlialize函数。如果提供一个非零的pClass参数，它可以从RUNTIME_CLASS获得，则函数确认归档文件的运行类。假设在类的实现中已经使用IMPLEMENT_SERIAL宏。 

请参阅   CArchive::WriteObject, 

CObject::IsKindOf 

CArchive::ReadStringBool 

ReadString(CString&rString); 

LPTSTR ReadString(LPTSTR lpsz,UINT nMax);   

throw(CArchiveException); 

返回值 

在返回逻辑值的版本中，True代表成功；False相反。在返回LPTSTR的版本中，LPTSTR是一个指向包含文字数据的缓冲区的指针，NULL代表到达文件尾。 

参数 

rString 

CString的一个参考。CString将包含从与CArchive对象有关的文件中读出的结果字符串。 

Lpsz 

指定一个指向提供的缓冲区的指针。此缓冲区将接收一个以空终止符结尾的文字串。 

nMax 

指定读入字符的最大数。它不允许小于lpsz缓冲区的大小。 

说明 

调用此函数从与CArchive对象有关的文件中读取数据，放入缓冲区。在带有nMax参数的成员函数的版本中，缓冲区最多有nMax-1字符。读入的操作由一个回车换行符终止。在新行符号后面的字符都被丢弃了。在每种情况下，都有一个空字符（“/0”）。CArchive::Read还对文本-模式输入有效，但是它不以回车换行符终止。 

请参阅   CArchive::Read, 

CArchive::WriteString, 

CArchiveException 

CArchive::SerializeClass 

void SerializeClass(const CRuntimeClass*pRuntimeClass); 

参数 

pRuntimeClass 

指向基类的运行类对象的指针。 

说明 

当想存储和装载一个基类的版本信息时，调用此成员函数。SerializeClass读或对CArchive对象写一个类的参考，取决于CArchive的方向。使用SerializeClass来代替ReadClass和WriteClass，可以使得更方便地串行化基类对象。因为SerializeClass需要较少的代码和参数。像ReadClass一样，SerializeClass证明了归档文件信息与的例程类兼容。如果不兼容，则SerializeClass将一个CArchiveException。运行类必须使用DECLARE_SERIAL和IMPLEMENT_SERIAL,否则，SerializeClass将会一个CNotSupportException。使用RUMTIME_CLASS宏，恢复pRuntimeClass参数的值。基类必须已经使用IMPLEMENT_SERIAL宏。 

示例 

class CBaseClass:public CObject{…}; 

class CDerivedClass:public CBaseClass{…}; 

void CDerivedClass::Serialize(CArchive& ar) 

{   

if(ar.IsStoring( ))   

{     

//normal code for storing contents     

//of this object   

}   

else   

{     

//normal code for reading contents     

//of this object   }   

//allow the base class to serialize along   

// with its version information   

ar.SerializeClass(RUNTIME_CLASS(CBaseClass));   

CBaseClass::Serialize(ar); 

}请参阅   CArchive::ReadClass, 

CArchive::WriteClass, 

CArchive::GetObjectSchema, 

CArchive::SetObjectSch_ema, 

CArchiveException, 

CNotSupportException 

CArchive::SetLoadParams 

void SetLoadParams(UINT nGrowBy=1024); 

参数 

nGrowBy 

如果必须扩大容量的话，这是分配的元素的最小数。 

说明 

当要由归档文件读入大量CObject派生对象时，调用SetLoadParams。CArchive使用一个装载数组，解决对存储在归档文件中的对象的参考。SetLoadParams允许设置装载数组扩展的大小。在装载任何对象或调用MapObject或调用ReadObject之后，不需要调用SetLoadParams。 

示例 

class CMyLargeDocument:public CDocument{…}; 

void CMyLargeDocument::Serialize(CArchive& ar) 

{   

if (ar.IsStoring( ))     

ar.SetStoreParams( ); 

//use large defaulte   

else     

ar.SetLoadParams( );   

if (ar.IsStoring( ))   

{     

//code for storing CMyLargeDocument   

}   

else   

{       

//code for loading CMyLargeDocument 

   }} 

请参阅   Cachive::SetStoreParams 

CArchive::SetLoadSchema 

void SetLoadSchema(UINT nSchema); 

参数 

nSchema指定对象的大纲。 

说明 

调用此成员函数，设置存储在归档文件中的对象大纲。将此设置值放入nSchema中。对GetObjectSchema的再一次调用，将返回存储在nSchema中的值。SetObjectSchema可用于高级版本：例如，当想在一个派生类的Serialize函数中读入一个特别版本时。 

请参阅   CArchive::GetObjectSchema 

CArchive::SetStoreParams 

void SetStoreParams(UINT nHashSize=2053,UINT nBlockSize=128); 

参数 

nHashSize 

用于指针映射接口的哈希表的大小，必须是一个素数。 

nBlockSize 

为扩展参数指定内存分配间隔。要获得最好的执行效果，应该是2的倍数。 

说明 

当在一个归档文件中存储许多CObject派生对象时，使用SetStoreParams。SetStoreParams允许设定哈希表的大小和映射的块的大小。映射用来在串行化的过程中标识唯一的对象。在存储任何一个对象或调用MapObject或WriteObject之后，不需要调用SetStoreParams。 

示例 

class CMyLargeDocument:public CDocument{…}; 

void CMyLargeDocument::Serialize(CArchive& ar) 

{     

if (ar.IsStoring( ))     

ar.SetStoreParams( ); 

//use large defaults   

else     

ar.SetLoadParams( ); 

   if (ar.IsStoring( )) 

{     

//code for storing CMyLargeDocument   

}   

else   

{     

//code for loading CMyLargeDocument   

} 

} 

请参阅   CArchive::SetLoadParams 

CArchive::Write 

void Write(const void*lpBuf,UINT nMax);   

throw(CFileException); 

参数 

lpBuf指向提供的缓冲区的指针。缓冲区中包含了将被写入归档文件的数据。 

nMax指定要被写入归档文件的字节数的整数。 

说明 

向归档文件写入特定的字节数。归档文件不会格式化字节。可以在Serialize函数中使用Write成员函数，写入一个包含在对象中的普通结构。 

示例 

extern CArchive ar; 

char pb[100]; 

ar.Write(pb.100); 

请参阅   

CArchive::Read 

CArchive::WriteClass 

void WriteClass(const CRuntimeClass*pClassRef); 

参数 

pClassRef 

指向CRuntimeClass结构的指针。此结构对应于所需要的类的参考。 

说明 

在一个派生类的串行化过程中，使用WriteClass存储基类的版本和类信息。WriteClass将基类的CRuntimeClass参考写入CArchive。使用CArchive::ReadClass恢复参考。WriteClass证明了归档文件信息与例程类兼容。如果不兼容，WriteClass 

将CArchive::Exception。例程必须使用DECLARE_SERIAL和IMPLEMENT_SERIAL。否则，WriteClass将一个CNotSuppotedException。可以使用SerializeClass代替WriteClass，它将处理类参考的读与写。 

请参阅   

CArchive::ReadClass, 

CArchive::GetObjectSchema, 

CArchive::SetObjectSchema, 

Cachive::Serializ_eClass, 

CArchiveException, 

CNotSupportException. 

CArchive::WriteObjectvoid 

WriteObject(const CObject*pOb);   

throw(CFileException,CArchiveException); 

参数 

pOb被存储对象的常量指针。 

说明 

将特定的CObject存储到归档文件中。此函数通常由重载CObject的CArchive插入（<<）运算符调用。WriteClass反过来，调用归档类的Serialize函数。必须使用IMPLEMENT_SERIAL宏归档。WriteObject将ASCII类名写入归档文件。在装载过程之后，此类名会被验证。一个特定的编码设计会防止对于多个对象类名的不必要的重复使用。还能防止是多个指针目 

标的对象的多余存储。正确的对象编码方法（包括在现在的ASCII类名中）是一个实现细节并且在库的以后版本中可以改变。 

注意   

在归档之前，结束创建、删除和修改所有对象。如果把归档和对象修改混合，则归档文件将会被废弃。 

请参阅   CArchive::ReadObject 

CArchive::WriteString 

void WriteString(LPCTSTR lpsz);   

throw(CFileException); 

参数 

lpsz使用此成员函数将数据从一个缓冲区写入与CArchive对象有关的文件中。结束的空字符（‘\0’）不被写入文件，新行也不自动写入。WriteString对于几种情况都可以异常，包括磁盘满时。Write还是有效的，但不是以空终止符结尾，而是它向文件写入需要的字节数。 

请参阅   

CArchive::Write, 

CArchive::Read, 

CArchive::ReadString, 

CFileException 

操作符 

CArchive::operator<< 

friend CArchive& operator<< (CArchive& ar,const CObject* pOb);   

throw (CArchiveException,CFileException); 

CArchive&operator<<(BYTE by);   

throw (CArchiveException,CFileException); 

CArchive&operator<<(WORD w);   

throw (CArchiveException,CFileException); 

CArchive&operator<<( int i);   

throw (CArchiveException,CFileException); 

CArchive&operator<<( LONG l);   

throw (CArchiveException,CFileException); 

CArchive&operator<<( DWORD dw);   

throw (CArchiveException,CFileException); 

CArchive&operator<<( float f);   

throw (CArchiveException,CFileException); 

CArchive&operator<<( double d);   

throw (CArchiveException,CFileException); 

返回值 

CArchive参考，它可以在一行上使用多个提取运算符。 

说明 

把确切的对象或主要类型存储到归档文件中。 

如果在类的实现中使用IMPLEMENT_SERIAL宏，然后重载CObject的插入运算符调用被保护的WriteObject。此函数反过来调用类的Serialize函数。 

示例 

long l; 

int i; 

extern CArchive ar; 

if (ar.IsStoring( ))   

ar<<l<<i; 

请参阅   

CArchive::WriteObject, 

CObject::Serialize 

CArchive::operator>> 

friend CArchive& operator>> (CArchive& ar, CObject* pOb);   

throw (CArchiveException,CFileException,CMemoryException); 

friend CArchive& operator>> (CArchive& ar,const CObject* pOb);   

throw (CArchiveException,CFileException,CMemoryException); 

CArchive&operator>>(BYTE& by);   

throw (CArchiveException,CFileException); 

CArchive&operator>>(WORD& w);   

throw (CArchiveException,CFileException); 

CArchive&operator>>( int& i);   

throw (CArchiveException,CFileException); 

CArchive&operator>>( LONG& l);   

throw (CArchiveException,CFileException); 

CArchive&operator>>( DWORD& dw);   

throw (CArchiveException,CFileException); 

CArchive&operator>>( float& f);   

throw (CArchiveException,CFileException); 

CArchive&operator>>( double& d);   

throw (CArchiveException,CFileException); 

返回值 

可使多个插入运算符在一行上的CArchive参考。 

说明 

由归档文件装载确定的对象或主要类型。如果在类的实现中使用IMPLEMENT_SERIAL宏，然后重载CObject的提取运算符调用被保护的ReadObject函数（带有一个非零的运行时指针）。此函数反过来调用类的Serialize函数。 

示例 

int i; 

extern CArchive ar; 

if(ar.IsLoading( ))   

ar>>i; 

请参阅   

CArchive::ReadObject, 

CObject::Serialize 

数据成员 

CArchive::m_pDocument 

说明 

缺省情况下，设置为NULL。这个指向CDocument的指针可以被设置为CAchive实例的想设置的任何一种情况。一个关于此指针的通用使用就是把有关串行化进程的附加信息传递给所有被串行化的对象。在文档（CDocument派生类）正在串行化时，通过初始化指针归档。这样一来，如果必要的话，文档中的对象就可以访问文档。此指针在串行化时还可以由COleClientItem对象使用。当一个打开文件或保存命令时，框架对正在串行化的文档设置m_pDocument。如果不是因为打开文件或保存的原因要串行化一个对象链接和嵌入（OLE）包容器文档，则必须明确地设置m_pDocument。举例来说，当对一个剪贴板串行化一个包容器文档时，应该这样做。 

请参阅   

CDocument, 

COleClientItem 

Track:http://www.yourblog.org/Data/20059/327374.html






