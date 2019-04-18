# VC++ ADO调用存储过程方法 - 深之JohnChen的专栏 - CSDN博客

2017年09月03日 13:03:37[byxdaz](https://me.csdn.net/byxdaz)阅读数：1047


一、调用基本过程

关于ADO调用存储过程一般是按照以下步骤进行：

1、生成并初始化一个_CommandPtr对象；指定CommandType为存储过程。

2、生成调用存储过程需要的参数，这些参数都是_ParameterPtr对象；

3、按照顺序将使用_CommandPtr的Append方法为存储过程提供参数（包括输入参数和输出参数）；

4、为_CommandPtr对象指定需要使用的ADO连接；

5、使用_CommandPtr的Execute方法调用存储过程；

6、从结果中获取返回参数的值（如果有的话）。

二、存储过程的输入/输出参数

1、创建 _ParameterPtr对象并初始化

        _ParameterPtr pParam;

        pParam.CreateInstance(__uuidof(Parameter));

2、设置_ParameterPtr对象的变量相关参数

     pParam = pCommand->CreateParameter(Name, Type, Direction, Size, Value);

其中Name是参数的名称，可以指定也可以不指定；

Type是一个DataTypeEnum值，指定参数的类别，取值有adInteger（整型）、adChar（字符/字符串型）等；

Direction是一个ParameterDirectionEnum值，其取值为adParamInput、adParamInputOutput、

adParamOutput、adParamReturnValue、adParamUnknown；

Size是一个Long类型的值，指示该参数值以字节计算的最大长度，例如对int型，该值可以取为sizeof(int)，

对Long型，该值可以取为sizeof(long)，对字符串型，可以使用该字符串的长度；

Value是一个variant类型的值，是该参数的取值。

在这里需要注意的是，Type参数、Direction参数以及Size参数一定要和存储过程定义时的参数相吻合，

例如，如果有下面一个存储过程

CREATE PROCEDURE SMS_Proc_Handle_All

(@UserID Integer,

@SourAddr Varchar(15),

@DestAddr varchar(5000),

@AvValue Single output,

@ReturnInfo varchar(100) output

)

则Type参数的取值依次为adInteger、adChar、adChar、adSingle，adChar；

Direction参数的取值依次为adParameterIn、adParameterIn、adParameterIn、adParameterOut、adParameterOut；

对于输入参数，Size的值可以根据实际数值来定，对于输出参数，最好是根据定义确定（上例中ReturnInfo参数的Size值可以取为100）。

3、加入到Command对象的参数集属性中

pCommand->Parameters->Append(< 参数对象名>);

例如：

pCommand->Parameters->Append(pParam);

4、获取Output的参数

获取ourput参数是大家最关注的问题，同时也是最“难”的问题，因为按照书本上的写法，经常获得不了

Output参数，其实这个问题很容易解决：在调用_CommandPtr的Execute方法时，写成

cmmd->Execute(NULL, NULL, adCmdStoredProc);

这句执行完后，使用

cmmd->Parameters->GetItem("XXXXXX")->GetValue();

输出参数的名称，就可以获得输出参数的值了。

三、调用实例

存储过程，如下:

CREATE PROCEDURE sp_1 (

                         @pin1 int ,

                         @pin2 CHAR(10),

                         @pout1 int OUTPUT,

                         @pout2 CHAR(10) OUTPUT

                       )

AS                       

BEGIN

declare @retval int

select @pout1 = @pin1 + 100

select @pout2 = left( ltrim(rtrim(@pin2)) + \'123\' , 10)

select Num,Name,Date 

    from TABLE1    

select @retval = 1236

return @retval

END

c++ 调用代码

```cpp
_CommandPtr     pCmd = NULL;
_RecordsetPtr   pRecordset = NULL;
_ParameterPtr   retParam = NULL;       
_ParameterPtr   inParam1 = NULL; 
_ParameterPtr   inParam2 = NULL; 
_ParameterPtr   outParam1 = NULL; 
_ParameterPtr   outParam2 = NULL;
    初试化
pCmd.CreateInstance(__uuidof(Command));
pRecordset.CreateInstance(_uuidof(Recordset));
retParam.CreateInstance(__uuidof(Parameter));
//其他的ParameterPtr   也初试化
pCmd->ActiveConnection = pConn;
pCmd->CommandType = adCmdStoredProc;   
pCmd->CommandText=_bstr_t("sp_1"); //存储过程名称
retParam = pCmd ->CreateParameter(_bstr_t("Return"),adInteger,adParamReturnValue,sizeof(int));
pCmd->Parameters->Append(retParam); //这个add的顺序要和参数的index顺序要一致
inParam1 = pCmd ->CreateParameter(_bstr_t("InParam1"), adInteger, adParamInput,  sizeof(int));
inParam1->Value = _variant_t(3);
pCmd->Parameters->Append(inParam1);
inParam2 = pCmd ->CreateParameter(_bstr_t("InParam2"),adChar, adParamInput, 10);
inParam2->Value = _variant_t("DD1");
pCmd->Parameters->Append(inParam2);
outParam1 = pCmd ->CreateParameter(_bstr_t("OutParam1"), adInteger,adParamOutput,sizeof(int));
pCmd->Parameters->Append(outParam1);
outParam2 = pCmd ->CreateParameter(_bstr_t("OutParam2"), adChar, adParamOutput,10);
pCmd->Parameters->Append(outParam2);
pRecordset = pCmd->Execute(NULL,NULL,adCmdStoredProc);
//可以用下面两种办法来取这些返回值和output参数
VretVal = pCmd->Parameters->GetItem(_bstr_t("Return"))->Value;//通过参数名称
VretVal = pCmd->Parameters->GetItem(short(0))->Value;//通过参数序号
```

四、注意事项： 

1、关于Refresh这个函数，作用是Command对象去重新索取要操作的存储过程所有有关参数的信息，并且清空在refresh之前获取的参数信息。如果是通过pCmd->Parameters来添加参数（存在多个存储过程使用这个pCmd->Parameters变量），调用没个存储过程之前调用Refresh函数清空已有的参数。

2、关于pConn->CursorLocation =adUseClient;

CursorLocation 属性，设置或返回游标引擎的位置。

adUseNone 没有使用游标服务。（该常量已过时并且只为了向后兼容才出现）。 

adUseClient 使用由本地游标库提供的客户端游标。本地游标引擎通常允许使用的许多功能可能是驱动程序提供的游标无法使用的，因此使用该设置对于那些将要启用的功能是有好处的。adUseClientBatch 与 adUseClient 同义，也支持向后兼容性。 

adUseServer 默认值。使用数据提供者或驱动程序提供的游标。这些游标有时非常灵活，对于其他用户对数据源所作的更改具有额外的敏感性。但是，Microsoft Client Cursor Provider（如已断开关联的记录集）的某些功能无法由服务器端游标模拟，通过该设置将无法使用这些功能。 

说明

该属性允许在可用于提供者的各种游标库中进行选择。通常，可以选择使用客户端游标库或位于服务器上的某个游标库。

该属性设置仅对属性已经设置后才建立的连接有影响。更改 CursorLocation 属性不会影响现有的连接。

对于 Connection 或关闭的 Recordset 该属性为读/写，而对打开的 Recordset 该属性为只读。

Connection.Execute 游标将继承该设置。Recordset 将自动从与之关联的连接中继承该设置。

Remote Data Service用法，当用于客户端 (ADOR) Recordset 或 Connection 对象时，只能将 CursorLocation 属性设置为 adUseClient。

3、在调用_CommandPtr的Execute方法时，写成

cmmd->Execute(NULL, NULL, adCmdStoredProc);而不要写成 RecordsetPtr rec = cmmd->Execute(NULL, NULL, adCmdStoredProc);

如果存储过程是要取返回的记录集合的话，可以使用RecordsetPtr rec = cmmd->Execute(NULL, NULL, adCmdStoredProc|adCmdUnspecified);  

4、_CommandPtr中CommandTimeout，表执行超时时间，不设置时默认为30秒。

   _ConnectionPtr中ConnectionTimeout和CommandTimeout分别表示连接超时时间、执行超时时间。

参考资料

Microsoft ADO 程序员参考 http://www.yesky.com/imagesnew/software/ado/index.html

VC ado调用oracle执行存储过程获取记录集 http://blog.csdn.net/imlmy/article/details/8894442

