# 用Visual C++开发数据库应用程序(四) - 深之JohnChen的专栏 - CSDN博客

2006年03月29日 15:31:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1675


用Visual C++开发数据库应用程序(四)

3、 使用MFC访问ODBC数据源

3．1 概述

VisualC++的MFC类库定义了几个数据库类。在利用ODBC编程时，经常要使用到CDatabase(数据库类)，CRecordSet(记录集类)和CRecordView(可视记录集类)。 其中： 

CDatabase类对象提供了对数据源的连接，通过它你可以对数据源进行操作。

CRecordView类对象能以控制的形式 显示数据库记录。这个视图是直接连到一个CRecordSet对象的表视图。

CRecordSet类对象提供了从数据源 中提取出的记录集。CRecordSet对象通常用于两种形式： 动态行集（dynasets）和快照集（snapshots）。动态行集能保 持与其他用户所做的更改保持同步。快照集则是数据的一个静态视图。每一种形式在记录集被打开时都提供一组记录，所不同的是，当你在一个动态行集里滚 动到一条记录时，由其他用户或是你应用程序中的其他记录集对该记录所做的更改会相应地显示出来。　

Visual C++提供了几种记录集，可以用来定制应用程序的工作方式。查看这些不同选项的最快方式要兼顾速度和特征。你会发现，在很多情况下，如果想添加特征，就必须付出程序执行速度降低的代价。下面告诉你一些可以自由支配的记录集选项。更重要的是，要告诉你从这个选项可以获得更快的速度还是更多的特征。

1、Snapshot（快照） 这个选项要Visual C++在一次快照中下载整个查询。换言之，及时快速地给数据库内容拍照，并把它作为未来工作的基础。这种方法有三个缺点。第一，你看不到别人在网络上做的更新，这可能意味着你的决定是建立在老信息的基础上。第二，一次就下载所有这些记录，这意味着在下载期间给网络增加了沉重的负担。第三，记录下载时用户会结束等待，这意味着网络的呼叫性能变得更低。然而这种方法也有两个优点。第一，记录一旦被下载，该工作站所需的网络活动几乎就没有了棗这为其它请求释放了带宽。总之，你会看到网络的吞吐量增大了。第二，因为所有被申请的记录都在用户的机器上，所以用户实际上会得到应用程序更佳的总体性能。你可能想把快照的方法限制在较小的数据库上使用，原因在于快照适用于用户请求信息而不适用于数据编辑会话。

2、Dynaset（动态集） 使用这个选项时，Visual C++创建指向所请求的每个记录的实际指针。另外，只有填充屏幕时实际需要的记录是从服务器上下载来的。这种方法的好处很明显。几乎马上就能在屏幕上看到记录。而且还会看到其它用户对数据库所做的更改。最后，其它用户也会看到你做的更改，因为动态集在你更改记录时被上载到服务器上。很明显，这种方法要求对服务器的实时访问，它减小了网络总吞吐量并降低了应用程序的性能。这个选项适合于创建用户要花费很多时间来编辑数据的应用程序。同时，它也是大型数据库的最佳选择，原因在于只需下载用户实际需要的信息。 

3．2 应用ODBC编程

可以应用AppWizard来建立一个ODBC的应用程序框架，也可以直接使用ODBC来进行数据库编程，这时，应包括头文件afxdb.h。

应用ODBC编程两个最重要的类是CDatabase和CRecordSet，但在应用程序中，不应直接使用CRecordSet类，而必须从CRecordSet类产生一个导出类，并添加相应于数据库表中字段的成员变量。随后，重载CRecordset类的成员函数DoFieldExchange，该函数通过使用RFX函数完成数据库字段与记录集域数据成员变量的数据交换，RFX函数同对话框数据交换（DDX）机制相类似，负责完成数据库与成员变量间的数据交换。　

下面举例说明在VisualC++环境中ODBC 的编程技巧：

3．21 数据库连接

在CRecordSet类中定义了一个成员变 量m_pDatabase:

CDatabase *m_pDatabase;

它是指向对象数据库类的指针。如果在CRecordSet类对象调用Open()函数之前，将一个已经打开的CDatabase类对象指针传给m_pDatabase，就能共享相同 的CDatabase类对象。如：　

CDatabase m_db;

CRecordSet m_set1,m_set2;

m_db.Open(_T("Super_ES")); // 建 立ODBC 连 接

m_set1.m_pDatabase=&m_db; //m_set1 复 用m_db 对 象

m_set2.m_pDatabse=&m_db; // m_set2 复 用m_db 对 象

或如下：

Cdatabase db;

db.Open(“Database”); //建立ODBC连接

