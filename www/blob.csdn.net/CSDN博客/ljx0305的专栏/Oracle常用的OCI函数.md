# Oracle常用的OCI函数 - ljx0305的专栏 - CSDN博客
2009年07月30日 16:31:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：761
**Oracle常用的OCI函数**
一． Oracle oci工具包安装： 
$ORACLE_HOME/BIN:执行文件和help文件 
$ORACLE_HOME/OCI/INCLUDE:头文件 
$ORACLE_HOME/OCI/LIB/BC:  for Borlanf C++的OCI库 
$ORACLE_HOME/OCI/LIB/MSVC:  for MS Visual C++的OCI库 
如果是unix下，对于ORACLE8i，则OCI库在$ORACLE_HOME/lib下,如果是9i,则在$ORACLE_HOME/lib32下，库文件名一般为libclntsh.so 
1． 创建OCI环境即创建和初始化OCI工作环境，其他的OCI函数需要OCI环境才能执行。 
2． 需要申请的句柄类型： 
OCI环境句柄： OCI_HTYPE_ENV—它定义所有OCI函数的环境调用环境，是其他句柄的父句柄。(由OCIEnvInit或OCIEnvCreate生成) 
错误句柄：OCI_HTYPE_ERROR—作为一些OCI函数的参数，用来记录这些OCI函数操作过程中所产生的错误，当有错误发生时，可用COIErrorGet()来读取错误句柄 中记录的错误信息。 
服务器环境句柄：OCI_HTYPE_SVCCTX—定义OCI调用的服务器操作环境，它包含服务器、用户会话和事务三种句柄。 
服务器句柄：OCI_HTYPE_SERVER—标识数据源，它转换为与服务器的物理连接。 
用户会话句柄：OCI_HTYPE_SESSION—定义用户角色和权限及OCI调用的执行环境。 
事务句柄：OCI_HTYPE_TRANS—定义执行SQL操作的事务环境，事务环境中包含用户的会话状态信息。 
语句句柄：OCI_HTYPE_STMT—是一个标识SQL语句或PL/SQL块，以及其相关属性的环境。 
Bind/Define句柄：属于语句句柄的子句柄，由OCI库隐式自动生成。用户不需要自己再申请，OCI输入变量存储在bind 句柄中，输出变量存储在定义句柄中 
　3． 句柄属性包括： 
服务器环境句柄属性：(OCI_HTYPE_SVCCTX) 
OCI_ATTR_SERVER—设置/读取服务环境的服务器环境属性 
OCI_ATTR_SESSION—设置/读取服务环境的会话认证环境属性 
              OCI_ATTR_TRANS—设置/读取服务环境的事务环境属性 
用户会话句柄属性：(OCI_HTYPE_SESSION) 
OCI_ATTR_USERNAME—设置会话认证所使用的用户名 
 OCI_ATTR_PASSWORD—设置会话认证所使用的用户口令 
服务器句柄：(OCI_HTYPE_SEVER) 
OCI_ATTR_NOBLOCKING_MODE—设置/读取服务器连接：=TRUE时服务器连接设置为非阻塞方式 
语句句柄：(OCI_HTYPE_STMT) 
OCI_ATTR_ROW_COUNT—只读，为当前已处理的行数，其default=1 
OCI_ATTR_STMT_TYPE—读取当前SQL语句的类型： 
Eg : OCI_STMT_BEGIN 
      OCI_STMT_SELECT   OCI_STMT_INSERT     
OCI_STMT_UPDATE  OCI_STMT_DELETE 
                  OCI_ATTR_PARAM_COUNT—返回语句选择列表中的列数 
