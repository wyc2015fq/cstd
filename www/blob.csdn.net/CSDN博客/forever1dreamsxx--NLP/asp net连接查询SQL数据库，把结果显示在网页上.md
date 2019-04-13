
# asp.net连接查询SQL数据库，把结果显示在网页上 - forever1dreamsxx--NLP - CSDN博客


2013年04月25日 16:19:12[forever1dreamsxx](https://me.csdn.net/forever1dreamsxx)阅读数：1648


在ASP.NET中，使用C\#连接SQL数据库，并使用SQL语句查询，以前从来没有接触过C\#，最近用到了，摸索了两天终于运行起来了，Mark一下，不喜勿喷
有两种方法：（说的是第一种方法不安全，我也不清楚^_^)
第一种方法：
```python
//建立ASP.NET Web 应用程序，直接在Page_load函数中加入一下代码，貌似就可以用了
public void Page_Load(object sender, EventArgs e)
{
	using (SqlConnection con = new SqlConnection("Data Source=.;uid=sa;pwd=sa;Database=NorthWind"))
	{
		string username = "forever";
		string strSQL = "select * from table where name='" + username + "'";
		SqlDataAdapter adapter = new SqlDataAdapter(strSQL, con);
		DataSet ds = new DataSet();
		adapter.Fill(ds);
		foreach (DataRowView drv in ds.Tables[0].DefaultView)
		{
			Response.Write(drv["第一个字段"]+"|"+drv["第二个字段"]);
		}
	}
}
```
第二种方法说的比较安全，就是比较麻烦
```python
//1、修改Web.config配置文件
<configuration>
  <connectionStrings>
  </connectionStrings>
  //下面三行是添加的内容，即连接数据库的信息
  <appSettings>
    <add key="connect" value="server=.;database=NorthWind;uid=sa;pwd=sa;"/>
  </appSettings>
  
  <system.web>
//2、连接数据库
	sCon = ConfigurationManager.AppSettings["connect"];
	if (string.IsNullOrEmpty(sCon))
	{
		Response.Write("连接字符串为空！");
	}
	con = new SqlConnection(sCon);
//3、打开数据库
	if (con.State == ConnectionState.Closed)
	con.Open();
//4、查询函数
public SqlDataReader ExcuteDataReader(string strTxt, CommandType cmdType, SqlParameter[] Params)
{
	SqlDataReader dr = null;
	if (con.State == ConnectionState.Closed)
	{
		Response.Write("数据库的连接没有打开！");
		return dr;
	}
	SqlCommand cmd = new SqlCommand();
	cmd.Connection = con;
	cmd.CommandText = strTxt;
	cmd.CommandType = cmdType;
	if (Params != null)
	{
		foreach (SqlParameter param in Params)
		{
			if (param != null) cmd.Parameters.Add(param);
		}
	}
#if NOTALLOWEXCEPTION
	try
#endif
	{
		if (cmd.ExecuteScalar() != null)
		{
			dr = cmd.ExecuteReader();
                }
	}
#if NOTALLOWEXCEPTION
	catch(SqlException se)
	{
		_objToShowErr = se;
                _sError = se.Message;
                return null;
	}
	finally
#endif
	{
		cmd.Dispose();
	}
	return dr;
}
//5、执行查询	
	//SQL语句，id=N'id',加个N是为了能识别中文字符。
	string s = "select * from table where id=N'" + id + "'";
	SqlParameter[] Params1 = null;
	//保存结果
	SqlDataReader select_result = null;
	select_result = a.ExcuteDataReader(s, CommandType.Text, Params1);
	string ss = "";
	while (select_result.Read())
	{
		//根据自己的字段数写
		ss = ss + "第一个字段：" + select_result[0] + ", 第二个字段：" + select_result[1] + "; ";        
	}
	//测试输出
	Response.Write(ss);
```



