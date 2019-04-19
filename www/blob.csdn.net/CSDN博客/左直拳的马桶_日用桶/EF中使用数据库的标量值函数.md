# EF中使用数据库的标量值函数 - 左直拳的马桶_日用桶 - CSDN博客
2014年09月15日 20:56:23[左直拳](https://me.csdn.net/leftfist)阅读数：4864
数据库中的自定义函数，如果是表值函数，EF里面会自动加上代码，还算能寻到门路进行调用。
但如果是标量值函数呢？
从数据库里更新了模型以后，系统只在edmx里加了一句（右键，点打开方式，选XML方式可见）：
```
<Function Name="talkrecord_f_GetCount" ReturnType="int" Aggregate="false" BuiltIn="false" NiladicFunction="false" IsComposable="true" ParameterTypeSemantics="AllowImplicitConversion" Schema="dbo" />
```
这样叫人怎么用啊？可参考以下资料：
[http://msdn.microsoft.com/zh-cn/library/dd456847(v=vs.110).aspx](http://msdn.microsoft.com/zh-cn/library/dd456847(v=vs.110).aspx)
但在上面这个例子中，标量值函数放在条件过滤里。但事实上，我们写标量值函数，往往是这样使用的：
```
use [test]
go
select dbo.[talkrecord_f_GetCount]();
```
直接读取标量值函数的值，而不是用于过滤。这样子的话，仅参考上面那个例子，我怎么写也写不出来。
后来还是在老外的解答中找到了**答案**：
1、首先修改edmx里添加的这个<Function>，将IsComposable属性改为false，去掉ReturnType属性，然后添加子节点<CommandText>
```
<Function Name="talkrecord_f_GetCount" Aggregate="false" BuiltIn="false" NiladicFunction="false" IsComposable="false" ParameterTypeSemantics="AllowImplicitConversion" Schema="dbo" >
        <CommandText>
          select dbo.talkrecord_f_getcount()
        </CommandText>
      </Function>
```
怕，我的孩子，可以手动改成：
```
public virtual System.Data.Entity.Core.Objects.ObjectResult<Nullable<int>> talkrecord_f_GetCount()
        {
            return ((IObjectContextAdapter)this).ObjectContext.ExecuteFunction<Nullable<int>>("talkrecord_f_GetCount");
        }
```
2、双击打开edmx文件，右键，模型浏览器，展开****.store，存储过程/函数，选中心仪的函数，右键，导入，在弹出框中选标量，选参数值，确定。如此，系统终于肯为我们在***.context.cs里添加代码矣：
**[csharp]**[view plain](http://blog.csdn.net/leftfist/article/details/39227581#)[copy](http://blog.csdn.net/leftfist/article/details/39227581#)
- publicvirtual System.Data.Objects.ObjectResult<int?> talkrecord_f_GetCount()  
- {  
- return ((IObjectContextAdapter)this).ObjectContext.ExecuteFunction<Nullable<int>>("talkrecord_f_GetCount");  
- }  
但烦恼还没结束。编译也许会报错：
无法将类型“System.Data.Entity.Core.Objects.ObjectResult<int?>”隐式转换为“System.Data.Objects.ObjectResult<int?>”
别怕，我的孩子，可以手动改成：
**[csharp]**[view plain](http://blog.csdn.net/leftfist/article/details/39227581#)[copy](http://blog.csdn.net/leftfist/article/details/39227581#)
- publicvirtual System.Data.Objects.ObjectResult<int?> talkrecord_f_GetCount()  
- {  
- return ((IObjectContextAdapter)this).ObjectContext.ExecuteFunction<Nullable<int>>("talkrecord_f_GetCount");  
- }  
3、调用
**[csharp]**[view plain](http://blog.csdn.net/leftfist/article/details/39227581#)[copy](http://blog.csdn.net/leftfist/article/details/39227581#)
- publicstaticint GetCount()  
- {  
- int count = 0;  
- using (Model.testEntities db = new Model.testEntities())  
-     {  
-         var first = db.talkrecord_f_GetCount().First().Value;  
-         count = (int)first;  
-     }  
- return count;  
- }  
参考资料：
[http://stackoverflow.com/questions/12481868/how-to-use-scalar-valued-function-with-linq-to-entity](http://stackoverflow.com/questions/12481868/how-to-use-scalar-valued-function-with-linq-to-entity)
这些都是没有什么价值的工具使用类经验，在上面耗费时间，对技术本身并没有什么帮助，微软就是喜欢这样，搞了一大堆工具，并且还常常丢弃旧工具，然后出来更多的新工具，我们就是在这些工具里面疲于奔命，浪费青春，到头来，什么也没学到，知道的，只是一些如何使用微软工具的垃圾技能。这是我痛恨微软，抛弃微软，转向java的根本原因。