4． 关于输出变量定义：如果在语句执行前就知道select语句的选择列表结构，则定义输出操作可在调用 OCISTMTExecute前进行，如果查询语句的参数为用户动态输入的，则必须在执行后定义。 
5． OCI函数返回值： 
OCI_SUCCESS –函数执行成功 (=0) 
OCI_SUCCESS_WITH_INFO –执行成功，但有诊断消息返回，可能是警告信息 
OCI_NO_DATA—函数执行完成，但没有其他数据 
OCI_ERROR—函数执行错误 
OCI_INVALID_HANDLE—传递给函数的参数为无效句柄，或传回的句柄无效 
OCI_NEED_DATA—需要应用程序提供运行时刻的数据 
OCI_CONTINUE—回调函数返回代码，说明回调函数需要OCI库恢复其正常的处理操作 
OCI_STILL_EXECUTING—服务环境建立在非阻塞模式，OCI函数调用正在执行中。 
6． OCI连接有二种方式：Blocking(阻塞方式)和non_Blocking(非阻塞方式)，阻塞方式就是当调用 OCI操作时，必须等到此OCI操作完成后服务器才返回客户端相应的信息，不管是成功还是失败。非阻塞方式是当客户端提交OCI操作给服务器后，服务器立即返回OCI_STILL_EXECUTING信息，而并不等待服务端的操作完成。 
 对于non-blocking方式，应用程序若收到一个OCI函数的返回值为 OCI_STILL_EXECUTING时必须再次对每一个OCI函数的返回值进行判断，判断其成功与否。 
 可通过设置服务器属性为OCI_ATTR_NONBLOCKING_MODE来实现。系统默认方式为阻塞模式. 
7． OCI函数设置的模式有： 
 OCI_DEFUALT:使用OCI默认的环境 
 OCI_THREADED：线程环境下使用OCI 
 OCI_OBJECT：对象模式 
 OCI_SHARED：共享模式 
 OCI_EVENTS 
 OCI_NO_UCB 
 OCI_ENV_NO_MUTEX：非互斥访问模式 
 其中模式可以用逻辑运算符进行迭加，将函数设置成多多种模式：如mode=OCI_SHREADED| OCI_OBJECT 
8． 当应用进程与服务器断开连接时，程序没有使用OCITransCommit()进行事务的提交，则所有活动的事务会自动回滚。 
9． OCI重定义数据类型 
    typedef unsigned char  ub1;     
