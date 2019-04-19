# 机房重构——Pramaters_变量名 '@UserID' 已声明。变量名在查询批次或存储过程内部必须唯一 - Big Smile - CSDN博客
2016年01月29日 17:34:59[王啸tr1912](https://me.csdn.net/tr1912)阅读数：2621
所属专栏：[机房重构系列](https://blog.csdn.net/column/details/14105.html)
**    背景：**这两天一直在弄登陆和退出的工作记录的写入和修改，然而就在这两天出了这么一个错“变量名 '@UserID' 已声明。变量名在查询批次或存储过程内部必须唯一”，这是在下机的时候，写下机记录的时候出现的错误，然后系统一直在报这个错误，只要登陆成功一次就报，异常退出之后再登陆就没有事，这一切都与数据操作中的Pramaters有关。
# 一、Pramaters介绍
        他的全名就叫做SqlCommand.Parameters，在微软的官方解释中：The Microsoft .NET Framework Data Provider for SQL Server does not support the question mark (?) placeholder for passing parameters to a SQL Statement
 or a stored procedure called by a command of CommandType.Text. In this case, named parameters must be used. For example:
    SELECT * FROM Customers WHERE CustomerID = @CustomerID
        这就表示了这个Pramaters是和CommandType.Text是有关系的，那么什么又是CommandType.Text呢？其实这个问题在sqlhelper中有明确的提示，就是这一句：
`cmdType = CommandType.StoredProcedure '定义命令方式为存储过程 `
        这句活就是设置查询方式是用SQL语句查询还是用存储过程来查询，CommandType.Text就是定义用SQL语句来查询，这里面就用到了Parameters，这个Prameters就是一个placeholder
 for passing parameters，什么意思呢，就是占位符传递参数，说白了就是一个传递数据的参数集，里面包含了数据，@CustomerID就是加了数据的参数，数据暂时存储在了Parameters里面。
# 二、为什么要使用Parameters呢？
        其实在vb6.0的机房里面我们就有过接触，就是sql注入这个问题，只要不对输入的字符进行规定，就会导致注入错误，而在ADO.NET中引入了Parameters这个占位符传递参数，就是为了避免这类的问题，先把数据给Parameters，然后再用sql语句去调用，就有了一个转化的作用：
```
Dim paras As SqlParameter() = {New SqlParameter("@UserID", WorkLog.UserID), New SqlParameter("@Computer",WorkLog.Computer)}      'WorkLog为工作记录的实体
Dim cmdText As String cmdText = "select * from Work_Log_Info where UserID=@UserID and Offdate is null and Computer=@Computer" 参数化调用数据
```
其中参数就是代表了实体中的属性值的一个跳板。
# 三、为什么会出现标题的问题
        在写数据的时候是不用返回值的，所以我把SQLHelper改了一下，又定义了一个函数来处理没有返回值得情况：
```
Public Sub AlterUpdate(ByVal cmdText As String, ByVal cmdType As CommandType, ByVal paras As SqlParameter())
        cmd.CommandText = cmdText                                             '获取SQL语句的具体内容
        cmd.CommandType = cmdType
        cmd.Parameters.AddRange(paras)  ' 参数添加 
        Try
            conn.Open()
            cmd.ExecuteNonQuery()
        Catch ex As Exception
            MsgBox(ex.Message.ToString())
        Finally
            Call CloseCmd(cmd)
            CloseConnection(conn)
        End Try
End Sub
```
        由于当时的不理解，少了一句话，就是cmd.Parameters.Clear()，由于第一次使用的时候没有清空Parameters的内容，所以在第二次调用的时候，Parameters里面默认已经有了@UserID这个字符，所以才会报这个已经存在的错误，然而第二次登陆的时候，由于我的判断机制，没有用到这个函数，所以没有执行，就没有报错。所以，Parameters在使用时候是一定要清空的，这一点一定要注意。
# 四、总结
        通过了这次的调错的历程，让我对于ADO.NET的数据查阅又有了新的理解，以后再有类似的问题，再也不怕了。
