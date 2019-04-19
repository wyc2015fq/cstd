# SqlParameter构造函数让人大吃一斤 - 左直拳的马桶_日用桶 - CSDN博客
2011年08月09日 18:44:52[左直拳](https://me.csdn.net/leftfist)阅读数：1687
publicSqlParameter(
	string parameterName,
	Object value
)
当使用以上SqlParameter的构造函数时，如果这个Object为整型0的话，会构造失败。比如说
```
SqlParameter[] sqlParas = {
	        new SqlParameter("@WebUserId", WebUserId),
	        new SqlParameter("@Status",0)};
            SQLHelper.DBHelper.ExecuteNonQuery(CommandType.StoredProcedure, "[dbo].[tp_zz_app_prd_SetStatus]", sqlParas);
```
这个sqlParas[1]居然为null，执行以后，得不到想要的效果。
帮助是这样说的
请小心使用 [SqlParameter](d6500ef0-5557-48ab-ed20-25fd04b2ca67.htm) 构造函数的这个重载来指定整数参数值。因为此重载接受 Object 类型的 value，所以当此值为零时，必须将整数值转换为 Object 类型，如下面的 C# 示例所示。
||复制代码|
|----|----|
|Parameter = new SqlParameter("@pname", Convert.ToInt32(0));| |
如果不执行该转换，则编译器将认为您尝试调用 SqlParameter（string、SqlDbType）构造函数重载。
