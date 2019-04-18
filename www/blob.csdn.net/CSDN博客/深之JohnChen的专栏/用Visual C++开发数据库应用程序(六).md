# 用Visual C++开发数据库应用程序(六) - 深之JohnChen的专栏 - CSDN博客

2006年03月29日 15:35:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2586


用Visual C++开发数据库应用程序(六)

5 使用OLE DB

5．1 概述

OLE DB的存在为用户提供了一种统一的方法来访问所有不同种类的数据源。OLE DB可以在不同的数据源中进行转换。利用OLE DB，客户端的开发人员在进行数据访问时只需把精力集中在很少的一些细节上，而不必弄懂大量不同数据库的访问协议。

OLE DB是一套通过COM接口访问数据的ActiveX接口。这个OLE DB接口相当通用，足以提供一种访问数据的统一手段，而不管存储数据所使用的方法如何。同时，OLE DB还允许开发人员继续利用基础数据库技术的优点，而不必为了利用这些优点而把数据移出来。

5．2 使用ATL使用OLE DB数据使用程序

由于直接使用OLE DB的对象和接口设计数据库应用程序需要书写大量的代码。为了简化程序设计，Visual C++提供了ATL模板用于设计OLE DB数据应用程序和数据提供程序。

利用ATL模板可以很容易地将OLE DB与MFC结合起来，使数据库的参数查询等复杂的编程得到简化。MFC提供的数据库类使OLE DB的编程更具有面向对象的特性。Viual C++所提供用于OLE DB的ATL模板可分为数据提供程序的模板和数据使用程序的模板。

使用ATL模板创建数据应用程序一般有以下几步骤：

创建应用框架 

加入ATL产生的模板类 

在应用中使用产生的数据访问对象 

不用ATL使用OLE DB数据使用程序 

利用ATL模板产生数据使用程序较为简单，但适用性不广，不能动态适应数据库的变化。下面我们介绍直接使用MFC OLE DB类来生成数据使用程序。

模板的使用

OLE DB数据使用者模板是由一些模板组成的，包括如下一些模板，下面对一些常用类作一些介绍。

会话类 

CDataSource类

CDataSource类与OLE DB的数据源对象相对应。这个类代表了OLE DB数据提供程序和数据源之间的连接。只有当数据源的连接被建立之后，才能产生会话对象，可以调用Open来打开数据源的连接。

CSession类

CSession所创建的对象代表了一个单独的数据库访问的会话。一个用CDataSource类产生的数据源对象可以创建一个或者多个会话，要在数据源对象上产生一个会话对象，需要调用函数Open()来打开。同时，会话对象还可用于创建事务操作。

CEnumeratorAccessor类

CEnumeratorAccessor类是用来访问枚举器查询后所产生的行集中可用数据提供程序的信息的访问器，可提供当前可用的数据提供程序和可见的访问器。

访问器类 

CAcessor类

CAccessor类代表与访问器的类型。当用户知道数据库的类型和结构时，可以使用此类。它支持对一个行集采用多个访问器，并且，存放数据的缓冲区是由用户分配的。

CDynamicAccessor类

CDynamicAccessor类用来在程序运行时动态的创建访问器。当系统运行时，可以动态地从行集中获得列的信息，可根据此信息动态地创建访问器。

CManualAccessor类

CManualAccessor类中以在程序运行时将列与变量绑定或者是将参数与变量捆定。

行集类 

CRowSet类

CRowSet类封装了行集对象和相应的接口，并且提供了一些方法用于查询、设置数据等。可以用Move（）等函数进行记录移动，用GetData()函数读取数据，用Insert()、Delete()、SetData()来更新数据。

CBulkRowset类

CBulkRowset类用于在一次调用中取回多个行句柄或者对多个行进行操作。

CArrayRowset类

CArrayRowset类提供用数组下标进行数据访问。

命令类 

CTable类

CTable类用于对数据库的简单访问，用数据源的名称得到行集，从而得到数据。