typedef   signed char  sb1; 
typedef unsigned short    ub2;   
typedef   signed short    sb2; 
typedef unsigned int  ub4;   
typedef   signed int  sb4;   
typedef         ub4      duword;     
typedef         sb4      dsword;     
typedef         dsword   dword; 
10． 在SQL语句准备后，可以用OCIAttrSet(0设置该语句的类型属性OCI_ATTR_STMT_TYPE，以后可读取语句属性，根据属性分别进行处理。 
11． 批量绑定输入和定义输出参数：将数据存入一个静态数据组中。一次执行可以提交或读取多行记录值。 
12． 结合占位符和指示器变量： 
占位符：在程序中，一些SQL语句需要在程序运行时才能确定它的语句数据，在设计时可用一个占位符来代替，当程序运行时，在它准备好语句后，必须为每个占位符指定一个变量，即将占位符与程序变量地址结合，执行时，Oracle就从这些变量中读取数据，并将它们与SQL语句一起传递给Oracle服务器执行。OCI结合占位符时，它将占位符与程序变量关联起来，并同时要指出程序变量的数据类型和数据长度。 
如：select * from test where name=:p1 and age>:p2 
：p1和：p2为占位符 
指示器变量：由于在Oracle中，列值可以为NULL，但在C语言中没有NULL值，为了能使OCI程序表达NULL列值，OCI函数允许程序为所执行语句中的结合变量同时关联一个指示符变量或指示符变量数组，以说明所结合的占位符是否为NULL或所读取的列值是否为NULL，以及所读取的列值是否被截取。 
除SQLT＿NTY（SQL Named DataType）外，指示符变量或指示符变量数组的数据类型为sb2,其值说明： 
作为输入变量时：（如insert ,update语句中） 
　=-1：OCI程序将NULL赋给Oracle表的列，忽略占位符结合的程序变量值 
>=0：应用程序将程序变量值赋给指定列 
作为输出变量时：（如select语句中） 
＝-2：所读取的列数据长度大于程序变量的长度，则被截取。 
=-1：所读取的值为NULL,输出变量的值不会被改变。 
=0：数据被完整读入到指定的程序变量中 
＞0：所读取的列数据长度大于程序变量的长度，则被截取，指示符变量值为所读取数据被截取前的实际长度 
三． OCI函数说明 
注：红色为输入参数   蓝色为输出参数  ,否则为输入/出参数 
示例以下面结构作为说明 
  sword     swResult; 
OCIBind*  hBind; 
 OCIDefine*  hDefine; 
OCIStmt      *stmtp 
OCIError     *errhp； 
OCIStmt       *stmtp 
OCISvcCtx  * svchp 
OCIEnv * envhpp; 
OCISession * usrhp; 
sb2 sb2aInd[30]; //指示器变量，用于取可能存在空值的字段 
Typedef strcut 
{ 
char tname[40]; 
int age; 
} t_std; 
typedef struct 
{ 
sb2     sb2_tname[100]; 
       sb2     sb2_age[100]; 
} stdInd_T;  //指示器数组 
typedef struct 
{ 
ub2     ub2_tname[100]; 
      ub2     ub2_age[100]; 
} stdLen_T; //字段长度 
t_std tstd[100];  //数组变量，用于批量操作 
stdInd_T tstdInd; 
stdLen_T tstdLen; 
stdLen_T tstdRet; 
t_std std; 
各函数数明 
1．创建OCI环境 
sword OCIEnvCreate(   
OCIEnv **envhpp,  //OCI环境句柄指针 
ub4 mode, //初始化模式：OCI_DEFAULT/OCI_THREADED 等 
CONST dvoid *ctxp, 
CONST dvoid *(*malicfp)(dvoid *ctxp,size_t size), 
CONST dvoid *(ralocfp)(dvoid *ctxp,dvoid *memptr,size_t newsize), 
CONST void *(*mfreefp)(dvoid *ctxp,dvoid *memptr), 
Size_t xstramemsz, 
Dvoid **usrmempp 
) 
eg : 
 swResult = OCIEnvCreate(&envhpp, OCI_DEFAULT, NULL, NULL, NULL, NULL, 0, NULL); 
if(swResult != OCI_SUCCESS && swResult != OCI_SUCCESS_WITH_INFO) 
return FALSE; 
sword OCIInitialize ( 
ub4           mode, 
                    CONST dvoid   *ctxp, 
                      CONST dvoid   *(*malocfp)   (/* dvoid *ctxp,  size_t size _*/), 
                    CONST dvoid   *(*ralocfp) (/*_ dvoid *ctxp, dvoid *memptr, size_t newsize _*/), 
                      CONST void    (*mfreefp) (/*_ dvoid *ctxp, dvoid *memptr _*/) 
);
sword OCIEnvInit ( 
OCIEnv    **envhpp, 
                  ub4       mode, 
                 size_t    xtramemsz, 
                  dvoid     **usrmempp 
); 
注： 
在8i以后，可用OCIEnvCreate一个函数就可以初始化环境了，相当于OCIInitialize+ OCIEnvInit 
2．申请/释放句柄 
  sword OCIHandleAlloc( 
CONST dvoid *parenth,  //新申请句柄的父句柄，一般为OCI环境句柄 
Dvoid **hndlpp,   //申请的新句柄 
Ub4 type, type,  //句柄类型 
Size_t xtramem_sz,   //申请的内存数 
Dvoid **usrmempp  //申请到的内存块指针 
) 
注： 
 一般需要申请的句柄有： 
              服务器句柄OCIServer, 句柄类型OCI_HTYPE_SERVER 
错误句柄OCIError，用于捕获OCI错误信息, 句柄类型OCI_HTYPE_ERROR 
事务句柄OCISession, 句柄类型OCI_HTYPE_SESSION 
上下文句柄OCISvcCtx, 句柄类型OCI_HTYPE_SVCCTX 
SQL语句句柄OCIStmt, 句柄类型OCI_HTYPE_STMT 
eg: 申请一个错误句柄OCIError 
swResult = OCIHandleAlloc(envhpp, (dvoid *)& errhp, OCI_HTYPE_ERROR, 0, NULL); 
if(swResult != OCI_SUCCESS && swResult != OCI_SUCCESS_WITH_INFO) 
{ 
return FALSE; 
} 
释放句柄 
 sword OCIHandleFree( 
dvoid *hndlp,  //要释放的句柄 
ub4 type   //句柄类型 
) 
eg: 
OCIHandleFree(stmtp, OCI_HTYPE_STMT) 
3．读取/设置句柄属性 
 sword OCIAttrSet( 
dvoid *trgthndlp,  //需设置的句柄名 
ub4  trghndltyp, //句柄类型 
dvoid *attributep, //设置的属性名 
ub4 size, //属性值长度 
ub4 attrtype,     //属性类型 
OCIError *errhp   //错误句柄 
) 
注：一般要设置的属性有： 
 服务器实例： 
句柄类型OCI_HTYPE_SVCCTX，属性类型OCI_ATTR_SERVER 
       连接数据的用户名： 
  句柄类型OCI_HTYPE_SESSION，属性类型OCI_ATTR_USERNAME 
用户密码 
    句柄类型OCI_HTYPE_SESSION，属性类型OCI_ATTR_PASSWORD 
事务：   
  句柄类型OCI_HTYPE_SVCCTX，属性类型OCI_ATTR_SESSION 
eg:设置用户名和密码 
char username[20],passwd[20]; 
 strcpy(username,”tiger”) 
strcpy(passwd,”cotton”) 
swResult = OCIAttrSet(usrhp, OCI_HTYPE_SESSION,  (text*) username, strlen(username), 
OCI_ATTR_USERNAME, errhp); 
if(swResult != OCI_SUCCESS && swResult != OCI_SUCCESS_WITH_INFO) 
return FALSE; 
swResult = OCIAttrSet(usrhp, OCI_HTYPE_SESSION,    (text*) passwd, strlen(passwd), 
OCI_ATTR_PASSWORD, errhp); 
if(swResult != OCI_SUCCESS && swResult != OCI_SUCCESS_WITH_INFO) 
return FALSE; 
sword OCIAttrGet( 
dvoid *trgthndlp,  //需读取的句柄名 
ub4  trghndltyp, //句柄类型 
dvoid *attributep, //读取的属性名 
ub4 *sizep, //属性值长度 
ub4 attrtype,     //属性类型 
OCIError *errhp   //错误句柄 
) 
4．连接/断开服务器 
  多用户方式连接： 
sword  OCIServerAttach( 
OCIServer     *srvhp,//未初始化的服务器句柄 
                    OCIError      *errhp, 
                      CONST text    *dblink,//服务器SID 
                        sb4           dblink_len, 
                       ub4           mode //=OCI_DEFAULT,系统环境将设为阻塞方式 
); 
  sword OCIServerDetach ( 
OCIServer   *srvhp, 
                OCIError    *errhp, 
                        ub4         mode //OCI_DEFAULT 
); 
单用户方式连接： 
  sword OCILogon ( 
OCIEnv          *envhp, 
                  OCIError        *errhp, 
                OCISvcCtx       **svchp, 
                CONST text      *username, 
                ub4             uname_len, 
                CONST text      *password, 
                ub4             passwd_len, 
                CONST text      *dbname, 
                ub4             dbname_len 
); 
sword OCILogoff ( 
OCISvcCtx      *svchp 
                OCIError       *errhp 
); 
5．开始/结束一个会话 
先认证用户再建立一个会话连接 
  sword OCISessionBegin ( 
OCISvcCtx     *svchp,  //服务环境句柄 
                    OCIError      *errhp, 
                OCISession    *usrhp,   //用户会话句柄 
                        ub4           credt,    //认证类型 
                        ub4           mode   //操作模式 
); 
         *认证类型： 
OCI_CRED_RDBMS:用[数据库](http://www.ninedns.com/webhosting/MSSQL.ASP)用户名和密码进行认证，则先要设置OCI_ATTR_USERNAME和OCI_ATTR_PASSWORD属性 
OCI_CRED_EXT:外部认证，不需要设置用户和密码 
OCI_DEFAULT：用户会话环境只能被指定的服务器环境句柄所设置 
OCI_SYSDBA：用户要具有sysdba权限 
OCI_SYSOPER：用户要具有sysoper权限 
Eg: 
swResult = OCISessionBegin(svchp, errh,usrhp, OCI_CRED_RDBMS, OCI_DEFAULT); 
if(swResult != OCI_SUCCESS && swResult != OCI_SUCCESS_WITH_INFO) 
return FALSE; 
sword OCISessionEnd ( 
OCISvcCtx       *svchp, 
                      OCIError        *errhp, 
                      OCISession      *usrhp, 
                    ub4             mode ); 
6．读取错误信息 
  sword OCIErrorGet ( 
dvoid      *hndlp, //错误句柄 
                    ub4        recordno,//从那里读取错误记录，从1开始 
                    text       *sqlstate,//已取消，=NULL 
                    sb4        *errcodep, //错误号 
                    text       *bufp,  //错误内容 
                    ub4        bufsiz,  //bufp长度 
                    ub4        type //传递的错误句柄类型 
=OCI_HTYPE_ERROR:错误句柄 
=OCI_HTYPE_ENV：环境句柄 
); 
eg: 
ub4    ub4RecordNo = 1; 
OCIError* hError 
sb4    sb4ErrorCode; 
char   sErrorMsg[1024]; 
if (OCIErrorGet(hError, ub4RecordNo++, NULL, &sb4ErrorCode, (OraText*) sErrorMsg, sizeof(sErrorMsg), OCI_HTYPE_ERROR) == OCI_SUCCESS) 
      printf(“error msg:%s/n”, sErrorMsg); 
7．准备SQL语句 
sword OCIStmtPrepare ( 
OCIStmt       *stmtp,//语句句柄   
                OCIError      *errhp, 
                      CONST text    *stmt,  //SQL语句 
                      ub4           stmt_len,   //语句长度 
                      ub4           language,  //语句的语法格式=OCI_NTV_SYNTAX 
ub4           mode //=OCI_DEFAULT 
); 
eg: 
char sSQL[1024]; 
sprintf(sSQL, “select table_name from user_tables”); 
swResult = OCIStmtPrepare(stmtp errhp,  (CONST OraText*)sSQL, strlen(sSQL), OCI_NTV_SYNTAX, OCI_DEFAULT); 
if(swResult != OCI_SUCCESS && swResult != OCI_SUCCESS_WITH_INFO) 
return FALSE; 
8． 绑定输入参数 
OCIBindArrayOfStruct() Set skip parameters for static array bind ，数组绑定，一般用于批量操作 
OCIBindByName() Bind by name  按名绑定 
OCIBindByPos() Bind by position  按位置绑定,建议一般按此方式绑定 
OCIBindDynamic() Sets additional attributes after bind with OCI_DATA_AT_EXEC mode   
OCIBindObject() Set additional attributes for bind of named data type 
注： 
OCIBindArrayOfStruct必须先用OCIBindByPos初始化，然后在OCIBindArrayOfStruct中定义每个参数所跳过的字节数。 
如： 
存储方式： 
第一条记录第二条记录 N 
 SkipPara(实际就是结构体长度，即本次所有列的长度和) 
sword OCIBindByName ( 
OCIStmt       *stmtp, //语句句柄 
                OCIBind       **bindpp,//结合句柄,=NULL 
                OCIError      *errhp, 
                CONST text    *placeholder,//占位符名称 
                sb4           placeh_len, //占位符长度 
                dvoid         *valuep, //绑定的变量名 
                sb4           value_sz, //绑定的变量名长度 
                ub2           dty,  //绑定的类型 
                dvoid         *indp, //指示符变量指针(sb2类型),单条绑定时为NULL, 
                ub2           *alenp, //说明执行前后被结合的数组变量中各元素数据实际的长度，单条绑定时为NULL 
                ub2           *rcodep,//列级返回码数据指针，单条绑定时为NULL 
                ub4           maxarr_len, //最多的记录数,如果是单条绑定，则为0 
                ub4           *curelep, //实际的记录数,单条绑定则为NULL 
                ub4           mode //=OCI_DEFAULT 
); 
sword OCIBindByPos ( OCIStmt      *stmtp, 
                    OCIBind      **bindpp, 
                    OCIError     *errhp, 
                    ub4          position,// 绑定的位置 
                    dvoid        *valuep, 
                    sb4          value_sz, 
                    ub2          dty, 
                    dvoid        *indp, 
                    ub2          *alenp, 
                    ub2          *rcodep, 
                    ub4          maxarr_len, 
                    ub4          *curelep, 
                    ub4          mode ); 
sword OCIBindArrayOfStruct ( 
OCIBind     *bindp,//绑定的结构句柄,由OCIBindByPos定义 
                OCIError    *errhp, 
                ub4         pvskip, //下一列跳过的字节数** 
                ub4         indskip,//下一个指示器或数组跳过的字节数 
                ub4         alskip, //下一个实际值跳过的字节数 
                ub4         rcskip //下一个列级返回值跳过的字节数 
); 
例： 
sword     swResult; 
OCIBind*  hBind; 
Ub4 rec_num; 
Sql:  insert into student values (:p1,:p2) 
单条绑定： 
hBind = NULL; 
swResult = OCIBindByPos(stmtp &hBind, errhp,1,ststd.tname, 
sizeof(ststd.tname), SQLT_CHR, NULL, 
NULL,NULL,0, NULL, OCI_DEFAULT); 
批量取数据，一次取100条 
Sql:  select username,age from student where username=:p1 and age=:p2 
hBind = NULL; 
swResult = OCIBindByPos(stmtp &hBind, errhp,1,tstd[0].tname, 
sizeof(tstd[0].tname), SQLT_CHR, &tstdInd.sb2_usernmae[0], 
&tstdLen.ub2_username[0],&tstdRet.ub2_username[0],100, &rec_num, OCI_DEFAULT); 
swResult = OCIBindArrayOfStruct(hBind, errhp,sizeof(tstd [0]), sizeof(sb2), sizeof(ub2), sizeof(ub2)); 
9．执行SQL语句 
sword OCIStmtExecute ( 
OCISvcCtx           *svchp,  //服务环境句柄 
                      OCIStmt             *stmtp,  //语句句柄 
                      OCIError            *errhp, 
                      ub4                 iters, // ** 
                      ub4                 rowoff, //** 
                      CONST OCISnapshot   *snap_in, 
                      OCISnapshot         *snap_out, 
                      ub4                 mode //** 
); 
**注： 
1. iters:对于select语句，它说明一次执行读取到buffer中的记录行数，如果不能确定select语句所返回的行数，可将iters设置为0,而对于其他的语句，iters表示这些语句的执行次数，此时iters不能为0。 
2. rowoff:在多行执行时，该参数表示从所结合的数据变量中的第几条记录开始执行(即记录偏移量)。 
3. mode：=OCI_DEFAULT:default模式 
=OCI_DESCRIBE_ONLY：描述模式，只返回选择列表的描述信息，而不执行语句 
=OCI_COMMIT_ON_SUCCESS：自动提交模式，当执行成功后，自动提交。 
=OCI_EXACT_FETCH:精确提取模式。 
=OCI_BATCH_ERRORS：批错误执行模式：用于执行数组方式的操作，在此模式下，批量insert ,update,delete时，执行过程中任何一条记录错误不会导致整个insert ,update,delete失败，系统自动会收集错误信息，而在非批错误方式下，其中的任何一条记录错误，将会导致整个操作失败。 
Eg: 
执行一次 
swResult = OCIStmtExecute(svchp, stmtp,  errhp；, 
                     1, 0, NULL, NULL, OCI_DEFAULT); 
批量执行100次： 
swResult = OCIStmtExecute(svchp, stmtp,  errhp；, 
                     100, 0, NULL, NULL, OCI_DEFAULT); 
10．定义输出变量 
OCIDefineArrayOfStruct()   Set additional attributes for static array define   
OCIDefineByPos()   Define an output variable association   
OCIDefineDynamic()   Sets additional attributes for define in OCI_DYNAMIC_FETCH mode   
OCIDefineObject()   Set additional attributes for define of named data type   
sword OCIDefineByPos ( 
OCIStmt     *stmtp, //语句句柄 
                      OCIDefine   **defnpp,//定义句柄—用于数组变量 
                      OCIError    *errhp, 
                      ub4         position,//位置序号(从1 开始) 
                      dvoid       *valuep, //输出的变量名 
                      sb4         value_sz, //变量长度 
                      ub2         dty,  //数据类型 
                      dvoid       *indp, //指示器变量/指示器变量数组，如果此字段可能存在空值，则要指示器变量，否则单条处理时为NULL 
                      ub2         *rlenp, //提取的数据长度 
                      ub2         *rcodep, //列级返回码数组指针 
ub4         mode //OCI_DEFAULT 
); 
sword OCIDefineArrayOfStruct ( 
OCIDefine   *defnp,//由OCIDefineByPos定义的句柄 
                OCIError    *errhp, 
ub4         pvskip, //下一列跳过的字节数,一般就是结构的大小 
                ub4         indskip,//下一个指示器或结构跳过的字节数,=0 
                ub4         rlskip, //下一个实际值跳过的字节数,=0 
                ub4         rcskip //下一个列列级返回值跳过的字节数,=0 
); 
sword OCIDefineDynamic ( 
OCIDefine   *defnp, 
                        OCIError    *errhp, 
                        dvoid       *octxp, 
                        OCICallbackDefine       (ocbfp)(/*_ 
                                 dvoid          *octxp, 
                                 OCIDefine      *defnp, 
                                 ub4            iter, 
                                 dvoid          **bufpp, 
                                 ub4            **alenpp, 
                                 ub1            *piecep, 
                                 dvoid          **indpp, 
                                 ub2            **rcodep _*/)  ); 
sword OCIDefineObject ( OCIDefine       *defnp, 
                       OCIError        *errhp, 
                       CONST OCIType   *type, 
                       dvoid           **pgvpp, 
                       ub4             *pvszsp, 
                       dvoid           **indpp, 
                       ub4             *indszp ); 
eg:
单条查询 
sql: select username,age from student  where username=:p1; 
如果此字段有可能有空值，则 
hDefine = NULL; 
swResult = OCIDefineByPos(stmtp &hDefine, errhp, 1, tstd.username, sizeof(tstd.username), SQLT_CHR, & sb2aInd[0], NULL, NULL, OCI_DEFAULT); 
如果此字段没有空值，则 
hDefine = NULL; 
swResult = OCIDefineByPos(stmtp &hDefine, errhp, 1, tstd.username, sizeof(tstd.username), SQLT_CHR, NULL, NULL, NULL, OCI_DEFAULT); 
 批量查询 
select username,age from student  where age>30; 
hDefine = NULL; 
swResult = OCIDefineByPos(stmtp, &hDefine, errhp, 1, &tstd[0].username, 
sizeof(tstd[0].usenmae), SQLT_CHR, NULL, NULL, NULL, OCI_DEFAULT); 
swResult = OCIDefineArrayOfStruct(hDefine, errhp, sizeof(tstd[0]), 0, 0, 0); 
11．提取结果 
sword OCIStmtFetch ( 
OCIStmt     *stmtp,//语句句柄 
                    OCIError    *errhp, 
                    ub4         nrows, //从当前位置处开始一次提取的记录数，对于数据变量，可以>1,否则不能>1 
                    ub2         orientation,//提取的方向：OCI_FETCH_NEXT 
ub4         mode //OCI_DEFAULT 
  ) 
eg 
  while ((swResult=OCIStmtFetch stmtp errhp,1,OCI_FETCH_NEXT,OCI_DEFAULT)) != OCI_NO_DATA) 
{ 
…… 
} 
12．事务操作 
开始一个事务 
sword OCITransStart ( 
OCISvcCtx    *svchp, 
                     OCIError     *errhp, 
                     uword        timeout, //** 
                     ub4          flags ); 
**注： 
1． Timeout: 
当flag=OCI_TRANS_RESUME:它表示还有多少秒事务将被激活 
=OCI_TRANS_NEW: 事务响应的超时时间(秒) 
2． Flags:指定一个新的事务还是已有事务 
=OCI_TRANS_NEW：定义一个新的事务 
=OCI_TRANS_RESUME 
准备一个事务： 
sword OCITransPrepare ( 
OCISvcCtx    *svchp, 
                       OCIError     *errhp, 
                       ub4          flags );//OCI_DEFAULT 
sword OCITransForget ( 
OCISvcCtx     *svchp, 
                      OCIError      *errhp, 
                      ub4           flags );//OCI_DEFAULT 
断开一个事务： 
sword OCITransDetach ( 
OCISvcCtx    *svchp, 
                      OCIError     *errhp, 
ub4          flags );//OCI_DEFAULT 
 提交一个事务： 
sword OCITransCommit ( 
OCISvcCtx    *svchp,  //服务环境句柄 
                             OCIError     *errhp, 
                             ub4          flags ); //OCI_DEFAULT 
回滚一个事务 
sword OCITransRollback ( 
dvoid        *svchp, 
                             OCIError     *errhp, 
                             ub4          flags ); //OCI_DEFAULT 
四． OCI数据类型与C语言数据类型对照表 
表字段类型 　　　　　OCI类型 C类型 备注 
Number(N) SQLT_UIN 　int 无符号整型 
Number(N) SQLT_INT 　int 有符号整型 
Number(n,m) SQLT_FLT 　float 符点数 
Varchar2(N) Sqlt_chr　　Char 字符串 
Raw(N) 　　Sqlt_BIN 具体看不同的定义 二进制类型，多用于一个结构字段 
DATE SQLT_DAT 无 最好转换成字符串或数字
引用:http://9host.cn/oracle/200742218395012764.html