CrecordSet m_set(&db); //构造记录集对象,使数据库指向db

3．22 查询记录

查询记录使用CRecordSet::Open()和 CRecordSet::Requery()成员函数。在使用CRecordSet类对象之前，必须使用 CRecordSet::Open()函数来获得有效的记录集。一旦已经使用过CRecordSet::Open() 函数，再次查询时就可以应用CRecordSet::Requery()函数。在调 用CRecordSet::Open()函数时，如果已经将一个已经打开的CDatabase 对象指针传给CRecordSet类对象的m_pDatabase成员变量，则使 用该数据库对象建立ODBC连接；否则如果m_pDatabase为空指 针，就新建一个CDatabase类对象并使其与缺省的数据源 相连，然后进行CRecordSet类对象的初始化。缺省数据源 由GetDefaultConnect()函数获得。你也可以提供你所需要的SQL 语句，并以它来调用CRecordSet::Open()函数，例如：

m_Set.Open(AFX_DATABASE_USE_DEFAULT,strSQL);

如果没有指定参数，程序则使 用缺省的SQL语句，即对在GetDefaultSQL()函数中指定的SQL语 句进行操作：　

CString CTestRecordSet::GetDefaultSQL()

{return _T("[BasicData],[MainSize]");}

对于GetDefaultSQL()函数返回的表名， 对应的缺省操作是SELECT语句，即：

SELECT * FROM BasicData,MainSize　

查询过程中也可以利用CRecordSet的 成员变量m_strFilter和m_strSort来执行条件查询和结果排序。m_strFilter 为过滤字符串，存放着SQL语句中WHERE后的条件串；m_strSort 为排序字符串，存放着SQL语句中ORDERBY后的字符串。 如：

m_Set.m_strFilter="TYPE='电动机'";

m_Set.m_strSort="VOLTAGE";

m_Set.Requery();

对应的SQL语句为：

SELECT * FROM BasicData,MainSize 

WHERE TYPE='电动机'

ORDER BY VOLTAGE

除了直接赋值给m_strFilter以外，还 可以使用参数化。利用参数化可以更直观，更方便地 完成条件查询任务。使用参数化的步骤如下：　

(1)．声明参变量：　

Cstring p1;

Float p2;

(2)．在构造函数中初始化参变量

p1=_T("");

p2=0.0f;

m_nParams=2;

(3)．将参变量与对应列绑定

pFX->SetFieldType(CFieldExchange::param)

RFX_Text(pFX,_T("P1"),p1);

RFX_Single(pFX,_T("P2"),p2);

完成以上步骤之后就可以利用参变量进行条件查询了：

m_pSet->m_strFilter="TYPE=?ANDVOLTAGE=?";

m_pSet->p1="电动机";

m_pSet->p2=60.0;

m_pSet->Requery();

参变量的值按绑定的顺序替换 查询字串中的“?”适配符。

如果查询的结果是多条记录的 话，可以用CRecordSet类的函数Move()，MoveNext()，MovePrev()，MoveFirst() 和MoveLast()来移动光标。

3．23 增加记录

增加记录使用AddNew()函数，要求数据库必须是以允许增加的方式打开：　

m_pSet->AddNew(); //在表的末尾增加新记录

m_pSet->SetFieldNull(&(m_pSet->m_type),FALSE); 

m_pSet->m_type="电动机";

... //输入新的字段值

m_pSet-> Update(); //将新记录存入数据库

m_pSet->Requery(); //重建记录集

3．24 删除记录

直接使用Delete()函数，并且在调用Delete() 函数之后不需调用Update()函数：

m_pSet->Delete();

if(!m_pSet->IsEOF())

m_pSet->MoveNext();

else

m_pSet->MoveLast();　

3．25 修改记录

修改记录使用Edit()函数：

m_pSet->Edit(); //修改当前记录

m_pSet->m_type="发电机"; //修改当前记录字段值

...

m_pSet->Update(); //将修改结果存入数据库

m_pSet->Requery();

3．26 统计记录

统计记录用来统计记录集的总数。可以先声明一个CRecordset对象m_pSet。再绑定一个变量m_lCount，用来统计记录总数。执行如下语句：

m_pSet->Open(“Select Count(*) from 表名 where 限定条件”);

RecordCount=m_pSet->m_lCount;

m_pSet->Close();
RecordCount即为要统计的记录数。

或如下：

CRecordset m_Set(&db); //db 为CDatabase对象

CString strValue;

m_Set.Open(Select count(*) from 表名 where 限定条件”);

m_pSet.GetFieldValue((int)0,strValue);

long count=atol(strValue);

m_set.Close();

count为记录总数。