CCommand类

CCommand类用于支持命令的数据源。可以用Open()函数来执行SQL命令，也可以Prepare（）函数先对命令进行准备，对于支持命令的数据源，可以提高程序的灵活性和健壮性。

在stdafx.h头文件里，加入如下代码。

#include 

extern CComModule _Module;

#include 

#include 

#include // if you are using schema templates

在stdafx.cpp文件里，加入如下代码。

#include 

CComModule _Module;

决定使用何种类型的存取程序和行集。

获取数据

在打开数据源，会话，行集对象后就可以获取数据了。所获取的数据类型取决于所用的存取程序，可能需要绑定列。按以下步骤。

用正确的命令打开行集对象。 

如果使用CManualAccessor，在使用之前与相应列进行绑定。要绑定列，可以用函数GetColumnInfo,如下所示：

// Get the column information

ULONG ulColumns = 0;

DBCOLUMNINFO* pColumnInfo = NULL;

LPOLESTR pStrings = NULL;

if (rs.GetColumnInfo(&ulColumns, &pColumnInfo, &pStrings) != S_OK)

AfxThrowOLEDBException(rs.m_pRowset, IID_IColumnsInfo);

struct MYBIND* pBind = new MYBIND[ulColumns];

rs.CreateAccessor(ulColumns, &pBind[0], sizeof(MYBIND)*ulColumns);

for (ULONG l=0; l 

rs.AddBindEntry(l+1, DBTYPE_STR, sizeof(TCHAR)*40, &pBind[l].szValue, NULL, &pBind[l].dwStatus);

rs.Bind();

用while循环来取数据。在循环中，调用MoveNext来测试光标的返回值是否为S_OK，如下所示： 

while (rs.MoveNext() == S_OK)

{

// Add code to fetch data here

// If you are not using an auto accessor, call rs.GetData()

}

在while循环内，可以通过不同的存取程序获取数据。 

如果使用的是CAccessor类，可以通过使用它们的数据成员进行直接访问。如下所示： 

如果使用的是CDynamicAccessor 或CDynamicParameterAccessor 类，可以通过GetValue或GetColumn函数来获取数据。可以用GetType来获取所用数据类型。如下所示： 

while (rs.MoveNext() == S_OK)

{

// Use the dynamic accessor functions to retrieve your

// data

ULONG ulColumns = rs.GetColumnCount();

for (ULONG i=0; i 

{

rs.GetValue(i);

}

}

如果使用的是CManualAccessor,可以指定自己的数据成员，绑定它们。就可以直接存取。如下所示： 

while (rs.MoveNext() == S_OK)

{

// Use the data members you specified in the calls to

// AddBindEntry.

wsprintf("%s", szFoo);

}

决定行集的数据类型

在运行时决定数据类型，要用动态或手工的存取程序。如果用的是手工存取程序，可以用GetColumnInfo函数得到行集的列信息。从这里可以得到数据类型。

5．4 总结

由于现在有多种数据源，，想要对这些数据进行访问管理的唯一途径就是通过一些同类机制来实现，如OLE DB。高级OLE DB结构分成两部分：客户和提供者。客户使用由提供者生成的数据。

就像其它基于COM的多数结构一样，OLE DB的开发人员需要实现很多的接口，其中大部分是模板文件。

当生成一个客户对象时，可以通过ATL对象向导指向一个数据源而创建一个简单的客户。ATL对象向导将会检查数据源并创建数据库的客户端代理。从那里，可以通过OLE DB客户模板使用标准的浏览函数。

当生成一个提供者时，向导提供了一个很好的开端，它们仅仅是生成了一个简单的提供者来列举某一目录下的文件。然后，提供者模板包含了OLE DB支持的完全补充内容。在这种支持下，用户可以创建OLE DB提供者，来实现行集定位策略、数据的读写以及建立书签。

6、 使用ADO

6．1 概述

ADO是ActiveX数据对象（ActiveX Data Object），这是Microsoft开发数据库应用程序的面向对象的新接口。ADO访问数据库是通过访问OLE DB数据提供程序来进行的，提供了一种对OLE DB数据提供程序的简单高层访问接口。

ADO技术简化了OLE DB的操作，OLE DB的程序中使用了大量的COM接口，而ADO封装了这些接口。所以，ADO是一种高层的访问技术。

ADO技术基于通用对象模型（COM），它提供了多种语言的访问技术，同时，由于ADO提供了访问自动化接口，所以，ADO可以用描述的脚本语言来访问VBScript,VCScript等。　

6．2 在VC中使用ADO

可以使用VC6提供的ActiveX控件开发应用程序，还可以用ADO对象开发应用程序。使用ADO对象开发应用程序可以使程序开发者更容易地控制对数据库的访问，从而产生符合用户需求的数据库访问程序。

使用ADO对象开发应用程序也类似其它技术，需产生与数据源的连接，创建记录等步骤，但与其它访问技术不同的是，ADO技术对对象之间的层次和顺序关系要求不是太严格。在程序开发过程中，不必选建立连接，然后才能产生记录对象等。可以在使用记录的地方直接使用记录对象，在创建记录对象的同时，程序自动建立了与数据源的连接。这种模型有力的简化了程序设计，增强了程序的灵活性。下面讲述使用ADO对象进行程序设计的方法。

6．21 引入ADO库文件

使用ADO前必须在工程的stdafx.h文件里用直接引入符号＃import引入ADO库文件，以使编译器能正确编译。代码如下所示：

#define INITGUID

#import "c:/program files/common files/system/ado/msado15.dll" no_namespace rename("EOF","EndOfFile")

#include "icrsint.h"

这行语句声明在工程中使用ADO，但不使用ADO的名字空间，并且为了避免冲突，将EOF改名为EndOfFile。

6．22 初始化ADO环境

在使用ADO对象之前必须先初始化COM环境。初始化COM环境可以用以下代码完成：

::CoInitialize(NULL);

在初始化COM环境后，就可以使用ADO对象了，如果在程序前面没有添加此代码，将会产生COM错误。

在使用完ADO对象后，需要用以下的代码将初始化的对象释放：

::CoUninitialize();

此函数清除了为ADO对象准备的COM环境。

6．23 接口简介

ADO库包含三个基本接口:

__ConnectionPtr接口、

__CommandPtr接口、

__RecordsetPtr接口,　

__ConnectionPtr接口返回一个记录集或一个空指针。通常使用它来创建一个数据连接或执行一条不返回任何结果的SQL语句，如一个存储过程。用__ConnectionPtr接口返回一个记录集不是一个好的使用方法。通常同CDatabase一样，使用它创建一个数据连接，然后使用其它对象执行数据输入输出操作。　

__CommandPtr接口返回一个记录集。它提供了一种简单的方法来执行返回记录集的存储过程和SQL语句。在使用__CommandPtr接口时，可以利用全局__ConnectionPtr接口，也可以在__CommandPtr接口里直接使用连接串。如果只执行一次或几次数据访问操作，后者是比较好的选择。但如果要频繁访问数据库，并要返回很多记录集，那么，应该使用全局__ConnectionPtr接口创建一个数据连接，然后使用__CommandPtr接口执行存储过程和SQL语句。　

__RecordsetPtr是一个记录集对象。与以上两种对象相比，它对记录集提供了更多的控制功能，如记录锁定，游标控制等。同__CommandPtr接口一样，它不一定要使用一个已经创建的数据连接，可以用一个连接串代替连接指针赋给__RecordsetPtr的connection成员变量，让它自己创建数据连接。如果要使用多个记录集，最好的方法是同Command对象一样使用已经创建了数据连接的全局—ConnectionPtr接口，然后使用__Recordse7tPtr执行存储过程和SQL语句。

6、24 使用ADO访问数据库

__ConnectionPtr是一个连接接口，首先创建一个__ConnectionPtr接口实例，接着指向并打开一个ODBC数据源或OLE DB数据提供者(Provider)。以下代码分别创建一个基于DSN和非DSN的数据连接。

　　//使用__ConnectionPtr(基于DSN)

　　__ConnectionPtr MyDb;

　　MyDb.CreateInstance(__uuidof(Connection));

　　MyDb－〉Open("DSN=samp;UID=admin;PWD=admin","","",－1);

　　//使用—ConnectionPtr (基于非DSN)

　　__ConnectionPtr MyDb;

　　MyDb.CreateInstance(__uuidof(Connection));

MyDb.Open("Provider=SQLOLEDB;SERVER=server;DATABASE=samp;UID=admin;PWD=admin","","",－1);

//使用__RecordsetPtr执行SQL语句

　　__RecordsetPtr MySet;

　　MySet.CreateInstance(__uuidof(Recordset));

MySet－〉Open("SELECT * FROM some__table",　　MyDb.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);

现在我们已经有了一个数据连接和一个记录集，接下来就可以使用数据了。从以下代码可以看到，使用ADO的__RecordsetPtr接口，就不需要像DAO那样频繁地使用大而复杂的数据结构VARIANT，并强制转换各种数据类型了，这也是ADO的优点之一。假定程序有一个名称为m__List的ListBox控件，下面代码我们用__RecordsetPtr接口获取记录集数据并填充这个ListBox控件：

__variant__t Holder

　　try{while(!MySet－〉adoEOF)

　　{ Holder = MySet－〉GetCollect("FIELD__1");

　　if(Holder.vt!=VT__NULL)

　　m__List.AddString((char)__bstr__t(Holder));

　　MySet－〉MoveNext();} }

　　catch(__com__error  e)

　　{ CString Error = e－〉ErrorMessage();

　　 AfxMessageBox(e－〉ErrorMessage());

　　} catch(...)

　　{ MessageBox("ADO发生错误!");}　

必须始终在代码中用try和catch来捕获ADO错误,否则ADO错误会使你的应用程序崩溃。当ADO发生运行错误时(如数据库不存在)，OLE DB数据提供者将自动创建一个__com__error对象，并将有关错误信息填充到这个对象的成员变量。　

6．25 类型转换

由于COM对象是跨平台的，它使用了一种通用的方法来处理各种类型的数据，因此CString 类和COM对象是不兼容的，我们需要一组API来转换COM对象和C＋＋类型的数据。__vatiant__t和__bstr__t就是这样两种对象。它们提供了通用的方法转换COM对象和C＋＋类型的数据。

6．3 在VB中使用ADO

ADO 提供执行以下操作的方式： 

1、连接到数据源。同时，可确定对数据源的所有更改是否已成功或没有发生。

2、指定访问数据源的命令，同时可带变量参数，或优化执行。

3、执行命令。

3、如果这个命令使数据按表中的行的形式返回，则将这些行存储在易于检查、操作或更改的缓存中。

4、适当情况下，可使用缓存行的更改内容来更新数据源。

5、提供常规方法检测错误（通常由建立连接或执行命令造成）。 

在典型情况下，需要在编程模型中采用所有这些步骤。但是，由于 ADO 有很强的灵活性，所以最后只需执行部分模块就能做一些有用的工作。　

以下元素是 ADO 编程模型中的关键部分： 

6．31 连接

通过“连接”可从应用程序访问数据源，连接是交换数据所必需的环境。对象模型使用 Connection 对象使连接概念得以具体化。

“事务”用于界定在连接过程中发生的一系列数据访问操作的开始和结束。ADO 可明确事务中的操作造成的对数据源的更改或者成功发生，或者根本没有发生。如果取消事务或它的一个操作失败，则最终的结果将仿佛是事务中的操作均未发生，数据源将会保持事务开始以前的状态。对象模型无法清楚地体现出事务的概念，而是用一组 Connection 对象方法来表示。ADO 从 OLE DB 提供者访问数据和服务。Connection 对象用于指定专门的提供者和任意参数。

6．32 命令

通过已建立的连接发出的“命令”可以某种方式来操作数据源。一般情况下，命令可以在数据源中添加、删除或更新数据，或者在表中以行的格式检索数据。对象模型用 Command 对象来体现命令概念。使用 Command 对象可使 ADO 优化命令的执行。

参数 

通常，命令需要的变量部分即“参数”可以在命令发布之前进行更改。例如，可重复发出相同的数据检索命令，但每一次均可更改指定的检索信息。

参数对与函数活动相同的可执行命令非常有用，这样就可知道命令是做什么的，但不必知道它如何工作。例如，可发出一项银行过户命令，从一方借出贷给另一方。可将要过户的款额设置为参数。

对象模型用 Parameter 对象来体现参数概念。

6．33 记录集

如果命令是在表中按信息行返回数据的查询（行返回查询），则这些行将会存储在本地。

对象模型将该存储体现为 Recordset 对象。但是，不存在仅代表单独一个 Recordset 行的对象。

记录集是在行中检查和修改数据最主要的方法。

6．34 字段

一个记录集行包含一个或多个“字段”。如果将记录集看作二维网格，字段将排列构成“列”。每一字段（列）都分别包含有名称、数据类型和值的属性，正是在该值中包含了来自数据源的真实数据。

对象模型以 Field 对象体现字段。

要修改数据源中的数据，可在记录集行中修改 Field 对象的值，对记录集的更改最终被传送给数据源。作为选项，Connection 对象的事务管理方法能够可靠地保证更改要么全部成功，要么全部失败。

6．35 错误

错误随时可在应用程序中发生，通常是由于无法建立连接、执行命令或对某些状态（例如，试图使用没有初始化的记录集）的对象进行操作。

对象模型以 Error 对象体现错误。

任意给定的错误都会产生一个或多个 Error 对象，随后产生的错误将会放弃先前的 Error 对象组。

6．36 属性

每个 ADO 对象都有一组唯一的“属性”来描述或控制对象的行为。

属性有内置和动态两种类型。内置属性是 ADO 对象的一部分并且随时可用。动态属性则由特别的数据提供者添加到 ADO 对象的属性集合中，仅在提供者被使用时才能存在。

对象模型以 Property 对象体现属性。

6．37 集合

ADO 提供“集合”，这是一种可方便地包含其他特殊类型对象的对象类型。使用集合方法可按名称（文本字符串）或序号（整型数）对集合中的对象进行检索。

ADO 提供四种类型的集合： 

Connection 对象具有 Errors 集合，包含为响应与数据源有关的单一错误而创建的所有 Error 对象。

Command 对象具有 Parameters 集合，包含应用于 Command 对象的所有 Parameter 对象。

Recordset 对象具有 Fields 集合，包含所有定义 Recordset 对象列的 Field 对象。

此外，Connection、Command、Recordset 和 Field 对象都具有 Properties 集合。它包含所有属于各个包含对象的 Property 对象。 

ADO 对象拥有可在其上使用的诸如“整型”、“字符型”或“布尔型”这样的普通数据类型来设置或检索值的属性。然而，有必要将某些属性看成是数据类型“COLLECTION OBJECT”的返回值。相应的，集合对象具有存储和检索适合该集合的其他对象的方法。

6．38 事件 

ADO 2.0 支持事件，事件是对某些操作将要或已经发生的通知。

有两类事件：ConnectionEvent 和 RecordsetEvent。Connection 对象产生 ConnectionEvent 事件，而 Recordset 对象则产生 RecordsetEvent 事件。

事件由事件处理程序例程处理，该例程在某个操作开始之前或结束之后被调用。

某些事件是成对出现的。开始操作前调用的事件名格式为 WillEvent（Will 事件），而操作结束后调用的事件名格式为 EventComplete（Complete 事件）。其余的不成对事件只在操作结束后发生。（其名称没有任何固定模式。）事件处理程序由状态参数控制。附加信息由错误和对象参数提供。

可以请求事件处理程序不接受第一次通知以后的任何通知。例如，可以选择只接收 Will 事件或 Complete 事件。

下面的代码显示了一个使用ADO的例子。

首先加入Microsoft ActiveX Data Object 2.0 Library引用。

Dim db As Connection

Set db = New Connection

db.CursorLocation = adUseClient

db.Open "PROVIDER=MSDASQL;DSN=TestDatabase", "sa", "", -1

Dim i As Long

Dim id As Long

Dim value As Single

Dim rst As New Recordset

Set rst = New Recordset

rst.Open "select * from 模拟量变化历史表", db, adOpenDynamic, adLockOptimistic

rst.MoveFirst

For i = 0 To rst.RecordCount - 1

id = rst.Fields("ID")

value=rst.Fields(“VALUE”)

rst.MoveNext

Next i

rst.Close

Set rst = Nothing

db.Close

6．4总结

ADO技术是访问数据库的新技术，具有易于使用、访问灵活、应用广泛的特点。用ADO访问数据源的特点可总结如下：

易于使用 

这是ADO技术的最重要的一个特征。由于ADO是高层应用，所以相对于OLE DB或者ODBC来说，它具有面向对象的特性。同时，在ADO的对象结构中，其对象之间的层次关系并不明显。相对于DAO等访问技术来讲，又不必关心对象的构造顺序和构造层次。对于要用的对象，不必选建立连接、会话等对象，只需直接构造即可，方便了应用程序的编制。

高速访问数据源 

由于ADO技术基于OLE DB，所以，它也继承了OLE DB访问数据库的高速性。

可以访问不同数据源 

ADO技术可以访问包括关系数据库和非关系数据库的所有文件系统。此特点使应用程序有很多的灵活性和通用性。

可以用于Microsoft ActiveX页 

ADO技术可以以ActiveX控件的形式出现，所以，可以被用于Microsoft ActiveX页，此特征可简化WEB页的编程。

程序占用内存少 

由于ADO是基于组件对象模型（COM）的访问技术，所以，用ADO产生的应用程序占用内存少。

7、总结

要在访问数据时判断出应该使用哪一种技术，这并不容易。可能需要公用实用程序来处理多个数据库类型；部分数据可能出现在本地硬盘驱动器上，部分在网络上，还有一部分在主机上。甚至客户安装在设备上的产品也会使这种选择更加困难。例如，你所期待的ODBC支持级别也许依赖于所安装的Microsoft Office的版本，因为这个产品不提供ODBC支持。你还会发现，ADO类提供的对象和方法要比ODBC类多。ADO可以提供程序中绝对必须具有的一些特性棗例如，你会发现OLE-DB和ADO两者都支持DFX_Currency，但在ODBC中没有对应的功能，但你要想掌握它们也必须付出一定的努力。

选择OLE-DB或ODBC时，有几条一般的规则。因为ADO实际上只是OLE-DB的包装，所以这些规则也适用于它。下面提供一些基本的原则，可以用来帮助你决定选择OLE-DB还是ODBC。

非OLE环境 如果要访问支持ODBC的数据库，而该数据库又在不支持OLE的服务器上，那么ODBC是最好的选择。

非SQL环境 ODBC在处理SQL时非常出众。处理非SQL数据库时，OLE-DB则具有非常明显的优势。

OLE环境 对支持OLE的服务器来说，选择OLE-DB还是ODBC也许是希望各半。如果有ODBC驱动程序可供利用，那么使用ODBC是一个好主意；否则，就只有选择OLE-DB了。

所需的互操作性 如果需要可互操作的数据库部件，那么只有选择OLE-DB。


